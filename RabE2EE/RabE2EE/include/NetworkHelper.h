#pragma once
#include "Prerequisites.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

/**
 * @file NetworkHelper.h
 * @brief Encapsula operaciones de red basadas en WinSock.
 */
class NetworkHelper {
public:
  /**
   * @brief Inicializa la librería WinSock.
   */
  NetworkHelper();

  /**
   * @brief Libera recursos de WinSock y cierra sockets.
   */
  ~NetworkHelper();

  /**
   * @brief Inicia un socket servidor en el puerto indicado y lo deja en escucha.
   *
   * @param port Puerto TCP para escuchar conexiones entrantes.
   * @return true Si el servidor se inicia correctamente.
   * @return false Si ocurre un error en cualquier paso.
   */
  bool StartServer(int port);

  /**
   * @brief Espera y acepta un cliente entrante.
   *
   * @return Socket del cliente aceptado o INVALID_SOCKET si falla.
   */
  SOCKET AcceptClient();

  /**
   * @brief Conecta al servidor especificado por IP y puerto.
   *
   * @param ip Dirección IP del servidor.
   * @param port Puerto del servidor.
   * @return true Si la conexión fue exitosa.
   * @return false Si la conexión falló.
   */
  bool ConnectToServer(const std::string& ip, int port);

  /**
   * @brief Envía una cadena de texto por el socket.
   */
  bool SendData(SOCKET socket, const std::string& data);

  /**
   * @brief Envía datos binarios por el socket.
   */
  bool SendData(SOCKET socket, const std::vector<unsigned char>& data);

  /**
   * @brief Recibe una cadena de texto del socket.
   */
  std::string ReceiveData(SOCKET socket);

  /**
   * @brief Recibe datos binarios del socket.
   * @param socket Socket de origen.
   * @param size Número de bytes a leer; si es 0 se lee hasta cerrar.
   */
  std::vector<unsigned char> ReceiveDataBinary(SOCKET socket, int size = 0);

  /**
   * @brief Cierra un socket dado.
   */
  void close(SOCKET socket);

  /**
   * @brief Envía todos los bytes solicitados a través del socket.
   */
  bool SendAll(SOCKET s, const unsigned char* data, int len);

  /**
   * @brief Recibe exactamente la cantidad de bytes indicada.
   */
  bool ReceiveExact(SOCKET s, unsigned char* out, int len);

public:
  SOCKET m_serverSocket = -1; ///< Socket principal usado por la instancia.

private:
  bool m_initialized; ///< Indica si WSAStartup se ejecutó correctamente.
};
