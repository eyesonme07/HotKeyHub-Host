#ifndef SESSION_H
#define SESSION_H

#include<boost/asio.hpp>
#include<boost/json.hpp>
#include"systemController.h"
#include<iostream>

class Session : public std::enable_shared_from_this<Session> {
public:
	Session(boost::asio::ip::tcp::socket socket);
	void start();
	void do_read_header();
	void do_read_body(std::size_t length);
	void processJson(const boost::json::value &value);

private:
	char buffer_head_[4]{};
	std::vector<char> buffer_data_{};
	boost::asio::ip::tcp::socket socket_;
	SystemController systemController_;
};

#endif // SESSION_H
