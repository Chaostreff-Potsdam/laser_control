import collections

PosPoint = collections.namedtuple('PosPoint', 'x y')
LaserPoint = collections.namedtuple('LaserPoint', 'x y r g b i u1 u2')

BBox = collections.namedtuple('BBox', 'bottomleft topright')

UINT16_MAX = 2**16 - 1
col_c = lambda col: int((col / 255.) * UINT16_MAX)


