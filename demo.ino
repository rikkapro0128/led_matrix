#define latchPin 8
#define dataPin 11
#define clockPin 12
#define NUMBER_HC595 2
#define NUMBER_CHARACTER 12

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(100);
	pinMode(latchPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
}

/*     __________________________________________________  
    0  |      |      |      |      |      |      |      |     
    0  |      |      |      |      |      |      |      |
    0  |      |      |      |      |      |      |      |
    0  |      |      |      |      |      |      |      |
    0  |      |      |      |      |      |      |      |
    0  |      |      |      |      |      |      |      |
    0  |      |      |      |      |      |      |      |
    0  |      |      |      |      |      |      |      |
    0  |      |      |      |      |      |      |      |
       __________________________________________________
        1     1      1      1      1      1      1      1
*/

const char ledStatus[NUMBER_CHARACTER][8] = // 1 is LOW, 0 is HIGH and this is array scan arrow
{
    {0xFF,0xC1,0x80,0xBE,0xBE,0x80,0xC1,0xFF}, // 0
    {0xFF,0xE7,0xF3,0xF9,0x00,0x00,0xFF,0xFF}, // 1
    {0xFF,0xB3,0x99,0x8D,0xA1,0xA3,0xBF,0xFF}, // 2
    {0xFF,0xDB,0xBD,0xAD,0xAD,0xAD,0xD3,0xFF}, // 3
    {0xFF,0xCF,0xD7,0xDB,0x81,0x81,0xDF,0xFF}, // 4
    {0xFF,0xD1,0x91,0xB5,0xB5,0x85,0xCD,0xFF}, // 5
    {0xFF,0xC3,0x81,0xB5,0xB5,0x85,0xCD,0xFF}, // 6
    {0xFF,0xFD,0xFD,0x0D,0x05,0xF1,0xF9,0xFF}, // 7
    {0xFF,0xC9,0x80,0xB6,0xB6,0x80,0xC9,0xFF}, // 8
    {0xFF,0xD9,0x90,0xB6,0xB6,0x80,0xC1,0xFF}, // 9
    {0xE3,0xDD,0xBD,0x7B,0x7B,0xBD,0xDD,0xE3}, // heart
    {0xEF,0xDF,0xB1,0xBF,0xBF,0xB1,0xDF,0xEF}, // smile
};

void clearData() {
    for(int i = 0; i <= NUMBER_HC595; i++) {
        digitalWrite(latchPin, 0);
        shiftOut(dataPin, clockPin, LSBFIRST, 0x00);
        digitalWrite(latchPin, 1);
    }
}

void showCharacter(const char ledStatus[][8], unsigned int duration) 
{
    for(int i = 0; i < NUMBER_CHARACTER; i++) {
        // duration = (i == 11) ? 5000 : duration;
        // duration = (i == 10) ? 1000 : duration;
        unsigned int scroll = 0;
        unsigned long state = 0b1000000000000000; 
        while(scroll < 16) {
            for (int hold = 0; hold < duration; hold++) // loop delay effect show one character
            {
                int dem = 0;
                unsigned long temp = 0;
                for(int j = 0; j < 8; j++) { // loop for show one character
                    clearData(); // clear data before latch pin
                    digitalWrite(latchPin, 0);
                    shiftOut(dataPin, clockPin, LSBFIRST, ledStatus[i][j]);
                    temp = (dem == 0) ? state : temp >> 1;
                    shiftOut(dataPin, clockPin, LSBFIRST, temp);
                    digitalWrite(latchPin, 1);
                    dem++;
                }
                delay(1);
            }
            scroll++;
            state = (state >> 1);
        }
    }
}

void loop()
{
    showCharacter(ledStatus, 10);
}
