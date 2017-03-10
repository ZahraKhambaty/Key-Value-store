#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<unistd.h>
#include<string.h>
#include<semaphore.h>
#define TABLE_SIZE 256 
#define MAX_KEY_SIZE 32
#define MAX_VALUE_SIZE 256
#define MAX_NUM_Slots 256
#define MAX_NUM_Pairs 256
#define MY_SHM "ZahraKhambaty"

int fd;

typedef struct {
    char key[MAX_KEY_SIZE];
    char value[MAX_VALUE_SIZE];
}KVPair;


typedef struct {
    sem_t mutex; 
    int index;
     KVPair pairs[MAX_NUM_Pairs];
    }Slot ;
    

typedef struct {
     Slot slots[MAX_NUM_Slots];
}Store;

Store* s;

int kv_store_create(){
     fd= shm_open(MY_SHM, O_CREAT | O_RDWR,0666);
     if(fd==-1){
        printf("shm_open() Failed!!\n");
        exit(1);
     }
    ftruncate(fd,sizeof( Store));
    s = ( Store*) mmap(NULL,sizeof( Store),PROT_WRITE| PROT_READ, MAP_SHARED,fd,0);
    if(s == MAP_FAILED){
        printf("mmap Failed!\n");
        exit(1);
    }return 0;
}

int hashCode(char *key){
    int hashAddress = 5381;
    for (int counter = 0; key[counter]!='\0'; counter++){
        hashAddress = ((hashAddress << 5) + hashAddress) + key[counter];
    }
    return hashAddress % TABLE_SIZE < 0 ? -hashAddress % TABLE_SIZE : hashAddress % TABLE_SIZE;
}
int kv_store_write(char *key,char *value){
    int hashIndex =hashCode (key);
    //hash the key to get the required slot
    Slot sl = s->slots[hashIndex];
    //store the key value pair
    memcpy(sl.pairs[sl.index].key,key,strlen(key));
    memcpy(sl.pairs[sl.index].value,value,strlen(value));
    //cycle to avoid overflow
    ++sl.index;;                     // go to the next cell
    sl.index %= MAX_NUM_Pairs;        //wrap aound the store
    return 0;
}

char* kv_store_read(char *key){
    
    int hashIndex = hashCode(key); // get the hash
    Slot* current = &(s->slots[hashIndex]);
    char * res=(char*)malloc(sizeof(char)*MAX_VALUE_SIZE);
    memcpy
    return NULL;
}

char ** kv_store_read_all(){return NULL;}
          
