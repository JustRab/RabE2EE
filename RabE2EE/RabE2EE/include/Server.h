#pragma once

class Server {
public:
  Server() = default;
  Server(int port);

  ~Server();

  bool Start();
  void WaitForClient();
  void ReceiveEncryptedMessage();

private:
  int m_port;
  int m_clientSock;
  int m_net;
  int m_crypto;
};