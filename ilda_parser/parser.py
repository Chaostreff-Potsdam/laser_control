#!/usr/bin/env python2

import struct

class LaserPoint(object):
	
	def __init__(self, x, y, state, color):
		self.x = x
		self.y = y
		self.state = state
		self.color = color

	def __repr__(self):
		return "LaserPoint(%d, %d, %d, %d)" % (self.x, self.y, self.state, self.color)

class ILDA(object):
	
	TYPE_3D = 0
	TYPE_2D = 1
	TYPE_COLOR_PALETTE = 2

	headerFmt = ">4sxxxB8s8sHHHBx"
	Point2DFmt = ">hhbb"
	Point3DFmt = ">hhhbb"

	def __init__(self, data):
		self.readHeader(data[:32])
		self.data = data[32:]

	def correctString(self, s):
		return s.partition("\s")[0]

	def readHeader(self, headerData):
		self.signature, self.formatT, \
		self.name, self.company_name, self.dataCount, \
		self.frameId, self.frameCount, self.scannerHead \
				= struct.unpack(self.headerFmt, headerData)

		assert self.signature == "ILDA"
		assert self.formatT == self.TYPE_2D, "Can only parse 2D coordinates"
		self.name = self.correctString(self.name)
		self.company_name = self.correctString(self.company_name)

	def __iter__(self):
		return self.readPoints()

	def readPoints(self):
		pointDataSz = struct.calcsize(self.Point2DFmt)
		assert pointDataSz * self.dataCount <= len(self.data)
		for i in xrange(self.dataCount):
			pointData = self.data[i * pointDataSz:(i + 1) * pointDataSz]
			point = LaserPoint(*struct.unpack(self.Point2DFmt, pointData))
			yield point

	def leftData(self):
		return self.data[pointDataSz * self.dataCount:]
		
	def hasDataLeft(self):
		return bool(leftData)

def test(filename):
	for p in ILDA(open(filename).read()):
		print p

if __name__ == "__main__":
	import sys
	if len(sys.argv) > 1:
		test(sys.argv[1])
	else:
		print "Usage: %s FILE" % sys.argv[0]

