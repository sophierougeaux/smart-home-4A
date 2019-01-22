#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

///////////////Parameters/////////////////
// WIFI params
const char* ssid = "OnePlus 5";
const char* password = "USA441996";

// CSE params
//const char* host = "172.20.10.4"; //https://127.0.0.1
const char* host = "192.168.43.224";
const String Host = host;
const int httpPort = 8080;

// AE params
const int aePort   = 80;
String login = "admin";
String pwd = "admin";
String origin = "admin:admin";

// Servo params
int servoPin = D1;
int maxpos = 140;
int minpos = 40;

// Useful URLs
String url_incse = "http://" + Host + ":" + httpPort + "/~/in-cse"; //http://192.168.43.234:8080/~/in-cse
String url = "http://" + Host + ":" + httpPort + "/~/in-cse/in-name/MY_FACE/DATA/la";
String url_bo = "http://" + Host + ":" + httpPort + "/~/in-cse/in-name/MOTOR1/DATABO/la";
String url_so = "http://" + Host + ":" + httpPort + "/~/in-cse/in-name/MOTOR1/DATASO/la";
String url_nrv = "http://" + Host + ":" + httpPort + "/~/in-cse/in-name/MOTOR1/DATANRV/la";

// Useful variables
String rep = "";
String rep1 = "";
String rep2 = "";
String rep3 = "";

// Objects
ESP8266WiFiMulti WiFiMulti;
Servo myservo;


/*******************************************
********* Fonctions SEND GET **************
*******************************************/

String sendGETxml(String url) {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    http.begin(url.c_str());
    http.setAuthorization(login.c_str(), pwd.c_str());

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    http.addHeader(String("X-M2M-Origin"), origin);
    http.addHeader(String("Accept"), String("application/xml"));
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == 200) {
        String payload = http.getString();
        Serial.println(payload);
        return payload;
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}

/*******************************************
********* Fonctions SEND POST XML **************
*******************************************/

String sendPOSTxml(String url, String ty, String rep) {

  /*// prepare the HTTP request
    String req = String() + "POST " + url + " HTTP/1.1\r\n" +
               "Host: " + host + ":" + httpPort + "\r\n" +
               "X-M2M-Origin: " + origin + "\r\n" +
               "Content-Type: application/xml;ty=" + ty + "\r\n" +
               "Content-Length: " + rep.length() + "\r\n"
               "Connection: close\r\n\n" +
               rep;
    //Serial.println(".......... requête POST envoyée en xml : ...........");
    //Serial.println(req + "\n");*/

  if ((WiFiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    http.begin(url.c_str());
    http.setAuthorization(login.c_str(), pwd.c_str());

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header
    http.addHeader(String("X-M2M-Origin"), origin);
    http.addHeader(String("Content-Type"), String("application/xml;ty=" + ty));
    int httpCode = http.POST(rep.c_str());   //Send the request

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == 200) {
        String payload = http.getString();
        Serial.println(payload);
        return payload;
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();

  } else {
    Serial.println("Error in WiFi connection");
  }

}

/*******************************************
********* Fonction setup   **************
*******************************************/

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();

  /*// Configure pin for LED control
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, 0);*/

  // Configuration pin pour controler le moteur
  pinMode(servoPin, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(minpos);
  delay(1000);
  myservo.detach();

  // Configuration Wifi
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
  wl_status_t S = WiFiMulti.run();
  while (S != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    S = WiFiMulti.run();
  }
  Serial.println();
  Serial.println("WiFi connected");

  // Creation des ressources
  //////////////////////////////////////////////// MOTOR1 //////////////////////////////////////////////
  Serial.println("----------- Creation of ressources ------------");

  //sendGETxml(url_incse);

  //Create MOTOR1 application
  sendPOSTxml(url_incse, "2", "<m2m:ae xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"MOTOR1\" > <api>app-sensor</api> <lbl>Type/motor Category/servomotor Location/home</lbl> <rr>false</rr> </m2m:ae>");

  // Create DESCRIPTOR in MOTOR1
  sendPOSTxml(url_incse + "/in-name/MOTOR1", "3", "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DESCRIPTOR\"> </m2m:cnt>");  //String resultat2 = sendPOST(url_incse + "/in-name/MOTOR1", 3, "{\"rn\":\"DESCRIPTOR\"}");

  // Create DATABO,SO,NRV in MOTOR1
  sendPOSTxml(url_incse + "/in-name/MOTOR1", "3", "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DATABO\"> </m2m:cnt>");
  sendPOSTxml(url_incse + "/in-name/MOTOR1", "3", "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DATASO\"> </m2m:cnt>");
  sendPOSTxml(url_incse + "/in-name/MOTOR1", "3", "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DATANRV\"> </m2m:cnt>");

  // Create Content Instance in MOTOR1/DESCRIPTOR
  sendPOSTxml(url_incse + "/in-name/MOTOR1/DESCRIPTOR", "4", "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>application/xml</cnf> <con> &lt;obj&gt; &lt;str name=&quot;type&quot; val=&quot;motor&quot;/&gt; &lt;str name=&quot;Location&quot; val=&quot;Home&quot;/&gt; &lt;str name=&quot;appId&quot; val=&quot;MOTOR1&quot;/&gt; &lt;op name=&quot;getValue&quot; href=&quot;/in-cse/in-name/MOTOR1/DATA/la&quot; in=&quot;obix:Nil&quot; out=&quot;obix:Nil&quot; is=&quot;retrieve&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>");

  // Create Content Instance =0 in MOTOR1/DATABO,SO,NRV
  sendPOSTxml(url_incse + "/in-name/MOTOR1/DATABO", "4", "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>message</cnf> <con> &lt;obj&gt; &lt;str name=&quot;appId&quot; val=&quot;MOTOR1&quot;/&gt; &lt;str name=&quot;category&quot; val=&quot;Servomotor &quot;/&gt; &lt;int name=&quot;data&quot; val=&quot;0&quot;/&gt; &lt;int name=&quot;unit&quot; val=&quot;0:off,1:on&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>");
  sendPOSTxml(url_incse + "/in-name/MOTOR1/DATASO", "4", "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>message</cnf> <con> &lt;obj&gt; &lt;str name=&quot;appId&quot; val=&quot;MOTOR1&quot;/&gt; &lt;str name=&quot;category&quot; val=&quot;Servomotor &quot;/&gt; &lt;int name=&quot;data&quot; val=&quot;0&quot;/&gt; &lt;int name=&quot;unit&quot; val=&quot;0:off,1:on&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>");
  sendPOSTxml(url_incse + "/in-name/MOTOR1/DATANRV", "4", "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>message</cnf> <con> &lt;obj&gt; &lt;str name=&quot;appId&quot; val=&quot;MOTOR1&quot;/&gt; &lt;str name=&quot;category&quot; val=&quot;Servomotor &quot;/&gt; &lt;int name=&quot;data&quot; val=&quot;0&quot;/&gt; &lt;int name=&quot;unit&quot; val=&quot;0:off,1:on&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>");

  Serial.println("------------ Resources created ------------");
}



/*******************************************
********* Fonction Loop    **************
*******************************************/

// Checke régulièrement la valeur de MYFACE/DATA et de MOTOR1/DATAcorrespondant-à-myface dans om2m.
//Si elle est à BO,SO ou NRV, checke la valeur du moteur dans le MOTOR1/DATAcorrespondant et agit en conséquence.

void loop() {
  // Checke la dernière valeur de MY_FACE/DATA
  rep = sendGETxml(url);
  //Serial.println(rep);
  
  if (rep != NULL) {
    // si MY_FACE vaut SO, BO ou NRV (sourire, bouche ouverte ou énervé)
    
    if (rep.indexOf("data&quot; val=&quot;R") == -1) { // Not R
      
      if (rep.indexOf("data&quot; val=&quot;BO") != -1) { //BO detected
        Serial.println("BOOOO");
        // demander la valeur du moteur
        rep1 = sendGETxml(url_bo);
        
        if (rep1.indexOf("data&quot; val=&quot;1") != -1) {
          Serial.println(".......... Rotation Moteur ...........");
          //Faire bouger le moteur
          myservo.attach(servoPin);
          myservo.write(maxpos);
          delay(1500);
          myservo.write(minpos);
          delay(1500);
          myservo.detach();
        }
      }
      
      else if (rep.indexOf("data&quot; val=&quot;SO") != -1) { //SO détected
        Serial.println("SOOOO");
        // demander la valeur du moteur
        rep2 = sendGETxml(url_so);
       
        if (rep2.indexOf("data&quot; val=&quot;1") != -1) {
          Serial.println(".......... Rotation Moteur ...........");
          //Faire bouger le moteur
          myservo.attach(servoPin);
          myservo.write(180);
          delay(1000);
          myservo.write(0);
          delay(1000);
          myservo.detach();
        }
      }
      
      else if (rep.indexOf("data&quot; val=&quot;NRV") != -1) { // NRV detected
        Serial.println("NRVVVV");
        // demander la valeur du moteur
        rep3 = sendGETxml(url_nrv);
       
        if (rep3.indexOf("data&quot; val=&quot;1") != -1) {
          Serial.println(".......... Rotation Moteur ...........");
          //Faire bouger le moteur
          myservo.attach(servoPin);
          myservo.write(180);
          delay(1000);
          myservo.write(0);
          delay(1000);
          myservo.detach();
        }
      }
     
      
      else Serial.println("Valeur inscrite dans MY_FACE/DATA non conventionnelle");
    }
    /* rep = "";
      rep1 = "";
      rep2 = "";
      rep3 = "";*/
  }
  delay(2000);
}
