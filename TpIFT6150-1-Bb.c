/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Bb.c                           */
/* Auteurs : Ahmed Amine DAOU                           */
/* Date    : 02/10/2018                                 */
/* version :                                            */
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
#define NAME_IMG_IN  "D1r"

#define NAME_IMG_OUT "image-TpIFT6150-1-Ba"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/



void annulerzone(float** Matrix,int min,int max,int length,int width){
int i,j;
for(i=0; i<length; i++) {
          for(j=0; j<width; j++) {
              if((i > width/2 - min && i < width/2 + min && j > width/2 - min && j < width/2 + min) ||
                 (i < width/2 - max || i > width/2 + max || j < width/2 - max || j > width/2 + max)) {

                  Matrix[i][j] = 0.0;
              }
          }
      }
    return;

    }
char * concat(const char * s1,const char * s2) {
  char * result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
  // in real code you would check for errors in malloc here
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}
void recalagecyclique(float** Matrix, int length, int width) {
    int x,y;
    for(x=0; x<length; x++)
        for(y=0; y<width; y++)
                Matrix[x][y] = Matrix[x][y]*pow(-1,(x+y)) ;
}
int main(int argc,char **argv)
 {
  int i,j,k, min, max;
  int length,width;
  float** MatriceImgR;
  float** MatriceImgI;
  float** LastMatriceImgR;
  float** LastMatriceImgI;
  char* name_img_out;

  int interg[] = {0, 2, 6, 16};
  int interd[] = {1, 5, 15, 63};


  LastMatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
  LastMatriceImgI=fmatrix_allocate_2d(length,width);
  for(i=0;i<length;i++)
          for(j=0;j<width;j++) {
            LastMatriceImgR[i][j]=0.0;
            LastMatriceImgI[i][j]=0.0;
          }
  
  for(k=0; k<4; k++) {
      /*Allocation memoire pour la matrice image*/
      MatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
      MatriceImgI=fmatrix_allocate_2d(length,width);
      
      
      /*Initialisation a zero de toutes les matrices */
      for(i=0;i<length;i++)
          for(j=0;j<width;j++) {
              MatriceImgI[i][j]=0.0;
          }
      
      recalagecyclique(MatriceImgR, length, width);
      
      
      /*FFT*/
      FFTDD(MatriceImgR,MatriceImgI,length,width);
      
      min = interg[k];
      max = interd[k];
      annulerzone(MatriceImgR,min,max,length,width);
      annulerzone(MatriceImgI,min,max,length,width);

      for (int i = 0; i < length; i++)
      {
          for (int j = 0; j < width; j++)
          {
              MatriceImgR[i][j]+=LastMatriceImgR[i][j];
              MatriceImgI[i][j]+=LastMatriceImgI[i][j];
          }
      }
      for (int i = 0; i < length; i++)
      {
          for (int j = 0; j < width; j++)
          {
      LastMatriceImgR[i][j]=MatriceImgR[i][j];
      LastMatriceImgI[i][j]=LastMatriceImgI[i][j];
        }
      }

      IFFTDD(MatriceImgR, MatriceImgI, length, width);

      
      recalagecyclique(MatriceImgR, length, width);

      Recal(MatriceImgR, length, width);
      
      char c[32];
      sprintf(c, "%d", min);
      char c1[32];
      sprintf(c1, "%d", max);
      name_img_out=concat("image-TpIFT6150-1-Bb_",c);
      name_img_out=concat(name_img_out,"_");
      name_img_out=concat(name_img_out,c1);

     
      /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
      SaveImagePgm(name_img_out,MatriceImgR,length,width);
       
      /*Liberation memoire pour les matrices*/
      free_fmatrix_2d(MatriceImgR);
      free_fmatrix_2d(MatriceImgI);
  }
      free_fmatrix_2d(LastMatriceImgR);
      free_fmatrix_2d(LastMatriceImgI);
  return 0;
}
