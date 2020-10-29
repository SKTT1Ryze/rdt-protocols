#ifndef SR_RDT_SENDER_H
#define SR_RDT_SENDER_H
#include "RdtSender.h"


class SRRdtSender :public RdtSender
{
private:
	int win_len;
	int base;
	int nextseqnum;
	int packetsACK[MAX_SEQNUM];
	Packet* packetsQueue[MAX_SEQNUM];
public:
	bool getWaitingState();
	bool send(const Message &message);
	void receive(const Packet &ackPkt);
	void timeoutHandler(int seqNum);
	bool mkpkt(Packet *pak, const Message &message, int seqnum);
public:
	SRRdtSender();
	virtual ~SRRdtSender();
};

#endif