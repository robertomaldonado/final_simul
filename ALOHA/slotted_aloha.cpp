#include<iostream>
#include<vector>
#include<algorithm>

using namespace std;

class Packet{
public: 
    int id, src, dest, pkt_size, r_time;
public: 
	Packet(){
		this->id = 0;
		this->src = 0;
		this->dest = 0;
		this->pkt_size = 0;
		this->r_time= 0;
	}
	Packet(int id, int src, int dest, int pkt_size,int ready_time){
		this->id = id;
		this->src = src;
		this->dest = dest;
		this->pkt_size = pkt_size;
		this->r_time = ready_time;
	}
};

vector<Packet> myPackets;

bool compare(const Packet & l, const Packet & r){
	return l.r_time < r.r_time;
}


//Function to read in from stdin the values of the traffic file
void read_stdin(){
	int num_packets = 0;
	cin >> num_packets;

	int tempID, tempSrc, tempDest, tempPktSz, tempRTime;
	for(int i = 0; i < num_packets; i++){
		cin >> tempID;
		cin >> tempSrc;
		cin >> tempDest;
		cin >> tempPktSz;
		cin >> tempRTime;

		Packet tempPacket(tempID, tempSrc, tempDest, tempPktSz, tempRTime);
		myPackets.push_back(tempPacket);
	}
	sort(myPackets.begin(), myPackets.end(), compare);
}

void slotted_aloha(){
	int num_successful = 0;
	int num_unsuccessful = 0;
	bool wasCollision = false;
	bool incoming = true;
	vector<Packet> slot_queue;
	auto itr = myPackets.begin();
	int window_size = itr->pkt_size;
	int window_start = 0;

	while(incoming){
		bool inWindow = true;
		while(inWindow){
			if(itr->r_time < window_start){
				slot_queue.push_back(*itr); //Push to queue
				itr++; //Move one
				if(itr == myPackets.end()){//Check end
					incoming = false;
					inWindow = false;
				}
			}else{
				inWindow = false; //Not in window
			}
		}

		if(slot_queue.size() == 0){ //Empty Slot!"
			//cout << "Empty Slot!" << endl;
			//num_unsuccessful++;
		}else if(slot_queue.size() == 1){

			cout << "Time: " << slot_queue.front().r_time;
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
		}else if(slot_queue.size() >= 2){

			for(auto itrQueue = slot_queue.begin();;itrQueue++){

				cout << "Time: " << window_start + window_size - 1
			     << ", Packet " << itrQueue->id
			     << ": " << itrQueue->src
			     << " " << itrQueue->dest
			     << " " << itrQueue->pkt_size
			     << " " << itrQueue->r_time
				 << " finish sending: failed" << endl;
				 if(itrQueue == slot_queue.end())
				 	break;

			}
			num_unsuccessful += slot_queue.size();
			// slot_queue.clear();
		}
		slot_queue.clear();
		window_start += window_size;
	}

	cout << endl << num_successful <<" packets successfully transmitted."  << endl; 
	    cout <<  num_unsuccessful << " packets had colissions." <<endl; 
	    cout <<  "The throughput is: " 
	    << (((double)num_successful * (double)window_size * 1000.0) / ((double)window_start)) 
	    <<" kbps"<< endl;
	
}

int main(void){
	read_stdin();
	slotted_aloha();

	return 0;
}
