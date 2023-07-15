#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ThingSpeak.h>

#define DHTTYPE DHT22

int R_relayPin = 12;
int B_relayPin = 2;
int dhtPin = 4;
int canal = INSIRA_CHANNEL_ID;
const char* API = INSIRA_WRITE_API_KEY;
const char* ssid = "Wokwi-GUEST";
const char* senha = "";
const char* servidor = "api.thingspeak.com";

DHT dht(dhtPin, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 20, 4);
WiFiClient client;

void setup() {
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(R_relayPin, OUTPUT);
  pinMode(B_relayPin, OUTPUT);
  delay(10);
  lcd.setCursor(0, 0);
  lcd.print("Conectando na rede");
  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print(".");
  }
  lcd.clear();
  lcd.print("Conectado!");
  delay(1000);
  lcd.clear();
  ThingSpeak.begin(client);
}

void loop() {
  //Temperatura
  float TEMP = dht.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("Temperatura: ");
  lcd.setCursor(0, 1);
  lcd.print(TEMP, 1);
  lcd.print((char)223);
  lcd.print("C");
  if (TEMP > 35){
    digitalWrite(R_relayPin, HIGH);
  } else {
    digitalWrite(R_relayPin, LOW);
  }
  //Umidade
  float HUM = dht.readHumidity();
  lcd.setCursor(0, 2);
  lcd.print("Umidade: ");
  lcd.setCursor(0, 3);
  lcd.print(HUM, 1);
  lcd.print("%");
  if (HUM > 40) {
    digitalWrite(B_relayPin, HIGH);
  } else {
    digitalWrite(B_relayPin, LOW);
  }
  //Registro dos valores obtidos no canal ThingSpeak
  //Múltiplos valores podem ser inseridos entre parênteses. Ex.: (x, y)
  ThingSpeak.writeField(canal, (1, 2), (TEMP, HUM), API);
  delay(5000);   //Intervalo de 5 segundos
}
