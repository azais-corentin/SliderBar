#ifndef SLIDERBAR_H
#define SLIDERBAR_H

#include <cstdint>

#include <Buffer/Buffer.hpp>
#include <DataInterface/DataInterface.h>

#include <protocol/messages/sliderbar.pb.h>
#include <protocol/protocol_definition.h>
#include <protocol/protocol_structures.h>

/**
 * @brief The SliderBar class.
 *
 * This is the main controller of the SliderBar system.
 * It's main jobs are:
 * - Interface to the computer via USB CDC
 * - Control the SliderBar's motor
 * - Read the SliderBar's position
 * - Capacitive Touch Sensing (not yet implemented)
 */
class SliderBar : public DataOutInterface {
public:
    SliderBar()  = default;
    ~SliderBar() = default;

    void decode();

    void connect() final {};
    void disconnect() final {};

private:
    /**
     * @brief Receive new data.
     *
     * Called by the data layer (ie: USB, serial, ...) when there is new data
     * available. This should normally contain startflag, packet data
     * (type, value, crc) and endflag.
     *
     * @param buf Pointer to the data.
     * @param len Length of the data.
     */
    void receive(uint8_t* buf, const uint16_t& len) final;

    /**
     * @brief Transmits data.
     * @note Called by encode() internally when a new packed is being sent.
     *
     * @param buf Pointer to the data.
     * @param len Length of the data.
     * @return True if the transfer was successful.
     */
    bool transmit(uint8_t* buf, const uint16_t& len) const final;

    void send(const Response& response) const;
    void sendNack() const;

    // Process a request
    void process(const Request& request);
    void process(const Request_SetValue& value);
    void process(const Request_GetValue& value);
    void process(const Request_Vibrate& value);
    void process(const Request_ResistAt& value);
    void process(const Request_ResistClear& value);
    void process(const Request_Calibration& value);

    /**
     * @brief Calibrates the SliderBar min and max positions, velocities.
     */
    protocol::CalibrationData calibrate() const;

    /**
     * @brief Vibrates the SliderBar for a specified duration
     * @param duration Duration in ms
     */
    void vibrate(uint8_t duration);

private:
    struct Data {
        uint16_t position = 0;
        uint16_t velocity = 0;
        uint16_t gainP    = 10;
        uint16_t gainI    = 1;
    };

    Buffer<protocol::MAX_PACKET_SIZE> m_decodeBuffer;

    protocol::CalibrationData m_dataCalibration;
    Data m_values;
};

#endif // SLIDERBAR_H
