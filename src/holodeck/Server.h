#pragma once

#include "../Painter.h"
#include "InstructionCaller.h"

#include <boost/asio.hpp>
#include <mutex>
#include <memory>

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
		void createObject(const char *name, FuncType constructor)
		{
			InstructionCaller<num_points> caller;

			int instructionId = readInt32();
            addObjectToPainter(instructionId, name, caller(&constructor, instructionId, readTurkerIds(), readPoints(num_points)));
		}

	protected:
		typedef std::function<void(Server *)> Handler;
		static const std::vector<Handler> Handlers;

		int readChar();
		int readInt32();
		std::vector<int> readTurkerIds();
		std::vector<Point> readPoints(int n);

		void startAccept();
		void handleAccept(boost::asio::ip::tcp::socket *socket, const boost::system::error_code &error);
		void handleRead();
		void addObjectToPainter(const int id, const char *name, const ObjectPtr & object);

		Painter& m_painter;

		boost::asio::io_service m_ioService;
		std::vector<boost::asio::ip::tcp::socket*> m_connections;
		boost::asio::ip::udp::socket m_socket;
		boost::asio::ip::udp::endpoint m_localEndpoint;
		boost::asio::ip::udp::endpoint m_senderEndpoint;
		std::mutex m_connectionsMutex;
		std::mutex m_painterMutex;

		std::thread m_pollThread;

		int m_current;
		unsigned char m_buf[2048];
	};

}} // namespace laser::holodeck
