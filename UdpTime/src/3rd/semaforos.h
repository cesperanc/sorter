/**
* @file semaforos.h
* @brief API para semáforos UNIX
* @date 02-05-2005
* @author {vmc, rui, nuno.costa, adias, loureiro}@estg.ipleiria.pt
*/

#ifndef SEMAFOROS_H
#define SEMAFOROS_H

#if _SEM_SEMUN_UNDEFINED
	union semun 
	{
	   int val;                /* valor para SETVAL */
	   struct semid_ds *buf;   /* buffer para IPC_STAT, IPC_SET */
	   unsigned short *array;  /* array para GETALL, SETALL */
	};
#endif


/* Protótipos da API para semáforos UNIX */


/**
* Cria ou abre um conjunto de semáforos identificado por sem_key
* Esta função apenas existe por uma questão coerência.
* @param sem_key chave do recurso
* @param num_of_sems número de semáforos a criar
* @param sem_flags  opções de criação
* @return identificador do conjunto de semáforos ou -1 em caso de erro
*/
int sem_create(key_t sem_key, int num_of_sems, int sem_flags);


/**
* Inicializa o valor de um conjunto de semáforos identificado por sem_id
* @param sem_id identificador do conjunto de semáforos
* @param *values array com valores a atribuir ao conjunto de semáforos
* @return -1 em caso de erro ou 0 em caso de sucesso
*/
int sem_init(int sem_id, unsigned short *values);


/**
* Remove um conjunto de semáforos identificado por sem_id
* @param sem_id identificador do conjunto de semáforos
* @return 0 em caso de sucesso e -1 em caso de erro.  
*/
int sem_delete(int sem_id);


/**
* Especifica o número de recursos que o semáforo deverá controlar
* @param sem_id identificador do conjunto de semáforos
* @param sem_num índice do semáforo a contemplar (começa em 0)
* @param valor número de recursos 
* @return -1 em caso de erro ou 0 em caso de sucesso
*/
int sem_setvalue(int sem_id, int sem_num, int valor);


/**
* Devolve o número actual de recursos ainda disponíveis no semáforo visado
* @param sem_id identificador do conjunto de semáforos
* @param sem_num índice do semáforo a contemplar (começa em 0)
* @return -1 em caso de erro ou o número de recursos disponiveis no semáforo
*/
int sem_getvalue(int sem_id, int sem_num);


/**
* Incrementa, em 1, o número de recursos disponiveis no semáforo visado
* @param sem_id identificador do conjunto de semáforos
* @param sem_num índice do semáforo a contemplar (começa em 0)
* @return -1 em caso de erro ou 0 em caso de sucesso 
*/
int sem_up(int sem_id, int sem_num);


/**
* Decrementa, em 1, o número de recursos disponiveis no semáforo visado
* @param sem_id identificador do conjunto de semáforos
* @param sem_num índice do semáforo a contemplar (começa em 0)
* @return -1 em caso de erro ou 0 em caso de sucesso 
*/
int sem_down(int sem_id, int sem_num);


#endif
