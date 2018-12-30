#include "grayscale.c"
#include <math.h>
#include <string.h>

typedef struct{
    int x,y;
    float corr;
    unsigned char marcat;
    int index_culoare;
}detectii;

typedef struct
{
    int x,y;
}coord;

typedef struct
{
    unsigned char R,G,B;
}culoare;

/*void extrag_dimensiuni(char *nume_poza,unsigned int *W,unsigned int *H)
{
    FILE * f = fopen(nume_poza,"rb");
    if( f == NULL)
    {
        printf("Eroare la deschiderea fisierului");
        return ;
    }
    else
    {

        fseek(f,18,SEEK_SET);
        fread(W,sizeof(int),1,f);
        fseek(f,22,SEEK_SET);
        fread(H,sizeof(int),1,f);
    }
    fclose(f);


} */


//save an imagine considering the fact that it is a grayscale one(R=G=B)

void salvez_pixeli_imagine(unsigned int ***a,char *nume_poza)
{
    unsigned int W,H;
    unsigned char x;
    int i,j;
    FILE *f =fopen(nume_poza,"rb");
    extrag_dimensiuni(nume_poza,&W,&H);
    int padding;
    if(W % 4 != 0)
        padding = 4 - (3 * W) % 4;
    else
        padding = 0;
    (*a)=(unsigned int **)malloc(H*sizeof(unsigned int*));
    for(i=0;i<H;i++)
        (*a)[i]=(unsigned int *)malloc(W*sizeof(unsigned int));
    fseek(f,54,SEEK_SET);
    for(i=0;i<H;i++)
    {

        for(j=0;j<W;j++)
    {
        fread(&x,1,1,f);
        fread(&x,1,1,f);
        fread(&x,1,1,f);
        (*a)[i][j]=x;


    }
    fseek(f,padding,SEEK_CUR);
    }

    fclose(f);



}

//compute the average value of a given matrix

float medie_pixel_tablou(unsigned int  **a, unsigned int W, unsigned int H)
{
    float S=0;
    int i,j;
    for(i=0;i<H;i++)
        for(j=0;j<W;j++)
            S+=a[i][j];
    return (S/(float)(W*H));
}
//calculate the standard deviation of a given matrix

double deviatie_standard(unsigned int **a,unsigned int W, unsigned int H)
{
    double deviatie=0;
    double sigma=0;
    double nr_pixel_tablou = W*H;
    double medie_tablou = medie_pixel_tablou(a,W,H);
    int i,j;
    for(i=0;i<H;i++)
        for(j=0;j<W;j++)
             sigma+=((a[i][j]-medie_tablou)*(a[i][j]-medie_tablou));
    sigma = sigma *((1.0)/(nr_pixel_tablou-1));
    deviatie = sqrt(sigma);
    return deviatie;


}
//compute the correlation between a template S and a window f

double corelatie(unsigned int **f, unsigned int **S,unsigned int W, unsigned int H,double medie_ferestra,double medie_sablon) //calculeaza corelatia dintre un sablon S si fereastra f
{
    double corr=0,Sigma=0;
    int i,j;
    double nr_pixeli=W*H;
    double sigmaS=deviatie_standard(S,W,H);
    double sigmaF=deviatie_standard(f,W,H);
    for(i=0;i<H;i++)
        for(j=0;j<W;j++)
        Sigma+=((f[i][j]-medie_ferestra)*(S[i][j]-medie_sablon));
    corr=(1/(double)(nr_pixeli))*1/(sigmaS*sigmaF)*Sigma;
    return corr;

}
// save the names of the templates in an array

void extrag_nume_sabloane(char ***A,char *nume_fisier)
{
    FILE *f=fopen(nume_fisier,"r");
    (*A)=(char **)malloc(10*sizeof(char *));
    char *s=(char *)malloc(15*sizeof(char));
    int i;
    for(i=0;i<10;i++)
    {
        (*A)[i]=(char *)malloc(15*sizeof(char));
        fgets(s,15,f);
        s[strlen(s)-1]='\0';
        strcpy((*A)[i],s);
    }
    fclose(f);


}

//save the given colours in an array

void extrag_culori(culoare **C, char *nume_fisier)
{
    FILE *f=fopen(nume_fisier,"r");
    (*C)=(culoare *)malloc(10*sizeof(culoare));
    int i;
    int x;
    for( i=0;i<10;i++)
    {
        fscanf(f,"%u",&x);
        (*C)[i].R=x;
        fscanf(f,"%u",&x);
        (*C)[i].G=x;
        fscanf(f,"%u",&x);
        (*C)[i].B=x;
    }
    fclose(f);

}
//extract a window centered in the point of coordinates (i,j) from the whole imagine
void get_fereastra_get_medie_feresatra(int **f,int **I, unsigned int WF,unsigned int HF,int i, int j,double *medie)  //creez fereastra centrata (i,j) din Imaginea mare
{
    int p,q;
    double S=0;
    int i_stanga_sus=i-HF/2;
    int j_stanga_sus=j-WF/2;
    for(p=i_stanga_sus;p<=(HF+i_stanga_sus-1);p++)
    {


        for(q=j_stanga_sus;q<=(WF+j_stanga_sus-1);q++)
        {
            S+=I[p][q];

               f[p-i_stanga_sus][q-j_stanga_sus]=I[p][q];

        }
    }
    *medie=S/(double)(WF*HF);



}
//draw the border of the window with a specific colour C

void desenez_contur_fereastra(culoare **I,unsigned int WF, unsigned int HF,detectii f,culoare C)
{

    int i_colt_sus=f.x-HF/2;
    int j_colt_sus=f.y-WF/2;
    int i,j;
    for(i=i_colt_sus;i<HF+i_colt_sus;i++)
    {
        I[i][j_colt_sus].R=C.R;
        I[i][j_colt_sus].B=C.B;
        I[i][j_colt_sus].G=C.G;
    }
    for(i=j_colt_sus;i<WF+j_colt_sus;i++)
    {
        I[i_colt_sus][i].R=C.R;
        I[i_colt_sus][i].B=C.B;
        I[i_colt_sus][i].G=C.G;
    }
    for(i=i_colt_sus;i<HF+i_colt_sus;i++)
    {
        I[i][j_colt_sus+WF-1].R=C.R;
        I[i][j_colt_sus+WF-1].B=C.B;
        I[i][j_colt_sus+WF-1].G=C.G;
    }
    for(i=j_colt_sus;i<WF+j_colt_sus;i++)
    {
        I[i_colt_sus+HF-1][i].R=C.R;
        I[i_colt_sus+HF-1][i].B=C.B;
        I[i_colt_sus+HF-1][i].G=C.G;
    }



}
//save the coloured image in a matrix

void incarc_imagine_color(char *nume_poza,culoare ***P)
{
    unsigned int W,H;
    W=H=0;
    extrag_dimensiuni(nume_poza,&W,&H);
    FILE * f = fopen(nume_poza,"rb");

    if( f == NULL)
    {
        printf("Eroare la deschiderea fisierului %s",nume_poza);
        return ;

    }
    else
    {


        int padding;
       if(W % 4 != 0)
        padding = 4 - (3 * W) % 4;
        else
        padding = 0;
        (*P)=(culoare **)malloc(H*sizeof(culoare*));
        for(int i=0;i<H;i++)
            (*P)[i]=(culoare *)malloc(W*sizeof(culoare));
        unsigned char x;
        fseek(f,54,SEEK_SET);   //trec de header
        unsigned int i,j;
        for(i=0;i<H;i++)
        {

            for(j=0;j<W;j++)

      {

            fread(&x,1,1,f);
           (*P)[i][j].B=x;          //salvez ordine inversa B G R
            fread(&x,1,1,f);
            (*P)[i][j].G=x;
            fread(&x,1,1,f);
            (*P)[i][j].R=x;
        }
        fseek(f,padding,SEEK_CUR);
        }


    }
    fclose(f);
}

//save an image externally
void salvez_extern_poza(char *nume_poza,char *poza_originala,culoare **P)
{
    unsigned int W,H;
    W=H=0;
    extrag_dimensiuni(poza_originala,&W,&H);
    FILE * f= fopen(nume_poza,"wb");
    FILE *pep=fopen(poza_originala,"rb");
    unsigned char x;
    int i,j;

    for(i=0;i<54;i++) //copy the header
    {
        fread(&x,1,1,pep);
        fwrite(&x,1,1,f);
    }

    for(i=0;i<H;i++)
        for(j=0;j<W;j++)
   // B G R
    {

        fwrite(&P[i][j].B,1,1,f);
        fwrite(&P[i][j].G,1,1,f);
        fwrite(&P[i][j].R,1,1,f);

    }
    fclose(f);
    fclose(pep);


}
//find the detections and save them in an array
void gasesc_detectii_sablon_x(int **I,culoare **I_color,char *nume_sablon,unsigned int WI ,unsigned int HI, unsigned int WS, unsigned int HS,float prag,culoare C, detectii *D,int *k,int index_culoare)
{
    int **S,**f;
    unsigned int HF,WF;
    int i,j,p,q;
    f=(int **)malloc(HS*sizeof(int *));
    for(p=0;p<HS;p++)
        f[p]=(int *)malloc(WS*sizeof(int));
    salvez_pixeli_imagine(&S,nume_sablon);
    double medie_sablon=medie_pixel_tablou(S,WS,HS);

     for(i=HS/2;i<HI-HS/2;i++)
        for(j=WS/2;j<WI-WS/2;j++)
    {
        {
            double medie_ferestra = 0;


            get_fereastra_get_medie_feresatra(f,I,WS,HS,i,j,&medie_ferestra);
            float rez_corelatie=corelatie(f,S,WS,HS,medie_ferestra,medie_sablon);

          if(rez_corelatie>=prag)
            {
                D[(*k)].x=i;
                D[(*k)].y=j;
                D[(*k)].corr=rez_corelatie;
                D[(*k)].marcat=0;
                D[(*k)].index_culoare=index_culoare;
                desenez_contur_fereastra(I_color,WS,HS,D[(*k)],C);
                (*k)++;

            }

        }




    }


}

//combine the results of the template matching algorithm in an array and save the result externally
void template_matching(char *nume_poza,char *nume_poza_gray,char **lista_sabloane_gray,char *imagine_detectii,float prag,culoare *C,detectii **D,int *k)
{
    *k=0;
    *D=(detectii*)malloc(100000*sizeof(detectii));
    unsigned int WS,HS,WI,HI;
    int i;
    int **I;
    culoare **I_color;
    extrag_dimensiuni(nume_poza_gray,&WI,&HI);
    extrag_dimensiuni(lista_sabloane_gray[0],&WS,&HS);
    salvez_pixeli_imagine(&I,nume_poza_gray);
    incarc_imagine_color(nume_poza,&I_color);
    for(i=0;i<10;i++)
    {
    gasesc_detectii_sablon_x(I,I_color,lista_sabloane_gray[i],WI,HI,WS,HS,prag,C[i],*D,k,i);
    }
    salvez_extern_poza(imagine_detectii,nume_poza,I_color);
}
//used for the qsort function
int cmp(const void *a , const void *b)
{
    detectii va,vb;
    va=*(detectii *)a;
    vb=*(detectii *)b;
    if(va.corr>vb.corr) return -1;
    if(va.corr<vb.corr) return 1;
    return 0;

}
//find the right_top and left_bottom coordinates of a windows centered in (x,y)

void get_colturi_fereastra(coord *right_top,coord *left_bottom,detectii f, unsigned int W, unsigned int H)
{
    (*right_top).x=f.x-H/2-1;
    (*right_top).y=f.y+W/2+1;
    (*left_bottom).x=f.x+H/2;
    (*left_bottom).y=f.y-W/2;
}
int maxi(int a, int b)
{
    return a>b?a:b;
}
int mini(int a, int b)
{
    return a<b?a:b;
}
//calculate the overlapping area of two given windows
double arie_suprapunere_detectii(coord right_topA,coord left_bottomA,coord right_topB,coord left_bottomB)
{
    int x=maxi(0,(mini(left_bottomA.x,left_bottomB.x)-maxi(right_topA.x,right_topB.x)));
    int y=maxi(0,(mini(right_topA.y,right_topB.y)-maxi(left_bottomA.y,left_bottomB.y)));
    if(x*y==0) return -1;
    return x*y;

}
//computer the spatial overlap between two given detections
double suprapunere_spatiala(detectii A, detectii B)
{
    coord right_topA,left_buttomA,right_topB,left_buttomB;
    get_colturi_fereastra(&right_topA,&left_buttomA,A,11,15);
    get_colturi_fereastra(&right_topB,&left_buttomB,B,11,15);
    double arie_intersect=arie_suprapunere_detectii(right_topA,left_buttomA,right_topB,left_buttomB);
    if(arie_intersect == -1) return -1;
    else
    {
    double arie_detectie_A=11*15;
    double arie_detectie_B=11*15;
    double suprapunere = arie_intersect/(double)(arie_detectie_A+arie_detectie_B-arie_intersect);
    return suprapunere;
    }


}

//eliminate those detections which have the spatial overlap over 0.2 and save the result externally
void eliminare_non_maxime(detectii *D, int n,char *nume_poza_originala,char *poza_nonmax,culoare *C)
{
    qsort(D,n,sizeof(detectii),cmp);
    culoare *I_color;
    incarc_imagine_color(nume_poza_originala,&I_color);
    int i,j;
    for(i=0;i<n-1;i++)

        for(j=i+1;j<n;j++)
        {


              if(suprapunere_spatiala(D[i],D[j])>0.2)
        {

         D[j].marcat=1;
        }
      }
    for(i=0;i<n;i++)
        if(D[i].marcat == 0 ) desenez_contur_fereastra(I_color,11,15,D[i],C[D[i].index_culoare]);
    salvez_extern_poza(poza_nonmax,nume_poza_originala,I_color);

}

