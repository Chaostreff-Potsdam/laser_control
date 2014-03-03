#ifndef LASERSERVER_H
#define LASERSERVER_H

#include "../Painter.h"

#include <boost/asio.hpp>
#include <mutex>
#include <memory>

namespace laser { namespace holodeck {

	class EXPORT_LASER_CONTROL Server
	{
	public:
		Server(Painter &painter);

		void poll();

	protected:

		enum CommandType
		{
			INVALID = 0,
			DELETE,
			PLAYER,
			WALL,
			DOOR,
			TABLE,
			BUTTON
		};

		static unsigned int parseToInt(unsigned char *array, int at);
		std::vector<Point> readPoints(int n);

		void startAccept();
		void handleAccept(boost::asio::ip::tcp::socket *socket, const boost::system::error_code &error);
		void handleRead();
		void handleDelete();
		void handleWall();
		void handleTable();
		void handlePlayer();
		void handleButton();
		void handleDoor();

		Painter& m_painter;

		boost::asio::io_service m_ioService;
		//boost::asio::ip::tcp::acceptor m_acceptor;
		std::vector<boost::asio::ip::tcp::socket*> m_connections;
		boost::asio::ip::udp::socket m_socket;
		boost::asio::ip::udp::endpoint m_localEndpoint;
		boost::asio::ip::udp::endpoint m_senderEndpoint;
		std::mutex m_connectionsMutex;
		std::mutex m_painterMutex;

		unsigned char m_buf[2048];
	};

}} // namespace laser::holodeck

#endif // LASERSERVER_H
