#include <LiquidCrystal.h>
#include "pitches.h"

#define LED_VERMELHO 7 
#define LED_VERDE 6
#define BUTTONSTOP 5
#define BUTTONPLAY 4
#define BUTTONUP 3
#define BUTTONDOWN 2
#define BUZZER 10

#define MENU 0
#define TOCANDO 1
#define PAUSADO 2

int estado = MENU; // variável que guarda o estado atual, começa no menu

// o loop roda várias vezes quando se aperta o botão 1 vez
// por isso ele precisa saber como era o estado da última iteração
//por conta do pullup os botões estão "desligados" no HIGH
int ultimoPlay = HIGH;
int ultimoStop = HIGH;
int ultimoUp = HIGH;
int ultimoDown = HIGH;

//ENEMY
int melody[] = {
  NOTE_B4, REST,
  NOTE_FS4, NOTE_FS4, NOTE_B4, NOTE_FS4, NOTE_E4, REST, NOTE_B3,

  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_B3, NOTE_B3, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_AS3, NOTE_AS3, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_AS3, NOTE_B3,
  NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_B3, NOTE_B3, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_AS3, NOTE_AS3, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_AS3,

  NOTE_B4, NOTE_A4, NOTE_G4, NOTE_D4, NOTE_FS4, NOTE_E4, NOTE_B4,
  NOTE_B4, NOTE_A4, NOTE_G4, NOTE_D4, NOTE_FS4, NOTE_AS4,

  REST, NOTE_E4, NOTE_FS4, NOTE_E4, NOTE_D4, NOTE_B3,
  NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_B4,
  REST, NOTE_E4, NOTE_FS4, NOTE_E4, NOTE_D4, NOTE_B3,
  NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_B4, REST, NOTE_B3, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_FS3, NOTE_E3, NOTE_FS3, NOTE_FS4, NOTE_B4, NOTE_FS4, NOTE_E4,
  
  REST
};

int durations[] = {
  4, 2,
  4, 8, 4, 8, 4, 2, 8,

  4, 4, 8, 8, 2, 8, 4, 4, 8, 8, 2, 8,
  4, 4, 8, 8, 2, 8, 4, 4, 8, 8, 2, 8,
  4, 4, 8, 8, 2, 8, 4, 4, 8, 8, 2, 8,
  4, 4, 8, 8, 2, 8, 4, 4, 8, 8, 2,

  2, 2, 2, 4, 1, 1, 8,
  2, 2, 2, 4, 1, 1,

  2, 2, 8, 8, 8, 2,
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2,
  2, 2, 8, 8, 8, 2,
  8, 8, 8, 8, 8, 8, 2, 2, 8, 8, 8,
  2, 2, 2, 8, 4, 4, 8, 4, 8, 2,

  1
};

void enemy(){
  int size = sizeof(durations) / sizeof(int);

  for (int note = 0; note < size; note++) {
    //to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int duration = 1000 / durations[note];
    tone(BUZZER, melody[note], duration);

    //to distinguish the notes, set a minimum time between them.
    //the note's duration + 30% seems to work well:
    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    //stop the tone playing:
    noTone(BUZZER);
}

void setup()
{
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BUTTONSTOP, INPUT_PULLUP);
  pinMode(BUTTONPLAY, INPUT_PULLUP);
  pinMode(BUTTONUP, INPUT_PULLUP);
  pinMode(BUTTONDOWN, INPUT_PULLUP);  
  pinMode(BUZZER, OUTPUT); 
}

void loop()
{
  int play = digitalRead(BUTTONPLAY);
  int stop = digitalRead(BUTTONSTOP);
  int up = digitalRead(BUTTONUP);
  int down = digitalRead(BUTTONDOWN);

  
  //BOTÃO PLAY/PAUSE
  // com o && ele só executa quando o botão foi apertado
  // evitando o problema de ser executado várias vezes
  if (play == LOW && ultimoPlay == HIGH) { // botão play/pause -> indica uma transição entre HIGH e LOW
    if (estado == TOCANDO) {
      estado = PAUSADO;
    }
    else {
      estado = TOCANDO;
    }
  } // se tiver pausado, apertando o botão você despausa e vice-versa

  
  //BOTÃO STOP
  if (stop == LOW && ultimoStop == HIGH) { // botão stop que volta ao menu
    estado = MENU;
  }

  // guarda como os botões q estavam nesta volta
  ultimoPlay = play;
  ultimoStop = stop;
  ultimoUp = up;
  ultimoDown = down;

  // LEDS E BUZZER
  if (estado == TOCANDO) {
    digitalWrite(LED_VERDE, HIGH); // liga a luz verde ao tocar
    digitalWrite(LED_VERMELHO, LOW);
    enemy();
  }
  if (estado == PAUSADO) {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH); // ativa a luz vermelha
    noTone(BUZZER);
  }
  if (estado == MENU) { // no menu não precisa de luz
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    noTone(BUZZER);
  }
}
