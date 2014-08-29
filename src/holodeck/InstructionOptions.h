#pragma once

#include "../laser_utilities.h"

namespace laser { namespace holodeck { namespace opts {

typedef std::vector<std::vector<Point>> PointLUT;

static const PointLUT number_points =
{
	/* ##################### #0 ########################### */
	{Point(1255, 394), Point(1492, 460), Point(1639, 657), Point(1682, 902),
	Point(1704, 1151), Point(1716, 1401), Point(1690, 1649), Point(1612, 1885),
	Point(1434, 2049), Point(1193, 2103), Point(970, 2000), Point(846, 1786),
	Point(806, 1540), Point(783, 1291), Point(792, 1041), Point(832, 795),
	Point(927, 565), Point(1127, 431), Point(1255, 394)},
	/* ##################### #1 ########################### */
	{Point(975, 581), Point(1213, 505), Point(1451, 429), Point(1511, 411),
	Point(1523, 407), Point(1525, 416), Point(1525, 578), Point(1524, 828),
	Point(1524, 1078), Point(1523, 1328), Point(1523, 1578), Point(1522, 1828),
	Point(1522, 2078), Point(1522, 2094)},
	/* ##################### #2 ########################### */
	{Point(1765, 2095), Point(1515, 2094), Point(1265, 2094), Point(1015, 2093),
	Point(765, 2093), Point(752, 2093), Point(740, 2093), Point(741, 2087),
	Point(876, 1923), Point(1031, 1727), Point(1186, 1531), Point(1341, 1335),
	Point(1496, 1138), Point(1582, 908), Point(1563, 662), Point(1412, 464),
	Point(1172, 406), Point(936, 476), Point(788, 567)},
	/* ##################### #3 ########################### */
	{Point(891, 491), Point(1128, 413), Point(1376, 413), Point(1568, 560),
	Point(1572, 803), Point(1411, 988), Point(1176, 1073), Point(1118, 1095),
	Point(1106, 1099), Point(1107, 1100), Point(1269, 1094), Point(1502, 1177),
	Point(1656, 1370), Point(1678, 1615), Point(1568, 1837), Point(1371, 1988),
	Point(1131, 2050), Point(884, 2090), Point(814, 2101)},
	/* ##################### #4 ########################### */
	{Point(1792, 1555), Point(1542, 1555), Point(1292, 1556), Point(1042, 1556),
	Point(792, 1557), Point(729, 1557), Point(717, 1557), Point(711, 1553),
	Point(799, 1417), Point(940, 1211), Point(1081, 1004), Point(1223, 798),
	Point(1364, 592), Point(1491, 406), Point(1498, 396), Point(1501, 399),
	Point(1498, 649), Point(1495, 899), Point(1491, 1149), Point(1488, 1399),
	Point(1485, 1649), Point(1482, 1899), Point(1479, 2109)},
	/* ##################### #5 ########################### */
	{Point(1713, 408), Point(1463, 415), Point(1213, 421), Point(1013, 427),
	Point(1001, 427), Point(997, 436), Point(996, 461), Point(987, 711),
	Point(978, 961), Point(971, 1173), Point(971, 1185), Point(978, 1185),
	Point(988, 1179), Point(1221, 1092), Point(1465, 1127), Point(1620, 1314),
	Point(1679, 1551), Point(1614, 1790), Point(1448, 1975), Point(1220, 2073),
	Point(971, 2068), Point(787, 2059)},
	/* ##################### #6 ########################### */
	{Point(1598, 397), Point(1351, 432), Point(1135, 554), Point(974, 744),
	Point(870, 971), Point(826, 1217), Point(790, 1463), Point(806, 1713),
	Point(892, 1945), Point(1103, 2075), Point(1347, 2094), Point(1565, 1981),
	Point(1679, 1764), Point(1701, 1515), Point(1624, 1283), Point(1418, 1148),
	Point(1171, 1145), Point(953, 1261), Point(836, 1480), Point(798, 1564)},
	/* ##################### #7 ########################### */
	{Point(802, 395), Point(1051, 410), Point(1301, 408), Point(1551, 407),
	Point(1688, 406), Point(1697, 407), Point(1692, 418), Point(1652, 496),
	Point(1537, 718), Point(1428, 943), Point(1330, 1173), Point(1231, 1403),
	Point(1133, 1633), Point(1035, 1863), Point(936, 2092), Point(931, 2105)},
	/* ##################### #8 ########################### */
	{Point(1248, 1191), Point(1479, 1104), Point(1608, 901), Point(1628, 656),
	Point(1474, 465), Point(1234, 410), Point(1014, 513), Point(867, 704),
	Point(885, 949), Point(1053, 1120), Point(1290, 1194), Point(1527, 1268),
	Point(1669, 1465), Point(1699, 1707), Point(1603, 1935), Point(1391, 2052),
	Point(1146, 2081), Point(923, 1975), Point(828, 1751), Point(809, 1506),
	Point(942, 1300), Point(1174, 1213), Point(1248, 1191)},
	/* ##################### #9 ########################### */
	{Point(891, 2102), Point(1137, 2068), Point(1356, 1949), Point(1525, 1767),
	Point(1635, 1543), Point(1673, 1297), Point(1697, 1048), Point(1700, 799),
	Point(1622, 564), Point(1417, 423), Point(1170, 400), Point(948, 506),
	Point(817, 716), Point(790, 963), Point(870, 1197), Point(1076, 1336),
	Point(1320, 1355), Point(1542, 1247), Point(1670, 1037), Point(1708, 893)}
};

static const double assumedNumberWidth = 933.0;
static const double assumedNumberHeight = 1709.0;

static const double Number0Right = 1716;
static const double Number0Bottom = 2103;

static const double IndicatorWidth = 700;
static const double IndicatorHeight = 750;
static const double IndicatorDistance = 850;

static const double SwitchHandleSize = 1500.0;

static const double ZipLineInnerCircle = 500.0;
static const double ZipLineOuterCircle = 1000.0;


}}}
