#include "analyse_syntaxique.h"

#include <string.h>
#include <stdio.h>

#define MAX_ARRAY_SIZE (500)

#define isOperatorP(x)      (x == PLUS || x == MOINS)
#define isOperatorM(x)      (x == MULTIPLIER || x == DIVISER || \
    x == MODULO)
#define isOperatorPow(x)    (x == POW)
#define isVar(x)            (x == VARIABLE || x == VALUE || x == PI)
#define isOperator(x)       ((PLUS <= x && x <= POW) || (x == PARENTHESE_FERMANTE) \
    || (x == PARENTHESE_OUVRANTE) || (x == FIN))

typedef enum
{
    PLUS_MOINS = 0, MULTI_DIV_MOD = 1, POW_PASSE = 2, FONCT = 3, VAR = 4, SEARCH_END = 5
}
Passe;

bool isFunction(TypeLexem type)
{
    if((int)SIN <= type && type <= (int)FIN_FONCTIONS)
        return true;
    return false;
}

int lastOperation(Lexem lexem, int size, Passe passe, int *error)
{
    if(passe == SEARCH_END)
        return -1;
    int p = 0, i;
    //for(i = 0; i < size; i++)
    for(i = size - 1; i >=0; i--)
    {
        if(lexem[i].type == PARENTHESE_OUVRANTE)
            p++; // Si on est dans une parenthèse
        else if(lexem[i].type == PARENTHESE_FERMANTE)
            p--; // Si on sort d'une parenthèse
        else if(p == 0)
        {
            // Si on n'est pas dans une parenthèse
            switch(passe)
            {
            case PLUS_MOINS:
                if(isOperatorP(lexem[i].type))
                    return i;
                break;
            case MULTI_DIV_MOD:
                if(isOperatorM(lexem[i].type))
                    return i;
                break;
            case POW_PASSE:
                if(isOperatorPow(lexem[i].type))
                    return i;
                break;
            case FONCT:
                if(isFunction(lexem[i].type))
                    return i;
                break;
            case VAR:
                if(isVar(lexem[i].type))
                    return i;
                break;
            default:
                return -1;
            }
        }
    }
    if(p != 0)
    {
        *error = ERREUR_PARENTHESES;
        return -1;
    }
    return lastOperation(lexem, size, passe+1, error);
}

Lexem lexem_copy(Lexem lexem)
{
    Lexem l = malloc(sizeof(struct Lexem));
    l->type = lexem->type;
    l->value = lexem->value;
    return l;
}

bool needMultiplication(TypeLexem t1, TypeLexem t2)
{
    if(t1 == PARENTHESE_FERMANTE && t2 == PARENTHESE_OUVRANTE)
        return true;
    if((isVar(t1) && (!isOperator(t2))) || (isVar(t2) && (!isOperator(t1))))
        return true;
    if((isVar(t1) && t2 == PARENTHESE_OUVRANTE) || (t1 == PARENTHESE_FERMANTE && isVar(t2)))
        return true;
    if(isVar(t1) && isVar(t2))
        return true;
    return false;
}

Lexem analyseSyntax(Lexem in, int size_in, int *size_out)
{
    int tab[MAX_ARRAY_SIZE];
    int i = 0, j = 0, x = 0, xi = 0;
    for(i = 0; i < size_in; i++)
    {
        // détecter multiplier invisible
        // et l'ajouter la dedans!
        if(i+1 < size_in && needMultiplication(in[i].type, in[i+1].type))
            tab[x++] = i;
    }

    Lexem out = malloc(sizeof(struct Lexem) * (size_in + x + 1));
    for(i = 0; i < size_in; i++)
    {
        out[j].type = in[i].type;
        out[j].value = in[i].value;
        j++;
        if(xi < x && tab[xi] == i)
        {
            out[j].type = MULTIPLIER;
            out[j].value = 0;
            j++;
            xi++;
        }
    }
    out[j].type = FIN;
    out[j].value = 0;
    *size_out = size_in + x;
    return out;
}

Arbre analyseToArbre(Lexem lexem, int size, int *error)
{
    if(lexem == NULL || size < 1)
        return NULL;
    // On supprime les parenthèses si il y en a qui entourent le lexem
    if(lexem[0].type == PARENTHESE_OUVRANTE && lexem[size - 1].type == PARENTHESE_FERMANTE)
    {
        int pa = 0, i;
        bool coupe = true;
        for(i = 0; i < size && 0 <= pa; i++)
        {
            if(lexem[i].type == PARENTHESE_OUVRANTE)
                pa++;
            if(lexem[i].type == PARENTHESE_FERMANTE)
            {
                pa--;
                if(pa <= 0 && i < size - 1)
                {
                    coupe = false;
                    break;
                }
            }

        }
        //if(!(pa < 0) && coupe)
        if(pa == 0 && coupe)
            return analyseToArbre(lexem + 1, size - 2, error);
    }

    // A vérifier!
    if(isOperatorP(lexem[0].type))
    {
        lexem = lexem + 1;
        size--;
    }

    Arbre arbre = arbreCreer();
    int lo = lastOperation(lexem, size, PLUS_MOINS, error);

    if(*error != 0)
        return NULL;
    if(lo == -1)
        return NULL;
    arbre->lexem = lexem_copy(lexem + lo);

    // Le cas où l'on a plus que un lexem
    if(size == 1)
        return arbre;

    // Si c'est le premier lexem, on ne met pas de filsGauche
    // Puis on sépare le lexem en deux enfants
    if(lo == 0)
        arbre->filsGauche = NULL;
    else
    {
        arbre->filsGauche = analyseToArbre(lexem, lo, error);
        if(*error != 0)
            return NULL;
    }

    arbre->filsDroit = analyseToArbre(lexem + lo + 1, size - lo - 1, error);
    if(*error != 0)
        return NULL;

    return arbre;
}

Arbre analyseSyntaxique(Lexem lexem, int *error)
{
    int size;
    Lexem l2 = analyseSyntax(lexem, getLexemSize(lexem), &size);
    Arbre arbre = analyseToArbre(l2, size, error);
    free(l2);
    return arbre;
}

void afficheLexem(Lexem l, int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
        printf("%d(%d) ", l[i].type, (int)(l[i].value));
    }
    printf("\n");
}





