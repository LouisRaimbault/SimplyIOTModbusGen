#ifndef GENERATEUR_DONNEES
#define GENERATEUR_DONNEES

#include <time.h>
#include <ostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

struct Standart_Size
{
    uint16_t size_udp;
    uint16_t size_tcp;
    uint16_t size_arp;
    uint16_t cu_icmp_inf;
    uint16_t cu_icmp_sup;
    uint16_t cu_ntp_inf;
    uint16_t cu_ntp_sup;
    uint16_t cu_snmp_inf;
    uint16_t cu_snmp_sup;
    uint16_t cu_modbus_inf;
    uint16_t cu_modbus_sup;
};

struct Timer_Size
{
    double delay_rqt;
    double delay_asw;
    uint32_t size_block;
    uint32_t size_charge_sup;
    uint32_t size_charge_less;
};

struct Packet
{
    double timer;        // 8
    char* ip_src;        //8
    char* ip_dst;        //8
    char* mac_src;       //8
    char* mac_dst;       //8
    char* concat;        //8
    char* protocol;      //8
    uint16_t pck_length; // 2
    uint16_t charge_utile;//2
    uint16_t port_src;   //2
    uint16_t port_dst;   //2
    uint16_t tra;        //2
    uint16_t direction;  //2
    uint16_t label;      //2

};

struct Master
{
    char* ip;
    char* mac;  
    uint16_t port_modbus;
    uint16_t port_snmp;
    uint16_t port_ntp;
};

struct Slave
{
    char* ip;
    char* mac; 
    double * starts_action; 
    double duration_action;
    struct Packet* tab_pck_safe;
    struct Packet* tab_pck_anomalies;
    struct Timer_Size ts_arp;
    struct Timer_Size ts_icmp;
    struct Timer_Size ts_ntp;
    struct Timer_Size ts_snmp;
    struct Timer_Size ts_modbus;
    uint32_t nb_pck_safe; 
    uint32_t nb_pck_anomalies;
    uint16_t port_modbus;
    uint16_t port_snmp;
    uint16_t port_ntp;
    uint8_t is_attacker;
    uint16_t nb_starts_action;
};

struct Master_Slaves_Flow
{
    double duration_scenario;
    double ratio_duration;
    struct Slave* tab_slaves;
    struct Slave* tab_attackers;
    struct Packet* tab_pck_safe;
    struct Packet* tab_pck_anomalies;
    struct Master M;
    uint32_t nb_pck_safe;
    uint32_t nb_pck_anomalies;
    uint16_t nb_slaves;
    uint16_t nb_attackers;
};

struct Modbus_Network
{
    char* path;
    double duration_scenario;
    double ratio_duration;
    struct Master_Slaves_Flow* tab_msf;
    uint16_t nb_msf;
};



void get_standart_size (struct Standart_Size* SS);
int randomAB_int(int min, int max) ;
double randomAB_double(double min, double max) ;
double elapsed_time(struct timespec start, struct timespec end) ;
char* create_random_ipv4_address() ;
char* create_random_mac_address() ;
void init_slave (struct Slave* S, struct Standart_Size* SS, struct Master_Slaves_Flow* MSF);
void sort_doubles(double* arr, int n) ;
void set_attackers_starts_action (struct Slave* S, struct Master_Slaves_Flow* MSF);
void init_MSF (struct Master_Slaves_Flow* MSF, struct Standart_Size* SS);
void export_packets(struct Packet* tab_pck, int nb_pck, const char* pathout, struct Modbus_Network* MN);
void Generateur_Donnnees_MSF(struct Master_Slaves_Flow* MSF, int i, struct Modbus_Network* MN);
struct Packet* replace_order_array (struct Packet* tab_1, struct Packet* tab_2, int nb1, int nb2);
void Generateur_Donnnees_MN(struct Modbus_Network* MN);


#endif