#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// Pinos da placa
int IN1 = 13;
int IN2 = 12;
int IN3 = 14;
int IN4 = 27;

int frontLed1 = 25;
int frontLed2 = 33;
int backLed1 = 26;
int backLed2 = 32;
int neonLed1 = 34;

bool laternsLedsOn = false;
bool neonLedsOn = false;

// Configurações PWM
const int frequency = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int pwmChannel3 = 2;
const int pwmChannel4 = 3;
const int resolution = 8;
int originalDutyCycle = 210;
int dutyCycle = originalDutyCycle;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("RedeESP32");
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(frontLed1, OUTPUT);
  pinMode(frontLed2, OUTPUT); 
  pinMode(backLed1, OUTPUT);
  pinMode(backLed2, OUTPUT);
  pinMode(neonLed1, OUTPUT); 

  ledcSetup(pwmChannel1, frequency, resolution);
  ledcSetup(pwmChannel2, frequency, resolution);
  ledcSetup(pwmChannel3, frequency, resolution);
  ledcSetup(pwmChannel4, frequency, resolution);

  ledcAttachPin(IN1, pwmChannel1);
  ledcAttachPin(IN2, pwmChannel2);
  ledcAttachPin(IN3, pwmChannel3);
  ledcAttachPin(IN4, pwmChannel4);

  digitalWrite(frontLed1, LOW);
  digitalWrite(frontLed2, LOW);
  digitalWrite(backLed1, LOW);
  digitalWrite(backLed2, LOW);
  digitalWrite(neonLed1, LOW);

}

void loop() {
  if(Serial.available()) {
    SerialBT.write(Serial.read());
  }

  if(SerialBT.available()) {
    char recebido = SerialBT.read();
    Serial.write(recebido);

    if(laternsLedsOn){
      digitalWrite(frontLed1, HIGH);
      digitalWrite(frontLed2, HIGH);
      digitalWrite(backLed1, HIGH);
      digitalWrite(backLed2, HIGH);
    }

    if(!laternsLedsOn){
      digitalWrite(frontLed1, LOW);
      digitalWrite(frontLed2, LOW);
      digitalWrite(backLed1, LOW);
      digitalWrite(backLed2, LOW);
    }

    if(neonLedsOn){
      digitalWrite(neonLed1, HIGH);
    }    

    if(!neonLedsOn){
      digitalWrite(neonLed1, LOW);
    }

    // Liga as luzes dianteiras
    if(recebido == 'W'){
      laternsLedsOn = true;
    }

    // Desliga as luzes dianteiras
    if(recebido == 'w'){
      laternsLedsOn = false;
    }

    // Liga as luzes traseiras
    if(recebido == 'U'){
      neonLedsOn = true;
    }

    // Desliga as luzes traseiras
    if(recebido == 'u'){
      neonLedsOn = false;
    }

    // Parado
    if(recebido == 'S'){
      ledcWrite(pwmChannel1, 0);
      ledcWrite(pwmChannel2, 0);
      ledcWrite(pwmChannel3, 0);
      ledcWrite(pwmChannel4, 0);
    }

    // Para frente
    if(recebido == 'F'){
      ledcWrite(pwmChannel1, 0);
      ledcWrite(pwmChannel2, dutyCycle);
      ledcWrite(pwmChannel3, dutyCycle);
      ledcWrite(pwmChannel4, 0);
    }

    // Para traz
    if(recebido == 'B'){
      ledcWrite(pwmChannel1, dutyCycle);
      ledcWrite(pwmChannel2, 0);
      ledcWrite(pwmChannel3, 0);
      ledcWrite(pwmChannel4, dutyCycle);
    }

    // Para direita total
    if(recebido == 'R'){
      ledcWrite(pwmChannel1, dutyCycle);
      ledcWrite(pwmChannel2, 0);
      ledcWrite(pwmChannel3, dutyCycle);
      ledcWrite(pwmChannel4, 0);
    }

    // Para esquerda total
    if(recebido == 'L'){
      ledcWrite(pwmChannel1, 0);
      ledcWrite(pwmChannel2, dutyCycle);
      ledcWrite(pwmChannel3, 0);
      ledcWrite(pwmChannel4, dutyCycle);
    }

    // Diagonal esquerda frente
    if(recebido == 'G'){
      ledcWrite(pwmChannel1, 0);
      ledcWrite(pwmChannel2, dutyCycle);
      ledcWrite(pwmChannel3, (dutyCycle-10));
      ledcWrite(pwmChannel4, 0);
    }

    // Diagonal direita frente
    if(recebido == 'I'){
      ledcWrite(pwmChannel1, 0);
      ledcWrite(pwmChannel2, (dutyCycle-10));
      ledcWrite(pwmChannel3, dutyCycle);
      ledcWrite(pwmChannel4, 0);
    }

    // Diagonal esquerda traz
    if(recebido == 'H'){
      ledcWrite(pwmChannel1, dutyCycle);
      ledcWrite(pwmChannel2, 0);
      ledcWrite(pwmChannel3, 0);
      ledcWrite(pwmChannel4, (dutyCycle-10));
    }

    // Diagonal direita traz
    if(recebido == 'J'){
      ledcWrite(pwmChannel1, (dutyCycle-10));
      ledcWrite(pwmChannel2, 0);
      ledcWrite(pwmChannel3, 0);
      ledcWrite(pwmChannel4, dutyCycle);
    }

    if(recebido == '0') {
      dutyCycle = originalDutyCycle;
    }

    if(recebido == '4') {
      dutyCycle = (originalDutyCycle+10);
    }

    if(recebido == '8') {
      dutyCycle = (originalDutyCycle+30);
    }

    if(recebido == 'q') {
      dutyCycle = (originalDutyCycle+45);
    }
  }
}
