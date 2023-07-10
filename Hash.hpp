/*
 *  Copyright (c) (2023) SPHINX_ORG
 *  Authors:
 *    - (C kusuma) <thekoesoemo@gmail.com>
 *      GitHub: (https://github.com/chykusuma)
 *  Contributors:
 *    - (Contributor 1) <email1@example.com>
 *      Github: (https://github.com/yourgit)
 *    - (Contributor 2) <email2@example.com>
 *      Github: (https://github.com/yourgit)
 */


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// The provided code implements a SWIFFTX 256 digest size hash function for calculating the hash of a message.

// The SPHINX_256 function takes a message as input and calculates its SWIFFTX-256 hash using the SWIFFTX library. Here's how the process works:

// A hashState object state and a BitSequence array hashval of size 32 (corresponding to the 256-bit hash output) are declared.
  // The Init function is called to initialize the state object with a hash bit length of 256.

  // The Update function is called to process the message. The message is converted to a BitSequence array and its length is multiplied by 8 to convert it to bits. The Update function updates the state object with the message data.

  // The Final function is called to finalize the hash calculation. The resulting hash value is stored in the hashval array.

  // The hash value is then converted to a hexadecimal string representation. This is done by iterating over each element in the hashval array, converting it to a two-digit hexadecimal representation using the sprintf function, and appending it to the result string.

  // The result string, containing the hexadecimal hash value, is returned as the output of the sha3_256 function.

// Note: that the code provided is a simplified version and may require additional implementation details for error handling, memory management, and other functionalities related to using the SWIFFTX library and calculating SSPHINX-256 hashes.
/////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef HASH_HPP
#define HASH_HPP

#pragma once

#include <string>
#include <vector>

#include "Lib/Swifftx/stdint.h"
#include "Lib/swifftx/SWIFFTX.h"


namespace SPHINXHash {

std::string SPHINX_256(const std::string& message) {
    swifftx::hash::SWIFFTX256 hasher;
    swifftx::hash::digest256_t digest;

    hasher.init();
    hasher.update(reinterpret_cast<const uint8_t*>(message.c_str()), message.length());
    hasher.final(digest);

    // Convert the digest value to a hexadecimal string
    std::string result;
    for (int i = 0; i < 32; ++i) {
        char hex[3];
        sprintf(hex, "%02x", digest[i]);
        result += hex;
    }

    return result;
}

} // namespace SPHINXHash

#endif // HASH_HPP