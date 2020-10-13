#define filename	"/dev/procid"

#define GHz			1 << 30
#define RAM			1 << 3
#define physical_memory		48
#define conv_jiff		1e7

typedef unsigned long uu64;
typedef struct prev_task_info {
        char                    comm[16];
        long                    state;
        int                     pid;

        uu64                    utime;
        uu64                    stime;

        uu64                    cutime;
        uu64                    cstime;

        uu64                    start_time;
        uu64                    total_time;

        uu64                    vma_start;
        uu64                    vma_end;
        uu64                    vma_size;
}prev_task_info;

uu64 cpu_total_time;

int open_file();
void *read_file(void *argp);
void *read_data(void *argp);
void *get_signal(void *argp);


