/*
*   LIBRARIES
*       IF YOU DON'T HAVE THIS LIBS INSTALLED IN YOUR COMPUTER, PLEASE **INSTALL IT**
*       The link for this libs are in the main readme.md 
*/
    #include <ESP8266WiFi.h> //Used for ESP8266 Board connect to some ssid 
    #include <WiFiManager.h> //Used for manage your ssid's and passwords
    #include <ESP8266HTTPClient.h> //Used to make and recieve http requests
    #include <EEPROM.h>

//initializing the libs
    WiFiManager wifiManager; //Manager of SSID's
    HTTPClient http;         //HTTP Requests

//Creating variables who WifiManager will use to connect HTTPClient 
    String ssid = " ";      //PLEASE DON'T CHANGE IT 
    String password = " ";  //PLEASE DON'T CHANGE IT

//PINS USED 
//    #define LED_RED D2
//  #define LED_GREEN D3

    #define LED_RED D6
    #define LED_GREEN D7

//CREATE A SERVER IN YOUR ESP8266 BOARD
    WiFiServer server(80); //IF YOU HAVE PROBLEMS USE ANOTHER PORT LIKE 8082 OR 8089 || AND WHEN CALL IT, DO IT LIKE ip_address:PORT 

//SETS THE FIRST IP ADDRESS, GATEWAY AND SUBNET
    IPAddress ip(192,168,88,51); //The first ip_address of your ESP8266, when he don't find an linked camera
    IPAddress gateway(192,168,88,51);
    IPAddress subnet(255,255,255,0);

//SET CAMERA IP's
  //if you want can set the ip address of the tally's here
    IPAddress cam1(192,168,88,61);
    IPAddress cam2(192,168,88,62);
    IPAddress cam3(192,168,88,63);
    IPAddress cam4(192,168,88,64);
    IPAddress cam5(192,168,88,65);
    IPAddress cam6(192,168,88,66);
    IPAddress cam7(192,168,88,67);
    IPAddress cam8(192,168,88,68);
    

//CONTROLLER VARIABLES
    int cam=0;          //LINKED CAMERA ON ATEM
    int verified = 0;   //USED TO KNOW IF THE LED HAS ALREADY CHANGED 

/*
*   void SETUP 
*   arduino required 
*/
void setup(){
  EEPROM.begin(4);//Inicia a EEPROM com tamanho de 4 Bytes (minimo).
  pinMode(LED_RED, OUTPUT);     //SET THE LED PORT AS AN OUTPUT
  pinMode(LED_GREEN, OUTPUT);   //SET THE LED PORT AS AN OUTPUT
  Serial.begin(115200);         //START SERIAL PORT (USED TO PRINT SOME MESSAGE IN PROMP OR ARDUINO TERMINAL)

    //CREATE AN WIFI NETWORK IF DON'T FIND ANY KNOWN
        wifiManager.autoConnect("Tally Light"); //Create the network called Tally Light 1
        /*
        * IF YOU SEE THIS NETWORK CONNECT ON IT GO TO THE IP ADDRESS SET UPWARD
        * select your network put your SSID and the PASSWORD
        * do it like on the following repository
        * https://github.com/tzapu/WiFiManager
        */
        
        //Save the SSID and the PASSWORD
        ssid = wifiManager.getSSID();
        password = wifiManager.getPassword(); 

    //TRY TO CONNECT ON THE NEW NETWORK
    WiFi.begin(ssid, password);
    WiFi.config(ip, gateway, subnet); //set the default IP ADDRESS
    while (WiFi.status() != WL_CONNECTED){//do it while don't get connected
        delay(500);
        Serial.print("."); //will print some . in your console
    }

    //START THE SERVER IN THE PORT DEFINED UPWARD
    server.begin(); 
    verified =0;
    Serial.println(WiFi.localIP()); //Print the IP ADDRESS where your ESP8266 GET CONNECTED
}


/*
*   void LOOP 
*   arduino required 
*/
void loop(){
    if(WiFi.localIP()!=ip && verified == 1){//IF IT NOT HAVE SOME CAM SET
      WiFiClient client = server.available(); //TRY TO GET SOME MESSAGE FROM THE MASTER SERVER
      if(client){
        Serial.println("We have a new connection!");
        while(!client.available()){ //WAITING FOR THE MESSAGE 
          delay(1); 
        }
        String request = client.readStringUntil('\r'); //READ THE MESSAGE UNTIL THE END OF THE FIRST LINE
        Serial.println(request); //PRINT THE REQUEST AT YOUR PROMPT
        client.flush(); //CLEAR THE MESSAGE BUFFER  
        if(request == "GET /live HTTP/1.1"){
            client.println("HTTP/1.1 200 OK");
            client.stop();
            digitalWrite(LED_RED, HIGH);  
            digitalWrite(LED_GREEN, LOW);
        }else if(request == "GET /pgm HTTP/1.1"){
            client.println("HTTP/1.1 200 OK");
            client.stop();
            digitalWrite(LED_GREEN, HIGH);
        }else if(request == "GET /off_live HTTP/1.1"){
            client.println("HTTP/1.1 200 OK");
            client.stop();
            digitalWrite(LED_RED, LOW); 
        }else if(request == "GET /off_preview HTTP/1.1"){
            client.println("HTTP/1.1 200 OK");
            client.stop();
            digitalWrite(LED_GREEN, LOW);
        }else if(request == "GET /resetTally HTTP/1.1"){
            verified = 0;
            cam = 0;
            EEPROM.write(0, 0);
            EEPROM.commit();
            WiFi.disconnect();
            WiFi.begin(ssid, password);
            WiFi.config(ip, gateway, subnet); 
        }else{
            client.println("HTTP/1.1 200 OK");
            client.stop();
            digitalWrite(LED_GREEN, LOW);
            digitalWrite(LED_RED, LOW);
        }
      }
      return ;
      
    }else if(EEPROM.read(0) >0 && EEPROM.read(0) <= 8 ){
      Serial.println("Configurado");
      cam = EEPROM.read(0);
      WiFi.disconnect();
      WiFi.begin(ssid, password);
      if(cam == 1){
         WiFi.config(cam1, gateway, subnet); 
      }
      else if(cam == 2){
         WiFi.config(cam2, gateway, subnet);  
      }
      else if(cam == 3){
         WiFi.config(cam3, gateway, subnet);  
      }
      else if(cam == 4){
         WiFi.config(cam4, gateway, subnet);  
      }
      else if(cam == 5){
         WiFi.config(cam5, gateway, subnet);  
      }
      else if(cam == 6){
         WiFi.config(cam6, gateway, subnet);  
      }
      else if(cam == 7){
         WiFi.config(cam7, gateway, subnet);  
      }
      else if(cam == 8){
         WiFi.config(cam8, gateway, subnet);  
      }else{
        WiFi.config(ip, gateway, subnet); 
      }
      while (WiFi.status() != WL_CONNECTED){
        delay(500);
          Serial.print(".");
      }
      verified=1;
      server.begin();
      Serial.println(WiFi.localIP());
    }
    else{
     wait_request(); //TRY TO GET SOME REQUEST
    }

}

void wait_request(){
    Serial.println(WiFi.localIP());
    WiFiClient client = server.available(); //TRY TO GET SOME MESSAGE FROM THE MASTER SERVER
    if(client){
      Serial.println("We have a new connection!");
      while(!client.available()){ //WAITING FOR THE MESSAGE 
        delay(1); 
      }
      String request = client.readStringUntil('\r'); //READ THE MESSAGE UNTIL THE END OF THE FIRST LINE
      Serial.println(request); //PRINT THE REQUEST AT YOUR PROMPT
      client.flush(); //CLEAR THE MESSAGE BUFFER
      if(request == "GET / HTTP/1.1"){ 
          client.println("HTTP/1.1 200 OK"); //WRITE THE CALLBACK TO THE SERVER CLIENT WHO SENT THE REQUEST
          client.println("Content-Type: text/html"); //WRITE TO THE CLIENT THE CALLBACK MESSAGE
          client.println("");
          client.println("<!DOCTYPE HTML>"); 
          client.println("<html>"); 
          client.println("<h1><center>Hello! Welcome to <br>Atem Television Studio Tally Light v1.0</center></h1>");
          client.println("<form action='/setIP'>");
          client.println("<center><label for='camera'>Camera num:</label></center>");
          client.println("<center><font size='5'><input type='number' min='1' max='8' id='camera' name='id'></center>"); 
          client.println("<center><button type='submit'>Set Camera</button></center>");
          client.println("<center>Powered by Samuel Lujan</center>");
          client.println("</form>");
          client.println("</html>");
          delay(1); //INTERVALO DE 1 MILISEGUNDO
          Serial.println("Client Disconnected");
          Serial.println("");    
          return;    
      }
      else if(request == "GET /setIP?id=1 HTTP/1.1"){
        cam = 1;
      }
      else if(request == "GET /setIP?id=2 HTTP/1.1"){
        cam = 2;
      }
      else if(request == "GET /setIP?id=3 HTTP/1.1"){
        cam = 3;
      }
      else if(request == "GET /setIP?id=4 HTTP/1.1"){
        cam = 4;
      }
      else if(request == "GET /setIP?id=5 HTTP/1.1"){
        cam = 5;
      }
      else if(request == "GET /setIP?id=6 HTTP/1.1"){
        cam = 6;
      }
      else if(request == "GET /setIP?id=7 HTTP/1.1"){
        cam = 7;
      }
      else if(request == "GET /setIP?id=8 HTTP/1.1"){
        cam = 8;
      }
      EEPROM.write(0, cam);
      EEPROM.commit();
      client.println("HTTP/1.1 200 OK"); //WRITE THE CALLBACK TO THE SERVER CLIENT WHO SENT THE REQUEST
      client.println("Content-Type: text/html"); //WRITE TO THE CLIENT THE CALLBACK MESSAGE
      client.println("");
      client.println("<!DOCTYPE HTML>"); 
      client.println("<html>"); 
      client.println("<h1><center>See you soon! :)</center></h1>");
      client.print("<center><p>This tally was completely configured, as tally number:</p></center>");
      client.print("<center>");
      client.println(cam);
      client.print("</center>");
      client.println("<center>Powered by Samuel Lujan</center>");
      client.println("</html>");
      delay(1); //INTERVALO DE 1 MILISEGUNDO
      Serial.println("Client Disconnected");
      Serial.println("");        
      return; 
    }else{
      return;
    }
}
