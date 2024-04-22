#include "UdpPacket.h"


UdpPacket UdpPacket::Normal(PacketKey key, ICodable& codable)
{
	UdpPacket newPacket;
	newPacket << PacketType::NORMAL << key << codable;

	return newPacket;
}

UdpPacket UdpPacket::Critical(PacketKey key, ICodable& codable)
{
	UdpPacket newPacket;
	static CriticalPacketId newId = 0;
	newPacket << PacketType::CRITICAL << key << newId << codable;

	newId++;

	if (newId >= 18446744073709551615)
	{
		newId = 0;
	}

	return newPacket;
}

UdpPacket UdpPacket::CriticalResponse(PacketKey key, CriticalPacketId criticalPacketId, ICodable& codable)
{
	UdpPacket newPacket;
	newPacket << PacketType::CRITICALRESPONSE << key << criticalPacketId << codable;

	return newPacket;
}

UdpPacket UdpPacket::Acumulated(CPVector<UdpPacket> packetsList)
{
	UdpPacket newPacket;
	newPacket << PacketType::ACUMULATED << packetsList;

	return newPacket;
}

void UdpPacket::Code(sf::Packet& packet)
{
	size_t size = this->getDataSize();
	packet << size;
	packet.append(this->getData(), size);
}

void UdpPacket::Decode(sf::Packet& packet)
{
	size_t size;
	packet >> size;
	this->append(this->getData(), size);
}
