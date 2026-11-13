#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
//Complete the following function.


void calculate_the_maximum(int n, int k) {
  //Write your code here.
  int result_or=0;
  int result_and=0;
  int result_xor=0;
  for(int i=1;i<=n;i++){
    for(int j=i+1;j<=n;j++){
       int temp_or=i|j;
       int temp_and=i&j;
       int temp_xor=i^j;
       if(temp_or<k&&temp_or>result_or) result_or=temp_or;
        if(temp_and<k&&temp_and>result_and) result_and=temp_and;
         if(temp_xor<k&&temp_xor>result_xor) result_xor=temp_xor;
    }
  }
  printf("%d\n%d\n%d\n",result_and,result_or,result_xor);
  
}

int main() {
    int n, k;
  
    scanf("%d %d", &n, &k);
    calculate_the_maximum(n, k);
 
    return 0;
}
