// Simple TCP Protocol Implementation
// 

#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "TCPRdtSender.h"
#include "TCPRdtReceiver.h"

int main(int argc, char* argv[])
{
    TCPRdtSender *ps = new TCPRdtSender();
	TCPRdtReceiver *pr = new TCPRdtReceiver();
	pns->setRunMode(0);  //VERBOS mode
	// pns->setRunMode(1);  //silent mode
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr);
	pns->setInputFile("/home/hustccc/rdt-protocols/input.txt");
	pns->setOutputFile("/home/hustccc/rdt-protocols/output.txt");

	pns->start();

	delete ps;
	delete pr;
	delete pUtils;
	delete pns;
	
	return 0;
}