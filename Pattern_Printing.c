#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() 
{

    int n;
    scanf("%d", &n);
  	// Complete the code to print the pattern.
    int size=2*n-1;
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            int mini=i;
            if(j<mini) mini=j;
            if(size-1-i<mini) mini=size-1-i;
            if(size-1-j<mini) mini=size-1-j;
            printf("%d ",n-mini);
        }
        printf("\n");
    }
    return 0;
}
