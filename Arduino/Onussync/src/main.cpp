#include <Arduino.h>
#include <LiquidCrystal.h>
#include <HX711.h>
#include <SoftwareSerial.h>

// --- CONFIGURAÇÕES DA BALANÇA ---
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 5;
const float FATOR_CALIBRACAO = 691.43;
const float LIMITE_ALERTA_GRAMAS = 50.0; // Ajuste conforme necessário

// --- CONFIGURAÇÕES DO WI-FI E API ---
String SSID = "motog54"; 
String PASSWORD = "caio12345"; 
String SERVER = "ony-test.onrender.com";
String ENDPOINT = "/atualizar";
String PORTA = "443"; 

// --- VARIÁVEIS DE CONTROLE ---
int totalRedes = 0;
int indiceRedeLCD = 0;
String nomeParaMostrar = "";
unsigned long tempoUltimaLeitura = 0;
const long INTERVALO_LEITURA = 5000;
float pesoAtual = 0;
bool alertaEnviado = false;

// --- PINOS E COMPONENTES ---
const int BOTAO_PIN = 6;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
HX711 scale;
SoftwareSerial esp8266(2, 3); 

// --- FUNÇÕES AUXILIARES ---
void enviarComandoAT(String comando, int tempo, bool lerResposta) {
  esp8266.println(comando);
  delay(tempo);
}

void dispararParaAPI(String tipoDeEvento, float pesoDaVez) {
  int porcentagem = (int)(pesoDaVez * 100);
  if (porcentagem > 100) porcentagem = 100;
  if (porcentagem < 0) porcentagem = 0;

  Serial.println("Enviando para API: " + String(porcentagem) + "%");
  
}

void setup() {
  Serial.begin(9600);
  esp8266.begin(9600);
  pinMode(BOTAO_PIN, INPUT_PULLUP);
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(FATOR_CALIBRACAO);
  scale.tare();
  
  lcd.begin(16, 2);
  lcd.print("Iniciando...");
}

void loop() {
  unsigned long tempoAtual = millis();
  
  if (tempoAtual - tempoUltimaLeitura >= INTERVALO_LEITURA) {
     tempoUltimaLeitura = tempoAtual;
     pesoAtual = scale.get_units(20);
     
     lcd.setCursor(0, 1);
     lcd.print("Peso: " + String(pesoAtual, 2) + "g    ");
  }
}