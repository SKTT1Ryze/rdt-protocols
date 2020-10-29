// GBNRdtReceiver Implementation
// 

#include "Global.h"
#include "GBNRdtReceiver.h"


GBNRdtReceiver::GBNRdtReceiver() {
    lastpak.acknum = -1;
    lastpak.checksum = 0;
    lastpak.seqnum = -1;
    for(int i = 0; i < Configuration::PAYLOAD_SIZE; i++) {
        lastpak.payload[i] = '.';
    }
    lastpak.checksum = pUtils->calculateCheckSum(lastpak);
}

GBNRdtReceiver::~GBNRdtReceiver() {
    
}

void GBNRdtReceiver::receive(const Packet &packet) {
    int checksum = pUtils->calculateCheckSum(packet);
    if(checksum == packet.checksum && this->exceptseqnum == packet.seqnum) {
        pUtils->printPacket("[Receiver]receive correct packet: ", packet);

        Message message;
        memcpy(message.data, packet.payload, sizeof(packet.payload));
        pns->delivertoAppLayer(RECEIVER, message);

        this->lastpak.acknum = packet.seqnum;
        this->lastpak.checksum = pUtils->calculateCheckSum(this->lastpak);
        pUtils->printPacket("[Receiver]send ACK: ", this->lastpak);
        
        pns->sendToNetworkLayer(SENDER, this->lastpak);
        this->exceptseqnum = (this->exceptseqnum + 1) % MAX_SEQNUM;
    }
    else
    {
        pns->sendToNetworkLayer(SENDER, this->lastpak);
    }
    
}