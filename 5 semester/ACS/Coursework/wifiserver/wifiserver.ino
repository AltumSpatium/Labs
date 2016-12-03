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

bool needClear = true;
bool canPrint = false;
bool isPrinting = false;

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

bool loadFromSdCard(String path)
{
  String dataType = "text/plain";

  if (path.endsWith("/")) path += "index.htm";
  if (path.endsWith(".js")) dataType  = "application/javascript";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";

  File dataFile = SD.open(path.c_str());
  if (dataFile.isDirectory())
  {
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile) return false;

  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size())
    DBG_OUTPUT_PORT.println("Sent less data than expected!");

  dataFile.close();

  return true;
}

void handleNotFound()
{
  if (hasSD && loadFromSdCard(server.uri())) return;

  String message = "SD card not detected\n\n";
  message += "URI: " + server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET ? "GET" : "POST");
  message += "\nArguments: " + server.args();
  for (uint8_t i = 0; i < server.args(); i++)
    message += "\n\n NAME: " + server.argName(i) + "\n VALUE: " + server.arg(i);

  DBG_OUTPUT_PORT.println(message);
  server.send(404, "text/plain", message);
}

void handleCreate()
{
  if (server.args() == 0) return returnFail("BAD ARGS");

  String path = server.arg(0);
  if (path == "/" || SD.exists((char*) path.c_str()))
    return returnFail("BAD PATH");

  if (path.indexOf('.') > 0)
  {
    File file = SD.open((char*) path.c_str(), FILE_WRITE);
    if (file)
    {
      file.write((const char*) 0);
      file.close();
    }
    else SD.mkdir((char*) path.c_str());
  }

  returnOK();
}

void handleDelete()
{
  if (server.args() == 0) return returnFail("BAD ARGS");

  String path = server.arg(0);
  if (path == "/" || SD.exists((char*) path.c_str()))
    return returnFail("BAD PATH");

  deleteRecursive(path);

  returnOK();
}

void deleteRecursive(String path)
{
  File file = SD.open((char*) path.c_str());
  if (!file.isDirectory())
  {
    file.close();
    SD.remove((char*) path.c_str());
    return;
  }

  file.rewindDirectory();
  while(true)
  {
    File entry = file.openNextFile();
    if (!entry) break;

    String entryPath = path + "/" + entry.name();
    if (entry.isDirectory())
    {
      entry.close();
      deleteRecursive(entryPath);
    }
    else
    {
      entry.close();
      SD.remove((char*) entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char*) path.c_str());
  file.close();
}

void handleFileUpload()
{
  if (server.uri() != "/edit") return;

  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START)
  {
    if (SD.exists((char*) upload.filename.c_str()))
      SD.remove((char*) upload.filename.c_str());

    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    DBG_OUTPUT_PORT.println("Upload: START, filename: " + upload.filename);
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    DBG_OUTPUT_PORT.println("Upload: WRITE, bytes: " + upload.currentSize);
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (uploadFile) uploadFile.close();
    DBG_OUTPUT_PORT.println("Upload: END, size: " + upload.totalSize);
  }
}

void showString()
{
  if (needClear)
  {
    matrix.Fill(0x00);
    needClear = false;
  }

  if ((chr = str[curr_pos]) != 0)
  {
    if (chr != '\n')
    {
      if (offset < (LEDMATRIX_FONT_WIDTH + 1) && chr != '\r')
      {
        matrix.UpdateLock(1);
        matrix.Scroll(0, -1);
        matrix.DrawChar(lastpos - offset, 0, chr);
        matrix.UpdateLock(0);
        offset++;
      }
      else
      {
        offset = 0;
        curr_pos++;
      }
    }
    else
    {
      if (offset < matrix.Height())
      {
        matrix.Scroll(1, -1);
        offset++;
      }
      else
      {
        offset = 0;
        curr_pos++;
      }
    }
  }
  else
  {
    needClear = true;
    curr_pos = 0;
  }
}

void tickProcedure()
{
  flipper.detach();

  if (canPrint)
  {
    isPrinting = true;
    showString();
    isPrinting = false;
  }

  flipper.attach(0.05, tickProcedure);
}

void printString()
{
  if (server.hasArg("str"))
  {
    canPrint = false;

    while(isPrinting) {}
    str = server.arg("str");
    curr_pos = offset = 0;
    needClear = true;

    canPrint = true;

    server.sendHeader("Location", "/text");
    server.send(301, "text/html", "");
  }
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

  server.on("/msg", HTTP_POST, printString);
  server.on("/edit", HTTP_DELETE, handleDelete);
  server.on("/edit", HTTP_PUT, handleCreate);
  server.on("/edit", HTTP_POST, []() { returnOK(); }, handleFileUpload);
  server.onNotFound(handleNotFound);

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");

  if (SD.begin(SS))
  {
    DBG_OUTPUT_PORT.println("SD card initialized.");
    hasSD = true;
  }

  flipper.attach(1, tickProcedure);
}

void loop()
{
  dnsServer.processNextRequest();
  server.handleClient();
}
