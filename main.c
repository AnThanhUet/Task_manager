#include "./info_sys/info_sys.h"
#include "./test/test_proc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
int main(int argc, char **argv)
{
	int sel;
	sscanf(argv[1], "%d", &sel);
	printf("Select option: %d\n", sel);

	if(sel == 1)
	{
		display_sys();
	}
	
	else if (sel == 2)
	{
		pthread_t thread_read_file;
		pthread_t thread_signal;

		pthread_create(&thread_read_file, NULL, read_data, NULL);
		pthread_create(&thread_signal, NULL, get_signal, NULL);

		pthread_exit(NULL);
	}

	return 0;
	
}
