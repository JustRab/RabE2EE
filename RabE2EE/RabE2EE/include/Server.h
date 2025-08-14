#pragma once
#include "NetworkHelper.h"
#include "CryptoHelper.h"
#include "Prerequisites.h"

/**
 * @file Server.h
 * @brief Declaración de la clase Server que maneja la comunicación del lado del servidor.
 */
class Server {
public:
        /**
         * @brief Construye un servidor sin inicializar.
         */
        Server() = default;

        /**
         * @brief Construye un servidor que escuchará en el puerto indicado.
         * @param port Puerto en el que se aceptarán conexiones.
         */
        Server(int port);

        /**
         * @brief Libera los recursos y cierra la conexión con el cliente.
         */
        ~Server();

        /**
         * @brief Inicia el servidor en el puerto especificado.
         * @return true si el servidor se inicializó correctamente.
         * @return false si ocurrió un error.
         */
        bool Start();

        /**
         * @brief Espera a que un cliente se conecte e intercambia claves públicas.
         */
        void WaitForClient();

        /**
         * @brief Recibe un mensaje cifrado del cliente, lo descifra y lo imprime.
         */
        void ReceiveEncryptedMessage();

        /**
         * @brief Bucle que recibe mensajes cifrados del cliente.
         */
        void StartReceiveLoop();

        /**
         * @brief Bucle que solicita al usuario mensajes para enviarlos cifrados.
         */
        void SendEncryptedMessageLoop();

        /**
         * @brief Inicia los hilos necesarios para el chat bidireccional.
         */
        void StartChatLoop();

private:
        int m_port;                 ///< Puerto en el que escucha el servidor.
        SOCKET m_clientSock;         ///< Socket del cliente aceptado.
        NetworkHelper m_net;         ///< Utilidad de red para comunicaciones.
        CryptoHelper m_crypto;       ///< Utilidad criptográfica para operaciones RSA y AES.
        std::thread m_rxThread;      ///< Hilo usado para recibir datos.
        std::atomic<bool> m_running{ false }; ///< Indica si el servidor sigue en ejecución.
};

