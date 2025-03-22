#include "PortChecker.h"
#include "GlobalVariables.h"


PortChecker::PortChecker()
{
	WSADATA version;
	WSAStartup(MAKEWORD(2, 2), &version);
}

PortChecker::~PortChecker()
{
	WSACleanup();
}


bool PortChecker::check(const std::string& host, int port)
{
	if (stopScanning) return false;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET) return false;

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if (inet_pton(AF_INET, host.c_str(), &server.sin_addr) <= 0) {
        closesocket(sock);
        return false;
    }

    int timeout = 500;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

    bool isOpen = (connect(sock, (sockaddr*)&server, sizeof(server)) == 0);
    closesocket(sock);
    return isOpen;
}