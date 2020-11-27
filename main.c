
#include <canlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "interfaces/CANlib.h"

#define CHANNEL 0
#define CANID 123
#define FREQUENCY canBITRATE_250K

#define PACKET_SIZE 10

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
    srand(time(0));
    signal(SIGINT,sig_handler); // Register signal handler

    printf("--- Parameters ---\n");
    printf("Channel: %d\n", CHANNEL);
    printf("Frequency: %d\n", FREQUENCY);
    printf("Send Can ID: %d\n", CANID);

    printf("--- Initializing CANlib ---\n");
    CANlib *can = new CANlib("CANlib",CHANNEL, FREQUENCY);
    can->Init();
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
        errorStatus ret = can->SendPacket(output_packet, target_device);

        sleep(1);

        can->ReadPacketFrom(input_packet, sender_addr);
        if(ret = E_OK){
            printf("Received Packet\n");
            print_packet_payload(input_packet);
        }else{
            printf("No Packet Received\n");
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