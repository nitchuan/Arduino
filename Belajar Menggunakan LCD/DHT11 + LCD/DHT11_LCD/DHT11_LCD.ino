// LCD 16X2 i2C
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

// DHT11
#include "DHT.h" //library sensor yang telah diimportkan
#define DHTPIN 2     //Pin apa yang digunakan
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  Serial.println("Pengujian DHT11!");
  dht.begin();
}

void loop() {
  delay(3000);

  //Pembacaan untuk data kelembaban
  int humidity = dht.readHumidity();
  //Pembacaan dalam format celcius (c)
  int celcius = dht.readTemperature();
  //pembacaan dalam format Fahrenheit
  int fahrenheit = dht.readTemperature(true);

  //mengecek pembacaan apakah terjadi kegagalan atau tidak
  if (isnan(humidity) || isnan(celcius) || isnan(fahrenheit)) {
    Serial.println("Pembacaan data dari module sensor gagal!");
    return;
  }

  int htof = dht.computeHeatIndex(fahrenheit, humidity); 
  //Prosedur pembacaaan data indeks panas dalam bentuk fahreheit
  int htoc = dht.computeHeatIndex(celcius, humidity, false);
 //Prosedur pembacaaan data indeks panas dalam bentuk celcius

  //pembacaan nilai pembacaan data kelembaban
  Serial.print("Kelembaban: ");
  Serial.print(humidity);
  Serial.print(" %\t");

  // Menampilkan Kelembapan di LCD
  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.setCursor(3, 0);
  lcd.print(humidity);
  lcd.setCursor(5, 0);
  lcd.print("%");
  
  //pembacaan nilai pembacaan data suhu
  Serial.print("Suhu : ");
  Serial.print(celcius); //format derajat celcius
  Serial.print("°"); //simbol derajat
  Serial.print("C / ");
  Serial.print(fahrenheit); //format derajat fahrenheit
  Serial.print("°");
  Serial.print("F\t");

  // Menampilkan Suhu di LCD
  lcd.setCursor(7, 0);
  lcd.print("S:");
  lcd.setCursor(10, 0);
  lcd.print(celcius);
  lcd.setCursor(12, 0);
  lcd.print("C");

  Serial.print("Indeks Panas: ");
  Serial.print(htof); 
  Serial.print("°");
  Serial.print("F / ");
  Serial.print(htoc);
  Serial.print("°");
  Serial.println("C ");

}
