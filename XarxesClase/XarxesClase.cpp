#include <conio.h>
#include <iostream>
#include <SFML/Network.hpp>
#include "ConsoleControl.h"
#include "Chat.h"

void RunClient();
void RunServer();

unsigned short port = 3000;

int main()
{
    std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;

    char mode = ' ';

    do
    {
        mode = ConsoleControl::WaitForReadNextChar();
    } while (mode != 'C' && mode != 'c' && mode != 'S' && mode != 's');

    switch (mode) {
    case 'C':
    case 'c': {
        RunClient();
        break;

    }
    case 'S':
    case 's': {
        RunServer();
        break;
    }
    default:
        break;
    }
    while (true)
    {

    }
}

void RunClient() {
    std::cout << "Client";

    std::cout << std::endl << "Set server IP --> ";

    std::string ip;
    std::getline(std::cin, ip);

    Chat* chat = Chat::Client(ip, port);
}

void RunServer() {
    std::cout << "Server";

    Chat* chat = Chat::Server(port);
}
