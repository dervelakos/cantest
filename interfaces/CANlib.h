#pragma once

#include <canlib.h>
#include <string>

#include "interfaces/MultiDestInterface.h"

#define CAN_WRITE_TIMEOUT 500

class CANlib: public MultiDestInterface
{

private:
    //Init variables
    std::string interfaceName;
    int channel;
    kvBusParamsTq arbit;
    kvBusParamsTq data;


    canHandle hnd;

    size_t maxPacketLen = 8;

    bool connectionActive = false;

public:
    CANlib(std::string interfaceName,
           int channel,
           kvBusParamsTq paramArbitration,
           kvBusParamsTq paramData
           );
    ~CANlib();

    errorStatus Init();
    errorStatus Close();

    errorStatus RecvFrom(void *buf, size_t len, DestCAN *dest);
    errorStatus Write(void *buf, size_t len, DestCAN *dest);

    errorStatus ReadPacketFrom(Packet *packet, DestCAN *dest);
    errorStatus SendPacket(Packet *packet, DestCAN *dest);

    bool IsConnected();

    size_t GetMaxPacketSize();
};
