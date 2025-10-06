#ifndef AFFICHE_DEBOG
#define AFFICHE_DEBOG

#include <stdio.h>
#include <string.h>
#include <stdint.h>

void print_debog(uint16_t nb_spaces, const char* text, const char* color_st) ;
void print_debog_nb(uint16_t nb_spaces, const char* text, int nb ,const char* color_st) ;

#endif