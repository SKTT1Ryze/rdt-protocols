// SRRdtSender Implementation

#include "Global.h"
#include "SRRdtSender.h"

SRRdtSender::SRRdtSender() {

}

SRRdtSender::~SRRdtSender() {

}

bool SRRdtSender::getWaitingState() {

}

bool SRRdtSender::send(const Message &message) {

}

void SRRdtSender::receive(const Packet &ackPkt) {

}

void SRRdtSender::timeoutHandler(int seqNum) {

}

bool SRRdtSender::mkpkt(Packet *pak, const Message &message, int seqnum) {
    pak->seqnum = seqnum;
    pak->acknum = -1;
    pak->checksum = 0;
    memcpy(pak->payload, message.data, sizeof(message.data));
    pak->checksum = pUtils->calculateCheckSum(*pak);
    return true;
}