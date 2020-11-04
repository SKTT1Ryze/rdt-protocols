#ifndef TCP_RDT_RECEIVER_H
#define TCP_RDT_RECEIVER_H
#include "RdtReceiver.h"

class TCPRdtReceiver :public RdtReceiver
{
private:
	int expect_seqNum;
	vector<Packet *> packets;
public:
	TCPRdtReceiver();
	virtual ~TCPRdtReceiver();

public:
	void receive(const Packet &packet);
};

#endif