#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<map>

using namespace std;

class Packet{
public: 
    unsigned long long id, src, dest, pkt_size, r_time;
public:
	Packet(){
		this->id = 0;
		this->src = 0;
		this->dest = 0;
		this->pkt_size = 0;
		this->r_time= 0;
	}
	Packet(unsigned long long id, int src, int dest, int pkt_size,unsigned long long ready_time){
		this->id = id;
		this->src = src;
		this->dest = dest;
		this->pkt_size = pkt_size;
		this->r_time = ready_time;
	}
};

vector<Packet> myPackets;

bool comparer(const Packet & l, const Packet & r){
	return l.r_time < r.r_time;
}
class Compare{
public:
        bool operator()(Packet l,Packet r){
                return l.r_time > r.r_time;
        }
};

//Function to read in from stdin the values of the traffic file
void read_stdin(){
	int num_packets = 0;
	cin >> num_packets;

	unsigned long long tempID, tempSrc, tempDest, tempPktSz, tempRTime;
	for(int i = 0; i < num_packets; i++){
		cin >> tempID;
		cin >> tempSrc;
		cin >> tempDest;
		cin >> tempPktSz;
		cin >> tempRTime;

		Packet tempPacket(tempID, tempSrc, tempDest, tempPktSz, tempRTime);
		myPackets.push_back(tempPacket);
	}
	sort(myPackets.begin(), myPackets.end(), comparer);
//	for(auto x : myPackets)
//		cout << x.r_time << endl;
}

void slotted_aloha(){
	int num_successful = 0;
	int num_unsuccessful = 0;
	bool wasCollision = false;
	bool incoming = true;
	vector<Packet> slot_queue;
	auto itr = myPackets.begin();
	int window_size = itr->pkt_size;
        int pkt_size = myPackets.front().pkt_size;      //Size of the packet in bits, known to be constant for all packets
        unsigned long long window_opening = 0; //Time at which packet(s) will finish sending and the channel will be free
	unsigned long long current_time = 0;
        //This map holds the waiting queue of packets for each transmitting node
        //The waiting queue is a priority queue where all the packets are ordered 
        //such that the top element is the packet ready to transmit first for that node
        map<int,priority_queue<Packet,vector<Packet>,Compare> > node_wq;

	for(auto itr = myPackets.begin(); itr != myPackets.end(); itr++)
        	node_wq[itr->src].push(*itr);

        unsigned long long wnum = 0;
        for(auto itr = node_wq.begin(); itr != node_wq.end(); itr++){
                wnum += itr->second.size();
	}

	while(wnum > 0){
                //count the number of packets ready to send at the current open time window
                int rnum = 0;
                for(auto itr = node_wq.begin(); itr != node_wq.end(); itr++)
			if(!itr->second.empty())
                        	if(itr->second.top().r_time < window_opening)
                                	rnum++;

                //If the number of stations ready to send packets is zero
                //then we advance the window by one microsecond
                if(rnum == 0)
                        window_opening = current_time + window_size;
                //If one station is ready to send a packet, send the packet. This is a success
                //because on the next time step other nodes will sense that the channel is busy
                //And will wait for an open window
                else if(rnum == 1){
                        //Find the node that is ready to send, send (pop) the packet
                        for(auto itr = node_wq.begin(); itr != node_wq.end(); itr++){
				if(!itr->second.empty())
	                                if(itr->second.top().r_time < window_opening){
	                                        itr->second.pop();
	                                        break;
        	                        }
                        }
                        //Increment number of successfuul transmissions
                        num_successful++;
                        //Advance window to the when the channel will be idle
                        //Transmission is 1Mbps and we are in the microsecond time domain,
                        //hence why we are able to treat packet size as the time it takes to
                        //transmit the packet. Hooray for natural units!
                        window_opening = current_time + window_size;
                }
                //If multiple station are ready to transmit when the channel becomes idel
                else{
                        for(auto itr = node_wq.begin(); itr != node_wq.end(); itr++){
				if(!itr->second.empty())
	                                if(itr->second.top().r_time < window_opening){
	                                        //////////////////////////////////////////////////////////////////////////
	                                        //Commenting out this line of code within this block will make it such that 
	                                        //if a packet transmission is interupted then a station will NOT attempt to 
	                                        //retransmit it.
	                                        itr->second.pop();
	                                        //////////////////////////////////////////////////////////////////////////
	
	                                        //Give a random time to wait for the station to send the next packet
	                                        //This section is strange because the behaviour of the protocol is highly dependent
	                                        //on the maximum time that a station is allowed to wait
	                                        //itr->second.top().r_time = current_time + (rand()%(2*pkt_size)) + pkt_size;
	                                }
                        }
                        //Advance the window to the idle state
                        window_opening = current_time + window_size;                       
                }

                //Count the number of packets which still need to be sent
                wnum = 0;
                for(auto itr = node_wq.begin(); itr != node_wq.end(); itr++){
                        wnum += itr->second.size();
		}

                //Take us to the idle state!
                current_time = window_opening;
        }

/*	while(incoming){
		bool inWindow = true;
		while(inWindow){
			if(itr->r_time <= window_start){
				slot_queue.push_back(*itr);
				itr++;
				if(itr == myPackets.end()){
					incoming = false;
					inWindow = false;
				}
			}
			else{
				inWindow = false;
			}
		}

		if(slot_queue.size() == 0){
			//cout << "Empty Slot!" << endl;
			//num_unsuccessful++;
		}
		else if(slot_queue.size() == 1){
//			cout << "Success!" << endl;
			//cout << "Time: " << slot_queue.front().r_time
			cout << "Time: " << window_start
				<< ", Packet " << slot_queue.front().id
				<< ": " << slot_queue.front().src
				<< " " << slot_queue.front().dest
				<< " " << slot_queue.front().pkt_size
				<< " " << slot_queue.front().r_time
				<< " start sending" << endl;
			cout << "Time: " << window_start + window_size - 1
				<< ", Packet " << slot_queue.front().id
				<< ": " << slot_queue.front().src
				<< " " << slot_queue.front().dest
				<< " " << slot_queue.front().pkt_size
				<< " " << slot_queue.front().r_time
				<< " finish sending: successfully transmitted" << endl;
			num_successful++;
		}
//    int id, src, dest, pkt_size, r_time;
		else{
//			cout << "No Success!" << endl;	
			num_unsuccessful += slot_queue.size();
		}

		slot_queue.clear();
		window_start += window_size;
	}
*/
//	cout << num_successful << endl;
//	cout << num_unsuccessful << endl;
//	cout << myPackets.size() << endl;
	cout << (int)(((double)num_successful * (double)window_size * 1000.0) / ((double)current_time)) << endl;
}

int main(void){
	read_stdin();
	slotted_aloha();
//	p_persistent(1);	

	return 0;
}
