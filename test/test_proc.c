#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <math.h>
#include <pthread.h>
#include <ctype.h>
#include <signal.h>

#include "../kernel_modules/task-list.h"
#include "test_proc.h"


struct task_info *test_info;
struct task_info *test_write;
int count = 0;
int check_first_time = 1;

int open_file ()
{
	int fd = open(filename, O_RDWR);
	if (fd < 0) {
		printf("Error when opening file\n");
		return -1;
	}

	return fd;
}

void *read_data(void *argp)
{
	int num_byte, fd, prev_count;
        int i = 0;
//	static int check_first_time = 1;
        double mem_per;
	unsigned long total_time_diff, cpu_total_time, prev_cpu_total_time;
        unsigned long total_mem = (1UL << physical_memory);
	struct task_info *prev_task;

	unsigned long prev_info[33000];

	fd = open_file();
        if (fd < 0) {
                perror("open failed\n");
                return NULL;
        }

	while (1) {
		if (ioctl (fd, IOCTL_NUMBER, &count) < 0) {
			return NULL;
		}

		test_info = (struct task_info *)malloc(count * sizeof(struct task_info));
		prev_task = (struct task_info *)malloc(count * sizeof(struct task_info));
		if ((num_byte = read(fd, test_info, count * sizeof(struct task_info))) < 0) {
			perror("read failed\n"); 
			return NULL;
		}

		FILE *fstat = fopen("/proc/stat", "r");
   		if (fstat == NULL) {
        		perror("FOPEN ERROR ");
        		fclose(fstat);
        		return NULL;
    		}

		unsigned long cpu_time[10];
    		bzero(cpu_time, sizeof(cpu_time));
    		if (fscanf(fstat, "%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
                		&cpu_time[0], &cpu_time[1], &cpu_time[2], &cpu_time[3],
                		&cpu_time[4], &cpu_time[5], &cpu_time[6], &cpu_time[7],
                		&cpu_time[8], &cpu_time[9]) == EOF) {
        		fclose(fstat);
        		return NULL;
    		}

    		fclose(fstat);

    		for(i = 0; i < 10; i++) {
        		cpu_total_time += cpu_time[i];
		}
		
		printf("Gia tri cpu_total_time: %lu\n", cpu_total_time);


		for (i = 0; i < count; i++) {
			if (test_info[i].vma_size / total_mem > 1) {
				mem_per = 100;	
			} else if (test_info[i].vma_size == 0) {
				mem_per = 0;
			} else {
				mem_per = (double)(test_info[i].vma_size) / (double)(total_mem) * 100; 
			}
			
			if (check_first_time == 1) {
				printf("Process : %s\t, PID: %d\t, stack_start :%lx\t, stack_end: %lx\t, heap_start: %lx\t, heap_end:%lx\n"
                                , test_info[i].comm, test_info[i].pid,test_info[i].stack_start
                                , test_info[i].stack_end, test_info[i].heap_start, test_info[i].heap_end); 
				prev_task[i].total_time = test_info[i].total_time;
				prev_info[i] = test_info[i].total_time;
					prev_cpu_total_time = cpu_total_time;
				continue;
//				printf("Gia tri diff : %lu\n", total_time_diff);
			}
			total_time_diff = cpu_total_time - prev_cpu_total_time;
			printf("Process : %s\t, PID: %d\t, stack_start :%lx\t, stack_end: %lx\t, heap_start: %lx\t, heap_end:%lx\n"
                                , test_info[i].comm, test_info[i].pid,test_info[i].stack_start
                                , test_info[i].stack_end, test_info[i].heap_start, test_info[i].heap_end); 

//			printf("Prev: %lu,   Cur: %lu\n", prev_info[i], test_info[i].total_time);	
//			printf("Time_diff: %lu\n", total_time_diff);				
			
			unsigned long minus = 100 * (test_info[i].total_time - prev_info[i]);
					
			double usage = (double )minus / total_time_diff; 
			usage /= 10;
			printf("user_usage: %.5f\n", (usage));

			prev_task[i].total_time = test_info[i].total_time;
			prev_info[i] = test_info[i].total_time;
		}
		prev_cpu_total_time = cpu_total_time;

		check_first_time++;
		printf("First_time: %d\n", check_first_time);
		prev_count = count;	
		free(test_info);
		
		sleep(10);
			
	}
	close(fd);
	return NULL;
}

void *get_signal (void *argp)
{
	char get_pid[8];
	int i, pid;
	while (1) {
		fgets(get_pid, 8, stdin);
		for (i = 0; i < 8; i++) {
			if (!isdigit(get_pid[i]))
				break;
		}
		pid = atoi(get_pid);
		kill(pid, SIGKILL);
                printf("Hello world\n");
                sleep(1);
        }
	return NULL;
}

//int main()
//{
//	pthread_t thread_read_file;
//	pthread_t thread_signal;	
//
//	pthread_create(&thread_read_file, NULL, read_data, NULL);
//	
//	pthread_create(&thread_signal, NULL, get_signal, NULL);
//
//	pthread_exit(NULL);
//	return 0;
//}

