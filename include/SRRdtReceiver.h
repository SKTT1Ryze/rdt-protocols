#ifndef SR_RDT_RECEIVER_H
#define SR_RDT_RECEIVER_H
#include "RdtReceiver.h"
class SRRdtReceiver :public RdtReceiver
{
private:
	int base;
	int win_len;
	int packetsRecv[MAX_SEQNUM];
	Packet *packetsQueue[MAX_SEQNUM];

public:
	SRRdtReceiver();
	virtual ~SRRdtReceiver();

public:
	void receive(const Packet &packet);
};

#endif