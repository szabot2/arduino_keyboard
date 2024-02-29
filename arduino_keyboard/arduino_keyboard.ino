#define CLOCK 7 //D-
#define DATA 6  //D+

const char keymap[] = {
  0, 0,  0,  0,  0,  0,  0,  0,
  0, 0,  0,  0,  0,  0, '`', 0,
  0, 0 , 0 , 0,  0, 'q', '1', 0,
  0, 0, 'y', 's', 'a', 'w', '2', 0,
  0, 'c', 'x', 'd', 'e', '4', '3', 0,
  0, ' ', 'v', 'f', 't', 'r', '5', 0,
  0, 'n', 'b', 'h', 'g', 'z', '6', 0,
  0, 0, 'm', 'j', 'u', '7', '8', 0,
  0, ',', 'k', 'i', 'o', '0', '9', 0,
  0, '.', '/', 'l', ';', 'p', '-', 0,
  0, 0, '\'', 0, '[', '=', 0, 0,
  0, 0, 13, ']', 0, '\\', 0, 0,
  0, 0, 0, 0, 0, 0, 127, 0,
  0, '1', 0, '4', '7', 0, 0, 0,
  '0', '.', '2', '5', '6', '8', 0, 0,
  0, '+', '3', '-', '*', '9', 0, 0,
  0, 0, 0, 0
};



void setup()
{
  Serial.begin(115200);
  pinMode(CLOCK, INPUT_PULLUP); //For most keyboards the builtin pullups are sufficient, so the 10k pullups can be omitted
  pinMode(DATA, INPUT_PULLUP);
  bitSet(PCICR, PCIE2); // Enable pin change interrupts on pin D0-D7
  bitSet(PCMSK2, CLOCK); // Pin change interrupt on Clock pin
}

uint8_t lastscan;
uint8_t line = 0, col = 0;


ISR(PCINT2_vect)
{
  uint16_t scanval = 0;
  for (int i = 0; i < 11; i++)
  {
    while (digitalRead(CLOCK));
    scanval |= digitalRead(DATA) << i;
    while (!digitalRead(CLOCK));
  }
  scanval >>= 1;
  scanval &= 0xFF;
  //Serial.println(scanval, HEX);
  if (lastscan != 0xF0 && scanval != 0xF0)
    switch (scanval)
    {
      case 0x5A: //Enter
        Serial.println();
        break;
      default:
        Serial.print(keymap[scanval]);
        col++;
    }
  lastscan = scanval;
  bitSet(PCIFR, PCIF2);
}

void loop()
{
  /*digitalWrite(13, LOW);
    delay(500);
    digitalWrite(13, HIGH);
    delay(500);*/
}
