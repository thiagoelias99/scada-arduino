#include <LiquidCrystal_I2C.h>
#include <DHT11.h>

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
  // lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("H: ");
  lcd.print(humidity);
  lcd.print("%");
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
}

void dht11_read()
{
  dht11.readTemperatureHumidity(temperature, humidity);
}

void setup()
{
  lcd_setup();
  serial_setup();
}

void loop()
{
  if (Serial.available() > 0)
  {
    command = Serial.readStringUntil('\n');
    command.trim();

    lcd_print_serial_command();
  }
  dht11_read();
  lcd_print_general_info();
  serial_send_general_info();
}