#include <stdio.h>
#include <stdlib.h>
#include "template_matching.c"
#include "template_matching.h"
#include "criptare.c"
void extrag_nume_poze_parte_criptare(char *nume_poza,char *secret_key,char *nume_poza_cript,char *nume_poza_decript,char *nume_fisier)
{
    FILE *f=fopen(nume_fisier,"r");
    if( f == NULL)
    {
        printf("Nu s-a putut deschide fisierul %s ",nume_fisier);
        return ;
    }
    fgets(nume_poza,50,f);
    fgets(secret_key,50,f);
    fgets(nume_poza_cript,50,f);
    fgets(nume_poza_decript,50,f);
    nume_poza[strlen(nume_poza)-1]='\0';
    secret_key[strlen(secret_key)-1]='\0';
    nume_poza_decript[strlen(nume_poza_decript)-1]='\0';
    nume_poza_cript[strlen(nume_poza_cript)-1]='\0';
    fclose(f);

}
void extrag_nume_poze_parte_template_matching(char *nume_poza_nonmax,char *nume_template_matching,char *nume_poza_gray,char *nume_fisier)
{
    FILE *f=fopen(nume_fisier,"r");
    if( f == NULL)
    {
        printf("Nu s-a putut deschide fisierul %s ",nume_fisier);
        return ;
    }
    fgets(nume_poza_nonmax,50,f);
    fgets(nume_template_matching,50,f);
    fgets(nume_poza_gray,50,f);
    nume_poza_nonmax[strlen(nume_poza_nonmax)-1]='\0';
    nume_template_matching[strlen(nume_template_matching)-1]='\0';
    nume_poza_gray[strlen(nume_poza_gray)-1]='\0';
    fclose(f);


}
int main()
{
    pixel *Cprim;
    char *nume_poza,*nume_poza_decript,*secret_key,*nume_poza_cript;
    nume_poza=(char *)malloc(50*sizeof(char));
    nume_poza_decript=(char *)malloc(50*sizeof(char));
    nume_poza_cript=(char *)malloc(50*sizeof(char));
    secret_key=(char *)malloc(50*sizeof(char));
    if( (nume_poza && nume_poza_cript && nume_poza_decript && secret_key ) == NULL)
    {
        printf("Nu s-a putut aloca memorie pentru denumirile pozelor");
        return ;

    }
    extrag_nume_poze_parte_criptare(nume_poza,secret_key,nume_poza_cript,nume_poza_decript,"fisiere_criptare.txt");

    //criptare

    salvez_imagine_criptata(nume_poza,secret_key,nume_poza_cript,&Cprim);
    imagine_decriptata(nume_poza_cript,nume_poza_decript,secret_key,Cprim);

    //chi-patrat
    float chiR, chiG, chiB,chiR_cript,chiB_cript,chiG_cript;
    unsigned int W,H;
    extrag_dimensiuni(nume_poza,&W,&H);
    pixel *P,*P_cript;
    incarc_poza(nume_poza,&P,W,H);
    incarc_poza(nume_poza_cript,&P_cript,W,H);
    frecvente_pentru_culoare(nume_poza,P,&chiR,&chiG,&chiB);
    printf("%f %f %f\n",chiR,chiG,chiB);

    frecvente_pentru_culoare(nume_poza_cript,P_cript,&chiR_cript,&chiG_cript,&chiB_cript);
    printf("%f %f %f\n",chiR_cript,chiG_cript,chiB_cript);

    free(P);
    free(P_cript);
    free(Cprim);
    free(secret_key);

   // template matching


    culoare **I_color;
    int k;
    detectii *D;
    char **S,**S_gray;
    culoare *C;
    char *nume_poza_gray,*nume_template_matching,*nume_poza_nonmax;
    nume_poza_gray=(char *)malloc(50*sizeof(char));
    nume_poza_nonmax=(char *)malloc(50*sizeof(char));
    nume_template_matching=(char *)malloc(50*sizeof(char));
    extrag_nume_sabloane(&S,"sabloane.txt");
    extrag_nume_sabloane(&S_gray,"s_gray.txt");
    extrag_culori(&C,"culori.txt");
    extrag_nume_poze_parte_template_matching(nume_poza_nonmax,nume_template_matching,nume_poza_gray,"fis_temp_matching.txt");

    grayscale_image(nume_poza,nume_poza_gray);
    template_matching(nume_poza,nume_poza_gray,S_gray,nume_template_matching,0.5,C,&D,&k);  //ne hotaram daca salvam in alta poza sau doar punem detectiile in vector
    eliminare_non_maxime(D,k,nume_poza,nume_poza_nonmax,C);
    free(I_color);
    free(D);
    free(nume_poza_cript);
    free(nume_poza_decript);
    free(nume_poza_gray);
    free(nume_poza_nonmax);
    free(nume_template_matching);
    free(S);
    free(C);
    free(nume_poza);
    free(S_gray);

    return 0;
}
