//
//  storage_mgr.c
//  525StorageManager
//
//  Created by mac on 1/23/16.
//  Copyright © 2016 mac. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>// for read() but is not used, use fread instead

#include "storage_mgr.h"
/**************************************************************************************
 *Function Name: joinString
 *
 *Description: 
 *            join two *char type variable together
 *Parameters: 
 *            char *a: the first half String
 *            char *b: the last half String
 *Return: 
 *            char *tempc: the result String which merges a and b
 *Author: 
 *            BO PENG
 *Hisory: 
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/27/2016             BO PENG<BPENG4@hawk.iit.edu>        self-create
 **************************************************************************************/
char *joinString(char *a, char *b) {
    char *c = (char *) malloc(strlen(a) + strlen(b) + 1);// allocate a new memory for the joined string
    if (c == NULL) {
        exit (1);
    }
    char *tempc = c; //save the first byte address
    while (*a != '\0') {// save a into c
        *c++ = *a++;
    }
    while ((*c++ = *b++) != '\0') {//save b into c
        
    }
    return tempc;
}
/**************************************************************************************
 *Function Name: createPageFile
 *
 *Description: 
 *            create a txt file into "/Users/mac/Desktop/"
 *            Initial content is: first page is full of "\0"
 *Parameters:
 *            char *fileName: the fileName user want to call
 *
 *Return:
 *            RC_OK;
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/26/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/26/2016             BO PENG<BPENG4@hawk.iit.edu>        add comments
 *            1/27/2016             BO PENG<BPENG4@hawk.iit.edu>        use joinString to rewrite
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        run on main(), and it worked
 **************************************************************************************/
RC createPageFile(char *fileName){
    if(fileName==NULL){  //when fileName is NULL
        return RC_FILE_NOT_FOUND;
    }
    //  join the file path, fileName and file type.
    //   For example:
    //   "/Users/mac/Desktop/" + "fileName" + ".txt"
    char *address = "/Users/mac/Desktop/";
    char *firstHalf = joinString(address, fileName);
    char *houzhui = ".txt";
    char *fullAddress = joinString(firstHalf, houzhui);
    //initial the new file's first page with "\0"
    FILE *fp=fopen(fullAddress,"w");         //build a file and get file stream
    char str[PAGE_SIZE];//allocate a block space, which size is PAGE_SIZE bytes
    memset(str,'\0',PAGE_SIZE);           // fill the array with '/0'
    fwrite(str, PAGE_SIZE, 1, fp);        //put the block into the file
    fclose(fp);
    
    return RC_OK;

}
/**************************************************************************************
 *Function Name: openPageFile
 *
 *Description:
 *            open a txt file called fileName at "/Users/mac/Desktop/"
 *Parameters:
 *            char *fileName: the fileName user want to open
 *            SM_FileHandle *fHandle: a strcture to save opened txt file's Info
 *Return:
 *            RC_OK;
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/26/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/26/2016             BO PENG<BPENG4@hawk.iit.edu>        add comments
 *            1/27/2016             BO PENG<BPENG4@hawk.iit.edu>        use joinString to rewrite
 *            1/29/2016             BO PENG<BPENG4@hawk.iit.edu>        add show file content onto screen part
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        run on main(), and it worked
 **************************************************************************************/
RC openPageFile(char *fileName, SM_FileHandle *fHandle){
    //initial variable
    FILE *fwatch;
    FILE *fp;
    char ch;
    long fileSize;
    long lastPos = -1;
    //  join the file path, fileName and file type.
    //   For example:
    //   "/Users/mac/Desktop/" + "fileName" + ".txt"
    char *address = "/Users/mac/Desktop/";
    char *firstHalf = joinString(address, fileName);
    char *houzhui = ".txt";
    char *fullAddress = joinString(firstHalf, houzhui);
    // fullAddress = "/Users/mac/Desktop/" + "fileName" + ".txt"
    // show the file on screen
    fwatch=fopen(fullAddress,"rt");
    ch = fgetc(fwatch);  //character of file
    while(ch!=EOF) { //end of file(EOF)
        putchar(ch); // show the file character by character
        ch=fgetc(fwatch);
    }
    fclose(fwatch);
    //collect file info: 1)fileName, 2)totalNumPages, 3)curPagePos, 4)mgmtInfo
    fp =fopen(fullAddress,"rt+");
    if(fp ==NULL){
        return RC_FILE_NOT_FOUND;
    }
    int Ban;
    Ban = fseek(fp, 0L, SEEK_END);
    if(Ban == -1){//if failed
        return RC_UNESPECTED_ERROR;
    }
    // if rellocating pointer is success
    lastPos = ftell(fp); //locate the lastPosistion in the file
    fileSize = lastPos + 1;
    fHandle->fileName = fileName;
    fHandle->totalNumPages = (int) fileSize/PAGE_SIZE;
    fHandle->mgmtInfo = fp;
    fHandle ->curPagePos =0;
    return RC_OK;
    
}
/**************************************************************************************
 *Function Name: closePageFile
 *
 *Description:
 *            close a txt file which already has an SM_FileHandle
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *Return:
 *            RC_OK;
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/26/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/26/2016             BO PENG<BPENG4@hawk.iit.edu>        add comments
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 **************************************************************************************/
RC closePageFile(SM_FileHandle *fHandle){
    
    if (fHandle == NULL){                         //if the fHandle is null, then return not init
        return RC_FILE_HANDLE_NOT_INIT;
    }
    //clean the screen
    system("cls");
    
    //set the fhandle structure
    fHandle->fileName = NULL;                //resettle the fileName to NULL
    fHandle->mgmtInfo = NULL;                ////resettle the mgmtInfo to NULL
    fHandle->curPagePos = 0;                 //resettle the curPagePos to 0
    fHandle->totalNumPages = 0;              //resettle the totalPage to 0
    
    return RC_OK;
    
}
/**************************************************************************************
 *Function Name: destroyPageFile
 *
 *Description:
 *            destroy a txt file called fileName at "/Users/mac/Desktop/"
 *Parameters:
 *            char *fileName: the fileName user want to open
 *Return:
 *            RC_OK;
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/28/2016             BO PENG<BPENG4@hawk.iit.edu>        first create using joinString to rewrite
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        run on main(), and it worked
 **************************************************************************************/
RC destroyPageFile(char *fileName){  //extern RC destroyPageFile (char *fileName)
    
    
    if (fileName == NULL){ 
        
        return RC_FILE_NOT_FOUND;
    }
    //  join the file path, fileName and file type.
    //   For example:
    //   "/Users/mac/Desktop/" + "fileName" + ".txt"
    char *address = "/Users/mac/Desktop/";
    char *firstHalf = joinString(address, fileName);
    char *houzhui = ".txt";
    char *fullAddress = joinString(firstHalf, houzhui);
    //delete such file
    remove(fullAddress);
    return RC_OK;
    
}
/**************************************************************************************
 *Function Name: readBlock
 *
 *Description:
 *            read any page(block) which the user want to read
 *            in a txt file which already has an SM_FileHandle
 *Parameters:
 *            int pageNum: page number of the page user want to read
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK;
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/28/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/28/2016             BO PENG<BPENG4@hawk.iit.edu>        add comments
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        rewrite totally
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        run on main(), and it worked
 **************************************************************************************/
//
//
/******************below is BO PENG history code 1/28/2016****************
 RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
    if (fHandle== NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if (fHandle->totalNumPages<=pageNum) {
        return RC_READ_NON_EXISTING_PAGE;
    }
    // show such page content on screen
 
    char result[PAGE_SIZE];
    int count = 0;
    count=pageNum;
    while (count !=0) {//fgets(temp, PAGE_SIZE, fHandle->mgmtInfo)
    fgets(result, PAGE_SIZE, fHandle->mgmtInfo);
    count--;
    }
    printf("%s",result);
    return RC_OK:
 }
*******************************************************************/
RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
    if (fHandle== NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if (fHandle->totalNumPages<=pageNum) {
        return RC_READ_NON_EXISTING_PAGE;
    }
    int Ban = fseek(fHandle->mgmtInfo, PAGE_SIZE*pageNum, SEEK_SET);// reallocate the pointer
    if(Ban != 0){//if failed
        return RC_SET_POINTER_FAILED;
    }
    // if rellocating pointer is success
    long readBan = fread(memPage, PAGE_SIZE, 1, fHandle->mgmtInfo);
    if(readBan == 0){//fread failed
        return RC_READ_FAILED;
    }
    //read success
    fHandle->curPagePos=pageNum;
    printf("%s",memPage);// show page info onto screen
    return RC_OK;
    
}


/**************************************************************************************
 *Function Name: getBlockPos
 *
 *Description:
 *            find which page user is reading
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *Return:
 *            (int)page number reading right now;
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/29/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/29/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 **************************************************************************************/
int getBlockPos (SM_FileHandle *fHandle){
    if(fHandle == NULL){
        return RC_FILE_HANDLE_NOT_INIT;
    }
    return fHandle->curPagePos;
}
/**************************************************************************************
 *Function Name: readFirstBlock
 *
 *Description:
 *            show first page content onto screen
 *            update the fHandle info
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/29/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/29/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 **************************************************************************************/
RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    if(fHandle == NULL){
        return RC_FILE_HANDLE_NOT_INIT;
    }
    int Ban = fseek(fHandle->mgmtInfo, PAGE_SIZE*0, SEEK_SET);// make sure the pointer is at the first page
    if(Ban != 0){//if failed
        return RC_SET_POINTER_FAILED;
    }
    long readBan = fread(memPage, PAGE_SIZE, 1, fHandle->mgmtInfo);
    if(readBan == 0){
        return RC_READ_FAILED;
    }
    fHandle->curPagePos=0;
    printf("%s",memPage);// show page info onto screen
    return RC_OK;
}
/**************************************************************************************
 *Function Name: readPreviousBlock
 *
 *Description:
 *            show previous page content onto screen
 *            update the fHandle info
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 **************************************************************************************/

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    if (fHandle == NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if (fHandle->curPagePos == 0) {
        return RC_READ_NON_EXISTING_PAGE;//there is no page before the first page
    }
    long goPrev = fseek(fHandle->mgmtInfo, -PAGE_SIZE,SEEK_CUR);// move the pointer one page earlier
    if (goPrev !=0) {
        return RC_UNESPECTED_ERROR;
    }
    // read the first page into memPage(is char* variable(string variable))
    long readPrev = fread(memPage, PAGE_SIZE, 1, fHandle->mgmtInfo);
    // if read donesnt work
    if (readPrev == 0)
    {
        return RC_UNESPECTED_ERROR;
    }
    fHandle->curPagePos =fHandle->curPagePos - 1;
    // show prevous page info onto screen
    printf("%s",memPage);
    return RC_OK;
}
/**************************************************************************************
 *Function Name: readCurrentBlock
 *
 *Description:
 *            show previous page content onto screen
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/29/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        concern some special circumstance
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 **************************************************************************************/
RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    if (fHandle == NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if(fHandle->totalNumPages<1){
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if (fHandle->curPagePos < 0){
        return RC_READ_NON_EXISTING_PAGE;
    }
    if (fHandle->curPagePos > fHandle->totalNumPages-1){
        return RC_READ_NON_EXISTING_PAGE;
    }
    long readCur = fread(memPage, PAGE_SIZE, 1, fHandle->mgmtInfo); //
    if (readCur == 0) // if read donesnt work
    {
        return RC_UNESPECTED_ERROR;
    }
    // show prevous page info onto screen
    printf("%s",memPage);
    return RC_OK;
    
}
/**************************************************************************************
 *Function Name: readNextBlock
 *
 *Description:
 *            show next page content onto screen
 *            update the fHandle info
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/30/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 **************************************************************************************/
RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    if (fHandle == NULL) {
        return RC_FILE_HANDLE_NOT_INIT;
    }
    if (fHandle->curPagePos == fHandle->totalNumPages-1) {
        return RC_READ_NON_EXISTING_PAGE;//there is no page after the last page
    }
    long goBack = fseek(fHandle->mgmtInfo, +PAGE_SIZE,SEEK_CUR);//move the pointer one page later
    if (goBack !=0) {
        return RC_UNESPECTED_ERROR;
    }
    long readBack = fread(memPage, PAGE_SIZE, 1, fHandle->mgmtInfo); //
    // if read donesnt work
    if (readBack == 0){
        return RC_UNESPECTED_ERROR;
    }
    fHandle->curPagePos =fHandle->curPagePos + 1;
    // show prevous page info onto screen
    printf("%s",memPage);
    return RC_OK;
}
/**************************************************************************************
 *Function Name: readLastBlock
 *
 *Description:
 *            show last page content onto screen
 *            update the fHandle info
 *Parameters:
 *            SM_FileHandle *fHandle: a strcture which saved opened txt file's Info
 *            SM_PageHandle memPage: a variable to save such page's content
 *Return:
 *            RC_OK
 *Author:
 *            BO PENG
 *Hisory:
 *            Date                  Name                                Content
 *            ----------            ------------------------------      ---------------
 *            1/29/2016             BO PENG<BPENG4@hawk.iit.edu>        first create
 *            1/29/2016             BO PENG<BPENG4@hawk.iit.edu>        add header comments
 **************************************************************************************/
RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
    if(fHandle == NULL){
        return RC_FILE_HANDLE_NOT_INIT;
    }
    // reallocate the pointer to make sure the pointer is at the beginning of the last page
    int Ban = fseek(fHandle->mgmtInfo, -PAGE_SIZE, SEEK_END);    if(Ban != 0){//if failed
        return RC_SET_POINTER_FAILED;
    }
    long readBan = fread(memPage, PAGE_SIZE, 1, fHandle->mgmtInfo);
    if(readBan == 0){
        return RC_READ_FAILED;
    }
    fHandle->curPagePos=fHandle->totalNumPages-1;
    printf("%s",memPage);// show page info onto screen
    return RC_OK;
}



