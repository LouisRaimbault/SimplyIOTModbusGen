#include "Generateur_Donnees.h"
#include "Usefull_Fonctions.h"
#include "Traffic_Creator.h"

// Attention, les ports de la struct esclaves sont les ports ouvert par le master , car les esclaves sont en fait serveurs


void get_standart_size (struct Standart_Size* SS)
{
	SS->size_tcp = 20;
	SS->size_udp = 8;
	SS->size_arp = 42;
	SS->cu_icmp_inf = 40; // + size udp
	SS->cu_icmp_sup = 72; // + size udp
	SS->cu_ntp_inf = 48; // + size udp
	SS->cu_ntp_sup = 64; // + size udp
	SS->cu_snmp_inf = 20; // + size_tcp
	SS->cu_snmp_sup = 484; // +size_tcp
	SS->cu_modbus_inf = 8; // + size_tcp
	SS->cu_modbus_sup = 259; // +size_tcp

}

int randomAB_int(int min, int max) 
{
    // random() renvoie un long entre 0 et RAND_MAX
    return min + (int)(random() % (max - min + 1));
}

double randomAB_double(double min, double max) 
{
    // Normalisation : random() / RAND_MAX donne un double dans [0,1]
    double normalized = (double)random() / (double)RAND_MAX;
    return min + normalized * (max - min);
}

double elapsed_time(struct timespec start, struct timespec end) 
{
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9;
}


// /!\ A gerer la taille paquet = taille block + taille charge utile qui elle est aléatoire


char* create_random_ipv4_address() 
{
    char *ip = (char*)malloc(16); // "255.255.255.255" + '\0' = 16 octets
    if (!ip) return NULL;

    uint8_t a = randomAB_int(0, 256);
    uint8_t b = randomAB_int(0, 256);
    uint8_t c = randomAB_int(0, 256);
    uint8_t d = randomAB_int(0, 256);

    snprintf(ip, 16, "%u.%u.%u.%u", a, b, c, d);
    return ip;
}

char* create_random_mac_address() 
{
    char *mac = (char*)malloc(18); // 6 octets × 2 hex + 5 ':' + '\0' = 18
    if (!mac) return NULL;

    unsigned char bytes[6];
    for (int i = 0; i < 6; i++) {
        bytes[i] = randomAB_int(0, 256); // 0..255
    }

    // Formate en chaîne hexadécimale avec deux chiffres par octet
    snprintf(mac, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
             bytes[0], bytes[1], bytes[2],
             bytes[3], bytes[4], bytes[5]);

    return mac;
}

void init_slave (struct Slave* S, struct Standart_Size* SS, struct Master_Slaves_Flow* MSF)
{
	S->ip = create_random_ipv4_address();
	S->mac = create_random_mac_address();
	S->port_ntp = randomAB_int(49151, 65535);
	S->port_snmp = randomAB_int(49151, 65535);
	while (S->port_snmp == S->port_ntp)
		{
			S->port_snmp = randomAB_int(49151, 65535);
		}
	S->port_modbus = randomAB_int(49151, 65535);
	while (S->port_snmp == S->port_ntp || S->port_modbus == S->port_snmp)
		{
			S->port_modbus = randomAB_int(49151, 65535);
		}

	S->ts_arp.delay_rqt = randomAB_double(30,120)*MSF->ratio_duration;
	S->ts_arp.delay_asw = randomAB_double(0.0005,0.001)*MSF->ratio_duration;
	S->ts_arp.size_block = 42;
	S->ts_arp.size_charge_sup = 0;
	S->ts_arp.size_charge_less = 0;

	S->ts_icmp.delay_rqt = randomAB_double(5,10)*MSF->ratio_duration;
	S->ts_icmp.delay_asw = randomAB_double(0.0005,0.001)*MSF->ratio_duration;
	S->ts_icmp.size_block = SS->size_udp;
	S->ts_icmp.size_charge_sup = SS->cu_icmp_inf;
	S->ts_icmp.size_charge_less = SS->cu_icmp_sup;

	S->ts_ntp.delay_rqt = randomAB_double(16,512)*MSF->ratio_duration;
	S->ts_ntp.delay_asw = randomAB_double(0.001,0.010)*MSF->ratio_duration;
	S->ts_ntp.size_block = SS->size_udp;
	S->ts_ntp.size_charge_sup = SS->cu_ntp_inf;
	S->ts_ntp.size_charge_less = SS->cu_ntp_sup;

	S->ts_snmp.delay_rqt = randomAB_double(5,300)*MSF->ratio_duration;
	S->ts_snmp.delay_asw = randomAB_double(0.001,0.020)*MSF->ratio_duration;
	S->ts_snmp.size_block = SS->size_tcp;
	S->ts_snmp.size_charge_sup = SS->cu_snmp_inf;
	S->ts_snmp.size_charge_less = SS->cu_snmp_sup;

	S->ts_modbus.delay_rqt = randomAB_double(0.010,5)*MSF->ratio_duration;
	S->ts_modbus.delay_asw = randomAB_double(0.001,0.010)*MSF->ratio_duration;
	S->ts_modbus.size_block = SS->size_tcp;
	S->ts_modbus.size_charge_sup = SS->cu_modbus_inf;
	S->ts_modbus.size_charge_less = SS->cu_modbus_sup;
}

void sort_doubles(double* arr, int n) 
{
    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = i + 1; j < n; j++) 
        {
            if (arr[i] > arr[j]) 
            {
                double tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

void set_attackers_starts_action (struct Slave* S, struct Master_Slaves_Flow* MSF)
{

	for (int i = 0; i < S->nb_starts_action; i++)
		{
			S->starts_action[i] = randomAB_double(0, MSF->duration_scenario);	
		}

	sort_doubles(S->starts_action, S->nb_starts_action);	
	S->starts_action[S->nb_starts_action] = MSF->duration_scenario* (2.00);

}

void init_MSF (struct Master_Slaves_Flow* MSF, struct Standart_Size* SS)
{
	MSF->M.ip = create_random_ipv4_address();
	MSF->M.mac = create_random_mac_address();
	MSF->M.port_ntp = 123;
	MSF->M.port_snmp = 161;
	MSF->M.port_modbus = 502;
	MSF->tab_slaves = (struct Slave*)malloc(MSF->nb_slaves*sizeof(struct Slave));

	for (int i = 0; i < MSF->nb_slaves; i++)
		{
			init_slave(&(MSF->tab_slaves[i]), SS, MSF);
			MSF->tab_slaves[i].is_attacker = 0;
			MSF->tab_slaves[i].starts_action = (double*)malloc(1*sizeof(double));
			MSF->tab_slaves[i].starts_action[0] = 0.00;
    		MSF->tab_slaves[i].nb_starts_action = 1;
    		MSF->tab_slaves[i].duration_action = MSF->duration_scenario;
		}

	MSF->tab_attackers = (struct Slave*)malloc(MSF->nb_attackers*sizeof(struct Slave));
	for (int i = 0; i < MSF->nb_attackers; i++)
		{
			init_slave(&(MSF->tab_attackers[i]), SS, MSF);
			MSF->tab_attackers[i].is_attacker = 1;
			MSF->tab_attackers[i].starts_action = (double*)malloc((3+1)*sizeof(double));
    		MSF->tab_attackers[i].nb_starts_action = 3;
    		MSF->tab_attackers[i].duration_action = MSF->duration_scenario/10;
    		set_attackers_starts_action (&(MSF->tab_attackers[i]), MSF);
		}	
}

void export_packets(struct Packet* tab_pck, int nb_pck, const char* pathout, struct Modbus_Network* MN)
{
	print_debog (15, pathout, "blanc");
    std::ofstream fichier (pathout); // ouvre en écriture par défaut
    fichier << "Id Packet,timestamp,mac_src,mac_dst,ip_src,ip_dst,pck_length,port_src,port_dst,tra,direction,charge_utile,protocol,label\n";
    for (int i = 0; i < nb_pck; i++)
    	{	
    		fichier << i << ",";
    		fichier << tab_pck[i].timer /MN->ratio_duration << ",";
			fichier << tab_pck[i].mac_src << ",";
			fichier << tab_pck[i].mac_dst << ",";
			fichier << tab_pck[i].ip_src << ",";
			fichier << tab_pck[i].ip_dst << ",";
			fichier << tab_pck[i].pck_length << ",";
			fichier << tab_pck[i].port_src << ",";
			fichier << tab_pck[i].port_dst << ",";
			fichier << tab_pck[i].tra << ",";
			fichier << tab_pck[i].direction << ",";
			fichier << tab_pck[i].charge_utile << ",";
			fichier << tab_pck[i].protocol << ",";
			fichier << tab_pck[i].label <<"\n";
    	}
    fichier.close(); // ferme le fichier
    return ;
}


void Generateur_Donnnees_MSF(struct Master_Slaves_Flow* MSF, int i, struct Modbus_Network* MN)
{
	srandom(time(NULL));		
	struct Standart_Size SS;
	get_standart_size(&SS);
	init_MSF(MSF, &SS);
	get_packet_array_MSF(MSF, 's');
	char* pathout_safe;
	asprintf(&pathout_safe, "%s_safe_%d.txt", MN->path, i); // GNU extension
	export_packets(MSF->tab_pck_safe, MSF->nb_pck_safe, pathout_safe, MN);
	get_packet_array_MSF(MSF, 'a');
	char* pathout_anomalies;
	asprintf(&pathout_anomalies, "%s_anomalies_%d.txt", MN->path, i); // GNU extension
	export_packets(MSF->tab_pck_anomalies, MSF->nb_pck_anomalies, pathout_anomalies, MN);
	free(pathout_safe);
	free(pathout_anomalies);
	return;
}

struct Packet* replace_order_array (struct Packet* tab_1, struct Packet* tab_2, int nb1, int nb2)
{

	int n1 = 0;
	int n2 = 0;
	struct Packet* ret_table = (struct Packet*)malloc((nb1+nb2) * sizeof(struct Packet));
	for (int i = 0; i < (nb1+nb2); i++)
		{
			if (tab_1[n1].timer < tab_2[n2].timer)
				{
					ret_table[i] = tab_1[n1];
					n1++;
					if (n1 == nb1)
						{
							n1--; 
							tab_1[n1].timer = 10000000.00;
						}
					continue;
				}

			ret_table[i] = tab_2[n2];
			n2++;			
			if (n2 == nb2)
				{
					n2--; 
					tab_2[n2].timer = 10000000.00;
				}
			continue;
		}

	free(tab_1);
	free(tab_2);

	return ret_table;
}

void Generateur_Donnnees_MN(struct Modbus_Network* MN)
{
	clean_stdin();
	selection_string_manuelle(&(MN->path), "string", "MSF.path");
	selection_valeur_manuelle(&(MN->nb_msf), "uint16_t", "MSF.nb_msf");
	
	MN->tab_msf = (struct Master_Slaves_Flow*)malloc(MN->nb_msf*sizeof(struct Master_Slaves_Flow));
	for (int i = 0; i < MN->nb_msf; i++)
		{	
			print_debog_nb(1,"Please, enter parameter for master ", i+1, "blanc");
			selection_valeur_manuelle(&(MN->tab_msf[i].nb_slaves), "uint16_t", "MSF.nb_slaves");
			selection_valeur_manuelle(&(MN->tab_msf[i].nb_attackers), "uint16_t", "MSF.nb_attackers");
		}
	struct Packet* tab_pck_safe = (struct Packet*)malloc(sizeof(struct Packet));
	tab_pck_safe[0].timer = 10000001.00;
	int nb_pck_safe = 0;
	struct Packet* tab_pck_anomalies = (struct Packet*)malloc(sizeof(struct Packet));
	tab_pck_anomalies[0].timer = 10000001.00;
	int nb_pck_anomalies = 0;
	for (int i = 0; i < MN->nb_msf; i++)
		{
			print_debog_nb(10, "Build Master  ", i, "white");
			MN->tab_msf[i].duration_scenario = MN->duration_scenario;
			MN->tab_msf[i].ratio_duration = MN->ratio_duration;
			Generateur_Donnnees_MSF(&(MN->tab_msf[i]), i, MN);
			tab_pck_safe = replace_order_array(tab_pck_safe, MN->tab_msf[i].tab_pck_safe, nb_pck_safe, MN->tab_msf[i].nb_pck_safe);
			nb_pck_safe = nb_pck_safe + MN->tab_msf[i].nb_pck_safe;
			tab_pck_anomalies = replace_order_array(tab_pck_anomalies, MN->tab_msf[i].tab_pck_anomalies, nb_pck_anomalies, MN->tab_msf[i].nb_pck_anomalies);
			nb_pck_anomalies = nb_pck_anomalies + MN->tab_msf[i].nb_pck_anomalies;
		}

	print_debog(10, "Merging Master ... ", "white");
	char* pathout_safe;	
	asprintf(&pathout_safe, "%s_network_safe.txt", MN->path);	
	export_packets(tab_pck_safe, nb_pck_safe, pathout_safe, MN);
	char* pathout_anomalies;	
	asprintf(&pathout_anomalies, "%s_network_anomalies.txt", MN->path);	
	export_packets(tab_pck_anomalies, nb_pck_anomalies, pathout_anomalies, MN);
	free(MN->tab_msf);
	free(tab_pck_anomalies);
	free(tab_pck_safe);

	return;	
}