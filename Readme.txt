CONTENTS: 
FOLDERS:
    ALOHA 
    CSMA
FILES:
    Makefile
    Readme.txt

To compile: 
    make all
To clean:
    make clean

Each folder has a traffic file generator, used by the command (with options):
    ./traffic_generator num_node pkt_size offered_load num_pkts_per_node [seed]
Outputs a traffic_file in the same directory

To run the aloha protocol (In the ALOHA folder): ./aloha < input_file_name i.e. (./aloha < traffic_file)
Outputs a file called "aloha_output" in the same directory

To run the slotted aloha protocol (In the ALOHA folder): ./slotted_aloha < input_file_name >> slotted_output
Outputs a file called "slotted_output" in the same directory

To run the CSMA 1-Persistent protocol (In the CSMA folder): ./persist < input_file_name >> csma_output
Outputs a file called "csma_output" in the same directory

Note: The make file has independent tasks as well, if you desire you can use: 
    make [executable] i.e.: make aloha