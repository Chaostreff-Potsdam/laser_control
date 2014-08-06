#include "Server.h"
#include "InstructionFactory.h"
#include "Config.h"

#include <iostream>
#include <assert.h>
#include <boost/bind.hpp>
#include <json/value.h>

namespace laser { namespace holodeck {

#define HANDLER(Name) \
	[](Server *s, Json::Value& root) { \
		s->handle##Name(root); \
	}

#define HANDLE_OBJECT(ObjectT, num_points) \
	[](Server *s, Json::Value& root) { \
		s->createObject<num_points>(#ObjectT, InstructionFactory::ObjectT, root); \
	}

#define EMPTY_LINE(ObjectT, num_points) \
	[](Server *, Json::Value& /*root*/) {}

// If you need the enum for the client side, run getHolodeckEnum.py
const std::vector<Server::Handler> Server::Handlers = {
	EMPTY_LINE(Invalid, dummy),

	HANDLER(Delete),
	HANDLER(DeleteAll),
	HANDLE_OBJECT(Player, 1),
	HANDLE_OBJECT(Wall, 2),
	HANDLE_OBJECT(Door, 2),
	HANDLE_OBJECT(Table, 4),
	HANDLE_OBJECT(Switch, 2),
	HANDLE_OBJECT(Beam, 2),
	HANDLE_OBJECT(PortalInactive, 2),
	HANDLE_OBJECT(PortalActive, 2),
	HANDLE_OBJECT(Zipline, 2),
	HANDLE_OBJECT(Corpse, 4),
	HANDLE_OBJECT(Stool, 4),
	HANDLE_OBJECT(Water, 1),
	HANDLE_OBJECT(Poke, 2),
	HANDLE_OBJECT(Stomper, 2),
	HANDLE_OBJECT(Footwear, 1),
	HANDLE_OBJECT(Heat, 1),
	HANDLE_OBJECT(Elevator, 3),
	HANDLE_OBJECT(Guardrail, 2),
	HANDLE_OBJECT(BlueprintWall, 2),
	HANDLE_OBJECT(MovingWallWarning, 2)
};

namespace basioip = boost::asio::ip;

Server::Server(Painter &painter, bool deferStart)
:	m_painter(painter),
	m_jsonreader(),
	m_socket(m_ioService),
	m_localEndpoint(basioip::udp::v4(), 30000),
	m_senderEndpoint(basioip::address::from_string(laser::config::testServer ?
													   "127.0.0.1"
													 : "192.168.1.148"),
					 30000)
{
	m_socket.open(boost::asio::ip::udp::v4());
	m_socket.bind(m_localEndpoint);
	startAccept();
	if (!deferStart)
		poll();
}

void Server::poll(bool blocking)
{
	if (!blocking)
		m_pollThread = std::thread([&](){
			m_ioService.run();
		});
}

void Server::startAccept()
{
	m_socket.async_receive_from(boost::asio::buffer(m_buf),
								m_senderEndpoint,
								boost::bind(&Server::handleRead,
											this,
											boost::asio::placeholders::error,
											boost::asio::placeholders::bytes_transferred));

}

void Server::handleRead(const boost::system::error_code &/*ec*/, std::size_t transferred_bytes)
{
	Json::Value root;
	//std::cout << std::string(m_buf, transferred_bytes) << std::endl;
	if (m_jsonreader.parse(m_buf, m_buf + transferred_bytes, root))
	{
		unsigned int instructionCode = root.get("instruction", Json::Value(0)).asUInt();
		if (0 < instructionCode && instructionCode < Handlers.size())
				Handlers[instructionCode](this, root);
	}
	else
	{
		std::cerr << m_jsonreader.getFormatedErrorMessages() << std::endl;
	}
	startAccept();
}

int Server::readInt32()
{
	int res = ((m_buf[m_current+3]<<24)|
			   (m_buf[m_current+2]<<16)|
			   (m_buf[m_current+1]<< 8)|
			   (m_buf[m_current+0]<< 0));
	m_current += sizeof(int32_t);
	return res;
}

std::vector<int> Server::readTurkerIds()
{
	std::vector<int> turkerIds;

	int numberOfTurkers = readInt32();
	for (int i = 0; i < numberOfTurkers; i++)
		turkerIds.emplace_back(readInt32());

	return turkerIds;
}

std::vector<Point> Server::readPoints(Json::Value &root, unsigned int n)
{
	std::vector<Point> ps;

	Json::Value points = root.get("points", Json::Value());

	assert(n == points.size());

	for (Json::Value::iterator it = points.begin(); points.end() != it; ++it) {
		const int x = (*it).get("x", Json::Value()).asInt();
		const int y = (*it).get("y", Json::Value()).asInt();
		ps.emplace_back(x, y);
	}
	return ps;
}

void Server::handleDelete(const Json::Value &root)
{
	int instructionId = root.get("instance", Json::Value()).asInt();

	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.deleteObject(instructionId);
}

void Server::handleDeleteAll(const Json::Value &root)
{
	std::cerr << "delete all" << std::endl;
	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.deleteAll();
}

void Server::addObjectToPainter(const int id, const char *name, const ObjectPtr &object)
{
	(void)name;
	std::lock_guard<std::mutex> lock(m_painterMutex);
	m_painter.add(id, object, false);
}

}} // namespace laser::holodeck
