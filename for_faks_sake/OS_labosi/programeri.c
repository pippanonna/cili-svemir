#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

pthread_mutex_t m;
pthread_cond_t uv[2];
char lin[10] = "Linux";
char ms[10] = "Microsoft";
int imaProgramera[2];
int siti[2];
int ceka[2];
int n = 3;

void udi(int vrsta) {
	pthread_mutex_lock (&m);
	
	ceka[vrsta]++;
	
	while(imaProgramera[1 - vrsta] > 0 || siti[vrsta] == n) {
		printf("Programer %d kaze: Zasto trebam cekati? :(\n", vrsta);
		pthread_cond_wait(&uv[vrsta], &m);
	}
	
	ceka[vrsta]--;
	imaProgramera[vrsta]++;
	
	if(ceka[1 - vrsta] > 0) {
		printf("Programer %d kaze: Sta me ovi pozuruju??? \n", vrsta);
		siti[vrsta]++;
	}	
		
	pthread_mutex_unlock(&m);
}

void izadi(int vrsta) {
	pthread_mutex_lock (&m);
	
	printf("Vrsta: %d Br programera: %d\n", vrsta, imaProgramera[vrsta]);
	imaProgramera[vrsta]--;
	
	if (imaProgramera[vrsta] == 0) {
		siti[1 - vrsta] = 0;
		pthread_cond_broadcast(&uv[1 - vrsta]);
	}
	
	pthread_mutex_unlock (&m);
}

void *programer(void *br) {
	int *vrsta = br;
	char ime[20];
	
	if (*vrsta) {
		strcpy(ime, lin);
	} else {
		strcpy(ime, ms);
	}
	
	udi(*vrsta);
	printf("%s programer ulazi u restoran\n", ime);
	
	//jedi();
	int komentar = rand() % 2;
	printf("%s programer: ", ime);
	if (komentar) {
		printf("Ovo je najbolji obrok koji sam u zivotu pojeo!\n");
	} else {
		printf("Kakva je ovo splacina??\n");
	}
	sleep(1);
	
	printf("%s programer izlazi iz restorana\n", ime);
	izadi(*vrsta);
	
}


int main() {
	srand(time(NULL)); 

	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&uv[0], NULL);
	pthread_cond_init(&uv[1], NULL);
	
	int brMS;
	int brLinux;
	
	printf("Upisite broj Microsoft programera: ");
	scanf("%d", &brMS);
	printf("Upisite broj Linux programera: ");
	scanf("%d", &brLinux);
	
	for (int i = 0; i < 2; i++) {
		imaProgramera[i] = 0;
		siti[i] = 0;
		ceka[i] = 0;
	}
	
	
	pthread_t thr_id[brMS + brLinux];
	int nMS = 0, nLin = 1;
	
	int prosliMS = 0;
	int prosliLinux = 0;

	/* pokretanje dretvi */
	while(brMS > 0 || brLinux > 0) {
		if (brMS > 0) {
			int randBroj = rand() % brMS + 1;
			for (int i = 0; i < randBroj; i++) {
			if (pthread_create(&thr_id[prosliMS + i], NULL, programer, &nMS) != 0) {
				printf("Greska pri stvaranju dretve!\n");
				exit(1);
			}
			}
			prosliMS += randBroj;
			brMS -= randBroj;
		}
		if (brLinux > 0) {
			int randBroj = rand() % brLinux + 1;
			for (int i = 0; i < randBroj; i++) {
			if (pthread_create(&thr_id[prosliLinux + i], NULL, programer, &nLin) != 0) {
				printf("Greska pri stvaranju dretve!\n");
				exit(1);
			}
			}
			prosliLinux += randBroj;
			brLinux -= randBroj;
		}
		sleep(1);
	}
	sleep(2);
	
	for (int i = 0; i < brMS + brLinux; i++) {
		printf("Br oslobodenih dretvi: %d\n", i);
		pthread_join(thr_id[i], NULL);
	}
	
	
	pthread_mutex_destroy(&m);
	
}
