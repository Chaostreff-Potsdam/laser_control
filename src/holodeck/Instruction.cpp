#include "Instruction.h"

#include "../objects/Polygon.h"

#include <json/value.h>

#include <vector>

laser::holodeck::InstructionPtr laser::holodeck::Instruction::construct(const std::vector<laser::ObjectPtr> &objects, int instructionId, const std::vector<int> & turkerIds)
{
	InstructionPtr group(new Instruction(instructionId, turkerIds));

	group->self = group;
	group->add(objects);
	group->add(group->turkerIdsToPolygons());

    return group;
}

laser::holodeck::InstructionPtr laser::holodeck::Instruction::construct(const ObjectPtr object, int instructionId, const std::vector<int> &turkerIds)
{
	InstructionPtr group(new Instruction(instructionId, turkerIds));

    group->self = group;
	group->add(object);
	group->add(group->turkerIdsToPolygons());

	return group;
}

laser::holodeck::InstructionPtr laser::holodeck::Instruction::construct(const Json::Value &root)
{
	std::vector<int> turkerIds;
	int id = root.get("instance", Json::Value(0)).asInt();

	Json::Value turkers = root.get("turkers", Json::Value());

	// TODO we should be able to refine this
	for (Json::Value::iterator it = turkers.begin(); it != turkers.end(); ++it)
	{
		turkerIds.push_back((*it).asInt());
	}

	return InstructionPtr(new Instruction(id, turkerIds));
}

void laser::holodeck::Instruction::putTurkerIdsAt(laser::Point p)
{
	ObjectPtr group = CompositeObject::construct(turkerIdsToPolygons());
	group->move(p);
	add(group);
}

laser::holodeck::Instruction::Instruction(int instructionId, const std::vector<int> &turkerIds)
	: laser::CompositeObject(),
	  m_instructionId(instructionId),
	  m_turkerIds(turkerIds)
{
	;;
}

std::vector<laser::ObjectPtr> laser::holodeck::Instruction::turkerIdsToPolygons()
{
	std::vector<ObjectPtr> polygons;
	for (int turkerId : m_turkerIds)
	{
		polygons.push_back(std::make_shared<Polygon>(s_numbers[turkerId]));
	}

	return polygons;
}

const laser::holodeck::PointLUT laser::holodeck::Instruction::s_numbers =
{
	/* ##################### #0 ########################### */
	{Point(5018, 358), Point(6051, 679), Point(6621, 1599),
	Point(6753, 2689), Point(6853, 3784), Point(6831, 4883),
	Point(6608, 5957), Point(6012, 6866), Point(4971, 7204),
	Point(3955, 6849), Point(3384, 5924), Point(3215, 4841),
	Point(3132, 3745), Point(3200, 2648), Point(3444, 1578),
	Point(4041, 669), Point(5018, 358)},
	/* ##################### #1 ########################### */
	{Point(3899, 1121), Point(4947, 788), Point(5996, 456),
	Point(6044, 441), Point(6091, 426), Point(6101, 462),
	Point(6099, 1412), Point(6097, 2512), Point(6094, 3612),
	Point(6092, 4712), Point(6090, 5812), Point(6088, 6912),
	Point(6087, 7174)},
	/* ##################### #2 ########################### */
	{Point(7059, 7117), Point(5959, 7115), Point(4859, 7112),
	Point(3759, 7110), Point(3009, 7108), Point(2959, 7108),
	Point(2964, 7085), Point(3130, 6899), Point(3812, 6036),
	Point(4495, 5173), Point(5178, 4311), Point(5860, 3448),
	Point(6312, 2470), Point(6252, 1386), Point(5559, 544),
	Point(4490, 378), Point(3488, 799), Point(3153, 1004)},
	/* ##################### #3 ########################### */
	{Point(3566, 726), Point(4611, 395), Point(5694, 478),
	Point(6367, 1288), Point(6089, 2320), Point(5176, 2885),
	Point(4471, 3141), Point(4424, 3158), Point(4428, 3162),
	Point(4676, 3135), Point(5748, 3321), Point(6538, 4060),
	Point(6727, 5125), Point(6271, 6109), Point(5393, 6759),
	Point(4325, 6996), Point(3256, 7167)},
	/* ##################### #4 ########################### */
	{Point(7166, 4932), Point(6066, 4934), Point(4966, 4936),
	Point(3866, 4939), Point(2916, 4941), Point(2866, 4941),
	Point(2843, 4926), Point(2870, 4884), Point(3478, 3968),
	Point(4099, 3060), Point(4721, 2152), Point(5342, 1245),
	Point(5964, 337), Point(5964, 337), Point(5992, 296),
	Point(6003, 308), Point(5991, 1308), Point(5977, 2408),
	Point(5963, 3508), Point(5949, 4608), Point(5936, 5708),
	Point(5922, 6808), Point(5918, 7147)},
	/* ##################### #5 ########################### */
	{Point(6852, 383), Point(5752, 412), Point(4652, 441),
	Point(4053, 457), Point(4003, 458), Point(3987, 494),
	Point(3973, 893), Point(3935, 1993), Point(3896, 3092),
	Point(3884, 3442), Point(3882, 3492), Point(3911, 3490),
	Point(4497, 3213), Point(5580, 3153), Point(6388, 3827),
	Point(6718, 4855), Point(6455, 5911), Point(5710, 6705),
	Point(4683, 7062), Point(3585, 7008), Point(3148, 6986)},
	/* ##################### #6 ########################### */
	{Point(6390, 339), Point(5311, 511), Point(4392, 1102),
	Point(3746, 1986), Point(3410, 3025), Point(3213, 4108),
	Point(3175, 5203), Point(3421, 6269), Point(4233, 6955),
	Point(5288, 7140), Point(6261, 6672), Point(6725, 5705),
	Point(6806, 4610), Point(6289, 3665), Point(5280, 3277),
	Point(4218, 3506), Point(3511, 4305), Point(3191, 5006)},
	/* ##################### #7 ########################### */
	{Point(3206, 375), Point(4302, 436), Point(5402, 428),
	Point(6502, 420), Point(6752, 418), Point(6790, 425),
	Point(6767, 470), Point(6422, 1136), Point(5917, 2113),
	Point(5476, 3120), Point(5043, 4132), Point(4610, 5143),
	Point(4178, 6154), Point(3745, 7165), Point(3723, 7216)},
	/* ##################### #8 ########################### */
	{Point(4991, 3539), Point(5995, 3131), Point(6466, 2185),
	Point(6389, 1126), Point(5531, 481), Point(4459, 535),
	Point(3607, 1217), Point(3458, 2285), Point(4027, 3182),
	Point(5060, 3542), Point(6107, 3848), Point(6715, 4726),
	Point(6756, 5801), Point(6204, 6733), Point(5175, 7076),
	Point(4108, 6952), Point(3409, 6167), Point(3187, 5096),
	Point(3621, 4109), Point(4601, 3655), Point(4991, 3539)},
	/* ##################### #9 ########################### */
	{Point(3565, 7175), Point(4642, 7002), Point(5577, 6435),
	Point(6257, 5577), Point(6635, 4551), Point(6741, 3456),
	Point(6836, 2361), Point(6629, 1286), Point(5842, 551),
	Point(4779, 354), Point(3790, 788), Point(3240, 1724),
	Point(3188, 2814), Point(3705, 3743), Point(4684, 4207),
	Point(5755, 4031), Point(6547, 3290), Point(6834, 2338)}
};
