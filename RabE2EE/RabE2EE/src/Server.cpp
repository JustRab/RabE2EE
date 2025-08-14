#include "Server.h"

/**
 * @file Server.cpp
 * @brief Implementación de la lógica del servidor para el chat cifrado.
 */

 /**
  * @brief Constructor que inicializa el puerto y genera las claves RSA.
  * @param port Puerto donde el servidor escuchará.
  */
Server::Server(int port) : m_port(port), m_clientSock(-1) {
  // Generar claves RSA al construir
  m_crypto.GenerateRSAKeys();
}

/**
 * @brief Destructor que cierra la conexión con el cliente si está activa.
 */
Server::~Server() {
  if (m_clientSock != -1) {
    m_net.close(m_clientSock);
  }
}

/**
 * @brief Inicia el servidor en el puerto indicado.
 */
bool 
Server::Start() {
  std::cout << "[Server] Iniciando servidor en el puerto " << m_port << "...\n";
  return m_net.StartServer(m_port);
}

/**
 * @brief Espera a que un cliente se conecte e intercambia claves.
 */
void 
Server::WaitForClient() {
  std::cout << "[Server] Esperando conexión de un cliente...\n";

  // Aceptar conexión entrante
  m_clientSock = m_net.AcceptClient();
  if (m_clientSock == INVALID_SOCKET) {
    std::cerr << "[Server] No se pudo aceptar cliente.\n";
    return;
  }
  std::cout << "[Server] Cliente conectado.\n";

  // 1. Enviar clave pública del servidor al cliente
  std::string serverPubKey = m_crypto.GetPublicKeyString();
  m_net.SendData(m_clientSock, serverPubKey);

  // 2. Recibir clave pública del cliente
  std::string clientPubKey = m_net.ReceiveData(m_clientSock);
  m_crypto.LoadPeerPublicKey(clientPubKey);

  // 3. Recibir clave AES cifrada con la pública del servidor
  std::vector<unsigned char> encryptedAESKey = m_net.ReceiveDataBinary(m_clientSock, 256);
  m_crypto.DecryptAESKey(encryptedAESKey);

  std::cout << "[Server] Clave AES intercambiada exitosamente.\n";
}

/**
 * @brief Recibe un mensaje cifrado del cliente, lo descifra y lo imprime.
 */
void
Server::ReceiveEncryptedMessage() {
  // 1. Recibir el IV (vector de inicialización)
  std::vector<unsigned char> iv = m_net.ReceiveDataBinary(m_clientSock, 16);

  // 2. Recibir el mensaje cifrado
  std::vector<unsigned char> encryptedMsg = m_net.ReceiveDataBinary(m_clientSock, 128);

  // 3. Descifrar el mensaje
  std::string msg = m_crypto.AESDecrypt(encryptedMsg, iv);

  // 4. Mostrar mensaje
  std::cout << "[Server] Mensaje recibido: " << msg << "\n";
}

/**
 * @brief Bucle que recibe y muestra mensajes del cliente.
 */
void 
Server::StartReceiveLoop() {
  while (true) {
    // 1) IV (16)
    auto iv = m_net.ReceiveDataBinary(m_clientSock, 16);
    if (iv.empty()) {
      std::cout << "\n[Server] Conexión cerrada por el cliente.\n";
      break;
    }

    // 2) Tamaño (4 bytes network/big-endian)
    auto len4 = m_net.ReceiveDataBinary(m_clientSock, 4);
    if (len4.size() != 4) {
      std::cout << "[Server] Error al recibir tamaño.\n";
      break;
    }
    uint32_t nlen = 0;
    std::memcpy(&nlen, len4.data(), 4);
    uint32_t clen = ntohl(nlen);   // convierte de network a host

    // 3) Ciphertext (clen bytes)
    auto cipher = m_net.ReceiveDataBinary(m_clientSock, static_cast<int>(clen));
    if (cipher.empty()) {
      std::cout << "[Server] Error al recibir datos.\n";
      break;
    }

    // 4) Descifrar y mostrar
    std::string plain = m_crypto.AESDecrypt(cipher, iv);
    std::cout << "\n[Cliente]: " << plain << "\nServidor: ";
    std::cout.flush();
  }
}

/**
 * @brief Bucle que solicita mensajes al usuario y los envía cifrados.
 */
void 
Server::SendEncryptedMessageLoop() {
  std::string msg;
  while (true) {
    std::cout << "Servidor: ";
    std::getline(std::cin, msg);
    if (msg == "/exit") break;

    std::vector<unsigned char> iv;
    auto cipher = m_crypto.AESEncrypt(msg, iv);

    // 1) IV (16)
    m_net.SendData(m_clientSock, iv);

    // 2) Tamaño en network order (htonl)
    uint32_t clen = static_cast<uint32_t>(cipher.size());
    uint32_t nlen = htonl(clen);
    std::vector<unsigned char> len4(
      reinterpret_cast<unsigned char*>(&nlen),
      reinterpret_cast<unsigned char*>(&nlen) + 4
    );
    m_net.SendData(m_clientSock, len4);

    // 3) Ciphertext
    m_net.SendData(m_clientSock, cipher);
  }
  std::cout << "[Server] Saliendo del chat.\n";
}

/**
 * @brief Inicia los hilos de envío y recepción para el chat.
 */
void 
Server::StartChatLoop() {
  std::thread recvThread([&]() {
    StartReceiveLoop();
    });

  SendEncryptedMessageLoop();

  if (recvThread.joinable())
    recvThread.join();
}
