long arrayManipulation(int n, int queries_rows, int queries_columns, int** queries) {
    long *arr=calloc(2+n,sizeof(long));
    if(arr==NULL) return 0;
    for(int i=0;i<queries_rows;i++){
        int a=queries[i][0];
        int b=queries[i][1];
        long k=queries[i][2];
        arr[a]+=k;
        arr[b+1]-=k;
    }
    long max=0;
    long sum=0;
    for(int i=1;i<=n;i++){
        sum+=arr[i];
        if(sum>max)max=sum;
        
    }
    free(arr);
    return max;
    
    
    

}