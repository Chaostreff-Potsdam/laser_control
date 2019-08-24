from svgpathtools import svg2paths
import re


def load_svg(filename):
    paths, attributes = svg2paths(filename)

    assert len(paths) == 1, "File must contain exactly one path."
    assert "style" in attributes[0].keys(), "Path must have inline style."
    assert "stroke" in attributes[0]["style"], "Path must have stroke style attribute"

    hex_color = re.search("stroke:#(([0-9]|[A-F]|[a-f]){6});?", attributes[0]["style"])[1]
    rgb_color = tuple(bytearray.fromhex(hex_color))
    return paths[0].d(), rgb_color


if __name__ == "__main__":
    file_name = "fairydustb.svg"
    from pysrc.path2polygon import path2polygonPoints, print_samples
    path, (r, g, b) = load_svg(file_name)
    # print(f"r:{r}, g:{g}, b:{b}, path: {path}")
    points = path2polygonPoints(path, 16)
    result = print_samples(points, file_name.split(".")[0])
    print(result)
