#ifndef GET_AND_TRANSFORME_DATA_H 
#define GET_AND_TRANSFORME_DATA_H

#include "Generateur_Donnees.h"
#include <string>

struct Packet_1
{
    double ts_d;
    char* concat;        //8
    char* ip_src;        //8
    char* ip_dst;        //8
    char* mac_src;       //8
    char* mac_dst;       //8
    char* protocol;
    uint32_t symbole;    // 4
    uint16_t label;      //2
	char ts;        // 1
    char pck_length; // 1
    char charge_utile;//1
    char port_src;   //1
    char port_dst;   //1
    char tra;        //1
    char direction;  //1
};

struct Packet_0
{
	double timer;        // 8
    char* ip_src;        //8
    char* ip_dst;        //8
    char* mac_src;       //8
    char* mac_dst;       //8
    char* concat;        //8
    char* protocol;      // 8
    uint16_t pck_length; // 2
    uint16_t charge_utile;//2
    uint16_t port_src;   //2
    uint16_t port_dst;   //2
    uint16_t tra;        //2
    uint16_t direction;  //2
    uint16_t label;      //2

};

struct conversation_values
{
	
	double last_timer;
    double somme_timer;
    double avg_timer;
    int nb_pck;
	uint32_t somme_pck_length;
    uint32_t somme_charge_utile;
    uint32_t avg_charge_utile;
	uint16_t avg_pck_length;
	
};

struct Data
{
	
	struct Packet_0* tab_packet_0;
	struct Packet_1* tab_packet_1;
    int nb_pck;
};

struct mappy_label
{
    std::unordered_map<std::string,uint32_t>* mappy;
	uint32_t cursymb;
	
};

void libere_data (struct Data* D);
int get_nb_pck_files(const char* filename);
void get_pck0_tab (struct Data* D, const char* filename);
int get_mac_order(const char* mac1_str, const char* mac2_str) ;
char set_port_groupe (uint16_t port);
void get_symbole (struct Packet_1* pck1, struct mappy_label* ml);
void get_pck1_tab (struct Data* D, const char* filename, struct mappy_label* ml);
void export_packets_symb (struct Data* D, const char* pathout);
void apply_transformation (const char* filename_input_safe, const char* filename_input_anomalies, const char* pathout);

#endif