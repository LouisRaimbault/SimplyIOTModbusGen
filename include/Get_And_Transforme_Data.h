#ifndef GET_AND_TRANSFORME_DATA_H 
#define GET_AND_TRANSFORME_DATA_H

#include "Generateur_Donnees.h"
#include <string>

struct Packet_1
{
	char ts;        // 8
    char pck_length; // 2
    char charge_utile;//2
    char port_src;   //2
    char port_dst;   //2
    char tra;        //2
    char direction;  //2
    char* concat;        //8
    uint16_t label;      //2
    char* ip_src;        //8
    char* ip_dst;        //8
    char* mac_src;       //8
    char* mac_dst;       //8
    char* protocol;
    uint32_t symbole;
    
};

struct Packet_0
{
	double timer;        // 8
    uint16_t pck_length; // 2
    uint16_t charge_utile;//2
    uint16_t port_src;   //2
    uint16_t port_dst;   //2
    uint16_t tra;        //2
    uint16_t direction;  //2
    uint16_t label;      //2
    char* ip_src;        //8
    char* ip_dst;        //8
    char* mac_src;       //8
    char* mac_dst;       //8
    char* concat;        //8
    char* protocol;
};

struct conversation_values
{
	int nb_pck;
	double last_timer;
	uint32_t somme_pck_length;
	uint16_t avg_pck_length;
	uint32_t somme_charge_utile;
	uint32_t avg_charge_utile;
};

struct Data
{
	int nb_pck;
	struct Packet_0* tab_packet_0;
	struct Packet_1* tab_packet_1;
};

struct mappy_label
{
	uint32_t cursymb;
	std::unordered_map<std::string,uint32_t>* mappy;
};

int get_nb_pck_files(const char* filename);
void get_pck0_tab (struct Data* D, const char* filename);
char* concat_mac_order(const char* mac1_str, const char* mac2_str);
char set_port_groupe (uint16_t port);
void get_symbole (struct Packet_1* pck1, struct mappy_label* ml);
void get_pck1_tab (struct Data* D, const char* filename, struct mappy_label* ml);
void export_packets_symb (struct Data* D, const char* pathout);
void apply_transformation (const char* filename_input_safe, const char* filename_input_anomalies, const char* pathout);

#endif