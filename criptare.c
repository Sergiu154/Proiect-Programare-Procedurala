
typedef struct
{
    unsigned char R,G,B;
}pixel;

//get the dimensions of the picture

void extrag_dimensiuni(char *nume_poza,unsigned int *W,unsigned int *H)
{
    FILE * f = fopen(nume_poza,"rb");
    if( f == NULL)
    {
        printf("Eroare la deschiderea fisierului %s",nume_poza);
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


}

//save the picture in an array

void incarc_poza(char *nume_poza,pixel **P,unsigned int W, unsigned int H)
{
    FILE * f = fopen(nume_poza,"rb");
    if( f == NULL)
    {
        printf("Eroare la deschiderea fisierului %s",nume_poza);
        return ;

    }
    else
    {
        *P=(pixel *)malloc(W*H*sizeof(pixel));
        unsigned char x;
        fseek(f,54,SEEK_SET); //trec de header
        int padding;
    if(W % 4 != 0)
        padding = 4 - (3 * W) % 4;
    else
        padding = 0;
        unsigned int i,j;
        for(i=0;i<H;i++)
        {

            for(j=0;j<W;j++)

       {

            //save the matrix backwards
             fread(&x,1,1,f);
           (*P)[(H-i-1)*W+j].B=x;          //the pixel is stored as (B,G,R)
            fread(&x,1,1,f);
            (*P)[(H-i-1)*W+j].G=x;
            fread(&x,1,1,f);
            (*P)[(H-i-1)*W+j].R=x;
        }
        fseek(f,padding,SEEK_CUR);
        }


    }
    fclose(f);
}

// save the picture externally

void salvez_extern_poza_liniarizata(char *nume_poza,char *poza_originala,pixel *P,unsigned int W, unsigned int H)
{

    FILE * f= fopen(nume_poza,"wb");
    FILE *pep=fopen(poza_originala,"rb");
    unsigned char x;
    int i,k;
    unsigned char y=0;
    for(i=0;i<54;i++) //copy the header
    {
        fread(&x,1,1,pep);
        fwrite(&x,1,1,f);
    }
    int j;
    int padding;               //find the padding
    if(W % 4 != 0)
        padding = 4 - (3 * W) % 4;
    else
        padding = 0;
    for(i=0;i<H;i++)
    {


        for(j=0;j<W;j++)
    {
       //printf("%u %u %u\n",P[(H-i-1)*W+j].R,P[(H-i-1)*W+j].G,P[(H-i-1)*W+j].B);
        fwrite(&P[(H-i-1)*W+j].B,1,1,f);
        fwrite(&P[(H-i-1)*W+j].G,1,1,f);
        fwrite(&P[(H-i-1)*W+j].R,1,1,f);

    }
    //add padding
    for(k=0;k<padding;k++)
         fwrite(&y,1,1,f);

    }
    fclose(f);
    fclose(pep);


}

// generate a sequence of 2*W*H random numbers using the seed R0
void xorShift(unsigned int R0,unsigned int **R,int W, int H)
{
   *R= (unsigned int *)malloc(2*H*W*sizeof(unsigned int));
   int i;
   unsigned long r=R0;
    for( i=1;i<2*H*W;i++)
    {
        r^=r<<13;
        r^=r>>17;
        r^=r<<5;
       (*R)[i]=r;


    }

}
//get the seed and the starting value from a text file
void get_cheie_secreta(unsigned int *R0, unsigned int *SV,char *cheie_secreta)
{
    FILE *f=fopen(cheie_secreta,"r");
    fscanf(f,"%u",R0);
    fscanf(f,"%u",SV);

}
// generate a permutation using the random sequence R
void generare_permutare(unsigned int *R, int W, int H,unsigned int **perm)
{
    *perm=(int *)malloc((W*H)*sizeof(int));
    for(int i=0;i<W*H;i++)
        (*perm)[i]=i;
    for(int i=W*H-1;i>=1;i--)
    {
        int poz =R[W*H-i]%(i+1);
        int aux=(*perm)[poz];
        (*perm)[poz]=(*perm)[i];
        (*perm)[i]=aux;
    }

}
// function that permutes the pixels of an imagine saved in a linearized matrix
void permut_pixeli_imagine(pixel *P,int W, int H,int *perm)
{
    pixel *Pprim;


    Pprim =(pixel *)malloc(W*H*sizeof(pixel));
    int i;
    for(i=0;i<W*H;i++)
    {
        Pprim[perm[i]]=P[i];
    }
    for(int i=0;i<W*H;i++)
        P[i]=Pprim[i];
    free(Pprim);


}
// extract the first three bytes from an integer

void byte(unsigned int x, unsigned char *byte1, unsigned char *byte2, unsigned char *byte3)
{
    unsigned char *p=&x;
    *byte1=p[0];
    *byte2=p[1];
    *byte3=p[2];
}
//crypt a given image using the a sequence of random numbers, a permutation and a starting value
void criptez_imagine(int *R,pixel **C,pixel *P,unsigned int SV,unsigned int W, unsigned int H)
{
        unsigned int R0,R1,R2;
        unsigned int SV0,SV1,SV2;
        int k;
        *C=(pixel *)malloc(W*H*sizeof(pixel));
        byte(R[W*H],&R0,&R1,&R2);
        byte(SV,&SV0,&SV1,&SV2);
        (*C)[0].R=SV2^(P[0].R)^R2;
        (*C)[0].G=SV1^(P[0].G)^R1;
        (*C)[0].B=SV0^(P[0].B)^R0;
        for(k=1;k<W*H;k++)
        {
         byte(R[W*H+k],&R0,&R1,&R2);
        (*C)[k].R=(*C)[k-1].R^P[k].R^R2;
        (*C)[k].G=(*C)[k-1].G^P[k].G^R1;
        (*C)[k].B=(*C)[k-1].B^P[k].B^R0;
        }

}
//save the encrypted picture externally
void salvez_imagine_criptata(char *poza_intiala,char *cheie_secreta,char *imagine_criptata,pixel **Cript)
{
    unsigned int *R,*perm,i;
    pixel *P,*C;
    unsigned int SV=0,R0=0,H=0,W=0;
    get_cheie_secreta(&R0,&SV,cheie_secreta);
    extrag_dimensiuni(poza_intiala,&W,&H);
    (*Cript)=(pixel *)malloc(W*H*sizeof(pixel));
    xorShift(R0,&R,W,H);
    incarc_poza(poza_intiala,&P,W,H);
    generare_permutare(R,W,H,&perm);
    permut_pixeli_imagine(P,W,H,perm);
    criptez_imagine(R,&C,P,SV,W,H);
    for(i=0;i<W*H;i++)
          (*Cript)[i]=C[i];
    salvez_extern_poza_liniarizata(imagine_criptata,poza_intiala,C,W,H);

}

//generate the inverse permutation

void permutare_inversa(int *perm, int **perm_invers,unsigned int W,unsigned int H)
{
    int i,j;
    *perm_invers=(int *)malloc(W*H*sizeof(int));
    for(int i=0;i<W*H;i++)
        (*perm_invers)[perm[i]]=i;

}

//decrypt an encrypted imagine using the inverse permutation and commutativity of the xor operation

void inversul_criptarii(pixel **Cprim,pixel *C, unsigned int *R,unsigned int SV,unsigned int W, unsigned int H)
{
    unsigned int SV0,SV1,SV2,R0,R1,R2;
    *Cprim=(pixel *)malloc(W*H*sizeof(pixel));
    byte(SV,&SV0,&SV1,&SV2);
    byte(R[W*H],&R0,&R1,&R2);
    (*Cprim)[0].R=SV2^C[0].R^R2;
    (*Cprim)[0].G=SV1^C[0].G^R1;
    (*Cprim)[0].B=SV0^C[0].B^R0;
    int k;
    for( k=1;k<W*H;k++)
    {
        byte(R[W*H+k],&R0,&R1,&R2);
        (*Cprim)[k].R=C[k].R^C[k-1].R^R2;
        (*Cprim)[k].G=C[k].G^C[k-1].G^R1;
        (*Cprim)[k].B=C[k].B^C[k-1].B^R0;
    }


}

//apply the algorithm explained above and save the decrypted image externally

void imagine_decriptata(char *poza_cript,char *poza_decrip,char *cheie_secreta,pixel *C)
{

    pixel *Cprim;
    int *R,*perm,*perm_invers;
    unsigned int SV,R0,W,H;
    extrag_dimensiuni(poza_cript,&W,&H);
    get_cheie_secreta(&R0,&SV,cheie_secreta);
    xorShift(R0,&R,W,H);
    generare_permutare(R,W,H,&perm);
    permutare_inversa(perm,&perm_invers,W,H);
    inversul_criptarii(&Cprim,C,R,SV,W,H);
    permut_pixeli_imagine(Cprim,W,H,perm_invers);
    salvez_extern_poza_liniarizata(poza_decrip,poza_cript,Cprim,W,H);

}

// find the value of the chi-squared function for a specific channel

float chi_patrat(int *fr,float fbarat)
{
    int i;
    float chi=0;
    for(i=0;i<256;i++)
        chi=chi+((fr[i]-fbarat)*(fr[i]-fbarat))/fbarat;
    return chi;
}
// save the value of the chi-squared function for all the three channels

void frecvente_pentru_culoare(char *nume_poza,pixel *P,float *chiR,float *chiG,float *chiB)
{
    int *fr;
    *chiB=*chiG=*chiR=0;
    fr=(int *)malloc(256*sizeof(int));
    int i;
    for(i=0;i<256;i++) fr[i]=0;
    unsigned int W,H;
    extrag_dimensiuni(nume_poza,&W,&H);
    float fbarat = (W*H)/256.0;
    for(i=0;i<W*H;i++)
         fr[P[i].R]++;
    *chiR=chi_patrat(fr,fbarat);
    for(i=0;i<256;i++) fr[i]=0;
    for(i=0;i<W*H;i++)
         fr[P[i].G]++;
    *chiG=chi_patrat(fr,fbarat);
    for(i=0;i<256;i++) fr[i]=0;
    for(i=0;i<W*H;i++)
         fr[P[i].B]++;
    *chiB=chi_patrat(fr,fbarat);

}
