#!/usr/bin/env python2

import Tkinter
from parser import ILDA

class Renderer(object):

	def __init__(self, dim=512):
		self.dim = dim
		self.scale = float(dim) / 2**16

		self.master = Tkinter.Tk()
		self.w = Tkinter.Canvas(self.master, width=dim, height=dim)
		self.w.pack()
		self.reset()

	def colorFor(self, point):
		"""if point.blank:
			return "#000000"
		else:"""
		return "#00ff00"

	def reset(self):
		self.w.create_rectangle(0, 0, self.dim, self.dim, fill="#000000")

	def drawChunk(self, chunk):
		self.reset()
		if not chunk:
			return

		_t = lambda v: int((v * self.scale) + self.dim * 0.5)

		lastCol = self.colorFor(chunk.data[0])
		polyLine = []
		for point in chunk:
			newColor = self.colorFor(point)
			if newColor != lastCol:
				if len(polyLine) > 4:
					self.w.create_line(polyLine, fill=lastCol)
				currentCol = lastCol
				polyLine = []
			polyLine.append(_t(point.x))
			polyLine.append(_t(point.y))
		if len(polyLine) > 4:
			self.w.create_line(polyLine, fill=lastCol)

	def run(self):
		Tkinter.mainloop()

def openAndDisplay(filename):
	r = Renderer()
	for chunk in ILDA(open(filename).read()):
		r.drawChunk(chunk)
	r.run()

if __name__ == "__main__":
	import sys
	if len(sys.argv) > 1:
		openAndDisplay(sys.argv[1])
	else:
		print "Usage: %s FILE" % sys.argv[0]
