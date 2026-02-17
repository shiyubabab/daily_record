/*************************************************************************
	> File Name: producer.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 17 Feb 2026 08:43:40 PM CST
 ************************************************************************/

#include "common.h"

int main(void)
{
	int shmid = shmget(SHM_KEY, sizeof(struct shared_data), IPC_CREAT | 0666);
	struct shared_data *data = (struct shared_data *)shmat(shmid,NULL,0);

	int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
	semctl(semid, 0, SETVAL, 1);
	
	int count = 0;
	while(1) {
		semop(semid, &sem_lock, 1);

		data->id = ++count;
		snprintf(data->message,MAX_SIZE,"Industrial Payload #%d",count);
		data->update_flag = 1;
		printf("[Producer] Wrote:%s\n",data->message);

		semop(semid, &sem_unlock, 1);

		sleep(1);
	}
	shmdt(data);
	return 0;
}

