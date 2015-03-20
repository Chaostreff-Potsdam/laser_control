#!/usr/bin/env python

__all__ = ["EtherdreamWrapper", "NoEtherdreamFound"]
__doc__ = "Python bindings for unix etherdream lib"

import sys
import time
import ctypes

if sys.version_info[0] == 2:
	from itertools import imap
else:
	imap = map

UINT16_MAX = 2 ** 16 - 1

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
		return cls(x, y, 0, UINT16_MAX, 0, 0, 0, 0, 0)

	@classmethod
	def fromLaserPoint(cls, point):
		"""New etherdream_point from a parser.LaserPoint (ignoring color tables)"""
		g = UINT16_MAX if point.visible else 0
		return cls(point.x, point.y, 0, g, 0, 0, 0, 0)

etherdream_p = ctypes.c_void_p
etherdream_point_p = ctypes.POINTER(etherdream_point)

class NoEtherdreamFound(Exception):
	pass

class EtherdreamWrapper(object):

	libname = "/Users/sven/Master/HCIPS13/laser_control/ilda_parser/etherdream.so"
	
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
		func = etherdream_point.fromPos if type(points[0]) == tuple else etherdream_point.fromLaserPoint
		return (etherdream_point * len(points))(*imap(func, points))

	def connect(self):
		self.lib.etherdream_lib_start()
		time.sleep(1.2)
		if self.lib.etherdream_dac_count() == 0:
			raise NoEtherdreamFound

		self.etherdream = self.lib.etherdream_get(0)
		self.lib.etherdream_connect(self.etherdream)

	def writePoints(self, points, pps=20000, reps=-1):
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

if __name__ == "__main__":
	if len(sys.argv) > 1:
		openAndDisplay(sys.argv[1])
	else:
		edw = EtherdreamWrapper()
	
