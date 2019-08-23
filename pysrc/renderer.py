#!/usr/bin/env python2

import Tkinter
import parser
import time
import sys

def _create_circle(self, x, y, r, **kwargs):
    return self.create_oval(x-r, y-r, x+r, y+r, **kwargs)
Tkinter.Canvas.create_circle = _create_circle

class Renderer(object):

	def __init__(self, dim=512):
		self.dim = dim
		self.scale = float(dim) / 2**16

		self.master = Tkinter.Tk()
		self.w = Tkinter.Canvas(self.master, width=dim, height=dim)
		self.w.pack()
		self.reset()

	def _t(self, v):
		return int((v * self.scale) + self.dim * 0.5)

	def colorFor(self, point):
		if point.blank:
			return "#000000"
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
		Tkinter.mainloop(1)
		return True

	def drawChunkCircs(self, chunk):
		self.reset("#ffffff")
		if not chunk or chunk.formatT == parser.TYPE_COLOR_PALETTE:
			return False

		for point in chunk:
			self.w.create_circle(self._t(point.x), self._t(-point.y), 4, fill=self.colorFor(point))

		self.master.update()
		Tkinter.mainloop(1)
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
		openAndDisplay(sys.argv[1], circs )
	else:
		print "Usage: %s FILE" % sys.argv[0]
