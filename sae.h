/**
 * \file sae.h
 * \brief Fichier contenant tous les prototypes des fonctions
 * \author MIELCAREK Félix
 * \date 17 novembre 2021
 * 
 * Le fichier inclut les différentes librairies nécessaires à l'utilisation des outils présents dans l'application.
 * Les prototypes sont placées dans l'ordre d'apparition des fonctions dans le fichier 'sae.c'. 
 */ 

//	Bibliothèques à inclure

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//	Constantes

#define TTP 10  // TTP = Taille Tableaux Physique = taille physique des tableaux des éléments des stocks
#define TTN 31  // TTN = Taille Tableaux Nom = taille physique des tableaux des noms des éléments du stocks

//	Prototypes de fonction

void global(void);
void chargStock(int tabStockRefTel[],char tabStockNomTel[][31],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel);
void affichMenu(void);
void ajoutStock(int tabStockRefTel[],char tabStockNomTel[][TTN],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel);
void etatStock(int tabStockRefTel[],char tabStockNomTel[][31],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel);
void traitementCommande(int tabStockRefTel[],char tabStockNomTel[][31],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel,int recapVentesTel[][2],int* nombreTelVentes);
void recapitulatifVentes(int tabStockRefTel[],char tabStockNomTel[][31],float tabStockPrixTel[],int* nombreTel,int recapVentesTel[][2],int* nombreTelVentes);
void approvisionnement(char tabStockNomTel[][31],int tabStockQteTel[],int tabStockSeuilTel[],int* nombreTel);
void sauvStock(int tabStockRefTel[],char tabStockNomTel[][31],int tabStockQteTel[],float tabStockPrixTel[],int tabStockSeuilTel[],int* nombreTel);
void devisCommande(int tabStockRefTel[],char tabStockNomTel[][TTN],float tabStockPrixTel[],int* nombreTel);
int rechercheRefProduit(int tabStockRefTel[],int* nombreTel,int choix);

