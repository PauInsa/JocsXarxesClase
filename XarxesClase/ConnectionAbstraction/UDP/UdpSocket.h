#pragma once
#include "UdpConnection.h"

class UdpSocket: sf::UdpSocket, public UdpSendPacketDelegate
{
public:
	typedef std::function<void(UdpConnection* connection)> OnConnectionEnter;

public:

	UdpSocket(UdpAddress::PortNumber port, OnConnectionEnter onConnectionEnter);
	~UdpSocket();

	void ConnectTo(UdpAddress address);

	UdpAddress GetAddress();

private:

	bool _isRunning = false;
	std::mutex _isRunningMutex;

	OnConnectionEnter _onConnectionEnter;
	UdpAddress _address;

	const UdpPacket::PacketKey CONNECTIONKEY = 0;
	std::map<std::string, UdpConnection*> _connectionsMap;
	std::map<std::string, UdpConnection*> _pendantConnectionsMap;
	std::mutex _pendantConnectionsMapMutex;

	void ReceiveLoop();

	void ManageReceivedPacketDone(UdpPacket packet, UdpAddress address);
	



	// Inherited via UdpSendPacketDelegate
	void SendAcumulating(UdpAddress address, UdpPacket packet) override;

	void SendImmediately(UdpAddress address, UdpPacket packet) override;

};