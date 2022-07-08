#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int ID[4];
sem_t *mjesto;
sem_t *sjeo;
sem_t *kraj;
sem_t *sisao;

void vrtuljak(int br, int br_pon) {
	while(br_pon > 0) {
		for(int i = 0; i < br; i++) {
			sem_post(mjesto);
		}
		for(int i = 0; i < br; i++) {
			sem_wait(sjeo);
		}
		
		// pokreni vrtuljak;
		printf("Svi su sjeli, pocinjem voznju\n");
		sleep(1);
      		// zaustavi vrtuljak;
      		printf("Kraj voznje\n");
      		
      		for(int i = 0; i < br; i++) {
			sem_post(kraj);
		}
		for(int i = 0; i < br; i++) {
			sem_wait(sisao);
		}
		printf("Svi su sisli\n");
		br_pon--;
	}
}

void posjetitelj() {
	pid_t mojpid = getpid();
	
	sem_wait(mjesto);
	
	//sjedi();
	printf("Proces %d: Sjeo sam!\n", mojpid);
	sem_post(sjeo);
	
	printf("Proces %d: Woooooooohoooooo\n", mojpid);
	
	sem_wait(kraj);
	
	//sidji();
	printf("Proces %d: Sisao sam!\n", mojpid);
	sem_post(sisao);
}



int main() {
	int n;
	printf("Upisite broj mjesta: ");
	scanf("%d", &n);
	
	int br_posjet;
	do {
		printf("Upisite broj posjetitelja: ");
		scanf("%d", &br_posjet);
		if (br_posjet % n != 0) 
			printf("Broj posjetitelja mora biti djeljiv s brojem mjesta!\n");
	} while(br_posjet % n != 0);
	
	ID[0] = shmget (IPC_PRIVATE, sizeof(sem_t), 0600);
	mjesto = shmat (ID[0], NULL, 0);
	shmctl (ID[0], IPC_RMID, NULL);
	sem_init(mjesto, 1, 0); //početna vrijednost = 0, 1=>za procese
	
	ID[1] = shmget (IPC_PRIVATE, sizeof(sem_t), 0600);
	sjeo = shmat (ID[1], NULL, 0);
	shmctl (ID[1], IPC_RMID, NULL);
	sem_init(sjeo, 1, 0);
	
	ID[2] = shmget (IPC_PRIVATE, sizeof(sem_t), 0600);
	kraj = shmat (ID[2], NULL, 0);
	shmctl (ID[2], IPC_RMID, NULL);
	sem_init(kraj, 1, 0);
	
	ID[3] = shmget (IPC_PRIVATE, sizeof(sem_t), 0600);
	sisao = shmat (ID[3], NULL, 0);
	shmctl (ID[3], IPC_RMID, NULL);
	sem_init(sisao, 1, 0);	
	
	switch (fork()) {
	case 0:
		vrtuljak(n, br_posjet / n);
		exit(0);
	case -1:
		printf("Nemoguće stvoriti više procesa!");
		exit(1);
	default:
		;
	
	}
	
	while(br_posjet > 0) {
		for(int i = 0; i < n; i++) {
			switch (fork()) {
			case 0:
				posjetitelj();
				exit(0);
			case -1:
				printf("Nemoguće stvoriti više procesa!");
				exit(1);
			default:
				br_posjet--;
			
			}
		}
		sleep(2);
		for (int i = 0; i < n; i++) {
			wait(NULL);
		}
	}
	
	wait(NULL);
	
	sem_destroy(mjesto);
	shmdt(mjesto);
	
	sem_destroy(sjeo);
	shmdt(sjeo);
	
	sem_destroy(kraj);
	shmdt(kraj);
	
	sem_destroy(sisao);
	shmdt(sisao);
	
	return 0;
}
