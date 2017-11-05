#include <iostream>
#include <vector>
#include<fstream>
#include <cmath>

using namespace std;

class Packet{
public: 
    unsigned long id, src, dest, pkt_size, r_time;
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
        this->r_time= ready_time;
    }
};

void readData(vector<Packet>& my_pkts){
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
		my_pkts.push_back(tempPacket);
    }
}

string printPacket(Packet pkt){
    
    char buffer [150];
    sprintf(buffer, "%lu %lu %lu %lu %lu", pkt.id, pkt.src, pkt.dest, pkt.pkt_size, pkt.r_time);
    string str = buffer;
    return str;
}
    
int main(){

    vector<bool> bools;
    vector<int> vect;
    vector<Packet> pkts_v;
    ofstream myfile;

    int finish_time=0;
    int begin_time=0; 
    int next_begin_time  = 0;
    cout << "Reading data from file..." << endl;
    readData(pkts_v);
    cout << "Done loading packets..." << endl;    

    for(int i=0; i<pkts_v.size(); i++){
        bools.push_back(1);
    }
    //Open output file
    myfile.open("aloha_output.txt"); 
    cout << "Processing packets..." << endl;    

    for (int j=0; j < pkts_v.size()-1; j++){
        begin_time = pkts_v[j].r_time;
        // Time: 1, Packet 1: 1 4 100 1 start sending
        if(bools[j] != 0)
        myfile << "Time: " << begin_time << " Packet " 
             << printPacket(pkts_v[j]).c_str()<< " start sending" << endl; 

        finish_time = begin_time + pkts_v[j].pkt_size;
        next_begin_time = pkts_v[j+1].r_time;
        if(finish_time > next_begin_time){ //Collision
            // myfile << ": collision" << endl;
            myfile << "Time: " << finish_time << " Packet " 
            << printPacket(pkts_v[j]).c_str() << " start sending: collision" << endl;
            myfile << "Time: " << next_begin_time << " Packet " 
                 << printPacket(pkts_v[j+1]).c_str() << " start sending: collision" << endl;
            bools[j] = 0;                
            bools[j+1] = 0;
        }else{
            if(bools[j] != 0)
            myfile << "Time: " << finish_time << " Packet " 
            << printPacket(pkts_v[j]).c_str() << " finish sending: successfully transmitted" << endl;
        }
    }
    //Last Packet
    begin_time = pkts_v[pkts_v.size()-2].r_time;
    finish_time = begin_time + pkts_v[pkts_v.size()-2].pkt_size;
    next_begin_time = pkts_v[pkts_v.size()-1].r_time;
    
    if(finish_time < next_begin_time){ //Collision
        myfile << "Time: " << begin_time << " Packet " 
        << printPacket(pkts_v[pkts_v.size()-1]).c_str()<< " start sending" << endl; 
        myfile << "Time: " << finish_time << " Packet " 
        << printPacket(pkts_v[pkts_v.size()-1]).c_str() << " finish sending: successfully transmitted" << endl;
    }else{
        myfile << "Time: " << next_begin_time << " Packet " 
        << printPacket(pkts_v[pkts_v.size()-1]).c_str() << " finish sending: failed" << endl;
    }


    int sum=0;
    for(int i=0; i<bools.size();i++){
        sum += bools[i];
    }
    myfile << endl << sum <<" packets successfully transmitted."  << endl; 
    myfile <<  bools.size()-sum << " packets had colissions." <<endl;

    double tpt = (sum * pkts_v[pkts_v.size()-1].pkt_size) / (double) ( pkts_v[pkts_v.size()-1].r_time + pkts_v[pkts_v.size()-1].pkt_size );
    myfile << "The global throughput is "<< tpt*1000 << " kbps."  <<endl;  
    myfile.close();   
    cout << "Done. Output in file: aloha_output.txt" << endl;    
    
    return 0;
}