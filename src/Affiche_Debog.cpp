#include "Affiche_Debog.h"

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
