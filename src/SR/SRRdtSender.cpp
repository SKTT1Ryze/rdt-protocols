// SRRdtSender Implementation

#include "Global.h"
#include "SRRdtSender.h"

SRRdtSender::SRRdtSender():win_len(4), base(0), nextseqnum(0) {
    for(int i = 0; i < MAX_SEQNUM; i++) {
        packetsQueue[i] = new Packet();
        packetsACK[i] = 0;
    }
}

SRRdtSender::~SRRdtSender() {
    for(int i = 0; i < MAX_SEQNUM; i++) {
        delete packetsQueue[i];
        packetsACK[i] = 0;
    }
}

bool SRRdtSender::getWaitingState() {
    // if((this->nextseqnum + MAX_SEQNUM - this->base) % MAX_SEQNUM < this->win_len) return false;
    if((this->nextseqnum - this->base) < this->win_len) return false;
    cout << "[Sender]refuse message." << endl;
    return true;
}

bool SRRdtSender::send(const Message &message) {
    this->mkpkt(this->packetsQueue[this->nextseqnum % MAX_SEQNUM], message, this->nextseqnum);
    
    this->packetsACK[this->nextseqnum % MAX_SEQNUM] = 0; 
    pUtils->printPacket("[Sender]send packet: ", *(this->packetsQueue[this->nextseqnum % MAX_SEQNUM]));
    pns->sendToNetworkLayer(RECEIVER, *(this->packetsQueue[this->nextseqnum % MAX_SEQNUM]));
    pns->startTimer(SENDER, Configuration::TIME_OUT, this->packetsQueue[this->nextseqnum % MAX_SEQNUM]->seqnum);
    
    // nextseqnum = (nextseqnum + 1) % MAX_SEQNUM;
    this->nextseqnum++;
    return true;
}

void SRRdtSender::receive(const Packet &ackPkt) {
    cout<< "[Sender]receive ACK: " << ackPkt.acknum << endl;
    int checksum = pUtils->calculateCheckSum(ackPkt);
    if(checksum == ackPkt.checksum) {
        cout << "[Sender]check ACK pass" << endl;
        if(ackPkt.acknum >= this->base && ackPkt.acknum <= (this->base + this->win_len)) {
            this->packetsACK[ackPkt.acknum % MAX_SEQNUM] = 1;
            pns->stopTimer(SENDER, ackPkt.acknum);
            if(ackPkt.acknum == this->base) {
                for(int i = this->base % MAX_SEQNUM; ;i = (i + 1) % MAX_SEQNUM, this->base++) {
                    if(this->packetsACK[i] == 0) {
                        // this->base = i;
                        break;
                    }
                    this->packetsACK[i] = 0;
                }
            }
        }  
    }
    // else {
    //     pns->stopTimer(SENDER, ackPkt.acknum);
    //     pns->startTimer(SENDER, Configuration::TIME_OUT, ackPkt.acknum);
    // }
    cout << "[Sender]window: ";
    for(int i = this->base; i < this->nextseqnum; i++ ) {
        cout << this->packetsQueue[i % MAX_SEQNUM]->checksum << " ";
    }
    cout << endl;
}

void SRRdtSender::timeoutHandler(int seqNum) {
    cout << "[Sender]seqNum " << seqNum << " time out, resend." << endl;
    pns->sendToNetworkLayer(RECEIVER, *(this->packetsQueue[seqNum % MAX_SEQNUM]));
    pns->stopTimer(SENDER, seqNum);
    pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
}

bool SRRdtSender::mkpkt(Packet *pak, const Message &message, int seqnum) {
    pak->seqnum = seqnum;
    pak->acknum = -1;
    pak->checksum = 0;
    memcpy(pak->payload, message.data, sizeof(message.data));
    pak->checksum = pUtils->calculateCheckSum(*pak);
    return true;
}