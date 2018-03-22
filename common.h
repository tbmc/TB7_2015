#ifndef COMMON_H
#define COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <math.h>

#define bool    int
#define TRUE    (1)
#define FALSE   (0)
#define true    TRUE
#define false   FALSE

#ifndef NULL
#define NULL (0)
#endif

#define PI_VALUE (3.14125)

typedef struct
{
    float x, y;
}
Point;

typedef enum
{
    FIN = 0, VARIABLE = 1,
    PARENTHESE_OUVRANTE = 2,
    PARENTHESE_FERMANTE = 3,
    PLUS = 4, MOINS = 5, MULTIPLIER = 6, DIVISER = 7, MODULO = 8,
    POW = 9,

    SIN = 51, COS = 52, TAN = 53, LN = 54, LOG = 55,
    SQRT = 56, EXP = 57, ABS = 58, ASIN = 59, ACOS = 60,
    SINH = 61, COSH = 62, ATAN = 63, TANH = 64,
    FACTORIELLE = 65, ARRINF = 66, ARRSUP = 67,

    FIN_FONCTIONS = 68,


    PI = 135, VALUE = 136
}
TypeLexem;

struct Lexem
{
    TypeLexem type;
    float value;
};

struct Arbre
{
    struct Lexem *lexem;
    struct Arbre *filsGauche, *filsDroit;
};

typedef struct Lexem * Lexem;
typedef struct Arbre * Arbre;

Arbre arbreCreer();
void arbreLiberer(Arbre arbre);

int getLexemSize(Lexem lexem);


#define NAME_LENGHT (10)

struct Ligne
{
    char n[NAME_LENGHT];
    double (*f)(double);
};
typedef struct Ligne Ligne;

double factoriel(double x);
double absolu(double d);
double arrinf(double a);
double arrsup(double a);

static const Ligne tab_functions[] =
{
    {"sin", sin}, {"cos", cos}, {"tan", tan},
    {"ln", log}, {"log", log10}, {"sqrt", sqrt},
    {"exp", exp}, {"abs", absolu}, {"asin", asin},
    {"acos", acos}, {"sinh", sinh}, {"cosh", cosh},
    {"atan", atan}, {"tanh", tanh}, {"!", factoriel},
    {"arrinf", arrinf}, {"arrsup", arrsup}
};
static const char tab_operators[] = "\0x()+-*/%^";

#ifdef __cplusplus
}
#endif

#endif // COMMON_H

