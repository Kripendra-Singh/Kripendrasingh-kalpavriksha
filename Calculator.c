#include<stdio.h>
#include<string.h>
#include<ctype.h>
int max=100;


int isoperator(char ch){
    return ch=='+'||ch=='-'||ch=='*'||ch=='/';
}

 int isvalid(char exp[]){      //function for checking the validity of expression
    int i=0;
    int num=1;

    while(exp[i]!='\0'){
        char ch=exp[i];
        if(isspace(ch)){          //ignoring the spaces in the expression
            i++;
            continue;
        }
        if(num){
             if (ch=='-'){
                i++;
                while (isspace(exp[i])) i++; // Skip spaces after minus
                if (!isdigit(exp[i])&&exp[i]!='.') return 0;
                    
            }
            if(isdigit(exp[i])||ch=='.'){
                int digitcount=0;
                int decimalcount=0;
                while(isdigit(exp[i])||exp[i]=='.'){
                    if(isdigit(exp[i])) digitcount++;
                    if(exp[i]=='.') decimalcount++;

                    if(decimalcount>1) return 0; // if more than one decimal is present it is invalid
                    i++;
                }
                if(digitcount==0) return 0;    //if only decimal is present it is invalid 

                num=0;

            }
            else return 0;
        }
        else {
            if(isoperator(ch)){
                num=1;
                i++;
            }
            else return 0;
        }



    }
    if(num) return 0;    // if expression ends with opereater it is invalid 
    

    return 1;

}




int calculate(char exp[],int *error){     //function to evaluate the expression
    int result = 0;
    int term = 0;
    int number = 0;
    char op = '+';
    int i = 0;
    while (exp[i] != '\0'){
        if (isdigit(exp[i])){
            number = 0;
            while (isdigit(exp[i])){
                number=number*10+(exp[i]-'0');
                i++;
            }

            if (op=='+'){
                result+=term;
                term=number;
            }
            else if (op=='-'){
                result+=term;
                term=-number;
            }
            else if (op == '*'){
                term=term*number;
            }
            else if (op == '/'){
                if(number==0){//printf("error divide by zero\n");
                    *error=1;
                return 0;}
                term=term/number;
            }
        }
        else{
            op=exp[i];
            i++;
        }
    }

    result+=term;
return result;
}

int main() {

    printf("Calculator Program\n");

    char exp[100];
    int error=0;
    

    do
    {
        printf("Enter your expression or Enter Exit to close:  ");
       scanf(" %[^\n]", exp);

        // Converting input in lower case  to avoid case sensitivity of string
        for (int i=0;exp[i]!='\0';i++){
            exp[i]=tolower(exp[i]);
        }

       
        if (strcmp(exp, "exit") == 0)     // comparing input string with exit 
            break;
        error=0;
        printf("You have entered the following Expression:  \n  ");
        printf("%s\n", exp);

        int val=isvalid(exp);
        if(val){    
            int res=0;
            res=calculate(exp,&error);
            if(error==0){
            printf("Result of Expression is : %d \n",res);
            }
            else printf("error ocurred divide by zero\n");
        }
        else printf("not valid expression  \n");

    } while (1);

    printf("Calculator closed.\n");    // when while loop ends than claculator is closed 

    return 0;
}