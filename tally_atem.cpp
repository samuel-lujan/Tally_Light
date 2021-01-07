#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

#include <Streaming.h>
#include <MemoryFree.h>
#include <SkaarhojPgmspace.h>
#include <ATEM.h>
#include <ATEMext.h>
#include <ATEMbase.h>

ATEMext AtemSwitcher;
IPAddress IP_ATEM(192, 168, 88, 50);  // IP do ATEM Switcher
const char camera_1[] = "192.168.88.61";
const char camera_2[] = "192.168.88.62";
const char camera_3[] = "192.168.88.63";
const char camera_4[] = "192.168.88.64";
const char camera_5[] = "192.168.88.65";
const char camera_6[] = "192.168.88.66";
const char camera_7[] = "192.168.88.67";
const char camera_8[] = "192.168.88.68";

byte ip[] = { 192, 168, 88, 60 };    

const char off_live[] = "/off_live";
const char off_preview[] = "/off_preview";
const char live[] = "/live";
const char preview[] = "/pgm";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

int pgm = 0;
int prev = 0;
int last_pgm =0;
int last_prev =0;

void setup(){
  Serial.begin(115200); 
  Ethernet.begin(mac, ip);
  AtemSwitcher.begin(IP_ATEM);
  AtemSwitcher.serialOutput(2);
  AtemSwitcher.connect();  
  
}
EthernetClient c;
void loop(){
  AtemSwitcher.runLoop();
  pgm = AtemSwitcher.getProgramInputVideoSource(0);
  prev = AtemSwitcher.getPreviewInputVideoSource(0); 
  /*Serial.print("PGM: ");
  Serial.print(pgm);
  Serial.print(" last pgm: ");
  Serial.println(last_pgm);

  Serial.print("Preview: ");
  Serial.print(prev);
  Serial.print(" last prev: ");
  Serial.println(last_prev);
  */
  if(pgm != last_pgm){ //O PGM FOI MUDADO DEVEMOS ATUALIZAR A TALLY
    send_live(pgm);
    send_not_live(last_pgm);
    last_pgm = pgm;
  }

  if(prev != last_prev){//O PREVIEW FOI MUDADO DEVEMOS ATUALIZAR A TALLY
    send_preview(prev);
    send_not_preview(last_prev);
    last_prev = prev;
  }
}

void send_not_live(int pgm){
  HttpClient http_not_live(c);
  if(pgm == 1){
    http_not_live.get(camera_1, off_live);
  }
  else if(pgm == 2){
    http_not_live.get(camera_2, off_live);
  }
  else if(pgm == 3){
    http_not_live.get(camera_3, off_live);
  }
  else if(pgm == 4){
    http_not_live.get(camera_4, off_live);
  }
  else if(pgm == 5){
    http_not_live.get(camera_5, off_live);
  }
  else if(pgm == 6){
    http_not_live.get(camera_6, off_live);
  }
  else if(pgm == 7){
    http_not_live.get(camera_7, off_live);
  }
  else if(pgm == 8){
    http_not_live.get(camera_8, off_live);
  }
  else{
  }
  http_not_live.stop();
}



void send_live(int pgm){
  HttpClient http_live(c);
  if(pgm == 1){
    http_live.get(camera_1, live);
  }
  else if(pgm == 2){
    http_live.get(camera_2, live);
  }
  else if(pgm == 3){
    http_live.get(camera_3, live);
  }
  else if(pgm == 4){
    http_live.get(camera_4, live);
  }
  else if(pgm == 5){
    http_live.get(camera_5, live);
  }
  else if(pgm == 6){
    http_live.get(camera_6, live);
  }
  else if(pgm == 7){
    http_live.get(camera_7, live);
  }
  else if(pgm == 8){
    http_live.get(camera_8, live);
  }
  else{
  }
  http_live.stop();
}

void send_preview(int prev){
  HttpClient http_preview(c);
  if(prev == 1){
    http_preview.get(camera_1, preview);
  }
  else if(prev == 2){
    http_preview.get(camera_2, preview);
  }
  else if(prev == 3){
    http_preview.get(camera_3, preview);
  }
  else if(prev == 4){
    http_preview.get(camera_4, preview);
  }
  else if(prev == 5){
    http_preview.get(camera_5, preview);
  }
  else if(prev == 6){
    http_preview.get(camera_6, preview);
  }
  else if(prev == 7){
    http_preview.get(camera_7, preview);
  }
  else if(prev == 8){
    http_preview.get(camera_8, preview);
  }
  else{
  }
  http_preview.stop();
}

void send_not_preview(int prev){
  HttpClient http_not_preview(c);
  if(prev == 1){
    http_not_preview.get(camera_1, off_preview);
  }
  else if(prev == 2){
    http_not_preview.get(camera_2, off_preview);
  }
  else if(prev == 3){
    http_not_preview.get(camera_3, off_preview);
  }
  else if(prev == 4){
    http_not_preview.get(camera_4, off_preview);
  }
  else if(prev == 5){
    http_not_preview.get(camera_5, off_preview);
  }
  else if(prev == 6){
    http_not_preview.get(camera_6, off_preview);
  }
  else if(prev == 7){
    http_not_preview.get(camera_7, off_preview);
  }
  else if(prev == 8){
    http_not_preview.get(camera_8, off_preview);
  }
  else{
  }
  http_not_preview.stop();
}
