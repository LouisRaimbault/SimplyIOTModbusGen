#ifndef TRAFFIC_CREATOR_H
#define TRAFFIC_CREATOR_H


struct Packet* get_packet_array (struct Slave* S, struct Timer_Size* TS, struct Master_Slaves_Flow* MSF);
void get_packet_array_slaves (struct Slave* S, struct Master_Slaves_Flow* MSF, char s_o_a);
void get_packet_array_MSF (struct Master_Slaves_Flow* MSF, char s_o_a);

#endif