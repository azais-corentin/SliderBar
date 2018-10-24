#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <array>
#include <cstdint>

#include "SliderBar.h"

typedef std::array<uint8_t, MAX_PACKET_SIZE> sbarray;

class Buffer {
public:
    Buffer();
    Buffer(uint8_t* _data, uint8_t length);
    Buffer(sbarray _data);
    virtual ~Buffer();

    uint8_t* data() { return data.data(); }

    uint8_t at8(uint8_t i) const;
    uint16_t at16(uint8_t i) const;
    void clear();

    /**
     * @brief Copies data to the end of the buffer.
     * @note The data will be truncated if it goes past the length of the
     *  buffer.
     *  
     * @param data: The data to copy.
     * @param len: The length of the data to copy.
     * @return true All the data was copied.
     * @return false Not enough space to copy all the data.
     */
    bool append(uint8_t* data, uint8_t len);
    bool append8(uint8_t ch);
    bool append16(uint16_t ch);
    bool write8(uint8_t ch, uint8_t i);
    bool write16(uint16_t ch, uint8_t i);

    /**
     * @brief Returns the size of the buffer.
     * @note Size is the highest modified position, not maximum size.
     * 
     * @return uint8_t The size of the array.
     */
    uint8_t size() { return index; }

    /**
     * @brief Resizes the buffer to length bytes.
     * 
     * @param length The new size of the array (in bytes).
     */
    void resize(uint8_t length) { index = length; }

    /**
     * @brief Returns whether the buffer is full.
     * 
     * @return true The buffer is full.
     * @return false The buffer isn't full.
     */
    bool full() { return index >= MAX_PACKET_SIZE; };

    /**
     *  @brief Returns the number of ch in the buffer.
     *  
     *  @param ch: The value to count occurences.
     *  @retval The number of occurences.
     */
    uint8_t count(uint8_t ch);

    /**
     *  @brief Returns whether or no ch is contained in the buffer.
     *  
     *  @param ch: The value to search.
     *  @retval true if ch is in the buffer, false otherwise.
     */
    bool contains(uint8_t ch);

    /**
     *  @brief Returns the index of the first x.
     *  
     *  @param x: The value to search.
     *  @retval The index of the first x, or -1 if not found.
     */
    int indexOf(uint8_t x);
    /**
     *  @brief Returns the index of the last x.
     *  
     *  @param x: The value to search.
     *  @retval The index of the last x, or -1 if not found.
     */
    int lastIndexOf(uint8_t ch);

    /**
     *  @brief Returns a copy of Buffer cut from position to length.
     *  
     *  @param position: The position to cut from.
     *  @param length: The length of the copy.
     *  @retval The new Buffer object.
     */
    Buffer mid(uint8_t position, uint8_t length = -1);

    /**
     *  @brief Removes the last n values.
     *  This replaces the last n values with 0 and changes the index.
     *  
     *  @param n: number of values to erase.
     */
    void chop(uint8_t n);

private:
    sbarray buffer;
    uint8_t index;
};

#endif // __BUFFER_H__
