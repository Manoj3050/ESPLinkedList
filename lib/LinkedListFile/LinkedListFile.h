/* This class implements linked list for embedded systems uses ESP32
uses SPIFF file system for using data since memory is too small to handle data list*/

#ifndef LINKED_LIST_FILE_H
#define LINKED_LIST_FILE_H
#include "Arduino.h"
#include "FS.h"
#include "SPIFFS.h"

#define LIST_FILE "/linkedList.bin"
//This struct keeps a track of location of each item inside the file.
typedef struct INNER_ITEM {
    int itemSize;
    int offset;
    struct INNER_ITEM * nextItem;
}INNER_ITEM;

class LinkedListFile{
private:
    INNER_ITEM  *_begin;
    INNER_ITEM  *_curItem;
    int         _listSize;
    /*open file for read */
    void openFileForRead();
    /*open file for write */
    void openFileForWrite();
    /* delete file*/
    void deleteFile();

    int lastFilePos();
    int getFilePosat(int,int *);

public:
    //constructor
    /* This will open the file at start and prepare for storing data*/
    LinkedListFile();
    /* close and delete the file when destroying */
    ~LinkedListFile();
    /*Add method */
    void addItem(int);
    /*Get item method using index*/
    int getItem(int);
    /*get n number of items at once from given point */
    void getItems(int,int,int *);
    /* get size of the List */
    int getSize();
    /* remove item from the list using index */
    void removeItem(int);

    void printList();
};

#endif
