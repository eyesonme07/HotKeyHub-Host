#ifndef JSONPARSE
#define JSONPARSE

#include<string>
#include<boost/json.hpp>
#include"systemController.h"
#include<iostream>
#include<variant>

class JsonParse {
public:
	JsonParse() = default;
	//парсим json и выполняем команды
	void parse(const boost::json::value& jsonValue);
	//получаем нужную клавишу из строки
	WORD getVirtualKey(const std::string& keyName);
private:
	SystemController systemController_;

	std::vector<std::variant<WORD, wchar_t>> keys;
};


#endif JSONPARSE