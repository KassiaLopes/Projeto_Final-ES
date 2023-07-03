#include <Ultrasonic.h>
#include <Password.h> // Biblioteca utilizada para controle de senha
#include <Keypad.h> // Biblioteca para controle do teclado de matrizes
 
#define PINO_LED_VERMELHO 12
#define PINO_LED_VERDE 13

const int echoPin = 3; //pino echo
const int trigPin = 4; //pino trig
 const int buzzer = 2; 
const byte LINHAS = 4; // Linhas do teclado
const byte COLUNAS = 3; // Colunas do teclado

int distancia;

Ultrasonic ultrasonic(trigPin,echoPin); // inicilizando os pinos

Password senha = Password( "3005" ); // Senha utilizada para liberacao
const char TECLAS_MATRIZ[LINHAS][COLUNAS] = { // Matriz de caracteres (mapeamento do teclado)
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

 byte PINOS_LINHAS[LINHAS] = {11,10,9,8}; // Pinos de conexao com as linhas do teclado
 byte PINOS_COLUNAS[COLUNAS] = {7,6,5}; // Pinos de conexao com as colunas do teclado

Keypad teclado_personalizado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS); // Inicia teclado

void setup() {
  
  Serial.begin(9600); // Inicializa serial monitor
  
  pinMode(PINO_LED_VERMELHO, OUTPUT); // Define pino 10 como saida
  pinMode(PINO_LED_VERDE, OUTPUT); // Define pino 11 como saida  
  pinMode(buzzer, OUTPUT);
  pinMode(echoPin, INPUT); 
  pinMode(trigPin, OUTPUT); 
}

void loop() {
    hcsr04(); // chama o metodo "hcsr04()"
 
  if(distancia <= 30){// se a distancia entre o objeto e o sensor for menor que 30cm, aciona o alarme
  tone(buzzer,1500);//aciona o buzzer
  }else{
  noTone(buzzer);//caso não, o buzzer permanece desligado
}
char leitura_teclas = teclado_personalizado.getKey(); // Atribui a variavel a leitura do teclado
  if(leitura_teclas){ // Se alguma tecla foi pressionada
    if(leitura_teclas == '#'){ // Se a tecla '#' foi pressionada  
      if(senha.evaluate()){ // Verifica se a senha digitada esta correta
        Serial.println("Senha correta!"); // Exibe a mensagem que a senha esta correta
        for(int i = 0; i < 5; i++){ // Pisca o LED 5 vezes rapidamente
          digitalWrite(PINO_LED_VERDE, HIGH);
          delay(50);
          digitalWrite(PINO_LED_VERDE, LOW);
          delay(50);
          noTone(buzzer);
          }
      } else { // Caso a senha esteja incorreta
        Serial.println("Senha incorreta!"); // Exibe a mensagem que a senha esta errada
        for(int i = 0; i < 5; i++){ // Pisca o LED 5 vezes rapidamente
          digitalWrite(PINO_LED_VERMELHO, HIGH);
          delay(50);
          digitalWrite(PINO_LED_VERMELHO, LOW);
          delay(50);
          
        }
      }
      senha.reset(); // Limpa a variavel senha
      
    } else { // Caso outra tecla tenha sido pressionada
      
      Serial.println(leitura_teclas); // Exibe a tecla pressionada
      senha.append(leitura_teclas); // Salva o valor da tecla pressionada na variavel senha
      
    }
  }

}

void hcsr04(){
digitalWrite(trigPin, LOW); 
delayMicroseconds(2); 
digitalWrite(trigPin, HIGH); 
delayMicroseconds(10); 
digitalWrite(trigPin, LOW); 
// a funcao ranging faz a conversao de tempo de resposta em centimetros e armazena na variavel distancia
distancia = (ultrasonic.Ranging(CM)); // variavel que armazena o valor da distancia
delay(500); 
}
