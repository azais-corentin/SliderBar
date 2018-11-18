#ifndef __PROTODEC_H__
#define __PROTODEC_H__

#include "protodef.h"

#include "../Buffer/Buffer.hpp"

namespace protocol {
/**
 * @brief Encodes data into the buffer, protocol escaping if needed.
 * @note The MSB is encoded first.
 * @note Support types for data are:
 *  - uint8_t, uint16_t, uint32_t, uint64_t
 *  - int8_t, int16_t, int32_t, int64_t
 *  - float
 *  - std::string
 *
 * @param buffer The buffer to write into.
 * @param data The data to encode. Can be of any suported type.
 * @param escape Whether to escape the byte if needed or not.
 */
template <class T>
void encode(Buffer<MAX_PACKET_SIZE>& buffer, const T& data, const bool& escape = true);

/**
 * @brief Decodes data from the buffer and increments the index.
 *
 * @param buffer The buffer to decode from.
 * @param i The index of the data to decode.
 * @return The decoded data.
 */
template <class T>
T decode(Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i);

namespace detail {
    // Implementation details

    // Primary template
    template <class T, size_t n>
    struct F {
        static void encodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, const T& data, const bool& escape = true);
        static T decodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i);
    };

    // <float, n> partial specialization
    template <size_t n>
    struct F<float, n> {
        static void encodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, const float& data, const bool& escape = true);
        static float decodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i);
    };

    // <uint8_t, 1> full specialization
    template <>
    struct F<uint8_t, 1> {
        static void encodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, const uint8_t& data, const bool& escape = true);
        static uint8_t decodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i);
    };

} // namespace detail

/**
 * @brief Decodes a buffer containing 1 command.
 * Sets the 'crc_valid' member variable to false if there was an error decoding.
 * @note The packet must only contain the command with the CRC: no start/end flags.
 * 
 * @param buf The buffer to decode.
 * @return command The decoded command. command.crc_valid is set to false if there was an error.
 */
message decode(Buffer<MAX_PACKET_SIZE>& packet);

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
