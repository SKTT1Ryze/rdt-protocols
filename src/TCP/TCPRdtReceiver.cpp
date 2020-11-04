// TCPRdtReceiver Implementation

#include "Global.h"
#include "TCPRdtReceiver.h"

TCPRdtReceiver::TCPRdtReceiver():expect_seqNum(0) {
    this->packets = vector<Packet *> {};
}

TCPRdtReceiver::~TCPRdtReceiver() {
    for(int i = 0; i < this->packets.size(); i++) {
        delete this->packets[i];
    }
    this->packets.clear();
}

void TCPRdtReceiver::receive(const Packet &packet) {
    int checksum = pUtils->calculateCheckSum(packet);
    if(checksum == packet.checksum) {
        pUtils->printPacket("[Receiver]receive correct packet: ", packet);
        Packet *new_ack = new Packet();
        new_ack->seqnum = -1;
        for(int i = 0; i < Configuration::PAYLOAD_SIZE; i++) {
            new_ack->payload[i] = '.';
        }
        if(packet.seqnum == this->expect_seqNum) {
            this->expect_seqNum = packet.seqnum + sizeof(packet.payload);
            Message message;
            memcpy(message.data, packet.payload, sizeof(packet.payload));
            pns->delivertoAppLayer(RECEIVER, message);
            new_ack->acknum = this->expect_seqNum;
            new_ack->checksum = pUtils->calculateCheckSum(*new_ack);
            pUtils->printPacket("[Receiver]send correct ACK: ", *new_ack);
        } else {
            new_ack->acknum = this->expect_seqNum;
            new_ack->checksum = pUtils->calculateCheckSum(*new_ack) + 1;
            pUtils->printPacket("[Receiver]send wrong ACK: ", *new_ack);
        }
        pns->sendToNetworkLayer(SENDER, *(new_ack));
    }
}