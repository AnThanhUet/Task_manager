
typedef struct task_info {
	char comm[16];
        long state;
        int pid;

//        struct task_info *next;
}task_info;


#define MAJIC_NO			100
#define IOCTL_NUMBER			_IOR(MAJIC_NO, 5, int)
#define IOCTL_PROCESS			_IOR(MAJIC_NO, 10, task_info)
