#pragma once
#include "IPortChecker.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


class PortChecker : public IPortChecker
{
public:
	PortChecker();
	~PortChecker();


	bool check(const std::string& host, int port) override;

};