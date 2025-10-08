#ifndef USEFULL_FONCTIONS
#define USEFULL_FONCTIONS

#include <stdio.h>
#include <string.h>
#include <stdint.h>

void clean_stdin();
void selection_valeur_manuelle(void* valeur, const char* type, const char* nom_valeur);
void selection_string_manuelle(char** str, const char* type, const char* nom_valeur);
void print_debog(uint16_t nb_spaces, const char* text, const char* color_st) ;
void print_debog_nb(uint16_t nb_spaces, const char* text, int nb ,const char* color_st) ;

#endif