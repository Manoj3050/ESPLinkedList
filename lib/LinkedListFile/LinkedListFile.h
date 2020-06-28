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

template <typename T>
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
    void addItem(T&);
    /*Get item method using index*/
    T getItem(int);
    /*get n number of items at once from given point */
    void getItems(int,int,T *);
    /* get size of the List */
    int getSize();
    /* remove item from the list using index */
    void removeItem(int);

    void printList();
};

template <typename T>
LinkedListFile<T>::LinkedListFile(){
    SPIFFS.begin(true);
    _begin = NULL;
    _curItem = NULL;
    _listSize = 0;
    SPIFFS.remove(LIST_FILE);
}

template <typename T>
LinkedListFile<T>::~LinkedListFile(){
    delete _begin;
    delete _curItem;
}

template <typename T>
void LinkedListFile<T>::openFileForWrite(){

}

template <typename T>
int LinkedListFile<T>::lastFilePos(){
    int offSet = 0;
    if(_begin != NULL){
        INNER_ITEM * itterItem = _begin;
        while(itterItem->nextItem != NULL){
            offSet += itterItem->itemSize;
            itterItem = itterItem->nextItem;
        }
    }
    return offSet;
}

template <typename T>
int LinkedListFile<T>::getFilePosat(int pos,int *itemSize){
    int offSet = 0;
    int itemCounter = 0;
    if(_begin != NULL){
        INNER_ITEM * itterItem = _begin;
        for(itemCounter = 0; (itemCounter<pos) & (itterItem!= NULL); itemCounter++ ){
            itterItem = itterItem->nextItem;
        }
        offSet = itterItem->offset;
        *itemSize = itterItem->itemSize;
    }
    return offSet;
}

template <typename T>
void LinkedListFile<T>::addItem(T &item){
    File listFile = SPIFFS.open(LIST_FILE, FILE_APPEND);
    if(_begin == NULL){ //check if the list is empty
        _begin = new INNER_ITEM();
        _begin->itemSize = sizeof(item);
        _begin->nextItem = NULL;
        _begin->offset = 0;
        _curItem = _begin;
        //write item to the file
        listFile.seek(0,SeekSet);
        listFile.write((byte*)&item, sizeof(item));

    }
    else{
        INNER_ITEM * newItem = new INNER_ITEM();
        //calculate position to write in the File
        int offSet = lastFilePos();
        newItem->itemSize = sizeof(item);
        newItem->nextItem = NULL;
        newItem->offset = offSet + _curItem->itemSize;
        _curItem->nextItem = newItem;
        listFile.seek(offSet,SeekSet);
        listFile.write((byte*)&item, sizeof(item));
        _curItem = newItem;

    }
    _listSize++;
    listFile.close();
}

template <typename T>
T LinkedListFile<T>::getItem(int pos){
    T resultItem;
    int resultItemSize;
    File listFile = SPIFFS.open(LIST_FILE, FILE_READ);
    int offSet = getFilePosat(pos, &resultItemSize);
    listFile.seek(offSet,SeekSet);
    listFile.read((byte *)&resultItem, sizeof(resultItemSize));
    listFile.close();
    return resultItem;

}

template <typename T>
void LinkedListFile<T>::getItems(int pos, int noOfitems, T * resultItems){
    /*int resultItemSize;
    File listFile = SPIFFS.open(LIST_FILE, FILE_READ);
    int offSet = getFilePosat(pos, &resultItemSize);
    int pointerOffset = 0;
    int * resultItem;
    for(int i=0;i<noOfitems;i++){
        resultItem = (int*)malloc(resultItemSize);
        listFile.seek(offSet,SeekSet);
        listFile.read((byte *)(resultItems+pointerOffset), sizeof(resultItemSize));
        pointerOffset += resultItemSize;
        offSet += resultItemSize;

    }*/
}

template <typename T>
int LinkedListFile<T>::getSize(){
    return _listSize;
}

template <typename T>
void LinkedListFile<T>::removeItem(int pos){
    int itemCounter = 0;
    if(_begin != NULL){
        INNER_ITEM * itterItem = _begin;
        if(pos == 0){
            _begin = itterItem->nextItem;
        }
        else{
            for(itemCounter = 0; (itemCounter<pos-1) & (itterItem->nextItem != NULL); itemCounter++ ){ // iterate until item before the index.
                itterItem = itterItem->nextItem;
            }
            INNER_ITEM * tmpItem = itterItem;
            tmpItem->nextItem = itterItem->nextItem->nextItem;
            if(tmpItem->nextItem == NULL){ //had we removed last item, then set cur item to our new last
                _curItem = tmpItem;
            }
        }
        //delete itterItem; // need to take care
        _listSize--;
    }
}

template <typename T>
void LinkedListFile<T>::printList(){
    Serial.println("Start of the List");
    if(_begin!= NULL){
        for(int i=0; i<_listSize;i++){
            T item = getItem(i);
            Serial.println(item.x);
        }
    }
    Serial.println("End of the List");
}

#endif
