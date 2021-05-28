#include "BluetoothSerial.h"
#include <RBDdimmer.h>//
BluetoothSerial SerialBT;

#include <Wire.h>
#include <Adafruit_SSD1306.h> //Libreria especial para la pantalla que uso 
#include <Adafruit_GFX.h> //Libreria generica para oled


#include <Adafruit_MLX90614.h>

#define ANCHO 128 // Cambia cada vez que vea la palabra ancho por el numero 128
#define ALTO 64  // Igual con este por el numero 64

#define OLED_RESET 4 // esto no sera usado pero el objeto debe ser definido porq la libreria lo pide 

Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET); // creamos objeto llamado oled le pasamos los paramatros de inicializacion 


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float sensorValue = 0;


/////////////////////PID
float kp=65;//43.05452335;
float ki=0.03;//0.03759132612;
int T=120;

float r1,r2; 
float  c1;
float  e1;
float  m1;

float c2;
float e2;
float m2;

float mk;
float ck;
float ek;
int rk=30;

float mkT;

int incoming;

////////////////////
//#define USE_SERIAL  SerialUSB //Serial for boards whith USB serial port
#define USE_SERIAL  Serial
#define outputPin  26 //GPI013 
#define zerocross  27 // for boards with CHANGEBLE input pins

dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards
//dimmerLamp dimmer(outputPin); //initialase port for dimmer for MEGA, Leonardo, UNO, Arduino M0, Arduino Zero
int i = 1;
int outVal = 0;

const int relay = 25;
void setup() {
  SerialBT.begin("ESP32");
   Wire.begin(); // inicializacion de comunicacion por I2C

  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // inicializacion de objeto con parametros importantes primero de alimentacion segundo direccion de comunicacion I2C 
  Serial.begin(19200);
  mlx.begin();
  pinMode(relay, OUTPUT);


dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE) 

  
}

void loop() {
//Recivir Datos
  if (SerialBT.available()) 
  {
    incoming = SerialBT.read(); //Read what we receive 

    // separate button ID from button value -> button ID is 10, 20, 30, etc, value is 1 or 0
    int button = floor(incoming / 10);
    int value = incoming % 10;
    
    switch (button) {
      case 1:  
        Serial.print("Button 1:"); Serial.println(value);
        
        if (value == 1)
        {
          rk=rk-1;
          Serial.print("rk "); Serial.println(rk);
        }
        if (rk<20)
        {
          rk=20;
          Serial.print("rk "); Serial.println(rk);
        }
        
        break;
       
      case 2:  
        
        break;
      case 3:  
        Serial.print("Button 3:"); Serial.println(value);
        
        if (value == 1)
        {
          rk=rk+1;
          Serial.print("rk "); Serial.println(rk);
        }
        if (rk>50)
        {
          rk=50;
          Serial.print("rk "); Serial.println(rk);
        }
        
        break;
    }
  }

   sensorValue = mlx.readObjectTempC();
   Serial.print("Tiempo ");
   Serial.println(i);
   Serial.print("Temperatura: ");
   Serial.print(sensorValue);

//
//  SerialBT.print(i);
//  SerialBT.print(",");
//  SerialBT.print(sensorValue);
//  SerialBT.println(";");
if(i==1)
{
ck=sensorValue;
e1=rk-sensorValue;
m1=0.5*((2*kp+T*ki)*e1);

  if (mk>100)
  {
     mk=100;
  }

  if (mk>33.33)
  {
   //  mk=33;
     mkT=33;
  }

  if (mk<33.33)
  {
   //  mk=33;
     mkT=mk;
  }

  if (mk<0)
  {
        mk=0;
        mkT=0;
  }
   
}



if (i%T==0)
{
  ck=sensorValue;
  ek=rk-ck;
  mk=0.5*(2*m1+(2*kp+T*ki)*ek+(T*ki-2*kp)*e1);

 
  if (mk>100)
  {
     mk=100;
  }

  if (mk>33.33)
  {
   //  mk=33;
     mkT=33;
  }

  if (mk<33.33)
  {
   //  mk=33;
     mkT=mk;
  }

  if (mk<0)
  {
        mk=0;
        mkT=0;
  }
  
  m1=mk;
  e1=ek;
  c1=ck;


  m2=m1;
  e2=e1;
  c2=c1;

}

  oled.clearDisplay();        // limpia pantalla
  oled.setTextColor(WHITE);   // establece color al unico disponible debido a que se trata como una pantalla monocromo
  oled.setCursor(0,0);        //colocar el cursor en el pixel que se desea empezar a escribir 
  oled.setTextSize(1);        //Se elige la escala de pixeles que tomara un caracter se puede
  oled.print("Proyecto Control");   // escribe en pantalla el texto
  oled.setCursor (0, 10);    // ubica cursor en coordenas 10,30
  oled.print("mkT:"); 
  oled.setCursor (30, 10);
  //oled.print(mkT); 
  oled.print(15); 
  
  oled.setCursor (70, 10);
  oled.print(rk); 
  oled.setCursor (0, 20);    // ubica cursor en coordenas 10,30
  oled.print("ck:   ek:   mk:");
  oled.setCursor (0, 30); 
  oled.print(ck); 


  
  oled.setCursor (40, 30);
  oled.print(ek);
 
  
  oled.setCursor (80, 30);
  oled.print(mk); 
 
  
  oled.setCursor (0,40);
  oled.print("Temperatura sensada:");
  oled.setCursor (50, 50); 
oled.print(mlx.readObjectTempC()); // para el dato
 
 
  oled.print(" C"); // grados
  oled.display();     // muestra en pantalla todo lo establecido anteriormente
  //T
  SerialBT.print(i);
  SerialBT.print(",");
    //T
  SerialBT.print(sensorValue);
  SerialBT.print(",");
  //ck
  SerialBT.print(rk);
  SerialBT.print(",");
  //ck
  SerialBT.print(ck);
  SerialBT.print(",");
  //mk
  SerialBT.print(mk);
  SerialBT.print(",");
  //ek
  SerialBT.print(ek);
  SerialBT.print(",");
  //mkT
  SerialBT.print(mkT);
  SerialBT.println(";");



if(mkT<33.33){
  digitalWrite(relay, HIGH);
  dimmer.setPower(mkT); // name.setPower(0%-100%)33.33 = 40v
}

  
  
  i = i + 1;

  delay(1000);
  
}
