#include "Generateur_Donnees.h"
#include "Usefull_Fonctions.h"
#include "Traffic_Creator.h"
#include "Anomalies.h"
#include "Get_And_Transforme_Data.h"




int main(int argc, char** argv)
{
	srandom(time(NULL));
	print_debog(0,"Welcome to the SimplyIOTModbusGen program : ", "white");
	print_debog(0,"Please, set 1 to generate data, or 2 to generate symbols from data ...", "white");
	uint16_t choice;
	selection_valeur_manuelle(&choice, "uint16_t", "Utilisation du Software : ");
	
	
	if (choice == 1)
		{
			print_debog(1,"Main : Welcome to the data generator prog", "white");
			struct Modbus_Network MN;
			selection_valeur_manuelle(&(MN.duration_scenario), "double", "MN.duration_scenario");
			selection_valeur_manuelle(&(MN.ratio_duration), "double", "MSF.ratio_duration");
			MN.duration_scenario = MN.duration_scenario * MN.ratio_duration;
			Generateur_Donnnees_MN(&MN);
			std::cout << "End Prog \n";	
		}

	if (choice == 2)
		{
			clean_stdin();
			print_debog(1,"Main : Welcome to the data gestion prog ", "white");
			//char* safe_input = "/home/Louis_RAIMBAULT/Bureau/Generateur_Donnees_Modbus/V2/Alex_network_safe.txt";
			//char* anomalie_input = "/home/Louis_RAIMBAULT/Bureau/Generateur_Donnees_Modbus/V2/Alex_network_anomalies.txt";
			//char* pathout = "/home/Louis_RAIMBAULT/Bureau/Generateur_Donnees_Modbus/V2/Wololo";
			char* safe_input = NULL;
			char* anomalie_input = NULL;
			char* pathout = NULL;
			
			selection_string_manuelle(&safe_input, "string", "safe data ");
			selection_string_manuelle(&anomalie_input, "string", "anomalies data ");
			selection_string_manuelle(&pathout, "string", "path out (with no extention, (txt will be attribued) ");
			apply_transformation (safe_input, anomalie_input, pathout);
		}


	return 0;

}