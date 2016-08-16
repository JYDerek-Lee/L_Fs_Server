#include "stdafx.h"
#include "session.h"

std::string L::Session::re_name(std::string cur_name, std::string desc_name) {
	path cur(cur_name); path desc(desc_name);
	if (exists(cur_name))
		rename(cur, desc);
	else
		return "No such file or directory";
	return "success!!";
}

std::string L::Session::getPath() {
	path my_path(initial_path());
	return my_path.string();
}

std::string L::Session::remove_file(std::string file_name) {
	path my_path(file_name);
	if (exists(my_path)) {
		remove_all(my_path);
	}
	else {
		return "No such file or directory";
	}
	return "success!!";
}

std::string L::Session::remove_Dir(std::string dir_name) {
	try {
		if (is_directory(dir_name))
			remove_all(dir_name);
	}
	catch (filesystem_error const& e) {
		std::cout << e.what() << std::endl;
		return "No such file or directory";
	}
	return "success!!";
}

std::string L::Session::create_file(std::string file_name) {
	std::ofstream out(file_name);
	return "success!!";
}

std::string L::Session::create_Dir(std::string dir_name) {
	path dir(dir_name);
	if (create_directory(dir)) {
		return "success!!";
	}
	return "failure!!";
}

std::string L::Session::getList() {
	std::string cur_dir_list = "\n";
	path p(initial_path());
	directory_iterator end_itr;

	for (directory_iterator itr(p); itr != end_itr; ++itr) {
		cur_dir_list += itr->path().stem().string();
		cur_dir_list += "\n";
	}
	return cur_dir_list;
}

std::string L::Session::file_upload(std::string file_name) {

	char read_buf[200] = { 0, };
	std::ofstream ost;
	ost.open(file_name);

	while (m_Socket.read_some(boost::asio::buffer(read_buf)) != 0)
		ost << read_buf << std::endl;
	ost.close();
	return "success!!";
}

void L::Session::PostReceive() {
	memset(&m_ReceiveBuffer, 0x00, sizeof(m_ReceiveBuffer));

	m_Socket.async_read_some
		(
		boost::asio::buffer(m_ReceiveBuffer),
		boost::bind(&Session::handle_receive, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)

		);
}

void L::Session::handle_write(const boost::system::error_code& error, size_t bytes_transferred)                                                               {
 m_WriteMessage = ""; }

void L::Session::handle_receive(const boost::system::error_code& error, size_t bytes_transferred) {

	if (error) {
		if (error == boost::asio::error::eof) {
			std::cout << "Client disconnect." << std::endl;
		}
		else {
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
	}
	else {
		const std::string strRecvMessage = m_ReceiveBuffer;
		std::cout << "Message from Client: " << strRecvMessage << std::endl;
		std::stringstream msgBuffer(strRecvMessage);
		msgBuffer >> mbuffer;
		if (mbuffer == "pwd") {
			m_WriteMessage = getPath();
		}
		else if (mbuffer == "ls") {
			m_WriteMessage = getList();
		}
		else if (mbuffer == "mkdir") {
			msgBuffer >> cur_name;
			m_WriteMessage = create_Dir(cur_name);
		}
		else if (mbuffer == "rmdir") {
			msgBuffer >> cur_name;
			m_WriteMessage = remove_Dir(cur_name);
		}
		else if (mbuffer == "mkfile") {
			msgBuffer >> cur_name;
			m_WriteMessage = create_file(cur_name);
		}
		else if (mbuffer == "rm") {
			msgBuffer >> cur_name;
			m_WriteMessage = remove_file(cur_name);
		}
		else if (mbuffer == "cp") {
			msgBuffer >> cur_name;
			msgBuffer >> desc_name;
		}
		else if (mbuffer == "rename") {
			msgBuffer >> cur_name;
			msgBuffer >> desc_name;
			m_WriteMessage = re_name(cur_name, desc_name);
		}
		else if (mbuffer == "upload") {
			msgBuffer >> cur_name;
			m_WriteMessage = file_upload(cur_name);
		}
		else {
			m_WriteMessage = "command not found";
		}
		boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage),
			boost::bind(&Session::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
			);
		PostReceive();
	}
}