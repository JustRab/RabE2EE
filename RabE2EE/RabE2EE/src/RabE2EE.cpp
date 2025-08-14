#include "openssl/crypto.h"
#include <iostream>

/**
 * @file RabE2EE.cpp
 * @brief Punto de entrada para mostrar la versión de OpenSSL.
 */

/**
 * @brief Función principal del programa de ejemplo.
 */
int
main() {
  std::cout << "OpenSSL version: " << OpenSSL_version(OPENSSL_VERSION) << std::endl;
  return 0;
}

