
/*Define for both kernel and user space*/
typedef	unsigned long uu64;


typedef struct task_info {
	char			comm[16];
        long			state;
        int			pid;

	uu64			utime;
	uu64			stime;

	uu64			cutime;
	uu64			cstime;

	uu64			start_time;
	uu64			total_time;

       	uu64			vma_start;
	uu64			vma_end;
	uu64			vma_size;
	
	uu64			text_start;
	uu64			text_end;

	uu64                    data_start;
        uu64                    data_end;

	uu64                    bss_start;
        uu64                    bss_end;

	uu64                    heap_start;
        uu64                    heap_end;

	uu64                 	stack_start;
        uu64                    stack_end;


}task_info;


#define MAJIC_NO			100
#define IOCTL_NUMBER			_IOR(MAJIC_NO, 5, int)
#define IOCTL_PROCESS			_IOR(MAJIC_NO, 10, task_info)
