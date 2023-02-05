#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <direct.h>
#include <unistd.h>

char* clipboard;
char* projpath;
char *substr(const char *str1,int st,int ed)
{
int i;
int stlen=ed-st+1 ;
    char *temp;
    temp = (char*) malloc(stlen*sizeof(char));
    for (i=0;i<ed-st+1;i++)
    {
       *(temp+i) = *(str1+i+st);
    }
    return temp;
}

char* checkpath(char* folder_st)
{
int k1 , k2;
int result,result2;
int folderlen=strlen(folder_st);

printf("\nProjpath%s\n",projpath);
chdir(projpath);
   char cwd[PATH_MAX];
 //  if (getcwd(cwd, sizeof(cwd)) != NULL) {
//printf("%s",getcwd(cwd,sizeof(cwd)));
char* temp;
temp = (char*) malloc(folderlen*sizeof(char));
k1=1;
for (int i=1;i<folderlen;i++)
{
    if (*(folder_st+i)=='/')
    {
        k2=i-1;
        temp = substr(folder_st,k1,k2);
        result = chdir(temp);

        if (result != 0)
        {
            perror("ERROR changing directory");
            result2 = mkdir(temp);
            if (result2 == 0)
            {
                printf("Directory %s created\n",temp);
                result = chdir(temp);
            }
            else
                printf("Could not create directory \n");
        }
        else
//            printf("Current directory now :%s\n",temp);
        k1=k2+2;
    }
}

    temp = substr(folder_st,k1,folderlen);
//printf("\npath: %s %d %d %s",folder_st,k1,folderlen,temp);
return temp;
}

void create_file(char *fname) {

char *filename ;
filename = checkpath(fname);
    FILE *file_creation = fopen(filename,"w");
    fclose(file_creation);
}
void scanning(char command5[100],char command6[10]) {
    int i = 0;
    while(1) {
        scanf("%c",&command5[i]);
        if(command5[i] == '-') {
            i++;
            scanf("%c",&command5[i]);
            if(command5[i] == 'p') {
                i++;
                scanf("%c",&command5[i]);
                if(command5[i] == 'o') {
                    i++;
                    scanf("%c",&command5[i]);
                    if(command5[i] == 's') {
                    command6[0] = command5[i-3];
                    command6[1] = command5[i-2];
                    command6[2] = command5[i-1];
                    command6[3] = command5[i];
                    command6[4] = NULL;
                    command5[i-4] = '\0';
                    return;}
                }
            }
        }
        i++;
    }

}
char *modify_string(char *target_string) {
    char* result;
    int j;
    result = (char *)malloc(100*sizeof(char));
    if(*(target_string + 0) != 34) {return target_string;}
    else {
        j=0;
        for (int i = 1; *(target_string + i + 1) != NULL;i++) {
            if(*(target_string+i) == 92)
            {
                if (*(target_string+i+1) == 'n')
                {
                    *(result + j) = '\n';
                    j++;
                    i++;
                }
                else
                    if(*(target_string+i+1) == 92)
                    {
                        *(result + j ) = *(target_string + i);
                        j++;
                      i++;
                    }
                    else
                    {
                        continue;
                    }
            }
            else
            {
                *(result + j ) = *(target_string + i);
                j++;
            }
        }
    }
    *(result + j ) = NULL;
    return result;
}

void insert (char *fname,char *target_string,int line_number,int starting_position) {
    char *filename;
    filename = checkpath(fname);
    FILE *file = fopen(filename,"r+");
    fseek(file,0,SEEK_SET);
    char *buffer = (char*)malloc(10000*sizeof(char));
    int counter = 1;
    int i = 0;
    while (counter != line_number) {
        char a = fgetc(file);
        if(a == '\n') counter++;
        i++;
    }
    char c;
    for(int j=0; j < starting_position ; j++){
        c = fgetc(file);
        i++;
    }
    int k=0;
    if (i>0){*buffer = c;k=1;}
    do {
        char c = fgetc(file);
        if (feof(file)) break;
        *(buffer+k) = c;
        k++;
    }  while(1);
    *(buffer+k) = '\0';
    fseek(file,i,SEEK_SET);
    target_string = modify_string(target_string);
//    printf("%s %s",target_string,buffer);
    fputs(target_string,file);
    fputs(buffer,file);
    printf("Successful\n");
    fclose(file);
}
void cat (char *fname) {
char *filename;
char buff[255];
filename = checkpath(fname);

    FILE *file = fopen(filename,"r");
    printf("\n");
    do {
        char c = fgetc(file);
        if (feof(file)) {printf("\n\n");break;}
        printf("%c", c);
    }  while(1);
    fclose(file);
}

void removestr (char *fname,int line_number,int starting_position,int rem_size,char *switcher) {
    char *filename;
    filename = checkpath(fname);
    FILE *file = fopen(filename,"r");
    fseek(file,0,SEEK_SET);
    char *buffer = (char*)malloc(10000*sizeof(char));
    int counter = 1;
    int i = 0;
    while (counter != line_number) {
        char a = fgetc(file);
        *(buffer+i) = a;
        if(a == '\n') counter++;
        i++;
    }
    for(int j=0; j < starting_position-1 ; j++){
        *(buffer+i) = fgetc(file);
        i++;
    }
    if(strcmp(switcher,"-b") == 0) {i = i-rem_size;}
    else if(strcmp(switcher,"-f") == 0){
        for(int k=0; k<rem_size;k++)
            fgetc(file);
    }
    do {
        char c = fgetc(file);
        if (feof(file)) break;
        *(buffer+i) = c;
        i++;
    }  while(1);
    *(buffer+i) = '\0';
    fclose(file);
    file = fopen(filename,"w");
    int buffsize = strlen(buffer);
    for (int k=0;k<buffsize;k++)
        fputc(*(buffer+k),file);
    fclose(file);
    printf("Successful\n");
}
void copystr (char *fname,int line_number,int starting_position,int cpy_size,char *switcher) {
    char *filename;
    filename = checkpath(fname);
    char cwd[PATH_MAX];
//printf("%s",getcwd(cwd,sizeof(cwd)));
    FILE *file = fopen(filename,"r+");
    if (file==NULL) {printf("ERROR open file");exit ;}
    fseek(file,0,SEEK_SET);
    int counter = 1;
    int i = 0;
    while (counter != line_number) {
        char a = fgetc(file);
        printf(" %c ",a);
        if(a == '\n') counter++;
        i++;
    }
    for(int j=0; j < starting_position ; j++){i++;}
    char* buff;
    buff = (char*)malloc(cpy_size*sizeof(char));
    if(strcmp(switcher,"-b") == 0)
        fseek(file,i-cpy_size,SEEK_SET);
    else if (strcmp(switcher,"-f") == 0)
            fseek(file,i,SEEK_SET);
       for(int k=0;k<cpy_size;k++)
       {
        char c = fgetc(file);
        if (feof(file)) break;
        *(buff+k) = c;
    }
    clipboard = (char*)malloc(cpy_size*sizeof(char));
    strcpy(clipboard,buff);

    printf("\nClipboard: %s \nBuff: %s\n",clipboard,buff);
    fclose(file);
}

void cutstr (char *fname,int line_number,int starting_position,int cut_size,char *switcher) {
    char* fname1 = strdup(fname);
    copystr(fname1,line_number,starting_position,cut_size,switcher);
//    printf("\nClipboard: %s \n",clipboard);
    removestr(fname,line_number,starting_position,cut_size,switcher);

}
void pastestr (char *fname,int line_number,int starting_position)
{
    insert(fname,clipboard,line_number,starting_position);
}
void findstr (char *finding_string,char *fname)
{
    char *filename;
    filename = checkpath(fname);
    FILE *file = fopen(filename,"r");
    fseek(file,0,SEEK_SET);
    char *buffer = (char*)malloc(255*sizeof(char));
    char *buff = (char*)malloc(255*sizeof(char));
    int pos=0;
    int len1,len2,found=0;

    do
    {
        fgets(buffer,255,file);
        len1=strlen(buffer);
        buff = strstr(buffer,finding_string);
        if (buff != NULL)
        {
            len2 = strlen(buff);
            pos += len1-len2;
            found = 1;
            break;
        }
        else pos +=len1;
//        printf("\nBUFF:%s %d \nBUFFER:%s %d \n",buff,len2,buffer,len1);
    }while (!feof(file));
    if (found==1) printf("\n%d\n",pos+1);
    else printf("\n-1\n");
    fclose(file);
}
int main()
{
    char command1[50],command2[50],command3[50];
    char command4[10],command5[100],command6[10];
    int  command7,command8,command9;


    char cwd[PATH_MAX];
    projpath = (char*)malloc(100);
    projpath = getcwd(cwd,sizeof(cwd));
//    printf("%s",projpath);
do
{
    scanf("%s",command1);
//    printf("\ncommand1:%s ",command1);

/*****************************************************************
********************* Create File ********************************
******************************************************************/
        if (strcmp(command1,"createfile")==0)
            {
                scanf("%s",command2);
                if (strcmp(command2,"-file")==0)
                {
                    scanf("%s",command3);
                    create_file(command3);
                }
                else printf("Invalid Command");
            }
/*****************************************************************
********************* Insert Str *********************************
******************************************************************/

        if (strcmp(command1,"insertstr")==0)
            {
                scanf("%s",command2);
                if (strcmp(command2,"-file")==0)
                {
                    scanf("%s %s ",command3,command4);
                    scanning(command5,command6);
//                    printf("999 %s %s\n",command5,command6);
//printf("%d",strcmp(command6,"-pos"));
                     if ((strcmp(command4,"-str")==0) && (strcmp(command6,"-pos")==0) )
                     {
                        scanf("%d:%d",&command7,&command8);
                        insert(command3,command5,command7,command8);
                     }
                     else printf("Invalid Command");
                }
                else printf("Invalid Command");
            }
/*****************************************************************
********************* Cat ****************************************
******************************************************************/

        if (strcmp(command1,"cat")==0)
            {
                scanf("%s",command2);
                if (strcmp(command2,"-file")==0)
                {
                    scanf("%s",command3);
                    cat(command3);
                }
                else printf("Invalid Command");
            }
/*****************************************************************
********************* Remove Str *********************************
******************************************************************/

        if (strcmp(command1,"removestr")==0)
            {
                scanf("%s",command2);
                if (strcmp(command2,"-file")==0)
                {
                    scanf("%s %s ",command3,command4);
                    scanf("%d:%d",&command7,&command8);
                    scanf("%s %d %s",command5,&command9,command6);

                     if ((strcmp(command4,"-pos")==0) && (strcmp(command5,"-size")==0) && ((strcmp(command6,"-b")==0) || (strcmp(command6,"-f")==0) ))
                     {
  //   printf("11111 %s %d %d %d %s",command3,command7,command8,command9,command6);
                        removestr(command3,command7,command8,command9,command6);
                     }
                     else printf("Invalid Command");
                }
                else printf("Invalid Command");
            }

/*****************************************************************
********************* Copy Str ***********************************
******************************************************************/

        if (strcmp(command1,"copystr")==0)
            {
                scanf("%s",command2);
                if (strcmp(command2,"-file")==0)
                {
                    scanf("%s %s ",command3,command4);
                    scanf("%d:%d",&command7,&command8);
                    scanf("%s %d %s",command5,&command9,command6);

                     if ((strcmp(command4,"-pos")==0) && (strcmp(command5,"-size")==0) && ((strcmp(command6,"-b")==0) || (strcmp(command6,"-f")==0) ))
                     {
//     printf("11111 %s %d %d %d %s",command3,command7,command8,command9,command6);
                        copystr(command3,command7,command8,command9,command6);
                     }
                     else printf("Invalid Command");
                }
                else printf("Invalid Command");
            }

/*****************************************************************
********************* Cut Str ***********************************
******************************************************************/

        if (strcmp(command1,"cutstr")==0)
            {
                scanf("%s",command2);
                if (strcmp(command2,"-file")==0)
                {
                    scanf("%s %s ",command3,command4);
                    scanf("%d:%d",&command7,&command8);
                    scanf("%s %d %s",command5,&command9,command6);

                     if ((strcmp(command4,"-pos")==0) && (strcmp(command5,"-size")==0) && ((strcmp(command6,"-b")==0) || (strcmp(command6,"-f")==0) ))
                     {
//     printf("11111 %s %d %d %d %s",command3,command7,command8,command9,command6);
                        cutstr(command3,command7,command8,command9,command6);
                     }
                     else printf("Invalid Command");
                }
                else printf("Invalid Command");
            }
/*****************************************************************
********************* Paste Str ***********************************
******************************************************************/

        if (strcmp(command1,"pastestr")==0)
            {
                scanf("%s",command2);
                if (strcmp(command2,"-file")==0)
                {
                    scanf("%s %s ",command3,command4);
                    scanf("%d:%d",&command7,&command8);

                     if ((strcmp(command4,"-pos")==0))
                     {
                        pastestr(command3,command7,command8);
                     }
                     else printf("Invalid Command");
                }
                else printf("Invalid Command");
            }
/*****************************************************************
********************* Find Str ***********************************
******************************************************************/
        if (strcmp(command1,"find")==0)
            {
                scanf("%s",command2);
                if (strcmp(command2,"-str")==0)
                {
                    scanf("%s %s %s",command3,command4,command5);
                   // scanf("%d:%d",&command7,&command8);

                     if ((strcmp(command4,"-file")==0))
                     {
                        findstr(command3,command5);
                     }
                     else printf("Invalid Command");
                }
                else printf("Invalid Command");
            }


/*****************************************************************
********************* exit Str ***********************************
******************************************************************/

        if (strcmp(command1,"exit")==0)
            {
                break;
            }

}while(1);

 return 0;
}
