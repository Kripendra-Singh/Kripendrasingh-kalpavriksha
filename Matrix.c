#include<stdio.h>
#include<stdlib.h>

# define MAX 10
# define MIN 2
int get_input(){
    int n=0;
    printf("enter size of Matrix\n");
    while (1)
    {
        if(scanf("%d",&n)==1&&(n>=MIN &&n<=MAX)) return n;
        printf("enter a valid number between 2 to 10 \n");
        int ch;
        while(getchar()!='\n'&& ch!=EOF);
    }
    

}
void generateMatrix(int n, int a[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i][j] = rand() % 256;
        }
    }
}
void display(int n ,int a[n][n]){
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            printf("%4d", a[i][j]);
        }
        printf("\n");
    }

}
void Transpose(int n,int a[n][n]){
    
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            int *p=&a[i][j];
            int *q=&a[j][i];
            int temp=*p;
            *p=*q;
            *q=temp;

        }
    }
}
void reverse(int n,int a[n][n]){
    Transpose(n,a);
    for(int i=0;i<n;i++){
        for(int j=0;j<n/2;j++){
             int *p=&a[i][j];
            int *q=&a[i][n-1-j];
            int temp=*p;
            *p=*q;
            *q=temp;
        }

    }

}

void smoothing(int n, int a[n][n]) {

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0, count = 0;

            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    int r = i + x;
                    int c = j + y;

                    if (r >= 0 && r < n &&
                        c >= 0 && c < n) {
                        int *p = &a[r][c];
                        sum += (*p & 255);
                        count++;
                    }
                }
            }

            int *p = &a[i][j];
            *p = (*p & 255) | ((sum / count) << 8);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int *p = &a[i][j];
            *p = *p >> 8;
        }
    }
}

int main(){
    int n=get_input();
    int a[n][n];
    generateMatrix(n,a);
    display(n,a);
    printf("reverse of matrix \n");
    reverse(n,a);
    display(n,a);
    printf("Matix after smoothening \n");
    smoothing(n,a);
    display(n,a);
}

