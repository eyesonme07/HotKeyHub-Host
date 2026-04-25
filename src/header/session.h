#ifndef SESSION_H
#define SESSION_H

#include<boost/asio.hpp>
#include<boost/json.hpp>
#include"jsonParse.h"
#include<iostream>

//это сессия для одного подулючения(тут она будет получать сообщения и управлять ПК)
class Session : public std::enable_shared_from_this<Session> {
public:
	//конструктор
	Session(boost::asio::ip::tcp::socket socket);

	void start();
	//тут читаем сколько данных пришло(размер)
	void do_read_header();
	//а тут уже читаем полностью
	void do_read_body(std::size_t length);
	//отправка json обратно клиенту(статус и т.д.)
	void sendJson(const boost::json::value &value);
	//загружаем данные(текущая песня громкость и тд)
	void load_data();
	//бесконечный таймер для загрузки данных(текущая песня громкость и тд)
	void recursiveTimer(boost::asio::steady_timer& timer);
private:
	char buffer_head_[4]{};
	std::vector<char> buffer_data_{};
	boost::asio::ip::tcp::socket socket_;
	SystemController systemController_;

	boost::asio::steady_timer loadTimer_;
	
	JsonParse jsonParse_;
};

#endif // SESSION_H
