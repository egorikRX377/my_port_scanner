#pragma once
#include "IPortChecker.h"

class PortChecker : public IPortChecker
{
public:
	PortChecker();
	~PortChecker();


	bool check(const std::string& host, int port) override;

};