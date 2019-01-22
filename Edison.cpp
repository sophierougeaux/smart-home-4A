#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

#include <mraa/aio.h>
#include <mraa/gpio.h>
#include <curl/curl.h>

using namespace std;

// Affectation des pin
const int LED_PIN = 2;
//const int LED_PIN2 = 4;
const int BUTTON_PIN = 3;
const int FAN_PIN = 4;

// Définition des adresses url et des corps des requêtes
//string url = "http://192.168.43.234:8080/~/in-cse";
//string url = "http://172.20.10.4:8080/~/in-cse";
string url = "http://192.168.43.224:8080/~/in-cse";

string url_myface_la = url + "/in-name/MY_FACE/DATA/la";
string url_fan1bo_la = url + "/in-name/FAN1/DATABO/la";
string url_fan1so_la = url + "/in-name/FAN1/DATASO/la";
string url_fan1nrv_la = url + "/in-name/FAN1/DATANRV/la";
string url_led1bo_la = url + "/in-name/LED1/DATABO/la";
string url_led1so_la = url + "/in-name/LED1/DATASO/la";
string url_led1nrv_la = url + "/in-name/LED1/DATANRV/la";

string url_myface = url + "/in-name/MY_FACE/";
string url_myface_descriptor = url + "/in-name/MY_FACE/DESCRIPTOR";
string url_myface_data = url + "/in-name/MY_FACE/DATA";

string url_fan1 = url + "/in-name/FAN1";
string url_fan1_descriptor = url + "/in-name/FAN1/DESCRIPTOR";
string url_fan1_databo = url + "/in-name/FAN1/DATABO";
string url_fan1_dataso = url + "/in-name/FAN1/DATASO";
string url_fan1_datanrv = url + "/in-name/FAN1/DATANRV";

string url_led1 = url + "/in-name/LED1";
string url_led1_descriptor = url + "/in-name/LED1/DESCRIPTOR";
string url_led1_databo = url + "/in-name/LED1/DATABO";
string url_led1_dataso = url + "/in-name/LED1/DATASO";
string url_led1_datanrv = url + "/in-name/LED1/DATANRV";

string body_descriptor = "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DESCRIPTOR\"> </m2m:cnt>";
string body_data = "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DATA\"> </m2m:cnt>";
string body_databo = "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DATABO\"> </m2m:cnt>";
string body_dataso = "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DATASO\"> </m2m:cnt>";
string body_datanrv = "<m2m:cnt xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"DATANRV\"> </m2m:cnt>";

string body_led1 = "<m2m:ae xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"LED1\"> <api>app-sensor</api> <lbl>Type/light Category/led Location/home</lbl> <rr>false</rr> </m2m:ae>";
string body_led1_descriptorCI = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>application/xml</cnf> <con> &lt;obj&gt;&lt;str name=&quot;type&quot; val=&quot;Led&quot;/&gt; &lt;str name=&quot;location&quot; val=&quot;Home&quot;/&gt; &lt;str name=&quot;appId&quot; val=&quot;LED1&quot;/&gt; &lt;op name=&quot;getValue&quot; href=&quot;/in-cse/in-name/LED1/DATA/la&quot; in=&quot;obix:Nil&quot; out=&quot;obix:Nil&quot; is=&quot;retrieve&quot;/&gt; &lt;/obj&gt;</con> </m2m:cin>";
string body_led1_dataCI0 = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>message</cnf> <con>&lt;obj&gt; &lt;str name=&quot;appId&quot; val=&quot;LED1&quot;/&gt; &lt;str name=&quot;category&quot; val=&quot;led&quot;/&gt; &lt;int name=&quot;data&quot; val=&quot;0&quot;/&gt; &lt;int name=&quot;unit&quot; val=&quot;1:allumer&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>";

string body_fan1 = "<m2m:ae xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"FAN1\"> <api>app-sensor</api> <lbl>Type/motor Category/fan Location/home</lbl> <rr>false</rr> </m2m:ae>";
string body_fan1_descriptorCI = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>application/xml</cnf> <con> &lt;obj&gt;&lt;str name=&quot;type&quot; val=&quot;Fan&quot;/&gt; &lt;str name=&quot;location&quot; val=&quot;Home&quot;/&gt; &lt;str name=&quot;appId&quot; val=&quot;FAN1&quot;/&gt; &lt;op name=&quot;getValue&quot; href=&quot;/in-cse/in-name/FAN1/DATA/la&quot; in=&quot;obix:Nil&quot; out=&quot;obix:Nil&quot; is=&quot;retrieve&quot;/&gt; &lt;/obj&gt;</con> </m2m:cin>";
string body_fan1_dataCI0 = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>message</cnf> <con>&lt;obj&gt; &lt;str name=&quot;appId&quot; val=&quot;FAN1&quot;/&gt; &lt;str name=&quot;category&quot; val=&quot;fan&quot;/&gt; &lt;int name=&quot;data&quot; val=&quot;0&quot;/&gt; &lt;int name=&quot;unit&quot; val=&quot;0:off,1:on,2:R&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>";

string body_myface = "<m2m:ae xmlns:m2m=\"http://www.onem2m.org/xml/protocols\" rn=\"MY_FACE\"> <api>app-sensor</api> <lbl>Type/Kinect_Sensor Category/camera Location/home</lbl> <rr>false</rr> </m2m:ae>";
string body_myface_descriptorCI = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>application/xml</cnf> <con> &lt;obj&gt;&lt;str name=&quot;type&quot; val=&quot;Kinect_Sensor&quot;/&gt; &lt;str name=&quot;location&quot; val=&quot;Home&quot;/&gt; &lt;str name=&quot;appId&quot; val=&quot;MY_FACE&quot;/&gt; &lt;op name=&quot;getValue&quot; href=&quot;/in-cse/in-name/MY_FACE/DATA/la&quot; in=&quot;obix:Nil&quot; out=&quot;obix:Nil&quot; is=&quot;retrieve&quot;/&gt; &lt;/obj&gt;</con> </m2m:cin>";
string body_myface_dataCIR = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>message</cnf> <con>&lt;obj&gt; &lt;str name=&quot;appId&quot; val=&quot;MY_FACE&quot;/&gt; &lt;str name=&quot;category&quot; val=&quot;Kinect &quot;/&gt; &lt;int name=&quot;data&quot; val=&quot;R&quot;/&gt; &lt;str name=&quot;unit&quot; val=&quot;BO,SO,NRV&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>";/*
string body_myface_dataCIBO = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>message</cnf> <con>&lt;obj&gt; &lt;str name=&quot;appId&quot; val=&quot;MY_FACE&quot;/&gt; &lt;str name=&quot;category&quot; val=&quot;Kinect &quot;/&gt; &lt;int name=&quot;data&quot; val=&quot;BO&quot;/&gt; &lt;str name=&quot;unit&quot; val=&quot;BO,SO,NRV&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>";
string body_myface_dataCISO = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>message</cnf> <con>&lt;obj&gt; &lt;str name=&quot;appId&quot; val=&quot;MY_FACE&quot;/&gt; &lt;str name=&quot;category&quot; val=&quot;Kinect &quot;/&gt; &lt;int name=&quot;data&quot; val=&quot;SO&quot;/&gt; &lt;str name=&quot;unit&quot; val=&quot;BO,SO,NRV&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>";
string body_dataCINRV = "<m2m:cin xmlns:m2m=\"http://www.onem2m.org/xml/protocols\"> <cnf>message</cnf> <con>&lt;obj&gt; &lt;str name=&quot;appId&quot; val=&quot;MY_FACE&quot;/&gt; &lt;str name=&quot;category&quot; val=&quot;Kinect &quot;/&gt; &lt;int name=&quot;data&quot; val=&quot;NVR&quot;/&gt; &lt;str name=&quot;unit&quot; val=&quot;BO,SO,NRV&quot;/&gt; &lt;/obj&gt; </con> </m2m:cin>";*/



////////////////////////////////////////////////////////////////////////
//////////////////////// FONCTION POST /////////////////////////////////
////////////////////////////////////////////////////////////////////////

int sendPost(string url, string ty, string body){
  CURL *curl;
  CURLcode res;
  struct curl_slist *list = NULL; //list of headers
  
  // get a curl handle
  curl = curl_easy_init();
  
  if(curl) {
    // URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // METHOD
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");

    // HEADERS
    list = curl_slist_append(list, "X-M2M-Origin: admin:admin");
    string h = "Content-type: application/xml;ty=";
    h = h + ty;
    list = curl_slist_append(list,h.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    //DATA
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    // SEND REQUEST
    res = curl_easy_perform(curl);

    
    curl_slist_free_all(list); // free the list again
  }
  
  if(res != CURLE_OK){
    cout<<"curl_easy_perform() failed"<<endl;
    return 1;
  }

  curl_easy_cleanup(curl);

  return 0;  
}


////////////////////////////////////////////////////////////////////////
//////////////////////// FONCTION GET //////////////////////////////////
////////////////////////////////////////////////////////////////////////

// Structure to store the response from server
struct MemoryStruct {
  char *memory;
  size_t size;
};
static size_t
// Function to store the response from server
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    // out of memory!  
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}


////////////////// GET

char * sendGet(string url){
  
  CURL *curl;
  CURLcode res;

  struct MemoryStruct chunk;
 
  chunk.memory = malloc(1);  // will be grown as needed by the realloc above
  chunk.size = 0;    //no data at this point

  struct curl_slist *list = NULL; //list of headers
  
  // get a curl handle 
  curl = curl_easy_init();
  
  if(curl) {
    // URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // HEADERS
    list = curl_slist_append(list, "X-M2M-Origin: admin:admin");
    list = curl_slist_append(list, "Accept: application/xml");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

    // PREPARE TO STORE RESPONSE
    // send all data to this function  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    // we pass our 'chunk' struct to the callback function 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    
    // SEND REQUEST
    res = curl_easy_perform(curl);
    
    curl_slist_free_all(list); // free the list again 
  }
  
    // Check for errors 
  if(res != CURLE_OK){
    cout<<"curl_easy_perform() failed"<<endl;
  }
  // Return response from server
    return chunk.memory;    
    //printf("%d bytes retrieved\n", chunk.size);
    // printf(chunk.memory);
  
  // always cleanup 
  curl_easy_cleanup(curl);
  free(chunk.memory);
}


////////////////////////////////////////////////////////////////////////
//////////////////////// FONCTION MAIN /////////////////////////////////
////////////////////////////////////////////////////////////////////////

int main(){
  
  // Variables
  mraa_gpio_context gpio_button, gpio_led, gpio_fan;
  int buttonvalue = 0;
  char * response = NULL;
  char * fan = NULL;
  char * led  = NULL;
  char * keyword0 = NULL;
  char * keyword1 = NULL;
  char * keyword2 = NULL;
  char * keyword3 = NULL;
 
  // Led and button Initialisation
  mraa_init();
  gpio_button = mraa_gpio_init(BUTTON_PIN);
  gpio_fan = mraa_gpio_init(FAN_PIN);
  gpio_led = mraa_gpio_init(LED_PIN);
  mraa_gpio_dir(gpio_button, MRAA_GPIO_IN);
  mraa_gpio_dir(gpio_led, MRAA_GPIO_OUT);
  mraa_gpio_dir(gpio_fan, MRAA_GPIO_OUT);
  mraa_gpio_write(gpio_led, 0);
  mraa_gpio_write(gpio_fan, 0);

  
  // Obligatoire
  curl_global_init(CURL_GLOBAL_ALL);

  // Creation des ressources OM2M
  cout<<"Creation resources"<<endl;
  sendPost(url, "2", body_myface);
  sendPost(url_myface, "3", body_data);
  sendPost(url_myface, "3", body_descriptor);
  sendPost(url_myface_descriptor, "4", body_myface_descriptorCI);
  sendPost(url_myface_data,"4", body_myface_dataCIR);
  
  sendPost(url, "2", body_led1);
  sendPost(url_led1, "3", body_databo);
  sendPost(url_led1, "3", body_dataso);
  sendPost(url_led1, "3", body_datanrv);
  sendPost(url_led1, "3", body_descriptor);
  sendPost(url_led1_descriptor, "4", body_led1_descriptorCI);
  sendPost(url_led1_databo,"4", body_led1_dataCI0);
  sendPost(url_led1_dataso,"4", body_led1_dataCI0);
  sendPost(url_led1_datanrv,"4", body_led1_dataCI0);

  sendPost(url, "2", body_fan1);
  sendPost(url_fan1, "3", body_databo);
  sendPost(url_fan1, "3", body_dataso);
  sendPost(url_fan1, "3", body_datanrv);
  sendPost(url_fan1, "3", body_descriptor);
  sendPost(url_fan1_descriptor, "4", body_fan1_descriptorCI);
  sendPost(url_fan1_databo,"4", body_fan1_dataCI0);
  sendPost(url_fan1_dataso,"4", body_fan1_dataCI0);
  sendPost(url_fan1_datanrv,"4", body_fan1_dataCI0);
  cout<<"Resouces ok"<<endl;
  
  /////////////////////////////// LOOP ////////////////////////////

  while(1){
    cout<<"while"<<endl;
    
    // Si bouton appuye, éteindre ventilateur
    buttonvalue = mraa_gpio_read(gpio_button);
    
    if (buttonvalue) {
      cout<<"Button Pushed ---> Fan Off"<<endl;
      mraa_gpio_write(gpio_fan, 0);
      }

    // Récupération valeur visage
    response = sendGet(url_myface_la);

    // String à chercher : 
    keyword0 = strstr (response, "data&quot; val=&quot;BO");
    keyword1 = strstr (response, "data&quot; val=&quot;SO");
    keyword2 = strstr (response, "data&quot; val=&quot;NRV");
    keyword3 = strstr (response, "data&quot; val=&quot;R");

    if (keyword3 == NULL){
      
      if (keyword0 != NULL){ // Bouche ouverte détectée
	cout<<"Bouche Ouverte"<<endl;
	fan = sendGet(url_fan1bo_la);
	led = sendGet(url_led1bo_la);

	// Gestion ventilateur
	if (strstr(fan, "data&quot; val=&quot;2") == NULL){
	  if (strstr(fan, "data&quot; val=&quot;0") != NULL){
	    cout<<"fan off"<<endl;
	    mraa_gpio_write(gpio_fan, 0);
	  } 
	  else if (strstr(fan, "data&quot; val=&quot;1") != NULL){
	    cout<<"fan on"<<endl;
	    mraa_gpio_write(gpio_fan, 1);
	  }
	}

	// Gestion led
	if (strstr(led, "data&quot; val=&quot;1") != NULL){
	  // allumer led pendant deux secondes puis remettre à 0
	  mraa_gpio_write(gpio_led, 1);
	  sleep(2);
	  mraa_gpio_write(gpio_led, 0);
	}
      }
    
      else if (keyword1 != NULL){ // Sourire détecté
	cout<<"Sourire"<<endl;
	fan = sendGet(url_fan1so_la);
	led = sendGet(url_led1so_la);

	// Gestion ventilateur
	if (strstr(fan, "data&quot; val=&quot;2") == NULL){
	  if (strstr(fan, "data&quot; val=&quot;0") != NULL){
	    cout<<"fan off"<<endl;
	    mraa_gpio_write(gpio_fan, 0);
	  } 
	  else if (strstr(fan, "data&quot; val=&quot;1") != NULL){
	    cout<<"fan on"<<endl;
	    mraa_gpio_write(gpio_fan, 1);
	  }
	}

	// Gestion led
	if (strstr(led, "data&quot; val=&quot;1") != NULL){
	  // allumer led pendant deux secondes puis remettre à 0
	  mraa_gpio_write(gpio_led, 1);
	  sleep(2);
	  mraa_gpio_write(gpio_led, 0);
	}
      }
    
      else if (keyword2 != NULL){ // Sourcils froncés détecté
	cout<<"Sourcils froncés"<<endl;
	fan = sendGet(url_fan1nrv_la);
	led = sendGet(url_led1nrv_la);

	// Gestion ventilateur
	if (strstr(fan, "data&quot; val=&quot;2") == NULL){
	  if (strstr(fan, "data&quot; val=&quot;0") != NULL){
	    cout<<"fan off"<<endl;
	    mraa_gpio_write(gpio_fan, 0);
	  } 
	  else if (strstr(fan, "data&quot; val=&quot;1") != NULL){
	    cout<<"fan on"<<endl;
	    mraa_gpio_write(gpio_fan, 1);
	  }
	}

	// Gestion led
	if (strstr(led, "data&quot; val=&quot;1") != NULL){
	  // allumer led pendant deux secondes puis remettre à 0
	  mraa_gpio_write(gpio_led, 1);
	  sleep(2);
	  mraa_gpio_write(gpio_led, 0);
	}
      }
      
      else {cout<<"Mauvais CI ecrit dans MY_FACE/DATA"<<endl;}
      
      // RAZ
      fan = NULL;
      led = NULL;
      response = NULL;
      keyword0 = NULL;
      keyword1 = NULL;
      keyword2 = NULL;
      keyword3 = NULL;      
    }
    
    sleep(2);
  }
  

 
  // fin (code jamais atteint, ecrit pour la forme si amélioration du code)
  mraa_gpio_close(gpio_button);
  mraa_gpio_close(gpio_led);
  mraa_gpio_close(gpio_fan);
  curl_global_cleanup();
  
  return(0);
}
