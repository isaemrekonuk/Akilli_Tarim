/*********************************************
*             Proje Adı : Akıllı Tarım
*    Projeyi Hazırlayan : İsa Emre Konuk
* Projeyi Hazırlayan No : G221210306
*********************************************/

#define BLYNK_TEMPLATE_ID "TMPL67VqJGpuG"  //Blynk platformunda oluşan id'miz
#define BLYNK_TEMPLATE_NAME "Projee"  //Blynk platformunda oluşturduğumuz projenin adı
#define BLYNK_PRINT Serial 

 //Gerekli kütüphanelerimizi ekliyoruz.
#include <BlynkSimpleEsp8266.h> 
#include <ESP8266WiFi.h> 
#include "ThingSpeak.h" 

char token[] = "QVmRk4fVUyN5I5nEXcEqeoSO7vAGQl-5";  //Blynk proje token'ı

#define WLAN_SSID  "Emre"      //WiFi ağ adı
#define WLAN_PASSWORD  "0123456789"  //WiFi ağ şifresi

/* ThingSpeak Kurulumu */
const char* thingSpeakHost = "api.thingspeak.com";
String writeAPIKey = "66Q6WSD86W8M0TCF";  //ThingSpeak proje writeAPIKey'i

//Değişken ve pin tanımlamalarını yapıyoruz.
int toprakSensor=A0;
int deger;
int nem, donusum;

WiFiClient client;

void wifiSetup () //ESP8266 WiFi Kurulum Fonksiyonu
{ 
  delay (10); 
  Serial.println(); Serial.println(WLAN_SSID); 
  Serial.print(" Kablosuz Agina Baglaniyor");
  WiFi.begin(WLAN_SSID, WLAN_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)  //WiFi durum kontrolü
  { 
      delay(500); 
      Serial.print("."); 
  } 
  Serial.println(); 
  Serial.print(WLAN_SSID); Serial.println("Kablosuz Aga Baglandi"); 
  Serial.println("IP adresi: "); Serial.println(WiFi.localIP()); 
} 

void setup() 
{
  Serial.begin(115200); //Seri haberleşme 115200 baud hızında başlatılıyor.
  wifiSetup();  //Kablosuz ağ kurulum fonksiyonu
  Blynk.begin(token, WLAN_SSID, WLAN_PASSWORD);  //Blynk bağlantısı 
  pinMode (A0, INPUT);  //A0 ucu sensör okumak için giriş moduna ayarlandı.
}

void loop() 
{
  nem = analogRead(toprakSensor); //Analog uçtan okunan değer nem değişkenine atandı.
 
  Blynk.virtualWrite (V1, nem); //Blynk uygulamasında oluşturulan virtual pine nem değeri yazdırıldı.
  
  if(nem > 900) //Eğer nem değeri 900'den büyük ise bu kod bloğu çalışacak.
  {
    //Blynk platformu üzerinde tanımlanan alarm çalıştırılacak.
    Blynk.logEvent("toprak_nem_alarm", "Toprak çok kuru lütfen sulama yapınız."); 
  }
  Blynk.run(); //Blynk çalıştırılıyor.

  //ThingSpeak platformuna bağlanıyoruz ve yazdırma işlemlerini yapıyoruz.
  if (client.connect (thingSpeakHost, 80)) 
  {
    String postStr = "field1=";
    postStr+= String (nem);
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + writeAPIKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    client.print("\n\n");

    Serial.print("Toprak Nem : "); Serial.println(nem);
    Serial.println("Thingspeak veri gonderildi");
  }

  client.stop();
  delay(15000);

}
