#include "Anomalies.h"
#include "Generateur_Donnees.h"
#include "Traffic_Creator.h"
#include "Usefull_Fonctions.h"

// Label 0 sans anomalies 
// Label 1 icmp flood
// Label 2 port scan
// Label 3 rst flood
// Label 4 slaves to master without request
// Label 5 no answer from rqt 

// Label 100 sans anomalies Mais fausse IP
// Label 101 icmp flood
// Label 102 port scan
// Label 103 rst flood
// Label 104 slaves to master without request
// Label 105 no answer from rqt 


void icmp_flood (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF)
{
	uint32_t n = *compteur;
	struct timespec start_icmp;
	struct timespec now_icmp;
	double ecart_icmp = 0.0;
	double delay_usleep_icmp = randomAB_double(0.1,1.00) * MSF->ratio_duration;
	double duration_icmp = randomAB_double(1,5)* MSF->ratio_duration;
	clock_gettime(CLOCK_MONOTONIC, &start_icmp);

	char* ip_src = MSF->M.ip;
	char* ip_dst = S->ip;
	char* mac_src = MSF->M.mac;
	char* mac_dst = S->mac;
	uint8_t direction = 0;

	int sender = randomAB_int(1,2);
	if (sender == 2)
		{
			ip_src = S->ip;
			ip_dst = MSF->M.ip;
			mac_src = S->mac;
			mac_dst = MSF->M.mac;
			direction = 1 ;
		}

	while (ecart_icmp < duration_icmp)
		{
			clock_gettime(CLOCK_MONOTONIC, &now_icmp);
			ecart_icmp = elapsed_time(start_icmp, now_icmp);
			tab_pck_anomalies[n].timer = curtime + ecart_icmp;
			tab_pck_anomalies[n].charge_utile = randomAB_int(S->ts_icmp.size_charge_less, S->ts_icmp.size_charge_sup);
			tab_pck_anomalies[n].pck_length = S->ts_icmp.size_block + tab_pck_anomalies[n].charge_utile;
			tab_pck_anomalies[n].port_src = 0;
			tab_pck_anomalies[n].port_dst = 0;
			tab_pck_anomalies[n].tra = 0;
			tab_pck_anomalies[n].ip_src = ip_src;
			tab_pck_anomalies[n].ip_dst = ip_dst;
			tab_pck_anomalies[n].mac_src = mac_src;
			tab_pck_anomalies[n].mac_dst = mac_dst;
			tab_pck_anomalies[n].direction = direction;
			tab_pck_anomalies[n].protocol = "icmp";
			tab_pck_anomalies[n].label = 1;	
			n++;		
			usleep(delay_usleep_icmp);
		}

	*compteur = n;
}

void port_scan (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF)
{
	uint32_t n = *compteur;
	struct timespec start_portscan;
	struct timespec now_portscan;
	double ecart_portscan = 0.0;
	double delay_usleep_portscan = randomAB_double(1.5,3.00) * MSF->ratio_duration;
	double duration_portscan = randomAB_double(5,20)* MSF->ratio_duration;
	clock_gettime(CLOCK_MONOTONIC, &start_portscan);

	char* ip_src = MSF->M.ip;
	char* ip_dst = S->ip;
	char* mac_src = MSF->M.mac;
	char* mac_dst = S->mac;
	uint8_t direction = 0;

	while (ecart_portscan < duration_portscan)
		{
			clock_gettime(CLOCK_MONOTONIC, &now_portscan);
			ecart_portscan = elapsed_time(start_portscan, now_portscan);
			tab_pck_anomalies[n].timer = curtime + ecart_portscan;
			tab_pck_anomalies[n].charge_utile = 0;
			tab_pck_anomalies[n].pck_length = 54;
			tab_pck_anomalies[n].port_src = randomAB_int(49152,65535);
			tab_pck_anomalies[n].port_dst = randomAB_int(0,49152);
			tab_pck_anomalies[n].tra = 6;
			tab_pck_anomalies[n].ip_src = ip_src;
			tab_pck_anomalies[n].ip_dst = ip_dst;
			tab_pck_anomalies[n].mac_src = mac_src;
			tab_pck_anomalies[n].mac_dst = mac_dst;
			tab_pck_anomalies[n].direction = direction;
			tab_pck_anomalies[n].protocol = "TCP";
			tab_pck_anomalies[n].label = 2;	
			n++;		
			usleep(delay_usleep_portscan);
		}

	*compteur = n;
}

void rstflood (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF)
{
	uint32_t n = *compteur;
	struct timespec start_rstflood;
	struct timespec now_rstflood;
	double ecart_rstflood = 0.0;
	double delay_usleep_rstflood = randomAB_double(0.5,1) * MSF->ratio_duration;
	double duration_rstflood = randomAB_double(1,3)* MSF->ratio_duration;
	clock_gettime(CLOCK_MONOTONIC, &start_rstflood);

	char* ip_src = MSF->M.ip;
	char* ip_dst = S->ip;
	char* mac_src = MSF->M.mac;
	char* mac_dst = S->mac;
	uint8_t direction = 0;

	while (ecart_rstflood < duration_rstflood)
		{
			clock_gettime(CLOCK_MONOTONIC, &now_rstflood);
			ecart_rstflood = elapsed_time(start_rstflood, now_rstflood);
			tab_pck_anomalies[n].timer = curtime + ecart_rstflood;
			tab_pck_anomalies[n].charge_utile = 0;
			tab_pck_anomalies[n].pck_length = 54;
			tab_pck_anomalies[n].port_src = randomAB_int(49152,65535);
			tab_pck_anomalies[n].port_dst = 502;
			tab_pck_anomalies[n].tra = 6;
			tab_pck_anomalies[n].ip_src = ip_src;
			tab_pck_anomalies[n].ip_dst = ip_dst;
			tab_pck_anomalies[n].mac_src = mac_src;
			tab_pck_anomalies[n].mac_dst = mac_dst;
			tab_pck_anomalies[n].direction = direction;
			tab_pck_anomalies[n].protocol = "TCP";
			tab_pck_anomalies[n].label = 3;	
			n++;		
			usleep(delay_usleep_rstflood);
		}

	*compteur = n;
}

void asw_no_rqt (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF)
{
	uint32_t n = *compteur;
	struct timespec start_asw_no_rqt;
	struct timespec now_asw_no_rqt;
	double ecart_asw_no_rqt = 0.0;
	double delay_asw_no_rqt = randomAB_double(1,3) * MSF->ratio_duration;
	double duration_asw_no_rqt = randomAB_double(10,20)* MSF->ratio_duration;
	clock_gettime(CLOCK_MONOTONIC, &start_asw_no_rqt);

	char* ip_src = S->ip;
	char* ip_dst = MSF->M.ip;
	char* mac_src = S->mac;
	char* mac_dst = MSF->M.mac;
	uint16_t port_src = MSF->M.port_modbus;
	uint16_t port_dst = S->port_modbus;
	uint8_t direction = 1;

	while (ecart_asw_no_rqt < duration_asw_no_rqt)
		{
			clock_gettime(CLOCK_MONOTONIC, &now_asw_no_rqt);
			ecart_asw_no_rqt = elapsed_time(start_asw_no_rqt, now_asw_no_rqt);
			tab_pck_anomalies[n].timer = curtime + ecart_asw_no_rqt;
			tab_pck_anomalies[n].charge_utile = randomAB_int(S->ts_modbus.size_charge_less, S->ts_modbus.size_charge_sup);
			tab_pck_anomalies[n].pck_length = S->ts_modbus.size_block + tab_pck_anomalies[n].charge_utile;
			tab_pck_anomalies[n].port_src = port_src;
			tab_pck_anomalies[n].port_dst = port_dst;
			tab_pck_anomalies[n].tra = 6;
			tab_pck_anomalies[n].ip_src = ip_src;
			tab_pck_anomalies[n].ip_dst = ip_dst;
			tab_pck_anomalies[n].mac_src = mac_src;
			tab_pck_anomalies[n].mac_dst = mac_dst;
			tab_pck_anomalies[n].direction = direction;
			tab_pck_anomalies[n].protocol = "modbus";
			tab_pck_anomalies[n].label = 4;	
			
			n++;		
			usleep(delay_asw_no_rqt);
		}

	*compteur = n;
}


void rqt_no_asw (struct Packet* tab_pck_anomalies, double curtime, uint32_t* compteur, struct Slave* S, struct Master_Slaves_Flow* MSF)
{
	uint32_t n = *compteur;
	struct timespec start_asw_no_rqt;
	struct timespec now_rqt_no_asw;
	double ecart_rqt_no_asw = 0.0;
	double delay_rqt_no_asw = S->ts_modbus.delay_asw;
	double duration_rqt_no_asw = randomAB_double(10,20)* MSF->ratio_duration;
	clock_gettime(CLOCK_MONOTONIC, &start_asw_no_rqt);

	
	char* ip_src = MSF->M.ip;
	char* ip_dst = S->ip;
	char* mac_src = MSF->M.mac;
	char* mac_dst = S->mac;
	uint16_t port_src = S->port_modbus;
	uint16_t port_dst = MSF->M.port_modbus;
	uint8_t direction = 0;

	while (ecart_rqt_no_asw < duration_rqt_no_asw)
		{
			clock_gettime(CLOCK_MONOTONIC, &now_rqt_no_asw);
			ecart_rqt_no_asw = elapsed_time(start_asw_no_rqt, now_rqt_no_asw);
			tab_pck_anomalies[n].timer = curtime + ecart_rqt_no_asw;
			tab_pck_anomalies[n].charge_utile = randomAB_int(S->ts_modbus.size_charge_less, S->ts_modbus.size_charge_sup);
			tab_pck_anomalies[n].pck_length = S->ts_modbus.size_block + tab_pck_anomalies[n].charge_utile;
			tab_pck_anomalies[n].port_src = port_src;
			tab_pck_anomalies[n].port_dst = port_dst;
			tab_pck_anomalies[n].tra = 6;
			tab_pck_anomalies[n].ip_src = ip_src;
			tab_pck_anomalies[n].ip_dst = ip_dst;
			tab_pck_anomalies[n].mac_src = mac_src;
			tab_pck_anomalies[n].mac_dst = mac_dst;
			tab_pck_anomalies[n].direction = direction;
			tab_pck_anomalies[n].protocol = "modbus";
			tab_pck_anomalies[n].label = 5;	
			n++;		
			usleep(delay_rqt_no_asw);
		}

	*compteur = n;
}



struct Packet* get_anomalies_array (struct Slave* S, struct Master_Slaves_Flow* MSF)
{

	
	double delay_usleep = 1.00 * MSF->ratio_duration * 1000000.00;
	uint32_t compteur = 0;
	uint32_t max_packet = 10000000;
	struct Packet* tab_pck_anomalies = (struct Packet*)malloc(max_packet * sizeof(struct Packet));

	struct timespec start, timer, old ,now;
	double ecart_timerstart = 0.0;
	double ecart_oldnow;
	clock_gettime(CLOCK_MONOTONIC, &start);
	old = start;

	uint32_t num_alea = 0;

	while (ecart_timerstart < MSF->duration_scenario)
		{	
			clock_gettime(CLOCK_MONOTONIC, &timer);
			ecart_timerstart = elapsed_time(start, timer); 
			double alea = randomAB_double(0, 2000);
			
			if (alea > 5) 
				{

					usleep(delay_usleep);
					continue;
				} 

			if (alea > 4)
				{
					
					icmp_flood(tab_pck_anomalies, ecart_timerstart, &compteur, S, MSF);
					usleep(delay_usleep);
					continue;
				}
			if (alea > 3)
				{	
					
					port_scan(tab_pck_anomalies, ecart_timerstart, &compteur, S, MSF);
					usleep(delay_usleep);
					continue;
				}
			if (alea > 2)
				{
					
					rstflood(tab_pck_anomalies, ecart_timerstart, &compteur, S, MSF);
					usleep(delay_usleep);
					continue;
				}

			if (alea > 1)
				{
					// envoies de esclaves sans requête master 
					
					asw_no_rqt(tab_pck_anomalies, ecart_timerstart, &compteur, S, MSF);
					usleep(delay_usleep);
					continue;
				}

			if (alea > 0)
				{
					// pas de réponses aux requête
					
					rqt_no_asw(tab_pck_anomalies, ecart_timerstart, &compteur, S, MSF);
					usleep(delay_usleep);
					continue;
				}
			


		}


	struct Packet* tab_pck = (struct Packet*)malloc((compteur+1)*sizeof(struct Packet));	
	tab_pck[compteur].timer = -1.0; // Pour eviter d'avoir à return le nb de pck 
	for (int i = 0; i < compteur; i++)
		{
			tab_pck[i] = tab_pck_anomalies[i]; 
		}
	free(tab_pck_anomalies);

	return tab_pck;

};

void get_packet_array_attackers (struct Slave* A, struct Master_Slaves_Flow* MSF, char s_o_a) 
{
	struct timespec start, timer, old ,now;
	double ecart_timerstart = 0.00;
	double ecart_oldnow;
	clock_gettime(CLOCK_MONOTONIC, &start); 
	double delay_usleep = 1 * MSF->ratio_duration * 1000000.00 * MSF->ratio_duration;

	for (int i = 0; i < A->nb_starts_action; ++i)
		{
			while (ecart_timerstart < A->starts_action[i])
				{
					clock_gettime(CLOCK_MONOTONIC, &timer);
					ecart_timerstart = elapsed_time(start, timer); 
					usleep(delay_usleep);
				}

			if (randomAB_int(0,100) < 30)
				{
					get_packet_array_slaves (A, MSF, 'a');
					for (int i = 0; i < A->nb_pck_anomalies; ++i)
						{
							A->tab_pck_anomalies[i].label = A->tab_pck_anomalies[i].label + 100;
							A->tab_pck_anomalies[i].timer = A->tab_pck_anomalies[i].timer + ecart_timerstart;
						}
					return;
				}
		}
	A->tab_pck_anomalies = (struct Packet*)malloc(sizeof(Packet));
	A->tab_pck_anomalies[0].timer = 100000;
	A->nb_pck_anomalies = 0;
}
