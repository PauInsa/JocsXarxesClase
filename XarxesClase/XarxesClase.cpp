#include <conio.h>
#include <iostream>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "ConsoleControl.h"
#include "Chat.h"
#include "ConnectionAbstraction/SocketsManager.h"
#include "WindowsHelpers/Window.h"
#include "ConnectionAbstraction/UDP/UdpSocket.h"

void RunClient();
void RunServer();
void RunWindows();

enum PackagesIds : Packet::PacketKey { Message = 0 };

unsigned short port = 3000;

class SimpleCodable: public ICodable
{
public:
    std::string text = "";

    SimpleCodable() {

    };
    SimpleCodable(std::string text) {
        this->text = text;
    };
    ~SimpleCodable() {};

    virtual void Code(sf::Packet& packet) override
    {
        packet << text;
    }

    virtual void Decode(sf::Packet& packet) override
    {
        packet >> text;
    }
};

int main()
{
    
    SimpleCodable c1 = SimpleCodable("Hola");
    SimpleCodable c2 = SimpleCodable("Adios");

    UdpPacket p1 = UdpPacket::Normal(1, c1);
    UdpPacket p2 = UdpPacket::Normal(2, c2);

    //SimpleCodable cd1;
    //UdpPacket::PacketKey cd1Key;
    //sf::Uint8 cd1type;
    //SimpleCodable cd2;
    //UdpPacket::PacketKey cd2Key;
    //sf::Uint8 cd2type;

    //p1 >> cd1type >> cd1Key >> cd1;
    //p2 >> cd2type >> cd2Key >> cd2;

    CPVector<UdpPacket> packets;
    packets.push_back(&p1);
    packets.push_back(&p2);

    UdpPacket pAcumulated = UdpPacket::Acumulated(packets);

    sf::Uint8 type;
    pAcumulated >> type;

    CPVector<UdpPacket> packetsRead;
    pAcumulated >> packetsRead;
    
    std::vector<SimpleCodable> codablesReaded;

    for (UdpPacket* packet : packetsRead)
    {
        sf::Uint8 typeP;
        *packet >> typeP;

        UdpPacket::PacketKey keyP;
        *packet >> keyP;

        SimpleCodable codable;
        *packet >> codable;

        codablesReaded.push_back(codable);
    }

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
