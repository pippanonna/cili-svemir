#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

/* funkcije za obradu signala, navedene ispod main-a */

void obradi_dogadjaj(int sig);
void obradi_sigterm(int sig);
void obradi_sigint(int sig);

int nije_kraj = 1;
int broj;
const char *obrada = "obrada.txt";
const char *status = "status.txt";

int main()
{
	struct sigaction act;
	
	/* 1. maskiranje signala SIGUSR1 */
	act.sa_handler = obradi_dogadjaj; /* kojom se funkcijom signal 		obrađuje */
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGTERM); /* blokirati i SIGTERM za vrijeme 		obrade */
	act.sa_flags = 0; /* naprednije mogućnosti preskočene */
	sigaction(SIGUSR1, &act, NULL); /* maskiranje signala preko sučelja 		OS-a */
	
	/* 2. maskiranje signala SIGTERM */
	act.sa_handler = obradi_sigterm;
	sigemptyset(&act.sa_mask);
	sigaction(SIGTERM, &act, NULL);
	
	/* 3. maskiranje signala SIGINT */
	act.sa_handler = obradi_sigint;
	sigaction(SIGINT, &act, NULL);
	
	FILE *statusPtr, *obradaPtr;
	
	/*procitaj broj iz status.txt*/
	statusPtr = fopen(status, "r");
		
	fscanf(statusPtr, "%d", &broj);
	fclose(statusPtr);
	
	/*ako je prethodni program zaustavio SIGINT*/
	if (broj == 0) {
		obradaPtr = fopen(obrada, "r");
		
		int zadnjiProcitani;
		while (fscanf(obradaPtr, "%d/n", &zadnjiProcitani) != EOF)  
			/* ... */ ;
		broj = (int) sqrt(zadnjiProcitani);
		
		fclose(obradaPtr);
		
	}
	
	statusPtr = fopen(status, "w");
	fprintf(statusPtr, "%d", 0);
	fclose(statusPtr);
	
	obradaPtr = fopen(obrada, "a");
	
	while(nije_kraj) {
		broj = broj + 1;
		int x = broj * broj;
		fprintf(obradaPtr, "%d\n", x);
		sleep(5);
		
	}
	
	fclose(obradaPtr);
	
	
	
	return 0;
}

void obradi_dogadjaj(int sig)
{
	printf("%d\n", broj);
}

void obradi_sigterm(int sig)
{	
	FILE *statusPtr;
	statusPtr = fopen(status, "w");
	
	fprintf(statusPtr, "%d", broj);
	fclose(statusPtr);
	
	nije_kraj = 0;
	
}

void obradi_sigint(int sig)
{
	exit(1);
}
