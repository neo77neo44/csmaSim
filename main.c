#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <windows.h>
#include <signal.h>
#include <sched.h>

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

vc_uint32 totalMs = 2000;
vc_uint32 raminMs = 2000;
vc_uint32 currentMs = 0;
vc_uint32 pktSize = 15;
vc_uint8 pktSending = 0;
vc_uint32 waitForPktSend=0;
vc_uint32 NodeBackoffTime[100];
vc_uint32 NodeSendingStatus[100];
vc_uint32 visionTime=10;
vc_uint16 csmaSchTick[7]={0,25,50,75,575,1075,1575};
void* NodeSch(int nodeIdx) {
    int a;
    int backoff=0;
    srand(nodeIdx);
    while(currentMs != totalMs){
        NodeBackoffTime[nodeIdx] = (rand()%100)+1;
        if(NodeBackoffTime[nodeIdx] >= currentMs)
        {
            if(pktSending){
                backoff = (rand()%100)+1;
                //printf("%lu,Collision!\r\n",currentMs);
                NodeBackoffTime[nodeIdx] = currentMs + backoff;
            }else{
                //printf("%lu,Sending!\r\n",currentMs);
                NodeSendingStatus[nodeIdx] = 1;
            }
        }
        Sleep(visionTime);
    }
}
void csma_sch(void* data) {
    while(1){
        if(currentMs!=totalMs){
            if(currentMs>=csmaSchTick[0] && currentMs < csmaSchTick[1]){
                    printf("%d->%s\r\n",currentMs,"BCN_A");
            }else if(currentMs>=csmaSchTick[1] && currentMs < csmaSchTick[2]){
                    printf("%d->%s\r\n",currentMs,"BCN_B");
            }else if(currentMs>=csmaSchTick[2] && currentMs < csmaSchTick[3]){
                    printf("%d->%s\r\n",currentMs,"BCN_C");
            }else if(currentMs>=csmaSchTick[3] && currentMs < csmaSchTick[4]){
                    printf("%d->%s\r\n",currentMs,"CSMA_A");
            }else if(currentMs>=csmaSchTick[4] && currentMs < csmaSchTick[5]){
                    printf("%d->%s\r\n",currentMs,"CSMA_B");
            }else if(currentMs>=csmaSchTick[5] && currentMs < csmaSchTick[6]){
                    printf("%d->%s\r\n",currentMs,"CSMA_C");
            }else{
                    printf("%d->%s\r\n",currentMs,"IDLE");
            }
        }else{
            pthread_exit(NULL);
        }
        currentMs++;
        currentMs%=2000;
        Sleep(visionTime);
    }
}
int main()
{
    int node_num = 2;
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
