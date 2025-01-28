#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#define MAX_LENGTH 50
/*
@brief tablo boyutunu hesaplar
@param fileName dosya adi
@param tableSize tablo boyutu
@return tablo boyutu
*/
int calculateTableSize(char *fileName, int *numberOfAllVariable){
    FILE *fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("File not found\n");
        return -1;
    }
    printf("%d",'1'-'A');
    int tableSize = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, " ,;\n"); //kelimeleri ayir     
        if (token != NULL) {
            if (strcmp(token, "char") == 0 || strcmp(token, "int") == 0 || strcmp(token, "float") == 0) { //eger char int veya float varsa degisken sayisini bul
                token = strtok(NULL, " ,;\n");
				while(token != NULL){
                    if (token[0] == '_') {
                        tableSize++;
                    }
                    token = strtok(NULL, " ,;\n");
                }
                
            }
            
        }
    }
    fclose(fp);
    *numberOfAllVariable=tableSize;
    //degisken sayisinin 2 katindan buyuk en kucuk asal sayiyi bul
    tableSize = tableSize * 2;
    int kok;
    kok=sqrt(tableSize);
    int i=2;
    while(i<=kok){
        if(tableSize%i==0){//asal sayi olup olmadigini anlamak icin kokune kadar inmek yeterli
            tableSize++;
            i=2;
        }
        else{
            i++;
        }
    }
    // for(i=2; i<=kok; i++){
    //     if(tableSize%i==0){
    //         tableSize++;
    //         i=2;
    //     }
    // }
    return tableSize;
}
/*
    @brief horner fonksiyonuna gore degiskenin hashe verilece degerini hesaplar
    @param name degisken adi
    @return degiskenin hashe verilecek degeri
*/
int horner(char*name){
    int sum=0,i;
    int length=strlen(name);
    for(i=0; i<length; i++){
        sum+=pow(31,length-1-i)*(name[i]-'A');//generally use 31
    }
    return sum;
}
/*
    @brief h1 in hesaplanmasi
    @param key degiskenin hashe verilecek degeri
    @param prime tablo boyutu ve kullanilan asal sayi
    @return value%prime
*/
int h1(int key, int prime){   
    return key%prime;
}
/*
    @brief h2 in hesaplanmasi
    @param key degiskenin hashe verilecek degeri
    @param prime tablo boyutu ve kullanilan asal sayi
    @return 1+ h1(key,prime-3)
*/
int h2(int key, int prime){
    return 1+ h1(key,prime-3);
}
/*
    @brief hash fonksiyonu
    @param key degiskenin hashe verilecek degeri
    @param i h2 nin katsayisi
    @param prime tablo boyutu ve kullanilan asal sayi
    @return (h1(key,prime)+(i*h2(key,prime)))%prime
*/
int hash(int key, int i, int prime){
    int result=(h1(key,prime)+(i*h2(key,prime)))%prime;
    return result;
}
/*
    @brief degiskenin tabloda olup olmadigini kontrol eder
    @param name degisken adi
    @param table sembol tablosu
    @param tableSize sembol tablosunun boyutu
    @param hashResult hash sonucu
    @param firstHash hashin ilk sonucu
    @param key degiskenin hashe verilecek degeri
    @param i h2 nin katsayisi
    @param sayi degiskenin hashe verilecek degerinin string hali
    @return 0 deger degisken tabloda yok, 1 deger degisken tabloda var
*/
int lookup(char *name, char***table, int tableSize, int *hashResult, int *firstHash){
    int i=0;
    char sayi[MAX_LENGTH];
    int key= horner(name);
    sprintf(sayi, "%d", key);
    *hashResult=hash(key,i,tableSize); 
    *firstHash=*hashResult;
    while(i<tableSize && strcmp(table[*hashResult][1], "null")!=0){ //tabloda hashResulttaki yer bos olana kadar don 
        if(strcmp(table[*hashResult][1], sayi)==0){//eger degisken tabloda varsa deklere edilmis
            return 1;
        }  
        else{//eger hashResulttaki yer dolu ve keye esitse bir sonraki bos yeri bul
            i++;
            *hashResult=hash(key,i,tableSize);           
        }
        
    }
    if(strcmp(table[*hashResult][1], "null")==0){ //eger hashResultaki yer bossa deklere edilmemis
        return 0;
    }
}
/*
    @brief degiskeni tabloya ekler
    @param name degisken adi
    @param type degiskenin türü
    @param table sembol tablosu
    @param tableSize sembol tablosunun boyutu
    @param hashResult hash sonucu
    @param firstHash hashin ilk sonucu
    @param key degiskenin hashe verilecek degeri
    @param sayi degiskenin hashe verilecek degerinin string hali
    @return 0 deger degisken tabloda var eklenmedi, 1 deger degisken tabloda yok eklendi
*/
int insert(char *name, char *type, char***table, int tableSize, int *hashResult, int *firstHash){
    int key= horner(name);
    char sayi[MAX_LENGTH];
    sprintf(sayi, "%d", key);
    if(lookup(name, table, tableSize, hashResult, firstHash)==0){//eklenecek degisken tabloda var mi yok mu kontrol et ve yoksa ekle
        strcpy(table[*hashResult][0],type);
        strcpy(table[*hashResult][1],sayi);
        //Tabloya yerlestirildi
        return 1;
    }else{
        //Tabloda var
        return 0;
    }
    
}
/*
    @brief sembol tablosuna yerlesenlerin bilgilerini tutan struct
    @param name degisken adi
    @param first degiskenin yerlesme adresinin ilk degeri
    @param last degiskenin yerlesme adresinin son degeri
*/
struct YERLESEN{
    char name[MAX_LENGTH];
    int first;
    int last;
};
int main(){
    char fileName[MAX_LENGTH];
    int choice, numberOfVariable=0;
    printf("1.NORMAL MOD\n2.DEBUG MOD\nCHOOSE MODE: ");
    scanf(" %d", &choice);
    if(!(choice==1 || choice==2)){
        printf("WRONG CHOICE!!!!");
    }
    else{
        printf("Enter the file name: ");
        scanf(" %s", &fileName);
        FILE *fp = fopen(fileName, "r");
        if(fp == NULL){
            printf("File not found\n");
            return 0;
        }
        
        int numberOfAllVariable=0;
        int tableSize = calculateTableSize(fileName, &numberOfAllVariable);//tablo boyutunu hesapla
        struct YERLESEN *yerlesenler;        
        char ***table;
        int i, hashResult, firstHash;

        yerlesenler = (struct YERLESEN *)malloc(numberOfAllVariable * sizeof(struct YERLESEN));

        table = (char ***)malloc(tableSize * sizeof(char **)); // satir icin yer ayir
        for (i = 0; i < tableSize; i++) {
            table[i] = (char **)malloc(2 * sizeof(char *)); // tur ve ad icin yer ayir
            table[i][0] = (char *)calloc(MAX_LENGTH, sizeof(char)); // tur
            table[i][1] = (char *)calloc(MAX_LENGTH, sizeof(char)); // ad
            strcpy(table[i][0], "null");//null atama
            strcpy(table[i][1], "null");
        }
        
        
        //dosyada kelimeleri oku ve degiskenleri bul
        char line[256], type[MAX_LENGTH];
        while (fgets(line, sizeof(line), fp)) {
            char *token = strtok(line, " ,;\n");//kelimeleri , ; ve \n e gore ayir  
            if (token != NULL) {
                if (strcmp(token, "char") == 0 || strcmp(token, "int") == 0 || strcmp(token, "float") == 0) {//eger char int veya float varsa siradakiler degisken
                    strcpy(type,token);
                    token = strtok(NULL, " ,;\n");
                    while(token != NULL){
                        if (token[0] == '_') {//eger kelimenin ilk harfi _ ise degisken
                            //Variable found
                            if(insert(token, type, table, tableSize, &hashResult, &firstHash)==0)//kelime tabloda varsa ekleme yoksa ekle
                                printf("\n%s daha once deklere edilmis ", token);
                            else{
                                strcpy(yerlesenler[numberOfVariable].name,token);//tabloya eklenenin bilgilerini structa ekle debug modda kullanmak icin
                                yerlesenler[numberOfVariable].first=firstHash;
                                yerlesenler[numberOfVariable].last=hashResult;
                                numberOfVariable++;
                            }
                                
                        }
                        token = strtok(NULL, " ,;\n");
                    }
                    
                }
                else{
                    while(token != NULL){//char int veya float ile deklere edilmedigi durumdaki degiskenler tabloda var mi yok mu kontrol et
                        if (token[0] == '_') {
                            //Variable found
                            if(lookup(token,table,tableSize,&hashResult,&firstHash)==0)
                                printf("\n%s deklere edilmemis ", token);
                        }
                        token = strtok(NULL, " ,;\n");
                    }
                }
                
            }
        }

        fclose(fp);  
        if(choice==2){//debug
            printf("\n\nDeklere edilmis tekrar etmeyen degisken sayisi: %d",numberOfVariable);
            printf("\nDeklere edilmis tum degiskenlerin sayisi: %d",numberOfAllVariable);
            printf("\nSembol tablosunun uzunlugu: %d", tableSize);
            printf("\n\nDeklere edilen her degiskenin hesaplanan ilk adresi ve  yerlestigi adres: "); 
            printf("\n--------------------");
            printf("\n| name | ilk | son |");
            printf("\n--------------------");
            for(i=0;i<numberOfVariable;i++){
                printf("\n| %s |  %d   |  %d  |", yerlesenler[i].name,yerlesenler[i].first,yerlesenler[i].last);
                printf("\n--------------------");
            }
            printf("\n\nSembol tablosunun son hali:\n");
            printf("\n---------------------------");
            printf("\n| satir | type | variable |");
            printf("\n---------------------------");
            for (i = 0; i < tableSize; i++) {
                printf("\n| %d\t| %s | %s\t  |",i,table[i][0],table[i][1]); 
                printf("\n---------------------------");              
            }
        }  
    }
    
   
    return 0;
}
