#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "123";
const char* password = "12345678";

// Configuración de la IP estática.
IPAddress local_IP(192, 168, 46, 3);
IPAddress gateway(192, 168, 46, 224);
IPAddress subnet(255, 255, 255, 0); 

WiFiServer server(80);

const int ledD0 = 16; //D0 //garage
const int ledD1 = 5; //D1  //
const int ledD2 = 4; //D2
const int ledD4 = 13; //D4

const int sM1 = 0; // D3
const int sM2 = 2; // D4

const int buzzer = 15; // D8

int pulsador3;
int pulsador4;
String estado3;
String estado4;
String respuesta = "0,0";



Servo servoPorton;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); // Configurar el pin del LED_BUILTIN como salida
  pinMode(ledD0,OUTPUT);
  pinMode(ledD1,OUTPUT);
  pinMode(ledD2,OUTPUT);
  pinMode(ledD4,OUTPUT);
  pinMode(sM1, INPUT);
  pinMode(sM2, INPUT);
  pinMode(buzzer, OUTPUT);
  servoPorton.attach(12);
  servoPorton.write(140);
  delay(1000);
  
  
  // Establecimiento de la IP estática.
  WiFi.config(local_IP, gateway, subnet);

  // Conecta a la red wifi.
  Serial.println();
  Serial.print("Conectando con ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado con WiFi.");

  // Inicio del Servidor web.
  server.begin();
  Serial.println("Servidor web iniciado.");

  // Esta es la IP
  Serial.print("Esta es la IP para conectar: ");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Consulta si se ha conectado algún cliente.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.print("Nuevo cliente: ");
  Serial.println(client.remoteIP());

  // Espera hasta que el cliente envíe datos.
  while (!client.available()) {
    delay(1);
  }

  /////////////////////////////////////////////////////
  // Lee la información enviada por el cliente.
  String req = client.readStringUntil('\r');
  Serial.println(req);

  // Realiza la petición del cliente.
  if (req.indexOf("on1") != -1) {
    //digitalWrite(LED_BUILTIN, LOW); // Enciende el LED interno
    digitalWrite(ledD0,HIGH);
    client.println("Foco del garage encendido");
  }
  if (req.indexOf("off1") != -1) {
    //digitalWrite(LED_BUILTIN, HIGH); // Apaga el LED interno
    digitalWrite(ledD0,LOW);
    client.println("Foco del garage apagado");
  }
  if (req.indexOf("on2") != -1) {
    //digitalWrite(LED_BUILTIN, LOW); // Enciende el LED interno
    digitalWrite(ledD1,HIGH);
    client.println("Foco del dormitorio encendido");
  }
  if (req.indexOf("off2") != -1) {
    //digitalWrite(LED_BUILTIN, HIGH); // Apaga el LED interno
    digitalWrite(ledD1,LOW);
    client.println("Foco del dormitorio apagado");
  }
  if (req.indexOf("on3") != -1) {
    //digitalWrite(LED_BUILTIN, LOW); // Enciende el LED interno
    digitalWrite(ledD2,HIGH);
    client.println("Foco de la sala encendido");
  }
  if (req.indexOf("off3") != -1) {
    //digitalWrite(LED_BUILTIN, HIGH); // Apaga el LED interno
    digitalWrite(ledD2,LOW);
    client.println("Foco de la sala apagado");
  }
  if (req.indexOf("on4") != -1) {
    //digitalWrite(LED_BUILTIN, LOW); // Enciende el LED interno
    digitalWrite(ledD4,HIGH);
    client.println("Foco de las escaleras encendido");
  }
  if (req.indexOf("off4") != -1) {
   // digitalWrite(LED_BUILTIN, HIGH); // Apaga el LED interno
    digitalWrite(ledD4,LOW);
    client.println("Foco de las escaleras apagado");
  }
  if (req.indexOf("offP") != -1) {
   // digitalWrite(LED_BUILTIN, HIGH); // Apaga el LED interno
    servoPorton.write(150);
    delay(1000);
    digitalWrite(ledD0,LOW);
    client.println("Portón abierto");
  }
  if (req.indexOf("onP") != -1) {
   // digitalWrite(LED_BUILTIN, HIGH); // Apaga el LED interno
    digitalWrite(ledD0,HIGH);
    servoPorton.write(20);
    delay(1000);
    
    client.println("Portón cerrado");
  }

  if (req.indexOf("estado") != -1){consulta();}

  //////////////////////////////////////////////
  // Página WEB. ////////////////////////////
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  Importante.

  Serial.print("Cliente desconectado: ");
  Serial.println(client.remoteIP());
  client.flush();
  client.stop();
}

void consulta(){
 pulsador3 = digitalRead(sM1);
   if (pulsador3 == HIGH) {
     estado3 = "Puerta 1 abierta";
     digitalWrite(buzzer, HIGH);
   } else {
     estado3 = "Puerta 1 cerrada";
     digitalWrite(buzzer, LOW);
   }
 /*pulsador4 = digitalRead(sM2);
   if (pulsador4 == HIGH) {
     estado4 = "Puerta 2 abierta";
     digitalWrite(buzzer, HIGH);
   } else {
     estado4 = "Puerta 2 cerrada";
     digitalWrite(buzzer, LOW);
   }*/
  
 respuesta = estado3 + "," + estado4;
 Serial.println(respuesta);
 delay(10);
}