#include <Arduino.h>
#include "LinkedListFile.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  LinkedListFile list;
  //list = LinkedListFile();

  list.addItem(10);
  list.addItem(20);
  list.addItem(30);
  list.addItem(40);
  list.addItem(25);
  Serial.println(list.getSize());
  list.printList();

  list.removeItem(2);
  list.printList();
  list.removeItem(3);
  list.printList();
  list.removeItem(0);
  list.printList();
  list.addItem(25);
  list.printList();
}


void loop() {
  // put your main code here, to run repeatedly:
}
