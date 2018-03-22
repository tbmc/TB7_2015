#include "affichage.h"
#include <string.h>
#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"
#include "eval.h"
#include "erreur.h"

//taille maximum de l'expression entrée par l'utilisateur
#define TAILLE_EXPRESSION 120

//vitesse du déplacement à la souris
#define VITESSE_SOURIS 2

//echelle d'affichage par défaut
#define ECHELLE 100.0F

void afficherRepere();
float orthoToGLX(float value);
float glToOrthoX(float value);
float orthoToGLY(float value);
float glToOrthoY(float value);
float mouseToGLX(float value);
float mouseToGLY(float value);

void modifierTexteErreur();

void diminuerZoom();
void augmenterZoom();
void afficherCurseur();

Point *pointsCourbe = NULL;
Arbre arbre = NULL;
int erreur = 0;
int nombreDePoints = 0;
int pos = 7;

// Le 8 correspond à la position de l'expression après la chaine "f(x) = "
char formule[TAILLE_EXPRESSION + 8] = {0};
char texteErreur[100] = {0};
char texteCurseur[100] = {0};
char tmpCursor = 0;
int xMouseTmp = -1;
int yMouseTmp = -1;
float decalageX = 0;
float decalageY = 0;
int fullscreenOn = 0;
int fullscreenTmp = 0;
float puissanceZoom = 1;
int cliqueGauche = 0;
Point curseur = {0, 0};

/**
 * @brief mouseToGLX
 *
 * @param value Valeur dans le système de coordonnées x de l'écran entre 0 et la longueur de l'écran
 * @return La valeur passée en paramètre convertie en coordonnées OpenGL
 */
float mouseToGLX(float value) {
    return (2*(value/(float)glutGet(GLUT_WINDOW_WIDTH)-0.5F)-(float)decalageX);
}

/**
 * @brief mouseToGLY
 *
 * @param value Valeur dans le système de coordonnées y de l'écran entre 0 et la hauteur de l'écran
 * @return La valeur passée en paramètre convertie en coordonnées OpenGL
 */
float mouseToGLY(float value) {
    return (-2*(value/(float)glutGet(GLUT_WINDOW_HEIGHT)-0.5F)-(float)decalageY);
}

/**
 * @brief cleanup
 *
 * Cette fonction est appelée automatiquement à la fermeture de la fenêtre et libère
 * tous les pointeurs non désalloués
 */
void cleanup() {
    if (arbre != NULL) {
        arbreLiberer(arbre);
        arbre = NULL;
    }
    if (pointsCourbe != NULL) {
        free(pointsCourbe);
        pointsCourbe = NULL;
    }
}

/**
 * @brief grapher
 *
 * Procédure de calcul des points de la courbe
 * L'intervalle de calculer de manière à calculer un point par pixel sur la longueur de l'écran
 * et seulements les points qui ont besoin d'êtres affichés
 * Cette procédure est appelée à chaque déplacement de l'utilisateur sur le repère (zoom ou translation)
 */

void grapher() {
    int j = 0;
    float i = 0;
    int width = glutGet(GLUT_WINDOW_WIDTH);
    if (puissanceZoom != 0)
    {
        float min = glToOrthoX(((-(float)decalageX - 1.0F)/puissanceZoom));
        float max = glToOrthoX(((-(float)decalageX + 1.0F)/puissanceZoom));
        float p = (max-min)/(width);
        if (pointsCourbe != NULL)
        {
            free(pointsCourbe);
        }
        pointsCourbe = (Point*)malloc(sizeof(Point)*width);
        nombreDePoints = width;
        for (i = min; i < max && j < nombreDePoints; i += p)
        {
            pointsCourbe[j] = eval(arbre, i, &erreur);
            j++;
        }
        modifierTexteErreur();
    }
}

/**
 * @brief modifierTexteErreur
 *
 * Procédure mettant à jour le texte d'erreur affiché à l'écran
 */
void modifierTexteErreur() {
    char texte[100];
    texteErreur[0] = 0;
    strcpy(texte, texte_erreur(erreur));
    strcat(texteErreur, "Erreur : ");
    strcat(texteErreur, texte);
}

/**
 * @brief calculerArbre
 *
 * Procédure de calcul de l'arbre à partir de la formule rentrée par l'utilisateur
 * Cette fonction est appelée à chaque modification de la formule par l'utilisateur
 */
void calculerArbre() {
    if (formule != NULL)
    {
        erreur = 0;
        Lexem lex = AnalyseLexicale(formule+7, &erreur);
        if (erreur == 0)
        {
            if (arbre != NULL)
            {
                arbreLiberer(arbre);
                arbre = NULL;
            }
            arbre = analyseSyntaxique(lex, &erreur);
        }
        if (lex != NULL)
        {
            free(lex);
            lex = NULL;
        }
          modifierTexteErreur();
          grapher();
    }
}

/**
 * @brief tracerCourbe
 *
 * Appelée à chaque rafraichissement de l'écran, cette procédure affiche la courbe à l'écran
 *
 * @param pointsCourbe précédemments calculés
 * @param nombreDePoints nombre de points dans le tableau
 */
void tracerCourbe(Point *pointsCourbe, int nombreDePoints) {
    setcolor(1.0F, 0.0F, 0.0F);
    if (pointsCourbe != NULL)
    {
        beginlines(orthoToGLX(pointsCourbe[0].x), orthoToGLY(pointsCourbe[0].y));
        int i;
        for (i = 1; i < nombreDePoints; i++)
        {
            lineto(orthoToGLX(pointsCourbe[i].x), orthoToGLY(pointsCourbe[i].y));
        }
        finishlines();
    }
}

/**
 * @brief renderScene
 *
 * Procédure appelée par Glut à chaque changemment des paramètres d'affichage
 * (déplacements de l'utilisateur, modification de l'expression, modification de la fenêtre)
 */
void renderScene() {
    afficherRepere();
    glPushMatrix();
    afficherCurseur();
    glTranslatef((float)(decalageX), (float)(decalageY), 0);
    glScalef(puissanceZoom, puissanceZoom, 1.0F);
    tracerCourbe(pointsCourbe, nombreDePoints);
    glPopMatrix();
    setcolor(0.0F, 0.0F, 0.0F);
    outtextxy(-0.99F, -0.97F, formule);
    outtextxy(-0.97F, 0.92F, texteErreur);
    outtextxy(0.0F, 0.90F, texteCurseur);
}


/**
 * @brief keyEvent
 *
 * Procédure appelée lors de l'appui sur une touche, celle ci permet :
 * - La saisie de l'expression
 * - L'augmentation ou la diminution du zoom
 * - Le passage en plein écran et inversement
 *
 * @param c
 */
void keyEvent(int c) {
    if (((c <= 122 && c >= 97) || (c <= 57 && c >= 48) || (c <= 47 && c >= 40) || c == 37) && strlen(formule) < TAILLE_EXPRESSION - 1)
    {
        formule[pos] = c;
        pos++;
        calculerArbre();
    }
    else if (c == 8 && pos > 7)
    {
        pos--;
        formule[pos] = '\0';
        calculerArbre();
    }
    else if (c == ' ')
    {
        decalageX = 0;
        decalageY = 0;
        puissanceZoom = 1;
        grapher();
    }
    else if (c == 38)
    {
        if (fullscreenOn == 0)
        {
            glutFullScreen();
            fullscreenOn = 1;
            fullscreenTmp = 0;
        }
        else
        {
            glutReshapeWindow(640, 480);
            fullscreenOn = 0;
            fullscreenTmp = 1;
        }
    }
    if (c == ';')
    {
        augmenterZoom();
    }
    else if (c == ':')
    {
        diminuerZoom();
    }
    else if (c == '$')
    {
        formule[pos] = '^';
        pos++;
        calculerArbre();
    }
    grapher();
    glutPostRedisplay();
}

/**
 * @brief mouseEvent
 *
 * Procédure qui :
 * - déplace le repère lors d'un clique gauche et d'un déplacement de la souris
 * - déplace le curseur sur une valeure d'abscisse entière  lors d'un clique sur la molette
 *
 * @param button Identificateur du bouton concerné
 * @param state Etat du bouton
 * @param x Coordonnée de la souris
 * @param y Coordonnée de la souris
 */
void mouseEvent(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            xMouseTmp = -1;
            yMouseTmp = -1;
            cliqueGauche = 1;
        }
        else if (state == GLUT_UP)
        {
            grapher();
            cliqueGauche = 0;
        }

    }
    else if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            curseur.x = mouseToGLX(x)/puissanceZoom;
            curseur.y = orthoToGLY(eval(arbre,glToOrthoX(curseur.x), &erreur).y);
            char texte[100] = {0};
            texteCurseur[0] = 0;
            sprintf(texte, "%f     ", glToOrthoX(curseur.x));
            strcat(texteCurseur, "x = ");
            strcat(texteCurseur, texte);
            sprintf(texte, "%f\n", glToOrthoY(curseur.y));
            strcat(texteCurseur, "y = ");
            strcat(texteCurseur, texte);

        }
    }
    else if (button == GLUT_MIDDLE_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            curseur.x = orthoToGLX(round(glToOrthoX(mouseToGLX(x)/puissanceZoom)));
            curseur.y = (orthoToGLY(eval(arbre,glToOrthoX(curseur.x), &erreur).y));
            char texte[100] = {0};
            texteCurseur[0] = 0;
            sprintf(texte, "%f     ", round(glToOrthoX(curseur.x)));
            strcat(texteCurseur, "x = ");
            strcat(texteCurseur, texte);
            sprintf(texte, "%f\n", glToOrthoY(curseur.y));
            strcat(texteCurseur, "y = ");
            strcat(texteCurseur, texte);
        }
    }

}

/**
 * @brief motionEvent
 *
 * Procédure qui permet le déplacement du repère lors d'un clique gauche
 *
 * @param x
 * @param y
 */
void motionEvent(int x, int y) {
    if (xMouseTmp != -1 && yMouseTmp != -1 && cliqueGauche == 1)
    {
        decalageX -= (float)(xMouseTmp - x)*(float)VITESSE_SOURIS/(float)glutGet(GLUT_WINDOW_WIDTH);
        decalageY += (float)(yMouseTmp - y)*(float)VITESSE_SOURIS/(float)glutGet(GLUT_WINDOW_HEIGHT);
    }
    xMouseTmp = x;
    yMouseTmp = y;
    glutPostRedisplay();
}

/**
 * @brief initOpenGL
 *
 * Procédure appelée à l'ouverture de la fenêtre qui affiche la courbe
 */
void initOpenGL() {
    grapher();
    glutCloseFunc(cleanup);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    strcat(formule, "f(x) = ");
}

/**
 * @brief afficherCurseur
 *
 * Procédure d'affichage du curseur
 */
void afficherCurseur() {
    glPushMatrix();
    glTranslatef((float)(decalageX), (float)(decalageY), 0);
    glScalef(puissanceZoom, puissanceZoom, 1.0F);
    setcolor(1.0F, 0.0F, 1.0F);
    line(curseur.x+orthoToGLX(-0.3F), curseur.y, curseur.x+orthoToGLX(+0.3F), curseur.y);
    line(curseur.x, curseur.y+orthoToGLY(-0.3F), curseur.x, curseur.y+orthoToGLY(+0.3F));
    glPopMatrix();
}

/**
 * @brief afficherRepere
 * Procédure d'afichage du repère
 */
void afficherRepere() {
    float i;
    glPushMatrix();
    glTranslatef((float)(decalageX), (float)(decalageY), 0);
    glScalef(puissanceZoom, puissanceZoom, 1.0F);
    setcolor(0.0F, 0.0F, 0.0F);
    line(-1000.0F, 0.0F, 1000.0F, 0.0F);
    line(0.0F, -1000.0F, 0.0F, 1000.0F);
    for (i = -1000.0F; i < 1000; i += 1)
    {
        line(orthoToGLX(-0.1F), orthoToGLY(i), orthoToGLX(0.1F), orthoToGLY(i));
    }
    for (i = -1000.0F; i < 1000; i += 1)
    {
        line(orthoToGLX(i), orthoToGLY(-0.1F), orthoToGLX(i), orthoToGLY(0.1F));
    }
    glPopMatrix();
}

/**
 * @brief orthoToGLX
 * @param value Valeur dans le système de coordonnées x du repère orthonormé (espace de calcul du programme)
 * @return La valeur passée en paramètre convertie en coordonnées OpenGL
 */
float orthoToGLX(float value) {
    return ((value/glutGet(GLUT_WINDOW_WIDTH))*ECHELLE);
}

/**
 * @brief glToOrthoX
 * @param value Valeur dans le système de coordonnées OpenGL
 * @return Valeur dans le système de coordonnées x du repère orthonormé (espace de calcul du programme)
 */
float glToOrthoX(float value) {
    return ((value/ECHELLE)*glutGet(GLUT_WINDOW_WIDTH));
}

/**
 * @brief orthoToGLY
 * @param value Valeur dans le système de coordonnées y du repère orthonormé (espace de calcul du programme)
 * @return La valeur passée en paramètre convertie en coordonnées OpenGL
 */
float orthoToGLY(float value) {
    return ((value/glutGet(GLUT_WINDOW_HEIGHT))*ECHELLE);
}

/**
 * @brief glToOrthoY
 * @param value Valeur dans le système de coordonnées OpenGL
 * @return Valeur dans le système de coordonnées y du repère orthonormé (espace de calcul du programme)
 */
float glToOrthoY(float value) {
    return ((value/ECHELLE)*glutGet(GLUT_WINDOW_HEIGHT));
}

/**
 * @brief augmenterZoom
 * A chaque appel de cette fonction, la 'puissance' du zoom est augmentée
 */
void augmenterZoom() {
    if (puissanceZoom < 4)
    {
        puissanceZoom += 0.1;
        grapher();
    }
    glutPostRedisplay();
}

/**
 * @brief augmenterZoom
 * A chaque appel de cette fonction, la 'puissance' du zoom est diminuée
 */
void diminuerZoom() {
    if (puissanceZoom > 0.1)
    {
        puissanceZoom -= 0.1;
        grapher();
    }
    glutPostRedisplay();
}

/**
 * @brief wheelEvent
 *
 * Cette procédure est appelée automatique pas Glut lors d'un dépacement de la molette
 *
 * @param wheel numéro de la molette
 * @param direction direction du déplacement
 * @param x coordonnée de la souris
 * @param y coordonnée de la souris
 */
void wheelEvent(int wheel, int direction, int x, int y)
{
    if (direction == 1)
    {
        augmenterZoom();
    }
    else if (direction == -1)
    {
        diminuerZoom();
    }
    glutPostRedisplay();
}

/**
 * @brief reshapeEvent
 * Procédure affichée à chaque modification de la taille de la fenêtre,
 * celle ci recalcul les points de la courbe
 */
void reshapeEvent() {
    grapher();
}
