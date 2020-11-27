#pragma once

class DestCAN{
private:
    long id;

public:
    DestCAN(long canID) : id(canID){};
    ~DestCAN(){};

    long getCanId(){return id;}
    void setCanId(long canID){canID = id;}
};