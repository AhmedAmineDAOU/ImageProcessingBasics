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

#define NAME_IMG_OUT "image-TpIFT6150-1-Ba"

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



float** genererCarre(){
float** matrix;
int bloc=0 , i=0 , j=0 , n=0;
int length=128, width=128;
matrix=fmatrix_allocate_2d(128,128);

printf("%s\n","Veuillez inserer la taille du carre blanc a generer dans la photo");

  while(bloc==0){
    printf("%s\n","la taille doit etre paire (pour que le carre soit au centre et < 128");
    scanf("%d", &n);
    if ( n%2==0 && n<128 )
      bloc=1;

  }
  
    for (i=0 ; i < length ; i++)
      for(j=0 ; j < width ; j++)
          matrix[i][j]=0.0;
    /*creer un carré blanc au milieu*/
    for (i= 0 ; i < n ; i++)
      for(j= 0 ; j < n ; j++)
          matrix[i][j]=255.0;
    return matrix;
 }


int main(int argc,char **argv)
 {
  int i,j,k, min, max;
  int length=128,width=128;
  float** MatriceImgR;
  float** MatriceImgI;
  float** MatriceOutR;
  float** MatriceOutI;
  char* name_img_out;
 
  MatriceImgR=fmatrix_allocate_2d(length,width);
  MatriceImgI=fmatrix_allocate_2d(length,width);
  MatriceOutR=fmatrix_allocate_2d(length,width);
  MatriceOutI=fmatrix_allocate_2d(length,width);


  for(int i=0 ; i < length ; i++)
    for(int j=0 ; j < length ; j++)
     { 
      MatriceImgI[i][j]=0.0;
      MatriceOutR[i][j]=0.0;
      MatriceOutI[i][j]=0.0;
     }

  MatriceImgR=genererCarre();

  SaveImagePgm("image-TpIFT6150-1-Ca_square", MatriceImgR,length,width);

  //remake centre 
  FFTDD(MatriceImgR,MatriceImgI,length,width);

 // autoconvolution(MatriceImgR, MatriceImgI, length, width); 

  SquareMatrix(MatriceOutR,MatriceOutI,MatriceImgR, MatriceImgI, length, width);
  
  
  IFFTDD(MatriceOutR, MatriceOutI, length, width);


  
  Recal(MatriceOutR, length, width);
  
  SaveImagePgm("image-TpIFT6150-1-Ca_convolution", MatriceOutR, length, width);

  
  free_fmatrix_2d(MatriceImgR);
  free_fmatrix_2d(MatriceImgI);
  free_fmatrix_2d(MatriceOutR);
  free_fmatrix_2d(MatriceOutI);

 return 0;
}
