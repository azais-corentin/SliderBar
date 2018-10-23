#ifndef __PROTODEC_H__
#define __PROTODEC_H__

#include "protodef.h"

#include "Buffer.hpp"
#include "SliderBar.h"

namespace protocol {
/**
 * @brief Encodes the data byte into packet, escaping the data if necessary.
 * 
 * @param packet The 
 * @param data 
 * @param escape 
 */
void encode8(BufferSB& buffer, const uint8_t& data, const bool& escape = true);
void encode16(BufferSB& buffer, const uint16_t& data, const bool& escape = true);
uint8_t decode8(const BufferSB& buffer, uint8_t& i);
uint16_t decode16(const BufferSB& buffer, uint8_t& i);

/**
 * @brief Decodes a buffer containing 1 command.
 * Sets the 'crc_valid' member variable to false if there was an error decoding.
 * 
 * @param buf The buffer to decode.
 * @return command The decoded command. command.crc_valid is set to false if there was an error.
 */
command decode(BufferSB& packet);
}

#endif // __PROTODEC_H__