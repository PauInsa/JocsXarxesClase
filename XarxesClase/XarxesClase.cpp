#include <conio.h>
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "ConsoleControl.h"
#include "Chat.h"
#include "ConnectionAbstraction/SocketsManager.h"
#include "WindowsHelpers/Window.h"

void RunClient();
void RunServer();
void RunWindows();

enum PackagesIds : Packet::PacketKey { Message = 0 };

unsigned short port = 3000;

int main()
{
    std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;

    char mode = ' ';

    //RunWindows();

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
    
}

void RunServer() {
   
}

void RunWindows() {

    Window window;

    window.RunWindowsLoop();
}
