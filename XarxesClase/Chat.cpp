#include "Chat.h"
#include <thread>
#include "ConsoleControl.h"

void Chat::ShowMessage(std::string message)
{
    _messagesMutex.lock();
    _messages.push_back(message);
    _messagesMutex.unlock();

    ConsoleControl::LockMutex();
    std::cout << std::endl << message;
    ConsoleControl::UnlockMutex();
}

void Chat::ShowAlert(std::string message)
{
    ConsoleControl::LockMutex();
    ConsoleControl::SetColor(ConsoleControl::DARKCYAN);
    std::cout << std::endl << message;
    ConsoleControl::SetColor(ConsoleControl::WHITE);
    ConsoleControl::UnlockMutex();
}

void Chat::ShowError(std::string message)
{
    ConsoleControl::LockMutex();
    ConsoleControl::SetColor(ConsoleControl::RED);
    std::cout << std::endl << message;
    ConsoleControl::SetColor(ConsoleControl::WHITE);
    ConsoleControl::UnlockMutex();
}

void Chat::ListenClientsConnections(unsigned short port)
{
    sf::TcpListener listener;

    if (listener.listen(port) != sf::Socket::Done)
    {
        ShowError("Error on start listener");
        return;
    }

    sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
    ShowAlert("Listening on IP: " + ipAddress.toString());

    while (true)
    {
        sf::TcpSocket* client = new sf::TcpSocket();
        sf::Socket::Status status = listener.accept(*client);

        switch (status)
        {
        case sf::Socket::Done:
        {
            std::thread clientThread = std::thread(&Chat::OnClientEnter, this, client);
            clientThread.detach();
            break;
        }
        case sf::Socket::NotReady:
        case sf::Socket::Partial:
        case sf::Socket::Disconnected:
        case sf::Socket::Error:
            break;
        default:
            ShowError("Error on Accept Client");
            break;
        }
    }
}

void Chat::ConnectToServer(std::string ip, unsigned short port)
{
    sf::TcpSocket* socket = new sf::TcpSocket();
    sf::Socket::Status status = socket->connect(ip, port);

    if (status != sf::Socket::Done)
    {
       ShowError("Error on connect to server");
        return;
    }


    _socketMutex.lock();
    _sockets.push_back(socket);
    _socketMutex.unlock();

    ShowAlert("Connected to server");

    std::thread keyboardThread = std::thread(&Chat::ListenKeyboardToSendMessage, this);
    keyboardThread.detach();

    std::thread listenerMessagesThread = std::thread(&Chat::ListenMessages, this, socket);
    listenerMessagesThread.detach();
}

void Chat::OnClientEnter(sf::TcpSocket* client)
{
    _socketMutex.lock();
    _sockets.push_back(client);
    _socketMutex.unlock();

    ShowAlert("Client Accepted IP:" + client->getRemoteAddress().toString());

    ListenMessages(client);

}

void Chat::ListenMessages(sf::TcpSocket* socket)
{
    while (true)
    {
        char data[100] = {};
        std::size_t received;

        std::string message;

        if (socket->receive(data, 100, received) != sf::Socket::Done)
        {
            ShowError("Error sending message");
        }
        else
        {
            for (size_t i = 0; i < received; i++)
            {
                char c = data[i];
                message += c;
            }
            ShowMessage(message);


            _isServerMutex.lock();
            bool isServer = _isServer;
            _isServerMutex.unlock();


            if (_isServer)
            {
                SendMessage(message);
            }
        }
    }
}

void Chat::ListenKeyboardToSendMessage()
{
    std::string message = " ";

    while (true)
    {
        char c = ConsoleControl::WaitForReadNextChar();

        if (c == KB_Enter)
        {
            SendMessage(message);
            _isServerMutex.lock();
            bool isServer = _isServer;
            _isServerMutex.unlock();

            if (isServer)
            {
                ShowMessage(message);
            }
            message = " ";
        }
        else
        {
            message += c;
        }
    }
}

void Chat::SendMessage(std::string message)
{
    char data[100] = {};

    int stringSize = message.length();

    for (int i = 0; i < stringSize; i++)
    {
        char c = message[i];
        data[i] = c;
    }    

    _socketMutex.lock();

    for (sf::TcpSocket* socket : _sockets)
    {
        if (socket->send(data, 100) != sf::Socket::Done)
        {
            ShowError("Error sending message");
        }
    }

    _socketMutex.unlock();
}

Chat* Chat::Server(unsigned short port)
{
	Chat* chat = new Chat();
	chat->_serverAddress = sf::IpAddress::getLocalAddress();
    chat->_isServer = true;

    std::thread listenerThread = std::thread(&Chat::ListenClientsConnections, chat, port);
    listenerThread.detach();

    std::thread keyboardThread = std::thread(&Chat::ListenKeyboardToSendMessage, chat);
    keyboardThread.detach();

	return chat;
}

Chat* Chat::Client(std::string ip, unsigned short port)
{
	Chat* chat = new Chat();

    chat->_serverAddress = sf::IpAddress(ip);

    chat->ConnectToServer(ip, port);

	return chat;
}
