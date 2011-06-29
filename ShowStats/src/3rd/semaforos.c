/**
* @file semaforos.c
* @brief API para semáforos UNIX
* @date 02-05-2005
* @author {vmc, rui, nuno.costa, adias, loureiro}@estg.ipleiria.pt
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaforos.h"

/**
* Cria ou abre um conjunto de semáforos identificado por sem_key
* Esta função apenas existe por uma questão coerência.
* @param sem_key chave do recurso
* @param num_of_sems número de semáforos a criar
* @param sem_flags  opções de criação
* @return identificador do conjunto de semáforos ou -1 em caso de erro
*/
int sem_create(key_t sem_key, int num_of_sems, int sem_flags)
{
	return semget(sem_key, num_of_sems, sem_flags);	
}

/**
* Inicializa o valor de um conjunto de semáforos identificado por sem_id
* @param sem_id identificador do conjunto de semáforos
* @param *values array com valores a atribuir ao conjunto de semáforos
* @return -1 em caso de erro ou 0 em caso de sucesso
*/
int sem_init(int sem_id, unsigned short *values)
{
	union semun arg;
	arg.array = values;
		
	return semctl(sem_id, 0, SETALL, arg);
}

/**
* Remove um conjunto de semáforos identificado por sem_id
* @param sem_id identificador do conjunto de semáforos
* @return 0 em caso de sucesso e -1 em caso de erro.  
*/
int sem_delete(int sem_id)
{
	return semctl(sem_id, 0, IPC_RMID, (union semun) 0);
}

/**
* Especifica o número de recursos que o semáforo deverá controlar
* @param sem_id identificador do conjunto de semáforos
* @param sem_num índice do semáforo a contemplar (começa em 0)
* @param valor número de recursos 
* @return -1 em caso de erro ou 0 em caso de sucesso
*/
int sem_setvalue(int sem_id, int sem_num, int valor)
{
	union semun arg;
	arg.val = valor;
		
	return semctl(sem_id, sem_num, SETVAL, arg);
}

/**
* Devolve o número actual de recursos ainda disponíveis no semáforo visado
* @param sem_id identificador do conjunto de semáforos
* @param sem_num índice do semáforo a contemplar (começa em 0)
* @return -1 em caso de erro ou o número de recursos disponiveis no semáforo
*/
int sem_getvalue(int sem_id, int sem_num)
{
	return semctl(sem_id, sem_num, GETVAL, (union semun) 0);
}

/**
* Incrementa, em 1, o número de recursos disponiveis no semáforo visado
* @param sem_id identificador do conjunto de semáforos
* @param sem_num índice do semáforo a contemplar (começa em 0)
* @return -1 em caso de erro ou 0 em caso de sucesso 
*/
int sem_up(int sem_id, int sem_num)
{
	struct sembuf buf = {sem_num, 1, 0};
	return semop(sem_id, &buf, 1);
}

/**
* Decrementa, em 1, o número de recursos disponiveis no semáforo visado
* @param sem_id identificador do conjunto de semáforos
* @param sem_num índice do semáforo a contemplar (começa em 0)
* @return -1 em caso de erro ou 0 em caso de sucesso 
*/
int sem_down(int sem_id, int sem_num)
{
	struct sembuf buf = {sem_num, -1, 0};
	return semop(sem_id, &buf, 1);
}

