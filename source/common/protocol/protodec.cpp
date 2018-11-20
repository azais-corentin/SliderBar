#include "protodec.h"
#include "protodef.h"

namespace protocol {

namespace data {

    template <class T>
    void encode(Buffer<MAX_PACKET_SIZE>& buffer, const T& data, const bool& escape)
    {
        detail::F<T, sizeof(T)>::encodeImpl(buffer, data, escape);
    }

    template <class T>
    T decode(Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
    {
        return detail::F<T, sizeof(T)>::decodeImpl(buffer, i);
    }

    namespace detail {
        // Primary template
        template <class T, size_t n>
        void F<T, n>::encodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, const T& data, const bool& escape)
        {
            for (size_t nByte = n; nByte > 0; nByte--) {
                F<uint8_t, 1>::encodeImpl(buffer, static_cast<uint8_t>(data >> (nByte - 1) * 8), escape);
            }
        }

        template <class T, size_t n>
        T F<T, n>::decodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
        {
            T value = T {};

            T byte;
            for (uint8_t nByte = n; nByte > 0; nByte--) {
                byte = F<uint8_t, 1>::decodeImpl(buffer, i);
                value |= (byte << (nByte - 1) * 8);
            }

            return value;
        }

        // <float, n> partial specialization
        template <size_t n>
        void F<float, n>::encodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, const float& data, const bool& escape)
        {
            // Copy each byte
            uint8_t bytes[n];
            memcpy(bytes, &data, n);

            // Copy the bytes in the buffer in the memory order
            for (size_t nByte = 0; nByte < n; nByte++)
                F<uint8_t, 1>::encodeImpl(buffer, bytes[nByte], escape);
        }

        template <size_t n>
        float F<float, n>::decodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
        {
            float value = 0.f;

            // Copy the bytes directly into the float.
            // This works because the order in the memory is right.
            memcpy(&value, buffer.data(i), n);

            return value;
        }

        // <uint8_t, 1> full specialization
        void F<uint8_t, 1>::encodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, const uint8_t& data, const bool& escape)
        {
            if (escape && isFlag(data)) {
                buffer.append(escapeflag);
                buffer.append(static_cast<uint8_t>(data ^ xorflag));
            } else
                buffer.append(data);
        }

        uint8_t F<uint8_t, 1>::decodeImpl(Buffer<MAX_PACKET_SIZE>& buffer, uint8_t& i)
        {
            if (isFlag(buffer.at8(i++)))
                return buffer.at8(i++) ^ xorflag;
            return buffer.at8(i - 1);
        }

    } // namespace detail

}

message decode(Buffer<MAX_PACKET_SIZE>& packet)
{
    message received;
    received.crc_valid = true;

    // Decodes the type, value, data from the data packet.
    uint8_t i      = 0;
    received.topic = data::decode<uint8_t>(packet, i);
    received.type  = data::decode<uint8_t>(packet, i);

    // 2^(message_type % 4) corresponds to the size in byte of the value
    uint8_t type_size = 1 << (received.type % 4);

    received.value.append(packet.extract(i, i + type_size));

    switch (received.type) {
    case UNSIGNED_INT_8:
        received.value_ui8 = decode<uint8_t>(packet, i);
        break;
    case UNSIGNED_INT_16:
        received.value_ui16 = decode<uint16_t>(packet, i);
        break;
    case UNSIGNED_INT_32:
        received.value_ui32 = decode<uint32_t>(packet, i);
        break;
    case UNSIGNED_INT_64:
        received.value_ui64 = decode<uint64_t>(packet, i);
        break;
    case INT_8:
        received.value_i8 = decode<int8_t>(packet, i);
        break;
    case INT_16:
        received.value_i16 = decode<int16_t>(packet, i);
        break;
    case INT_32:
        received.value_i32 = decode<int32_t>(packet, i);
        break;
    case INT_64:
        received.value_i64 = decode<int64_t>(packet, i);
        break;
    case FLOAT:
        received.value_flt = decode<float>(packet, i);
        break;

    default:
        break;
    }

    uint8_t crc_received = decode(packet, i);

    // Compute CRC
    Buffer<MAX_PACKET_SIZE> receivedBytes;
    encode(receivedBytes, received.type, false);
    encode8(receivedBytes, received.type, false);
    encode16(receivedBytes, received.value, false);
    // TODO: Use hardware CRC generation
    uint8_t crc_computed = CRC8::compute(receivedBytes.data(), receivedBytes.size());

    // Compares CRC (computed vs. received)
    if (crc_computed != crc_received) {
        received.crc_valid = false;
    }

    return received;
}

Buffer<MAX_PACKET_SIZE> encode(const message& msg, const bool& escape)
{
    Buffer<MAX_PACKET_SIZE> packet;

    // Encode topic
    encode(packet, msg.topic, escape);

    // Encode type
    encode(packet, msg.type, escape);

    // Encode value
    switch (msg.type) {
    case UNSIGNED_INT_8:
        encode(packet, msg.value_ui8) break;
    case UNSIGNED_INT_16:
        packet.value_ui16 = decode<uint16_t>(packet, i);
        break;
    case UNSIGNED_INT_32:
        packet.value_ui32 = decode<uint32_t>(packet, i);
        break;
    case UNSIGNED_INT_64:
        packet.value_ui64 = decode<uint64_t>(packet, i);
        break;
    case INT_8:
        packet.value_i8 = decode<int8_t>(packet, i);
        break;
    case INT_16:
        packet.value_i16 = decode<int16_t>(packet, i);
        break;
    case INT_32:
        packet.value_i32 = decode<int32_t>(packet, i);
        break;
    case INT_64:
        packet.value_i64 = decode<int64_t>(packet, i);
        break;
    case FLOAT:
        packet.value_flt = decode<float>(packet, i);
        break;

    default:
        break;
    }
}

} // namespace protocol
