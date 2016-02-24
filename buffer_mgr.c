//
//  buffer_mgr.c
//  525BufferManager
//
//  Created by mac on 2/12/16.
//  Copyright © 2016 mac. All rights reserved.
//

#include "buffer_mgr.h"
#include <stdio.h>
#include <stdlib.h>
#include "dberror.h"
#include "storage_mgr.h"

#define maxBufferPoolNum 10
//int currentBufferPoolNum 0
int BPCounter = 0;

typedef struct bufferPoolCollection{
    void *pointertoPool;
    int readNum;    //buffer pool read times
    int writeNum; //buffer pool write times
    int poolExsitTime; //a buffer bool exsit time(for LRU)
    int lru_k;// for pool LRU
    struct PageFrame *first;
    struct PageFrame  *last;
    int numPages;//bm->numPages
    
}bufferPoolCollection;
//queue BPCollection;

typedef struct PageFrame{
    int frameNum;
    bool dirty; //store the state of the "dirty"
    int lru_k;
    char *pageLastdata;
    int pageTimeRecord; // record the time record of a page,this value would be refreshed when the page was pinned into the buffer / or used by user(in LRU/LRU-K/CLOCK)
    int fixCount; //fix Count  : evict the page only when fixcount==0
    BM_PageHandle pageHandle;
    struct PageFrame *next;
    
} PageFrame;
/*******************bufferPoolQueue for bufferPoolCollection***********************/
typedef struct bufferPoolQueue
{
    int capacity;
    int size;
    int front;
    int rear;
    bufferPoolCollection *elements;
}bufferPoolQueue;


bufferPoolQueue * createQueue(int maxElements)
{
    /* Create a Queue */
    bufferPoolQueue *Q;
    Q = (bufferPoolQueue *)malloc(sizeof(bufferPoolQueue));
    /* Initialise its properties */
    Q->elements = (bufferPoolCollection *)malloc(sizeof(bufferPoolCollection)*maxElements);
    Q->size = 0;// queue current size
    Q->capacity = maxElements;
    Q->front = 0;
    Q->rear = -1;
    /* Return the pointer */
    return Q;
}
void Dequeue(bufferPoolQueue *Q)
{
    /* If Queue size is zero then it is empty. So we cannot pop */
    if(Q->size==0)
    {
        printf("buuferPoolQueue is Empty\n");
        return;
    }
    /* Removing an element is equivalent to incrementing index of front by one */
    else
    {
        Q->size--;
        Q->front++;
        /* As we fill elements in circular fashion */
        if(Q->front==Q->capacity)
        {
            Q->front=0;
        }
    }
    return;
}
bufferPoolCollection front(bufferPoolQueue *Q)
{
    if(Q->size==0)
    {
        printf("bufferPoolQueue is Empty\n");
        exit(0);
    }
    /* Return the element which is at the front*/
    return Q->elements[Q->front];
}
void Enqueue(bufferPoolQueue *Q,bufferPoolCollection element)
{
    /* If the Queue is full, we cannot push an element into it as there is no space for it.*/
    if(Q->size == Q->capacity)
    {
        printf("Queue is Full\n");
        Dequeue(Q);
    }
    Q->size++;
    Q->rear = Q->rear + 1;
    /* As we fill the queue in circular fashion */
    if(Q->rear == Q->capacity){
        Q->rear = 0;
    }
    /* Insert the element in its rear side */
    Q->elements[Q->rear] = element;
    
    return;
}
/********************************************************/
//There is only one buffer pool for each page file
//We assume Buffer Manager can manage 10 pools at the same time


bufferPoolQueue *BPQueue;


RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName, const int numPages, ReplacementStrategy strategy, void *stratData){
    //stratData can be used to pass parameters for the page replacement strategy. For example, for LRU-k this could be the parameter k.
    //initial PageFrame
    PageFrame *PageFrameNode[bm->numPages];  //
    int i;
    for(i=bm->numPages-1;i>=0;i--)
    {
        PageFrameNode[i]=(PageFrame *) malloc (sizeof(PageFrame));  //initialize every page with an array
        PageFrameNode[i]->pageHandle.pageNum=NO_PAGE;;
        PageFrameNode[i]->frameNum=i;
        PageFrameNode[i]-> pageTimeRecord =0;
        PageFrameNode[i]->dirty=0;
        PageFrameNode[i]->lru_k;
        PageFrameNode[i]->fixCount=0;
        PageFrameNode[i]->pageLastdata=bm->mgmtData+PAGE_SIZE*(i+1);//pointer
        if(i==bm->numPages-1){
            PageFrameNode[i]->next=NULL;
        }
        PageFrameNode[i]->next=PageFrameNode[i+1]; //pointer to the next page.
    }
    //initial Buffer Pool Collection
    char* cache = (char *)calloc(PAGE_SIZE*numPages,sizeof(char));
    bufferPoolCollection BP;
    BP.pointertoPool = bm->mgmtData;
    BP.readNum = 0;
    BP.writeNum=0;
    BP.poolExsitTime = 0;
    BP.lru_k=1;
    BP.numPages=bm->numPages;
    BP.first=PageFrameNode[0];
    BP.last=PageFrameNode[bm->numPages-1];
    Enqueue(BPQueue,BP);
    
    //set Buffer Pool Info
    bm->pageFile = (char*)pageFileName;
    bm->numPages = numPages;
    bm->strategy = strategy;
    bm->mgmtData = cache;
    
    //openPageFile
    SM_FileHandle *fh=(SM_FileHandle *)malloc(sizeof(SM_FileHandle));
    openPageFile(bm->pageFile,fh);
    
    
    return RC_OK;
}










