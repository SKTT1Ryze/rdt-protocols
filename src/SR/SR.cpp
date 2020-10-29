// SR Protocol Implementation
// 

#include "Global.h"
#include "RdtSender.h"
#include "RdtReceiver.h"
#include "SRRdtSender.h"
#include "SRRdtReceiver.h"

int main(int argc, char* argv[])
{
	SRRdtSender *ps = new SRRdtSender();
	SRRdtReceiver *pr = new SRRdtReceiver();
	pns->setRunMode(0);  //VERBOS mode
//	pns->setRunMode(1);  //silent mode
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