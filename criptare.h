#ifndef CRIPTARE_H_INCLUDED
#define CRIPTARE_H_INCLUDED



#endif // CRIPTARE_H_INCLUDED


void extrag_dimensiuni(char *,unsigned int *,unsigned int *);
void incarc_poza(char *,pixel **,unsigned int , unsigned int );
void salvez_extern_poza_liniarizata(char *,char *,pixel *,unsigned int, unsigned int);
void xorShift(unsigned int,unsigned int **,int , int );
void get_cheie_secreta(unsigned int *, unsigned int *,char *);
void generare_permutare(unsigned int *, int , int ,unsigned int **);
void permut_pixeli_imagine(pixel *,int , int ,int *);
void byte(unsigned int , unsigned char *, unsigned char *, unsigned char *);
void criptez_imagine(int *,pixel **,pixel *,unsigned int,unsigned int, unsigned int);
void salvez_imagine_criptata(char *,char *,char *,pixel **);
void permutare_inversa(int *, int **,unsigned int,unsigned int);
void inversul_criptarii(pixel **,pixel *, unsigned int *,unsigned int ,unsigned int, unsigned int);
void imagine_decriptata(char *,char *,char *,pixel *);
float chi_patrat(int *,float);
void frecvente_pentru_culoare(char *,pixel *,float *,float *,float *);


