#include "UdpAdress.h"
#include <sstream>

std::string UdpAdress::Tostring()
{
    std::stringstream str;
    str << this->ip << ":" << this->port;

    return str.str();
}
