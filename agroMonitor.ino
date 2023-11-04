#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// Informações da rede Wi-Fi
const char* ssid = "************"; // NOME DA REDE WIFI
const char* password = "************"; // SENHA DA REDE WIFI
const char* serverAddress = "https://api-agromonitor-production.up.railway.app/postData"; // Endereço da API

// Pino de dados do sensor DHT11
#define DHTPIN 26

DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(2500);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado à rede WiFi");

  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Falha ao ler o sensor DHT!");
    delay(2500);
    return;
  }

  HTTPClient http;

  http.begin(serverAddress);

  String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST(postData);

  if (httpResponseCode != 200) {
    Serial.println("Falha ao enviar os dados.");
    delay(2500);
    return;
  }

  http.end();

  delay(2500);
}
