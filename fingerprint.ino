#include<LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>
LiquidCrystal_I2C lcd(0X27,16,2);
#include <SoftwareSerial.h>
SoftwareSerial fingerPrint(2, 3);// FINGERPRINT SENSOR RX/TX
#include <Adafruit_Fingerprint.h>
uint8_t id;
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerPrint);
#define enroll 4
#define del 5
#define up 6
#define down 7
const int buzzer = 8;
String inbyte;
void printProgmem(const char* str) 
{
  char c;
  while ((c = pgm_read_byte(str++)) != 0) { // Read each character from PROGMEM
    Serial.print(c);
  }
}
void setup() 
{
    pinMode(enroll, INPUT);
    pinMode(up, INPUT); 
    pinMode(down, INPUT); 
    pinMode(del, INPUT); 
    pinMode(buzzer,OUTPUT);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5,0);
    lcd.print("WELCOME");
    lcd.setCursor(0,1);
 lcd.print("_______________________");
    delay(1000);
    finger.begin(57600);
    Serial.begin(9600);
    lcd.clear();
    lcd.print("Finding Module");
    lcd.setCursor(0,1);
    delay(500);
    if (finger.verifyPassword()) 
    {
      lcd.clear();
      lcd.print("Found Module ");
      delay(500);
    } 
    else 
    {
    lcd.clear();
    lcd.print("module not Found");
    lcd.setCursor(0,1);
    lcd.print("Check Connections");
    while (1);
    }
}
const char message1[] PROGMEM = "Name :Theanmadhi,Age :19,Blood group:o+,Phone number :7639689989,Location:ponnamapet salem,Last medicine intake : thyroxine";
const char message2[] PROGMEM = "Name :sowbaranigha S J,Age :19 ,Blood group:A+,Phone number :9894994725 ,Location:Kalipatti ,Last medicine intake :None";
const char message3[] PROGMEM = "Name : Sakthinavathara,Age : 19,Blood group: A+,Phone number : 9342606125,Location: Salem,Last medicine intake : None";
const char message4[] PROGMEM = "Name :Rajarajeswari ,Age : 19,Blood group:B+,Phone number : 96591 13591,Location: Tirichy,Last medicine intake : paracetamol, amoxicillin";

void loop() 
{
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("PLACE FINGER");
 lcd.setCursor(3,1);
 lcd.print("PRESS +/-");
 if(Serial.available()>0){
  inbyte = Serial.readString();
 }
 if(digitalRead(up)==0 || digitalRead(down)==0 || inbyte == "S")
 {
  for(int i=0;i<5;i++)
  {
    lcd.clear();
    lcd.print("Place Finger");
    delay(2000);
    int result=getFingerprintIDez();
    if(result>=0)
    {
        
         if(result==1){
        lcd.clear();
        lcd.print("Theanmadhi");
        printProgmem(message1);
        delay(3000);
        return;
         }
         if(result==2){
        lcd.clear();
        lcd.print("sowbaranigha S J");
        printProgmem(message2);
        delay(3000);
        return;
         }
         if(result==3){
        lcd.clear();
        lcd.print("Sakthinavathara");
        printProgmem(message3);
        delay(3000);
        return;
         }
         if(result==4){
        lcd.clear();
        lcd.print("Rajarajeswari");
        printProgmem(message4);
        delay(3000);
        return;
         }
         if(result==5){
        lcd.clear();
        lcd.print("PERSON 5");
        delay(3000);
        return;
         }
     }
   }
   inbyte ="";
 }
 checkKeys();
 delay(1000);
}

void checkKeys()
{
   if(digitalRead(enroll) == 0 || inbyte == "E")
   {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    while(digitalRead(enroll) == 0);
    Enroll();
    inbyte = "";
   }

   else if(digitalRead(del) == 0 || inbyte == "D")
   {
    lcd.clear();
    lcd.print("Please Wait");
    delay(1000);
    delet();
    inbyte = "";
   }  
}

void Enroll()
{
   int count=0;
   lcd.clear();
   lcd.print("Enroll Finger    ");
   lcd.setCursor(0,1);
   lcd.print("Location:");
   while(1)
   {
     lcd.setCursor(9,1);
     lcd.print(count);
     if(digitalRead(up) == 0)
     {
       count++;
       if(count>25)
       count=0;
       delay(500);
     }

     else if(digitalRead(down) == 0)
     {
       count--;
       if(count<0)
       count=25;
       delay(500);
     }
     else if(digitalRead(del) == 0)
     {
          id=count;
          getFingerprintEnroll();
          return;
     }

       else if(digitalRead(enroll) == 0)
     {        
          return;
     }
 }
}

void delet()
{
   int count=0;
   lcd.clear();
   lcd.print("Delete Finger    ");
   lcd.setCursor(0,1);
   lcd.print("Location:");
   while(1)
   {
     lcd.setCursor(9,1);
     lcd.print(count);
     if(digitalRead(up) == 0)
     {
       count++;
       if(count>25)
       count=0;
       delay(500);
     }

     else if(digitalRead(down) == 0)
     {
       count--;
       if(count<0)
       count=25;
       delay(500);
     }
     else if(digitalRead(del) == 0)
     {
          id=count;
          deleteFingerprint(id);
          return;
     }

       else if(digitalRead(enroll) == 0)
     {        
          return;
     }
 }
}

uint8_t getFingerprintEnroll() 
{
  int p = -1;
  lcd.clear();
  lcd.print("finger ID:");
  lcd.print(id);
  lcd.setCursor(0,1);
  lcd.print("Place Finger");
  delay(2000);
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
    switch (p)
    {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print("No Finger");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Comm Error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("Imaging Error");
      break;
    default:
      lcd.clear();
      lcd.print("Unknown Error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
       lcd.clear();
       lcd.print("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Comm Error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("Feature Not Found");
      return p;
    default:
      lcd.clear();
      lcd.print("Unknown Error");
      return p;
  }
  lcd.clear();
  lcd.print("Remove Finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
   lcd.clear();
      lcd.print("Place Finger");
      lcd.setCursor(0,1);
      lcd.print("   Again");
      delay(3000);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      break;
    case FINGERPRINT_IMAGEFAIL:
      break;
    default:
      return;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      return p;
    default:
      return p;
  }
  
  // OK converted!
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    return p;
  } else {
    return p;
  }   
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Stored!");
    delay(2000);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    return p;
  }
  else {
    return p;
  }   
}

int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  
  if (p != FINGERPRINT_OK)  
  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  
  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
   lcd.clear();
   lcd.print("UNAUTHORIZED");
   lcd.setCursor(0,1);
   lcd.print("PERSON");
   Serial.println("UNAUTHORIZED PERSON");
   digitalWrite(buzzer,HIGH);
   delay(3000);  
   digitalWrite(buzzer,LOW);
  return -1;
  }
  // found a match!
  return finger.fingerID; 
}

uint8_t deleteFingerprint(uint8_t id) 
{
  uint8_t p = -1;  
  lcd.clear();
  lcd.print("Please wait");
  p = finger.deleteModel(id);
  if (p == FINGERPRINT_OK) 
  {
    lcd.clear();
    lcd.print("Figer Deleted");
    lcd.setCursor(0,1);
    lcd.print("Successfully");
    delay(1000);
  } 
  
  else 
  {
    lcd.clear();
    lcd.print("Something Wrong");
    lcd.setCursor(0,1);
    lcd.print("Try Again Later");
    delay(2000);
    return p;
  }   
}

