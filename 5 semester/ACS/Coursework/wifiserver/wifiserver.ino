//**************************************************************************************************

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <Ticker.h>
#include <SPI.h>
#include <SD.h>
#include "ledmatrix.h"

//**************************************************************************************************

#define MATRIX_CS D3
#define MATRIX_DIN D2
#define MATRIX_CLK D1
#define MATRIX_CNT 5

//**************************************************************************************************

LedMatrix matrix(MATRIX_CS, MATRIX_DIN, MATRIX_CLK, MATRIX_CNT);

//**************************************************************************************************
