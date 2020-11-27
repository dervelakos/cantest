#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cstdlib>

class Packet{
protected:
    void *buffer;
    size_t len;
    size_t maxLen;
    int priority;

public:
    Packet(size_t bufferSize){
        buffer = malloc(bufferSize);
        maxLen = bufferSize;
    }

    ~Packet(){
        free(buffer);
    }

    size_t getLength(){return len;}
    void setLength(size_t len){this->len=len;}

    int getPriority(){return priority;}
    void setPriority(int prio){priority=prio;}

    void* getPayload(){return buffer;}
    size_t getMaxLength(){return maxLen;}
};