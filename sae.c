/**
 * \file sae.c
 * \brief Fichier contenant toutes les fonctions de l'application
 * \author MIELCAREK Félix
 * \date 17 novembre 2021
 * 
 * Le fichier inclut dès le début le fichier sae.h.
 * Les fonctions sont placées dans l'ordre d'apparition dans le menu. La fonction de rechercheRefProduit est à la fin car elle peut être utilisée par différentes fonctions. 
 */ 

//	Fichier à inclure

#include "sae.h"   

//	Fonctions

/**
 * \brief Appelle les autres fonctions de l'application 
 * 
 * La fonction est la première appelée, elle mets donc en place tout ce qui sera necessaire à l'utilisation des fonctions: les différents tableaux et variables et les appels des fonctions selon le choix.
 */

void global(void){
    int tabStockRefTel[TTP];
    char tabStockNomTel[TTP][TTN];
    int tabStockQteTel[TTP];
    float tabStockPrixTel[TTP];
    int tabStockSeuilTel[TTP];
    char choix;
    int nombreTel;
    int nombreTelVentes=0;

    chargStock(tabStockRefTel,tabStockNomTel,tabStockQteTel,tabStockPrixTel,tabStockSeuilTel,&nombreTel);

    int recapVentesTel[TTP][2];    

    while(1){
        affichMenu();

        if(choix=='x') printf("/!\\ Choix invalide, retapez votre choix: ");
        else printf("Tapez votre choix: ");
        scanf("%c%*c",&choix);

        if(choix=='1') ajoutStock(tabStockRefTel,tabStockNomTel,tabStockQteTel,tabStockPrixTel,tabStockSeuilTel,&nombreTel);
        else if(choix=='2') etatStock(tabStockRefTel,tabStockNomTel,tabStockQteTel,tabStockPrixTel,tabStockSeuilTel,&nombreTel);
        else if(choix=='3') traitementCommande(tabStockRefTel,tabStockNomTel,tabStockQteTel,tabStockPrixTel,tabStockSeuilTel,&nombreTel,recapVentesTel,&nombreTelVentes);
        else if(choix=='4') recapitulatifVentes(tabStockRefTel,tabStockNomTel,tabStockPrixTel,&nombreTel,recapVentesTel,&nombreTelVentes);
        else if(choix=='5') approvisionnement(tabStockNomTel,tabStockQteTel,tabStockSeuilTel,&nombreTel);
        else if(choix=='6') sauvStock(tabStockRefTel,tabStockNomTel,tabStockQteTel,tabStockPrixTel,tabStockSeuilTel,&nombreTel);
        else if(choix=='7') devisCommande(tabStockRefTel,tabStockNomTel,tabStockPrixTel,&nombreTel);
        else if(choix=='0') break;
        else choix='x';
    }
}

/**
 * \brief Charge les données dans les tableaux
 * \param [out] tabStockRefTel tableau contenant les références des produits
 * \param [out] tabStockNomTel tableau contenant les noms des produits
 * \param [out] tabStockQteTel tableau contenant les quantités pour chaque produit
 * \param [out] tabStockPrixTel tableau contenant les prix unitaires des produits
 * \param [out] tabStockSeuilTel tableau contenant les seuils des quantités minimums des produits
 * \param [out] nombreTel pointeur sur le nombre de produit différents
 * 
 * La fonction permet de charger les données dans les tableaux pour qu'elles soient manipulables.
 * Le fichier de données est 'stock.don'.
 * Si le programme est lancé pour la première fois le fichier de données sera vide et les tableaux aussi.
 */

void chargStock(int tabStockRefTel[],char tabStockNomTel[][TTN],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel)
{
    FILE *stock;
    int i=0;

    if((stock=fopen("stock.don", "r")) == NULL) {
        printf("Problème dans le chargement du fichier.\n");
        exit(0);
    }

    while(feof(stock)==0){
        fscanf(stock,"%d %[^\t]%*c %d %f %d",&tabStockRefTel[i],&tabStockNomTel[i-1][TTN],&tabStockQteTel[i],&tabStockPrixTel[i],&tabStockSeuilTel[i]);
        i++;
    }
    *nombreTel=i-1;

    fclose(stock);
}

/**
 * \brief affiche le menu
 * 
 * La fonction d'affichage du menu est bien séparé de l'appel des fonctions pour rendre le code plus clair.
 */

void affichMenu(void){
    printf("\n----------------------------------------------\n");
    printf("-------------------- MENU --------------------\n");
    printf("------------------ PRINCIPAL -----------------\n");
    printf("----------------------------------------------\n\n");
    printf("\t1\tAjouter des produits dans le stock\n");
    printf("\t2\tEtat du stock\n");
    printf("\t3\tTraitement d'une commande\n");
    printf("\t4\tRécapitulatif des ventes\n");
    printf("\t5\tApprovisionnement\n");
    printf("\t6\tSauvegarder dans un fichier\n");
    printf("\t7\tDevis d'une commande\n");
    printf("\n\t0\tSortir\n\n");
}

/**
 * \brief Permet d'ajouter un produit dans le stock
 * \param [in,out] tabStockRefTel tableau contenant les références des produits
 * \param [in,out] tabStockNomTel tableau contenant les noms des produits
 * \param [in,out] tabStockQteTel tableau contenant les quantités pour chaque produit
 * \param [in,out] tabStockPrixTel tableau contenant les prix unitaires des produits
 * \param [in,out] tabStockSeuilTel tableau contenant les seuils des quantités minimums des produits
 * \param [in,out] nombreTel pointeur sur le nombre de produit différents
 * 
 * Lors de l'ajout d'un produit dans le stock, si le produit est déjà présent, seule la quantité sera ajoutée et non pas tout un nouveau produit.
 * La fonction contrôle la cohérence des valeurs entrantes.
 * La fonction permet un nom composé de plusieurs mots.
 * La référence des produits est générée automatiquement pour une meilleure cohérence.
 */

void ajoutStock(int tabStockRefTel[],char tabStockNomTel[][TTN],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel){
    int nouvRefTel=1,nouvQteTel,nouvSeuilTel,testNouvProduit=0;
    char nouvNomTel[TTN];
    float nouvPrixTel;

    printf("\n----------------------------------------------\n");
    printf("---------- AJOUT D'UN NOUVEAU PRODUIT --------\n");
    printf("----------------------------------------------\n\n");

    printf("Entrez le nom du nouveau produit: ");
    scanf("%[^\n]%*c",nouvNomTel);
    
    for (int i = 0; i < *nombreTel; i++)
    {
        if (strcmp(tabStockNomTel[i],nouvNomTel)==0)
        {
            printf("Ce produit est déjà présent dans le stock. Quelle quantité de \"%s\" voulez vous ajouter: ",nouvNomTel);
            scanf("%d%*c",&nouvQteTel);
            while (nouvQteTel<=0)
            {
                printf("Veuillez entrer une quantité positive: ");
                scanf("%d%*c",&nouvQteTel);
            }
            
            tabStockQteTel[i]=tabStockQteTel[i]+nouvQteTel;

            testNouvProduit=1;
            break;
        }
        
    }
    if (testNouvProduit==0)
    {
        printf("Entrez la quantité du nouveau produit: ");
        scanf("%d%*c",&nouvQteTel);
        while (nouvQteTel<=0)
        {
            printf("Veuillez entrer une quantité positive: ");
            scanf("%d%*c",&nouvQteTel);
        }
        printf("Entrez le prix du nouveau produit: ");
        scanf("%f%*c",&nouvPrixTel);
        while (nouvPrixTel<=0)
        {
            printf("Veuillez entrer un prix positif: ");
            scanf("%f%*c",&nouvPrixTel);
        }
        printf("Entrez le seuil de sûreté du nouveau produit: ");
        scanf("%d%*c",&nouvSeuilTel);
        while (nouvSeuilTel<=0)
        {
            printf("Veuillez entrer un seuil positif: ");
            scanf("%d%*c",&nouvSeuilTel);
        }

        for (int i = 0; i < *nombreTel; i++){
            if (tabStockRefTel[i]==nouvRefTel) nouvRefTel++;
        }

        printf("La référence attribuée au nouveau produit est: %d\n",nouvRefTel);

        tabStockRefTel[*nombreTel]=nouvRefTel;
        strcpy(tabStockNomTel[*nombreTel],nouvNomTel);
        tabStockQteTel[*nombreTel]=nouvQteTel;
        tabStockPrixTel[*nombreTel]=nouvPrixTel;
        tabStockSeuilTel[*nombreTel]=nouvSeuilTel;
        *nombreTel+=1;
    }   
    
}

/**
 * \brief Affiche le contenu du stock
 * \param [in] tabStockRefTel tableau contenant les références des produits
 * \param [in] tabStockNomTel tableau contenant les noms des produits
 * \param [in] tabStockQteTel tableau contenant les quantités pour chaque produit
 * \param [in] tabStockPrixTel tableau contenant les prix unitaires des produits
 * \param [in] tabStockSeuilTel tableau contenant les seuils des quantités minimums des produits
 * \param [in] nombreTel pointeur sur le nombre de produit différents
 *  
 * Si le stock est vide un message correspondant est affiché.
 */

void etatStock(int tabStockRefTel[],char tabStockNomTel[][TTN],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel){
    printf("\n----------------------------------------------\n");
    printf("---------------- ETAT DU STOCK ---------------\n");
    printf("----------------------------------------------\n\n");

    if (*nombreTel==0)
    {
        printf("Le stock est vide\n");
    }
    else{
        printf("\nRéference\tNom du produit\t\tQuantité\tPrix\tSeuil de quantité\n\n");
        for (int i = 0; i < *nombreTel; i++){
            printf("%-9d\t%-20s\t%-8d\t%.2f\t%-17d\n",tabStockRefTel[i],tabStockNomTel[i],tabStockQteTel[i],tabStockPrixTel[i],tabStockSeuilTel[i]);
        }
    }
}

/**
 * \brief Permet le traitement d'une commande
 * \param [in,out] tabStockRefTel tableau contenant les références des produits
 * \param [in,out] tabStockNomTel tableau contenant les noms des produits
 * \param [in,out] tabStockQteTel tableau contenant les quantités pour chaque produit
 * \param [in,out] tabStockPrixTel tableau contenant les prix unitaires des produits
 * \param [in,out] tabStockSeuilTel tableau contenant les seuils des quantités minimums des produits
 * \param [in,out] nombreTel pointeur sur le nombre de produit différents
 * \param [in,out] recapVentesTel tableau à 2 dimensions contenant les  références des produits ayants eu au moins une commande ainsi que la quantité totale vendue pour chacun de ces produits 
 * \param [in,out] nombreTelVentes pointeur sur le nombre de produit différents ayant eu au moins une commande
 * 
 * La fonction affiche un message d'erreur et annule la commande si la quantité présente dans le stock n'est pas suffisante.
 * Si le seuil est dépassé alors la commande est quand même passée mais un message de prévention est affiché.
 * La fonction met à jour la table de récapitulatif des ventes en prévision de l'appel de la fonction correspondante.
 * La fonction contrôle la cohérence des valeurs entrantes.
 */

void traitementCommande(int tabStockRefTel[],char tabStockNomTel[][TTN],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel,int recapVentesTel[][2],int* nombreTelVentes){
    int choix,emplacement,quantite;
    printf("\n----------------------------------------------\n");
    printf("---------- TRAITEMENT D'UNE COMMANDE ---------\n");
    printf("----------------------------------------------\n\n");
    printf("Entrez la référence du produit : ");
    scanf("%d%*c",&choix);
    emplacement=rechercheRefProduit(tabStockRefTel,nombreTel,choix);
    if(emplacement==-1){
        printf("La référence du produit n'existe pas.\n");
    }
    else{
        printf("Entrez la quantité du produit \"%s\" demandée : ",tabStockNomTel[emplacement]);
        scanf("%d%*c",&quantite);
        while (quantite<=0)
        {
            printf("Veuillez entrer une quantité positive: ");
            scanf("%d%*c",&quantite);
        }
        if (tabStockQteTel[emplacement]<quantite) printf("/!\\ La quantité demandée est supérieur à celle du stock, la commande ne peut aboutir\n");
        else{
            printf("Le montant de la commande est de : %.2f €\n",tabStockPrixTel[emplacement]*quantite);
            tabStockQteTel[emplacement]=tabStockQteTel[emplacement]-quantite;
            if (tabStockQteTel[emplacement]<tabStockSeuilTel[emplacement]){
                printf("/!\\ Le seuil de quantité pour le produit \"%s\" est dépassé, veillez faire un approvisionnement\n",tabStockNomTel[emplacement]);
            }

            int premiereVente=0;
            if (*nombreTelVentes==0){
                recapVentesTel[0][0]=choix;
                recapVentesTel[0][1]=quantite;
                *nombreTelVentes=1;
            }
            else{
                for(int i =0;i<*nombreTelVentes;i++){
                    if (choix==recapVentesTel[i][0]){
                        recapVentesTel[i][1]=recapVentesTel[i][1]+quantite;
                        premiereVente=1;
                        break;
                    }
                }
                if (premiereVente==0){
                    recapVentesTel[*nombreTelVentes][0]=choix;
                    recapVentesTel[*nombreTelVentes][1]=quantite;
                    *nombreTelVentes=*nombreTelVentes+1;
                }  
            }
        }
    }
}

/**
 * \brief Affiche les ventes réalisées
 * \param [in] tabStockRefTel tableau contenant les références des produits
 * \param [in] tabStockNomTel tableau contenant les noms des produits
 * \param [in] tabStockPrixTel tableau contenant les prix unitaires des produits
 * \param [in] nombreTel pointeur sur le nombre de produit différents
 * \param [in] recapVentesTel tableau à 2 dimensions contenant les  références des produits ayants eu au moins une commande ainsi que la quantité totale vendue pour chacun de ces produits 
 * \param [in] nombreTelVentes pointeur sur le nombre de produit différents ayant eu au moins une commande
 * 
 * La fonction n'affiche le récapitulatif des ventes réalisée depuis le lancement actuel de l'application, pas depuis le premier lancement de l'application.
 * Si aucune vente n'a été réalisé un message correspondant est affiché.
 */

void recapitulatifVentes(int tabStockRefTel[],char tabStockNomTel[][TTN],float tabStockPrixTel[],int* nombreTel,int recapVentesTel[][2],int* nombreTelVentes){
    printf("\n----------------------------------------------\n");
    printf("----------- RECAPITULATIF DES VENTES ---------\n");
    printf("----------------------------------------------\n\n");

    if (*nombreTelVentes==0)
    {
        printf("Aucune vente n'a été réalisé\n");
    }
    else{
        printf("\nRéférence\tNom du produit\t\tPrix\tNombre de ventes\tChiffre d'affaire\n");

        int emplacement;

        for (int i = 0; i < *nombreTelVentes; i++){
                emplacement=rechercheRefProduit(tabStockRefTel,nombreTel,recapVentesTel[i][0]);
                printf("%-9d\t%-20s\t%.2f\t%-16d\t%-17.2f\n",tabStockRefTel[emplacement],tabStockNomTel[emplacement],tabStockPrixTel[emplacement],recapVentesTel[i][1],tabStockPrixTel[emplacement]*recapVentesTel[i][1]);
        }
    }
    
    
}

/**
 * \brief Affiche les produits nécessitant un approvisionnement
 * \param [in] tabStockNomTel tableau contenant les noms des produits
 * \param [in] tabStockQteTel tableau contenant les quantités pour chaque produit
 * \param [in] tabStockSeuilTel tableau contenant les seuils des quantités minimums des produits
 * \param [in] nombreTel pointeur sur le nombre de produit différents
 * 
 * Si aucun produit n'a une quantité inférieur à son seuil un message correspondant est affiché.
 */

void approvisionnement(char tabStockNomTel[][TTN],int tabStockQteTel[],int tabStockSeuilTel[],int* nombreTel){
    printf("\n----------------------------------------------\n");
    printf("-------------- APPROVISIONNEMENT -------------\n");
    printf("----------------------------------------------\n\n");

    int besoin=0;

    for (int i = 0; i < *nombreTel; i++){
        if (tabStockQteTel[i]<tabStockSeuilTel[i]){
            printf("Le produit \"%s\" doit être approvisionné d'au moins %d unité(s)\n",tabStockNomTel[i],tabStockSeuilTel[i]-tabStockQteTel[i]);
            besoin=1;
        }        
    }

    if (besoin==0){
        printf("Les stocks de tous les produits sont convenables\n");
    }
       
}

/**
 * \brief Sauvegarde les données dans les tableaux
 * \param [in] tabStockRefTel tableau contenant les références des produits
 * \param [in] tabStockNomTel tableau contenant les noms des produits
 * \param [in] tabStockQteTel tableau contenant les quantités pour chaque produit
 * \param [in] tabStockPrixTel tableau contenant les prix unitaires des produits
 * \param [in] tabStockSeuilTel tableau contenant les seuils des quantités minimums des produits
 * \param [in] nombreTel pointeur sur le nombre de produit différents
 * 
 * La fonction permet d'enregistrer les données dans les tableaux pour qu'elles soient réutilisables lors du prochain lancement de l'application.
 * Le fichier de données est le même que lors du chargement: 'stock.don'.
 */

void sauvStock(int tabStockRefTel[],char tabStockNomTel[][TTN],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel)
{
    FILE *stock;
    int i=0;

    if((stock=fopen("stock.don", "w")) == NULL) {
        printf("Problème dans le chargement du fichier.\n");
    }

    for (i = 0; i < *nombreTel; i++){
        fprintf(stock,"%d\t%s\t%d\t%.2f\t%d\n",tabStockRefTel[i],tabStockNomTel[i],tabStockQteTel[i],tabStockPrixTel[i],tabStockSeuilTel[i]);
    }

    fclose(stock);
}

/**
 * \brief Permet d'effectuer et d'afficher un devis
 * \param [in] tabStockRefTel tableau contenant les références des produits
 * \param [in] tabStockNomTel tableau contenant les noms des produits
 * \param [in] tabStockPrixTel tableau contenant les prix unitaires des produits
 * \param [in] nombreTel pointeur sur le nombre de produit différents
 * 
 * La fonction ne prend pas en compte la quantité réelle de produit puisqu'un devis n'est qu'à titre indicatif.
 * La fonction contrôle la cohérence des valeurs entrantes.
 * Contrairement à une vraie commande on  peut ici faire un devis sur plusieurs produits.
 */

void devisCommande(int tabStockRefTel[],char tabStockNomTel[][TTN],float tabStockPrixTel[],int* nombreTel){
    int tabDevisTel[TTN][2],nombreTelDevis=0,choix,quantite,emplacement;
    float prixTotal=0;

    printf("\n----------------------------------------------\n");
    printf("------------- DEVIS D'UNE COMMANDE -----------\n");
    printf("----------------------------------------------\n\n");

    printf("Entrez la référence du premier produit: ");
    scanf("%d%*c",&choix);
    while((emplacement=rechercheRefProduit(tabStockRefTel,nombreTel,choix))==-1){
        printf("La référence entrée n'existe pas, veuillez retaper une référence: ");
        scanf("%d%*c",&choix);
    }
    printf("Entrez la quantité du premier produit: ");
    scanf("%d%*c",&quantite);
    while (quantite<=0)
    {
        printf("Veuillez entrer une quantité positive: ");
        scanf("%d%*c",&quantite);
    }    
    tabDevisTel[nombreTelDevis][0]=choix;
    tabDevisTel[nombreTelDevis][1]=quantite;
    nombreTelDevis=1;

    while (choix!=-1)
    {
        printf("\nEntrez la référence d'un nouveau produit (ou \'-1\' pour terminer le devis): ");
        scanf("%d%*c",&choix);
        while((emplacement=rechercheRefProduit(tabStockRefTel,nombreTel,choix))==-1 && choix!=-1){
            printf("La référence entrée n'existe pas, veuillez retaper une référence: ");
            scanf("%d%*c",&choix);            
        }
        if (choix==-1) break;
        else{
            printf("Entrez la quantité du %dème produit: ",nombreTelDevis+1);
            scanf("%d%*c",&quantite);
            while (quantite<=0)
            {
                printf("Veuillez entrer une quantité positive: ");
                scanf("%d%*c",&quantite);
            }
            tabDevisTel[nombreTelDevis][0]=choix;
            tabDevisTel[nombreTelDevis][1]=quantite;
            nombreTelDevis++;
        }        
    }
    printf("\nDevis final : \n\n");
    printf("        \tRéférence\tNom du produit\t\tQuantité\tPrix\n\n");
    for (int i = 0; i < nombreTelDevis; i++)
    {
        emplacement=rechercheRefProduit(tabStockRefTel,nombreTel,tabDevisTel[i][0]);
        printf("        \t%-9d\t%-20s\t%-8d\t%.2f\n",tabDevisTel[i][0],tabStockNomTel[emplacement],tabDevisTel[i][1],tabDevisTel[i][1]*tabStockPrixTel[emplacement]);
        prixTotal+=tabDevisTel[i][1]*tabStockPrixTel[emplacement];
    }
    quantite=0;
    for (int i = 0; i < nombreTelDevis; i++)
    {
        quantite+=tabDevisTel[i][1];
    }
    printf("_____________________________________________________________________________________\n");
    printf("TOTAL ->\t%-9d\t                    \t%-8d\t%.2f\n",nombreTelDevis,quantite,prixTotal);
}

/**
 * \brief Cherche l'emplacement d'une référence dans les tableaux 
 * \param [in] tabStockRefTel tableau contenant les références des produits
 * \param [in] nombreTel pointeur sur le nombre de produit différents
 * \param [in] choix référence à chercher
 * \return emplacement dans le tableau référence de la référence voulu
 * 
 * Même si en théorie les références sont créées de sorte à ce que les tableaux soient triés en fonction de la référence du produit, la fonction existe dans le cas où le fichier de données a été modifié manuellement par exemple.
 */

int rechercheRefProduit(int tabStockRefTel[],int* nombreTel,int choix){
    for (int i = 0; i < *nombreTel; i++){
        if (choix==tabStockRefTel[i]) return i;
    }
    return -1;
}