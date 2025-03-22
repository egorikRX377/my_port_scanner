#include <iostream>
#include <thread>
#include <atomic>
#include <Windows.h>

#include "GlobalVariables.h"
#include "PortScanner.h"
#include "Validator.h"

std::atomic<bool> stopScanning(false);


void printUsage(const char* programName) {
    std::cout << "Использование: " << programName << " <хост> <начальный_порт> <конечный_порт> <количество_потоков>" << std::endl;
    std::cout << "Пример: " << programName << " 127.0.0.1 10 100 4" << std::endl;
    std::cout << std::endl << "Если аргументы не указаны, программа запустится в интерактивном режиме." << std::endl;
}

bool getInteractiveInput(std::string& host, int& startPort, int& endPort, int& threads) {
    std::cout << "Аргументы не указаны. Переход в интерактивный режим." << std::endl;
    std::cout << "Введите хост (IP-адрес, например, 127.0.0.1): ";
    std::cin >> host;

    if (!isValidIP(host)) {
        std::cout << "Ошибка: Неверный формат IP-адреса. Используйте формат xxx.xxx.xxx.xxx (0-255)." << std::endl;
        return false;
    }

    std::cout << "Введите начальный порт (1-65535): ";
    std::cin >> startPort;
    std::cout << "Введите конечный порт (1-65535): ";
    std::cin >> endPort;
    std::cout << "Введите количество потоков (например, 4): ";
    std::cin >> threads;

    if (std::cin.fail() || startPort < 1 || endPort > 65535 || startPort > endPort || threads < 1) {
        std::cout << "Ошибка: Неверные входные данные." << std::endl;
        std::cout << "Порты должны быть в диапазоне от 1 до 65535, начальный_порт <= конечный_порт, количество_потоков >= 1." << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::string host;
    int startPort, endPort, threads;

    if (argc == 1) {
        if (!getInteractiveInput(host, startPort, endPort, threads)) {
            return 1;
        }
    }
    else if (argc == 5) {
        host = argv[1];

        if (!isValidIP(host)) {
            std::cout << "Ошибка: Неверный формат IP-адреса. Используйте формат xxx.xxx.xxx.xxx (0-255)." << std::endl;
            printUsage(argv[0]);
            return 1;
        }

        try {
            startPort = std::stoi(argv[2]);
            endPort = std::stoi(argv[3]);
            threads = std::stoi(argv[4]);
        }
        catch (const std::exception& e) {
            std::cout << "Ошибка: Неверный формат чисел в аргументах." << std::endl;
            printUsage(argv[0]);
            return 1;
        }

        if (startPort < 1 || endPort > 65535 || startPort > endPort || threads < 1) {
            std::cout << "Ошибка: Неверные входные данные." << std::endl;
            std::cout << "Порты должны быть в диапазоне от 1 до 65535, начальный_порт <= конечный_порт, количество_потоков >= 1." << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    else {
        printUsage(argv[0]);
        return 1;
    }

    PortScanner scanner(host, startPort, endPort, threads);
    std::cout << std::endl << "Сканирование " << host << "..." << std::endl;

    std::thread scanThread([&scanner]() { scanner.scan(); });

    std::cout << "Нажмите Enter для остановки сканирования..." << std::endl;
    std::cin.get();
    scanner.stop();

    scanThread.join();
    scanner.printResults();

    return 0;
}