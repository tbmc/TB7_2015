#ifndef ANALYSE_LEXICALE_H_INCLUDED
#define ANALYSE_LEXICALE_H_INCLUDED
#include "common.h"

#define MAX_LEXEM               (1000)
#define POINTEUR_NULL           (101)
#define ECHEC_MALLOC            (102)
#define FONCTION_TROP_LONGUE    (103)
#define FORMULE_INCONNUE        (104)
#define VIRGULE_EN_TROP         (105)
#define EXPRESSION_INCONNUE     (106)

Lexem AnalyseLexicale(const char* tab, int* error);

#endif // ANALYSE_LEXICALE_H_INCLUDED
