#include <LiquidCrystal_I2C.h>
#include <DHT11.h>
#include <SPI.h>
#include <MFRC522.h>

// LCD
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_LINES);

// DHT11
#define DHTPIN 3
DHT11 dht11(3);
int temperature = 0;
int humidity = 0;
int dht_read_interval = 100;
int dht_read_count = 0;

// JoyStick
#define JOY_X A1
#define JOY_Y A2
int joy_x = 0;
int joy_y = 0;

// RFID
#define RF_SS_PIN 10
#define RF_RST_PIN 9
MFRC522 mfrc522(RF_SS_PIN, RF_RST_PIN);
byte rf_read_iu[4];

//Piezzo
#define PIEZZO 2

void piezzo_setup()
{
  pinMode(PIEZZO, OUTPUT);
}

void piezzo_play(int frequency = 800, int duration = 200)
{
  tone(PIEZZO, frequency, duration);
  delay(250);
  tone(PIEZZO, frequency, duration);
}

void rfid_setup()
{
  SPI.begin();
  mfrc522.PCD_Init();
}

String command = "";

void lcd_setup()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void lcd_print_serial_command()
{
  lcd.clear();
  lcd.print("Serial Command");
  lcd.setCursor(0, 1);
  lcd.print(command);
}

void lcd_print_general_info()
{
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("H: ");
  lcd.print(humidity);
  lcd.print("%");
  lcd.setCursor(11, 0);
  lcd.print("X: ");
  lcd.print(joy_x);
  lcd.setCursor(11, 1);
  lcd.print("Y: ");
  lcd.print(joy_y);
}

void serial_setup()
{
  Serial.begin(9600);
}

void serial_send_general_info()
{
  Serial.print("44 48 48 55 ");
  Serial.println(humidity, HEX);
  Serial.print("44 48 54 45 ");
  Serial.println(temperature, HEX);
  Serial.print("4A 59 58 49 ");
  Serial.print(joy_x, HEX);
  Serial.print(" ");
  Serial.println(joy_y, HEX);
}

void dht11_read()
{
  if (dht_read_count == 0)
  {
    dht11.readTemperatureHumidity(temperature, humidity);
    dht_read_count = dht_read_interval;
  }
  else
  {
    dht_read_count--;
  }
}

void joystick_setup()
{
  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);
}

void joystick_read()
{
  joy_x = map(analogRead(JOY_X), 0, 1023, 0, 100);
  joy_y = map(analogRead(JOY_Y), 0, 1023, 0, 100);
}

void rfid_read()
{
  if (!mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  if (!mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  for (byte i = 0; i < 4; i++)
  {
    rf_read_iu[i] = mfrc522.uid.uidByte[i];
  }
  mfrc522.PICC_HaltA();

  piezzo_play();

  Serial.print("52 46 49 44 ");
  for (byte i = 0; i < 4; i++)
  {
    Serial.print(rf_read_iu[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  lcd.clear();
  lcd.print("RFID: ");
  lcd.setCursor(0, 1);
  for (byte i = 0; i < 4; i++)
  {
    lcd.print(rf_read_iu[i]);
    lcd.print(" ");
  }

  delay(1000);
  lcd.clear();
}

void setup()
{
  lcd_setup();
  serial_setup();
  joystick_setup();
  rfid_setup();
  piezzo_setup();
}

void loop()
{
  if (Serial.available() > 0)
  {
    command = Serial.readStringUntil('\n');
    command.trim();

    lcd_print_serial_command();
    piezzo_play();
    delay(1000);
    lcd.clear();
  }
  dht11_read();
  joystick_read();
  lcd_print_general_info();
  serial_send_general_info();
  rfid_read();

  delay(100);
}