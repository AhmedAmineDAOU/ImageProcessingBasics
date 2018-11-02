/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Aa.c                           */
/* Auteur  :                                            */
/* Date    : 01/09/2004                                 */
/* version :                                            */
/* langage : C                                          */
/* labo    : DIRO                                       */
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
#define NAME_IMG_IN "D1r"

#define NAME_IMG_OUT "image-TpIFT6150-1-Ac"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/
/* permute deux point passés en parametre dans une matrice 
 * i,j,k,l parametres et 4 entiers (coordonnees de deux points a permuter)
 * matrice du spectre d'amplitude a modifier
 */
char * concat(const char * s1,
  const char * s2) {
  char * result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
  // in real code you would check for errors in malloc here
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

void permute(float ** mat, int i, int j, int k, int l) {
  float tmp = mat[k][l];
  mat[k][l] = mat[i][j];
  mat[i][j] = tmp;

}
/* modifie la matrice du spectre pour ramener le centre au point (0,0) et
 * non au zero informatique
 * prend en parametre le nom de la matrice(comme demandé dans bareme_1.dat), sa longueur et sa largeur
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
  for ( i = 0; i < * length; i++ )
    for ( j = 0; j < * width; j++ ) {
      fread( &
        var, 1, 1, fic);
      mat[i][j] =
        var;
    }
  /*---fermeture du fichier---*/
  fclose(fic);

  /*debut modification*/
  /*permuter les points du carré 1 avec celles de 4*/
  for ( i = 0; i < * length / 2; i++ )
    for ( j = 0; j < * width / 2; j++ ) {
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
  int i, j;
  int length, width;
  float ** MatriceImgR;
  float ** MatriceImgI;
  float ** MatriceImgM;
  float ** MatriceHarmonique;
  char * name_img_out;
  /*Allocation memoire pour la matrice image*/
  MatriceImgR = LoadImagePgm(argv[1], & length, & width);
  MatriceImgI = fmatrix_allocate_2d(length, width);
  MatriceImgM = fmatrix_allocate_2d(length, width);
  MatriceHarmonique = fmatrix_allocate_2d(length, width);

  /*Initialisation a zero de toutes les matrices */
  for (i = 0; i < length; i++)
    for (j = 0; j < width; j++) {
      MatriceImgI[i][j] = 0.0;
      MatriceImgM[i][j] = 0.0;
    }

  /*FFT*/
  FFTDD(MatriceImgR, MatriceImgI, length, width);

  /*Module*/
  Mod(MatriceImgM, MatriceImgR, MatriceImgI, length, width);
  Recal(MatriceImgM, length, width);
  /*application du log(1+MatImgM[][] sur la matrice du module)*/
  for (i = 0; i < length; i++)
    for (j = 0; j < width; j++) {
      MatriceImgM[i][j] = log(1 + MatriceImgM[i][j]);
    }

  /*Pour la visualisation visu*/
  Recal(MatriceImgM, length, width);
  Mult(MatriceImgM, 10.0, length, width);

  if (strcmp("D1r", argv[1]) == 0) {

    /*enregistrer temporairement) la photo du spectre non centré (avec les hautes frequences) */
    SaveImagePgm(NAME_IMG_OUT, MatriceImgM, length, width);
    /*centrer le spectre (avec les hautes frequences )*/
    MatriceHarmonique = harmoniser(NAME_IMG_OUT, & length, & width);
    /*ecraser l'ancien spectre par le bon*/
    SaveImagePgm(NAME_IMG_OUT, MatriceHarmonique, length, width);
  } else {
    char * tmp = concat("image-TpIFT6150-1-Ad", argv[1]);
    name_img_out = concat(tmp, "-spec");

    /*enregistrer temporairement) la photo du spectre non centré (avec les hautes frequences) */
    SaveImagePgm( name_img_out, MatriceImgM, length, width );

    /*centrer le spectre (avec les hautes frequences )*/
    MatriceHarmonique = harmoniser(name_img_out, & length, & width);

    /*ecraser l'ancien spectre par le bon*/
    SaveImagePgm(name_img_out, MatriceHarmonique, length, width);
  }

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgR);
  free_fmatrix_2d(MatriceImgI);
  free_fmatrix_2d(MatriceImgM);

  /*Commande systeme: visualisation du bon spectre de Ingout.pgm*/
  if (strcmp("D1r", argv[1]) == 0) {
    system("display image-TpIFT6150-1-Ac.pgm&");
  } else if (strcmp("D11r", argv[1]) == 0 || strcmp("D46r", argv[1]) == 0) {
    char * tmp = concat("display ", name_img_out);
    char * command = concat(tmp, ".pgm&");
    // char* fullcommand=concat(command,"&");
    system(command);
  }

  /*retour sans probleme*/
  printf("\n C'est fini ... \n\n\n");
  return 0;
}