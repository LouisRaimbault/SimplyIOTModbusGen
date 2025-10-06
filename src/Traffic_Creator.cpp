#include "Generateur_Donnees.h"
#include "Traffic_Creator.h"
#include "Anomalies.h"
#include "Affiche_Debog.h"


struct Packet* get_packet_array (struct Slave* S, struct Timer_Size* TS, struct Master_Slaves_Flow* MSF)
{
	print_debog(15, "#get_packet_array ", "cyan");
	print_debog(15, "get_packet_array 1 ", "cyan");
	double duration_scenario = MSF->duration_scenario;
	
	if ( S->is_attacker == 1)
		{
			duration_scenario = S->duration_action;
		}

	uint32_t compteur = 0;
	uint32_t max_packet = (uint32_t)(duration_scenario/TS->delay_rqt)*3 + 10;
	std::cout << "time Scenario = " << duration_scenario << " et TS->delay_rqt = " << TS->delay_rqt << " et max packet = " << max_packet << "\n";
	double * timers = (double*)malloc(max_packet*sizeof(double));
	uint8_t* q_o_s = (uint8_t*)malloc(max_packet*sizeof(double));
	struct timespec start, timer, old ,now;
	double ecart_timerstart = 0.0;
	double ecart_oldnow;
	clock_gettime(CLOCK_MONOTONIC, &start);
	old = start;
	print_debog(15, "#get_packet_array 2 ", "cyan");
	while (ecart_timerstart < duration_scenario)
		{	
			clock_gettime(CLOCK_MONOTONIC, &now);
			ecart_oldnow = elapsed_time(old, now);
			if (ecart_oldnow < TS->delay_rqt)
				{
					clock_gettime(CLOCK_MONOTONIC, &timer);
					ecart_timerstart = elapsed_time(start, timer);
					continue;
				}

			clock_gettime(CLOCK_MONOTONIC, &timer);	
			timers[compteur] = elapsed_time(start, timer);
			q_o_s[compteur] = 1;
			compteur++;

			ecart_oldnow = 0.0;
			clock_gettime(CLOCK_MONOTONIC, &old);	
			while (ecart_oldnow < TS->delay_asw)
				{
					clock_gettime(CLOCK_MONOTONIC, &now);
					ecart_oldnow = elapsed_time(old, now);
				} 

			clock_gettime(CLOCK_MONOTONIC, &timer);	
			timers[compteur] = elapsed_time(start, timer);
			q_o_s[compteur] = 0;
			compteur++;
			if (compteur >= max_packet)
				{
					print_debog(15, " COMPTEUR > MAX PCK", "rouge");
					print_debog_nb(15, "compteur = ", compteur, "rouge");
					print_debog_nb(15, "Max packet = ", max_packet, "rouge");
					std::cout << "elapsed_time = " << ecart_timerstart << "\n";
					exit(1);
				}
			ecart_oldnow = 0.0;
			ecart_timerstart = elapsed_time(start, timer);

		}

	print_debog(15, "#get_packet_array 3 ", "cyan");
	print_debog_nb(15, "#get_packet_array compteur =  ", compteur, "vert");	
	struct Packet* tab_pck = (struct Packet*)malloc((compteur+1)*sizeof(struct Packet));	
	tab_pck[compteur].timer = -1.0; // Pour eviter d'avoir à return le nb de pck 
	for (int i = 0; i < compteur; i++)
		{
			tab_pck[i].timer = timers[i];
			tab_pck[i].direction = 1-q_o_s[i];
		}

	print_debog(15, "#get_packet_array 4 ", "cyan");
	free(timers);
	free(q_o_s);
	print_debog(15, "#get_packet_array OUT /////////// ", "cyan");
	return tab_pck;

} 


// /!\ A gerer la taille paquet = taille block + taille charge utile qui elle est aléatoire

void get_packet_array_slaves (struct Slave* S, struct Master_Slaves_Flow* MSF, char s_o_a)
{
	print_debog(10, "#get_packet_array_slaves_anomalies ", "cyan");
	print_debog(10, "get_packet_array_slaves_anomalies : Debog 1", "cyan");
	struct Packet* tab_pck_arp = get_packet_array (S, &(S->ts_arp), MSF);
	int nb_pck_arp = 0;
	// Ne pas oublier de mettre une valeur abbérante sur le dernier +1
	print_debog(10, "get_packet_array_slaves_anomalies : Debog 2", "cyan");
	while (tab_pck_arp[nb_pck_arp].timer > 0)
		{
			tab_pck_arp[nb_pck_arp].charge_utile = randomAB_int(S->ts_arp.size_charge_less, S->ts_arp.size_charge_sup);
			tab_pck_arp[nb_pck_arp].pck_length = S->ts_arp.size_block + tab_pck_arp[nb_pck_arp].charge_utile;
			tab_pck_arp[nb_pck_arp].port_src = 0;
			tab_pck_arp[nb_pck_arp].port_dst = 0;
			tab_pck_arp[nb_pck_arp].tra = 0;
			tab_pck_arp[nb_pck_arp].ip_src = "0";
			tab_pck_arp[nb_pck_arp].ip_dst = "0";
			tab_pck_arp[nb_pck_arp].mac_src = MSF->M.mac;
			tab_pck_arp[nb_pck_arp].mac_dst = S->mac;
			tab_pck_arp[nb_pck_arp].protocol = "arp";
			tab_pck_arp[nb_pck_arp].label = 0;
			if (tab_pck_arp[nb_pck_arp].direction == 1)
				{
					tab_pck_arp[nb_pck_arp].mac_src = S->mac;
					tab_pck_arp[nb_pck_arp].mac_dst = MSF->M.mac;
				}

			nb_pck_arp++;
		}
	print_debog(10, "get_packet_array_slaves_anomalies : Debog 3", "cyan");	 
	tab_pck_arp[nb_pck_arp].timer = 100001.00;
	print_debog(10, "get_packet_array_slaves_anomalies : Debog 4", "cyan");
	struct Packet* tab_pck_icmp = get_packet_array (S, &(S->ts_icmp), MSF);
	int nb_pck_icmp = 0;
	while (tab_pck_icmp[nb_pck_icmp].timer > 0)
		{
			tab_pck_icmp[nb_pck_icmp].charge_utile = randomAB_int(S->ts_icmp.size_charge_less, S->ts_icmp.size_charge_sup);
			tab_pck_icmp[nb_pck_icmp].pck_length = S->ts_icmp.size_block + tab_pck_icmp[nb_pck_icmp].charge_utile;
			tab_pck_icmp[nb_pck_icmp].port_src = 0;
			tab_pck_icmp[nb_pck_icmp].port_dst = 0;
			tab_pck_icmp[nb_pck_icmp].tra = 0;
			tab_pck_icmp[nb_pck_icmp].ip_src = MSF->M.ip;
			tab_pck_icmp[nb_pck_icmp].ip_dst = S->ip;
			tab_pck_icmp[nb_pck_icmp].mac_src = MSF->M.mac;
			tab_pck_icmp[nb_pck_icmp].mac_dst = S->mac;
			tab_pck_icmp[nb_pck_icmp].protocol = "icmp";
			tab_pck_icmp[nb_pck_icmp].label = 0;
			
			if (tab_pck_icmp[nb_pck_icmp].direction == 1)
				{
					tab_pck_icmp[nb_pck_icmp].ip_src = S->ip;
					tab_pck_icmp[nb_pck_icmp].ip_dst = MSF->M.ip;
					tab_pck_icmp[nb_pck_icmp].mac_src = S->mac;
					tab_pck_icmp[nb_pck_icmp].mac_dst = MSF->M.mac;
				}

			nb_pck_icmp++;
		} 
	tab_pck_icmp[nb_pck_icmp].timer = 100001.00;
	print_debog(10, "get_packet_array_slaves_anomalies : Debog 5", "cyan");
	

	struct Packet* tab_pck_ntp = get_packet_array (S, &(S->ts_ntp), MSF);
	int nb_pck_ntp = 0;
	while (tab_pck_ntp[nb_pck_ntp].timer > 0)
		{
			tab_pck_ntp[nb_pck_ntp].charge_utile = randomAB_int(S->ts_ntp.size_charge_less, S->ts_ntp.size_charge_sup);
			tab_pck_ntp[nb_pck_ntp].pck_length = S->ts_ntp.size_block + tab_pck_ntp[nb_pck_ntp].charge_utile;
			tab_pck_ntp[nb_pck_ntp].port_src = S->port_ntp;
			tab_pck_ntp[nb_pck_ntp].port_dst = MSF->M.port_ntp;
			tab_pck_ntp[nb_pck_ntp].tra = 17;
			tab_pck_ntp[nb_pck_ntp].ip_src = MSF->M.ip;
			tab_pck_ntp[nb_pck_ntp].ip_dst = S->ip;
			tab_pck_ntp[nb_pck_ntp].mac_src = MSF->M.mac;
			tab_pck_ntp[nb_pck_ntp].mac_dst = S->mac;
			tab_pck_ntp[nb_pck_ntp].protocol = "ntp";
			tab_pck_ntp[nb_pck_ntp].label = 0;
			
			if (tab_pck_ntp[nb_pck_ntp].direction == 1)
				{
					tab_pck_ntp[nb_pck_ntp].port_src = MSF->M.port_ntp;
					tab_pck_ntp[nb_pck_ntp].port_dst = S->port_ntp;
					tab_pck_ntp[nb_pck_ntp].ip_src = S->ip;
					tab_pck_ntp[nb_pck_ntp].ip_dst = MSF->M.ip;					
					tab_pck_ntp[nb_pck_ntp].mac_src = S->mac;
					tab_pck_ntp[nb_pck_ntp].mac_dst = MSF->M.mac;
				}

			nb_pck_ntp++;
		} 
	tab_pck_ntp[nb_pck_ntp].timer = 100001.00;
	print_debog(10, "get_packet_array_slaves_anomalies : Debog 6", "cyan");

	struct Packet* tab_pck_snmp = get_packet_array (S, &(S->ts_snmp), MSF);
	int nb_pck_snmp = 0;
	while (tab_pck_snmp[nb_pck_snmp].timer > 0)
		{
			tab_pck_snmp[nb_pck_snmp].charge_utile = randomAB_int(S->ts_snmp.size_charge_less, S->ts_snmp.size_charge_sup);
			tab_pck_snmp[nb_pck_snmp].pck_length = S->ts_snmp.size_block + tab_pck_snmp[nb_pck_snmp].charge_utile;
			tab_pck_snmp[nb_pck_snmp].port_src = S->port_snmp;
			tab_pck_snmp[nb_pck_snmp].port_dst = MSF->M.port_snmp;
			tab_pck_snmp[nb_pck_snmp].tra = 6;
			tab_pck_snmp[nb_pck_snmp].ip_src = MSF->M.ip;
			tab_pck_snmp[nb_pck_snmp].ip_dst = S->ip;
			tab_pck_snmp[nb_pck_snmp].mac_src = MSF->M.mac;
			tab_pck_snmp[nb_pck_snmp].mac_dst = S->mac;
			tab_pck_snmp[nb_pck_snmp].protocol = "snmp";
			tab_pck_snmp[nb_pck_snmp].label = 0;

			if (tab_pck_snmp[nb_pck_snmp].direction == 1)
				{
					tab_pck_snmp[nb_pck_snmp].port_src = MSF->M.port_snmp;
					tab_pck_snmp[nb_pck_snmp].port_dst = S->port_snmp;
					tab_pck_snmp[nb_pck_snmp].ip_src = S->ip;
					tab_pck_snmp[nb_pck_snmp].ip_dst = MSF->M.ip;					
					tab_pck_snmp[nb_pck_snmp].mac_src = S->mac;
					tab_pck_snmp[nb_pck_snmp].mac_dst = MSF->M.mac;
				}

			nb_pck_snmp++;
		} 
	tab_pck_snmp[nb_pck_snmp].timer = 100001.00;
	print_debog(10, "get_packet_array_slaves_anomalies : Debog 7", "cyan");

	struct Packet* tab_pck_modbus = get_packet_array (S, &(S->ts_modbus), MSF);
	print_debog(10, "get_packet_array_slaves_anomalies : Debog 7 bis", "cyan");
	int nb_pck_modbus = 0;
	while (tab_pck_modbus[nb_pck_modbus].timer > 0)
		{

			tab_pck_modbus[nb_pck_modbus].charge_utile = randomAB_int(S->ts_modbus.size_charge_less, S->ts_modbus.size_charge_sup);
			tab_pck_modbus[nb_pck_modbus].pck_length = S->ts_modbus.size_block + tab_pck_modbus[nb_pck_modbus].charge_utile;
			tab_pck_modbus[nb_pck_modbus].port_src = S->port_modbus;
			tab_pck_modbus[nb_pck_modbus].port_dst = MSF->M.port_modbus;
			tab_pck_modbus[nb_pck_modbus].tra = 6;
			tab_pck_modbus[nb_pck_modbus].ip_src = MSF->M.ip;
			tab_pck_modbus[nb_pck_modbus].ip_dst = S->ip;
			tab_pck_modbus[nb_pck_modbus].mac_src = MSF->M.mac;
			tab_pck_modbus[nb_pck_modbus].mac_dst = S->mac;
			tab_pck_modbus[nb_pck_modbus].protocol = "modbus";
			tab_pck_modbus[nb_pck_modbus].label = 0;
			
			if (tab_pck_modbus[nb_pck_modbus].direction == 1)
				{
					tab_pck_modbus[nb_pck_modbus].port_src = MSF->M.port_modbus;
					tab_pck_modbus[nb_pck_modbus].port_dst = S->port_modbus;
					tab_pck_modbus[nb_pck_modbus].ip_src = S->ip;
					tab_pck_modbus[nb_pck_modbus].ip_dst = MSF->M.ip;					
					tab_pck_modbus[nb_pck_modbus].mac_src = S->mac;
					tab_pck_modbus[nb_pck_modbus].mac_dst = MSF->M.mac;
				}
			nb_pck_modbus++;
		} 

	tab_pck_modbus[nb_pck_modbus].timer = 100001.00;
	print_debog(10, "get_packet_array_slaves_anomalies : Debog A", "cyan");	
	struct Packet* tab_pck_anomalies = NULL;
	uint32_t nb_pck_anomalies = 0;
	std::cout << "S O A est " << s_o_a << "\n";
	if (s_o_a == 'a')
		{
			std::cout << "--------------- JULIANA \n";
			tab_pck_anomalies = get_anomalies_array (S, MSF);
			
		}
	if (tab_pck_anomalies == NULL) 
		{
			std::cout << "--------------- WTF \n";
			tab_pck_anomalies = (struct Packet*)malloc(sizeof(struct Packet));
			tab_pck_anomalies[0].timer = -1.00;
		}
	
	while (tab_pck_anomalies[nb_pck_anomalies].timer > 0) {++nb_pck_anomalies;}
	tab_pck_anomalies[nb_pck_anomalies].timer = 100001.00;
	std::cout << "### NB_PCK_ANOMALIES = " << nb_pck_anomalies << "\n";
	
	print_debog(10, "get_packet_array_slaves_anomalies : Debog C", "cyan");

	print_debog(10, "get_packet_array_slaves_anomalies : Debog 8", "cyan");
	

	int nb_pck = nb_pck_arp + nb_pck_icmp + nb_pck_ntp + nb_pck_snmp  + nb_pck_modbus + nb_pck_anomalies;

	struct Packet* tab_pck = (struct Packet*)malloc((nb_pck+1)*sizeof(struct Packet));
	tab_pck[nb_pck].timer = 100001.00;
	std::unordered_map<std::string, struct Packet*> mappy;
	mappy["arp"] = &tab_pck_arp[0];
	mappy["icmp"] = &tab_pck_icmp[0];
	mappy["ntp"] = &tab_pck_ntp[0];
	mappy["snmp"] = &tab_pck_snmp[0];
	mappy["modbus"] = &tab_pck_modbus[0];
	mappy["anomalies"] = &tab_pck_anomalies[0];

	std::string shorter = "";
	double val_shorter = 100000.00;
	double *pt_shorter;
	double val_arp = tab_pck_arp[0].timer;
	double val_icmp = tab_pck_icmp[0].timer;
	double val_ntp = tab_pck_ntp[0].timer;
	double val_snmp = tab_pck_snmp[0].timer;
	double val_modbus = tab_pck_modbus[0].timer;
	double val_anomalies = tab_pck_anomalies[0].timer;
	std::cout << "val anomale = " << val_anomalies << "\n";
	print_debog(10, "get_packet_array_slaves_anomalies : Debog 9", "cyan");
	print_debog_nb(10, "nb pck total = ", nb_pck, "vert");
	for (int i = 0; i < nb_pck; i++)
		{ 
			pt_shorter = &val_shorter;
			if (val_arp < *(pt_shorter))     {pt_shorter = &val_arp; shorter = "arp";}
			if (val_icmp < *(pt_shorter))    {pt_shorter = &val_icmp; shorter = "icmp";}
			if (val_ntp <  *(pt_shorter))    {pt_shorter = &val_ntp; shorter = "ntp";}
			if (val_snmp < *(pt_shorter))    {pt_shorter = &val_snmp; shorter = "snmp";}
			if (val_modbus < *(pt_shorter))  {pt_shorter = &val_modbus; shorter = "modbus";}
			if (val_anomalies < *(pt_shorter))  {pt_shorter = &val_anomalies; shorter = "anomalies";}
			tab_pck[i] = *(mappy[shorter]);
			mappy[shorter]++;
			*(pt_shorter) = mappy[shorter]->timer;
		}

	print_debog(10, "get_packet_array_slaves_anomalies : Debog 10", "cyan");	
	free(tab_pck_arp);
	free(tab_pck_icmp);
	free(tab_pck_ntp);
	free(tab_pck_snmp);
	free(tab_pck_modbus);
	free(tab_pck_anomalies);

	if (s_o_a == 's')
		{
			S->tab_pck_safe = tab_pck;
			S->nb_pck_safe= nb_pck;		
		}

	if (s_o_a == 'a')
		{
			S->tab_pck_anomalies = tab_pck;
			S->nb_pck_anomalies = nb_pck;		
		}


	print_debog(10, "get_packet_array_slaves_anomalies : Debog Out", "cyan");
}

void get_packet_array_MSF (struct Master_Slaves_Flow* MSF, char s_o_a)
{
	print_debog(5, "# get_packet_arra", "cyan");
	print_debog(5, "get_packet_array_MSF_anomalies : Debog 1", "cyan");
	int nb_pck = 0;
	struct Packet* mappy [MSF->nb_slaves + MSF->nb_attackers];
	double val_slaves [MSF->nb_slaves + MSF->nb_attackers];
	print_debog(5, "get_packet_array_MSF_anomalies : Debog 2", "cyan");
	uint16_t nb_machines = 0; 
	

	if (s_o_a == 's')
		{
			std::cout << "############### PARTIE SOA = S \n";
			for (int i = 0; i < MSF->nb_slaves; ++i)
				{
					get_packet_array_slaves(&(MSF->tab_slaves[i]), MSF, s_o_a);
					std::cout << "############################################################# ATTEND C EST LA LE PB ?? \n";
					nb_pck = nb_pck + MSF->tab_slaves[i].nb_pck_safe;
					std::cout << "########################################################### ATTEND C EST LA LE PB ?? \n";
					mappy[nb_machines] = &(MSF->tab_slaves[i].tab_pck_safe[0]);
					std::cout << "########################################################### ATTEND C EST LA LE PB ?? \n";
					val_slaves[nb_machines] = MSF->tab_slaves[i].tab_pck_safe[0].timer;
					std::cout << "########################################################### ATTEND C EST LA LE PB ?? \n";
					nb_machines++;
					std::cout << "########################################################### NEXT  ?? \n";
				}



		}

	if (s_o_a == 'a')
		{
			std::cout << "############### PARTIE SOA = A \n";
			for (int i = 0; i < MSF->nb_slaves; ++i)
				{
					get_packet_array_slaves(&(MSF->tab_slaves[i]), MSF, s_o_a);
					nb_pck = nb_pck + MSF->tab_slaves[i].nb_pck_anomalies;	
					mappy[nb_machines] = &(MSF->tab_slaves[i].tab_pck_anomalies[0]);
					val_slaves[nb_machines] = MSF->tab_slaves[i].tab_pck_anomalies[0].timer;
					nb_machines++;
				}

			for (int i = 0; i < MSF->nb_attackers; ++i)
				{
					get_packet_array_attackers(&(MSF->tab_attackers[i]), MSF, s_o_a);
					nb_pck = nb_pck + MSF->tab_attackers[i].nb_pck_anomalies;	
					mappy[nb_machines] = &(MSF->tab_attackers[i].tab_pck_anomalies[0]);
					val_slaves[nb_machines] = MSF->tab_attackers[i].tab_pck_anomalies[0].timer;
					nb_machines++;					
				}
		
		}

	std::cout << "########################################################### WTFFFF j arrive ici ? \n";
	print_debog(5, "get_packet_array_MSF_anomalies : Debog 3", "cyan");	
	struct Packet* tab_pck = (struct Packet*)malloc(nb_pck*sizeof(struct Packet));
	uint32_t shorter = 0;
	double val_shorter = 100000.00;
	print_debog(5, "get_packet_array_MSF_anomalies : Debog 5", "cyan");
	for (int i = 0; i < nb_pck; i++)
	{
		val_shorter = 100000.00;
		for (int j = 0; j < nb_machines; j++)
			{
				if (val_slaves[j] < val_shorter) {val_shorter = val_slaves[j]; shorter = j;}
			}

		tab_pck[i] = *(mappy[shorter]);
		mappy[shorter]++;
		val_slaves[shorter] = mappy[shorter]->timer;
	}
	print_debog(5, "get_packet_array_MSF_anomalies : Debog avant free", "cyan");

	if (s_o_a == 's')
		{
			for (int i = 0; i < MSF->nb_slaves; i++)
				{
					free(MSF->tab_slaves[i].tab_pck_safe);		
				}			
			}

	if (s_o_a == 'a')
		{
			for (int i = 0; i < MSF->nb_slaves; i++)
				{
					free(MSF->tab_slaves[i].tab_pck_anomalies);		
				}

			for (int i = 0; i < MSF->nb_attackers; i++)
				{
					free(MSF->tab_attackers[i].tab_pck_anomalies);		
				}			
		}
	print_debog(5, "get_packet_array_MSF_anomalies : Debog apres free", "cyan");

	if (s_o_a == 's')
		{
			MSF->tab_pck_safe = tab_pck;
			MSF->nb_pck_safe = nb_pck;			
		}

	if (s_o_a == 'a')
		{
			MSF->tab_pck_anomalies = tab_pck;
			MSF->nb_pck_anomalies = nb_pck;			
		}

	print_debog(5, "get_packet_array_MSF_anomalies : Debog OUT", "cyan");
}