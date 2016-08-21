#include "stdafx.h"
#include "session.h"

namespace L {
	class TCPServer : private boost::noncopyable{
	private:
		int m_nSeqNumber;
		io_service m_ioService;
		tcp::acceptor m_acceptor;
		typedef boost::shared_ptr <L::Session> ptrSeesion;

		void StartAccept();
		void acceptHandle(ptrSeesion pSession, const boost::system::error_code& error);


	public:
		TCPServer(unsigned short port)
			: m_acceptor(m_ioService, tcp::endpoint(boost::asio::ip::tcp::v4(), port), true) {
			
			ptrSeesion newConnection(new L::Session(m_ioService));

			m_acceptor.async_accept(newConnection->socket(),
				boost::bind(&TCPServer::acceptHandle, 
				    this, 
					newConnection, 
					placeholders::error));

			// 서버 함수 실행
			m_ioService.run();
		}

		~TCPServer() {
			m_ioService.stop();
		}

	}; //TCPServer
}