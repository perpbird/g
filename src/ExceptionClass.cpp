#pragma once
#include <iostream>

class ErrorException : public std::exception{
	std::string msg;
public:
	ErrorException() = default;
	ErrorException(const std::string&msg):msg(msg)
	{}
	const char* what() const noexcept override{
		return msg.c_str();
	}
};