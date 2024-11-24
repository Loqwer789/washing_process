#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <string.h>
#include <stdbool.h>

int main()
{

char  file_path[] = "posuda_washer.txt";
key_t key;

struct buff 
{
long mtype;
int dishes_type;
int free_place;
	
}mybuff;

if (( key == ftok(file_path, 0)) < 0)  // проверка на создание процесса
{
printf("Невозможно сгенерировать ключ процесса\n");
exit(1);
}	

int msqid ;
if (( msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
{
printf("невозможно получить номер процесса");
exit(1);
}	


int length = sizeof( struct buff);
FILE *dirty_dishes; // инициализиуем два файла

FILE *dryer_process ;

if (( dryer_process = fopen("posuda_process.txt", "r")) == NULL)  //проверим файлы на открытие
{
printf("Ошибка\n");
exit(1);
}	

if (( dirty_dishes = fopen("posuda_number.txt","r")) == NULL)
{
printf("Ошибка\n");
exit(1);
}	


char online_dryer[5], online_dirty_dishes[5];
int time_dryer, type_dish, quantity_dirty_dishes;

while( fgets(online_dirty_dishes, sizeof(online_dirty_dishes), dirty_dishes) != NULL) 
{
sscanf(online_dirty_dishes, "%d:%d\n", &mybuff.dishes_type, &quantity_dirty_dishes);
printf("тип посуды: %d количество %d\n", mybuff.dishes_type, quantity_dirty_dishes);
rewind(dryer_process);


	do {
	fgets(online_dryer, sizeof(online_dryer), dryer_process);
	sscanf(online_dryer,"%d:%d\n", &type_dish, &time_dryer);

	}while(type_dish != mybuff.dishes_type);	
}

while(quantity_dirty_dishes != 0 )
{
	puts("начинаю сушить\n");
	sleep(time_dryer);
	puts("заканчиваю сушить\n");
	mybuff.free_place --;
	quantity_dirty_dishes --;
	mybuff.mtype = 1;

	if(mybuff.free_place == 0)
	{
		printf("закончилось место");
		if(msgrcv(msqid, (struct buff*) &mybuff, length,2, 0) < 0)
		{
			printf("ошибка продвижения по очереди\n");
			fclose(dirty_dishes);
			fclose(dryer_process);

		}	

	
	}

}	


}










