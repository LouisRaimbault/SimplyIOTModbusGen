#include "Get_And_Transforme_Data.h"
#include "Generateur_Donnees.h"
#include "Usefull_Fonctions.h"
#include <string>


void libere_data (struct Data* D)
{
	for (int i = 0; i < D->nb_pck; i++)
		{
			free(D->tab_packet_0[i].mac_src);
			free(D->tab_packet_0[i].mac_dst);
			free(D->tab_packet_0[i].ip_src);
			free(D->tab_packet_0[i].ip_dst);
			free(D->tab_packet_0[i].protocol);
		}
	free(D->tab_packet_0);
	free(D->tab_packet_1);	
	free(D);
}

int get_nb_pck_files(const char* filename) 
{
    FILE *fichier = fopen(filename, "r");
    if (fichier == NULL) 
    {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;  // Code d'erreur
    }

    int nb_pck = 0;
    int caractere;

    while ((caractere = fgetc(fichier)) != EOF) 
    {
        if (caractere == '\n') 
        {
            nb_pck++;
        }
    }

    fclose(fichier);
    return nb_pck-2;
}



void get_pck0_tab (struct Data* D, const char* filename)
{
	D->nb_pck = get_nb_pck_files(filename)-1;
	char buf [1000];
	int y = 0;

    FILE *fichier = fopen(filename, "r");
    if (fichier == NULL) 
    {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    int nb_pck = 0;
    int c = fgetc(fichier);
    D->tab_packet_0 = (struct Packet_0*)malloc(D->nb_pck*sizeof(struct Packet_0));
    while (c != '\n') {c = fgetc(fichier);}
    for (int i = 0; i < D->nb_pck; i++)
    	{
    		c = fgetc(fichier);
    		while (c != ',') {c = fgetc(fichier);}  // id packet pas besoin 

    		c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // Timer
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].timer = atof(buf);
			
			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // MAC SRC
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].mac_src = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].mac_src, buf);
			
			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // MAC DST
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].mac_dst = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].mac_dst, buf);

			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // IP SRC
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].ip_src = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].ip_src, buf);		


			c = fgetc(fichier);    	
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // IP 
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].ip_dst = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].ip_dst, buf);   

			c = fgetc(fichier); 
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].pck_length = (uint16_t)atoi(buf);
		

			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].port_src = (uint16_t)atoi(buf);


			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].port_dst = (uint16_t)atoi(buf);


			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].tra = (uint16_t)atoi(buf);	


			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].direction = (uint16_t)atoi(buf);


			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].charge_utile = (uint16_t)atoi(buf);


			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // IP 
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].protocol = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].protocol, buf);

			c = fgetc(fichier);
			while (c != '\n') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].label = (uint16_t)atoi(buf);
			
    	}
}

int parse_mac(const char *str, uint8_t mac[6]) 
{
    // sscanf lit les valeurs hexadécimales
    int n = sscanf(str,
                   "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
                   &mac[0], &mac[1], &mac[2],
                   &mac[3], &mac[4], &mac[5]);

    if (n == 6) {return 1;} // succès si 6 champs lus
    return 0;
}

int get_mac_order(const char* mac1_str, const char* mac2_str) 
{
    uint8_t mac1[6], mac2[6];

    if (!parse_mac(mac1_str, mac1)) 
    {
        fprintf(stderr, "MAC invalide: %s\n", mac1_str);
        return 1;
    }
    if (!parse_mac(mac2_str, mac2)) 
    {
        fprintf(stderr, "MAC invalide: %s\n", mac2_str);
        return 1;
    }


    int cmp = memcmp(mac1, mac2, 6);



    if (cmp < 0) 
    {
    	return 1;
    } 

    return 2;
}

char set_port_groupe (uint16_t port)
{
	if (port == 0) {return 'z';}
	if (port < 1023) {return 'a';}
	if (port < 25000) {return 'b';}
	if (port < 49151) {return 'c';}
	return 'd';
}

void get_symbole (struct Packet_1* pck1, struct mappy_label* ml)
{
	std::string st = std::to_string(pck1->ts); 
	st = st + std::to_string(pck1->pck_length);
	st = st + std::to_string(pck1->port_src);
	st = st + std::to_string(pck1->port_dst);
	st = st + std::to_string(pck1->tra);
	st = st + std::to_string(pck1->direction);

	if (ml->mappy->operator[](st) == 0)
		{
			ml->mappy->operator[](st) = ml->cursymb;
			ml->cursymb = ml->cursymb +1;
		}

	pck1->symbole = ml->mappy->operator[](st);
}

void get_pck1_tab (struct Data* D, struct mappy_label* ml)
{	
	
	D->tab_packet_1 = (struct Packet_1*)malloc(D->nb_pck*sizeof(struct Packet_1));
	std::unordered_map<std::string, struct conversation_values*> mappy;
	struct conversation_values* cv;
	for (int i = 0; i < D->nb_pck; i++)
		{   
			int mac_order = get_mac_order(D->tab_packet_0[i].mac_src, D->tab_packet_0[i].mac_dst);
			std::string st = "";
			if (mac_order == 1)
				{
					st = st + std::string(D->tab_packet_0[i].mac_src);
					st = st + std::to_string(D->tab_packet_0[i].port_src);
					st = st + std::string(D->tab_packet_0[i].mac_dst);					
					st = st + std::to_string(D->tab_packet_0[i].port_dst);
					st = st + std::to_string(D->tab_packet_0[i].tra);					
				}
			if (mac_order == 2)
				{
					st = st + std::string(D->tab_packet_0[i].mac_dst);
					st = st + std::to_string(D->tab_packet_0[i].port_dst);
					st = st + std::string(D->tab_packet_0[i].mac_src);					
					st = st + std::to_string(D->tab_packet_0[i].port_src);
					st = st + std::to_string(D->tab_packet_0[i].tra);						
				}
			if (mappy[st] == NULL)
				{
					mappy[st] = (struct conversation_values*)malloc(sizeof(struct conversation_values));
					cv = mappy[st];
					cv->somme_timer = 0.00;
					cv->avg_timer = 0.00;
					cv->nb_pck = 0;
					cv->last_timer = 0.00;
					cv->somme_pck_length = 0;
					cv->somme_charge_utile = 0;
				}

			
			cv = mappy[st];
			D->tab_packet_1[i].ts_d = D->tab_packet_0[i].timer - cv->last_timer;
			cv->last_timer = D->tab_packet_0[i].timer;
			cv->somme_timer = cv->somme_timer + D->tab_packet_1[i].ts_d;
			cv->nb_pck = cv->nb_pck + 1;
			cv->somme_pck_length = cv->somme_pck_length + D->tab_packet_0[i].pck_length;
			cv->somme_charge_utile = cv->somme_charge_utile + D->tab_packet_0[i].charge_utile;
			D->tab_packet_1[i].port_src = set_port_groupe(D->tab_packet_0[i].port_src);
			D->tab_packet_1[i].port_dst = set_port_groupe(D->tab_packet_0[i].port_dst);
			D->tab_packet_1[i].ip_src = D->tab_packet_0[i].ip_src;
			D->tab_packet_1[i].ip_dst = D->tab_packet_0[i].ip_dst;
			D->tab_packet_1[i].tra = D->tab_packet_0[i].tra;
			D->tab_packet_1[i].direction = D->tab_packet_0[i].direction;
			D->tab_packet_1[i].label = D->tab_packet_0[i].label;
			D->tab_packet_1[i].protocol = D->tab_packet_0[i].protocol;
		}


	for (std::unordered_map<std::string, struct conversation_values*>::iterator it = mappy.begin(); it != mappy.end(); it++)
		{
			it->second->avg_pck_length = it->second->somme_pck_length/it->second->nb_pck;
			it->second->avg_charge_utile = it->second->somme_charge_utile/it->second->nb_pck;
			it->second->avg_timer = it->second->somme_timer/(double)(it->second->nb_pck);
		}

	for (int i = 0; i < D->nb_pck; i++)
		{   
			int mac_order = get_mac_order(D->tab_packet_0[i].mac_src, D->tab_packet_0[i].mac_dst);
			std::string st = "";
			if (mac_order == 1)
				{
					st = st + std::string(D->tab_packet_0[i].mac_src);
					st = st + std::to_string(D->tab_packet_0[i].port_src);
					st = st + std::string(D->tab_packet_0[i].mac_dst);					
					st = st + std::to_string(D->tab_packet_0[i].port_dst);
					st = st + std::to_string(D->tab_packet_0[i].tra);					
				}
			if (mac_order == 2)
				{
					st = st + std::string(D->tab_packet_0[i].mac_dst);
					st = st + std::to_string(D->tab_packet_0[i].port_dst);
					st = st + std::string(D->tab_packet_0[i].mac_src);					
					st = st + std::to_string(D->tab_packet_0[i].port_src);
					st = st + std::to_string(D->tab_packet_0[i].tra);						
				}
			cv = mappy[st];
			D->tab_packet_1[i].ts = 'a';
			if (D->tab_packet_1[i].ts_d > cv->avg_timer) {D->tab_packet_1[i].ts = 'b';};
			D->tab_packet_1[i].pck_length = 'a';
			if (D->tab_packet_0[i].pck_length > cv->avg_pck_length) {D->tab_packet_1[i].pck_length = 'b';}
			D->tab_packet_1[i].charge_utile = 'b';
			if (D->tab_packet_0[i].charge_utile > cv->avg_charge_utile) {D->tab_packet_1[i].charge_utile = 'c';}
			if (D->tab_packet_0[i].charge_utile == 0) {D->tab_packet_1[i].charge_utile = 'a';}
			get_symbole(&(D->tab_packet_1[i]), ml);
		}

	for (auto it = mappy.begin(); it != mappy.end(); it++)
		{
			free(it->second);
		}
}

void export_packets_symb (struct Data* D, const char* pathout)
{
	  std::ofstream fichier (pathout); // ouvre en écriture par défaut
	  fichier << "Id Packet,timestamp,mac_src,mac_dst,ip_src,ip_dst,pck_length,port_src,port_dst,tra,direction,charge_utile,protocol,symbole,label\n";
	  for (int i = 0; i < D->nb_pck; i++)
    	{	
    		fichier << i << ",";
    		fichier << D->tab_packet_0[i].timer  << ",";
			fichier << D->tab_packet_0[i].mac_src << ",";
			fichier << D->tab_packet_0[i].mac_dst << ",";
			fichier << D->tab_packet_0[i].ip_src << ",";
			fichier << D->tab_packet_0[i].ip_dst << ",";
			fichier << D->tab_packet_0[i].pck_length << ",";
			fichier << D->tab_packet_0[i].port_src << ",";
			fichier << D->tab_packet_0[i].port_dst << ",";
			fichier << D->tab_packet_0[i].tra << ",";
			fichier << D->tab_packet_0[i].direction << ",";
			fichier << D->tab_packet_0[i].charge_utile << ",";
			fichier << D->tab_packet_0[i].protocol << ",";
			fichier << D->tab_packet_1[i].symbole << ",";
			fichier << D->tab_packet_0[i].label <<"\n";
    	}
    
    fichier.close(); //  le fichier
    print_debog(1, pathout, "gray");
    return ;

}

void apply_transformation (const char* filename_input_safe, const char* filename_input_anomalies, const char* pathout)
{
	std::cout << "Welcome to Data Transformation ...\n";
	struct mappy_label ml;
	ml.mappy = new std::unordered_map<std::string, uint32_t>;
	ml.cursymb = 1;

	struct Data* DSafe = (struct Data*)malloc(sizeof(struct Data));
	get_pck0_tab(DSafe, filename_input_safe);
	get_pck1_tab (DSafe, &ml);
	char* pathout_safe;
	asprintf(&pathout_safe, "%s_safe.txt", pathout);
	export_packets_symb(DSafe, pathout_safe);
	libere_data(DSafe);
	free(pathout_safe);
	struct Data* Danomalies = (struct Data*)malloc(sizeof(struct Data));
	get_pck0_tab(Danomalies, filename_input_anomalies);
	get_pck1_tab (Danomalies, &ml);
	char* pathout_anomalies;
	asprintf(&pathout_anomalies, "%s_anomalies.txt", pathout);
	export_packets_symb(Danomalies, pathout_anomalies);
	libere_data(Danomalies);
	free(pathout_anomalies);
	delete ml.mappy;
	return ;
}