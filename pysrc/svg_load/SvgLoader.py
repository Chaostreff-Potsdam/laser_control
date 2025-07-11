from svgpathtools import svg2paths
import re


def load_svg(filename):
    paths, all_attributes = svg2paths(filename)

    for path, attributes in zip(paths, all_attributes):
        assert "style" in attributes.keys(), "Path must have inline style."
        assert "stroke" in attributes["style"], "Path must have stroke style attribute"

        hex_color = re.search("stroke:#(([0-9]|[A-F]|[a-f]){6});?", attributes["style"])[1]
        rgb_color = tuple(bytearray.fromhex(hex_color))
        yield path, rgb_color


if __name__ == "__main__":
    file_name = "../fairydustb.svg_load"
    from pysrc.svg_load.path2polygon import path2polygonPoints, print_samples
    path, (r, g, b) = load_svg(file_name)
    # print(f"r:{r}, g:{g}, b:{b}, path: {path}")
    points = path2polygonPoints(path, 16)
    result = print_samples(points, file_name.split(".")[0])
    print(result)
