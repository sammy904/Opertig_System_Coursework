#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char username[]="admin";
char password[]="1234";

void login();
void createFile();
void writeFile();
void readFile();
void deleteFile();
void filePermission();
void encryptFile();
void decryptFile();
void auditLog(char action[]);

int main()
{
    int choice;
    login();

    while(1)
    {
        printf("\n===== FILE SYSTEM MENU =====\n");
        printf("1. Create File\n");
        printf("2. Write File\n");
        printf("3. Read File\n");
        printf("4. Delete File\n");
        printf("5. File Permission\n");
        printf("6. Encrypt File\n");
        printf("7. Decrypt File\n");
        printf("8. Exit\n");

        printf("Enter Choice: ");
        scanf("%d",&choice);

        switch(choice)
        {
            case 1: createFile(); break;
            case 2: writeFile(); break;
            case 3: readFile(); break;
            case 4: deleteFile(); break;
            case 5: filePermission(); break;
            case 6: encryptFile(); break;
            case 7: decryptFile(); break;
            case 8: exit(0);
            default: printf("Invalid Choice\n");
        }
    }
}

void auditLog(char action[]){
    FILE *log=fopen("audit_log.txt","a");
    if(log){ fprintf(log,"%s\n",action); fclose(log); }
}

void login(){
    char user[30],pass[30];
    printf("Username: "); scanf("%29s",user);
    printf("Password: "); scanf("%29s",pass);
    if(strcmp(user,username)==0 && strcmp(pass,password)==0)
        printf("\nLogin Successful!\n");
    else{ printf("\nLogin Failed!\n"); exit(0); }
}

void createFile(){
    char filename[50];
    printf("Enter File Name: "); scanf("%49s",filename);
    FILE *fp=fopen(filename,"w");
    if(!fp){ printf("Cannot Create File\n"); return; }
    fclose(fp);
    printf("File Created Successfully.\n");
    auditLog("Created File");
}

void writeFile(){
    char filename[50],data[200];
    printf("Enter File Name: "); scanf("%49s",filename);
    FILE *fp=fopen(filename,"a");
    if(!fp){ printf("File Not Found\n"); return; }
    getchar();
    printf("Enter Data: ");
    fgets(data,sizeof(data),stdin);
    fprintf(fp,"%s",data);
    fclose(fp);
    printf("Data Written Successfully.\n");
    auditLog("Wrote to File");
}

void readFile(){
    char filename[50];
    int ch;
    printf("Enter File Name: "); scanf("%49s",filename);
    FILE *fp=fopen(filename,"r");
    if(!fp){ printf("File Not Found\n"); return; }
    printf("\n----- FILE CONTENT -----\n");
    while((ch=fgetc(fp))!=EOF) putchar(ch);
    fclose(fp);
    auditLog("Read File");
}

void deleteFile(){
    char filename[50];
    printf("Enter File Name: "); scanf("%49s",filename);
    if(remove(filename)==0){
        printf("File Deleted Successfully.\n");
        auditLog("Deleted File");
    }else printf("Unable to Delete File.\n");
}

void filePermission(){
    char pass[20];
    printf("Enter Admin Password: ");
    scanf("%19s",pass);
    if(strcmp(pass,"admin123")==0){
        printf("Permission Granted.\n");
        auditLog("Permission Granted");
    }else{
        printf("Permission Denied.\n");
        auditLog("Permission Denied");
    }
}

void encryptFile(){
    char filename[50];
    int ch;
    printf("Enter File Name: "); scanf("%49s",filename);
    FILE *fp=fopen(filename,"r");
    if(!fp){ printf("File Not Found\n"); return; }
    FILE *enc=fopen("encrypted.txt","w");
    while((ch=fgetc(fp))!=EOF) fputc(ch+3,enc);
    fclose(fp); fclose(enc);
    printf("File Encrypted Successfully.\n");
    auditLog("Encrypted File");
}

void decryptFile(){
    int ch;
    FILE *fp=fopen("encrypted.txt","r");
    if(!fp){ printf("Encrypted File Missing\n"); return; }
    FILE *dec=fopen("decrypted.txt","w");
    while((ch=fgetc(fp))!=EOF) fputc(ch-3,dec);
    fclose(fp); fclose(dec);
    printf("File Decrypted Successfully.\n");
    auditLog("Decrypted File");
}
