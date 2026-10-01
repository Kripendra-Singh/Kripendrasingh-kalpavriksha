#include<stdio.h>
#include<string.h>
#include<ctype.h>

struct User{                            
    int id;
    char name[50];
    int age;
};

int idexists(int id){               //function for checking id exists or not
    FILE *fp;
    int ID;
    fp=fopen("users.txt","r");
    if(fp==NULL) return 0;
   while(fscanf(fp,"%d",&ID)==1){
    if(ID==id){                         
        fclose(fp);
        return 1;
   }
     while (fgetc(fp)!='\n');   //skip other columns

}
fclose(fp);
return 0;
}


void create(){
     struct User u;      
    
        while (1){
    printf("\nEnter User ID ");

    if (scanf("%d", &u.id) != 1){
        printf("Invalid ID! Please enter an integer\n");
        while (getchar()!='\n');
        continue;
    }
    if (idexists(u.id)){
        printf("id already exists Please enter another id\n");
        continue;
    }

    break;
}

    
    printf("Enter User Name \n");
    scanf(" %49[^\n]", u.name);        
    while (1){
        printf("Enter User Age\n ");
        if (scanf("%d",&u.age)==1&&(u.age>0&&u.age<=120)) break;   
        printf("Invalid Age Please enter a valid Age\n");
        while (getchar()!='\n');
    }

     FILE *fp = fopen("users.txt","a");

    if (fp == NULL){       
        printf("unable to open file\n");
        return;}

    fprintf(fp,"%d|%s|%d\n",u.id,u.name,u.age); 
    fclose(fp);
    printf("User created successfully\n");
} 


void view(){
    FILE *fp;
    char line[1000]; 
    fp=fopen("users.txt","r");
    if(fp==NULL){
        printf("file does not exists or could not be opened\n");
        return;
    }
    printf("\n***************User Records****************\n");
    while(fgets(line,sizeof(line),fp)!=NULL){
        printf("%s",line);
    }
    fclose(fp);
}


void update(){
    struct User u;
    int searchId;
    printf("\nEnter User ID to update\n ");
    if(scanf("%d",&searchId)!=1){
        printf("\nNot valid user id \n");
         while(getchar()!='\n');
        return;
    }
    if(!idexists(searchId)){                   
        printf("User ID does not exist\n");
         while(getchar()!='\n');
        return;
    }

    printf("User ID found!\n");

    FILE *fp=fopen("users.txt","r");   
     FILE *temp=fopen("temp.txt","w");            

    if(fp==NULL||temp==NULL){
        printf("unable to open file\n");
        if(fp!=NULL) fclose(fp);
        if(temp!=NULL) fclose(temp);
        return;
    }
   while (fscanf(fp, "%d|%49[^|]|%d", &u.id, u.name, &u.age)==3){
        
        if(u.id==searchId){
            while (getchar() != '\n');
            printf("Enter new name\n ");
          scanf(" %[^\n]", u.name);
            while(1){
        printf("Enter New Age \n");

        if(scanf("%d",&u.age)==1&&(u.age>0&&u.age<=120)) break;  
        
        printf("Invalid Age Please enter a valid Age\n");
        while(getchar()!='\n');
    }
        }
        fprintf(temp,"%d|%s|%d\n",u.id,u.name,u.age);
        
    }

    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt","users.txt");
    printf("User updated successfully\n");
}
void Delete(){
    int searchId;
    struct User u;
    printf("\nEnter User ID to delete\n ");
    if(scanf("%d",&searchId)!=1){
        printf("id does not exists\n");
         while(getchar()!='\n');
         return;
    }
    if(!idexists(searchId)){
        printf("User ID does not exist\n");
         while(getchar()!='\n');
        return;
    }
    FILE *fp=fopen("users.txt","r");
     FILE *temp=fopen("temp.txt","w");
    if(fp==NULL||temp==NULL){
        printf("Unale to open file\n");

        if(fp!=NULL) fclose(fp);

        if(temp!=NULL) fclose(temp);

        return;
    }

                                                                
    while(fscanf(fp,"%d|%49[^|]|%d",&u.id,u.name,&u.age)==3){
        if(u.id!=searchId){
            fprintf(temp,"%d|%s|%d\n",u.id,u.name,u.age);}
    }
    fclose(fp);
    fclose(temp);
    remove("users.txt");
    rename("temp.txt","users.txt");
    printf("User deleted successfully\n");
}
int main(){
     int exit=1;

    do{
        int choice;
       
        printf("File Handling Operations \n");
        printf("Enter 1 to Create a User \n");
        printf("Enter 2 to View File \n");
        printf("Enter 3 to Update a User \n");
        printf("Enter 4 to Delete a User \n");
        printf("Enter 0 to Exit \n");
       if(scanf("%d",&choice)!=1) {
    printf("Invalid input! Please enter a number\n");

    while(getchar()!='\n');
    continue;
}
        
        switch (choice)
        {
        case 1:
            create();
            break;
        case 2:
        view();
            break;

        case 3:
        update();
            break;

        case 4:
        Delete();
            break;

        case 0:
            printf("Closing.... \n");
            exit=0;
            break;
        
        default:
         printf("Please Select a valid option \n");
            break;
        }
    }
    while(exit);
    
    printf("programe Closed");
    
}