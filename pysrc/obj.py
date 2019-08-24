import point

import math
import numpy as np

import itertools

TOP     =  2 ** 15 - 2000
VIS_BOTTOM  = -8000 # -2 ** 15

BOTTOM  = - 2 ** 15

FRAME=abs(TOP)+abs(BOTTOM)

def clamp(v):
	if v < BOTTOM:
		return v + FRAME
	if v > TOP:
		return v - FRAME
	return v


class LaserObject(object):

	def __init__(self, points):
		self._points = list(points)
		self._org = points[:]
		self._transform = np.identity(3)

		if not points:
			self._bbox = [(0,0), (0,0)]
		else:
			xs, ys = zip(*((p.x, p.y) for p in self._points))
			self._bbox = [(min(xs), min(ys)), (max(xs), max(ys))]

		self.visible = True

	def hide(self):
		self.visible = False

	def show(self):
		self.visible = True

	def transform(self, transform):
		self._transform = self._transform.dot(np.array(transform))

	def _updimAndTransform(self, pts):
		# Upscale (x, y) tuples to (x, y, 1)
		return np.array([(x, y, 1) for x, y in pts]).dot(self._transform.T)

	def _mask(self, p):
		if p.y < VIS_BOTTOM and self.visible:
			return tuple(p._replace(y=TOP, r=0, g=0, b=0))
		return tuple(p)

	@property
	def bbox(self):
		return point.BBox(*(point.PosPoint(x, y) for x, y, _ in self._updimAndTransform(self._bbox)))

	def reset(self):
		self._points = self._org
		self._transform = np.identity(3)

	def _replace(self, **kwargs):
		self._points = [p._replace(**kwargs) for p in self._points]

	def _transformedPts(self, pts, conv=int):
		transformed = self._updimAndTransform((p.x, p.y) for p in pts)

		return [self._mask(p._replace(x=conv(x), y=conv(y)))
				for (x, y, _), p in zip(transformed, pts)]

	def setColor(self, r, g, b):
		self._replace(r=point.col_c(r), g=point.col_c(g), b=point.col_c(b))

	def move(self, dx=0, dy=0):
		self.transform([[1, 0, dx],
		                [0, 1, dy],
		                [0, 0,  1]])

	def rotateAroundOrigin(self, theta):
		self.transform([[ math.cos(theta), math.sin(theta), 0],
		                [-math.sin(theta), math.cos(theta), 0],
						[               0,               0, 1]])
	
	def render(self, conv=int):
		if not self.visible or self.outofrange():
			return []

		return self._transformedPts(self._points)

	def outofrange(self):
		bbox = self.bbox
		return bbox.bottomleft.y > TOP or self.bbox.topright.y < VIS_BOTTOM


class CompositeObject(LaserObject):

	def __init__(self, *objects):
		super(CompositeObject, self).__init__([])
		self.objects = list(objects)

	def add(self, *objects):
		self.objects.extend(objects)

	@property
	def bbox(self):
		pts = itertools.chain.from_iterable((b.bottomleft, b.topright) for b in (o.bbox for o in self.objects))
		xs, ys = zip(*((p.x, p.y) for p in pts))
		base_bbox = [(min(xs), min(ys)), (max(xs), max(ys))]
		return point.BBox(*(point.PosPoint(x, y) for x, y, _ in self._updimAndTransform(base_bbox)))

	def render(self, conv=int):
		if not self.visible:
			return []
	
		pts = list(itertools.chain(*([point.LaserPoint(*p) for p in o.render(conv=float)] for o in self.objects)))
		return self._transformedPts(pts, conv=conv)


		

