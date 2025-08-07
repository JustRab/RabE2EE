#pragma once
#include "Prerequisites.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class 
NetworkHelper {
public:
	NetworkHelper();
	~NetworkHelper();

	// Server mode
  bool 
  StartServer(int port);

  SOCKET
  AcceptClient();

	// Client mode
  bool 
  ConnectToServer(const std::string& ip, int port);
  
  // Send and receive data
  bool 
  SendData(SOCKET socket, const std::string& data);
  
  bool 
  SendData(SOCKET socket, const std::vector<unsigned char>& data);
  
  std::string
  ReceiveData(SOCKET socket);

  std::vector<unsigned char>
  ReceiveDataBinary(SOCKET socket, int size = 0);

  void 
  Close(SOCKET socket);

public:
  SOCKET m_serverSocket = -1;
private:
  bool m_initialized = false;

};
