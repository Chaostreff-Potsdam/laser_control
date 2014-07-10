#pragma once

#include "../Painter.h"
#include "InstructionCaller.h"

#include <boost/asio.hpp>
#include <json/reader.h>

#include <mutex>
#include <memory>

#define LASERWRAPPER_SERVER_BUFFER_SIZE 2048

namespace laser { namespace holodeck {

	class EXPORT_LASER_CONTROL Server
	{
	public:
		Server(Painter &painter, bool deferStart = false);
		void poll(bool blocking = false);

	public:
		void handleDelete();
		void handleDeleteAll();

		template <size_t num_points, typename FuncType>
		void createObject(const char *name, FuncType constructor, Json::Value& root)
		{
			InstructionCaller<num_points> caller;

			int instructionId = root.get("id", Json::Value(0)).asInt();
			addObjectToPainter(instructionId, name, caller(&constructor, root, readPoints(num_points)));
		}

	protected:
		typedef std::function<void(Server *, Json::Value&)> Handler;
		static const std::vector<Handler> Handlers;

		int readChar();
		int readInt32();
		std::vector<int> readTurkerIds();
		std::vector<Point> readPoints(int n);

		void startAccept();
		void handleRead(const boost::system::error_code& /*ec*/, std::size_t transferred_bytes);
		void addObjectToPainter(const int id, const char *name, const ObjectPtr & object);

		Painter& m_painter;

		Json::Reader m_jsonreader;

		boost::asio::io_service m_ioService;
		std::vector<boost::asio::ip::tcp::socket*> m_connections;
		boost::asio::ip::udp::socket m_socket;
		boost::asio::ip::udp::endpoint m_localEndpoint;
		boost::asio::ip::udp::endpoint m_senderEndpoint;
		std::mutex m_connectionsMutex;
		std::mutex m_painterMutex;

		std::thread m_pollThread;

		int m_current;
		char m_buf[LASERWRAPPER_SERVER_BUFFER_SIZE];
	};

}} // namespace laser::holodeck
