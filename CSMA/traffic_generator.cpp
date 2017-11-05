#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <map>

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

string printPacket(Packet pkt){

    char buffer [250];
    sprintf(buffer, "%lu %lu %lu %lu %lu\n", pkt.id, pkt.src, pkt.dest, pkt.pkt_size, pkt.r_time);
    string str = buffer;
    return str;
}

int main(int argc, char* argv[]){

    if(argc < 5 || argc > 6){ //Validate inline call with parameters
        cerr<<"Usage: ./traffic_generator num_node pkt_size offered_load num_pkts_per_node [seed]"<<endl;
        return 0;
    }else if(atof(argv[3]) == 0){
        cerr<< "Offered load must be a positive number between 0.01 and 10, excluding 0." << endl;
        cerr<<"Usage: ./traffic_generator num_node pkt_size offered_load num_pkts_per_node [seed]"<<endl;
        return 0;
    }else{ 
        //Containers to be used
        vector<Packet> pkts_v; 
        multimap<int,Packet> vector_map;

        int num_node = atoi(argv[1]);
        int pkt_size = atoi(argv[2]);
        double offered_load = atof(argv[3]); 
        int num_pkts_per_node = atoi(argv[4]); 
        int seed = rand()%10;  

        if(argc == 6 )
            seed = atof(argv[5]);

        srand(seed); //Set Seed

        int gap = ((pkt_size * num_node) / (offered_load)) - pkt_size;
        int dest;  
        int counter=0;
        cout << "Generating traffic...." <<endl;
        for(int i=0; i< num_node ; i++){     
            int time_to_send = 0; 
            for( int j=0;j<num_pkts_per_node;j++, counter++){  
                if(gap!=0){
                    time_to_send += (int) rand()%(2*gap);
                }
                dest = rand()%num_node;
                while(dest == i)
                    dest = rand()%num_node;
                Packet tmp_obj(counter,i,dest,pkt_size,time_to_send);   
                pkts_v.push_back(tmp_obj);
                time_to_send += pkt_size;
            }   
        }
        cout << "Moving data to map (sorting)...." <<endl;
        //Sort by the transfer from vector to the map
        for(int i=0; i<pkts_v.size(); i++){
            vector_map.insert(std::pair<int ,Packet>(pkts_v[i].r_time,pkts_v[i]));
        }

        cout << "Printing to file...." <<endl;    
        //Create file 
        ofstream myfile;
        myfile.open("traffic_file"); 
        myfile << pkts_v.size() << endl; 
        //Create Iterator
        map<int,Packet>::iterator it = vector_map.begin();
        //Print to file while iterating   
        for(;it != vector_map.end();it++){
          myfile << printPacket(it->second);
        }
        myfile.close();
        cout << "Done. Output in file: traffic_file" <<endl;    
        
	    return 0;
    }
}