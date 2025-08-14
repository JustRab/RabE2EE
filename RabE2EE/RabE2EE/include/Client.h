#pragma once
/**
 * @file Client.h
 * @brief Declaraciones de la clase Client que maneja la comunicación del lado
 *        del cliente.
 */

#include "NetworkHelper.h"
#include "CryptoHelper.h"
#include "Prerequisites.h"

/**
 * @brief Representa a un cliente que se conecta a un servidor y realiza
 *        intercambio de claves y envío de mensajes cifrados.
 */
class Client {
public:
        /**
         * @brief Crea un cliente sin conexión inicial.
         */
        Client() = default;

        /**
         * @brief Crea un cliente con la dirección y puerto del servidor.
         *
         * @param ip Dirección IP del servidor.
         * @param port Puerto del servidor.
         */
        Client(const std::string& ip, int port);

        /**
         * @brief Libera recursos y cierra la conexión si está activa.
         */
        ~Client();

	/**
	 * @brief Establece conexión con el servidor.
	 *
	 * @return true si la conexión fue exitosa.
	 * @return false si ocurrió un error.
	 */
	bool
		Connect();

	/**
	 * @brief Intercambia claves públicas con el servidor.
	 */
	void
		ExchangeKeys();

	/**
	 * @brief Cifra la clave AES con la pública del servidor y la envía.
	 */
	void
		SendAESKeyEncrypted();

	/**
	 * @brief Cifra un mensaje con AES y lo envía al servidor.
	 *
	 * @param message Texto plano a cifrar y enviar.
	 */
	void
		SendEncryptedMessage(const std::string& message);

private:
	std::string m_ip;
	int m_port;
        SOCKET m_serverSock;
        NetworkHelper m_net;
        CryptoHelper m_crypto;
};
