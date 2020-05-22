/*informations*/
#define base_de_donnees_tab 0 /*Afficher la base de donnees via tab*/
#define base_de_donnees_liste_iris 1 /*Afficher la base de donnees via liste_iris*/
/*Tableaux*/
float tab[150][5];  /*tableau qui va stocker la base de donnees*/


void recuperation_base_donnees()
{
	FILE* fichier = NULL;

	fichier = fopen("./Base de données/Base de données IRIS.txt", "r");
	/*Diane : /home/diane/Documents/INFO/projet/Projet_IRIS_ver_6/Base_de_donnees*/
	/*Kyu : /Users/Kyu/Desktop/Projet_IRIS_ver_6/Base de données/Base de données IRIS.txt*/

	if(fichier != NULL)
	{
		for(int i=0; i<150; i++)
		{	
			for(int j=0; j<5; j++)
			{
				fscanf(fichier, "%f", &tab[i][j]);
			}
		}
	}
	fclose(fichier);
}

LISTE_IRIS* conversion_tbl_liste_iris()
{
	LISTE_IRIS* liste_iris = initialiser(tab[0][0], tab[0][1], tab[0][2], tab[0][3], tab[0][4]);
	for(int i=1; i<150; i++)
	{
		inserer_fin(liste_iris, tab[i][0], tab[i][1], tab[i][2], tab[i][3], tab[i][4]);
	}

	if(base_de_donnees_liste_iris)
	{
		printf("\nAfficher la base de donnees via liste_iris \n");
		afficher_iris_liste(liste_iris); /*Afficher la base de donnees via liste_iris*/
	}
	return liste_iris;
}



void informations_IRIS() /*Afficher la base de donnees via tab*/
{
	if(base_de_donnees_tab)
	{
		printf("\nAfficher la base de donnees via tab \n");
		printf("Sepal Length | Sepal Width | Petal Length | Petal Width | Iris Name\n");

		for(int k=0; k<150; k++)
		{
			for(int l=0; l<5; l++)
			{
				if(l != 4)
				{
					printf("   %f   ", tab[k][l]);
				}
				else
				{
					if(tab[k][4] == 0)
					{
						printf("   Iris_setosa");
					}

					if(tab[k][4] == 1)
					{
						printf("   Iris_versicolor");
					}

					if(tab[k][4] == 2)
					{
						printf("   Iris_virginica");
					}
				}
			}
		printf("\n");
		}
	printf("\n");
	}
}

