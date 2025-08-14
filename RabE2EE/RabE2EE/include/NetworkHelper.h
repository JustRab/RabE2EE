#pragma once
/**
 * @file NetworkHelper.h
 * @brief Encapsula operaciones básicas de sockets usando Winsock.
 */

#include "Prerequisites.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class
NetworkHelper {
public:
        /**
         * @brief Inicializa la librería Winsock.
         */
        NetworkHelper();

        /**
         * @brief Libera recursos de Winsock y cualquier socket activo.
         */
        ~NetworkHelper();

        /**
         * @brief Inicia un servidor TCP escuchando en el puerto especificado.
         *
         * @param port Puerto TCP donde escuchar.
         * @return true si el servidor se inició correctamente.
         */
        bool
  StartServer(int port);

  /**
   * @brief Acepta una conexión entrante del cliente.
   *
   * @return Socket del cliente conectado o INVALID_SOCKET en caso de error.
   */
  SOCKET
  AcceptClient();

        /**
         * @brief Conecta el cliente a un servidor dado su IP y puerto.
         *
         * @param ip Dirección IP del servidor.
         * @param port Puerto del servidor.
         * @return true si la conexión fue exitosa.
         */
        bool
  ConnectToServer(const std::string& ip, int port);

  /**
   * @brief Envía datos de texto plano a través de un socket.
   *
   * @param socket Descriptor de socket.
   * @param data Cadena a enviar.
   * @return true si los datos se enviaron correctamente.
   */
  bool
  SendData(SOCKET socket, const std::string& data);

  /**
   * @brief Envía datos binarios a través de un socket.
   *
   * @param socket Descriptor de socket.
   * @param data Vector con los bytes a enviar.
   * @return true si los datos se enviaron correctamente.
   */
  bool
  SendData(SOCKET socket, const std::vector<unsigned char>& data);

  /**
   * @brief Recibe datos en formato texto desde un socket.
   *
   * @param socket Descriptor de socket.
   * @return Datos recibidos como cadena.
   */
  std::string
  ReceiveData(SOCKET socket);

  /**
   * @brief Recibe datos binarios desde un socket.
   *
   * @param socket Descriptor de socket.
   * @param size Cantidad de bytes a recibir.
   * @return Vector con los datos recibidos.
   */
  std::vector<unsigned char>
  ReceiveDataBinary(SOCKET socket, int size = 0);

  /**
   * @brief Cierra un socket activo.
   *
   * @param socket Descriptor a cerrar.
   */
  void
  Close(SOCKET socket);

public:
  SOCKET m_serverSocket = -1; /**< Socket principal del servidor o cliente. */

private:
  bool m_initialized = false; /**< Indica si Winsock fue inicializado. */

};
