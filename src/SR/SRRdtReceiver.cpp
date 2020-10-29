// SRRdtReceiver Implementation

#include "Global.h"
#include "SRRdtReceiver.h"

SRRdtReceiver::SRRdtReceiver():win_len(4), base(0) {
    for(int i = 0; i < MAX_SEQNUM; i++) {
        packetsQueue[i] = new Packet();
        packetsRecv[i] = 0;
    }
}

SRRdtReceiver::~SRRdtReceiver() {
    for(int i = 0; i < MAX_SEQNUM; i++) {
        delete packetsQueue[i];
        packetsRecv[i] = 0;
    }
}

void SRRdtReceiver::receive(const Packet &packet) {
    int checksum = pUtils->calculateCheckSum(packet);
    if(checksum == packet.checksum) {
        cout << "[Receiver]packet.sequm: " << packet.seqnum << ", base: " << this->base << endl;
        if((packet.seqnum >= this->base) && (packet.seqnum <= (this->base + this->win_len - 1))) {
            pUtils->printPacket("[Receiver]receive correct packet: ", packet);
            this->packetsRecv[packet.seqnum % MAX_SEQNUM] = 1;
            this->packetsQueue[packet.seqnum % MAX_SEQNUM]->seqnum = packet.seqnum;
            this->packetsQueue[packet.seqnum % MAX_SEQNUM]->acknum = packet.seqnum;
            this->packetsQueue[packet.seqnum % MAX_SEQNUM]->checksum = packet.checksum;
            for(int i = 0; i < Configuration::PAYLOAD_SIZE; i++) {
                this->packetsQueue[packet.seqnum % MAX_SEQNUM]->payload[i] = packet.payload[i];
            }
            // memcpy(this->packetsQueue[packet.seqnum % MAX_SEQNUM]->payload, packet.payload, sizeof(packet.payload));
            pUtils->printPacket("[Receiver]send ACK: ", *(this->packetsQueue[packet.seqnum % MAX_SEQNUM]));
            pns->sendToNetworkLayer(SENDER, *(this->packetsQueue[packet.seqnum % MAX_SEQNUM]));

            if(packet.seqnum == this->base) {
                for(int i = packet.seqnum % MAX_SEQNUM; ; i = (i + 1) % MAX_SEQNUM) {
                    if(this->packetsRecv[i] == 1) {
                        Message message;
                        memcpy(message.data, this->packetsQueue[i]->payload, sizeof(this->packetsQueue[i]->payload));
                        pns->delivertoAppLayer(RECEIVER, message);
                        this->packetsRecv[i] = 0;
                        this->base++;
                    }
                    else
                    {
                        break;
                    }
                    
                }
                
            }
        }
        else if((packet.seqnum >= (this->base - this->win_len)) && (packet.seqnum <= (this->base - 1))) {
            pUtils->printPacket("[Receiver]receive correct packet: ", packet);
            Packet *ack_pak = new Packet();
            ack_pak->acknum = packet.seqnum;
            for(int i = 0; i < Configuration::PAYLOAD_SIZE; i++) {
                ack_pak->payload[i] = '.';
            }
            ack_pak->checksum = pUtils->calculateCheckSum(*ack_pak);
            pUtils->printPacket("[Receiver]send ACK: ", *ack_pak);
            pns->sendToNetworkLayer(SENDER, *ack_pak);
        }
    }
}