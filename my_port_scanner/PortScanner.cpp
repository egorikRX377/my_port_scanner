#include "PortScanner.h"
#include "PortChecker.h"
#include <iostream>

PortScanner::PortScanner(std::string h, int start, int end, int threads)
    : host(std::move(h)), startPort(start), endPort(end), threadCount(threads),
    checker(std::make_unique<PortChecker>()) {}

void PortScanner::scanRange(int rangeStart, int rangeEnd) {
    for (int port = rangeStart; port <= rangeEnd && !stopScanning; port++) {
        bool isOpen = checker->check(host, port);
        std::lock_guard<std::mutex> lock(mtx);
        results.emplace_back(port, isOpen);

        std::cout << "\rПросканировано " << ++scannedPorts << " из " << (endPort - startPort + 1)
            << " портов... Порт " << port << (isOpen ? " Открыт" : " Закрыт") << std::flush;
    }
}

void PortScanner::scan() {
    results.clear();
    scannedPorts = 0;
    stopScanning = false;

    std::vector<std::thread> threads;
    int portsPerThread = (endPort - startPort + 1) / threadCount;

    for (int i = 0; i < threadCount; i++) {
        int rangeStart = startPort + (i * portsPerThread);
        int rangeEnd = (i == threadCount - 1) ? endPort : rangeStart + portsPerThread - 1;
        threads.emplace_back(&PortScanner::scanRange, this, rangeStart, rangeEnd);
    }

    for (auto& t : threads) {
        t.join();
    }
    std::cout << "\nСканирование завершено!" << std::endl;
}

void PortScanner::stop() {
    stopScanning = true;
}

void PortScanner::printResults() const {
    std::cout << std::endl << "Итоговые результаты:" << std::endl;
    std::cout << "Порт\tСтатус" << std::endl;
    std::cout << "----------------" << std::endl;

    for (const auto& result : results) {
        std::cout << result.port << "\t" << (result.isOpen ? "Открыт" : "Закрыт") << std::endl;
    }
}