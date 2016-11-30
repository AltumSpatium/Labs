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

//..................................................................................................

#define DBG_OUTPUT_PORT Serial

//**************************************************************************************************

// Matrix

LedMatrix matrix(MATRIX_CS, MATRIX_DIN, MATRIX_CLK, MATRIX_CNT);
Ticker flipper;

//..................................................................................................

String str = "";

uint16_t offset, curr_pos = 0, lastpos = matrix.Width() - 1;
uint8_t chr;

//**************************************************************************************************

// Server settings

ESP8266WebServer server(80);
DNSServer dnsServer;
IPAddress apIP(192, 168, 1, 1);
const byte DNS_PORT = 53;

//..................................................................................................

const char* ssid = "Esp8266Web";
const char* password = "Topaz123";
const char* host = "esp8266sd.local";

//**************************************************************************************************

// SD card

File uploadFile;
static bool hasSD;

//**************************************************************************************************

void returnOK()
{
  server.send(200, "text/plain", "");
}

void returnFail(String msg)
{
  server.send(500, "text/plain", msg + "\r\n");
}

//**************************************************************************************************

void setup()
{
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.setDebugOutput(true);
  DBG_OUTPUT_PORT.print("\n");

  DBG_OUTPUT_PORT.println();
  DBG_OUTPUT_PORT.println("Configuring access point...");

  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(DNS_PORT, host, apIP);

  DBG_OUTPUT_PORT.print("AP IP address: ");
  DBG_OUTPUT_PORT.println(myIP);
  DBG_OUTPUT_PORT.print("AP IP host: ");
  DBG_OUTPUT_PORT.println(host);

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");
}

void loop()
{
  dnsServer.processNextRequest();
  server.handleClient();
}
