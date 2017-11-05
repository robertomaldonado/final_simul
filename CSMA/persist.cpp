#include <iostream>
#include <vector>

using namespace std;

class Packet{
public: 
	unsigned long id, src, dest, pkt_size, r_time, p_time;
public: 
	Packet(){
		this->id = 0;
		this->src = 0;
		this->dest = 0;
		this->pkt_size = 0;
		this->r_time= 0;
	}
	Packet(long id, long src, long dest, long pkt_size,long ready_time){
		this->id = id;
		this->src = src;
		this->dest = dest;
		this->pkt_size = pkt_size;
		this->r_time = ready_time;
		this->p_time = ready_time;
	}
};

void readData(vector<Packet>& my_pkts){
    int num_packets = 0;
	cin >> num_packets;

	unsigned long tempID, tempSrc, tempDest, tempPktSz, tempRTime;
	for(int i = 0; i < num_packets; i++){
		cin >> tempID;
 		cin >> tempSrc;
		cin >> tempDest;
		cin >> tempPktSz;
		cin >> tempRTime;
			
		Packet tempPacket(tempID, tempSrc, tempDest, tempPktSz, tempRTime);
		my_pkts.push_back(tempPacket);
    }
};

int main(){
    vector<Packet> data;
    vector<Packet> queued_data;

    // int data[0].pkt_size;
    bool is_free = 1;
    readData(data);

    int sent = 0;
    int colls = 0;    
    for(int tmp_time = 1, i=0; tmp_time < data.size()-2 ; tmp_time++, i++){

        if( tmp_time < data[i].r_time && is_free ){ //Channel free and timming is good

            if(queued_data.size() > 1){ //Send the packetS on queue
                for(int j=0; j< queued_data.size(); j++){ //Collision is guaranteed
                    // cout << "Collision for ids: " << queued_data[i].id << endl ; 
                    colls++;                   
                }
                queued_data.clear(); //Clear the queue
            }else{ //Sent if there exists only one packet on queue
                is_free=0; //Channel being used
                sent++; //Sent that framE
            }
            // cout << "Sending from station" << data[i].id << endl;
            is_free=0; //Channel being used
            sent++;        

        }else{ //if not free add to queue. Edit time.
            // cout<< "Channel busy" << endl;
            // data[i].r_time = data[i-1].r_time + data[i-1].pkt_size ;
            queued_data.push_back(data[i]);
            is_free=0;
            // cout <<"On queue: " << queued_data.size() << endl;
        }

        if( data[i].r_time + data[i].pkt_size < data[i+1].r_time ){
            is_free=1;
        }
        
    }

    double tpt = ((double)(data.size()-colls) * data[data.size()-1].pkt_size) / (double)(data[data.size()-1].r_time + data[data.size()-1].pkt_size);
    cout << endl << data.size()-colls <<" packets successfully transmitted."  << endl; 
	cout << colls << " packets had colissions." << endl;	
	cout <<  "The throughput is: " << tpt*1000 << " kbps." << endl;

    return 0;
}