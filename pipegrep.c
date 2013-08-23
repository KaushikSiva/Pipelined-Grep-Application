#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <fcntl.h>
#include <string.h>


#define TRUE 1

int success_count=0;
int BUFFER_SIZE;
int filesize;
int uid;
int gid;
char *word;

pthread_t thr_1,thr_2,thr_3,thr_4,thr_5;  
pthread_mutex_t mutex,mutex2_3,mutex3_4,mutex4_5;
sem_t full, empty,full2_3,empty2_3,empty3_4,full3_4,empty4_5,full4_5;
int counter,counter2_3,counter3_4,counter4_5;
    
char **buffer,**buffer2_3,**buffer3_4,**buffer4_5;

void initializeData();
void *produce_1(void *param); 
void *consume_1_produce_2(void *param); 
void *consume_2_produce_3(void *param);
void *consume_3_produce_4(void *param);
void *consume_4_produce_5(void *param);

int main(int argc, char *argv[])
 {
   
	if(argc != 6)
   	{
      		fprintf(stderr, "USAGE:./pipegrep <buffersize> <filesize> <uid> <gid> <word> \n");
   	}

   	BUFFER_SIZE = 1; 
	filesize = atoi(argv[2]); 
   	uid = atoi(argv[3]); 
  	gid = atoi(argv[4]);
   	word=argv[5];
   
   	initializeData();

   	pthread_create(&thr_1,NULL,produce_1,NULL);
   	pthread_create(&thr_2,NULL,consume_1_produce_2,NULL);
	pthread_create(&thr_3,NULL,consume_2_produce_3,NULL);
	pthread_create(&thr_4,NULL,consume_3_produce_4,NULL);
	pthread_create(&thr_5,NULL,consume_4_produce_5,NULL);

   	pthread_join(thr_1,NULL);
   	pthread_join(thr_2,NULL);
	pthread_join(thr_3,NULL);
	pthread_join(thr_4,NULL);
	pthread_join(thr_5,NULL);
	printf("********* You have found %d matches *******",success_count);
	printf("\nYou are done!!!!\n");
   	exit(0);
}



void initializeData() 
{
	
	buffer=(char **)malloc(BUFFER_SIZE);
	buffer2_3=(char **)malloc(BUFFER_SIZE);
	buffer3_4=(char **)malloc(BUFFER_SIZE);	
	buffer4_5=(char **)malloc(BUFFER_SIZE);


	pthread_mutex_init(&mutex, NULL);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	counter = 0;


	pthread_mutex_init(&mutex2_3, NULL);
	sem_init(&full2_3, 0, 0);
	sem_init(&empty2_3, 0, BUFFER_SIZE);
	counter2_3 = 0;

	pthread_mutex_init(&mutex3_4,NULL);
	sem_init(&full3_4,0,0);
	sem_init(&empty3_4,0,BUFFER_SIZE);
	counter3_4 = 0;

	pthread_mutex_init(&mutex4_5,NULL);
	sem_init(&full4_5,0,0);
	sem_init(&empty4_5,0,BUFFER_SIZE);
	counter4_5 = 0;


}


int nooffiles=0;

void *produce_1(void *param) 
{
   	char **file_name;
      	DIR             *dp;
        struct dirent   *de;
	int          i=0;
	file_name=(char **)malloc(1000);
	if ((dp = opendir(".")) == NULL)
       	{
               	perror("opendir problem:");
               	return 0;
        }
 	while ((de = readdir(dp)) != NULL)
        {
		if (de->d_type != DT_REG) 
        	continue;
		file_name[nooffiles]=(char *)malloc(strlen(de->d_name)+1);
               	strcpy(file_name[nooffiles], de->d_name);
                nooffiles++;
        }
 	//printf("\nfound a total of %d files\n",nooffiles);
 	if (closedir(dp) == -1)
        {
        	perror("closedir problem:");
               	return 0;
        }
 	
        while(TRUE) 
	{
      		sem_wait(&empty);
      		pthread_mutex_lock(&mutex);

		if(i<nooffiles)
                {		
			if(counter < BUFFER_SIZE) 
			{
      				buffer[counter] = file_name[i];
      				counter++;
				//printf("\nproducer_1 produced :%s\n",file_name[i]);
                        }
			i++;
		}
		pthread_mutex_unlock(&mutex);
      		sem_post(&full);

		if(i==nooffiles)
		break;
              

      	}
}

int nooffiles2_3 =0;
void *consume_1_produce_2(void *param)
 {
   		
		char *file_names[5000];
		char *filter_file_names[3000];
		char *file_name;
                int i=0,j=0;
		struct stat file_info;                 

              	while(TRUE) 
		{
      			sem_wait(&full);
      			pthread_mutex_lock(&mutex);
                      	if(i<nooffiles)
			{
	                        if(counter > 0)
				{
					file_name = buffer[(counter-1)];
					file_names[i]=file_name;
					counter--;
					//printf("\nconsumer_1 consumed :%s\n",file_name);	
                                        
				}
				i++;
				
                         }
			pthread_mutex_unlock(&mutex);
      			sem_post(&empty);
			
			if(i==nooffiles)
			break;
	
		}
		j=0;
		for(i=0;i<nooffiles;i++)
		{
			 int file=0;
			 
        		 if((file=open(file_names[i],O_RDONLY)) < -1)
           		 return (int *)1;
			
			 if(fstat(file,&file_info) < 0)    
        		 return (int *)1;
			
			//printf("\nfile name: %s size of file:%d uid:%d  gid:%d\n",file_names[i],file_info.st_size,file_info.st_uid,file_info.st_gid);
			
			 if((file_info.st_size>filesize || filesize==-1) && (file_info.st_uid==uid || uid==-1) && (file_info.st_gid==gid || gid==-1))
			 {
				filter_file_names[j]=file_names[i];
				j++;
				nooffiles2_3++;
			 }
		}
		
		i=0;
		while(TRUE) 
		{
      			sem_wait(&empty2_3);
      			pthread_mutex_lock(&mutex2_3);

			if(i<nooffiles2_3)
                	{		
				if(counter2_3 < BUFFER_SIZE) 
				{
      					buffer2_3[counter2_3] = filter_file_names[i];
      					counter2_3++;
					//printf("\nproducer_2 produced :%s\n",filter_file_names[i]);
                        	}
				i++;
			}
			pthread_mutex_unlock(&mutex2_3);
      			sem_post(&full2_3);

			if(i==nooffiles2_3)
			break;
              }

}


char *file_lines[200000];
int count=0;

void read_file(char *filename)
{
  	FILE *file = fopen (filename, "r" );
	char *lines;
	
	int j=0;
        if ( file != NULL )
   	{
		char line [ 500]; 
		j=0;
      		while ( fgets ( line, sizeof(line), file ) != NULL ) 
      		{
         		j++;
			lines=(char *)malloc(1000);
			sprintf(lines,"./%s (%d)",filename,j);
			strcat(lines,line);
			file_lines[count]=lines;
			count++;
                }
      		
		fclose ( file );
   	}

  	 else
   	{
		printf("\nInvalid file or directory\n");
   	}
	

}

void *consume_2_produce_3(void *param)
 {
   		char *file_name;
		char *file_names;
		char *filter_file_names[1000];
	
		int i=0,j=0,len;

              	while(TRUE) 
		{
      			sem_wait(&full2_3);
      			pthread_mutex_lock(&mutex2_3);
                      	if(i<nooffiles2_3)
			{
	                        if(counter2_3 > 0)
				{
					file_name = buffer2_3[(counter2_3-1)];
				        filter_file_names[i]=file_name;
					counter2_3--;
					//printf("\nconsumer_2 consumed :%s\n",file_name);	
                                        
				}
				i++;
				
                         }
			pthread_mutex_unlock(&mutex2_3);
      			sem_post(&empty2_3);
			
			if(i==nooffiles2_3)
			{
			
				break;
			}
	
		}

			for(i=0;i<nooffiles2_3;i++)
			{
			
				//if(strcmp(filter_file_names[i],"pipegrep")==0)
				//continue;
				read_file(filter_file_names[i]);
			}
		
		i=0;
		
		while(TRUE) 
		{
      			sem_wait(&empty3_4);
      			pthread_mutex_lock(&mutex3_4);

			if(i<count)
                	{		
				if(counter3_4 < BUFFER_SIZE) 
				{
      					buffer3_4[counter3_4] = file_lines[i];
      					counter3_4++;
				}
				i++;
			}
			pthread_mutex_unlock(&mutex3_4);
      			sem_post(&full3_4);

			if(i==count)
			break;
              }
			
}
char *success_line[100000];
int temp=0;
void *consume_3_produce_4(void *param)
{
		char *file_lines_out[100000];
		char *file_line;
		char *file_line_search;
		int i=0,j=0;
		                 
		while(TRUE) 
		{
      			sem_wait(&full3_4);
      			pthread_mutex_lock(&mutex3_4);
                      	if(i<count)
			{
	                        if(counter3_4 > 0)
				{
					file_line = buffer3_4[(counter3_4-1)];
					file_lines_out[i]=file_line;
					counter3_4--;
					//printf("\nconsumer_3 consumed :%s\n",file_lines[i]);	
                                }
				i++;
				
                         }
			pthread_mutex_unlock(&mutex3_4);
      			sem_post(&empty3_4);
			
			if(i==count)
			break;
	
		}

	       for(j=0;j<count;j++)
		{
			file_line_search=file_lines_out[j];
			if(strstr(file_line_search,word)!=NULL)
			{
				success_line[success_count]=file_line_search;
				success_count++;
				//printf("%s",file_line_search);
				
			}
		}
	      i=0;
		
		while(TRUE)
		{
			
			sem_wait(&empty4_5);
      			pthread_mutex_lock(&mutex4_5);

			if(i<success_count)
                	{		
				if(counter4_5 < BUFFER_SIZE) 
				{
      					buffer4_5[counter4_5] =success_line[i];
      					counter4_5++;
				}
				i++;
			}
			pthread_mutex_unlock(&mutex4_5);
      			sem_post(&full4_5);
			if(i==success_count)
			break;
		}
		
}

void *consume_4_produce_5(void *param)
{
		int i;
		char *success_line;                 
		while(TRUE) 
		{
			
      			sem_wait(&full4_5);
      			pthread_mutex_lock(&mutex4_5);
                      	if(i<success_count)
			{
	                        if(counter4_5 > 0)
				{
					printf("\n%s",buffer4_5[(counter4_5-1)]); 
					counter4_5--;
					
				}
				i++;
				
                         }
			pthread_mutex_unlock(&mutex4_5);
      			sem_post(&empty4_5);

			if(i==success_count)
			break;
		}
		
		

}

