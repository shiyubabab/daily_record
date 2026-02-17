/*************************************************************************
	> File Name: comsumer.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Tue 17 Feb 2026 09:01:38 PM CST
 ************************************************************************/

#include "common.h"

int main(void)
{
	int shmid = shmget(SHM_KEY, sizeof(struct shared_data), 0666);
	struct shared_data *data = (struct shared_data *)shmat(shmid,NULL,0);
	int semid = semget(SEM_KEY, 1, 0666);

	while(1){
		semop(semid, &sem_lock, 1);

		if(data->update_flag == 1){
			printf("[Consumer] Received: ID=%d, MSG=%s \n",data->id,data->message);
			data->update_flag = 0;
		}

		semop(semid, &sem_unlock, 1);
		usleep(100000);
	}

	shmdt(data);
	return 0;
}

