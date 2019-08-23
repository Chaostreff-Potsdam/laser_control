#!/usr/bin/env python

__all__ = ["EtherdreamWrapper", "NoEtherdreamFound", "Scene"]
__doc__ = "Python bindings for unix etherdream lib"

import ctypes
import itertools
import os
import sys
import time

import obj
import motive
import parser

if sys.version_info[0] == 2:
	from itertools import imap
else:
	imap = map

UINT16_MAX = 2 ** 16 - 1

import random

class etherdream_point(ctypes.Structure):
	_fields_ = [("x", ctypes.c_int16),
	            ("y", ctypes.c_int16),
	            ("r", ctypes.c_uint16),
	            ("g", ctypes.c_uint16),
	            ("b", ctypes.c_uint16),
	            ("i", ctypes.c_uint16),
	            ("u1", ctypes.c_uint16),
	            ("u2", ctypes.c_uint16)]

	def __repr__(self):
		return "etherdream_point(%d, %d, %d, %d, %d, %d, %d, %d)" % \
			(self.x, self.y, self.r, self.g, self.b, self.i, self.u1, self.u2)

	@classmethod
	def fromPos(cls, pos):
		"""New green etherdream_point from a (x, y)-tuple"""
		x, y = pos
		return cls(x, y, int(random.random * UINT16_MAX), int(random.random * UINT16_MAX), int(random.random * UINT16_MAX), 0, 0, 0)

	@classmethod
	def fromLaserPoint(cls, point):
		"""New etherdream_point from a parser.LaserPoint (ignoring color tables)"""
		"""
		r = int(random.random() * UINT16_MAX) if point.visible else 0
		g = int(random.random() * UINT16_MAX) if point.visible else 0
		b = int(random.random() * UINT16_MAX) if point.visible else 0
		"""
		r = b = 0
		g = UINT16_MAX if point.visible else 0
		return cls(point.x, point.y, r, g, b, 0, 0, 0)

etherdream_p = ctypes.c_void_p
etherdream_point_p = ctypes.POINTER(etherdream_point)

class NoEtherdreamFound(Exception):
	pass

class EtherdreamWrapper(object):

	libname = os.path.join(os.path.dirname(__file__), "etherdream.so")
	
	def __init__(self):
		self.__initlib()
		self.etherdream = None
		self.connect()

	def __del__(self):
		pass
		#if self.etherdream:
		#	self.lib.etherdream_disconnect(self.etherdream)
	
	def __initlib(self):
		self.lib = ctypes.cdll.LoadLibrary(self.libname)

		self.lib.etherdream_get.argtypes = [ctypes.c_int]
		self.lib.etherdream_get.restype = etherdream_p

		self.lib.etherdream_connect.argtypes = [etherdream_p]
		self.lib.etherdream_disconnect.artypes = [etherdream_p]

		self.lib.etherdream_write.argtypes = [etherdream_p, etherdream_point_p, ctypes.c_int, ctypes.c_int, ctypes.c_int]

	def __toEtherDreamPointArray(self, points):
		"""Non-empty list of points to ctypes-Array"""
		if type(points[0]) == etherdream_point:
			return points
		if type(points[0]) == tuple and len(points[0]) == 2:
			func = etherdream_point.fromPos
		elif type(points[0]) == parser.LaserPoint:
			func = etherdream_point.fromLaserPoint
		else:
			func = lambda t: etherdream_point(*t)
		return (etherdream_point * len(points))(*list(imap(func, points)))

	def connect(self):
		self.lib.etherdream_lib_start()
		time.sleep(1.2)
		if self.lib.etherdream_dac_count() == 0:
			raise NoEtherdreamFound

		self.etherdream = self.lib.etherdream_get(0)
		self.lib.etherdream_connect(self.etherdream)

	def writePoints(self, points, pps=20000, reps=-1, noconv=False):
		"""Sends a list of parser.LaserPoint or (x, y)-tuples to etherdream"""
		if not points:
			return
		ps = ctypes.cast(self.__toEtherDreamPointArray(points), etherdream_point_p)
		self.lib.etherdream_write(self.etherdream, ps, len(points), pps, reps)

def openAndDisplay(filename):
	import parser
	edw = EtherdreamWrapper()
	doc = parser.ILDA(open(filename).read())
	while True:
		for chunk in doc:
			edw.writePoints(chunk.data)
			time.sleep(1.0 / 15)

class Scene(object):

	def __init__(self):
		self.objects = []
		self.edw = EtherdreamWrapper()

	def add(self, obj):
		self.objects.append(obj)

	def update(self):
		self.edw.writePoints(list(itertools.chain(*(o.render() for o in self.objects))))


def main():
	scene = Scene()
	fairydusts = [
			obj.LaserObject(motive.fairydustb),
			obj.LaserObject(motive.fairydusty),
			obj.LaserObject(motive.fairydustg)]
	

	fairydusts[1].visible = False
	fairydusts[2].visible = False
	
	[scene.add(fd) for fd in fairydusts]

	acc = 3
	maxspeed = 360
	speed0 = 0
	delay = 0.04
	current = 0

	speed = speed0

	while True:
		fairydusts[current].move(dy=speed)
		speed = min(maxspeed, speed + acc)
		if fairydusts[current].outofrange():
			fairydusts[current].visible = False
			current = (current + 1) % len(fairydusts)
			fairydusts[current].reset()
			fairydusts[current].visible = True
			scene.update()
			speed = speed0
			time.sleep(0.25)
		scene.update()
		time.sleep(delay)
	

if __name__ == "__main__":
	if len(sys.argv) > 1:
		openAndDisplay(sys.argv[1])
	else:
		#scene = Scene()
		main()

