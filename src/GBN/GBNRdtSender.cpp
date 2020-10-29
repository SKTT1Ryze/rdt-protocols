// GBNRdtSender Implementation
// 

#include "Global.h"
#include "GBNRdtSender.h"

GBNRdtSender::GBNRdtSender():win_len(4), base(0), nextseqnum(0) {
    for(int i = 0; i < MAX_SEQNUM; i++) {
        packetsWaitingAck[i] = new Packet();
    }
}

GBNRdtSender::~GBNRdtSender() {
    for(int i = 0; i < MAX_SEQNUM; i++) {
        delete packetsWaitingAck[i];
    }
}

bool GBNRdtSender::getWaitingState() {
    if(this->nextseqnum < this->base + this->win_len) return false;
    cout << "[Sender]refuse message." << endl;
    return true;
}

bool GBNRdtSender::send(const Message &message) {
    this->mkpkt(this->packetsWaitingAck[this->nextseqnum], message, this->nextseqnum);
        
    pUtils->printPacket("[Sender]send packet: ", *(this->packetsWaitingAck[this->nextseqnum]));

    pns->sendToNetworkLayer(RECEIVER, *(this->packetsWaitingAck[this->nextseqnum]));

    if(this->base == this->nextseqnum) pns->startTimer(SENDER, Configuration::TIME_OUT, this->packetsWaitingAck[this->base]->seqnum);
    nextseqnum = (nextseqnum + 1) % MAX_SEQNUM;
    return true;
}

void GBNRdtSender::receive(const Packet &ackPkt) {
    cout << "[Sender]receive ACK." << endl;
    int checksum = pUtils->calculateCheckSum(ackPkt);
    
    if(checksum == ackPkt.checksum) {
        int pre_base = this->base;
        this->base = (ackPkt.acknum + 1) % MAX_SEQNUM;
        if(this->base == this->nextseqnum) {
            cout << "[Sender]all packet in this group receive ACK." << endl;
            pns->stopTimer(SENDER, this->packetsWaitingAck[pre_base]->seqnum);
        } else
        {
            pns->stopTimer(SENDER, this->packetsWaitingAck[pre_base]->seqnum);
            pns->startTimer(SENDER, Configuration::TIME_OUT, this->packetsWaitingAck[this->base]->seqnum);
        }
        this->showWindow();
    }
}

void GBNRdtSender::timeoutHandler(int seqNum) {
    cout << "[sender]time out, resend." << endl;
    pns->stopTimer(SENDER, seqNum);
    pns->startTimer(SENDER, Configuration::TIME_OUT, seqNum);
    int i = this->base;
    while (i != this->nextseqnum)
    {
        pns->sendToNetworkLayer(RECEIVER, *(this->packetsWaitingAck[i]));
        i = (i + 1) % MAX_SEQNUM;
    }
}

bool GBNRdtSender::mkpkt(Packet *pak, const Message &message, int seqnum) {
    pak->seqnum = seqnum;
    pak->acknum = -1;
    pak->checksum = 0;
    memcpy(pak->payload, message.data, sizeof(message.data));
    pak->checksum = pUtils->calculateCheckSum(*pak);
    return true;
}

void GBNRdtSender::showWindow() {
    cout << "[Sender]Window: ";
    cout << "{ ";
    for(int i = 0; i < this->win_len; i++) {
        cout << this->packetsWaitingAck[(this->base + i) % MAX_SEQNUM]->seqnum << " ";
    }
    cout << "}" << endl;
}