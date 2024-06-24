#include <LiquidCrystal_I2C.h> //Marco Schwartz

#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_LINES);

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

void serial_setup()
{
  Serial.begin(9600);
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
}