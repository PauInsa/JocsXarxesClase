#include "UdpAdress.h"
#include <sstream>



UdpAddress::UdpAddress(std::string ip, PortNumber port)
{
    this->ip = ip;
    this->port = port;
}

std::string UdpAddress::Tostring()
{
    std::stringstream str;
    str << this->ip << ":" << this->port;

    return str.str();
}

void UdpAddress::Code(sf::Packet& packet)
{
    packet << ip << port;
}

void UdpAddress::Decode(sf::Packet& packet)
{
    packet >> ip >> port;
}
