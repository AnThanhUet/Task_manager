#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

FILE *fp = NULL;

struct rootDir
{
    char shortName[9];
    char shortExtension[4];
    uint8_t attributes;
    int sizeOffile;
    uint16_t timeCre;
    uint16_t dateCre;
};

int displayFiles(struct rootDir files)
{
    printf("%-8s\t",files.shortName);                                                                        // Short name
    printf("%-5s\t",files.shortExtension);                                                                    // Extension
    printf("%-6d\t",(files.attributes>>4)&1);                                                                // SubDir mask
    if(!((files.attributes>>4)&1))                                                                            // Check: Subdirectory or file
    {
        printf("%d-%.2d-%d\t", (files.dateCre>>9)+1980, (files.dateCre&0x01E0)>>5, (files.dateCre&0x001F));    // Created date (year-month-day)
        printf("%d:%d:%d", files.timeCre>>11, (files.timeCre&0x07E0)>>5, 2*(files.timeCre&0x001F));         // Created time (Hour:min:sec)
    }
    printf("\n");
}

void main(int argc, char **argv) {
    int pid;
    sscanf(argv[1], "%d", &pid);
    printf("pid = %d\n", pid);

    char filename[1000];
    sprintf(filename, "/proc/%d/stat", pid);
    FILE *f = fopen(filename, "r");

    int unused;
    char comm[1000];
    char state;
    int ppid;
    fscanf(f, "%d %s %c %d", &unused, comm, &state, &ppid);
    printf("comm = %s\n", comm);
    printf("state = %c\n", state);
    printf("parent pid = %d\n", ppid);
    fclose(f);
}
