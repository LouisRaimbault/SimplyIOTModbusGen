#include "Usefull_Fonctions.h"


void clean_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void selection_valeur_manuelle(void* valeur, const char* type, const char* nom_valeur)
{
    if (strcmp(type, "double") == 0) 
    {
        printf("please set value %s (double) : ", nom_valeur);
        scanf("%lf", (double*)valeur);
        printf("value  : %.2f\n", *(double*)valeur);
        return;
    }

    if (strcmp(type, "uint16_t") == 0) 
    {
        printf("please set value %s (uint16_t) : ", nom_valeur);
        scanf("%hu", (uint16_t*)valeur);
        printf("value : %hu\n", *(uint16_t*)valeur);
        return;
    }

}

void selection_string_manuelle(char** str, const char* type, const char* nom_valeur)
{
    if (strcmp(type, "string") == 0)
    {
        char buffer[256];
        int c;

        printf("please set str %s (string) : ", nom_valeur);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        // alloue dynamiquement et renvoie le pointeur
        *str = strdup(buffer);

        printf("str : %s\n", (char*)(*str));
        return;
    }
}

void print_debog(uint16_t nb_spaces, const char* text, const char* color_st) 
{
    const char* color = "\033[0m"; // couleur par défaut

    if (strcmp(color_st, "bleu") == 0) {color = "\033[34m";} 
    else if (strcmp(color_st, "vert") == 0) {color = "\033[32m";}
    else if (strcmp(color_st, "rouge") == 0) {color = "\033[31m";}
    else if (strcmp(color_st, "jaune") == 0) {color = "\033[33m";}
    else if (strcmp(color_st, "cyan") == 0) {color = "\033[36m";}
    else if (strcmp(color_st, "blanc") == 0) {color = "\033[37m";}

    printf("%s%*s%s\033[0m\n", color, nb_spaces, "", text); 
}

void print_debog_nb(uint16_t nb_spaces, const char* text, int nb ,const char* color_st) 
{
    const char* color = "\033[0m"; // couleur par défaut

    if (strcmp(color_st, "bleu") == 0) {color = "\033[34m";} 
    else if (strcmp(color_st, "vert") == 0) {color = "\033[32m";}
    else if (strcmp(color_st, "rouge") == 0) {color = "\033[31m";}
    else if (strcmp(color_st, "jaune") == 0) {color = "\033[33m";}
    else if (strcmp(color_st, "cyan") == 0) {color = "\033[36m";}
    else if (strcmp(color_st, "blanc") == 0) {color = "\033[37m";}

     printf("%s%*s%s%u\033[0m\n", color, nb_spaces, "", text, nb);
}
