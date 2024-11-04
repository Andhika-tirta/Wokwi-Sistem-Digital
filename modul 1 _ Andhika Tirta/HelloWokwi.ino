#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2          // Pin yang terhubung ke DHT22
#define DHTTYPE DHT22     // Tipe sensor DHT yang digunakan
#define BUZZER_PIN 8      // Pin Buzzer
#define LED_PIN 9         // Pin LED
#define BUTTON_PIN 7      // Pin Push Button

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD (alamat I2C 0x27)

bool buttonState = false; // Variabel untuk menyimpan status tombol
bool buzzerOn = false;    // Status Buzzer

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Menggunakan pull-up internal

  lcd.begin(16, 2);       // Inisialisasi LCD 16x2
  lcd.backlight();        // Menyalakan lampu latar LCD
  dht.begin();            // Memulai sensor DHT
  
  lcd.setCursor(0, 0);
  lcd.print("Starting...");
  delay(2000);            // Tunggu sebentar sebelum mulai
  lcd.clear();
}

void loop() {
  // Membaca suhu dan kelembapan dari sensor DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Mengecek apakah data dari sensor tersedia
  if (isnan(temperature) || isnan(humidity)) {
    lcd.setCursor(0, 0);
    lcd.print("Error reading");
    lcd.setCursor(0, 1);
    lcd.print("DHT sensor");
    return;
  }

  // Menampilkan data suhu dan kelembapan di LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");

  // Membaca status push button
  if (digitalRead(BUTTON_PIN) == LOW) { // Tombol ditekan
    buttonState = !buttonState;         // Mengubah status tombol
    delay(300);                         // Debounce
  }

  // Jika tombol aktif, hidupkan LED dan Buzzer saat suhu lebih dari 30°C
  if (buttonState) {
    if (temperature > 30) {
      digitalWrite(LED_PIN, HIGH);  // Nyalakan LED
      if (!buzzerOn) {
        tone(BUZZER_PIN, 1000);     // Bunyikan Buzzer dengan frekuensi 1000 Hz
        buzzerOn = true;
      }
    } else {
      digitalWrite(LED_PIN, LOW);   // Matikan LED
      noTone(BUZZER_PIN);           // Matikan Buzzer
      buzzerOn = false;
    }
  } else {
    digitalWrite(LED_PIN, LOW);     // Matikan LED
    noTone(BUZZER_PIN);             // Matikan Buzzer
  }

  delay(1000); // Delay 1 detik sebelum loop berikutnya
}
