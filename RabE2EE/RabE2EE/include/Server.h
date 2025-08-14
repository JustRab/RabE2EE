#pragma once
/**
 * @file Server.h
 * @brief Declaraciones de la clase Server que gestiona las comunicaciones del
 *        lado del servidor.
 */

#include "NetworkHelper.h"
#include "CryptoHelper.h"
#include "Prerequisites.h"

/**
 * @brief Implementa un servidor capaz de intercambiar claves y recibir
 *        mensajes cifrados de un cliente.
 */
class
        Server {
public:
        /**
         * @brief Construye un servidor sin puerto asignado.
         */
        Server() = default;

        /**
         * @brief Construye un servidor que atenderá en el puerto dado.
         *
         * @param port Puerto donde escuchará el servidor.
         */
        Server(int port);

        /**
         * @brief Libera recursos y cierra la conexión con el cliente.
         */
        ~Server();

	/**
	 * @brief Inicia el servidor en el puerto especificado
	 *
	 * @return true si el servidor se inicializï¿½ correctamente
	 * @return false si hubo un error
	 */
	bool
		Start();

	/**
	 * @brief Espera a que un cliente se conecte e intercambia claves pï¿½blicas.
	 */
	void
		WaitForClient();

	/**
	 * @brief Recibe un mensaje cifrado del cliente, lo descifra y lo imprime.
	 */
	void
		ReceiveEncryptedMessage();

private:
	int m_port;
        SOCKET m_clientSock;
        NetworkHelper m_net;
        CryptoHelper m_crypto;
};
