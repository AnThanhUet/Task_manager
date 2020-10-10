#include <stdio.h>

FILE *fp = NULL;

int root_information(int *num,int *ptr,char *name);
void swap(int *a, int *b);
int insert(char *name, int *ptr, int n, int l);
void bubble_sort(int n, int *ptr, char *name);

int main()
{
    int arr[200];
    char name[100];
    int n;
	root_information(&n,arr,name);
	bubble_sort(n, arr, name);
	return 0;
}


int root_information(int *num,int *ptr,char *name)
{
	/* Nhap thong tin  */
    int i,j;
    do
    {
        printf("moi ban nhap so phan tu mang:");
        scanf("%d",num);
    }while((0>=*num)||(*num>10));

    for(i=0;i<*num;i++)
    {
        printf("moi ban nhap phan tu thu [%d]: ",i);
        scanf("%d",ptr+i);
    }
    
    /* Nhap ten file */
    printf("moi ban nhap ten file de luu:");
    fflush(stdin);
    scanf("%s",name);
    
    /* Mo file va ghi data */
    fp=fopen(name,"w+");
    if(fp!=NULL)
    {
    	fprintf(fp,"Array: ");
        for(i=0;i<*num;i++)
        {
            fprintf(fp,"%d ",*(ptr+i));
        }
        fprintf(fp,"\n");
        fclose(fp);
    }
    return 0;
}

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int insert(char *name, int *ptr, int n, int l)
{  
    int i;
    fp = fopen(name, "a");
        if(fp != NULL)
        {
            fprintf(fp,"%d.     ",l+1);
            for (i=0; i < n; i++)
            {
                fprintf(fp,"%d ",*(ptr+i));             
            }    
            fprintf(fp,"\n");
        }
    fclose(fp);
    return 0;
}

void bubble_sort(int n, int *ptr, char *name)
{   
    int i, j;
    for (i = 0; i < n-1; i++)
    {   
        for (j = 0; j < n-i-1; j++){
            if (ptr[j] > ptr[j+1]){
                swap(&ptr[j], &ptr[j+1]); 
            }
        }  
        insert(name, ptr, n,i);
    }
	printf("---------------DONE !------------\n");    
}



