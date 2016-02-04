//
//  main.c
//
//  Created by Young on 1/23/16.
//  Copyright © 2016 Young. All rights reserved.
//

#include <stdio.h>
#include "storage_mgr.h"
#include "dberror.h"
#include <stdlib.h>


int main(int argc, const char * argv[]) {
    
    FILE *f=fopen("//Users//young//Desktop//test.txt", "a+");

    //find the size of file
    fseek(f,0,SEEK_END);
    int size=(int)ftell(f);
    printf("size of file f:%d\n",size);
    fseek(f,0,SEEK_SET);
    
    //initialize fileHandle
    SM_FileHandle fh={"//Users//young//Desktop//test.txt",size/PAGE_SIZE,0,f};
    fh.mgmtInfo=f;
    
    //initialize pageHandle;
    SM_PageHandle ph;
    char pagetemp[4096]="abcdefgABCDEFGHI";
    ph=pagetemp;
    
    //testing
//    printf("ph1:%s\n",ph);
//    //ph=malloc(1024*sizeof(char));
//    printf("RC=%d\n",writeBlock(1, &fh, ph));
    
    printf("size of old file=%d\n",findsize(fh.mgmtInfo));
    printf("appendEmptyBlock return=%d\n",appendEmptyBlock(&fh));
    printf("size of new file=%d\n",findsize(fh.mgmtInfo));
    
    ensureCapacity(10, &fh);
    return 0;
}

int findsize(FILE *f)
{
    fseek(f,0,SEEK_END);
    int size=(int)ftell(f);
    fseek(f,0,SEEK_SET);
    return(size);
}


/**************************************************************************************
 * Function Name: writeBlock
 *
 * Description:
 *            write this block from memory to a file
 * Parameters:
 *            int pageNum: which page need to be written
 *            SM_FileHandle *fHandle: a structure that contains informations of a file(including pointer to this file)
 *            memPage:a buffer that stores data of block
 * Return:
 *            RC: return a status of success or fail
 * Author:
 *            Qi Yang
 * History:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            2016-01-23             Qi Yang<qyang18@hawk.iit.edu>        self-create
 **************************************************************************************/
RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    //write all the pages from memPage to fHandle->mgmtInfo
    int write_result=fwrite(memPage, sizeof(char), PAGE_SIZE, fHandle->mgmtInfo);
    fclose(fHandle->mgmtInfo);
    printf("write result:%d\n",write_result);
    if (write_result/PAGE_SIZE!=pageNum)
    {
        return RC_FILE_NOT_FOUND;//1
    }
    else
    {
        fHandle->curPagePos=pageNum+1;
        return RC_OK;//0
    }
}


/**************************************************************************************
 * Function Name: writeCurrentBlock
 *
 * Description:
 *            write current block to a file
 * Parameters:
 *            SM_FileHandle *fHandle: a structure that contains informations of a file(including pointer to this file)
 *            memPage:a buffer that stores data of block
 * Return:
 *            RC: return a status of success or fail
 * Author:
 *            Qi Yang
 * History:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            2016-01-23             Qi Yang<qyang18@hawk.iit.edu>        self-create
 **************************************************************************************/
RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
    if (fHandle==NULL) return RC_FILE_NOT_FOUND;
    //write current position from memPage to the file
    return writeBlock(fHandle->curPagePos, fHandle, memPage);
}


/**************************************************************************************
 * Function Name: appendEmptyBlock
 *
 * Description:
 *            increase the number of pages in the file by one
 * Parameters:
 *
 *            SM_FileHandle *fHandle: a structure that contains informations of a file(including pointer to this file)
 * Return:
 *            RC: return a status of success or fail
 * Author:
 *            Qi Yang
 * History:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            2016-01-23             Qi Yang<qyang18@hawk.iit.edu>        self-create
 **************************************************************************************/
RC appendEmptyBlock (SM_FileHandle *fHandle)
{
    FILE *file=fHandle->mgmtInfo;
    //allocate a new memory for new block
    char *newPage=malloc(PAGE_SIZE);
    //write newPage into current file
    int WriteResult=fwrite(newPage,1,PAGE_SIZE,file);
    if (WriteResult==0)
    {
        RC_WRITE_FAILED;
    }
    printf("newfile size=%d\n",findsize(file));
    fHandle->totalNumPages+=1;
    printf("newfile page=%d\n",fHandle->totalNumPages);
    return RC_OK;
}


/**************************************************************************************
 * Function Name: If the file has less than numberOfPages pages then increase the size to numberOfPages.
 *
 * Description:
 *            increase number of pages so that
 * Parameters:
 *
 *            SM_FileHandle *fHandle: a structure that contains informations of a file(including pointer to this file)
 *            memPage:a buffer that stores data of block
 * Return:
 *            RC: return a status of success or fail
 * Author:
 *            Qi Yang
 * History:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            2016-01-23             Qi Yang<qyang18@hawk.iit.edu>        self-create
 **************************************************************************************/
RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
    if (fHandle->totalNumPages<numberOfPages)
    {
        //how many pages need to be appended.
        int pages,i;
        pages=numberOfPages-fHandle->totalNumPages;
        //appendEmptyBlock for "pages" times
        for (i=0;i<pages;i++)
        {
            printf("iteration:%d\n",i);
            appendEmptyBlock(fHandle);
        }
        printf("after ensureCapacity total number of pages=%d\n",fHandle->totalNumPages);
        return RC_OK;
    }
    else return RC_READ_NON_EXISTING_PAGE;
}
