//
//  main.c
//  Otsu
//
//  Created by Sankarsan Seal on 04/03/17.
//  Copyright © 2017 Sankarsan Seal. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <math.h>

int main(int argc, const char * argv[]) {
    // insert code here...
    FILE * image=NULL;
    FILE * output=NULL;
    char imagetype[3];
    char imagewidth[5];
    char imageheight[5];
    char comment[80];
    char maxgray[4];
    unsigned char temp;
    int width=0;
    int height=0;
    int ** imagemat=NULL;
    int * histo=NULL;
    int i,j,k;
    int maxgrayval=0;
    int total=0;
    int maxk=0;
    
    double omega0=0,omega1=0;
    double mu0=0, mu1=0, mutotal=0;;
    double *normhisto=NULL;
    double sigmabetween=0;
    double sigmatotal=0;
    double eta=0;
    double maxeta=DBL_MIN;
    
    

    imagetype[2]='\0';
    
    image=fopen("swan.pgm","rb");
    
    if (image ==NULL)
        fprintf(stdout,"Problem with image file\n");
    else
    {
        fscanf(image,"%c%c%*c",&imagetype[0],&imagetype[1]);
        fprintf(stdout,"%s\n",imagetype);
        if (imagetype[0]=='P' && imagetype[1]=='5')
        {
            fscanf(image,"%c",&temp);
            if( temp=='#')
            {
                fscanf(image,"%[^\n]",comment);
                fprintf(stdout,"%s\n",comment);
            }
            else
            {
                ungetc(temp, image);
            }
            fscanf(image,"%s",imagewidth);
            fscanf(image,"%s",imageheight);
            
            width=atoi(imagewidth);
            height=atoi(imageheight);
            total=width*height;
            
            fprintf(stdout,"Dimensions of Image: %s %s\n",imagewidth, imageheight);
            fscanf(image,"%s",maxgray);
            fprintf(stdout,"Maximum gray value: %s\n",maxgray);
            maxgrayval=atoi(maxgray);
            
            imagemat=(int **)malloc(height*sizeof(int *));
            histo=(int *)malloc((maxgrayval+1)*sizeof(int));
            normhisto=(double *)malloc((maxgrayval+1)*sizeof(double));
            
            for(i=0;i<=maxgrayval;i++)
            {
                histo[i]=0;
                normhisto[i]=0;
            }
            
            if (imagemat!=NULL && histo !=NULL)
            {
                for(i=0;i<height;i++)
                {
                    *(imagemat + i)=(int *)malloc(width*sizeof(int));
                    for(j=0;j<width;j++)
                    {
                        fscanf(image,"%c",&temp);
                        imagemat[i][j]=(int)temp;
                        //fprintf(stdout,"%d ",imagemat[i][j]);
                        histo[imagemat[i][j]]++;
                    }
                    //%fprintf(stdout,"\n");
                }
                
//                for (i=0;i<=maxgrayval;i++)
//                {
//                    //fprintf(stdout,"%d ",histo[i]);
//                    sum+=histo[i];
//                }
//                
//                    fprintf(stdout,"Total Pixel:%d, Total Grayvalues:%d\n",width*height,sum);
//                
                for(i=0;i<=maxgrayval;i++)
                {
                    normhisto[i]=(double)histo[i]/(double)total;
                }
                
                for(k=0;k<=maxgrayval;k++)
                {
                    
                    for(i=0;i<=k;i++)
                    {
                        omega0+=normhisto[i];
                        mu0+=(double)i*normhisto[i];
                    }
                    for(i=k+1;i<=maxgrayval;i++)
                    {
                        omega1+=normhisto[i];
                        mu1+=(double)i*normhisto[i];
                    }
                    for(i=0;i<=maxgrayval;i++)
                    {
                        mutotal+=(double)i*normhisto[i];
                    }
                    
                    sigmabetween=omega0*omega1*pow((mu0-mu1),2);
                    
                    
                    for(i=0;i<=maxgrayval;i++)
                    {
                        sigmatotal+=pow(((double)i-mutotal),2)*normhisto[i];
                    }
                    
                    eta=sigmabetween/sigmatotal;
                    
                    if(eta > maxeta)
                    {
                        maxk=k;
                        maxeta=eta;
                    }
                    
                    
                    
                    
                }
                fprintf(stdout,"max K: %d\n",maxk);
                
                for(i=0;i<height;i++)
                {
                    for(j=0;j<width;j++)
                    {
                      if (imagemat[i][j] <=maxk)
                          imagemat[i][j]=0;
                      else
                          imagemat[i][j]=255;
                        
                    }
                    
                }
                
                output=fopen("Otsuthres.pgm","wb");
                
                if(output!=NULL)
                {
                    fprintf(output,"%s\n",imagetype);
                    fprintf(output,"%s ",imagewidth);
                    fprintf(output,"%s\n",imageheight);
                    fprintf(output, "%s\n",maxgray);
                    
                    for(i=0;i<height;i++)
                    {
                        for(j=0;j<width;j++)
                        {
                            fprintf(output,"%c",(char)imagemat[i][j]);
                        }
                    }
                    
                    
                    
                    fclose(output);
                }
                
                for(i=0;i<height;i++)
                {
                    free(imagemat[i]);
                }
                free(imagemat);
            }
            else
            {
                fprintf(stdout,"Problem with memory allocation.\n");
            }
            
        
        }
        
        fclose(image);
    }
    
    return 0;
}
