/*Pour compiler Kyu : gcc -Wall /Users/Kyu/Desktop/Projet_IRIS_ver_9/main.c -lm -o MAIN*/
/*Pour compiler Diane : gcc -Wall main.c -lm -o main */ 

/*Bibliothèque*/
#include "Headers.h" /*Il faut rajouter les prototypes au fur à mesure*/
#include "Biblio.h"
#include "Initialisation.h" /*Rien à modifier !!*/
#include "Entropy.h"

/*Pour mieux comprendre*/
#define NB_TYPE_IRIS_LOCAL 0
#define NB_IRIS_LOCAL 0
#define VAL_ENTROPY_LOCAL 0

/*Début*/
int main(int argc, char const *argv[])
{
	lancement();

	return 0;
}



/*Lancement de la creation de l'arbre par rapport à la base de donnees*/
void lancement()
{
	LISTE_IRIS* liste_iris = initialisation_base_donnees();
	creation_arbre(liste_iris);
}



/*Fonctions concernant la base de donnees*/
LISTE_IRIS* initialisation_base_donnees()
{
	recuperation_base_donnees();
	LISTE_IRIS* liste_iris = conversion_tbl_liste_iris();
	informations_IRIS(); /*Fonction non obligatoire -> utilisation avec #DEFINE dans Initialisation*/
	return liste_iris;
}


/*Fonctions concernant la création de l'arbre binaire de décisions*/
void creation_arbre(LISTE_IRIS* liste_iris)
{
	int i = 0;
	ELEMENT* element = NULL;

/* On commence d'abord par récupérer calculer l'entropie au niveau de la racine */
	double ifracine = 9.9; //permet d'ajouter la racine de l'arbre en n'effectuant qu'une iteration dans la boucle for de la fonction condition_test
	element = condition_test(liste_iris, ifracine);

	printf("racine\n");
	afficher_iris_liste(element->liste_iris_gauche); //on affiche ensuite la liste qui contient la base de données



	ifracine = 0.0; //On peut désormais effectuer des tests de 0.0 à 10.0 avec un pas de 0.1 dans la boucle for de la fonction condition_test

	while(i < 8)
	{
		element = condition_test(liste_iris, ifracine); /*element reçoit le test optimal (general) ainsi que ses caractéristiques*/
		printf("sous arbre gauche \n");
		afficher_iris_liste(element->liste_iris_gauche); //on affiche le "fils gauche" / "liste gauche"
		printf("sous arbre droit \n");
		afficher_iris_liste(element->liste_iris_droit); //on affiche le "fils droit" "liste droite"

		if(element->liste_iris_droit != NULL)
		liste_iris = element->liste_iris_droit; //on va toujours à droite tant que le fils de la liste d'iris n'est pas NULL

		i++;
	}
}





NODE* addnode(NODE* a, ELEMENT* element, LISTE_IRIS* liste_iris)
{
	if(a==NULL)
	{
		a = malloc(sizeof(NODE));
		a->entropy = element->entropy;	/*L'entropy dans le noeud*/
		a->test = element->test;		/*type de test effectué par le noeud */
		a->key = element->key;			/*valeur de test*/
		a->sample = element->sample;

		a->liste_iris = liste_iris;

		a->nbsetosa = element->nbsetosa;
		a->nbvirginica = element->nbvirginica;
		a->nbversicolor = element->nbversicolor;

		a->ls = NULL;
		a->rs = NULL;
	}
	else
	{
   		if(element->test == 0) /*On connaît le premier test : SL*/
   		{
   			if(liste_iris->premier->SL <= element->key)
   			{
   				a->ls = addnode(a->ls, element, element->liste_iris_gauche);
   			}
   			else
   			{
   				a->rs = addnode(a->rs, element, element->liste_iris_droit);
   			}
   		}
   			
   		else if(element->test == 1) /*On connaît le premier test : SL*/
   		{
   			if(liste_iris->premier->SW <= element->key)
   			{
   				a->ls = addnode(a->ls, element, element->liste_iris_gauche);
   			}
   			else
   			{
   				a->rs = addnode(a->rs, element, element->liste_iris_droit);
   			}
   		}

   		else if(element->test == 2) /*On connaît le premier test : SL*/
   		{
   			if(liste_iris->premier->PL <= element->key)
   			{
   				a->ls = addnode(a->ls, element, element->liste_iris_gauche);
   			}
   			else
   			{
   				a->rs = addnode(a->rs, element, element->liste_iris_droit);
   			}
   		}

   		else if(element->test == 3) /*On connaît le premier test : SL*/
   		{
   			if(liste_iris->premier->PW <= element->key)
   			{
   				a->ls = addnode(a->ls, element, element->liste_iris_gauche);
   			}
   			else
   			{
   				a->rs = addnode(a->rs, element, element->liste_iris_droit);
   			}
   		}	
	}
	return a;
}


/*ETAPE 1*/
ELEMENT* condition_test(LISTE_IRIS* liste_iris, double ifracine)
{
	///*DECLARATION ET INITIALISATIONS DES VARIABLES*///

	/* Initialisation des COMPTEURS */
	/*Compteurs du nombre total d'iris qui valident le type de test SL/SW/PL/PW [Local].
	  Ces compteurs sont remis à 0.0 à chaque itération dans la boucle for*/
	double cpt_iris_SL=0.00; double cpt_iris_SW=0.00; double cpt_iris_PL=0.00; double cpt_iris_PW=0.00; 

	/*Compteur du nombre total par type d'iris qui ont validé le type de test en question [Local].
	  Ces compteurs sont remis à 0.0 à chaque itération dans la boucle for*/
	double nbsetosa_SL=0.0; double nbversicolor_SL=0.0; double nbvirginica_SL=0.0; /*Nombre de Setosa/Versicolor/Virginica pour le test Sepal Length*/
	double nbsetosa_SW=0.0; double nbversicolor_SW=0.0; double nbvirginica_SW=0.0; /*Nombre de Setosa/Versicolor/Virginica pour le test Sepal Width*/
	double nbsetosa_PL=0.0; double nbversicolor_PL=0.0; double nbvirginica_PL=0.0; /*Nombre de Setosa/Versicolor/Virginica pour le test Petal Length*/
	double nbsetosa_PW=0.0; double nbversicolor_PW=0.0; double nbvirginica_PW=0.0; /*Nombre de Setosa/Versicolor/Virginica pour le test Petal Width*/

	/*Initialisation des variables qui contiennent la valeur de l'entropy pour chaque type de test SL/SW/PL/PW [Local].
	  Ces variables sont reinitialisées à 100.0 à chaque itération dans la boucle for. 
	  (la valeur 100.0 est une valeur improbable dans le calcul de l'entropy)*/
	double entropy_SL=100.0;
	double entropy_SW=100.0;
	double entropy_PL=100.0;
	double entropy_PW=100.0;


	/* Initialisation des varibles de SAUVEGARDE */
	/*Ces variables permettent de conserver les valeurs "optimales" pour chaque type de test SL/SW/PL/PW entre chaque
	  incrémentation dans la boucle for*/
	double entropy_SL_save=100.0; /*Variable permettant de conserver l'entropy optimale pour le type de test SL [global]*/
	double entropy_SW_save=100.0; /*Variable permettant de conserver l'entropy optimale pour le type de test SW [global]*/
	double entropy_PL_save=100.0; /*Variable permettant de conserver l'entropy optimale pour le type de test PL [global]*/
	double entropy_PW_save=100.0; /*Variable permettant de conserver l'entropy optimale pour le type de test PW [global]*/

	/*Ces variables permettent de conserver la valeur du test "key" pour chque type de test SL/SW/PL/PW entre chaque
	  incrémentation das la boucle for*/
	double key_SL_save=0.0;	/*Variable permettant de conserver la valeur du test "key" optimal pour le test SL [global]*/
	double key_SW_save=0.0; /*Variable permettant de conserver la valeur du test "key" optimal pour le test SW [global]*/
	double key_PL_save=0.0; /*Variable permettant de conserver la valeur du test "key" optimal pour le test PL [global]*/
	double key_PW_save=0.0; /*Variable permettant de conserver la valeur du test "key" optimal pour le test PW [global]*/

	/*Ces variables permettent de conserver le nombre total par type d'iris qui ont validé le type de test optimal en question.*/
	double cpt_iris_SL_save=0.00; double cpt_iris_SW_save=0.00; double cpt_iris_PL_save=0.00; double cpt_iris_PW_save=0.00; 

	/*Ces variables permettent de conserver le nombre total par type d'iris qui ont validé le type de test en question optimal.
	  Ils permettent donc de sauvegarder les compteurs optimales locales.*/
	double nbsetosa_SL_save=0.0; double nbversicolor_SL_save=0.0; double nbvirginica_SL_save=0.0; /*Sauvegarde le nombre de Setosa/Versicolor/Virginica pour le test Sepal Length optimale*/
	double nbsetosa_SW_save=0.0; double nbversicolor_SW_save=0.0; double nbvirginica_SW_save=0.0; /*Sauvegarde le nombre de Setosa/Versicolor/Virginica pour le test Sepal Widtg optimale*/ 
	double nbsetosa_PL_save=0.0; double nbversicolor_PL_save=0.0; double nbvirginica_PL_save=0.0; /*Sauvegarde le nombre de Setosa/Versicolor/Virginica pour le test Petal Length optimale*/
	double nbsetosa_PW_save=0.0; double nbversicolor_PW_save=0.0; double nbvirginica_PW_save=0.0; /*Sauvegarde le nombre de Setosa/Versicolor/Virginica pour le test Petal Width optimale*/


	/*Variable qui permet de sauvegarder le meilleur type de test parmi SL/SW/PL/PW [Global]*/
	int test_save=0; /*type de test optimal [general]*/

	/*Initialisation d'un element de type ELEMENT que nous allons retourner à la fin de la fonction*/
	ELEMENT* element=NULL;
	element = malloc(1*sizeof(ELEMENT));

	if(liste_iris->premier==NULL)
    {
		printf("La liste est vide !\n");
    }

    /*Initialisation d'un pointeur courant de type IRIS qui va permettre de parcourir la liste d'iris entièrement
      du début à la fin à chaque iteration dans la boucle for*/
    IRIS *courant = NULL;
    courant=liste_iris->premier;
    /*courant est maintenant au premier element de la liste d'iris,
      Lorsqu'il atteindra la fin de la liste pour une iteration dans la boucle for,
      courant est replacé au premier element de la liste d'iris*/


    /*Début de la boucle for qui va permettre de tester pour chaque valeur de test entre 0.0 et 10.0
      et pour une valeur de test, une boucle while permettra de parcourir la liste d'iris entièrement afin de
      d'incrémenter les compteurs vérifiant chaque type de test SL/SW/PL/PW.*/ 

	for(double i=ifracine; i<10.0; i=i+0.1)
	{
	/*la valeur de la variable ifracine est définie dans la fonction Creation_arbre, 
	  fixée à 9.9, elle permet de récupérer toute la base de donnnées en un pas de 0.1 et d'en calculer son entropy.
	  puis, fixée à 0.0, elle permet de débuter la séparation de la liste d'iris en deux selon le test optimal déterminé [global].*/ 

   		while(courant!=NULL)
   		{
   			/*La boucle while permet de parcourir la liste_iris entièrement*/

   			/*Incrémentation des compteurs pour le type de test SL*/
			if(courant->SL <= i)
			{
				/*Nombre d'iris total aillant validé le type de test SL pour la valeur de test "key" i*/
				cpt_iris_SL++; 

				if(courant->NAME == 0)
				{
					/*Nombre d'iris setosa parmi le nobmre d'iris total aillant validé le type de test SL pour la valeur de test "key" i*/
					nbsetosa_SL++;
				}
				else if(courant->NAME == 1)
				{
					/*Nombre d'iris versicolor parmi le nobmre d'iris total aillant validé le type de test SL pour la valeur de test "key" i*/
					nbversicolor_SL++;
				}
				else if(courant->NAME == 2)
				{
					/*Nombre d'iris virginica parmi le nobmre d'iris total aillant validé le type de test SL pour la valeur de test "key" i*/
					nbvirginica_SL++;
				}
			}
		
			/*Incrémentation des compteurs pour le type de test SW*/
			if(courant->SW <= i)
			{
				/*Nombre d'iris total aillant validé le type de test SW pour la valeur de test "key" i*/
				cpt_iris_SW++;

				if(courant->NAME == 0)
				{
					/*Nombre d'iris setosa parmi le nobmre d'iris total aillant validé le type de test SW pour la valeur de test "key" i*/
					nbsetosa_SW++;
				}
				else if(courant->NAME == 1)
				{
					/*Nombre d'iris versicolor parmi le nobmre d'iris total aillant validé le type de test SW pour la valeur de test "key" i*/
					nbversicolor_SW++;
				}
				else if(courant->NAME == 2)
				{
					/*Nombre d'iris virginica parmi le nobmre d'iris total aillant validé le type de test SW pour la valeur de test "key" i*/
					nbvirginica_SW++;
				}
			}

			/*Incrémentation des compteurs pour le type de test PL*/
			if(courant->PL <= i)
			{
				/*Nombre d'iris total aillant validé le type de test PL pour la valeur de test "key" i*/
				cpt_iris_PL++;

				if(courant->NAME == 0)
				{
					/*Nombre d'iris setosa parmi le nobmre d'iris total aillant validé le type de test PL pour la valeur de test "key" i*/
					nbsetosa_PL++;
				}
				else if(courant->NAME == 1)
				{
					/*Nombre d'iris versicolor parmi le nobmre d'iris total aillant validé le type de test PL pour la valeur de test "key" i*/
					nbversicolor_PL++;
				}
				else if(courant->NAME == 2)
				{
					/*Nombre d'iris virginica parmi le nobmre d'iris total aillant validé le type de test PL pour la valeur de test "key" i*/
					nbvirginica_PL++;
				}
			}

			/*Incrémentation des compteurs pour le type de test PW*/
			if(courant->PW <= i)
			{
				/*Nombre d'iris total aillant validé le type de test PW pour la valeur de test "key" i*/
				cpt_iris_PW++;

				if(courant->NAME == 0)
				{
					/*Nombre d'iris setosa parmi le nobmre d'iris total aillant validé le type de test PW pour la valeur de test "key" i*/
					nbsetosa_PW++;
				}
				else if(courant->NAME == 1)
				{
					/*Nombre d'iris versicolor parmi le nobmre d'iris total aillant validé le type de test PW pour la valeur de test "key" i*/
					nbversicolor_PW++;
				}
				else if(courant->NAME == 2)
				{
					nbvirginica_PW++;
				}
			}

		/* On fait pointer courant sur l'iris suivant dans la liste d'iris*/
        courant=courant->suivant;
    	}

    	/* On est toujours dans la boucle for, 
    	NB_IRIS_LOCAL activé (1), permet d'afficher le nombre d'iris qui vérifient la valeur de test "key" i*/

		if(NB_IRIS_LOCAL) /*Voir #define*/
		{
			printf("Au total pour Sepal Length test il y a %f iris inférieur à %fcm \n", cpt_iris_SL, i);	
  			printf("Au total pour Sepal Width test il y a %f iris inférieur à %fcm \n", cpt_iris_SW, i);
  			printf("Au total pour Petal Length test il y a %f iris inférieur à %fcm \n", cpt_iris_PL, i);
 			printf("Au total pour Petal Width test il y a %f iris inférieur à %fcm \n", cpt_iris_PW, i);
 			printf("\n");
		}


    	/* On est toujours dans la boucle for, 
    	   NB_TYPE_IRIS_LOCAL activé (1), permet d'afficher le nombre d'iris par variété (local) qui pour un type de test SL/SW/PL/PW vérifient le type de test "key" i*/

    	if(NB_TYPE_IRIS_LOCAL) /*Voir #define*/
    	{
  			printf("Sepal Length test : nbsetosa_SL: %f, nbversicolor_SL: %f, nbvirginica_SL: %f \n", nbsetosa_SL, nbversicolor_SL, nbvirginica_SL);
  			printf("Sepal Width test : nbsetosa_SW: %f, nbversicolor_SW: %f, nbvirginica_SW: %f \n", nbsetosa_SW, nbversicolor_SW, nbvirginica_SW);
			printf("Petal Length test : nbsetosa_PL: %f, nbversicolor_PL: %f, nbvirginica_PL: %f \n", nbsetosa_PL, nbversicolor_PL, nbvirginica_PL);
			printf("Petal Width test : nbsetosa_PW: %f, nbversicolor_PW: %f, nbvirginica_PW: %f \n", nbsetosa_PW, nbversicolor_PW, nbvirginica_PW);
			printf("\n");
		}



    	/* On est toujours dans la boucle for */
		/* Calcul d'entropie pour chaque type de test SL/SW/PL/PW pour une valeur de test "key" i */

    	entropy_SL = initialisation_calcul_entropy(nbsetosa_SL, nbversicolor_SL, nbvirginica_SL, cpt_iris_SL);
    	entropy_SW = initialisation_calcul_entropy(nbsetosa_SW, nbversicolor_SW, nbvirginica_SW, cpt_iris_SW);
    	entropy_PL = initialisation_calcul_entropy(nbsetosa_PL, nbversicolor_PL, nbvirginica_PL, cpt_iris_PL);
    	entropy_PW = initialisation_calcul_entropy(nbsetosa_PW, nbversicolor_PW, nbvirginica_PW, cpt_iris_PW);


    	/* Pour les 4 entropies calculées pour chaque type de test SL/SW/PL/PW, 
    	On cherche à déterminer l'entropie la plus basse entre l'entropy sauvegardée et celle qui a été calculée pour la valeur du test "key" i (local)*/
    	
    	/*Si l'entropy du test SL est la plus faible et donc la plus optimale*/
    	if(entropy_SL <= entropy_SL_save && cpt_iris_SL >= cpt_iris_SL_save)
    	{
    		entropy_SL_save = entropy_SL; //on conserve l'entropie optimale 
    		cpt_iris_SL_save = cpt_iris_SL; //on conserve le nombre d'iris qui ont validé le type de test SL/SW/PL/PW pour une valeur de test "key" i
    		nbsetosa_SL_save = nbsetosa_SL; //on conserve le nombre de setosa parmi le nombre d'iris qui ont validé le type de test
    		nbversicolor_SL_save = nbversicolor_SL; //on conserve le nombre de versicolor parmi le nombre d'iris qui ont validé le type de test
    		nbvirginica_SL_save = nbvirginica_SL; //on conserve le nombre de virginica parmi le nombre d'iris qui ont validé le type de test
    		key_SL_save = i; //on conserva la valeur de test "key" i optimale
    	}

    	/*Si l'entropy du test SW est la plus faible et donc la plus optimale*/
    	if(entropy_SW <= entropy_SW_save && cpt_iris_SW >= cpt_iris_SW_save)
    	{
    		entropy_SW_save = entropy_SW; //on conserve l'entropie optimale 
    		cpt_iris_SW_save = cpt_iris_SW; //on conserve le nombre d'iris qui ont validé le type de test SL/SW/PL/PW pour une valeur de test "key" i
    		nbsetosa_SW_save = nbsetosa_SW; //on conserve le nombre de setosa parmi le nombre d'iris qui ont validé le type de test
    		nbversicolor_SW_save = nbversicolor_SW; //on conserve le nombre de versicolor parmi le nombre d'iris qui ont validé le type de test
    		nbvirginica_SW_save = nbvirginica_SW; //on conserve le nombre de virginica parmi le nombre d'iris qui ont validé le type de test
    		key_SW_save = i; //on conserva la valeur de test "key" i optimale
    	}

    	/*Si l'entropy du test PL est la plus faible et donc la plus optimale*/
		if(entropy_PL <= entropy_PL_save && cpt_iris_PL >= cpt_iris_PL_save)
    	{
    		entropy_PL_save = entropy_PL; //on conserve l'entropie optimale 
    		cpt_iris_PL_save = cpt_iris_PL; //on conserve le nombre d'iris qui ont validé le type de test SL/SW/PL/PW pour une valeur de test "key" i
    		nbsetosa_PL_save = nbsetosa_PL; //on conserve le nombre de setosa parmi le nombre d'iris qui ont validé le type de test
    		nbversicolor_PL_save = nbversicolor_PL; //on conserve le nombre de versicolor parmi le nombre d'iris qui ont validé le type de test
    		nbvirginica_PL_save = nbvirginica_PL; //on conserve le nombre de virginica parmi le nombre d'iris qui ont validé le type de test
    		key_PL_save = i; //on conserva la valeur de test "key" i optimale
    	}

    	/*Si l'entropy du test PW est la plus faible et donc la plus optimale*/
		if(entropy_PW <= entropy_PW_save && cpt_iris_PW >= cpt_iris_PW_save) 
    	{
    		entropy_PW_save = entropy_PW; //on conserve l'entropie optimale 
    		cpt_iris_PW_save = cpt_iris_PW; //on conserve le nombre d'iris qui ont validé le type de test SL/SW/PL/PW pour une valeur de test "key" i
    		nbsetosa_PW_save = nbsetosa_PW; //on conserve le nombre de setosa parmi le nombre d'iris qui ont validé le type de test
    		nbversicolor_PW_save = nbversicolor_PW; //on conserve le nombre de versicolor parmi le nombre d'iris qui ont validé le type de test
    		nbvirginica_PW_save = nbvirginica_PW; //on conserve le nombre de virginica parmi le nombre d'iris qui ont validé le type de test
    		key_PW_save = i; //on conserva la valeur de test "key" i optimale
    	}

    	/* On est toujours dans la boucle for,
    	  VAL_ENTROPY_LOCAL activé (1), permet d'afficher l'entropie (local) pour chaque type de test SL/SW/PL/PW 
    	  et la valeur du test "key" i associée*/
    	if(VAL_ENTROPY_LOCAL)
    	{
    		printf("L'entropy pour Sepal Length test est : %.11lf pour la valeur key (i) = %f\n", entropy_SL, i);
    		printf("L'entropy pour Sepal Width test est : %.11lf pour la valeur key(i) = %f\n", entropy_SW, i);
    		printf("L'entropy pour Petal Length test est : %.11lf pour la valeur key (i) = %f\n", entropy_PL, i);
    		printf("L'entropy pour Petal Width test est : %.11lf pour la valeur key (i) = %f\n", entropy_PW, i);
    		printf("\n");
		}

    	/*Pour une itération dans la boucle for,
    	  On a maintenant pour chaque type de test : 
    	  - terminé de compter les iris qui ont validé la valeur de test
    	  - terminé de calculer l'entropie pour chaque type de test
    	  - terminé de comparer les entropies pour chaque type de test et d'en conserver la meilleure avec ses caractéristiques
    	*/

    	/*On reinitialise toutes les variables de comptages*/
    	nbsetosa_SL=0.0; nbversicolor_SL=0.0; nbvirginica_SL=0.0;
		nbsetosa_SW=0.0; nbversicolor_SW=0.0; nbvirginica_SW=0.0;
		nbsetosa_PL=0.0; nbversicolor_PL=0.0; nbvirginica_PL=0.0;
		nbsetosa_PW=0.0; nbversicolor_PW=0.0; nbvirginica_PW=0.0;
		entropy_SL=100.0; entropy_SW=100.0; entropy_PL=100.0; entropy_PW=100.0; 
		cpt_iris_SL=0.00; cpt_iris_SW=0.00; cpt_iris_PL=0.00; cpt_iris_PW=0.00; 

		/*le pointeur courant pointe à nouveau vers le premier iris de la liste d'iris*/
		courant=liste_iris->premier;
    }

    /*On a maintenant quitté la boucle for, ce qui signifie que : 

      On a maintenant pour les 4 types de test SL/SW/PL/PW, le test optimal pour chacun de ses tests 
      ainsi que leurs caractéristiques. 
      (valeur de test, nombre d'iris qui ont validé le test, nombre de setosa qui ont validé le test, 
      nombre de versicolor qui ont validé le test, nombre de virginica qui ont validé le test)
    */

    /*On affiche les caractéristiques associées aux 4 types de tests SL/SW/PL/PW qui sont optimales [global]*/
    printf("\n");
  	printf("test: 0 | entropy_SL: %.11lf | key_SL: %.2f pour %.1f iris dans le noeud || %.f setosa | %.f versicolor | %.f virginica ||\n", entropy_SL_save, key_SL_save, cpt_iris_SL_save, nbsetosa_SL_save, nbversicolor_SL_save, nbvirginica_SL_save);
  	printf("test: 1 | entropy_SW: %.11lf | key_SW: %.2f pour %.1f iris dans le noeud || %.f setosa | %.f versicolor | %.f virginica ||\n", entropy_SW_save, key_SW_save, cpt_iris_SW_save, nbsetosa_SW_save, nbversicolor_SW_save, nbvirginica_SW_save);
  	printf("test: 2 | entropy_PL: %.11lf | key_PL: %.2f pour %.1f iris dans le noeud || %.f setosa | %.f versicolor | %.f virginica ||\n", entropy_PL_save, key_PL_save, cpt_iris_PL_save, nbsetosa_PL_save, nbversicolor_PL_save, nbvirginica_PL_save);
  	printf("test: 3 | entropy_PW: %.11lf | key_PW: %.2f pour %.1f iris dans le noeud || %.f setosa | %.f versicolor | %.f virginica ||\n", entropy_PW_save, key_PW_save, cpt_iris_PW_save, nbsetosa_PW_save, nbversicolor_PW_save, nbvirginica_PW_save);


  	/*On cherche à déterminer le test le plus optimal parmi les 4 meilleurs tests, le test [general]*/
  	/*Afin de déterminer la test optimal il existe plusieurs cas possible :*/

  	/*Si pour les 4 types de tests sont identiques l'entropie ont la même valeur (pas forcément == 0.0) et les compteurs ont la même valeur,
  	  cela signifie qu'on n'a pas pu séparer la liste en deux car on a récupéré pour chaque type de test toute la liste d'iris entièrement
  	  On se situe donc au niveau de la racine*/
	if(entropy_SL_save <= entropy_SW_save && entropy_SL_save <= entropy_PL_save && entropy_SL_save <= entropy_PW_save && 
		entropy_SW_save <= entropy_SL_save && entropy_SW_save <= entropy_PL_save && entropy_SW_save <= entropy_PW_save && 
		entropy_PL_save <= entropy_SL_save && entropy_PL_save <= entropy_SW_save && entropy_PL_save <= entropy_PW_save && 
		entropy_PW_save <= entropy_SL_save && entropy_PW_save <= entropy_SW_save && entropy_PW_save <= entropy_PL_save && 
		cpt_iris_SL_save == cpt_iris_SW_save && cpt_iris_SL_save == cpt_iris_PL_save && cpt_iris_SL_save == cpt_iris_PW_save)
	{
		test_save = -2;
	}

	/*Si pour les 4 types de tests l'entropie est égale à 0.0 et que les compteurs ont la même valeur,
	  Cela signifie qu'on se situe au niveau d'une feuille et qu'on ne peut pas continuer la séparation en deux de la liste d'iris à ce niveau.*/
	else if(entropy_SL_save == 0.0 && entropy_SL_save <= entropy_SW_save && entropy_SL_save <= entropy_PL_save && entropy_SL_save <= entropy_PW_save && 
			entropy_SW_save <= entropy_SL_save && entropy_SW_save <= entropy_PL_save && entropy_SW_save <= entropy_PW_save && 
			entropy_PL_save <= entropy_SL_save && entropy_PL_save <= entropy_SW_save && entropy_PL_save <= entropy_PW_save && 
			entropy_PW_save <= entropy_SL_save && entropy_PW_save <= entropy_SW_save && entropy_PW_save <= entropy_PL_save && 
			cpt_iris_SL_save == cpt_iris_SW_save && cpt_iris_SL_save == cpt_iris_PL_save && cpt_iris_SL_save == cpt_iris_PW_save)
	{
		test_save = -1;
	}

	/*Si pour le type de test SL l'entropie est la plus petite ou égale à l'entropie de SW, PL et PW, 
	  Afin d'obtenir le test optimal [général], on s'interesse au nombre de fleurs triés. Plus ce nombre sera élevé, plus le test sera considéré comme optimal.
	  De même pour SW/PL/PW ensuite.*/
  	else if(entropy_SL_save <= entropy_SW_save && entropy_SL_save <= entropy_PL_save && entropy_SL_save <= entropy_PW_save)
	{
		/*déterminer_test_save va permettre de déterminer le type de test optimal [général] en fonction de l'entropie (la plus faible) mais aussi en fonction du nombre d'iris trié (le plus nombreux)
		  test_save peut donc prendre les valeurs : 0/1/2/3 par cette fonction*/
		test_save = determiner_test_save(entropy_SL_save, entropy_SW_save, entropy_PL_save, entropy_PW_save, cpt_iris_SL_save, cpt_iris_SW_save, cpt_iris_PL_save, cpt_iris_PW_save, 0, 1, 2 ,3);
	}
	else if(entropy_SW_save <= entropy_SL_save && entropy_SW_save <= entropy_PL_save && entropy_SW_save <= entropy_PW_save)
	{
		test_save = determiner_test_save(entropy_SW_save, entropy_PL_save, entropy_PW_save, entropy_SL_save, cpt_iris_SW_save, cpt_iris_PL_save, cpt_iris_PW_save, cpt_iris_SL_save, 1, 2, 3, 0);
	}
	else if(entropy_PL_save <= entropy_SL_save && entropy_PL_save <= entropy_SW_save && entropy_PL_save <= entropy_PW_save)
	{
		test_save = determiner_test_save(entropy_PL_save, entropy_PW_save, entropy_SL_save, entropy_SW_save, cpt_iris_PL_save, cpt_iris_PW_save, cpt_iris_SL_save, cpt_iris_SW_save, 2, 3, 0, 1);
	}
	else if(entropy_PW_save <= entropy_SL_save && entropy_PW_save <= entropy_SW_save && entropy_PW_save <= entropy_PL_save)
	{
		test_save = determiner_test_save(entropy_PW_save, entropy_SL_save, entropy_SW_save, entropy_PL_save, cpt_iris_PW_save, cpt_iris_SL_save, cpt_iris_SW_save, cpt_iris_PL_save, 3, 0, 1, 2);
	}


	/* On a désormais détermine le type de test le plus optimal et avons également conservé ses caractéristiques correspondantes*/
	/*selon la valeur du type de test : 
	- 0 : SL
	- 1 : SW
	- 2 : PL
	- 3 : PW
	On va enregistrer les caractéristiques correspondantes du test dans un element de type ELEMENT afin de le retourner ensuite à la fonction Creation_arbre.*/
  	if(test_save == 0)
  	{
  	  	/*On assigne maintenant les valeurs à notre element de type ELEMENT*/
  	  	element->test = test_save;
  	  	element->key = key_SL_save;
  	  	element->entropy = entropy_SL_save;
  	  	element->sample = cpt_iris_SL_save;
  	  	element->nbsetosa = nbsetosa_SL_save;
  	  	element->nbversicolor = nbversicolor_SL_save;
  	  	element->nbvirginica = nbvirginica_SL_save;
  	  	element->suivant = NULL;

  	  	/*On conserve également les 2 listes "fils" de la liste_iris dans element*/
  	  	element->liste_iris_gauche = condition_test_liste_fils(element, element->test, liste_iris, 0); // 0 : fils liste iris gauche
  	  	element->liste_iris_droit = condition_test_liste_fils(element, element->test, liste_iris, 1); // 1 : fils liste iris droit
  	  	printf("le test le plus optimal est le test %d \n\n", element->test); //on affiche le test optimal 

  	}

  	/*Même principe pour les autres*/
	else if(test_save == 1)
  	{
  		element->test = test_save;
  	  	element->key = key_SW_save;
  	  	element->entropy = entropy_SW_save;
  	  	element->sample = cpt_iris_SW_save;
  	  	element->nbsetosa = nbsetosa_SW_save;
  	  	element->nbversicolor = nbversicolor_SW_save;
  	  	element->nbvirginica = nbvirginica_SW_save;
   	  	element->suivant = NULL;

   	  	element->liste_iris_gauche = condition_test_liste_fils(element, element->test, liste_iris, 0);
  	  	element->liste_iris_droit = condition_test_liste_fils(element, element->test, liste_iris, 1);
   	  	printf("le test le plus optimal est le test %d \n\n", element->test);

  	}

	else if(test_save == 2)
  	{ 
  		element->test = test_save;
  	  	element->key = key_PL_save;
  	  	element->entropy = entropy_PL_save;
  	  	element->sample = cpt_iris_PL_save;
  	  	element->nbsetosa = nbsetosa_PL_save;
  	  	element->nbversicolor = nbversicolor_PL_save;
  	  	element->nbvirginica = nbvirginica_PL_save;
  	 	element->suivant = NULL;

  	 	element->liste_iris_gauche = condition_test_liste_fils(element, element->test, liste_iris, 0);
  	  	element->liste_iris_droit = condition_test_liste_fils(element, element->test, liste_iris, 1);
  	 	printf("le test le plus optimal est le test %d \n\n", element->test);

  	}

	else if(test_save == 3)
  	{ 
  		element->test = test_save;
  	  	element->key = key_PW_save;
  	  	element->entropy = entropy_PW_save;
  	  	element->sample = cpt_iris_PW_save;
  	  	element->nbsetosa = nbsetosa_PW_save;
  	  	element->nbversicolor = nbversicolor_PW_save;
  	  	element->nbvirginica = nbvirginica_PW_save;
  	  	element->suivant = NULL;

  	  	element->liste_iris_gauche = condition_test_liste_fils(element, element->test, liste_iris, 0);
  	  	element->liste_iris_droit = condition_test_liste_fils(element, element->test, liste_iris, 1);
  	  	printf("le test le plus optimal est le test %d \n\n", element->test);

  	}

  	/*Si le test optimal est égale à -1, cela signifie qu'on si situe au niveau d'une feuille, donc pas de fils liste iris gauche et droit*/
  	else if(test_save == -1) 
  	{
  		element->test = test_save;
  	  	element->key = key_PW_save;
  	  	element->entropy = entropy_PW_save;
  	  	element->sample = cpt_iris_PW_save;
  	  	element->nbsetosa = nbsetosa_PW_save;
  	  	element->nbversicolor = nbversicolor_PW_save;
  	  	element->nbvirginica = nbvirginica_PW_save;
  	  	element->suivant = NULL;

  	  	/*pas de fils liste iris gauche et droit*/
  	  	element->liste_iris_gauche = NULL; 
  	  	element->liste_iris_droit = NULL; 
  	  	printf("le test le plus optimal est le test %d \n\n", element->test);
  	}

  	/*Si le test optimal est égale à -2, cela signifique qu'on se situe au niveau de la racine*/
  	else if(test_save == -2)
  	{
  		element->test = test_save;
  	  	element->key = key_PW_save;
  	  	element->entropy = entropy_PW_save;
  	  	element->sample = cpt_iris_PW_save;
  	  	element->nbsetosa = nbsetosa_PW_save;
  	  	element->nbversicolor = nbversicolor_PW_save;
  	  	element->nbvirginica = nbvirginica_PW_save;
  	  	element->suivant = NULL;

  	  	/*Arbitrairement, on a décidé de conserver le "fils de la base de données" dans un "fils liste iris gauche" qui est en réalité la racine.*/
   	  	element->liste_iris_gauche = condition_test_liste_fils(element, element->test, liste_iris, 0);
   	  	/*Le fils liste iris droit est donc vide car toute la base de données à été récupérée dans la liste gauche*/
  	  	element->liste_iris_droit = NULL; 
  	  	printf("le test le plus optimal est le test %d \n\n", element->test);
  	}
  	
  	/*On retourne enfin l'element contenant le test optimal [general] pour une liste_iris*/
	return element;
}


/*determiner_test_save permet de retourner le meilleur type de test après avoir comparé le nombre d'iris trié pour chaque type de test où l'entropie est minimale et identique*/
int determiner_test_save(double a, double b, double c, double d ,double a_cpt, double b_cpt, double c_cpt, double d_cpt, double a_test, double b_test, double c_test, double d_test)
{
	/* double a/b/c/d correspondent aux valeurs d'entropie pour les 4 types de tests */
	/* double a_cpt/b_cpt/c_cpt/d_cpt correspondent aux nombres d'iris triées pour le type de test */
	/* double a_test/b_test/c_test/d_test correspondent aux types de test SL/SW/PL/PW */

	int test_save = -1; //initialisation de test_save sur une valeur ici, improbable

	if(a==b || a==c || a==d) //dans le cas où il y a au moins deux entropies de même valeur par rapport au type de test "a"
	{
		if(a==b && a==c && a==d)
		{
			test_save = test_cpt_iris(a_cpt, b_cpt, c_cpt, d_cpt, a_test, b_test, c_test, d_test);
		}
		else if(a==b && a==c)
		{
			test_save = test_cpt_iris(a_cpt, b_cpt, c_cpt, 0.0, a_test, b_test, c_test, 0.0);
		}		
		else if(a==b && a==d)
		{
			test_save = test_cpt_iris(a_cpt, b_cpt, 0.0, d_cpt, a_test, b_test, 0.0, d_test);
		}
		else if(a==c && a==d)
		{
			test_save = test_cpt_iris(a_cpt, 0.0, c_cpt, d_cpt, a_test, 0.0, c_test, d_test);
		}
		else if(a==b)
		{
			test_save = test_cpt_iris(a_cpt, b_cpt, 0.0, 0.0, a_test, b_test, 0.0, 0.0);
		}
		else if(a==c)
		{
			test_save = test_cpt_iris(a_cpt, 0.0, c_cpt, 0.0, a_test, 0.0, c_test, 0.0);
		}
		else if(a==d)
		{
			test_save = test_cpt_iris(a_cpt, 0.0, 0.0, d_cpt, a_test, 0.0, 0.0, d_test);
		}
	}
	else //dans le cas où il n'y a pas d'entropie de même valeur par rapport au type de test "a"
	{
		test_save = a_test;
	}

	return test_save;
}

/*Cette fonction permet de comparer les différents compteurs et de retourner le test adapté, c'est à dire le cas où le compteur est le plus grand*/
int test_cpt_iris(double a_cpt, double b_cpt, double c_cpt, double d_cpt, double a_test, double b_test, double c_test, double d_test)
{
	int test_save = -1;

	if(a_cpt >= b_cpt && a_cpt >= c_cpt && a_cpt >= d_cpt)
	{
		test_save = a_test;
	}
	else if(b_cpt >= a_cpt && b_cpt >= c_cpt && b_cpt >= d_cpt)
	{
		test_save = b_test;
	}
	else if(c_cpt >= a_cpt && c_cpt >= b_cpt && c_cpt >= d_cpt)
	{
		test_save = c_test;
	}
	else if(d_cpt >= a_cpt && d_cpt >= c_cpt && d_cpt >= b_cpt)
	{
		test_save = d_test;
	}

	return test_save;
}


/*La fonction condition_test_liste_fils est une fonction qui renvoie une liste d'iris. Cette fonction permet de séparer une liste d'iris en deux selon un type de test*/

LISTE_IRIS* condition_test_liste_fils(ELEMENT* element, int test_save, LISTE_IRIS* liste_iris, int gauche_droit)
{
	/*On initialise des listes d'iris avec comme premier iris, un iris avec des valeurs arbitraires*/
	LISTE_IRIS* liste_iris_gauche = initialiser(0.0, 0.0, 0.0, 0.0, 0); //La liste gauche
	LISTE_IRIS* liste_iris_droit = initialiser(0.0, 0.0, 0.0, 0.0, 0); //La liste droite
	LISTE_IRIS* liste_iris_return = initialiser(0.0, 0.0, 0.0, 0.0, 0); //La liste que nous allons retourner

	/*Dans le cas où le type de test optimal (général) est -1 ou -2, cela signifie qu'on se situe soit à la racine, soit sur une feuille,
	  il n'est donc pas nécessaire de poursuivre en séparant la liste d'iris. 
	  De ce fait, on va simplement récupérer la liste d'iris initiale et la retourner*/
	if(test_save == -1 || test_save == -2)
	{
		IRIS *courant = NULL;
    	courant=liste_iris->premier;//courant est au premier element de la liste

   		while(courant!=NULL)//permet de parcourir la liste_iris
   		{
   			/*On insere à la fin de la liste d'iris gauche, choisie arbitrairement*/
			inserer_fin(liste_iris_gauche, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME); //La fonction inserer fin permet d'ajouter un iris à la fin d'une liste d'iris
		    courant=courant->suivant;//permet d'aller a l'element d'apres dans la liste_iris
    	}
		
		/*On supprime le premier élément de la liste gauche car lors de l'initialisation, nous avons inséré des valeurs n'appartenant pas à la liste gauche*/
		supprimer_debut(liste_iris_gauche);
   		supprimer_debut(liste_iris_droit); //idem liste droite
	}

	/*Si le type de test optimal (général) est 0 (SL), alors on va utiliser les caractéristiques concernant SL de element qui contient toutes les caractéristiques optimales*/
	else if(test_save == 0)
	{
		IRIS *courant = NULL;
    	courant=liste_iris->premier;//courant est au premier element de la liste

   		while(courant!=NULL)//permet de parcourir la liste_iris
   		{
			if(courant->SL <= element->key)
			{
				/*On insère à gauche si l'élement courant lors du parcours de la liste d'iris est inférieure ou égale à la valeur test "key"*/
				inserer_fin(liste_iris_gauche, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
			}
			else
			{
				inserer_fin(liste_iris_droit, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
			}

        courant=courant->suivant;//permet d'aller a l'element d'apres dans la liste_iris
    	}

    	/*On supprime le premier élément de la liste gauche car lors de l'initialisation, nous avons inséré des valeurs n'appartenant pas à la liste gauche*/
    	supprimer_debut(liste_iris_gauche);
   		supprimer_debut(liste_iris_droit); //idem liste droite
	}

	else if(test_save == 1)
	{
		IRIS *courant = NULL;
    	courant=liste_iris->premier;//courant est au premier element de la liste

   		while(courant!=NULL)//permet de parcourir la liste_iris
   		{
			if(courant->SW <= element->key)
			{
				inserer_fin(liste_iris_gauche, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
			}
			else
			{
				inserer_fin(liste_iris_droit, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
			}

        courant=courant->suivant;//permet d'aller a l'element d'apres dans la liste_iris
    	}

		/*On supprime le premier élément de la liste gauche car lors de l'initialisation, nous avons inséré des valeurs n'appartenant pas à la liste gauche*/
    	supprimer_debut(liste_iris_gauche); 
   		supprimer_debut(liste_iris_droit); //idem liste droite
	}

	else if(test_save == 2)
	{
		IRIS *courant = NULL;
    	courant=liste_iris->premier;//courant est au premier element de la liste

   		while(courant!=NULL)//permet de parcourir la liste_iris
   		{
			if(courant->PL <= element->key)
			{
				inserer_fin(liste_iris_gauche, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
			}
			if(courant->PL > element->key)
			{
				inserer_fin(liste_iris_droit, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
			}

        courant=courant->suivant;//permet d'aller a l'element d'apres dans la liste_iris
    	}

    	/*On supprime le premier élément de la liste gauche car lors de l'initialisation, nous avons inséré des valeurs n'appartenant pas à la liste gauche*/
    	supprimer_debut(liste_iris_gauche);
   		supprimer_debut(liste_iris_droit); //idem liste droite
	}
	else if(test_save == 3)
	{
		IRIS *courant = NULL;
    	courant=liste_iris->premier;//courant est au premier element de la liste

   		while(courant!=NULL)//permet de parcourir la liste_iris
   		{
			if(courant->PW <= element->key)
			{
				inserer_fin(liste_iris_gauche, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
			}
			if(courant->PW > element->key)
			{
				inserer_fin(liste_iris_droit, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
			}

        courant=courant->suivant;//permet d'aller a l'element d'apres dans la liste_iris
    	}

    	supprimer_debut(liste_iris_gauche); /*On supprime le premier élément de la liste gauche car lors de l'initialisation, nous avons inséré des valeurs n'appartenant pas à la liste gauche*/
   		supprimer_debut(liste_iris_droit); //idem liste droit
	}


	if(gauche_droit == 0)
	{
		IRIS *courant = NULL;
    	courant=liste_iris_gauche->premier;//courant est au premier element de la liste

   		while(courant!=NULL)//permet de parcourir la liste_iris
   		{
			inserer_fin(liste_iris_return, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
		    courant=courant->suivant;//permet d'aller a l'element d'apres dans la liste_iris
    	}
		supprimer_debut(liste_iris_return);
	}
	
	if(gauche_droit == 1)
	{
		IRIS *courant = NULL;
    	courant=liste_iris_droit->premier;//courant est au premier element de la liste

   		while(courant!=NULL)//permet de parcourir la liste_iris
   		{
			inserer_fin(liste_iris_return, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
		    courant=courant->suivant;//permet d'aller a l'element d'apres dans la liste_iris
    	}
		supprimer_debut(liste_iris_return);
	}

	return liste_iris_return;
}



IRIS* add_new_iris() /* Ajour d'une nouvelle IRIS (pour comparer apres par exemple)*/
{
	/*creation de new_iris*/
	IRIS* new_iris=NULL;
	new_iris = malloc(1*sizeof(IRIS));

	printf("Sepal Length of your Iris: ");
	scanf("%f",&new_iris->SL);

    printf("Sepal Width of your Iris: ");
    scanf("%f",&new_iris->SW);

    printf("Petal Length of your Iris: ");
    scanf("%f",&new_iris->PL);

    printf("Petal Width of your Iris: ");
    scanf("%f",&new_iris->PW);

	printf("Sepal length: %.1f\nSepal width:  %.1f\nPetal length: %.1f\nPetal width: %.1f\n ", new_iris->SL, new_iris->SW, new_iris->PL, new_iris->PW);

	return new_iris;
}
