#include "InstructionOptions.h"
#include "../objects/CompositeObject.h"
#include "../objects/Line.h"
#include "../objects/GenObject.h"

namespace laser { namespace holodeck { namespace opts {

#define LASER_HOLODECK_OPTS_FULL (3000.0)
#define LASER_HOLODECK_OPTS_HALF (LASER_HOLODECK_OPTS_FULL / 2)
#define LASER_HOLODECK_OPTS_QURT (LASER_HOLODECK_OPTS_FULL / 4)

static void moveObjectFromNSpaceToOptsSpace(GenObjectPtr & obj)
{
	const double nSpace = 2500.0;
	obj->move(-nSpace * 0.5, -nSpace * 0.5);
	obj->flipVertically();
	obj->scale(1.5 * LASER_HOLODECK_OPTS_FULL / nSpace);
}

CompositeObjectPtr Digit::get(int digit)
{
	if (digit < 0 || digit > 9)
		return CompositeObjectPtr();
	return CompositeObjectPtr(new Digit(digit));
}

Digit::Digit(int digit) :
	CompositeObject(),
	m_digit(digit)
{
	GenObjectPtr o(new GenObject);

	switch (m_digit) {
		case 0:
			o->addPoints({Point(1255, 394), Point(1482, 453), Point(1630, 634), Point(1690, 863),
						  Point(1711, 1100), Point(1717, 1337), Point(1702, 1574), Point(1648, 1805),
						  Point(1523, 2004), Point(1311, 2102), Point(1078, 2073), Point(905, 1916),
						  Point(821, 1695), Point(789, 1460), Point(783, 1222), Point(798, 985),
						  Point(843, 753), Point(945, 540), Point(1137, 407), Point(1255, 394)});
			break;
		case 1:
			o->addPoints({Point(1442, 436), Point(1514, 410), Point(1525, 407), Point(1525, 419),
						  Point(1525, 532), Point(1525, 757), Point(1524, 982), Point(1524, 1207),
						  Point(1523, 1432), Point(1523, 1657), Point(1522, 1882), Point(1522, 2094)});
			break;
		case 2:
			o->addPoints({Point(1765, 2095), Point(1540, 2094), Point(1315, 2094), Point(1090, 2093),
						  Point(865, 2093), Point(752, 2093), Point(740, 2093), Point(741, 2087),
						  Point(802, 2024), Point(958, 1862), Point(1110, 1696), Point(1257, 1526),
						  Point(1392, 1346), Point(1507, 1153), Point(1577, 940), Point(1579, 716),
						  Point(1480, 518), Point(1284, 414), Point(1061, 422), Point(871, 538),
						  Point(705, 691), Point(597, 802)});
			break;
		case 3:
			o->addPoints({Point(891, 491), Point(1116, 415), Point(1352, 407), Point(1549, 528),
						  Point(1587, 755), Point(1463, 954), Point(1257, 1068), Point(1110, 1100),
						  Point(1103, 1100), Point(1115, 1099), Point(1178, 1093), Point(1410, 1128),
						  Point(1598, 1269), Point(1684, 1487), Point(1644, 1718), Point(1499, 1904),
						  Point(1295, 2022), Point(1065, 2083), Point(829, 2101), Point(814, 2101)});
			break;
		case 4:
			o->addPoints({Point(1792, 1555), Point(1554, 1555), Point(1317, 1556), Point(1079, 1556),
						  Point(842, 1557), Point(729, 1557), Point(717, 1557), Point(711, 1553),
						  Point(765, 1469), Point(892, 1269), Point(1021, 1069), Point(1152, 871),
						  Point(1285, 675), Point(1426, 483), Point(1489, 405), Point(1497, 396),
						  Point(1501, 398), Point(1497, 510), Point(1491, 748), Point(1485, 985),
						  Point(1480, 1223), Point(1477, 1460), Point(1475, 1698), Point(1477, 1935),
						  Point(1479, 2109)});
			break;
		case 5:
			o->addPoints({Point(1713, 408), Point(1476, 415), Point(1238, 421), Point(1001, 427),
						  Point(1013, 427), Point(1001, 427), Point(997, 436), Point(989, 661),
						  Point(981, 898), Point(972, 1136), Point(971, 1173), Point(971, 1185),
						  Point(978, 1185), Point(1136, 1112), Point(1371, 1095), Point(1574, 1210),
						  Point(1671, 1423), Point(1664, 1658), Point(1561, 1870), Point(1377, 2018),
						  Point(1150, 2085), Point(913, 2083), Point(787, 2059)});
			break;
		case 6:
			o->addPoints({Point(1598, 397), Point(1363, 428), Point(1154, 539), Point(994, 713),
						  Point(885, 923), Point(820, 1151), Point(792, 1387), Point(795, 1624),
						  Point(845, 1855), Point(989, 2039), Point(1215, 2102), Point(1447, 2062),
						  Point(1624, 1909), Point(1703, 1687), Point(1699, 1450), Point(1589, 1244),
						  Point(1380, 1139), Point(1145, 1152), Point(942, 1271), Point(819, 1472),
						  Point(798, 1564)});
			break;
		case 7:
			o->addPoints({Point(802, 395), Point(1038, 419), Point(1275, 425), Point(1513, 419),
						  Point(1687, 406), Point(1698, 406), Point(1692, 418), Point(1675, 451),
						  Point(1567, 662), Point(1457, 873), Point(1347, 1083), Point(1239, 1295),
						  Point(1137, 1509), Point(1044, 1728), Point(967, 1953), Point(931, 2105)});
			break;
		case 8:
			o->addPoints({Point(1248, 1191), Point(1469, 1111), Point(1618, 931), Point(1638, 698),
						  Point(1520, 497), Point(1302, 411), Point(1071, 452), Point(901, 612),
						  Point(859, 842), Point(950, 1058), Point(1150, 1178), Point(1385, 1212),
						  Point(1594, 1318), Point(1699, 1526), Point(1688, 1762), Point(1574, 1968),
						  Point(1368, 2079), Point(1133, 2078), Point(922, 1974), Point(809, 1770),
						  Point(803, 1534), Point(912, 1327), Point(1118, 1213), Point(1248, 1191)});
			break;
		case 9:
			// Nine becomes an U for distinguishing from 6
			o->addPoints({Point(605, 396), Point(603, 659), Point(602, 921), Point(601, 1184),
						  Point(600, 1446), Point(619, 1707), Point(732, 1941), Point(960, 2058),
						  Point(1219, 2102), Point(1480, 2081), Point(1715, 1972), Point(1845, 1752),
						  Point(1899, 1495), Point(1900, 1233), Point(1900, 970), Point(1900, 708),
						  Point(1901, 445), Point(1901, 396)});
			break;
		default:
			return;
	}
	moveObjectFromNSpaceToOptsSpace(o);
	add(o);
}

void Digit::tick()
{
	;;
}

}}}
