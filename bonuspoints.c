#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

char *file_name[5000];

char *directory_names[50];
char temp_name[150];
char *filename;
int i=0;
int k=0;
int count=0;

void listdir(char* dirname,int level)
{
DIR* dp;
struct dirent *de;
char longest_name[4096];

int j=0;
if ((dp = opendir(dirname)) == NULL)
{
fprintf(stderr, "Couldn't open directory: %s\n", dirname);
exit(-1);
}
while((de=readdir(dp)) != NULL) 
{

if(strncmp(de->d_name, "..", 2) != 0 &&strncmp(de->d_name, ".", 1) != 0) 
{

if (de->d_type == DT_DIR)
{
strcpy(longest_name, dirname);
strcat(longest_name, "/");
strcat(longest_name, de->d_name);
directory_names[i]=de->d_name;
j=i;
i++;
listdir(longest_name,level+1);

}

else 
{
if(level>1)
{

strcpy(temp_name,directory_names[0]);
strcat(temp_name,"//");
for(k=1;k<level;k++)	
{
strcat(temp_name,directory_names[k]);	
strcat(temp_name,"//"); 
}
strcat(temp_name,de->d_name);
filename=(char *)malloc(200);
strcpy(filename,temp_name);
file_name[count]=filename;
count++;

} 
else
{

file_name[count]=de->d_name;
count++;
}

}
}
}
closedir(dp);
return;

}

int main(int argc, char** argv)
{
listdir(".",0);
for(i=0;i<count;i++)
printf("%s\n",file_name[i]);
return 0;
}
