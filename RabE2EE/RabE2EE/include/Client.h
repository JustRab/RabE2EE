#pragma once
#include "NetworkHelper.h"
#include "CryptoHelper.h"
#include "Prerequisites.h"

/**
 * @file Client.h
 * @brief Declaración de la clase Client que maneja la comunicación del lado del cliente.
 */
class Client {
public:
        /**
         * @brief Construye un cliente sin conexión.
         */
        Client() = default;

        /**
         * @brief Construye un cliente con la información del servidor.
         * @param ip Dirección IP del servidor.
         * @param port Puerto del servidor.
         */
        Client(const std::string& ip, int port);

        /**
         * @brief Libera los recursos y cierra el socket del cliente.
         */
        ~Client();

        /**
         * @brief Establece conexión con el servidor.
         *
         * @return true si la conexión fue exitosa.
         * @return false si ocurrió un error.
         */
        bool Connect();

        /**
         * @brief Intercambia claves públicas con el servidor.
         */
        void ExchangeKeys();

        /**
         * @brief Cifra la clave AES con la pública del servidor y la envía.
         */
        void SendAESKeyEncrypted();

        /**
         * @brief Cifra un mensaje con AES y lo envía al servidor.
         *
         * @param message Texto plano a cifrar y enviar.
         */
        void SendEncryptedMessage(const std::string& message);

        /**
         * @brief Envía mensajes cifrados en un bucle hasta que el usuario salga.
         */
        void SendEncryptedMessageLoop();

        /**
         * @brief Inicia los hilos de envío y recepción para el chat.
         */
        void StartChatLoop();

        /**
         * @brief Hilo de recepción que descifra y muestra los mensajes del servidor.
         */
        void StartReceiveLoop();

private:
        std::string m_ip;          ///< Dirección IP del servidor.
        int m_port;                 ///< Puerto del servidor.
        SOCKET m_serverSock;        ///< Socket utilizado para la comunicación.
        NetworkHelper m_net;        ///< Utilidad de red para enviar y recibir datos.
        CryptoHelper m_crypto;      ///< Utilidad criptográfica para cifrado y descifrado.
};

