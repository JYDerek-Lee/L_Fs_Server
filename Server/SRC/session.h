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
		void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
		void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);

		std::string getPath();
		std::string getList();
		std::string remove_file(std::string file_name);
		std::string remove_Dir(std::string dir_name);
		std::string create_file(std::string file_name);
		std::string create_Dir(std::string dir_name);
		std::string file_upload(std::string file_name);
		std::string re_name(std::string cur_name, std::string desc_name);

		boost::asio::ip::tcp::socket m_Socket;
		std::string m_WriteMessage;
		char m_ReceiveBuffer[128];
		std::string mbuffer;
		std::string cur_name;
		std::string desc_name;
	};
}