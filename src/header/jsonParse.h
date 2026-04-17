#ifndef JSONPARSE
#define JSONPARSE

#include<string>
#include<boost/json.hpp>
#include"systemController.h"
#include<iostream>

class JsonParse {
public:
	JsonParse() = default;
	void parse(const boost::json::value& jsonValue);
private:
	SystemController systemController_;
};


#endif JSONPARSE