#pragma once
/**
 * @file CryptoHelper.h
 * @brief Proporciona utilidades de cifrado RSA y AES.
 */

#include "Prerequisites.h"
#include <openssl/rsa.h>
#include <openssl/aes.h>

class
CryptoHelper {
public:
  /**
   * @brief Inicializa la estructura, generando un par de claves vacío.
   */
  CryptoHelper();

  /**
   * @brief Libera los recursos asociados a las claves generadas.
   */
  ~CryptoHelper();

  /**
   * @brief Genera un nuevo par de claves RSA de 2048 bits.
   */
  void
  GenerateRSAKeys();

  /**
   * @brief Obtiene la clave pública en formato PEM.
   *
   * @return Clave pública serializada como cadena.
   */
  std::string
    GetPublicKeyString() const;

  /**
   * @brief Carga la clave pública del par con el que se intercambiarán datos.
   *
   * @param pemKey Clave pública en formato PEM.
   */
  void
    LoadPeerPublicKey(const std::string& pemKey);

  /**
   * @brief Genera una nueva clave AES-256 aleatoria.
   */
  void
  GenerateAESKey();

  /**
   * @brief Cifra la clave AES con la clave pública del par.
   *
   * @return Clave AES cifrada.
   */
  std::vector<unsigned char>
  EncryptAESKeyWithPeer();

  /**
   * @brief Descifra la clave AES utilizando la clave privada local.
   *
   * @param encryptedKey Clave AES cifrada recibida del par.
   */
  void
  DecryptAESKey(const std::vector<unsigned char>& encryptedKey);

  /**
   * @brief Cifra un mensaje de texto plano usando AES-CBC.
   *
   * @param plaintext Texto en claro a cifrar.
   * @param outIV Vector donde se almacena el IV generado.
   * @return Mensaje cifrado.
   */
  std::vector<unsigned char>
  AESEncrypt(const std::string& plaintext, std::vector<unsigned char>& outIV);

  /**
   * @brief Descifra un mensaje cifrado con AES-CBC.
   *
   * @param ciphertext Datos cifrados a descifrar.
   * @param iv Vector de inicialización utilizado durante el cifrado.
   * @return Mensaje en texto plano.
   */
  std::string
  AESDecrypt(const std::vector<unsigned char>& ciphertext,
             const std::vector<unsigned char>& iv);

private:
  RSA* rsaKeyPair; /**< Par de claves RSA para cifrado/descifrado. */
  RSA* peerPublicKey; /**< Clave pública del par para cifrado. */
  unsigned char aesKey[32]; /**< Clave AES-256 usada para cifrar mensajes. */

};
