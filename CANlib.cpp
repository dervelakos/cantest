#include "interfaces/CANlib.h"

static void checkStatus(std::string name, std::string id, canStatus stat)
{
  if (stat != canOK) {
    char buf[100];
    buf[0] = '\0';
    // Retreive informational text about the status code
    canGetErrorText(stat, buf, sizeof(buf));
    printf("%s - %s: failed, stat=%d (%s)\n", name.c_str(), id.c_str(), (int)stat, buf);
  }
} // checkStatus

CANlib::CANlib(std::string interfaceName, int channel, long frequency){
    this->channel = channel;
    this->frequency = frequency;

    this->maxPacketLen = 64;
}

CANlib::~CANlib(){

}

errorStatus CANlib::Init(){
    canStatus ret;

    canInitializeLibrary();

    hnd = canOpenChannel(channel, canOPEN_EXCLUSIVE);
    if (hnd < 0) {
        checkStatus(interfaceName, "canOpenChannel", ret);
        return E_NOK;
    }

    ret = canSetBusParams(hnd, frequency, 0, 0, 0, 0, 0);
    if (ret < 0){
        checkStatus(interfaceName, "canSetBusParams", ret);
        return E_NOK;
    }

    ret = canSetBusOutputControl(hnd, canDRIVER_NORMAL);
    if(ret < 0){
        checkStatus(interfaceName, "canSetBusOutputControl", ret);
        return E_NOK;
    }

    ret = canBusOn(hnd);
    if(ret < 0){
        checkStatus(interfaceName, "canBusOn", ret);
        return E_NOK;
    }

    connectionActive = true;
    return E_OK;

}

errorStatus CANlib::Close(){

    canBusOff(hnd);

    /* For now we will assume this is the only instance with channels opened */
    canClose(hnd);

    /* This would close all bus channels */
    // if(canUnloadLibrary() < 0){
    //     printf("Unload library failed.\n");
    //     return -3;
    // }

    connectionActive = false;

    return E_OK;

}

errorStatus CANlib::RecvFrom(void *buf, size_t len, DestCAN *dest){
    canStatus stat;
    unsigned long timestamp;
    unsigned int msg_len;
    unsigned int flags = canFDMSG_FDF;

    long id;
    stat = canRead(hnd, &id, buf, &msg_len, &flags, &timestamp);
    dest->setCanId(id);
    return (stat == 0 ? E_OK : E_NOK);
}

errorStatus CANlib::Write(void *buf, size_t len, DestCAN *dest){
    canStatus stat = canWriteWait(hnd, dest->getCanId(), buf, len, canFDMSG_FDF, CAN_WRITE_TIMEOUT);
    return (stat == 0 ? E_OK : E_NOK);
}

errorStatus CANlib::ReadPacketFrom(Packet *packet, DestCAN *dest){
    return RecvFrom(packet->getPayload(), packet->getLength(), dest);
}

errorStatus CANlib::SendPacket(Packet *packet, DestCAN *dest){
    return Write(packet->getPayload(), packet->getLength(), dest);
}

bool CANlib::IsConnected(){
    return connectionActive;
}

size_t CANlib::GetMaxPacketSize(){
    return maxPacketLen;
}
