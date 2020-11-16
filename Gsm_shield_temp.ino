//*****************************************************
//this code reads the temperature coming from the mlx sensor and sends an sms to a particular phone number if it exceeds the threashold

//*****************************************************


#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM900

SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8
#include <Adafruit_MLX90614.h>
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// Create variable to store incoming SMS characters
char incomingChar;

int MAX_TEMP=26; // change to desired temp

void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);

//Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);
  mlx.begin();

  Serial.println("Initializing..."); 
  delay(1000);
 mySerial.println("AT"); //Handshaking with SIM900
  delay(100);

  SIM900power(); //SOFTWEAR START

}

void SIM900power()
{
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}

 
void loop()
{
  updateSerial();

//SERIAL
  //IN DEGRESS
  Serial.print("Ambient in Celcius = ");
  Serial.print(mlx.readAmbientTempC());
  Serial.print("   Object in Celcius = ");
  Serial.print(mlx.readObjectTempC());
  Serial.println("C");

//IN FARAD
//  Serial.print("Ambient in Farad   = ");
//  Serial.print(mlx.readAmbientTempF());
//  Serial.print("   Object in Farad   = ");
//  Serial.print(mlx.readObjectTempF());
//  Serial.println("F");
 
  delay(500);
 
 if(mlx.readObjectTempC()>MAX_TEMP )

{
 
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+2348035825897\"");//2348039333116 change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
    // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT

   //  mySerial.print("HIGH BODY TEMPERATURE: "); //text content
  
      String dataMessage = ("HIGH BODY TEMPERATURE: " + String(mlx.readObjectTempC()) + "*C " + " AMBIENT: " + String(mlx.readAmbientTempC()) + "*C");
      // Uncomment to change message with farenheit temperature
      // String dataMessage = ("Temperature: " + String(f) + "*F " + " Humidity: " + String(h) + "%");      
      
      // Send the SMS text message
      mySerial.print(dataMessage);
      delay(100);
      // End AT command with a ^Z, ASCII code 26
      mySerial.println((char)26); 
      delay(100);
      mySerial.println();
      // Give module time to send SMS
      delay(2000);  
    }
  delay(10); 
} 

//void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
