
    
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
FILE *fp=NULL;


struct rootDir
{
    char shortName[9];
    char shortExtension[4];
    uint8_t attributes;
    int sizeOffile;
    uint16_t timeCre;
    uint16_t dateCre;
};


int saveDataToStruct(uint8_t *entryBuff, struct rootDir *files)
{
    int i;
    for(i=0;i<8;i++)
            files->shortName[i]=*(entryBuff+i);                                // Save 8 byte char to short name
    files->shortName[8]='\0';                                                // End of string = '\0'
            
    for(i=0;i<3;i++)
            files->shortExtension[i]=*(entryBuff+i+0x08);                    // Save 3 byte char to short extension
    files->shortExtension[3]='\0';                                            // End of string = '\0'
            
    files->attributes=*(entryBuff+0x0B);                                    // Mask of attributes = 0x0B
    files->timeCre=*(entryBuff+0x0E) | (*(entryBuff+0x0F) << 8);
    files->dateCre=*(entryBuff+0x10) | (*(entryBuff+0x11) << 8);
}

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


int exportRootDir(int rootDirOffset)
{
    struct rootDir files;
    fseek(fp,rootDirOffset,0);
    uint8_t *entryBuff=(uint8_t *)malloc(32);                                // 32 byte Buffer
    
    printf("%-8s\t%-5s\t%-6s\t%-7s\n","Name","Exten","SubDir","Created");
    
    fread(entryBuff,1,32,fp);                                                // Read 1 entry
    while(*entryBuff!=0)                                                    // Check end marker of a directory table
    {
        if(*(entryBuff+0x0B)!=0x0F)                                            // Not a long file name
        {
            saveDataToStruct(entryBuff,&files);                                // Save data from entry buffer to struct 
            displayFiles(files);                                            // Display struc
        }
        else;
        fread(entryBuff,1,32,fp);
    }
}


int main()
{
    int rootDirOffset = 512*19;
    int sizeOfSector = 512;
    int maxNumRootEntry = 240;
    int sizeOfEntry = 32;
    int sizeOfRootDir = maxNumRootEntry*sizeOfEntry;
    
    uint8_t *secRootBuff;
    secRootBuff=(uint8_t *)malloc(maxNumRootEntry*sizeOfEntry);
    fp=fopen("C:\\Users\\AnThanh\\Desktop\\HN20_FRF_EMB_01\\Fresher_2020_04\\floppy.img","r+b");//C:\\Users\\AnThanh\\Desktop\\HN20_FRF_EMB_01\\Fresher_2020_04
    if(fp!=NULL)
    {
        exportRootDir(rootDirOffset);
        fclose(fp);
    }
    else
        printf("Can't open file!!!");
    return 0;
}
 








