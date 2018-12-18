/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-D.c                           */
/* Auteurs : Ahmed Amine DAOU                           */
/* Date    : 02/10/2018                                 */
/* version :                                           */
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo1.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/
/*------------------------------------------------*/
#define _USE_MATH_DEFINES
#define NAME_IMG_OUT "image-TpIFT6150-1-Da"
#define NAME_IMG_OUT1 "image-TpIFT6150-1-Db"


/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/



int main(int argc,char **argv)
 {
  int length = 128, width = 128;
  int length1 = 32, width1 = 32;
  float** matrix = fmatrix_allocate_2d(length,width);
  float** matrix1 = fmatrix_allocate_2d(length1, width1);
  
  for(int x=0 ; x < length ; x++)
      for(int y=0 ; y < width ; y++) {
          matrix[x][y] = 128 + 127.0 * cos(2 * M_PI * (25 * x + 31 * y) / 128.0);
      }
  

  
  for(int i=0; i < 128; i += 4)
      for(int j=0; j < 128; j += 4) {

          matrix1[i / 4][j / 4] = matrix[i][j];
      }


  SaveImagePgm(NAME_IMG_OUT, matrix, length, width);	

  SaveImagePgm(NAME_IMG_OUT1, matrix1, length1, width1);


  free_fmatrix_2d(matrix1);
  free_fmatrix_2d(matrix);



 return 0;
}
