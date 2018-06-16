// Otávio da Cruz Mello
// Sistemas Operacionais
// Sincronização com Semáforos
// Compilar com: gcc -o otavio otavio.c -pthread

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<semaphore.h>
#include<unistd.h>
#include<pthread.h>
#include<stdbool.h>
#include<time.h>

sem_t mutex; // semaforo para exclusao mutua
int ocupado=0; // variavel global que controla o numero de clientes na linha de ocupados

struct args{
   int numeroCliente;
};

struct args * arg;

void ligacao(struct args * arg)
{
   sem_wait(&mutex); // entra na sessão critica
   ocupado--; // remove um de ocupado, pois um dos clientes ja foi atendido
   // Atendente realiza o atendimento e cliente é atendido
   printf("Atendente esta falando com algum cliente! Ha %d chamadas em espera\n",ocupado);
   printf("Cliente %d esta sendo atendido! Ha %d chamadas em espera\n",arg->numeroCliente,ocupado);
   sleep(rand()%15); // tempo de ligação
   sem_post(&mutex); // libera a sessão critica
}

void ligando(int j, int num_cliente)
{
   pthread_t cliente;
   printf("Cliente %d esta ligando! Havia %d chamadas em espera\n",num_cliente,ocupado);
   if(ocupado<j){ // se ainda existirem linhas que não estão ocupadas
      arg = malloc(sizeof(struct args));
      if(arg==NULL){
         printf("Sem Memoria\n");
         exit(1);
      }
      arg->numeroCliente = num_cliente;
      ocupado++; // adiciona mais uma linha ocupada
      if(pthread_create(&cliente,NULL,(void*) ligacao,arg)){
         printf("Erro ao criar thread.\n");
         exit(1);
      }
   }
   else{ // se todas as linhas estiverem ocupadas
      printf("Cliente %d nao consegue realizar a chamada. Todas as linhas ocupadas\n",num_cliente);
   }
}

int main(int argc, char *argv[]){
   int num_cliente=0;
   srand(time(NULL));
   // condicoes de inicio do programa
   if(argc!=2){
      printf("O programa exige a insercao de um argumento (numero de linhas).\n");
      exit(1);
   }
   if(atoi(argv[1]) <= 0){
      printf("O argumento deve ser positivo e maior que zero.\n");
      exit(1);
   }
   printf("----------------------------------------------------------------------------------\n");
   printf("         Call Center do Otavio (numero de linhas de espera: %d)\n",atoi(argv[1]));
   printf("----------------------------------------------------------------------------------\n");
   sem_init(&mutex,0,1); // inicia o semaforo
   while(true){
      num_cliente++; // numero do cliente atual
      ligando(atoi(argv[1]),num_cliente); // cliente realiza a ligacao
      sleep(1+rand()%5); // tempo até surgir outro cliente (adicionado 1 pra não ficar tão rapido)
   }
}
