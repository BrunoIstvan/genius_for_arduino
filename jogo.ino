#define LED_VERDE 5
#define LED_AMARELO 4
#define LED_VERMELHO 3
#define LED_AZUL 2
#define BOTAO_VERDE 11
#define BOTAO_AMARELO 10
#define BOTAO_VERMELHO 9
#define BOTAO_AZUL 8
#define UM_SEGUNDO 1000
#define MEIO_SEGUNDO 500
#define INDEFINIDO -1
#define TAMANHO_SEQUENCIA 4
#define POTENCIOMETRO 0
#define BUZZER 7

int sequenciaLuzes[TAMANHO_SEQUENCIA];
int rodada = 0;
int passos_respondidos = 0;

enum ESTADOS {
  PRONTO_PROXIMA_RODADA,
  USUARIO_RESPONDENDO,
  JOGO_FINALIZADO_SUCESSO,
  JOGO_FINALIZADO_FALHA
};


void setup() {
  Serial.begin(9600);
  
  iniciaPortas();
  iniciaBotoes();
  iniciaBuzzer();
  iniciaJogo();
}

void loop() {
  
  switch(estadoAtual()) {
    case PRONTO_PROXIMA_RODADA:
    	Serial.println("Pronto para a proxima rodada");
    	preparaNovaRodada();
    	break;
   	case USUARIO_RESPONDENDO:
    	Serial.println("Jogador respondendo");
		processaRespostaJogador();
    	break;
    case JOGO_FINALIZADO_SUCESSO:
      	Serial.println("Jogo finalizado com sucesso");
    	jogoFinalizadoSucesso();
      	break;
    case JOGO_FINALIZADO_FALHA:
      	Serial.println("Jogo finalizado com falha");
    	jogoFinalizadoFalha();
      	break;
  }

  delay(MEIO_SEGUNDO);
  
}

void preparaNovaRodada() {
  rodada++;
  passos_respondidos = 0;

  if(rodada <= TAMANHO_SEQUENCIA) {
    Serial.print("Rodada #"); 
    Serial.println(rodada);    
  	tocaLedsRodada();
  }
}

void processaRespostaJogador() {
  
  int resposta = leRespostaJogador();
  
  if(resposta == INDEFINIDO) return;
  
  if(resposta == sequenciaLuzes[passos_respondidos]) {
  	passos_respondidos++;
  } else {
    rodada = TAMANHO_SEQUENCIA + 2;
   	Serial.println("Resposta errada"); 
  }
}

int estadoAtual() {
 	
  if(rodada <= TAMANHO_SEQUENCIA) {
    if(passos_respondidos == rodada) {
      	return PRONTO_PROXIMA_RODADA;
    }
    return USUARIO_RESPONDENDO;  
  } else if (rodada == TAMANHO_SEQUENCIA + 1) {
  	return JOGO_FINALIZADO_SUCESSO; 
  }
  return JOGO_FINALIZADO_FALHA;
  
}

void tocaLedsRodada() {
  for(int i = 0; i < rodada; i++) {
    piscaLED(sequenciaLuzes[i]);
  }  
}

int leRespostaJogador() {
 
  if(digitalRead(BOTAO_VERDE) == LOW) {
  	return piscaLED(LED_VERDE);
  } else if(digitalRead(BOTAO_AMARELO) == LOW) {
  	return piscaLED(LED_AMARELO);
  } else if(digitalRead(BOTAO_VERMELHO) == LOW) {
  	return piscaLED(LED_VERMELHO);
  } else if(digitalRead(BOTAO_AZUL) == LOW) {
  	return piscaLED(LED_AZUL);
  } else {
    return INDEFINIDO;
  }
  
}

void iniciaBuzzer() {
  pinMode(BUZZER, OUTPUT);
}

void iniciaBotoes() {
  pinMode(BOTAO_VERDE, INPUT_PULLUP);
  pinMode(BOTAO_AMARELO, INPUT_PULLUP);
  pinMode(BOTAO_VERMELHO, INPUT_PULLUP);
  pinMode(BOTAO_AZUL, INPUT_PULLUP);
}

int sorteiaNumero() {
  return random(LED_AZUL, LED_VERDE+1); // 2-5
}

void iniciaJogo() {
  int jogo = analogRead(POTENCIOMETRO);
  Serial.println(jogo);
  randomSeed(jogo);
  
  for(int i = 0; i < TAMANHO_SEQUENCIA; i++) {
 	sequenciaLuzes[i] = sorteiaNumero();    
  }
}

void iniciaPortas() {
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
}

int piscaLED(int portaLED) {
  
  verificaSomDoLed(portaLED);
  
  digitalWrite(portaLED, HIGH);
  delay(MEIO_SEGUNDO); // Wait for 500 millisecond(s)
  digitalWrite(portaLED, LOW);
  delay(MEIO_SEGUNDO); // Wait for 1000 millisecond(s)
  return portaLED;
}

void jogoFinalizadoSucesso() {
  piscaLED(LED_VERDE);
  piscaLED(LED_AMARELO);
  piscaLED(LED_VERMELHO);
  piscaLED(LED_AZUL);
}

void jogoFinalizadoFalha() {
  
  Serial.println("Voce Perdeu"); 
  tocaSom(300);
  
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AZUL, HIGH);
  delay(UM_SEGUNDO);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AZUL, LOW);
  delay(MEIO_SEGUNDO);
}

void tocaSom(int frequencia) {
  tone(BUZZER, frequencia, 100);
}

void verificaSomDoLed(int portaLed) {
  switch (portaLed) {
    case LED_VERDE:
      tocaSom(2000);
      break;
    case LED_AMARELO:
      tocaSom(2200);
      break;
    case LED_VERMELHO:
      tocaSom(2400);
      break;
    case LED_AZUL:
      tocaSom(2500);
      break;
  }
}