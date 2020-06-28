#include "LinkedListFile.h"

LinkedListFile::LinkedListFile(){
    SPIFFS.begin(true);
    _begin = NULL;
    _curItem = NULL;
    _listSize = 0;
    SPIFFS.remove(LIST_FILE);
}

LinkedListFile::~LinkedListFile(){
    delete _begin;
    delete _curItem;
}

void LinkedListFile::openFileForWrite(){

}

int LinkedListFile::lastFilePos(){
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

int LinkedListFile::getFilePosat(int pos,int *itemSize){
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

void LinkedListFile::addItem(int item){
    File listFile = SPIFFS.open(LIST_FILE, FILE_APPEND);
    if(_begin == NULL){ //check if the list is empty
        _begin = new INNER_ITEM();
        _begin->itemSize = sizeof(item);
        _begin->nextItem = NULL;
        _begin->offset = 0;
        _begin->item = item;
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
        newItem->item = item;
        _curItem->nextItem = newItem;
        listFile.seek(offSet,SeekSet);
        listFile.write((byte*)&item, sizeof(item));
        _curItem = newItem;

    }
    _listSize++;
    listFile.close();
}

int LinkedListFile::getItem(int pos){
    int resultItem;
    int resultItemSize;
    File listFile = SPIFFS.open(LIST_FILE, FILE_READ);
    int offSet = getFilePosat(pos, &resultItemSize);
    listFile.seek(offSet,SeekSet);
    listFile.read((byte *)&resultItem, sizeof(resultItemSize));
    return resultItem;
    listFile.close();
}

void LinkedListFile::getItems(int pos, int noOfitems, int * resultItems){
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

int LinkedListFile::getSize(){
    return _listSize;
}

void LinkedListFile::removeItem(int pos){
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

void LinkedListFile::printList(){
    Serial.println("Start of the List");
    if(_begin!= NULL){
        for(int i=0; i<_listSize;i++){
            int item = getItem(i);
            Serial.println(item);
        }
    }
    Serial.println("End of the List");
}
