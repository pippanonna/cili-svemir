#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>
#include <unistd.h>

int Id; /* identifikacijski broj segmenta */

int *ZajednickaIzlVarijabla;
int ZajednickaUlVarijabla = 0;
const char *ispis = "ispis.txt";

void *ulaznaDretva(void *br)
{
	printf("Pokrenuta ULAZNA DRETVA\n");

	int *n = br;	
		
	for (int j = 0; j < *n; j++) {
		if (j != 0)
			sleep(3);
	
		while(ZajednickaUlVarijabla != 0) ;
		int broj = rand() % 100 + 1;
		printf("\nULAZNA DRETVA: broj %d\n", broj);
		ZajednickaUlVarijabla = broj;
		
		
	}
	sleep(1);
	printf("\n");
	printf("Završila ULAZNA DRETVA\n");

}

void *radnaDretva(void *br)
{
	printf("Pokrenuta RADNA DRETVA\n");
	
	int i, *n = br;
	
	for (int j = 0; j < *n; j++) {
		while(ZajednickaUlVarijabla == 0) sleep(1);
		
		printf("RADNA DRETVA: pročitan broj %d i povećan na %d\n", 			ZajednickaUlVarijabla, ZajednickaUlVarijabla + 1);
		i = *ZajednickaIzlVarijabla;
		while(i != 0) {
			sleep(1);
			i = *ZajednickaIzlVarijabla;
		}
		*ZajednickaIzlVarijabla = ZajednickaUlVarijabla + 1;
		ZajednickaUlVarijabla = 0;
	
	}
	
	
	sleep(1);
	printf("Završila RADNA DRETVA\n");

}

void brisi(int sig)
{

   /* oslobađanje zajedničke memorije */

   (void) shmdt((char *) ZajednickaIzlVarijabla);

   (void) shmctl(Id, IPC_RMID, NULL);

   exit(0);

}

void procesDvijeDretve(int n) {
	pthread_t thr_id[2];
	
	ZajednickaUlVarijabla = 0;

	/* pokretanje dretvi */
	if (pthread_create(&thr_id[0], NULL, radnaDretva, &n) != 0) {
		printf("Greska pri stvaranju dretve!\n");
		exit(1);
	}
	sleep(1);
	
	if (pthread_create(&thr_id[1], NULL, ulaznaDretva, &n) != 0) {
		printf("Greska pri stvaranju dretve!\n");
		exit(1);
	}
	
	pthread_join(thr_id[0], NULL);
	pthread_join(thr_id[1], NULL);
	
}

void procesJednaDretva(int n) {
	printf("Pokrenut IZLAZNI PROCES\n");
	
	int i;
	
	for (int j = 0; j < n; j++) {
		i = *ZajednickaIzlVarijabla;
		while(i == 0) {
			sleep(1);
			i = *ZajednickaIzlVarijabla;
		}
		
		FILE *ptr;
		ptr = fopen(ispis, "a");
		
		fprintf(ptr, "%d\n", *ZajednickaIzlVarijabla);
		
		fclose(ptr);
		printf("IZLAZNI PROCES: broj upisan u datoteku %d\n",
		*ZajednickaIzlVarijabla);
	
		*ZajednickaIzlVarijabla = 0;
	}
	
	sleep(1);
	printf("Završio IZLAZNI PROCES\n");
	
}

int main(int argc, char *argv[])
{

	if (argc < 2) {
		printf("Premalo argumenata\n");
		exit(1);
	}
	
	
	srand(time(NULL));
	
	truncate(ispis, 0);
	
	int n = atoi(argv[1]);
	/* zauzimanje zajedničke memorije */

	Id = shmget(IPC_PRIVATE, sizeof(int), 0600);
	if (Id == -1)
		exit(1);  /* greška - nema zajedničke memorije */
	
	ZajednickaIzlVarijabla = (int *) shmat(Id, NULL, 0);
	*ZajednickaIzlVarijabla = 0;
	
	struct sigaction act;
	
	act.sa_handler = brisi;  //u slučaju prekida briši memoriju
	sigaction(SIGINT, &act, NULL);

	/* pokretanje paralelnih procesa */
	
	
	
	for (int i = 0; i < 2; i++) {
		switch (fork()) {
		case 0:
			if (i == 0) {
				procesDvijeDretve(n);
			} else {
				procesJednaDretva(n);
			}
			exit(0);
		case -1:
			printf("Nemoguće stvoriti više procesa!");
			exit(1);
		default:
			;
		
		}
	}
	
	for (int i = 0; i < 2; i++) {
		wait(NULL);
	}
	
	brisi(0);
	
	
	return 0;
}
