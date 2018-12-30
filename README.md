
# Proiect Programare Procedurală

## Tema Proiectului 

 Scenariul de implementat în acest proiect este următorul:
 
  * persoana A îi trimite persoanei B o imagine I pe care o criptează folosind un algoritm de criptare. 
  * B poate decripta imaginea criptată primită de la A obținând astfel imaginea inițială I după care folosește un algoritm de recunoaștere a cifrelor scrise de mână în imaginea I. 
  * Persoana B trimite apoi persoanei A imaginea rezultată obținută pe care o criptează.
  * Persoana A decriptează imaginea criptată primită de la B și poate vizualiza soluția, recunoașterea cifrelor scrise de mână. 
---

### Modul Criptare
 
  Algoritmul de criptare se bazează pe generarea unei secvențe aleatoare de numere(vezi agloritmul [Xorshift](https://en.wikipedia.org/wiki/Xorshift)), folosită ulterior pentru generarea unei permutări aleatoare (vezi agloritmul [Durstenfeld](https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle)).
  După permutarea pixelilor din imagine,se aplică operația *XOR* între aceștia, rezultând imaginea criptată.
  
  Imagine inițială:
  
  ![alt text](https://github.com/Sergiu154/Proiect-Programare-Procedurala/blob/master/test.bmp)
  
  ---
  Imaginea criptată:
  
  ![alt text](https://github.com/Sergiu154/Proiect-Programare-Procedurala/blob/master/test_cripted.bmp)
  
  ---
  Algoritmul de decriptare are la bază proprietățile operației *XOR*,se aplică invers pașii de la punctul anterior.
  
 Imaginea decriptată:
 
   ![alt text](https://github.com/Sergiu154/Proiect-Programare-Procedurala/blob/master/test_decripted.bmp)
   
 ---

 ### Modul Template Matching
 
   Pentru imaginea dată, vom extrage toate ferestrele posibile de dimensiunea șablonului dat,ca mai apoi să calculăm corelația dintre cele două.În cazul în care corelația depășește un prag *ps*, fereastra dată este considerată o detecție, se salvează ,iar cifra este încadrată într-un chenar colorat.
   
   După rularea algoritmului pentru toate cele 10 șabloane(cifrele 0-9),rezultă următoarea imagine:
   
   ![alt text](https://github.com/Sergiu154/Proiect-Programare-Procedurala/blob/master/test_template.bmp)
   
   ---
   
   Deoarece foarte multe detecții se suprapun,dorim să le eliminăm pe cele care au o suprapunere spațiala mai mare decât un prag dat și corelația cât mai mică.Asftel,după aplicarea algorimtului de eliminare a non-maximelor,va rezulta imaginea:
   
   ![alt text](https://github.com/Sergiu154/Proiect-Programare-Procedurala/blob/master/test_nonmax.bmp)
   
   
 
 
 
