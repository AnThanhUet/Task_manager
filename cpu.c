#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

FILE *fp = NULL;
struct cpu 
{
    char m_cpu[100];
};
struct cpu a;


void read_cpu()
{
    
    fp = fopen("text.txt", "r");
    if(fp != NULL)
    {
        fscanf(fp, "\n\n\n%s", &a.m_cpu);
        printf("%s", a.m_cpu);
        fclose(fp);
    }
    else
        printf("Can't open file !");
}


int main() 
{
    read_cpu;
    return 0;
}