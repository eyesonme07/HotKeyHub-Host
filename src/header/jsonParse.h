#ifndef JSONPARSE
#define JSONPARSE

#include<string>
#include<boost/json.hpp>
#include"systemController.h"
#include<iostream>

class JsonParse {
public:
	JsonParse() = default;
	//парсим json и выполняем команды
	void parse(const boost::json::value& jsonValue);
	//нажатие и отпускание клавиш
	void pressKeyFunc(const std::string& keyName, WORD vkKey);
	//получаем нужную клавишу из строки
	WORD getVirtualKey(const std::string& keyName);
private:
	SystemController systemController_;

	std::map<std::string, std::thread> activeKeys_;

	//чтобы потоки не конфликтовали при доступе к map
	std::mutex keysMutex_;
};


#endif JSONPARSE