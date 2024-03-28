#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/semaphore.h"

int pingpong(int N){
    unsigned int sem = 1;
    while(sem_open(sem,0) == 0){
         sem++;
         if(sem == NMAX - 2){
            printf("ERROR: Ha excedido cantidad maxima de semaforos funcionando al mismo tiempo");
            exit(1);
         }
    }
    unsigned int sem2 = sem + 1;

    if (sem_open(sem2,0) == 0){
        printf("ERROR: no se pudo iniciar el semaforo 2");
    }

    int child = fork();
    if(child == -1){
        fprintf(2, "ERROR: falla el hijo en pingpong\n");
    }
    else if (child == 0){
       for(int i = 0; i < N; i++){
            if(sem_down(sem2) == 0){
                printf("ERROR: en el sem_down2");
            }
            printf("\tpong\n");
            if(sem_up(sem) ==0){
                printf("ERROR: en el sem_up1");
            }
        }
        exit(0); 
    }
    else{
        for(int i = 0; i < N; i++){           
            printf("ping\n");
            if(sem_up(sem2) ==0){
                printf("ERROR: en el sem_up2");
            }
            if(sem_down(sem) == 0){
                printf("ERROR: en el sem_down1");
            }
        }
        wait(0);
        if(sem_close(sem) == 0){;
            printf("ERROR: en el seg_close1");
        }
        if(sem_close(sem2) == 0){;
            printf("ERROR: en el seg_close2");
        }
        
    }
    return 1;
}

int
main(int argc, char *argv[])
{
    if(argc != 2){
        printf("ERROR: ha ingresado cantidad incorrecta de argumentos\n");
    }
    int N = atoi(argv[1]);
    if (N <= 0){
        printf("ERROR: ingrese un número mayor que 0\n");
    }
    pingpong(N);
    return 1;
}