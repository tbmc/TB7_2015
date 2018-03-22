#ifndef ERREUR_H_INCLUDED
#define ERREUR_H_INCLUDED


#define POINTEUR_NULL           (101)
#define ECHEC_MALLOC            (102)
#define FONCTION_TROP_LONGUE    (103)
#define FORMULE_INCONNUE        (104)
#define VIRGULE_EN_TROP         (105)
#define EXPRESSION_INCONNUE     (106)


#define ERREUR_PARENTHESES      (201)


#define ErreurStructArbre       (300)
#define DivisionPar0            (301)
#define ModuloPar0              (302)
#define FactorielleVirgule      (303)
#define ErreurLN                (304)
#define ErreurTAN               (305)
#define ErreurRacineCarre       (306)
#define ModuloVirgule           (307)
#define ErreurASIN              (308)
#define ErreurType              (309)
#define ErreurATAN              (310)
#define ErreurACOS              (311)
#define ErreurTANH              (312)
#define ErreurFactoriel         (313)

char* texte_erreur(int erreur);

#endif // ERREUR_H_INCLUDED
