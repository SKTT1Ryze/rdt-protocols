#ifndef GBN_RDT_SENDER_H
#define GBN_RDT_SENDER_H
#include "RdtSender.h"


class GBNRdtSender :public RdtSender
{
private:
	int win_len;
	int base;
	int nextseqnum;
	Packet* packetsWaitingAck[MAX_SEQNUM];
public:
	bool getWaitingState();
	bool send(const Message &message);
	void receive(const Packet &ackPkt);
	void timeoutHandler(int seqNum);
	bool mkpkt(Packet *pak, const Message &message, int seqnum);
	void showWindow();
public:
	GBNRdtSender();
	virtual ~GBNRdtSender();
};

#endif