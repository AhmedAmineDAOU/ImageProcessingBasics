/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Ab.c                           */
/* Auteur  :                                            */
/* Date    : 01/10/2018                                */
/* version :                                            */
/* langage : C                                          */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
/*############################################
Ahmed Amine DAOU ahmed.amine.daou@umontreal.ca
Wrushabh Warshe wrushabh.warshe@umontreal.ca
#############################################*/
#include <stdio.h> 
#include <math.h> 
#include <stdlib.h> 
#include <string.h>

#include "FonctionDemo1.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/
/*------------------------------------------------*/
#define NAME_IMG_IN "image-TpIFT6150-1-Aa" //image d'entree (spectre a harmoniser)

#define NAME_IMG_OUT "image-TpIFT6150-1-Ab" //image de sortie (spectre harmonisé)

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/
/* permute deux point passés en parametre dans une matrice 
 * i,j,k,l parametres et 4 entiers (coordonnees de deux points a permuter)
 * matrice du spectre d'amplitude a modifier
 */

void permute(float ** mat, int i, int j, int k, int l) {
  float tmp = mat[k][l];
  mat[k][l] = mat[i][j];
  mat[i][j] = tmp;

}
/* modifie la matrice du spectre pour ramener le centre au point (0,0) et
 * non au zero informatique
 * prend en parametre le nom de la matrice, sa longueur et sa largeur
 */
float ** harmoniser(char * name, int * length, int * width) {
  int i, j;
  unsigned char
  var;
  char buff[NBCHAR];
  float ** mat;
  char stringTmp1[NBCHAR], stringTmp2[NBCHAR];
  int ta1, ta2, ta3;
  FILE * fic;

  /*-----nom du fichier pgm (du spectre d'amplitude non modifié)-----*/
  strcpy(buff, name);
  strcat(buff, ".pgm");
  printf("---> Ouverture de %s", buff);

  /*----ouverture du fichier (du spectre d'amplitude non modifié)----*/
  fic = fopen(buff, "r");
  if (fic == NULL) {
    printf("\n- Grave erreur a l'ouverture de %s  -\n", buff);
    exit(-1);
  }

  /*--recuperation de l'entete--*/
  fgets(stringTmp1, 100, fic);
  fgets(stringTmp2, 100, fic);
  fscanf(fic, "%d %d", & ta1, & ta2);
  fscanf(fic, "%d\n", & ta3);

  /*--affichage de l'entete--*/
  printf("\n\n--Entete--");
  printf("\n----------");
  printf("\n%s%s%d %d \n%d\n", stringTmp1, stringTmp2, ta1, ta2, ta3);

  * length = ta1;
  * width = ta2;

  /*allocation de memoire pour la matrice du spectre d'amplitude a modifier*/
  mat = fmatrix_allocate_2d( * length, * width);

  /*--chargement dans la matrice source (spectre non harmonisé)--*/
  for (i = 0; i < * length; i++)
    for (j = 0; j < * width; j++) {
      fread( &
        var, 1, 1, fic);
      mat[i][j] =
        var;
    }
  /*---fermeture du fichier---*/
  fclose(fic);

  /*debut modification*/
  /*permuter les points du carré 1 avec celles de 4*/
  for (i = 0; i < * length / 2; i++)
    for (j = 0; j < * width / 2; j++) {
      permute(mat, i, j, i + * length / 2, j + * length / 2);
    }

  /*permuter les points du carré 2 avec celles du 3*/
  for (i = * length / 2; i < * length; i++)
    for (j = 0; j < * width / 2; j++) {
      permute(mat, i, j, i - * length / 2, j + * length / 2);
    }

  /* fin modification*/
  return (mat);
}

int main(int argc, char ** argv) {
  int i, j; //iterateurs
  int length, width; //longueur largeur
  float ** MatriceInitiale;
  float ** MatriceHarmonique;
  /*Allocation memoire pour la matrice spectre d'amplitude */
  /*load spectre d'amplitude*/
  MatriceInitiale = LoadImagePgm(NAME_IMG_IN, & length, & width);

  /*Allocation memoire pour la matrice spectre d'amplitude harmonique */
  MatriceHarmonique = fmatrix_allocate_2d(length, width);

  /*Initialisation de la matrice associe a l'image initiale */
  for ( i = 0; i < length ; i++ )

    for ( j = 0; j < width ; j++ ) {
      MatriceHarmonique[i][j] = 0.0;
    }
  
  /*harmoniser l'image du spectre d'amplitude image-TpIFT6150-1-Aa*/
  MatriceHarmonique = harmoniser( NAME_IMG_IN, & length, & width );


  /*Une autre maniere de le faire

  void recalagecyclique(float** Matrix, int length, int width) {
    int x,y;
    for(x=0; x<length; x++)
        for(y=0; y<width; y++)
                Matrix[x][y] = Matrix[x][y]*pow(-1,(x+y)) ;
}*/

  /*enregistrer l'image sous le nom de NAME_IMG_OUT=image-TpIFT6150-1-Ab*/
  SaveImagePgm( NAME_IMG_OUT, MatriceHarmonique, length, width );

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d( MatriceInitiale );
  free_fmatrix_2d( MatriceHarmonique );

  /*Commande systeme: visualisation de Ingout.pgm*/
  system( "display image-TpIFT6150-1-Ab.pgm&" );

  /*retour sans probleme*/
  printf( "\n C'est fini ... \n\n\n" );
  return 0;
}

