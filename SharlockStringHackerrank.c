char* isValid(char* s) {
int freq[26]={0};
for(int i=0;i<strlen(s);i++){
    freq[s[i]-'a']++;
}
int count[100001] = {0};
for(int i=0;i<26;i++){
    if(freq[i]>0){
        count[freq[i]]++;
    }}
    int first=0;
    int second=0;
    for(int i=0;i<=strlen(s);i++){
        if(count[i]>0){
        if(first==0){
            first=i;
        }
        else if(second==0) second=i;
        else return "NO";
        }
        
    }
    if(second==0) return "YES";
     if (count[first] == 1 && first == 1)
        return "YES";

    if (count[second] == 1 && second == first + 1)
        return "YES";

    return "NO";

}
