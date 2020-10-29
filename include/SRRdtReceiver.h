#ifndef SR_RDT_RECEIVER_H
#define SR_RDT_RECEIVER_H
#include "RdtReceiver.h"
class SRRdtReceiver :public RdtReceiver
{
private:
public:
	SRRdtReceiver();
	virtual ~SRRdtReceiver();

public:
	void receive(const Packet &packet);
};

#endif