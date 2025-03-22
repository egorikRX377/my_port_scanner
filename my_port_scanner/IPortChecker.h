#pragma once
#include <string>



class IPortChecker
{
public:
	virtual bool check(const std::string& host, int port) = 0;
	
	virtual ~IPortChecker() = default;
};