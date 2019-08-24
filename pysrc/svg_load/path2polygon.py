import numpy as np
from numpy.linalg import norm


def find_angle(p0, p1, c):
	p0c = norm(c - p0)  # p0->c (b)
	p1c = norm(c - p1)  # p1->c (a)
	p0p1 = norm(p1 - p0)  # p0->p1 (c)
	return np.arccos((p1c * p1c + p0c * p0c - p0p1 * p0p1) / (2 * p1c * p0c))


def is_corner(last_point, middle_point, first_point):
	return (np.pi - find_angle(last_point, first_point, middle_point)) > 0.5


def c2t(c: complex):
	return c.real, c.imag


def c2a(c: complex):
	return np.array(c2t(c))


def path2polygonPoints(path, step):
	corner_count = 0
	points = []
	path_length = path.length()
	last_point = None
	last_last_point = None
	count = 0
	last_was_corner = False

	# step through path in small steps
	for i in np.arange(0, path_length, 0.5):
		relative_position = i / path_length
		point = c2a(path.point(relative_position))  # convert complex number to tuple
		# at corners, always push multiple points
		if count > 2 and not last_was_corner and is_corner(point, last_point, last_last_point):
			corner_count += 1
			yield last_last_point
			yield last_point
			yield point
			last_was_corner = True
		else:
			last_was_corner = False
			# at non-corners push only on sample step
			if count % step == 0:
				yield point
		count += 1
		last_last_point = last_point
		last_point = point

	# always push last point
	yield c2a(path.point(1))


def print_samples(points, name="test"):
	points = list(points)
	left_most_x = np.min(points, axis=0)[0]
	right_most_x = np.max(points, axis=0)[0]

	output = f"int {name}[] = {{"
	output += ", ".join([f"Point({(point[0] * 25):.0f}, {(point[1] * 25):.0f})" for point in points])
	output += f"}};\n"
	output += f"addToPointLibrary(\"{name}\", {name}, (sizeof({name})/sizeof(*{name})), "
	output += f"{(left_most_x * 100):.0f}, {(right_most_x * 100):.0f} ); //{len(points)} points"
	return output

