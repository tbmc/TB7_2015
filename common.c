#include "common.h"

Arbre arbreCreer()
{
    Arbre arbre = malloc(sizeof(struct Arbre));
    arbre->filsGauche = NULL;
    arbre->filsDroit = NULL;
    arbre->lexem = NULL;
    return arbre;
}

void arbreLiberer(Arbre arbre)
{
    if(arbre == NULL)
        return;
    if(arbre->lexem != NULL)
    {
        free(arbre->lexem);
    }
    arbreLiberer(arbre->filsGauche);
    arbreLiberer(arbre->filsDroit);
    free(arbre);
}

int getLexemSize(Lexem lexem)
{
    if(lexem == NULL)
        return 0;
    int i;
    for(i = 0; lexem[i].type != FIN; i++);
    return i;
}


double absolu(double d)
{
    return d < 0 ? -d : d;
}

double factoriel(double x)
{
    int result = 1;
    for(x = x ; x >= 1 ; x--)
    {
        result *= x;
    }
    return result;
}

float partieDecimal(float a) //renvoie la partie décimal ainsi que le signe d'un nombre
{
    return a - (int)a;
}

double arrinf(double a)
{
    if(a >= 0)
        return a - partieDecimal(a);
    else
        return a - (1 - partieDecimal(a));
}

double arrsup(double a)
{
    if(a >= 0)
        return a + (1 - partieDecimal(a));
    else
        return a + partieDecimal(a);
}

