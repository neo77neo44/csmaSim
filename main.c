#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <windows.h>

typedef unsigned char vc_uint8;
typedef unsigned int vc_uint16;
typedef unsigned long vc_uint32;
typedef enum nodeState{
    nodeIdle,
    nodeBackoffing,
    nodeTransmiting,
}nodeState;
vc_uint32 NodeSeed[100];
pthread_t Node[100];
pthread_t CSMA_SCH;

vc_uint32 totalMs = 200;
vc_uint32 raminMs = 200;
vc_uint32 currentMs = 0;
vc_uint32 pktSize = 15;
vc_uint8 pktSending = 0;
void* NodeSch(int nodeSeed) {
    int a;
    srand(nodeSeed);
    while(currentMs != totalMs){
        a=(rand()%100)+1;
        printf("nodeSeed:%lu, %d,%d\r\n",currentMs,nodeSeed, a);
        Sleep(50);
    }
}
void csma_sch(void* data) {
    while(1){
        if(currentMs!=totalMs){

        }else{
            pthread_exit(NULL);
        }
        currentMs++;
        Sleep(50);
    }
}
int main()
{
    int node_num = 4;
    int i;
    for(i=0;i<node_num;i++){
        pthread_create(&Node[i], NULL, NodeSch, i);
    }
    pthread_create(&CSMA_SCH, NULL, csma_sch, "csma_sch");
    for(i=0;i<node_num;i++){
        pthread_join(Node[i], NULL);
    }
    pthread_join(CSMA_SCH, NULL);
    return 0;
}
