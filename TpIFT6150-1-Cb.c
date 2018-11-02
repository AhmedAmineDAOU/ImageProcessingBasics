/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Aa.c                           */
/* Auteurs : Nicolas Hurtubise, Mercedes Gauthier       */
/* Date    : 18/10/2016                                 */
/* version : 1                                          */
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
#define NAME_IMG_IN  "D1r"


/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/


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
void convolution(float** reel,float** imaginaire,float** reel1,float** imaginaire1,int length,int width){
    for (int i=0 ; i<length ; i++)

      for (int j=0 ; j<width ; j++){

        reel[i][j]  =   reel[i][j] * reel1[i][j] - imaginaire[i][j] * imaginaire1[i][j];
        imaginaire[i][j]  = reel[i][j] * imaginaire1[i][j] +  imaginaire[i][j] * reel1[i][j];
      }
}

float** genererCarre(int n){
    float** matrix;
    int bloc=0 , i=0 , j=0 ;
    int length=128, width=128;
    matrix=fmatrix_allocate_2d(128,128);
  
    for (i=0 ; i < length ; i++)
      for(j=0 ; j < width ; j++)
          matrix[i][j]=0.0;
    /*creer un carré blanc au milieu*/
    for (i= 0 ; i < n / 2 ; i++)
      for(j= 0 ; j < n / 2 ; j++)
          matrix[i][j]=255.0;

    for (i= (length - n/2 ) ; i < length ; i++)
      for(j= 0 ; j < n / 2 ; j++)
          matrix[i][j]=255.0;

    for (i= 0 ; i < n / 2  ; i++)
      for(j= width - n / 2 ; j <  width; j++)
          matrix[i][j]=255.0;

      for (i= (length - n/2 ) ; i < length ; i++)
        for(j= width - n / 2 ; j <  width; j++)
          matrix[i][j]=255.0;

    return matrix;
 }


int main(int argc,char **argv)
 {
  int i,j,k, min, max;
  int length=128,width=128;
  float** MatriceImgRmasque;
  float** MatriceImgImasque;
  float** MatriceImgR;
  float** MatriceImgI;
  float** matRout;
  float** matIout;
  char* name_img_out;

  MatriceImgRmasque=fmatrix_allocate_2d(length,width);
  MatriceImgImasque=fmatrix_allocate_2d(length,width);
  MatriceImgR=fmatrix_allocate_2d(length,width);
  MatriceImgI=fmatrix_allocate_2d(length,width);
  matRout=fmatrix_allocate_2d(length,width);
  matIout=fmatrix_allocate_2d(length,width);



  MatriceImgR = LoadImagePgm("D1r", &length, &width);


  for(int i=0 ; i < length ; i++)
    for(int j=0 ; j < length ; j++){
      MatriceImgImasque[i][j] = 0.0;
      MatriceImgI[i][j] = 0.0;
      matRout[i][j] = 0.0;
      matIout[i][j] = 0.0;
    }




  MatriceImgRmasque=genererCarre(3);


  FFTDD(MatriceImgR,MatriceImgI,length,width);
  FFTDD(matRout,matIout,length,width);


  FFTDD(MatriceImgRmasque,MatriceImgImasque,length,width);


  MultMatrix( matRout, matIout, MatriceImgRmasque, MatriceImgImasque, MatriceImgR,MatriceImgI,length,width);

  
  
  IFFTDD(matRout, matIout, length, width);


  Recal(matRout, length, width);
  
  SaveImagePgm("image-TpIFT6150-1-Cb_filtered3", matRout, length, width);

  
  free_fmatrix_2d(MatriceImgR);
  free_fmatrix_2d(MatriceImgI);
  free_fmatrix_2d(MatriceImgRmasque);
  free_fmatrix_2d(MatriceImgImasque);
  free_fmatrix_2d(matRout);
  free_fmatrix_2d(matIout);

 return 0;
}
