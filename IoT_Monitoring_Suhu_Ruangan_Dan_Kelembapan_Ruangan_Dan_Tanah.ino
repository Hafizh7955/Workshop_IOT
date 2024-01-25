//Rev :Rio M Taufik
//2209-18
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

DHT dht(DHTPIN, DHTTYPE);
const int sensor_pin = 34;
int sensor_analog;
float M;
float t;
float h;
const char *ssid = "DESKTOP-4IPQNN3 5875";
const char *password = "11111111";
#define CHAT_ID "6653416821"
#define BOTtoken "6561108507:AAHs5EDzVUEuM_s6ZktDiLeUX2byKmtMPOw"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void setup()
{
    Serial.begin(115200);
    Serial.println("Tunggu sebentar ya, lagi proses Menghubungkan WiFi Nih...");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.print("Alamat IP: ");
    Serial.println(WiFi.localIP());

    dht.begin();
}

void loop()
{
    sensor_analog = analogRead(sensor_pin);
    M = (100 - ((sensor_analog / 4095.00) * 100));
    t = dht.readTemperature();
    h = dht.readHumidity();
    if (isnan(h) || isnan(t) || isnan(M))
    {
        Serial.println("Sensor Tidak Terbaca!");
        return;
    }

    // Send preliminary message to Telegram
    bot.sendMessage(CHAT_ID, "Tunggu bentar ya..., Lagi di cek nih !");

    Serial.println("MONITORING TANAMAN");
    Serial.print("Kelembapan Udara: ");
    Serial.print(h);
    Serial.println("%");
    Serial.print("Suhu Lingkungan: ");
    Serial.print(t);
    Serial.println("°C");
    Serial.print("Kelembapan Air Tanah: ");
    Serial.print(M);
    Serial.println("%");

    // Send data to Telegram
    String message = "Kelembapan Udara: " + String(h) + "%\nSuhu Lingkungan: " + String(t) + "°C\nKelembapan Air Tanah: " + String(M) + "%";
    bot.sendMessage(CHAT_ID, message);

    delay(10000);
}