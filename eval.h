#ifndef EVAL_H
#define EVAL_H

#include <math.h>
#include "common.h"

#include <stdio.h>
#include "erreur.h"

Point eval(Arbre arbre, float x, int *error);
float evaluation_arbre(Arbre arbre, float x, int *error);
float fonction(TypeLexem type, float a, int *error);
float operation_eval(TypeLexem type, float a, float b, int *error);


#endif // EVAL_H

