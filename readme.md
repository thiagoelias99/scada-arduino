- [X] When the serial data is received, display it on the LCD for a brief moment.
- [X] Monitor humidity && temperature, display it on the LCD.
- [ ] Serial send humidity && temperature.
- [ ] Monitor joystick X | Y position, display it on the LCD.
- [ ] Serial send joystick X | Y position.
- [ ] When RFID tags is reded, display its id on the LCD for a brief moment.
- [ ] Serial send tag Id.

### Serial Patterns
- Humidity (send): 44 48 48 55 (VALUE IN HEX)
  - 44 48 48 55 3C -> 60%
  - 44 48 48 55 14 -> 20%
- Temperature (send): 44 48 54 45 (VALUE IN HEX)
  - 44 48 54 45 19 -> 25%
  - 44 48 54 45 25 -> 37%
- Joystick (send): 4A 59 58 49 (X VALUE IN HEX) (Y VALUE IN HEX)
  - 4A 59 58 49 1E 36 -> X=30 Y=70
  - 4A 59 58 49 32 00 -> X=50 Y=0