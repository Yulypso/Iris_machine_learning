/*Bibliothèque*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Structures*/
typedef struct NODE NODE;
typedef struct LISTE LISTE;
typedef struct ELEMENT ELEMENT;
typedef struct LISTE_IRIS LISTE_IRIS;
typedef struct IRIS IRIS;


struct NODE
{
	double entropy;  	/*L'entropy dans le noeud*/
	int test;
	double key;
	double sample; 		/*(nombre d'échantillon dans le noeud)*/

	LISTE_IRIS* liste_iris;

	double nbsetosa;		/*(nombre de setosa dans le noeud)*/
	double nbversicolor;	/*(nombre de versicolor dans le noeud)*/
	double nbvirginica;		/*(nombre de virginica dans le noeud)*/

	NODE* ls;
	NODE* rs;
};

/*Pour la création de l'arbre*/
struct LISTE
{
	ELEMENT* premier;
};

struct ELEMENT
{
	int test;
	double key;
	double entropy;
	double sample;

	LISTE_IRIS* liste_iris_gauche;
	LISTE_IRIS* liste_iris_droit;

	double nbsetosa;	
	double nbversicolor;	
	double nbvirginica;		

	ELEMENT* suivant;
};


 /*Pour la Base de données sous forme de liste*/
struct LISTE_IRIS
{
	IRIS* premier;
};

struct IRIS
{
	float SL;
	float SW;
	float PL;
	float PW;
	int NAME;
	IRIS* suivant;
};


/*Headers*/
int main(int argc, char const *argv[]);
void lancement();//K

LISTE_IRIS* initialisation_base_donnees();//K
void informations_IRIS(); /*fonction non obligatoire*/ //K

LISTE_IRIS* initialiser(float SL, float SW, float PL, float PW, int NAME);//K
LISTE_IRIS* conversion_tbl_liste_iris();//D

double calcul_entropy(double nbsetosa, double nbversicolor, double nbvirginica, double sample);//K
double initialisation_calcul_entropy(double nbsetosa, double nbversicolor, double nbvirginica, double sample);//K

void creation_arbre(LISTE_IRIS* liste_iris);

ELEMENT* condition_test(LISTE_IRIS* liste_iris, double ifracine);//K
LISTE_IRIS* condition_test_liste_fils(ELEMENT* element, int test_save, LISTE_IRIS* liste_iris, int gauche_droit);

int determiner_test_save(double a, double b, double c, double d ,double a_cpt, double b_cpt, double c_cpt, double d_cpt, double a_test, double b_test, double c_test, double d_test);
int test_cpt_iris(double a_cpt, double b_cpt, double c_cpt, double d_cpt, double a_test, double b_test, double c_test, double d_test);
NODE* addnode(NODE* a, ELEMENT* element, LISTE_IRIS* liste_iris);
//void addnode_condition(NODE* a, ELEMENT* element, LISTE_IRIS* liste_iris, double ifracine);
IRIS* add_new_iris();//d


