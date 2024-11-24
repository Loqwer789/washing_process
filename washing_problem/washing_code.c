#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/msg.h>
#include <string.h>
	
#define TABLE_LIMIT 6;



int main()
{
int dishes = open("posuda_number.txt", O_RDONLY);
	if(dishes == -1){
	printf("ошибка открытия файла\n");
	close(dishes);
	exit(1);
	}	
int dryer = open("posuda_process.txt", O_RDONLY);
	if(dryer == -1)
	{
	printf("ошибка открытия файла\n");
	close(dishes);
	exit(1);
	}	
int washer = open("posuda_washer.txt",O_RDONLY);
	if(washer == -1)
	{
	printf("ошибка открытия файла\n");
	close(washer);
	exit(1);
	}	
		
close(dishes);
close(dryer);
close(washer);

char pathname[] = "posuda_washer.txt" ;

struct buff
{
long mtype ;
int dishes_type;
int free_place;
}mybuff;

key_t key;
if ((key == ftok(pathname,0)) < 0)
{
printf("невозможно сгенерировать ключ процесса");
exit(1);
}	

int msqid;
if ((msqid = msgget(key,0666 | IPC_CREAT)) < 0) 
{
printf("невозможно получить номер процесса");
exit(1);

}	

mybuff.free_place = TABLE_LIMIT;

FILE *dirty_dishes;
FILE *washer_process;

if((dirty_dishes = fopen("posuda_number.txt","r")) == NULL)
{
printf("Ошибка\n");
exit(1);


}	
if((washer_process = fopen("posuda_process.txt","r")) == NULL)
{
printf("Ошибка\n");
exit(1);
}	

int lenght = sizeof(struct buff);
char online_dirty_dishes[5], online_washer[5];
int quantity_dirty_dishes, time_washer,type;

while(fgets(online_dirty_dishes, sizeof(online_dirty_dishes), dirty_dishes ) != NULL)
{	
sscanf(online_dirty_dishes,"%d:%d\n", &mybuff.dishes_type, &quantity_dirty_dishes);	
printf("тип посуды: %d количество %d\n", mybuff.dishes_type, quantity_dirty_dishes);
rewind(washer_process);


do{
fgets(online_washer, sizeof(online_washer),washer_process);
sscanf(online_washer,"%d:%d\n",&type, &time_washer);

}while(type != mybuff.dishes_type);
}

while(quantity_dirty_dishes != 0 ) 
{
	puts("начинаю мыть\n");
	sleep(time_washer);
	puts("закончил мыть\n");
	mybuff.free_place --;
	quantity_dirty_dishes--;
	mybuff.mtype = 1;
	
	if (mybuff.free_place == 0) 
	{       
		printf("закончилось пространство");
		if(msgrcv(msqid, (struct buff*) &mybuff, lenght, 2, 0) < 0 )
		{
			printf("ошибка продвижения по очереди\n");
			fclose(dirty_dishes);
			fclose(washer_process);
		}	
		
	}
	
}	


}	

