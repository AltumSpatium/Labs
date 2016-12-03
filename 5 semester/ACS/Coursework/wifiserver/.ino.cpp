#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2016-12-03 18:54:16

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <Ticker.h>
#include <SPI.h>
#include <SD.h>
#include "ledmatrix.h"
void returnOK() ;
void returnFail(String msg) ;
bool loadFromSdCard(String path) ;
void handleNotFound() ;
void handleCreate() ;
void handleDelete() ;
void deleteRecursive(String path) ;
void handleFileUpload() ;
void showString() ;
void tickProcedure() ;
void printString() ;
void setup() ;
void loop() ;

#include "wifiserver.ino"


#endif
