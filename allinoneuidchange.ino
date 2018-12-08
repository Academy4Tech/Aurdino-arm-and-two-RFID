/**
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from more than one PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 *
 * Example sketch/program showing how to read data from more than one PICC (that is: a RFID Tag or Card) using a
 * MFRC522 based RFID Reader on the Arduino SPI interface.
 *
 * Warning: This may not work! Multiple devices at one SPI are difficult and cause many trouble!! Engineering skill
 *          and knowledge are required!
 *
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_1_PIN        39         // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 2
#define SS_2_PIN        38          // Configurable, take a unused pin, only HIGH/LOW required, must be diffrent to SS 1

#define NR_OF_READERS   2

byte ssPins[] = {SS_1_PIN, SS_2_PIN};

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.


int flag,a,b,c,d,e,f,g,reader1;
uint8_t reader=0; 
unsigned long  alpha=0,temp=0;


/**
 * Initialize.
 */
void setup() {

  Serial.begin(9600); // Initialize serial communications with the PC
 // while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();        // Init SPI bus

  for (reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

/**
 * Main loop.
 */
void loop() {

  for (reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial()) {
      Serial.print(F("Reader "));
      Serial.print(reader);
      // Show some details of the PICC (that is: the tag/card)
      Serial.print(F(": Card UID:"));
      alpha=dump_byte_array(mfrc522[reader].uid.uidByte);
      Serial.println();
      reader1=reader;
      //Serial.print(F("PICC type: "));
      //MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
     // Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));

      // Halt PICC
      mfrc522[reader].PICC_HaltA();
      // Stop encryption on PCD
      mfrc522[reader].PCD_StopCrypto1();
    } //if (mfrc522[reader].PICC_IsNewC
  } //for(uint8_t reader

 // Serial.print("Outside ");
// if(alpha!=0){
 //Serial.print(alpha);
 //delay(1000);
// }


 if(temp==0 && temp!=alpha && reader1==0)
      {temp=alpha;
      Serial.print(temp);
      Serial.print("First initialization ");
      a=millis();
      flag=0;
        }
       if(temp!=0 && temp!=alpha && reader1==0)
      {
        Serial.print("New card detected  old card will be taken as gone. ");
        flag=1;
        temp=alpha;
        }
        
      if(alpha==temp&&reader1==0&&alpha!=0)
      {
      //flag=2;
       Serial.print("Card swiped Activation started  ");
       delay(1000);
     //lets start reading sensor data 
      }

     if(alpha==temp&&reader1==1&&alpha!=0)
      {
       Serial.print("Closing Sensors  ");
     //lets start reading sensor data 
     b=millis()-a;
     c=b/1000;
      Serial.print(c);
      delay(100);
      temp=0;
      alpha=0;
      flag=3;
      
      }
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  */
 double dump_byte_array(unsigned char *buffer)
{
    uint32_t value = 0;

    value |= (uint32_t)buffer[0] << 24;    // cast to target type before 
    value |= (uint32_t)buffer[1] << 16;    // shifting to avoid overflow
    value |= (uint32_t)buffer[2] << 8;     // no parentheses needed due to operator precedence
    value |= buffer[3];
    Serial.print(value);
    return (value);

  
}
