#include <conio.h>
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "ConsoleControl.h"
#include "Chat.h"
#include "ConnectionAbstraction/SocketsManager.h"

void RunClient();
void RunServer();
void RunWindows();

unsigned short port = 3000;

int main()
{
    std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;

    char mode = ' ';

    RunWindows();

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

void RunWindows() {

    sf::RenderWindow windows;

    windows.create(sf::VideoMode(800, 600), "Chat");
    windows.setFramerateLimit(60);

    sf::Text lAbel;

    sf::Font font;
    font.loadFromFile("Minecraft.ttf");

    lAbel.setFont(font);
    lAbel.setCharacterSize(100);
    lAbel.setFillColor(sf::Color::White);
    lAbel.setString("L'Abel es label");
    sf::Vector2u windowsSize = windows.getSize();
    //lAbel.setPosition(windowsSize.x * 0.5, windowsSize.y * 0.5);
    lAbel.setPosition(0.5, windowsSize.y * 0.5);

    while (windows.isOpen())
    {
        windows.clear(sf::Color(rand() % 200, rand() % 200, rand() % 200));
        
        lAbel.setFillColor(sf::Color(rand()%200, rand() % 200, rand() % 200));


        windows.draw(lAbel);
        windows.display();
    }
}
