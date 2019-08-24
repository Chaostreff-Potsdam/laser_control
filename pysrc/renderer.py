#!/usr/bin/env python2


import parser

import time
import sys

if sys.version_info[0] == 2:
	import Tkinter as tkinter
else:
	import tkinter


def _create_circle(self, x, y, r, **kwargs):
    return self.create_oval(x-r, y-r, x+r, y+r, **kwargs)
tkinter.Canvas.create_circle = _create_circle

class VirtualChunk(parser.ILDAChunk):

	formatT = parser.TYPE_2D

	UINT16_MAX = 2 ** 16 - 1

	def _c(self, v):
		return int((v / float(self.UINT16_MAX)) * 255)
	

	def __init__(self, etherdata):
		self.data = [parser.LaserPoint2D(x, y, 0, parser.LaserColor(self._c(r), self._c(g), self._c(b)))
				for x, y, r, g, b, _, _, _  in etherdata]


class Renderer(object):

	def __init__(self, dim=512):
		self.dim = dim
		self.scale = float(dim) / 2**16

		self.master = tkinter.Tk()
		self.w = tkinter.Canvas(self.master, width=dim, height=dim)
		self.w.pack()
		self.reset()

	def _t(self, v):
		return int((v * self.scale) + self.dim * 0.5)

	def colorFor(self, point):
		if point.blank:
			return "#000000"
		elif type(point.color) == parser.LaserColor:
			return "#%02x%02x%02x" % (point.color.r, point.color.g, point.color.b)
		else:
			return "#00ff00"

	def reset(self, fill="#000000"):
		self.w.delete("all")
		self.w.create_rectangle(0, 0, self.dim, self.dim, fill=fill)

	def drawChunk(self, chunk):
		self.reset()
		if not chunk or chunk.formatT == parser.TYPE_COLOR_PALETTE:
			return False

		lastCol = self.colorFor(chunk.data[0])
		polyLine = []
		for point in chunk:
			newColor = self.colorFor(point)
			if newColor != lastCol:
				if len(polyLine) > 4:
					self.w.create_line(polyLine, fill=lastCol)
				lastCol = newColor
				polyLine = []
			polyLine.append(self._t(point.x))
			polyLine.append(self._t(-point.y))
		if len(polyLine) > 4:
			self.w.create_line(polyLine, fill=lastCol)

		self.master.update()
		tkinter.mainloop(1)
		return True

	def writePoints(self, points):
		self.drawChunk(VirtualChunk(points))

	def drawChunkCircs(self, chunk):
		self.reset("#ffffff")
		if not chunk or chunk.formatT == parser.TYPE_COLOR_PALETTE:
			return False

		for point in chunk:
			self.w.create_circle(self._t(point.x), self._t(-point.y), 4, fill=self.colorFor(point))

		self.master.update()
		tkinter.mainloop(1)
		return True

def openAndDisplay(filename, showCircs):
	r = Renderer(600)
	f = r.drawChunkCircs if showCircs else r.drawChunk
	doc = parser.ILDA(open(filename).read())
	while True:
		for chunk in doc:
			if f(chunk):
				time.sleep(1.0 / 15)

def findArg(name):
	try:
		sys.argv.remove(name)
		return True
	except ValueError:
		return False

if __name__ == "__main__":
	circs = findArg("--circs")
	if len(sys.argv) > 1:
		openAndDisplay(sys.argv[1], circs)
	else:
		print("Usage: %s FILE" % sys.argv[0])
