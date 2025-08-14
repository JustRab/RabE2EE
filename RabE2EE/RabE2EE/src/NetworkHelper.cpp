#include "NetworkHelper.h"

/**
 * @file NetworkHelper.cpp
 * @brief Implementación de funciones de ayuda para networking con Winsock.
 */

/**
 * @brief Inicializa la biblioteca Winsock.
 */
NetworkHelper::NetworkHelper() : m_serverSocket(INVALID_SOCKET), m_initialized(false) {
  WSADATA wsaData;
  int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (result != 0) {
    std::cerr << "WSAStartup failed: " << result << std::endl;
    return;
  }
  else {
    m_initialized = true;
  }
}

/**
 * @brief Libera recursos de Winsock y cierra el socket principal.
 */
NetworkHelper::~NetworkHelper() {
  if (m_serverSocket != INVALID_SOCKET) {
    closesocket(m_serverSocket);
  }
  if (m_initialized) {
    WSACleanup();
  }
}

/**
 * @brief Inicia un servidor TCP en el puerto especificado.
 *
 * @param port Puerto de escucha.
 * @return true si el servidor se inicializó correctamente.
 */
bool
NetworkHelper::StartServer(int port) {
  // Create the TCP socket
  m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_serverSocket == INVALID_SOCKET) {
    std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
    return false;
  }

  // Setup the server address structure (IPv4, any local IP, specified port)
  sockaddr_in serverAddres{};
  serverAddres.sin_family = AF_INET;
  serverAddres.sin_port = htons(port);
  serverAddres.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the address and port
  if (bind(m_serverSocket, (sockaddr*)&serverAddres, sizeof(serverAddres)) == SOCKET_ERROR) {
    std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
    closesocket(m_serverSocket);
    m_serverSocket = INVALID_SOCKET;
    return false;
  }

  // Start listening for incoming connections
  if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR) {
    std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
    closesocket(m_serverSocket);
    m_serverSocket = INVALID_SOCKET;
    return false;
  }

  std::cout << "Server started on port " << port << std::endl;
  return true;
}

/**
 * @brief Espera y acepta una conexión entrante.
 *
 * @return Socket del cliente o INVALID_SOCKET si falla.
 */
SOCKET
NetworkHelper::AcceptClient() {
  SOCKET clientSocket = accept(m_serverSocket, nullptr, nullptr);
  if (clientSocket == INVALID_SOCKET) {
    std::cerr << "Error accepting client: " << WSAGetLastError() << std::endl;
    return INVALID_SOCKET;
  }
  std::cout << "Client connected." << std::endl;
  return clientSocket;
}

/**
 * @brief Conecta a un servidor usando una dirección y puerto dados.
 *
 * @param ip Dirección IP del servidor.
 * @param port Puerto del servidor.
 * @return true si la conexión fue exitosa.
 */
bool
NetworkHelper::ConnectToServer(const std::string& ip, int port) {
  //Create the TCP socket
  m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (m_serverSocket == INVALID_SOCKET) {
    std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
    return false;
  }
  // Setup the server address structure (IPv4, specified IP, specified port)
  sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  inet_pton(AF_INET, ip.c_str(), &serverAddress.sin_addr);

  // Attempt to connect to the server
  if (connect(m_serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
    std::cerr << "Connection failed: " << WSAGetLastError() << std::endl;
    closesocket(m_serverSocket);
    m_serverSocket = INVALID_SOCKET;
    return false;
  }
  std::cout << "Connected to server at " << ip << ":" << port << std::endl;
  return true;
}

/**
 * @brief Envía una cadena por el socket indicado.
 *
 * @param socket Descriptor de socket.
 * @param data Datos a enviar.
 * @return true si el envío fue exitoso.
 */
bool
NetworkHelper::SendData(SOCKET socket, const std::string& data) {
  return send(socket, data.c_str(), static_cast<int>(data.size()), 0) != SOCKET_ERROR;
}

/**
 * @brief Envía datos binarios por el socket indicado.
 *
 * @param socket Descriptor de socket.
 * @param data Bytes a enviar.
 * @return true si el envío fue exitoso.
 */
bool
NetworkHelper::SendData(SOCKET socket, const std::vector<unsigned char>& data) {
  return send(socket, reinterpret_cast<const char*>(data.data()),
              static_cast<int>(data.size()), 0) != SOCKET_ERROR;
}

/**
 * @brief Recibe datos de texto desde un socket.
 *
 * @param socket Descriptor de socket.
 * @return Cadena recibida.
 */
std::string
NetworkHelper::ReceiveData(SOCKET socket) {
  char buffer[4096] = {};
  int len = recv(socket, buffer, sizeof(buffer), 0);

  return std::string(buffer, len);
}

/**
 * @brief Recibe datos binarios desde un socket.
 *
 * @param socket Descriptor de socket.
 * @param size Número de bytes a leer.
 * @return Vector con los datos recibidos.
 */
std::vector<unsigned char>
NetworkHelper::ReceiveDataBinary(SOCKET socket, int size) {
  std::vector<unsigned char> buffer(size);
  int len = recv(socket, reinterpret_cast<char*>(buffer.data()), size, 0);
  return buffer;
}

/**
 * @brief Cierra un socket.
 *
 * @param socket Descriptor a cerrar.
 */
void
NetworkHelper::Close(SOCKET socket) {
    closesocket(socket);
}
