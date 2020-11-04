#ifndef TCP_RDT_SENDER_H
#define TCP_RDT_SENDER_H
#include "RdtSender.h"

class TCPRdtSender :public RdtSender
{
private:
	int win_len;
	int base;
	int nextseqnum;
	int pre_ack;
	int ack_count;
	int timer_run;
	vector<Packet*> packets;
public:
	bool getWaitingState();
	bool send(const Message &message);
	void receive(const Packet &ackPkt);
	void timeoutHandler(int seqNum);
	bool mkpkt(Packet *pak, const Message &message, int seqnum);
public:
	TCPRdtSender();
	virtual ~TCPRdtSender();
};

#endif