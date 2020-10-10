#include "info_sys.h"

FILE *fp = NULL;
struct sys_info 
{
    char m_cpu[100];
    char mem_total[100];
    char mem_free[100];
    char mem_available[100];
};
struct sys_info a;

void read_cpu()
{
    char x[100] = "/proc/cpuinfo";
    fp = fopen(x, "r");
    if(fp != NULL)
    {
        fseek( fp, 79, SEEK_SET );
        fgets (a.m_cpu, 100, fp);
        printf("CPU: %s", a.m_cpu);
        fclose(fp);
    }
    else
        printf("Can't open file !");
}

void read_memory()
{
    char x[100] = "/proc/meminfo";
    fp = fopen(x, "r");
    if(fp != NULL)
    {
        fgets (a.mem_total, 100, fp);
        fseek( fp, 28, SEEK_SET );
        fgets (a.mem_free, 100, fp);
        fseek( fp, 56, SEEK_SET );
        fgets (a.mem_available, 100, fp);
        printf("%s", a.mem_total);
        printf("%s", a.mem_free);
        printf("%s", a.mem_available);
        fclose(fp);
    }
    else
        printf("Can't open file !");
}

void display_sys()
{
    read_cpu();
    read_memory();
}
