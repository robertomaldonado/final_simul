File: traffic_generator 
Purpose: Generate random traffic for testing 
Usage: ./traffic_generator num_node pkt_size offered_load num_pkts_per_node [seed]
Output: traffic_file

To compile: 
make [all]
output: executable files for the simulation with the compiled class
    traffic_generator
    aloha
    slotted_aloha

To generate the traffic file use the executable:
    ./traffic_generator num_node pkt_size offered_load num_pkts_per_node [seed]
Output: 
    traffic_file 

To run the aloha protocol: ./aloha < input_file_name i.e. (./aloha < traffic_file)
To run the slotted aloha protocol: ./slotted_aloha

Note: The make file has independent tasks as well, if you desire you can use: 
    make [executable]