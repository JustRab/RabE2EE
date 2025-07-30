#pragma once
#include "Prerequisites.h"
#include <openssl/rsa.h>
#include <openssl/aes.h>

class
CryptoHelper {
public:
  CryptoHelper();
  ~CryptoHelper();

  //RSA encryption/decryption
  void 
  GenerateRSAKeys();

  std::string
    GetPublicKeyString() const;

  void
    LoadPeerPublicKey(const std::string& pemKey);

  // AES
  void
  GenerateAESKey();

  std::vector<unsigned char>
  EncryptAESKeyWithPeer();

  void
  DecryptAESKey(const std::vector<unsigned char>& encryptedKey);

  std::vector<unsigned char>
  AESEncrypt(const std::string& plaintext, std::vector<unsigned char>& outIV);

  std::string
  AESDecrypt(const std::vector<unsigned char>& ciphertext, 
             const std::vector<unsigned char>& iv);

private:
  RSA* rsaKeyPair; // RSA key pair for encryption/decryption
  RSA* peerPublicKey; // Peer public key for encryption
  unsigned char aesKey[32]; // AES-256 key

};