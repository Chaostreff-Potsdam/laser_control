#!/usr/bin/env python3

__all__ = ["EtherdreamWrapper", "NoEtherdreamFound", "Scene"]
__doc__ = "Python bindings for unix etherdream lib"

import argparse
import ctypes
import itertools
import math
import os
import sys
import time

import motive
import obj
import parser
import renderer

if sys.version_info[0] == 2:
	from itertools import imap
else:
	imap = map

UINT16_MAX = 2 ** 16 - 1

import random

randomize_col = False

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

def mayrand(t):
	point = etherdream_point(*t)
	if randomize_col:
		point.r = int(random.random() * UINT16_MAX)
		point.g = int(random.random() * UINT16_MAX)
		point.b = int(random.random() * UINT16_MAX)
	return point


def toEtherDreamPointArray(points):
	"""Non-empty list of points to ctypes-Array"""
	if type(points[0]) == etherdream_point:
		return points
	if type(points[0]) == tuple and len(points[0]) == 2:
		func = etherdream_point.fromPos
	elif type(points[0]) == parser.LaserPoint:
		func = etherdream_point.fromLaserPoint
	else:
		func = mayrand
	return (etherdream_point * len(points))(*list(imap(func, points)))


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
		ps = ctypes.cast(toEtherDreamPointArray(points), etherdream_point_p)
		self.lib.etherdream_write(self.etherdream, ps, len(points), pps, reps)

def openAndDisplay(filename, canvas):
	doc = parser.ILDA(open(filename).read())
	while True:
		for chunk in doc:
			canvas.writePoints(chunk.data)
			time.sleep(1.0 / 15)

class Scene(object):

	def __init__(self, canvas):
		self.root = obj.CompositeObject()
		self.canvas = canvas

	def add(self, *objs):
		self.root.add(*objs)

	def update(self):
		self.canvas.writePoints(self.root.render())


def run(canvas):
	scene = Scene(canvas)
	fairydusts_base = [
			obj.SvgObject("assets/fairydustb.svg", 4, 0x00, 0x86, 0xf2),
			obj.SvgObject("assets/fairydusty.svg", 5, 0xff, 0x76, 0x00),
			obj.SvgObject("assets/fairydustg.svg", 7, 0x99, 0xba, 0x00)]

	fairydusts = [obj.CompositeObject(f) for f in fairydusts_base]
	print("Go")

	start_corr = -1000

	fairydusts[1].visible = False
	fairydusts[2].visible = False
	
	sc = 2.5

	[scene.add(fd) for fd in fairydusts]
	[fd.move(dy=start_corr) for fd in fairydusts]
	[fd.scale(sc, sc) for fd in fairydusts]

	acc = 5
	maxspeed = 480
	speed0 = 0
	delay = 0.04
	current = 0

	speed = speed0

	start = time.time()

	wa = 2
	whiggle = 0

	while True:
		fairydusts[current].move(dy=speed)

		if wa < 5:
			wa += 0.2

		whiggle += wa
		whiggle %= 360

		roll = math.sin(math.radians(whiggle))
		fairydusts_base[current].reset()
		fairydusts_base[current].rotate(math.radians(3) * roll)

		speed = min(maxspeed, speed + acc)
		if fairydusts[current].outofrange():
			if time.time() - start > 600:
				print("Vorbei", time.time())
				sys.exit(0)
			fairydusts[current].hide()
			fairydusts[current].reset()
			fairydusts[current].move(dy=start_corr)
			fairydusts[current].scale(sc, sc)
			speed = speed0
			wa = 0
			whiggle = 0


			current = (current + 1) % len(fairydusts)
			fairydusts[current].show()

			scene.update()
			time.sleep(0.25)

		scene.update()
		time.sleep(delay)

def run_text(canvas, lx):
	scene = Scene(canvas)
	words = []
	for w in lx:
		letters = []
		for c in w:
			print(c)
			letters.append(obj.SvgObject("assets/chars/%s.svg" % c, 10, r=0, g=255, b=0, destwidth=3000))
		for i, l in enumerate(reversed(letters)):
			l.move(dy = i * 1.1 * 3000)
		o = obj.CompositeObject(*letters)
		o.move(dy=-3000)
		o.hide()
		words.append(o)
		scene.add(o)

	print("Go")

	for x in range(3):
		for i in range(len(words)):
			words[i].show()
			scene.update()
			time.sleep(2)
			words[i].hide()




def parseArgs():
	global randomize_col
	parser = argparse.ArgumentParser(description='Laser scene and file renderer')
	parser.add_argument('filename', type=str, nargs='?', help='display file instead of scene')
	parser.add_argument('--virtual', action='store_true', help='simulation laser')
	parser.add_argument('--sparkle', action='store_true', help='sparkling')
	parser.add_argument('--text', type=str, nargs='?', help='write text')

	args = parser.parse_args()
	if args.sparkle:
		randomize_col = True
	return args

if __name__ == "__main__":
	args = parseArgs()
	canvas = renderer.Renderer(600) if args.virtual else EtherdreamWrapper()
	if args.filename is not None:
		# Just that you know: For virtual rendering use ./render.py. No time atm (Camp Day 4)
		openAndDisplay(args.filename, canvas)
	else:
		if args.text is not None:
			run_text(canvas, args.text.upper().split())
		else:
			run(canvas)

