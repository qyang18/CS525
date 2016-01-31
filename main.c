//
//  main.c
//  TESTC
//
//  Created by Young on 1/23/16.
//  Copyright © 2016 Young. All rights reserved.
//

#include <stdio.h>
#include "storage_mgr.h"
#include "dberror.h"
#include <stdlib.h>


int main(int argc, const char * argv[]) {
    
    FILE *f=fopen("//Users//young//Desktop//test.txt", "rt");
    
    char string[100]="";
    fgets(string,10,f);
   // printf("%s",string);

    SM_FileHandle *fh;
    fseek(f,0,SEEK_END);
    int size=(int)ftell(f);
    
//    fh->curPagePos=0;
//    fh->fileName="//Users//young//Desktop//test.txt";
//    fh->mgmtInfo=f;
//    fh->totalNumPages=size;//
    
    struct SM_FileHandle fh_temp={"//Users//young//Desktop//test.txt",size,0,f};
    fh=&fh_temp;
    fh->mgmtInfo=f;
    SM_PageHandle ph;
    ph=malloc(1024*sizeof(char));
    printf("%d\n",writeBlock(1, fh, ph));
    return 0;
}


RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{   //write this block from file to memory
    //pageNum:
    //fHandle:pointer to the file
    //memPage:point to the memory position to start writing
    int write_result=fwrite(memPage, sizeof(char), 1024, fHandle->mgmtInfo);
    fclose(fHandle->mgmtInfo);
    if (write_result!=pageNum)
    {
        return RC_FILE_NOT_FOUND;
    }
    else
    {
        fHandle->curPagePos+=write_result;
        return RC_OK;
    }
}

/*
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
}

RC appendEmptyBlock (SM_FileHandle *fHandle)
{
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
}
*/