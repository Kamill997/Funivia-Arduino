  #include <ezButton.h>

  #include <MFRC522.h>
  #include <deprecated.h>
  #include <require_cpp11.h>
  #include <SPI.h>
  #include <Stepper.h>   
  #include <ezButton.h>

  ezButton toggleSwitch(3); 

  #define SS_PIN 10
  #define RST_PIN 9
  MFRC522 myRFID(SS_PIN, RST_PIN);  //Creazione oggetto RFID

  int revolution = 200;    
  // Pin Stepper
  int IN1 = 5;
  int IN2 = 6;
  int IN3 = 7;
  int IN4 = 8;
  
  void setup() 
  {
    Serial.begin(9600);  
    SPI.begin();      
    myRFID.PCD_Init();   //Iniziliazza il funzionamento dell'RFID
    Serial.println("Per favore scannerizza la tua carta");
    Serial.println();
    pinMode(2,INPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
  }
  void loop() 
  {
    toggleSwitch.setDebounceTime(10);
    //Attende carta RFID
    if ( ! myRFID.PICC_IsNewCardPresent()) 
    {

      return;
    }
    //Carta RFID passata ma senza UID 
    if ( ! myRFID.PICC_ReadCardSerial()) 
    {
      
      return;
    }
    //Mostra valore UID della carta
    
    Serial.print("USER ID tag :");
    String content= "";
  
    for (byte i = 0; i < myRFID.uid.size; i++) 
    {
      Serial.print(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " ");
      Serial.print(myRFID.uid.uidByte[i], HEX);
      content.concat(String(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " "));
      content.concat(String(myRFID.uid.uidByte[i], HEX));
    }
    
    content.toUpperCase();
    content = content.substring(1);

  if(content=="59 27 EB 97")
  {
      Serial.println("Accesso permesso!");
      bool stato=false;
      
        Serial.println("Sono prima del while");
      while(!stato)
      {
        toggleSwitch.loop();  
        int buttonState= toggleSwitch.getState();
        Serial.println("Sono dentro while");
          if(buttonState==HIGH)
          {
            Serial.println("ON");
            if(digitalRead(2)== LOW) {
            Serial.println("Finito!");
            stato=true;
            }
            else{
            stepper.setSpeed(100);  
            stepper.step(revolution);
          }
        }
      else
      {
        Serial.println("OFF");
        stato=true;
      }
    }
  } 
    else
    {
      Serial.println("Accesso negato!");
    }
  }
