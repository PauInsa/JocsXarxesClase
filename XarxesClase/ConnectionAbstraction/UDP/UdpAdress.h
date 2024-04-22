#pragma once
#include <string>


class UdpAdress {
public:
	typedef unsigned short PortNUmber;

public:
	std::string ip = "";
	PortNUmber port = 0;

	std::string Tostring();

};