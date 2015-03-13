#!/usr/bin/env python2

# Based on http://paulbourke.net/dataformats/ilda/

import struct

TYPE_3D = 0
TYPE_2D = 1
TYPE_COLOR_PALETTE = 2

class LaserColor(object):

	def __init__(self, r, g, b):
		self.r = r
		self.g = g
		self.b = b

	def __repr__(self):
		return "LaserColor(%d, %d, %d)" % (self.r, self.g, self.b)

class LaserPoint(object):
	
	def __init__(self, x, y, z, state, color):
		self.x = x
		self.y = y
		self.z = z
		self.state = state
		self.color = color

		self.blank = bool(state & 0x40)
		self.visible = not self.blank
		self.lastpoint = bool(state & 0x80)

	def __str__(self):
		return "point@(%d:%d)" % (self.x, self.y)

	def __repr__(self):
		return "LaserPoint(%d, %d, %d, %d, %d)" % (self.x, self.y, self.z, self.state, self.color)

LaserPoint2D = lambda x, y, state, color: LaserPoint(x, y, 0, state, color)

class ILDAChunk(object):

	headerFmt  = ">4sxxxB8s8sHHHBx"
	headerSz   = struct.calcsize(headerFmt)
	point2DFmt = ">hhBB"
	point3DFmt = ">hhhBB"
	colorPlFmt = ">BBB"

	def __init__(self, data):
		self.readHeader(data[:self.headerSz])
		if self.formatT == TYPE_3D:
			self.fmtStr = self.point3DFmt
			self.func = LaserPoint
		elif self.formatT == TYPE_2D:
			self.fmtStr = self.point2DFmt
			self.func = LaserPoint2D
		elif self.formatT == TYPE_COLOR_PALETTE:
			self.fmtStr = self.colorPlFmt
			self.func = LaserColor
		else:
			raise ValueError("Unknown format type %d" % self.formatT)

		self.fmtSz = struct.calcsize(self.fmtStr)
		self.data = list(self.readData(data[self.headerSz:]))
		self.rest = data[self.headerSz + self.dataCount * self.fmtSz:]

	def readHeader(self, headerData):
		self.signature, self.formatT, \
		self.name, self.company_name, self.dataCount, \
		self.frameId, self.frameCount, self.scannerHead \
				= struct.unpack(self.headerFmt, headerData)

		assert self.signature == "ILDA"
		self.name = self.correctString(self.name)
		self.company_name = self.correctString(self.company_name)

	def readData(self, dataStr):
		assert self.dataCount * self.fmtSz <= len(dataStr)
		for i in xrange(self.dataCount):
			entry = dataStr[i * self.fmtSz:(i + 1) * self.fmtSz]
			yield self.func(*struct.unpack(self.fmtStr, entry))

	def correctString(self, s):
		return s.partition("\s")[0]

	def isColorPalette(self):
		return self.formatT == TYPE_COLOR_PALETTE

	def __len__(self):
		return len(self.data)

	def __bool__(self):
		return bool(self.data)

	def __iter__(self):
		return iter(self.data)

	def __str__(self):
		if self.formatT == TYPE_3D:
			_type = "3D Points"
		elif self.formatT == TYPE_2D:
			_type = "2D Points"
		else:
			_type = "Color Palette"
		return "ILDAChunk(%s, %d entries)" % (_type, len(self.data))

class ILDA(object):

	def __init__(self, data):
		self.chunks = [ILDAChunk(data)]
		while self.chunks[-1].rest:
			self.chunks.append(ILDAChunk(self.chunks[-1].rest))
			self.chunks[-2].rest = None

	def __len__(self):
		return len(self.chunks)

	def __iter__(self):
		return iter(self.chunks)

def test(filename):
	for chunk in ILDA(open(filename).read()):
		print chunk

if __name__ == "__main__":
	import sys
	if len(sys.argv) > 1:
		test(sys.argv[1])
	else:
		print "Usage: %s FILE" % sys.argv[0]

