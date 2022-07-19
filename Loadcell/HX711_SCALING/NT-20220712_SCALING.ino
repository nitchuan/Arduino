#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>
#include "HX711.h"
#include<LiquidCrystal_I2C.h>

// Konfigurasi Thinger.io
#define USERNAME "Nitchuan_Tech"
#define DEVICE_ID "NodeMCU_Loadcell"
#define DEVICE_CREDENTIAL "O@fE!p0@8foEo?ds"

// Variabel untuk Thinger.io
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// Kongurasi WiFi
const char* ssid = "NitchuanTech 2.4GHz";
const char* password = "13ismillah";

LiquidCrystal_I2C lcd(0x27,16,2);

// HX711 wiring
#define DOUT  D5
#define CLK  D6

HX711 scale;

float calibration_factor = -96.35;
float ounces;
float lbs;
int grams;
int harga;

// Tare Push Botton
int taree = D4;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  scale.begin(DOUT, CLK);
  Serial.println("Starting..");
  lcd.setCursor(0,0);
  lcd.print("Starting..");
  scale.set_scale(calibration_factor);
  scale.tare();
  delay(1000);

  pinMode(taree, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Menghubungkan..");
    delay(500);
  }

  // WiFi Sudah Konek
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("WiFi Terhubung");
  delay(500);

  thing.add_wifi(ssid, password);
  thing["HX711"] >> [](pson & out)
  {
    out["Berat"] = grams;
    out["Harga"] = harga;
    out["Taree"] = taree;
  };
}

void loop() {
  grams = scale.get_units(10), 0;
  if (grams < 0)
  {
    grams = 0;
    lcd.clear();
  }
  else if (grams > 0 && grams <= 10)
  {
    grams = 0;
    lcd.clear();
  }
  
  ounces = grams * 0.035274;  // jika ingin menggunakan satuan ounce
  lbs = grams * 0.00220462;   // jika ingin menggunakan satuan lbs
  harga = grams * 10;
  Serial.print(grams);
  Serial.println(" grams");
  Serial.print(harga);
  Serial.println(" rupiah");
  lcd.setCursor(0,0);
  lcd.print("Grams  : ");
  lcd.setCursor(9,0);
  lcd.print(grams);
  lcd.setCursor(0,1);
  lcd.print("Rupiah : ");
  lcd.setCursor(9,1);
  lcd.print(harga);
  delay(500);

  if (digitalRead (taree) == LOW)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tare..");
    scale.set_scale(calibration_factor);
    scale.tare();
    delay(1000);
  }

  thing.handle();
}
