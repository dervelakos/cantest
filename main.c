
#include <canlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "interfaces/CANlib.h"

#define CHANNEL 0
#define CANID 123
#define FREQUENCY canFD_BITRATE_1M_80P

#define PACKET_SIZE 12

kvBusParamsTq paramsArbitration = {80, 2, 2, 2, 75, 1};
kvBusParamsTq paramsData = {10, 7, 2, 2, 0, 1};

int terminate = 0;

void generate_packet_payload(Packet *p, int len){
    uint8_t *buffer = (uint8_t *)p->getPayload();
    p->setLength(len);
    for(int i=0;i<p->getLength();i++){
        buffer[i] = (uint8_t)rand() & 0xFF;
    }
}

void print_packet_payload(Packet *p){
    uint8_t *buffer = (uint8_t *)p->getPayload();
    for(int i=0;i<p->getLength();i++){
        printf("%02x ", buffer[i] & 0xFF);
    }
    printf("\n");
}

void sig_handler(int signum){

  //Return type of the handler function should be void
  printf("\nExiting\n");
  terminate = 1;
}

int main(){
    errorStatus ret;

    srand(time(0));
    signal(SIGINT,sig_handler); // Register signal handler

    printf("--- Parameters ---\n");
    printf("Channel: %d\n", CHANNEL);
    printf("Frequency: %d\n", FREQUENCY);
    printf("Send Can ID: %d\n", CANID);

    printf("--- Initializing CANlib ---\n");
    CANlib *can = new CANlib("CANlib",CHANNEL ,paramsArbitration, paramsData);
    ret = can->Init();
    if(ret != 0){
	printf("Failed to initialize CAN (%d)\n", ret);
    }
    printf("Done\n");


    printf("--- Sending & Receiving ---\n");
    DestCAN *target_device = new DestCAN(CANID);
    DestCAN *sender_addr = new DestCAN(0);
    Packet *output_packet = new Packet(PACKET_SIZE);
    Packet *input_packet = new Packet(PACKET_SIZE);
    while(!terminate){
        generate_packet_payload(output_packet,PACKET_SIZE);

        printf("Sending Packet:\n");
        print_packet_payload(output_packet);
        ret = can->SendPacket(output_packet, target_device);

        sleep(1);

        ret = can->ReadPacketFrom(input_packet, sender_addr);
        if(ret == E_OK){
            printf("Received Packet\n");
            print_packet_payload(input_packet);
        }else if(ret = E_NOMSG){
            printf("No Packet Received\n");
        }else{
	    printf("Generic CANlib error\n");
	}
        
    }
    printf("Done\n");

    printf("--- Freeing Resources ---\n");
    can->Close();
    delete can;
    delete target_device;
    delete sender_addr;
    delete output_packet;
    delete input_packet;
    printf("Done\n");

}
