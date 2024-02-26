#pragma once
#include "SocketSelector.h"
#include <list>
class SocketsManager
{
public:
	typedef std::function<void(TcpSocket* socket)> OnSocketConnected;

	SocketsManager(OnSocketConnected onSocketConnected);
	~SocketsManager();

	void StartLoop();

	bool StartListener(unsigned short port);
	bool ConnectToServer(std::string ip, unsigned short port);

private:

	bool _isRunning = false;
	std::mutex _isRunningMutex;

	SocketSelector _selector;

	TcpListener* _listener;
	std::mutex _listenerMutex;

	std::list<TcpSocket*> _sockets;
	std::mutex _socketsMutex;

	OnSocketConnected _onSocketConnected;

	void SelectorLoop();
	void CheckListener();
	void CheckSockets();

	void AddSocket(TcpSocket* socket);
	void RemoveSocket(TcpSocket* socket);
	void RemoveSocketAsync(TcpSocket* socket);

};

