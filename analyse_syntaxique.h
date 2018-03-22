#ifndef ANALYSESYNTAXIQUE
#define ANALYSESYNTAXIQUE

#include "common.h"
#include "erreur.h"

Arbre analyseSyntaxique(Lexem lexem, int *error);

// Uniquement pour le debug
void afficheLexem(Lexem l, int size);

#endif // ANALYSESYNTAXIQUE

