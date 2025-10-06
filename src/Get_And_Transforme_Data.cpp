#include "Get_And_Transforme_Data.h"
#include "Generateur_Donnees.h"
#include "Affiche_Debog.h"
#include <string>



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
			//std::cout << "timer = " << D->tab_packet_0[i].timer << "\n";
			
			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // MAC SRC
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].mac_src = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].mac_src, buf);
			//std::cout << "mac_src = " << D->tab_packet_0[i].mac_src << "\n";
			
			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // MAC DST
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].mac_dst = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].mac_dst, buf);
			//std::cout << "mac_dst = " << D->tab_packet_0[i].mac_dst << "\n";

			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // IP SRC
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].ip_src = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].ip_src, buf);		
			//std::cout << "mac_ip_src = " << D->tab_packet_0[i].ip_src << "\n";


			c = fgetc(fichier);    	
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // IP 
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].ip_dst = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].ip_dst, buf);   
			//std::cout << "ip_dst = " << D->tab_packet_0[i].ip_dst << "\n";

			c = fgetc(fichier); 
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].pck_length = (uint16_t)atoi(buf);
			//std::cout << "pck length = " << D->tab_packet_0[i].pck_length << "\n";			

			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].port_src = (uint16_t)atoi(buf);
			//std::cout << "port_src = " << D->tab_packet_0[i].port_src << "\n";

			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].port_dst = (uint16_t)atoi(buf);
			c = fgetc(fichier);	
			//std::cout << "port_dst = " << D->tab_packet_0[i].port_dst << "\n";

			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].tra = (uint16_t)atoi(buf);	
			//std::cout << "tra = " << D->tab_packet_0[i].tra << "\n";

			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].direction = (uint16_t)atoi(buf);
			//std::cout << "direction = " << D->tab_packet_0[i].direction << "\n";	

			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].charge_utile = (uint16_t)atoi(buf);
			//std::cout << "charge utile " << D->tab_packet_0[i].charge_utile << "\n";

			c = fgetc(fichier);
			while (c != ',') {buf[y++] = c; c = fgetc(fichier);} // IP 
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].protocol = (char*)malloc(strlen(buf) + 1);
			strcpy(D->tab_packet_0[i].protocol, buf);
			//std::cout << "protocol = " << D->tab_packet_0[i].protocol << "\n";


			c = fgetc(fichier);
			while (c != '\n') {buf[y++] = c; c = fgetc(fichier);}
			buf[y] = '\0'; y = 0;
			D->tab_packet_0[i].label = (uint16_t)atoi(buf);
			//std::cout << "label = " << D->tab_packet_0[i].label << "\n";
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

char* concat_mac_order(const char* mac1_str, const char* mac2_str) 
{
    uint8_t mac1[6], mac2[6];

    if (!parse_mac(mac1_str, mac1)) 
    {
        fprintf(stderr, "MAC invalide: %s\n", mac1_str);
        return NULL;
    }
    if (!parse_mac(mac2_str, mac2)) 
    {
        fprintf(stderr, "MAC invalide: %s\n", mac2_str);
        return NULL;
    }


    int cmp = memcmp(mac1, mac2, 6);

    const char *first;
    const char *second;

    if (cmp < 0) 
    {
        first = mac1_str;
        second = mac2_str;
    } 
    else 
    {
        first = mac2_str;
        second = mac1_str;
    }

    size_t len = strlen(first) + strlen(second) + 2; // +1 séparateur +1 '\0'
    char *result = (char*)malloc(len*sizeof(char));
    if (!result) return NULL;

    snprintf(result, len, "%s-%s", first, second);
    return result;
}

char set_port_groupe (uint16_t port)
{
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
	print_debog(10, "get_pck1_tab (1) : ", "cyan");
	D->tab_packet_1 = (struct Packet_1*)malloc(D->nb_pck*sizeof(struct Packet_1));
	std::unordered_map<std::string, struct conversation_values*> mappy;
	struct conversation_values* cv;
	std::string st = "";
	print_debog(10, "get_pck2_tab (2) : ", "cyan");
	for (int i = 0; i < D->nb_pck; i++)
		{   
			std::cout << "i = " << i << " et nb pck = " << D->nb_pck << "\n";
			st = std::string (concat_mac_order(D->tab_packet_0[i].mac_src, D->tab_packet_0[i].mac_dst));
			if (mappy[st] == NULL)
				{
					mappy[st] = (struct conversation_values*)malloc(sizeof(struct conversation_values));
					cv = mappy[st];
					cv->nb_pck = 0;
					cv->last_timer = 0.00;
					cv->somme_pck_length = 0;
					cv->somme_charge_utile = 0;
				}
			std::cout << "aa \n";
			cv = mappy[st];
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
			std::cout << "bb \n";
		}

	print_debog(10, "get_pck2_tab : (3) ", "cyan");
	for (std::unordered_map<std::string, struct conversation_values*>::iterator it = mappy.begin(); it != mappy.end(); it++)
		{
			it->second->avg_pck_length = it->second->somme_pck_length/it->second->nb_pck;
			it->second->avg_charge_utile = it->second->somme_charge_utile/it->second->nb_pck;
		}

	for (int i = 0; i < D->nb_pck; i++)
		{   
			st = std::string (concat_mac_order(D->tab_packet_0[i].mac_src, D->tab_packet_0[i].mac_dst));
			cv = mappy[st];
			D->tab_packet_1[i].ts = D->tab_packet_0[i].timer - cv->last_timer;
			cv->last_timer = D->tab_packet_0[i].timer;
			D->tab_packet_1[i].pck_length = 'a';
			if (D->tab_packet_0[i].pck_length > cv->avg_pck_length) {D->tab_packet_1[i].pck_length = 'b';}
			D->tab_packet_1[i].charge_utile = 'a';
			if (D->tab_packet_0[i].charge_utile > cv->avg_charge_utile) {D->tab_packet_1[i].charge_utile = 'b';}
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
    std::cout << "Fichier bien écrit , succes \n";
    fichier.close(); //  le fichier
    return ;

}

void apply_transformation (const char* filename_input_safe, const char* filename_input_anomalies, const char* pathout)
{
	std::cout << "Welcome to apply transformation\n";
	struct mappy_label ml;
	ml.mappy = new std::unordered_map<std::string, uint32_t>;
	ml.cursymb = 0;

	struct Data* DSafe = (struct Data*)malloc(sizeof(struct Data));
	std::cout << "apply transformation 1\n";
	get_pck0_tab(DSafe, filename_input_safe);
	std::cout << "apply transformation 2\n";
	get_pck1_tab (DSafe, &ml);
	std::cout << "apply transformation 3\n";
	char* pathout_safe;
	asprintf(&pathout_safe, "%s_safe.txt", pathout);
	export_packets_symb(DSafe, pathout_safe);
	free(pathout_safe);
	struct Data* Danomalies = (struct Data*)malloc(sizeof(struct Data));
	get_pck0_tab(Danomalies, filename_input_anomalies);
	get_pck1_tab (Danomalies, &ml);
	char* pathout_anomalies;
	asprintf(&pathout_anomalies, "%s_anomalies.txt", pathout);
	export_packets_symb(Danomalies, pathout_anomalies);
	free(pathout_anomalies);
	delete ml.mappy;
	return ;
}