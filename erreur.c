#include "erreur.h"

char* texte_erreur(int erreur)
{
    switch(erreur)
    {
    case 0 :
    {
        return "Aucune erreur";
        break;
    }
    case POINTEUR_NULL :
    {
        return "Entree nulle";
        break;
    }
    case ECHEC_MALLOC :
    {
        return "Echec de malloc (plus de memoire sur la machine)";
        break;
    }
    case FONCTION_TROP_LONGUE :
    {
        return "Fonction trop longue pour etre calcule";
        break;
    }
    case FORMULE_INCONNUE :
    {
        return "Formule inconnue";
        break;
    }
    case VIRGULE_EN_TROP :
    {
        return "Virgule en trop";
        break;
    }
    case EXPRESSION_INCONNUE :
    {
        return "Expression inconnue";
        break;
    }
    case ERREUR_PARENTHESES:
    {
        return "Erreur de parentheses";
        break;
    }
    case ErreurStructArbre :
    {
        return "Erreur de structure de l'arbre";
        break;
    }
    case DivisionPar0 :
    {
        return "Division par 0";
        break;
    }
    case ModuloPar0 :
    {
        return "Modulo par 0";
        break;
    }
    case FactorielleVirgule :
    {
        return "Factorielle Virgule";
        break;
    }
    case ErreurLN :
    {
        return "Erreur LN";
        break;
    }
    case ErreurTAN :
    {
        return "Erreur TAN";
        break;
    }
    case ErreurRacineCarre :
    {
        return "Erreur SQRT";
        break;
    }
    case ModuloVirgule :
    {
        return "Erreur modulo virgule";
        break;
    }
    case ErreurASIN :
    {
        return "Erreur ASIN";
        break;
    }
    case ErreurType :
    {
        return "Erreur Type";
        break;
    }
    case ErreurATAN :
    {
        return "Erreur ATAN";
        break;
    }
    case ErreurACOS:
    {
        return "Erreur ACOS";
        break;
    }
    case ErreurTANH:
    {
        return "Erreur TANH";
        break;
    }
    case ErreurFactoriel:
    {
        return "Erreur factoriel";
        break;
    }
    default :
    {
        return "Code d'erreur inconnu";
        break;
    }
    }
}
