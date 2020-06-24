
void int_to_str(char* str, int value){
    int temp = value % 10;
    int i = 0;
    while (temp != 0){  
        str[i] = '0' + temp;
        value = value / 10;
        temp = value % 10;
        i++;
    }
    str[i] = '\0';
    int j = 0;
    i--;
    while(j < i){
        char t = str[j];
        str[j] = str[i];
        str[i] = t;
        i--;
        j++;
    }
}
