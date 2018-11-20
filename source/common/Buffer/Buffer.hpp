#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <array>
#include <cstdint>

template <uint8_t N>
class Buffer {
public:
    Buffer();
    Buffer(uint8_t* _data, uint8_t length);
    virtual ~Buffer() = default;

    uint8_t* data(uint8_t i = 0) { return buffer + i; }
    Buffer<N> extract(uint8_t iBegin, uint8_t iEnd);

    uint8_t at8(uint8_t i) const;
    uint16_t at16(uint8_t i) const;

    // TODO: Move this, this should be in the protodec part
    int8_t at8_signed(uint8_t i) const;
    void clear();

    /**
     * @brief Copies data to the end of the buffer.
     * @note The data will be truncated if it goes past the maximum size of the
     * buffer.
     *
     * @param data: The data to copy.
     * @param len: The length of the data to copy.
     * @return true All the data was copied.
     * @return false Not all the data was copied, or no data was copied.
     */
    bool append(const uint8_t* data, const uint8_t& len);
    bool append(const Buffer<N>& data);

    bool append(const uint8_t& ch);
    bool append(const uint16_t& ch);

    /**
     * @brief Copies data to a position of the buffer.
     * @note The copy will fail and return false if it goes past the maximum size of the
     * buffer.
     *
     * @param data: The data to copy.
     * @param len: The length of the data to copy.
     * @return true All the data was copied.
     * @return false No data was copied.
     */
    bool write(const uint8_t ch, const uint8_t i);
    bool write(const uint16_t ch, const uint8_t i);
    bool write(const uint32_t ch, const uint8_t i);

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
    bool full() { return index >= N; };

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
     *  @brief Keep the data from position to position + length, removing everything else.
     *  
     *  @param position: The position to cut from.
     *  @param length: The length of the copy. (-1 to copy until the end)
     */
    void mid(uint8_t position, uint8_t length = -1);

    /**
     *  @brief Removes the last n values.
     *  This replaces the last n values with 0 and changes the index.
     *  
     *  @param n: number of values to erase.
     */
    void chop(uint8_t n);

private:
    uint8_t buffer[N];
    uint8_t index;
};

#include "Buffer.tpp"

#endif // __BUFFER_H__
