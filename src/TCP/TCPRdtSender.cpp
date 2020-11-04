// TCPRdtSender Implementation

#include "Global.h"
#include "TCPRdtSender.h"

TCPRdtSender::TCPRdtSender():win_len(4), base(0), nextseqnum(0), pre_ack(0), ack_count(0) {
    this->packets = vector<Packet *> {};
}

TCPRdtSender::~TCPRdtSender() {
    for(int i = 0; i < this->packets.size(); i++) {
        delete this->packets[i];
    }
    this->packets.clear();
}

bool TCPRdtSender::getWaitingState() {
    if(this->packets.size() < this->win_len) {
        return false;
    }
    cout << "[Sender]refuse message." << endl;
    return true;
}

bool TCPRdtSender::send(const Message &message) {
    Packet *new_packet = new Packet();
    this->mkpkt(new_packet, message, this->nextseqnum);
    this->packets.push_back(new_packet);
    pUtils->printPacket("[Sender]send packet", *new_packet);
    pns->sendToNetworkLayer(RECEIVER, *new_packet);
    pns->startTimer(SENDER, Configuration::TIME_OUT, new_packet->seqnum);
    this->nextseqnum += sizeof(new_packet->payload);
    cout << "[Sender]packets size: " << this->packets.size() << endl;
    return true;
}

void TCPRdtSender::receive(const Packet &ackPkt) {
    int checksum = pUtils->calculateCheckSum(ackPkt);
    if(ackPkt.checksum == checksum) {
        pUtils->printPacket("[Sender]receive correct ack", ackPkt);
        if(ackPkt.acknum > this->base) {
            this->base = ackPkt.acknum;
            vector<int> remove_list = vector<int> {};
            for(int i = 0; i < this->packets.size(); i++) {
                if(this->packets[i]->seqnum < ackPkt.acknum) {
                    pns->stopTimer(SENDER, this->packets[i]->seqnum);
                    remove_list.push_back(i);
                } else {
                    pns->stopTimer(SENDER, this->packets[i]->seqnum);
                    pns->startTimer(SENDER, Configuration::TIME_OUT, this->packets[i]->seqnum);
                }
            }
            for(int i = 0; i < remove_list.size(); i++) {
                pUtils->printPacket("[Sender]erase: ", *this->packets[remove_list[i]]);
                this->packets.erase(this->packets.begin() + remove_list[i]);
                for(int j = i + 1; j < remove_list.size(); j ++) {
                    remove_list[j] -= 1;
                }
            }
        }
        cout << "[Sender]packets size: " << this->packets.size() << endl;
        cout << "[Sender]window: { ";
        for(int i = 0; i < this->packets.size(); i++) {
            cout << this->packets[i]->seqnum << " ";
        }
        cout << "}" << endl;
    } else {
        pUtils->printPacket("[Sender]receive wrong ack: ", ackPkt);
        if(ackPkt.acknum == this->pre_ack) {
            this->ack_count++;
        } else
        {
            this->pre_ack = ackPkt.acknum;
            this->ack_count = 0;
        }
        if(this->ack_count >= 2) {
            for(int i = 0; i < this->packets.size(); i++) {
                if(this->packets[i]->seqnum == ackPkt.acknum) {
                    pUtils->printPacket("[Sender]fast resend: ", *this->packets[i]);
                    pns->sendToNetworkLayer(RECEIVER, *this->packets[i]);
                    // pns->stopTimer(SENDER, this->packets[i]->seqnum);
                    // pns->startTimer(SENDER, Configuration::TIME_OUT, this->packets[i]->seqnum);
                    break;
                }
            }
        }
        // pns->stopTimer(SENDER, ackPkt.acknum);
        // pns->startTimer(SENDER, Configuration::TIME_OUT, ackPkt.acknum);
    }
}

void TCPRdtSender::timeoutHandler(int seqNum) {
    if(this->packets.empty()) {
        // do nothing
    } else {
        pns->sendToNetworkLayer(RECEIVER, *this->packets[0]);
        pns->stopTimer(SENDER, this->packets[0]->seqnum);
        pns->startTimer(SENDER, Configuration::TIME_OUT, this->packets[0]->seqnum);
    }
}

bool TCPRdtSender::mkpkt(Packet *pak, const Message &message, int seqnum) {
    pak->seqnum = seqnum;
    pak->acknum = -1;
    pak->checksum = 0;
    memcpy(pak->payload, message.data, sizeof(message.data));
    pak->checksum = pUtils->calculateCheckSum(*pak);
    return true;
}