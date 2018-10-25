#ifndef __PROTODEC_H__
#define __PROTODEC_H__

#include "protodef.h"

#include <Buffer.hpp>
#include <SliderBar.h>

namespace protocol {
/**
 * @brief Encodes a byte into the buffer, escaping if needed.
 * 
 * @param buffer The buffer to write into.
 * @param data The byte to encode.
 * @param escape Whether to escape the byte if needed or not.
 */
void encode8(Buffer& buffer, const uint8_t& data, const bool& escape = true);

/**
 * @brief Encodes 2 bytes into the buffer, escaping if needed.
 * @note MSB is first encoded.
 * 
 * @param buffer The buffer to write into.
 * @param data The byte to encode.
 * @param escape Whether to escape the byte if needed or not.
 */
void encode16(Buffer& buffer, const uint16_t& data, const bool& escape = true);

/**
 * @brief Decodes a byte from the buffer and increments the index.
 * @note The index might be increased by more than one if the data was escaped.
 * 
 * @param buffer The buffer to decode from
 * @param i The index of the data to decode.
 * @return uint8_t The decoded byte of data.
 */
uint8_t decode8(const Buffer& buffer, uint8_t& i);

/**
 * @brief Decodes 2 bytes from the buffer and increments the index.
 * @note MSB is first decoded.
 * @note The index might be increased by more than two if the data was escaped.
 * 
 * @param buffer The buffer to decode from
 * @param i The index of the data to decode.
 * @return uint8_t The decoded bytes of data.
 */
uint16_t decode16(const Buffer& buffer, uint8_t& i);

/**
 * @brief Decodes a buffer containing 1 command.
 * Sets the 'crc_valid' member variable to false if there was an error decoding.
 * @note The packet must only contain the command with the CRC: no start/end flags.
 * 
 * @param buf The buffer to decode.
 * @return command The decoded command. command.crc_valid is set to false if there was an error.
 */
command decode(Buffer& packet);

namespace CRC8 {
    /**
     * @brief Computes the next iteration of the crc
     * 
     * @param input_byte The byte to process
     * @param crc The crc of the current iteration
     * @return uint8_t The next crc iteration
     */
    static inline uint8_t roll(uint8_t input_byte, uint8_t crc)
    {
        for (uint8_t i = 8; i; i--, input_byte >>= 1) {
            uint8_t result = (crc ^ input_byte) & 0x01;
            crc >>= 1;
            if (result)
                crc ^= 0x8C;
        }
        return crc;
    }

    /**
     * @brief Computes the CRC of a byte array
     * 
     * @param input_array The byte array
     * @param length The length of the byte array (in bytes)
     * @return uint8_t The computed CRC
     */
    static inline uint8_t compute(const uint8_t* input_array, uint8_t length)
    {
        uint8_t crc = 0;
        for (uint8_t b = 0; b < length; b++)
            crc = roll(input_array[b], crc);
        return crc;
    }
} // namespace CRC8
} // namespace protocol

#endif // __PROTODEC_H__