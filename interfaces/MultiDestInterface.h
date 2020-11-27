#pragma once

#include "BaseTypes.h"
#include "interfaces/Packet.h"
#include "interfaces/DestCAN.h"

class MultiDestInterface{
public:
    virtual ~MultiDestInterface(){};


    virtual errorStatus Init() = 0;
    virtual errorStatus Close() = 0;

    /* Direct reads/writes to the interface */
    virtual errorStatus RecvFrom(void *buf, size_t len, DestCAN *dest) = 0;
    virtual errorStatus Write(void *buf, size_t len, DestCAN *dest) = 0;

    /* High level packet formated reads/writes */
    virtual errorStatus ReadPacketFrom(Packet *packet, DestCAN *dest) = 0;
    virtual errorStatus SendPacket(Packet *packet, DestCAN *dest) = 0;
    //virtual int QueuePacket(Packet *packet); //For future reference

    virtual bool IsConnected() = 0;
    virtual size_t GetMaxPacketSize() = 0;
};