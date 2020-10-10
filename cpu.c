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
    char x[100] = "/proc/cpuinfo";
    fp = fopen(x, "r");
    if(fp != NULL)
    {
        fseek( fp, 79, SEEK_SET );
        fgets (a.m_cpu, 100, fp);
        printf("Name: %s", a.m_cpu);
        fclose(fp);
    }
    else
        printf("Can't open file !");
}


int main() 
{

    read_cpu();
    return 0;
}
