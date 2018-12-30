#ifndef TEMPLATE_MATCHING_H_INCLUDED
#define TEMPLATE_MATCHING_H_INCLUDED



#endif // TEMPLATE_MATCHING_H_INCLUDED


void extrag_dimensiuni(char *,unsigned int *,unsigned int *);
void salvez_pixeli_imagine(unsigned int ***,char *);
float medie_pixel_tablou(unsigned int  **, unsigned int, unsigned int);
double deviatie_standard(unsigned int **,unsigned int, unsigned int);
double corelatie(unsigned int **, unsigned int **,unsigned int, unsigned int,double,double);
void extrag_nume_sabloane(char ***,char *);
void extrag_culori(culoare **, char *);
void get_fereastra_get_medie_feresatra(int **,int **, unsigned int ,unsigned int,int, int,double *);
void desenez_contur_fereastra(culoare **,unsigned int, unsigned int,detectii,culoare);
void incarc_imagine_color(char *,culoare ***);
void salvez_extern_poza(char *,char *,culoare **);
void gasesc_detectii_sablon_x(int **,culoare **,char *,unsigned int,unsigned int, unsigned int, unsigned int,float,culoare, detectii *,int *,int);
void template_matching(char *,char *,char **,char *,float,culoare *,detectii **,int *);
int cmp(const void * , const void *);
void get_colturi_fereastra(coord *,coord *,detectii , unsigned int, unsigned int);
int maxi(int, int);
int mini(int, int);
double arie_suprapunere_detectii(coord,coord,coord,coord);
double suprapunere_spatiala(detectii , detectii );
void eliminare_non_maxime(detectii *, int ,char *,char *,culoare *);






