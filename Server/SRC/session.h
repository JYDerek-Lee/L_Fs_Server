#include "stdafx.h"

namespace L {
	class Session {
	public:
		Session(boost::asio::io_service& io_service)
			: m_Socket(io_service) {}

		boost::asio::ip::tcp::socket& Socket() {
			return m_Socket;
		}

		void PostReceive();


	private:
		void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);
		void handleReceive(const boost::system::error_code& error, size_t bytes_transferred);

		std::string getPath();
		std::string getList();
		std::string removeFile(std::string file_name);
		std::string removeDir(std::string dir_name);
		std::string createFile(std::string file_name);
		std::string createDir(std::string dir_name);
		std::string fileUpload(std::string file_name);
		std::string reName(std::string cur_name, std::string desc_name);

		std::string mbuffer;
		std::string cur_name;
		std::string desc_name;
		char m_ReceiveBuffer[128];
		std::string m_WriteMessage;
		boost::asio::ip::tcp::socket m_Socket;
	};
}