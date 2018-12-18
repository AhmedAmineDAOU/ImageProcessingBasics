/*------------------------------------------------------*/
/* Prog    : TpIFT6150-1-Aa.c                           */
/* Auteur  : Ahmed Amine DAOU                           */
/* Date    : 18/12/2018                                 */
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
int my_global=0;
/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_IMG_IN  "image-TpIFT6150-1-Af-img"

#define NAME_IMG_OUT "image-TpIFT6150-1-Af-spc"

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
void permute(float** mat,int i,int j,int k,int l){

    float tmp=  mat[k][l];
    mat[k][l]=mat[i][j];
    mat[i][j]=tmp;

}
/* modifie la matrice du spectre pour ramener le centre au point (0,0) et
 * non au zero informatique
 * prend en parametre le nom de la matrice, sa longueur et sa largeur
*/
 float** harmoniser(char* name,int * length,int* width){

  int i,j;
  unsigned char var;
  char buff[NBCHAR];
  float** mat2;
  float** mat;
  char stringTmp1[NBCHAR],stringTmp2[NBCHAR];
  int ta1,ta2,ta3;
  FILE *fic;

  /*-----nom du fichier pgm (du spectre d'amplitude non modifié)-----*/
  strcpy(buff,name);
  strcat(buff,".pgm");
  printf("---> Ouverture de %s",buff);

  /*----ouverture du fichier (du spectre d'amplitude non modifié)----*/
  fic=fopen(buff,"r");
  if (fic==NULL)
    { printf("\n- Grave erreur a l'ouverture de %s  -\n",buff);
      exit(-1); }

  /*--recuperation de l'entete--*/
  fgets(stringTmp1,100,fic);
  fgets(stringTmp2,100,fic);
  fscanf(fic,"%d %d",&ta1,&ta2);
  fscanf(fic,"%d\n",&ta3);

  /*--affichage de l'entete--*/
  printf("\n\n--Entete--");
  printf("\n----------");
  printf("\n%s%s%d %d \n%d\n",stringTmp1,stringTmp2,ta1,ta2,ta3);

  *length=ta1;
  *width=ta2;

  /*allocation de memoire pour la matrice du spectre d'amplitude a modifier*/
  mat=fmatrix_allocate_2d(*length,*width);
   
  /*--chargement dans la matrice source (spectre non harmonisé)--*/
     for( i=0 ; i < *length ; i++ )
      for(j=0 ; j < *width ; j++)  
        { 
          fread( &var,1,1,fic );
          mat[i][j]=var; 
        }
   /*---fermeture du fichier---*/
  fclose(fic);
  
  /*debut modification*/
  /*permuter les points du carré 1 avec celles de 4*/
  for(i=0 ; i < *length/2 ; i++ )
    for(j=0 ; j < *width/2 ; j++ ){
        permute(mat,i,j,i+*length/2,j+*length/2); 
        }


  /*permuter les points du carré 2 avec celles du 3*/
      for(i=*length/2;i<*length;i++)
      for(j=0;j<*width/2;j++)  
        { 
        permute( mat,i,j,i-*length/2,j+*length/2 ); 
        }

  /* fin modification*/
  
  return(mat);
 }

float** genererCaree()
 {
  int i=0,j=0;
  float** matrix;
  int *imptr;
  int vsize=128;
  int hsize=128;
  int bloc=0;
  int n;
  matrix=fmatrix_allocate_2d(128,128);

  /*commencer par demander la taille du carré blanc en respectant certaines 
   criteres*/
  printf("%s\n","Veuillez inserer la taille du carre blanc a generer dans la photo");

  while(bloc==0){
    printf("%s\n","la taille doit etre paire (pour que le carre soit au centre et < 128");
    scanf("%d", &n);
    my_global=n;
    if ( n % 2 == 0 && n < 128 )
      bloc=1;

  }
  
    for ( i=0 ; i < vsize ; i++)
      for(j=0;j<hsize;j++)
          matrix[i][j]=0.0;
    /*creer un carré blanc au milieu*/
    for (i=(vsize-n)/2;i<(vsize+n)/2;i++)
      for(j=(hsize-n)/2;j<(hsize+n)/2;j++)
          matrix[i][j]=255.0;
    return matrix;
 }
 /*fonction de translation, prend en parametre la matrice du carré,
 et indice de translation toutes les translation se font sur x*/

 void translateCarre(float** matrice){
  int indice;
  int sens;
        
  printf("%s\n","dans quelle direction voulez-vous translater le carré? (1=x/2=y)" );
  scanf("%d",&sens);
    
     if(sens == 2){
        printf("%s\n","de combien voulez-vous translater" );
        scanf("%d", &indice);
        indice=indice;
      
      for (int i = 0; i < 128-indice; i++)
       for (int j = 0; j < 128; j++)
       { 
        permute(matrice,i,j,i+indice,j);
         
        }
        
       }
      else if(sens== 1){
        printf("%s\n","de combien voulez-vous translater" );
        scanf("%d", &indice);
        indice=indice%128;
      
      for (int i = 0; i < 128; i++)
       for (int j = 0; j < 128-indice; j++)
       { 
        permute(matrice,i,j,i,j+indice);
         
        }
       }
     }


      
    
 
 float** phase(float** ImgR,float** ImgI){
    float** phaseM;
     phaseM=fmatrix_allocate_2d(128,128);
     for (int i = 0; i < 128; i++)
      for (int j = 0; j < 128; j++)
     {
       phaseM[i][j]=atan2f(ImgI[i][j],ImgR[i][j]);
     }
    return phaseM;
 }

void main() 
{   int length,width;
    length=width=128;
    float** MatriceImgI;
    float** MatriceImgM;
    float** MatriceImgR;
    float** phaseM;

    MatriceImgR=fmatrix_allocate_2d(length,width);
    MatriceImgI=fmatrix_allocate_2d(length,width);
    MatriceImgM=fmatrix_allocate_2d(length,width);
    float **MatriceHarmonique=fmatrix_allocate_2d(length,width);
    phaseM=fmatrix_allocate_2d(length,width);
    //generer une matrice du carré blanc sur noir
    MatriceImgR=genererCaree();
    //translateCarre(MatriceImgR);
    //enregistrement de la photo dans le repertoire courant sous le nom "image-TpIFT6150-1-Af1-img"
    SaveImagePgm("image-TpIFT6150-1-Af1-img",MatriceImgR,length,width);
    /*Initialisation a zero de toutes les matrices */
    for(int i=0;i<length;i++) 
    for(int j=0;j<width;j++) 
      {
        MatriceImgI[i][j]=0.0;
        MatriceImgM[i][j]=0.0;
      }
  
  /*FFT*/
  FFTDD(MatriceImgR,MatriceImgI,length,width);

  /*Module*/
  Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);
  /*application du log(1+MatImgM[][] sur la matrice du module)*/
  for(int i=0;i<length;i++) 
    for(int j=0;j<width;j++){ 
      MatriceImgM[i][j]=log(1+MatriceImgM[i][j]);
    }

  /*Pour la visualisation visu*/ 
  Mult(MatriceImgM,10.0,length,width);  
  Recal(MatriceImgM,length,width);


  /*enregistrer temporairement) la photo du spectre non centré */
  SaveImagePgm("image-TpIFT6150-1-Af1-spc",MatriceImgM,length,width);

  /*centrer le spectre (non centré)*/
   MatriceHarmonique=harmoniser("image-TpIFT6150-1-Af1-spc",&length,&width);       

  /*ecraser l'ancien spectre non centré par le bon*/
  SaveImagePgm("image-TpIFT6150-1-Af1-spc",MatriceHarmonique,length,width);

    printf("%s\n","avant la translation");
    printf("%s\n","====================");
    printf("%s\n","valeur du modules au points [5][5]" );
    printf("%f\n",MatriceImgM[5][5] );
    printf("%s\n","valeur de la phase au points [5][5]" );
    phaseM=phase(MatriceImgR,MatriceImgI);
    printf("%f\n",phaseM[5][5]);
    /*enregistrer temporairement) la photo du spectre non centré */
    //SaveImagePgm("image-TpIFT6150-1-Af-spc",MatriceImgM,length,width);
    /*centrer le spectre (non centré)*/
    //float ** MatriceHarmonique=harmoniser("image-TpIFT6150-1-Af-spc",&length,&width);       
    /*ecraser l'ancien spectre non centré par le bon*/
    //SaveImagePgm("image-TpIFT6150-1-Ae-spc",MatriceHarmonique,length,width);

    /*Liberation memoire pour les matrices*/
    free_fmatrix_2d(MatriceImgR);
    free_fmatrix_2d(MatriceImgI); 
    free_fmatrix_2d(MatriceImgM);    
    free_fmatrix_2d(phaseM); 















  
   
    MatriceImgR=fmatrix_allocate_2d(length,width);
    MatriceImgI=fmatrix_allocate_2d(length,width);
    MatriceImgM=fmatrix_allocate_2d(length,width);
    phaseM=fmatrix_allocate_2d(length,width);
    MatriceImgR=LoadImagePgm("image-TpIFT6150-1-Af1-img",&length,&width);
    
    translateCarre(MatriceImgR);
    //enregistrement de la photo dans le repertoire courant sous le nom "image-TpIFT6150-1-Ae-img"
    SaveImagePgm("image-TpIFT6150-1-Af2-img",MatriceImgR,length,width);
    for(int i=0;i<length;i++) 
    for(int j=0;j<width;j++) 
      {
        MatriceImgI[i][j]=0.0;
        MatriceImgM[i][j]=0.0;
      }
      /*FFT*/
    FFTDD(MatriceImgR,MatriceImgI,length,width);

    /*Module*/
    Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);
    /*application du log(1+MatImgM[][] sur la matrice du module)*/
    for(int i=0;i<length;i++) 
      for(int j=0;j<width;j++){ 
        MatriceImgM[i][j]=log(1+MatriceImgM[i][j]);
      }

    /*Pour la visualisation visu*/ 
    Mult(MatriceImgM,10.0,length,width);  
    Recal(MatriceImgM,length,width);


    /*enregistrer temporairement) la photo du spectre non centré */
    SaveImagePgm("image-TpIFT6150-1-Af2-spc",MatriceImgM,length,width);

    /*centrer le spectre (non centré)*/
    MatriceHarmonique=harmoniser("image-TpIFT6150-1-Af2-spc",&length,&width);       

   /*ecraser l'ancien spectre non centré par le bon*/
    SaveImagePgm("image-TpIFT6150-1-Af2-spc",MatriceHarmonique,length,width);

    printf("%s\n","apres la translation");
    printf("%s\n","====================");
    printf("%s\n","valeur du modules au points [5][5]" );
    printf("%f\n",MatriceImgM[5][5] );
    printf("%s\n","valeur de la phase au points [5][5]" );
    phaseM=phase(MatriceImgR,MatriceImgI);
    printf("%f\n",phaseM[5][5]);

    free_fmatrix_2d(MatriceImgR);
    free_fmatrix_2d(MatriceImgI); 
    free_fmatrix_2d(MatriceImgM);    
    free_fmatrix_2d(phaseM); 


    return ;    
} 
