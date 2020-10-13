#include "info_sys.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
FILE *fp = NULL;
struct sys_info 
{
    char m_cpu[100];
    char mem_total[100];
    char mem_free[100];
    char mem_available[100];
};
struct sys_info a;

void handle_signal()
{
	FILE *file;
	file = fopen("cpu.txt", "w");

	fputs(a.m_cpu, file);
	fclose(file);
}

void handle_read_memory()
{
	FILE *file;
	file = fopen("memory.txt", "w");
  	fputs (a.mem_total, file);
        fputs (a.mem_free, file);
        fputs (a.mem_available, file);
	fclose(file);
}


void handle_new()
{
	FILE *file;
        file = fopen("total.txt", "a");

        fputs(a.m_cpu, file);
  
        fputs (a.mem_total, file);
        fputs (a.mem_free, file);
        fputs (a.mem_available, file);
        fclose(file);

}

void read_sysinfo()
{
	pid_t pid;
	FILE *file;
	pid = fork();
	if (pid > 0) {
		while (1) {
                int returnStatus;
       		sleep(1);
		int check = kill(pid, SIGUSR1);

//		printf("Signal co thanh cong khong : %d\n", check);

//		printf("PID cua thang con may la : %d\n", pid);
//       		if (returnStatus == 0)  // Verify child process terminated without error.
//                {
//                       printf("The child process terminated normally second.\n");
//
//                }
//
//                if (returnStatus == 1)
//                {
//                        printf("The child process terminated with an error!.");
//                }
//
		}
        }

	printf("CPU after all: %s\n", a.m_cpu);

	if (pid == 0) {
		while (1) {
		signal(SIGUSR1, handle_new);
//		printf("Signal cua ban than may : %d\n", getpid());
		char x[100] = "/proc/cpuinfo";
    		fp = fopen(x, "r");
    		if(fp != NULL)
    		{
        		fseek( fp, 79, SEEK_SET );
        		fgets (a.m_cpu, 100, fp);
        		printf("CPU: %s", a.m_cpu);
        		fclose(fp);
		}    	
	  	char y[100] = "/proc/meminfo";
                fp = fopen(y, "r");
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
               // }

		}
		else
        		printf("Can't open file !");
		printf("Child process of cpu terminated.\n");
	 sleep(1);
	}
	}
	
}

void read_memory()
{
	pid_t pid;
        FILE *file;
        pid = fork();
	if (pid > 0) {
		while (1) {
                int returnStatus;
		sleep(1);
		int check = kill(pid, SIGUSR2);

		printf("Signal cuar read_memory co thanh cong khong : %d\n", check);

		printf("PID cua thang con may la : %d\n", pid);
       		if (returnStatus == 0)  // Verify child process terminated without error.
                {
                       printf("The child process terminated normally second.\n");

                }

                if (returnStatus == 1)
                {
                        printf("The child process terminated with an error!.");
                }

		}
        }

	if (pid == 0) {
		while (1) {
		signal(SIGUSR2, handle_read_memory);
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


		sleep(1);
		}
    }
    else
        printf("Can't open file !");
}

void display_sys()
{
    printf("Check co nhay vao day ko.\n");
	pthread_t cpu_info;
    pthread_t mem_info;
    read_sysinfo();
//    read_memory();

//    pthread_create(&cpu_info, NULL, read_cpu, NULL);
//    pthread_create(&mem_info, NULL, read_memory, NULL);
//    pthread_create(&cpu_info, NULL, read_cpu, NULL);

//    pthread_exit(NULL);
}


