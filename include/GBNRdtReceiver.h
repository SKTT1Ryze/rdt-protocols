#ifndef GBN_RDT_RECEIVER_H
#define GBN_RDT_RECEIVER_H
#include "RdtReceiver.h"
class GBNRdtReceiver :public RdtReceiver
{
private:
	int exceptseqnum;
	Packet lastpak;
public:
	GBNRdtReceiver();
	virtual ~GBNRdtReceiver();

public:
	void receive(const Packet &packet);
};

#endif

