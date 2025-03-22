#pragma once
#include "IPortChecker.h"
#include "GlobalVariables.h"
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <memory>



struct ScanResult
{
	int port;
	bool isOpen;

	ScanResult(int port, bool isOpen) : port(port), isOpen(isOpen) {}
};


class PortScanner
{
private:
	std::string host;
	int startPort;
	int endPort;
	int threadCount;
	std::unique_ptr<IPortChecker> checker;
	std::mutex mtx;
	std::vector<ScanResult> results;
	std::atomic<int> scannedPorts{ 0 };

	void scanRange(int rangeStart, int rangeEnd);

public:
	PortScanner(std::string h, int start, int end, int threads = 12);

	void scan();
	void stop();
	void printResults() const;
};