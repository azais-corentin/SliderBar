#ifndef __SLIDERBAR_H__
#define __SLIDERBAR_H__

#include <cstdint>

#include <DataInterface.h>

#include <protocol_definition.h>

template <uint8_t N>
class Buffer;

/**
 * @brief The SliderBar class.
 * Serves as the main loop of the SliderBar system.
 * This receives commands through USB, and execute actions accordingly.
 * 
 */
class SliderBar : public DataInterface {
public:
    SliderBar();
    ~SliderBar();

    /**
     *  @brief Runs the main loop of the SliderBar
     *  It decodes the messages sent through USB, and moves the motor
     *  accordingly.
     *  
     *  @retval None
     */
    void run();

    /**
     * @brief Receive new data.
     * Called by the data layer (ie: USB, serial, ...) when there is new data
     * available. This should normally contain startflag, packet data 
     * (type, value, crc) and endflag.
     *
     * @param buf Pointer to the data.
     * @param len Length of the data.
     */
    void receive(uint8_t* buf, uint16_t len) final;

    /**
     * @brief Transmits data.
     * Called by encode() internally when a new packed is being sent.
     * 
     * @param buf Pointer to the data.
     * @param len Length of the data.
     * @return true If the transfer was successful.
     * @return false If the transfer failed.
     */
    bool transmit(uint8_t* buf, uint16_t len) final;

    void setTransmitter(DataInterface* _transmitter);

private:
    Buffer<protocol::MAX_PACKET_SIZE>* m_decodeBuffer = nullptr;
    bool newData                                      = false;

    void decode();

    template <class T>
    void encode(const T& msg);

    DataInterface* transmitter = nullptr;
};

#endif // __SLIDERBAR_H__
