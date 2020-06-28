#include <Arduino.h>
#include "LinkedListFile.h"
#define STRUCT

typedef struct {
    String Name;
    float x;
} MYDATA;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  #ifdef INT
  LinkedListFile<int> list;
  //list = LinkedListFile();
  int tmp = 10;
  list.addItem(tmp);
  tmp = 20;
  list.addItem(tmp);
  tmp = 30;
  list.addItem(tmp);
  tmp = 40;
  list.addItem(tmp);
  tmp = 45;
  list.addItem(tmp);
  Serial.println(list.getSize());
  list.printList();

  list.removeItem(2);
  list.printList();
  list.removeItem(3);
  list.printList();
  list.removeItem(0);
  list.printList();
  tmp = 35;
  list.addItem(tmp);
  list.printList();
  #endif
  #ifdef STRUCT
  LinkedListFile<MYDATA> listS;
  MYDATA tmpS = {"Hello", 10.5};
  listS.addItem(tmpS);
  tmpS.x = 5.43;
  tmpS.Name = "Hi";
  listS.addItem(tmpS);
  tmpS.x = 3.14567;
  tmpS.Name = "Bonjure";
  listS.addItem(tmpS);
  listS.printList();
  #endif
}


void loop() {
  // put your main code here, to run repeatedly:
}
