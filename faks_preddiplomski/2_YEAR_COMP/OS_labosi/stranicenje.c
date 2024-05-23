#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>



int main(int argc, char *argv[]) {
	srand(time(NULL));

	if (argc < 3) {
		printf("Premalo argumenata\n");
		exit(1);
	}
	
	int n = atoi(argv[1]);
	int m = atoi(argv[2]);
	
	char disk[n][16][64];
	char okvir[m][64];
	short tablica[n][16];
	
	memset( tablica, 0, n*16*sizeof(short) );
	
	printf("%d %d %d\n", rand(), rand(), rand());
	
	int t = 0;
	int a = 1;
	/*int x = 52;
	printf("0x%04x\n", x); */
	
	while(a) {
		for (int i = 0; i < n; i++) {
		printf("-------------------------------\nproces: %d\n", i);
		printf("t: %d\n", t);
			int x = rand() & 0x3FE;
			int indeksStran = (x >> 6) & 0xF;
			int pomak = x & 0x3F;
			int brOkvira = 0;
			
/*			printf("Logicka adresa: 0x%04x\n", x);
			printf("Indeks tablice stranicenja: 0x%04x\n", indeksStran);
			printf("Pomak: 0x%04x\n", pomak); 
*/		
			if ((tablica[i][indeksStran] >> 5 & 0x1) == 0) {
				printf("Promasaj!\n");
				// pronadi i dodijeli okvir
				int adresaMin = 0;
				bool naden = 0;
				bool prvi = 1;
				int bMin = 0, cMin = 0;
				for (int a = 0; a < m; a++) {
				naden = 0;
				for (int b = 0; b < n && !naden; b++) {
				for (int c = 0; c < 16 && !naden; c++) {
					int adresaPom = tablica[b][c];
					//printf("Adresa: %04x %d %d\n", adresaPom, adresaPom >> 6, m);
					if ((adresaPom >> 6) == a && ((adresaPom >> 5) & 0x1) == 0x1) {
					//printf("Yoooo hooo\n%d, %d\nTu sam!\n", b, c);
					naden = 1;
					if (prvi) {
					prvi = 0;
					adresaMin = adresaPom;
					bMin = b; cMin = c;
					} else if ((adresaPom & 0x1F) < (adresaMin & 0x1F)) {
					adresaMin = adresaPom;
					bMin = b; cMin = c;
					}
					}
				}
				}
				if (naden == 0) {
					
					brOkvira = a;
					break;
				}
					
				}
				
				if (naden) {
					printf("Izbacujem stranicu %04x iz procesa %d\n", adresaMin, bMin);
					brOkvira = adresaMin >> 6;
					tablica[bMin][cMin] = brOkvira << 6;
					
				}
					
				printf("Dodijeljen okvir %04x\n", brOkvira);
				okvir[brOkvira][pomak] = disk[i][indeksStran][pomak];
				int adresaTablice = (brOkvira * 2 + 1) * pow(2, 5) + t;
				
				tablica[i][indeksStran] = adresaTablice;
				//printf("Nova adresa: %04x\n Tablica: %04x\n", adresaTablice, tablica[i][indeksStran]);
			} else {
				printf("Pogodak!\n");
				tablica[i][indeksStran] = ((tablica[i][indeksStran] >> 5) << 5) + t;
				brOkvira = tablica[i][indeksStran] >> 6;
			}
			printf("Logicka adresa: 0x%04x\n", x);
			printf("Zapis tablice: 0x%04x\n", tablica[i][indeksStran]);
			
			disk[i][indeksStran][pomak] += 1;
			t += 1;
			if (t == 32) {
				t = 0;
				int adresa;
				
				for (int j = 0; j < n; j++) {
					for (int k = 0; k < 16; k++) {
					adresa = tablica[j][k];
					adresa = adresa >> 5 << 5;
					tablica[j][k] = adresa;
					}
				}
				adresa = tablica[i][indeksStran];
				adresa = adresa >> 5 << 5 + 1;
				tablica[i][indeksStran] = adresa;
			}
			
			
			printf("-------------------------------\n");
			sleep(1);
		}
		
		
	}
	
	
}
