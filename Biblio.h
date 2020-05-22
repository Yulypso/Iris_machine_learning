/* Pour la structure ELEMENT*/ 
LISTE *init_pile(LISTE *pile)
{
    pile=(LISTE *)malloc(sizeof(LISTE));
    pile->premier=NULL;

return pile;
}

int est_vide(LISTE *liste)
{

    int vide;
    if(liste->premier==NULL)
    {
        vide=1;
    }
    else
    {
        vide=0;
    }
return vide; //vide =1 si la pile est vide 
}

void empiler(LISTE *pile, int test, double key, double entropy)
{
    ELEMENT *new;
    new=(ELEMENT *)malloc(sizeof(ELEMENT));
    new->test=test;
    new->key=key;
    new->entropy=entropy;

    if(est_vide(pile))
    {
        new->suivant=NULL;
    }
    else 
    {
        new->suivant=pile->premier;
    }
    pile->premier=new;
}

ELEMENT* depiler(LISTE *pile)
{

    ELEMENT *courant;
    ELEMENT *stock;
    stock=malloc(sizeof(ELEMENT));
    
    courant=pile->premier;
    pile->premier=pile->premier->suivant;
    
    stock->test=courant->test;
    stock->key=courant->key;
    stock->entropy=courant->entropy;

    free(courant);

return stock;
}

void afficher_liste (LISTE *liste)
{
    
    if(liste->premier==NULL)
    { //la liste est vide
        exit(-1);
    }
    ELEMENT *courant;
    courant=liste->premier;//courant est au premier element de la liste
    while(courant->suivant!=NULL)//permet de parcourir la liste
    {
        printf("(test:%d | key:%f | entropy:%f) ->", courant->test, courant->key, courant->entropy);
        courant=courant->suivant;//permet d'aller a l'element d'apres
    }
    printf("(test:%d, key:%f, entropy:%f)\n", courant->test, courant->key, courant->entropy);
}


/* Pour la structure IRIS*/ 
LISTE_IRIS* initialiser(float SL, float SW, float PL, float PW, int NAME)
{
    LISTE_IRIS* liste_iris = NULL;
    liste_iris = malloc(1*sizeof(LISTE_IRIS));

    IRIS* nouveau = NULL;
    nouveau = malloc(1*sizeof(IRIS));

    if(liste_iris == NULL || nouveau == NULL)
    {
        exit(-1);
    }
    else
    {
        nouveau->SL= SL;
        nouveau->SW= SW;
        nouveau->PL= PL;
        nouveau->PW= PW;
        nouveau->NAME = NAME;
        liste_iris->premier = nouveau;
        nouveau->suivant = NULL;
    }

    return liste_iris;
}


void inserer_fin(LISTE_IRIS *liste_iris, float SL, float SW, float PL, float PW, int NAME)
{
    IRIS *courant = NULL;
    IRIS *nouveau = NULL;
    courant=liste_iris->premier;
    while(courant->suivant!=NULL)//permet de parcourir la liste
    {
        courant=courant->suivant;
    }
    nouveau=malloc(sizeof(IRIS));
    nouveau->SL=SL;
    nouveau->SW=SW;
    nouveau->PL=PL;
    nouveau->PW=PW;
    nouveau->NAME=NAME;
    nouveau->suivant=NULL;
    courant->suivant=nouveau;
}

void supprimer_debut(LISTE_IRIS* liste_iris)
{ 
    IRIS *courant = NULL; 
    courant = liste_iris->premier; 
    liste_iris->premier = liste_iris->premier->suivant; 

    free(courant);
} 

void copier_liste(LISTE_IRIS* new_liste_iris, LISTE_IRIS* liste_iris)
{
    if(liste_iris == NULL)
    {
        printf("PAS DE COPIE ! attention, liste_iris vide !\n");
    }
    else
    {
        IRIS* courant = NULL;
        courant = liste_iris->premier;

        while(courant != NULL)
        {
            inserer_fin(new_liste_iris, courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
            courant = courant->suivant;
        }
        supprimer_debut(new_liste_iris);
    }
}


void supprimer_liste(LISTE_IRIS* liste_iris)
{
    if(liste_iris == NULL)
    {
        printf("PAS DE SUPPRESSION, la liste_iris est vide\n");
    }
    else
    {
        while(liste_iris->premier->suivant != NULL)
        {
            supprimer_debut(liste_iris);
        }
    }
}

void afficher_iris_liste(LISTE_IRIS *liste_iris)
{
    if(liste_iris == NULL)
    { 
        printf("PAS D'AFFICHAGE, la liste_iris est vide \n");
    }
    else
    {
        IRIS *courant;
        courant=liste_iris->premier;//courant est au premier element de la liste
        while(courant->suivant!=NULL)//permet de parcourir la liste
        {
            printf("(SL:%f | SW:%f | PL:%f | PW:%f | NAME:%d) \n", courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
            courant=courant->suivant;//permet d'aller a l'element d'apres
        }
        printf("(SL:%f | SW:%f | PL:%f | PW:%f | NAME:%d) \n", courant->SL, courant->SW, courant->PL, courant->PW, courant->NAME);
    }
}


int profondeur_arbre(NODE *a, int cpt)
{
    int cptR=0;
    int cptL=0;

    if(a!=NULL)
    {
        if(a->ls!=NULL)
        {
            cptL=1+profondeur_arbre(a->ls, 0);
        }
        if(a->rs !=NULL)
        {
            cptR=1+profondeur_arbre(a->rs, 0);
        }

        if(cptL>=cptR)
        {
            cpt=cptL;
        }
        else
        {
            cpt=cptR;
        }
    return cpt;
    }
    else
    {
        return cpt;
    }
}


int afficher_arbre(NODE *a, int cpt)
{
    if(a!=NULL)
    {
        /*if(a->ls == NULL || a->rs == NULL)
        {
            printf("hauteur de l'arbre: %d\n", cpt);
            printf("///////////////////////////////\n");
            printf("//  C'est un noeud terminal  //\n");
            printf("///////////////////////////////\n");
        }
        else
        {
*/



        printf("Dans le noeud: \n");
        printf("entropy: %.11lf\n",a->entropy);
        if(a->test == 0)
        {
            printf("test: [0]Sepal Length\n");
        }
        else if(a->test == 1)
        {
            printf("test: [1]Sepal Width\n");
        }
        else if(a->test == 2)
        {
            printf("test: [2]Sepal Width\n");
        }
        else if(a->test == 3)
        {
            printf("test: [3]Sepal Width\n");
        }

        printf("key: %.2f\n",a->key);
        printf("sample: %.1f\n",a->sample);

        printf("nbsetosa: %.f\n",a->nbsetosa);
        printf("nbversicolor: %.f\n",a->nbversicolor);
        printf("nbvirginica: %.f\n",a->nbvirginica);

        if(a->liste_iris != NULL)
        {
            printf("hauteur de l'arbre: %d\n", cpt);
            afficher_iris_liste(a->liste_iris);
        }
    }
    else
    {
        printf("NULL");
    }

    if(a->ls!=NULL)
    {
        cpt = 1 + afficher_arbre(a->ls, cpt+1);
    }

    if(a->rs!=NULL)
    {
        cpt = 1 + afficher_arbre(a->rs, cpt+1);
    }

    return cpt;
}
