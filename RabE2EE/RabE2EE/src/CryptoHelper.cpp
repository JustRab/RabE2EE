#include "CryptoHelper.h"
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/err.h>

/**
 * @file CryptoHelper.cpp
 * @brief Implementación de utilidades criptográficas RSA y AES.
 */

/**
 * @brief Inicializa los punteros de clave y limpia la clave AES.
 */
CryptoHelper::CryptoHelper() : rsaKeyPair(nullptr), peerPublicKey(nullptr) {
  std::memset(aesKey, 0, sizeof(aesKey));
}

/**
 * @brief Libera las claves RSA al destruir el objeto.
 */
CryptoHelper::~CryptoHelper() {
  if (rsaKeyPair) {
    RSA_free(rsaKeyPair);
  }
  if (peerPublicKey) {
    RSA_free(peerPublicKey);
  }
}

/**
 * @brief Genera un par de claves RSA de 2048 bits.
 */
void
CryptoHelper::GenerateRSAKeys() {
  BIGNUM* bn = BN_new();
  BN_set_word(bn, RSA_F4); // Use public exponent 65537
  rsaKeyPair = RSA_new(); 
  RSA_generate_key_ex(rsaKeyPair, 2048, bn, nullptr);
  BN_free(bn);
}

/**
 * @brief Obtiene la clave pública en formato PEM.
 */
std::string
CryptoHelper::GetPublicKeyString() const {
  BIO* bio = BIO_new(BIO_s_mem());
  PEM_write_bio_RSA_PUBKEY(bio, rsaKeyPair);
  char* buffer = nullptr; //KeyData
  long length = BIO_get_mem_data(bio, &buffer);
  std::string publicKey(buffer, length);
  BIO_free(bio);
  return publicKey;
}

/**
 * @brief Carga la clave pública del par desde una cadena PEM.
 *
 * @param pemKey Cadena en formato PEM.
 */
void
CryptoHelper::LoadPeerPublicKey(const std::string& pemKey) {
  BIO* bio = BIO_new_mem_buf(pemKey.data(), static_cast<int>(pemKey.size()));
  peerPublicKey = PEM_read_bio_RSA_PUBKEY(bio, nullptr, nullptr, nullptr);
  BIO_free(bio);
  if (!peerPublicKey) {
    throw std::runtime_error("Failed to load peer public key" + std::string(ERR_error_string(ERR_get_error(), nullptr)));
  }
}
/**
 * @brief Genera una clave AES-256 aleatoria.
 */
void
CryptoHelper::GenerateAESKey() {
  RAND_bytes(aesKey, sizeof(aesKey));
}

/**
 * @brief Cifra la clave AES con la clave pública del par.
 *
 * @return Clave AES cifrada.
 */
std::vector<unsigned char>
CryptoHelper::EncryptAESKeyWithPeer() {
  if (!peerPublicKey) {
    throw std::runtime_error("Peer public key not loaded");
  }
  std::vector<unsigned char> encryptedKey(256);
  int result = RSA_public_encrypt(sizeof(aesKey),
                                  aesKey, 
                                  encryptedKey.data(), 
                                  peerPublicKey, 
                                  RSA_PKCS1_OAEP_PADDING);
  encryptedKey.resize(result);

  return encryptedKey;
}

/**
 * @brief Descifra la clave AES recibida.
 *
 * @param encryptedKey Clave cifrada a descifrar.
 */
void
CryptoHelper::DecryptAESKey(const std::vector<unsigned char>& encryptedKey) {
  RSA_private_decrypt(encryptedKey.size(),
                      encryptedKey.data(),
                      aesKey,
                      rsaKeyPair,
                      RSA_PKCS1_OAEP_PADDING);
}

/**
 * @brief Cifra un texto plano usando AES en modo CBC.
 *
 * @param plaintext Texto a cifrar.
 * @param outIV Vector donde se guardará el IV generado.
 * @return Texto cifrado en bytes.
 */
std::vector<unsigned char>
CryptoHelper::AESEncrypt(const std::string& plaintext,
                         std::vector<unsigned char>& outIV) {
  outIV.resize(AES_BLOCK_SIZE);
  RAND_bytes(outIV.data(), AES_BLOCK_SIZE);
  std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
  AES_KEY aesKeyEnc;
  AES_set_encrypt_key(aesKey, 256, &aesKeyEnc);
  AES_cbc_encrypt(reinterpret_cast<const unsigned char*>(plaintext.data()),
                  ciphertext.data(),
                  plaintext.size(),
                  &aesKeyEnc,
                  outIV.data(),
                  AES_ENCRYPT);
  return ciphertext;
}

/**
 * @brief Descifra un texto cifrado con AES-CBC.
 *
 * @param ciphertext Datos cifrados.
 * @param iv Vector de inicialización utilizado en el cifrado.
 * @return Texto plano resultante.
 */
std::string
CryptoHelper::AESDecrypt(const std::vector<unsigned char>& ciphertext,
                         const std::vector<unsigned char>& iv) {
  std::vector<unsigned char> plaintext(ciphertext.size());
  AES_KEY aesKeyDec;
  AES_set_decrypt_key(aesKey, 256, &aesKeyDec);
  AES_cbc_encrypt(ciphertext.data(),
                  plaintext.data(),
                  ciphertext.size(),
                  &aesKeyDec,
                  const_cast<unsigned char*>(iv.data()),
                  AES_DECRYPT);
  return std::string(reinterpret_cast<char*>(plaintext.data()), ciphertext.size());
}
