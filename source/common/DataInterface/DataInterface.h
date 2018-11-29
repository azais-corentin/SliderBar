#ifndef __DATAINTERFACE_H__
#define __DATAINTERFACE_H__

#include <cstdint>

/**
 * @brief Defines the physical layer <=> data layer Interface.
 * A class should inherit the DataInterface and implement the 
 * void receive(uint8_t* buf, uint8_t len)
 * and
 * void transmit(uint8_t buf, uint8_t len)
 * methods in order to transmit and receive data from and to 
 * the physical layer.
 */
class DataInterface {
public:
    //virtual ~DataInterface();

    /**
     * @brief The connect & disconnect functions
     */
    virtual void connect()    = 0;
    virtual void disconnect() = 0;

    /**
     * @brief The receive method.
     * The receive method should be implemented by child classes.
     * It will be called everytime new data is available from the physical layer.
     * 
     * @param buf The data bytes.
     * @param len The length of the data (in bytes).
     */
    virtual void receive(uint8_t* buf, const uint16_t& len) = 0;

    /**
     * @brief The transmit method.
     * The receive method should be implemented by child classes.
     * It should transmit data to the physical layer.
     * 
     * @param buf The data bytes.
     * @param len The length of the data (in bytes).
     * 
     * @return true If the transfer was successful.
     * @return false If the transfer failed.
     */
    virtual bool transmit(uint8_t* buf, const uint16_t& len) = 0;
};

class DataInInterface;
class DataOutInterface;

class DataInInterface : public DataInterface {
public:
    void setReceiver(DataOutInterface* _receiver)
    {
        receiver = _receiver;
    }

protected:
    DataOutInterface* receiver = nullptr;
};

class DataOutInterface : public DataInterface {
public:
    void setTransmitter(DataInInterface* _transmitter)
    {
        transmitter = _transmitter;
    }

protected:
    DataInInterface* transmitter = nullptr;
};

#endif // __DATAINTERFACE_H__
