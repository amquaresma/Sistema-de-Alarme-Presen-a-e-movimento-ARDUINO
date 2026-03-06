#include <LiquidCrystal.h>
//Si Sistema de Alarme - sensor de presença e  movimento.
//©Grupo Door - Todos Direitos Reservados
//Matheus Quaresma, Sophia Marques, Joaão V. Queiroz, Maria Arevalo, Tales Jardim - ETEC Taboão da Serra, 2025.
const int pinPIR = 8;          
const int pinBuzzer = 9;       
const int pinLCD_RS = 12;       
const int pinLCD_E = 11;        
const int pinLCD_D4 = 5;        
const int pinLCD_D5 = 4;        
const int pinLCD_D6 = 3;        
const int pinLCD_D7 = 2;        
const int pinBotaoGrave = 6;    
const int pinBotaoMedio = 7;    
const int pinBotaoAgudo = 10;   
const int pinLED = 13;         
const int pinFotorresistor = A0;


const int freqGrave = 200;      
const int freqMedio = 600;     
const int freqAgudo = 1000;     

int freqAtual = freqMedio;      
bool movimentoDetectado = false;
bool alarmeAtivado = false;     
bool noite = false;             
unsigned long ultimoBotaoPress = 0; 
const unsigned long tempoDisplayBotao = 2000; 

bool lastButtonGrave = LOW;
bool lastButtonMedio = LOW;
bool lastButtonAgudo = LOW;

LiquidCrystal lcd(pinLCD_RS, pinLCD_E, pinLCD_D4, pinLCD_D5, pinLCD_D6, pinLCD_D7);

void setup() {
  pinMode(pinPIR, INPUT);
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinBotaoGrave, INPUT);
  pinMode(pinBotaoMedio, INPUT);
  pinMode(pinBotaoAgudo, INPUT);
  pinMode(pinFotorresistor, INPUT);
  pinMode(pinLED, OUTPUT);

  
  lcd.begin(16, 2);
  atualizarDisplayStatus();
}

void loop() {
  
  int valorFotorresistor = analogRead(pinFotorresistor);
  noite = (valorFotorresistor < 500);

  
  verificarBotoes();

  if (noite) {
    
    movimentoDetectado = (digitalRead(pinPIR) == HIGH);

    if (movimentoDetectado && !alarmeAtivado) {
      ativarAlarme();
    } 
    else if (!movimentoDetectado && alarmeAtivado) {
      desativarAlarme();
    }
  } else {
    
    if (alarmeAtivado) {
      desativarAlarme();
    }
  }

  
  if (millis() - ultimoBotaoPress > tempoDisplayBotao && !alarmeAtivado) {
    atualizarDisplayStatus();
  }

  delay(50);
}

void verificarBotoes() {
  bool currentButtonGrave = digitalRead(pinBotaoGrave);
  bool currentButtonMedio = digitalRead(pinBotaoMedio);
  bool currentButtonAgudo = digitalRead(pinBotaoAgudo);

  
  if (currentButtonGrave == HIGH && lastButtonGrave == LOW && noite) {
    freqAtual = freqGrave;
    lcd.setCursor(0, 0);
    lcd.print("Garagem      ");
    lcd.setCursor(0, 1);
    lcd.print("Selecionado    ");
    ativarAlarmeTemporario();
    ultimoBotaoPress = millis();
  }
  
  if (currentButtonMedio == HIGH && lastButtonMedio == LOW && noite) {
    freqAtual = freqMedio;
    lcd.setCursor(0, 0);
    lcd.print("Porta      ");
    lcd.setCursor(0, 1);
    lcd.print("Selecionado    ");
    ativarAlarmeTemporario();
    ultimoBotaoPress = millis();
  }
  
  if (currentButtonAgudo == HIGH && lastButtonAgudo == LOW && noite) {
    freqAtual = freqAgudo;
    lcd.setCursor(0, 0);
    lcd.print("Janela      ");
    lcd.setCursor(0, 1);
    lcd.print("Selecionado    ");
    ativarAlarmeTemporario();
    ultimoBotaoPress = millis();
  }

  lastButtonGrave = currentButtonGrave;
  lastButtonMedio = currentButtonMedio;
  lastButtonAgudo = currentButtonAgudo;
}

void atualizarDisplayStatus() {
  lcd.setCursor(0, 0);
  if (noite) {
    lcd.print("Modo Noturno   ");
    lcd.setCursor(0, 1);
    lcd.print("Alarme Ativo   ");
  } else {
    lcd.print("Modo Diurno    ");
    lcd.setCursor(0, 1);
    lcd.print("Alarme Inativo ");
  }
}

void ativarAlarmeTemporario() {
  if (noite) { 
    tone(pinBuzzer, freqAtual, 100);
    digitalWrite(pinLED, HIGH);
    delay(100);
    noTone(pinBuzzer);
    digitalWrite(pinLED, LOW);
  }
}

void ativarAlarme() {
  lcd.setCursor(0, 0);
  lcd.print("ALERTA!!!        ");
  lcd.setCursor(0, 1);
  lcd.print("MOVIMENTO!!!");
  tone(pinBuzzer, freqAtual);
  digitalWrite(pinLED, HIGH);
  alarmeAtivado = true;
}

void desativarAlarme() {
  noTone(pinBuzzer);
  digitalWrite(pinLED, LOW);
  alarmeAtivado = false;
  atualizarDisplayStatus();
}