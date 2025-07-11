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
import random

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


def StepGenerator(steps_per_cycle=100, scale=1.0):
	step = 0
	while True:
		yield step * scale
		step += 1
		step %= steps_per_cycle


def SineGenerator(steps_per_cycle=100, scale=1.0, func=math.sin):
	for step in StepGenerator(steps_per_cycle, scale=1):
		angle = (step / steps_per_cycle) * 2 * math.pi
		sin_x = func(angle)
		yield sin_x * scale
		

def run(canvas):
	scene = Scene(canvas)
	scene_scale = 1.8 
	x_shift = 6000
	y_shift = -800


	waves_b = obj.SvgObject("assets/waves.svg", 4, 0x00, 0xff, 0xff, add_back_blacks=10)
	waves_b.scale(scene_scale, scene_scale)
	waves = obj.CompositeObject(waves_b)

	whale_b = obj.SvgObject("assets/whale.svg", 4, add_back_blacks=5)
	whale_b.scale(scene_scale, scene_scale)
	whale = obj.CompositeObject(whale_b)
	whale_center = whale.bwidth * 0.66, whale.bheight / 2

	
	scene.add(waves)
	scene.add(whale)
	print("Go")

	wave_speed = 1000
	whale_whiggle_max_angle = 15

	refresh_freq = 0.04

	wave_pos_x = SineGenerator(scale=wave_speed)
	wave_pos_y = SineGenerator(scale=wave_speed * 0.33, steps_per_cycle=33)

	whale_steps = 100
	whale_x_pos = StepGenerator(steps_per_cycle=whale_steps*2, scale=200)
	whale_y_pos = SineGenerator(steps_per_cycle=whale_steps, scale=100)
	whale_rot = SineGenerator(steps_per_cycle=whale_steps, scale=whale_whiggle_max_angle, func=math.cos)

	while True:
		waves.reset()
		waves.move(dx=x_shift, dy=y_shift)
		waves.move(dx=next(wave_pos_x), dy=next(wave_pos_y))

		whale.reset()
		whale.move(dx=x_shift - waves_b.bwidth*0.5, dy=y_shift)
		whale.move(dx=next(whale_x_pos), dy=next(whale_y_pos))
		whale.move(dx=-whale_center[0], dy=-whale_center[1])
		whale.rotate(math.sin(math.radians(next(whale_rot))))
		whale.move(dx=whale_center[0],  dy=whale_center[1])

		scene.update()
		time.sleep(refresh_freq)


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

