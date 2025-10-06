#include "Generateur_Donnees.h"
#include "Affiche_Debog.h"
#include "Traffic_Creator.h"
#include "Anomalies.h"
#include "Get_And_Transforme_Data.h"

int main (int argc, char** argv)
{
	srandom(time(NULL));
	
	if (strcmp(argv[1], "generator") == 0)
		{
			print_debog(1,"Main : Welcome to the data generator prog", "white");
			struct Modbus_Network MN;
			selection_valeur_manuelle(&(MN.duration_scenario), "double", "MN.duration_scenario");
			selection_valeur_manuelle(&(MN.ratio_duration), "double", "MSF.ratio_duration");
			MN.duration_scenario = MN.duration_scenario * MN.ratio_duration;
			Generateur_Donnnees_MN(&MN);
			std::cout << "End Prog \n";	
		}

	if (strcmp(argv[1], "data") == 0)
		{

			print_debog(1,"Main : Welcome to the data gestion prog ", "white");
			char* safe_input = "/home/Louis_RAIMBAULT/Bureau/Generateur_Donnees_Modbus/V2/pomme_network_safe.txt";
			char* anomalie_input = "/home/Louis_RAIMBAULT/Bureau/Generateur_Donnees_Modbus/V2/pomme_network_anomalies.txt";
			char* pathout = "/home/Louis_RAIMBAULT/Bureau/Generateur_Donnees_Modbus/V2/Wololo";
			//char* safe_input;
			//char* anomalie_input;
			//char* pathout;
			//selection_string_manuelle(&safe_input, "string", "safe data ");
			//selection_string_manuelle(&safe_input, "string", "safe data ");
			//selection_string_manuelle(&pathout, "string", "path out (with no extention, (txt will be attribued) ");
			apply_transformation (safe_input, anomalie_input, pathout);
		}


	return 0;

}