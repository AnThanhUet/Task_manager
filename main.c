#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	long rss = 0L;
	FILE* fp = NULL;
	int mem_size = 0, pid = 0;
	char path[100]; 

	puts("enter pid ");
	scanf("%d", &pid);
	sprintf(path, "/proc/%d/statm", pid);

	if ( (fp = fopen( path, "r" )) == NULL )
		return (size_t)0L;		/* Can't open? */
	if ( fscanf( fp, "%*s%ld", &rss ) != 1 )
	{
		fclose( fp );

		return (size_t)0L;		/* Can't read? */
	}
	fclose( fp );

	mem_size =  (size_t)rss ;//* (size_t)sysconf( _SC_PAGESIZE);
	printf("memsize is %d\n", mem_size);

	while(1)
		sleep(100);

	return 0;
}
