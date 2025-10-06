#ifndef ANOMALIES_H
#define ANOMALIES_H

#include "Generateur_Donnees.h"


void icmp_flood (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF);
void port_scan (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF);
void rstflood (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF);
void asw_no_rqt (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF);
void rqt_no_asw (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF);
struct Packet* get_anomalies_array (struct Slave* S, struct Master_Slaves_Flow* MSF);
void get_packet_array_attackers (struct Slave* S, struct Master_Slaves_Flow* MSF, char s_o_a);


#endif