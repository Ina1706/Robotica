#include "LedControl.h"
#include <Servo.h>
#include<LiquidCrystal.h>
#include<EEPROM.h>
#include<math.h>

#define SERVO_PIN 8
#define POT1 A0
#define POT2 A1
#define PWM 9
#define RS 2
#define E 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LedControl lc = LedControl( 12, 11, 10, 1 );
Servo servo;
LiquidCrystal lcd( RS, E, D4, D5, D6, D7 );

int jocNeterminat = 0;
int jocInDesfasurare = 0;
int nrJucatori = 0;
int jocPierdut = 0;
int nrSetariPotentiometru = 0;
int nrJoc = 0;
int punct; //retine pozitia jucatorului pe matrice, in varianta de un player
long long int timp, asteptare;
int valPot1, valVechePot1; //valorile citite de la potentiometrul 1
int valPot2, valVechePot2; //valorile citite de la potentiometrul 2
double scor;
double highscore = 0;
int predRand = 3; //pentru generarea hartii in varianta de un player
int randNum = 8; //pentru generarea hartii in varianta de un player
int vieti;
int punct1, punct2; //retine pozitia jucatorilor pe matrice, in varianta de doi playeri
int goaleRamase = 0; //numarul de randuri care trebuie lasate goale, pana cand vor aparea obstacole (varianta de doi playeri)
int randVechi = 5;
int stinge = 0; //folosit pentru palpaitul punctelor
int setJocuri = 0; //folosit pentru generarea setului de 5 jocuri in varianta de doi playeri
bool harta1p[8][8] =
{
  {0, 0, 0, 0, 0, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool matriceNumarJucatori[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 1, 1, 1},
  {1, 1, 0, 0, 0, 0, 0, 1},
  {0, 1, 0, 0, 0, 1, 1, 1},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool bifa[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 1, 0, 0},
  {1, 0, 0, 0, 1, 0, 0, 0},
  {0, 1, 0, 1, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool matriceUnPlayer[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 1, 1, 1, 0},
  {0, 1, 1, 0, 1, 0, 0, 1},
  {1, 0, 1, 0, 1, 1, 1, 0},
  {0, 0, 1, 0, 1, 0, 0, 0},
  {0, 0, 1, 0, 1, 0, 0, 0},
  {0, 0, 1, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool matriceDoiPlayeri[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 0, 1, 1, 1, 0},
  {0, 0, 1, 0, 1, 0, 0, 1},
  {1, 1, 1, 0, 1, 1, 1, 0},
  {1, 0, 0, 0, 1, 0, 0, 0},
  {1, 1, 1, 0, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool unu[8][8] =
{
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0},
  {0, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 0, 1, 1, 0, 0, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
};

bool doi[8][8] =
{
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 0, 0, 0, 1, 1, 0},
  {0, 0, 0, 1, 1, 1, 0, 0},
  {0, 0, 1, 1, 1, 0, 0, 0},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 1, 1, 1, 1, 0},
};

bool trei[8][8] =
{
  {0, 0, 1, 1, 1, 1, 0, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 0, 0, 0, 1, 1, 0},
  {0, 0, 0, 1, 1, 1, 0, 0},
  {0, 0, 0, 1, 1, 1, 1, 0},
  {0, 0, 0, 0, 0, 1, 1, 0},
  {0, 1, 1, 0, 0, 1, 1, 0},
  {0, 0, 1, 1, 1, 1, 0, 0},
};

bool pierdut[8][8] =
{
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
};

bool castigat[8][8] =
{
  {1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 1, 0, 0, 1, 0, 1},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1},
};

bool bifatMic[4][4] =
{
  {0, 0, 0, 0},
  {0, 0, 0, 1},
  {1, 0, 1, 0},
  {0, 1, 0, 0}
};

bool harta2p[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool remiza[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 1, 1, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool castigatorPlayer1[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {0, 0, 0, 0, 0, 1, 1, 0},
  {1, 0, 0, 1, 0, 0, 0, 0},
  {0, 1, 1, 0, 1, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool castigatorPlayer2[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 1, 0, 1, 1, 0},
  {0, 0, 0, 0, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 0, 0, 0},
  {1, 0, 0, 1, 1, 0, 0, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool unuMic[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {0, 0, 1, 0, 0, 1, 1, 0},
  {0, 1, 1, 1, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool doiMic[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

bool treiMic[8][8] =
{
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {0, 0, 1, 0, 0, 1, 0, 0},
  {1, 1, 1, 0, 0, 1, 1, 1},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

byte inima[8] =
{
  B00000,
  B00000,
  B11011,
  B11111,
  B01110,
  B00100,
  B00000,
};

void scrieMatrice(bool matrice[8][8], int timp = 0) {
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++)
      lc.setLed(0, 7 - col, row, matrice[row][col]);

  delay(timp); //folosit doar pentru a afisa matrice care furnizeaza informatii despre joc
}

void dreapta() { //folosita pentru setarile initiale ale potentiometrului (pozitionare spre dreapta)
  for (int col = 0; col < 8; col++) {
    lc.setLed(0, col + 2, 3, 1);
    lc.setLed(0, col + 2, 5, 1);
    lc.setLed(0, col, 4, 1);
    lc.setLed(0, col + 1, 4, 1);
    lc.setLed(0, col + 2, 4, 1);
    lc.setLed(0, col + 3, 4, 1);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
    lc.setLed(0, col, 4, 0);
    lc.setLed(0, col + 2, 3, 0);
    lc.setLed(0, col + 2, 5, 0);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
  }
}

void stanga() { //folosita pentru setarile initiale ale potentiometrului (pozitionare spre stanga)

  for (int col = 0; col < 8; col++) {
    lc.setLed(0, 7 - col, 4, 1);
    lc.setLed(0, 7 - col - 2, 3, 1);
    lc.setLed(0, 7 - col - 2, 5, 1);
    lc.setLed(0, 7 - col - 1, 4, 1);
    lc.setLed(0, 7 - col - 2, 4, 1);
    lc.setLed(0, 7 - col - 3, 4, 1);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
    lc.setLed(0, 7 - col, 4, 0);
    lc.setLed(0, 7 - col - 2, 3, 0);
    lc.setLed(0, 7 - col - 2, 5, 0);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
  }
}

void bifat() { //afiseaza informatii pentru setarea initiala potentiometrului
  lc.clearDisplay(0);
  scrieMatrice(bifa, 300); //afisez o matrice timp de 300 ms, nu e in "interiorul" jocului
  lc.clearDisplay(0);
}

void seteazaPotentiometru() { //setarea initiala a potentiometrului principal
  if (nrSetariPotentiometru == nrJoc) { //pentru a se executa instructiunile o singura data la inceputul fiecarui joc
    lcd.clear();
    lcd.print("seteaza");
    lcd.setCursor(0, 1);
    lcd.print("potentiometrul 1");
    lc.clearDisplay(0);
    int setat = 0;
    while (setat == 0) {
      valPot1 = analogRead(POT1);
      Serial.println(valPot1);

      if (valPot1 < 400)
        dreapta();

      if (valPot1 > 700)
        stanga();

      if (valPot1 > 400 && valPot1 < 700) {
        setat = 1;
        bifat();
        nrSetariPotentiometru++;
      }
    }
    lc.clearDisplay(0);
  }
}

void alegeNrJucatori() { //"meniul" de alegere a tipului de joc
  scrieMatrice(matriceNumarJucatori);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alege numarul de");
  lcd.setCursor(0, 1);
  lcd.print("jucatori");

  for (int col = 0; col < 8; col++) { //sageata spre stanga
    lc.setLed(0, 7 - col, 7, 1); 
    lc.setLed(0, 7 - col - 1, 7, 1);
    lc.setLed(0, 7 - col - 2, 7, 1);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
    lc.setLed(0, 7 - col, 7, 0);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
  }

  for (int col = 0; col < 8; col++) { //sageata spre dreapta
    lc.setLed(0, col, 7, 1);
    lc.setLed(0, col + 1, 7, 1);
    lc.setLed(0, col + 2, 7, 1);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
    lc.setLed(0, col, 7, 0);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
  }
}

void restaureazaHarta1p() { //folosita pentru restaurarea hartii, mod de joc 1 player. e apelata de fiecare data cand scade numarul de vieti
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++)
      harta1p[row][col] = 0;

  harta1p[0][4] = harta1p[0][5] = harta1p[0][6] = harta1p[0][7] = 1;
}

void jocSinglePlayer() {
  int stingeText = 0;
  int stingereLed = 0;
  predRand = 2;
  randNum = 8;
  vieti = 2;
  jocPierdut = 0;
  long long timpStingere = millis();
  restaureazaHarta1p();
  scrieMatrice(matriceUnPlayer, 1000);
  treiDoiUnu();
  lc.clearDisplay(0);
  scor = -7; //initializat cu -7 pentru a afisa scorul doar dupa ce apar obstacolele
  valVechePot1 = analogRead(POT1);
  punct = 7;
  long long int timpStart = millis();
  long long int timpCrestere = millis(); 
  int vitezaMiscare = 500; //dificultatea initiala (viteza in ms la care rescriu harta)
  int vitezaConstanta = 0;

  while (jocPierdut == 0) {
    miscare(); //gestioneaza miscarea punctului controlat de potentiometru
    clipeste(timpStingere, stingereLed, punct, 7);

    if (harta1p[7][7 - punct] == 1) { //jucatorul atinge un obstacol
      if (vieti == 0) { //jocul s-a terminat
        lc.clearDisplay(0);
        informatiiJocPierdut1p();
        return;
      } else { //jocul continua si numarul de vieti scade
        lc.clearDisplay(0);
        vieti--;
        pierdutViata();
      }
    }

    if (millis() > timpCrestere + 5000 && vitezaConstanta == 0) { //mareste dificultatea, daca nu a atins dificultatea maxima
      if (vitezaMiscare <= 100)
        vitezaConstanta = 1;

      vitezaMiscare = vitezaMiscare - 30;
      timpCrestere = millis();
    }

    if (millis() > timpStart + vitezaMiscare) { 
      coboaraHarta1p();
      scor++;

      if (scor > 0) { //afiseaza pe lcd informatii despre jocul curent
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("scor: ");
        lcd.print(scor);
        lcd.setCursor(14, 0);
        lcd.print(vieti + 1);
        lcd.write(byte(0));
        lcd.setCursor(0, 1);
        lcd.print("best ");
        lcd.print(highscore);

      }

      scrieMatrice(harta1p); //scrie matricea actualizata de coboaraHarta1p()

      if (stinge > 0) {
        lc.clearDisplay(0);
        delay(50);
        scrieMatrice(harta1p);
        stinge--;
      }

      timpStart = millis();
    }
  }
}

void coboaraHarta1p() { //generarea random a hartii in modul de un player
  for (int row = 0; row < 7; row++)
    for (int col = 0; col < 8; col++) {
      harta1p[7 - row][col] = harta1p[7 - row - 1][col];
    }

  if (predRand > randNum) {
    harta1p[0][predRand - 1] = 1;

    if (predRand > 3)
      harta1p[0][predRand - 4] = 0;

    predRand--;
  } else if (predRand < randNum) {
      harta1p[0][predRand] = 0;
      harta1p[0][predRand - 3] = 1;
      predRand++;
  }

  if (predRand == randNum) {
    randNum = random(3, 9);
  }
}

int miscare() { //gestioneaza miscarea potentiometrului in varianta de un player
                //punctul care simbolizeaza jucatorul se va misca atunci cand valoarea veche salvata se modifica cu 70 de unitati
  valPot1 = analogRead(POT1);

  if (valPot1 > valVechePot1 + 70) {
    lc.setLed(0, punct, 7, 0);

    if (punct == 7) punct = 6;

    punct++;
    lc.setLed(0, punct, 7, 1);
    valVechePot1 = valPot1;
    return 1;
  } else if (valPot1 < valVechePot1 - 70) {
    lc.setLed(0, punct, 7, 0);

    if (punct == 0) punct = 1;

    punct--;
    lc.setLed(0, punct, 7, 1);
    valVechePot1 = valPot1;
    return 1;
  }
}

int informatiiJocPierdut1p() { //afiseaza informatii la finalul jocului de un player si asteapta input de la jucator pentru a reveni la meniu 
  int schimbaText = 0;
  jocInDesfasurare = 0;
  jocPierdut = 1;
  nrJoc++;
  nrJucatori = 0;
  long long timp = millis();

  if (highscore <= scor) {
    scrieMatrice(castigat); 
    EEPROM.put(0, scor);
    highscore = scor;
  } else
    scrieMatrice(pierdut);

  delay(1000); //timpul de asteptare pana la primirea input-ului, nu influenteaza jocul 
  valVechePot1 = analogRead(POT1);

  while (1) { //aici se schimba textul afisat pe lcd, la interval de 2 secunde. se iese din while atunci cand e primit input de la utilizator
    if (schimbaText == 0)
      if (millis() > timp + 2000) {
        if (highscore <= scor) {//infomarii despre scor
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Highscore nou!!!");
          lcd.setCursor(0, 1);
          lcd.print(scor);
        } else {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("scor: ");
          lcd.print(scor);
          lcd.setCursor(0, 1);
          lcd.print("best ");
          lcd.print(highscore);
        }

        timp = millis();
        schimbaText = 1;
      }

    if (schimbaText == 1)
      if (millis() > timp + 2000) { //informatii despre revenirea la meniu
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Misca Pot1");
        lcd.setCursor(0, 1);
        lcd.print("pentru a incepe");
        timp = millis();
        schimbaText = 0;
      }

    if (miscare() == 1) //returneaza 1 cand este schimbata valoarea potentiometrului  
      break;
  }

}

void seteazaPotentiometre2p() { //pozitionarea potentiometrului secundar, initiala inceperii jocului de 2 playeri
  lc.clearDisplay(0);
  bifat2p();
  int setat2 = 0;
  int val1 = analogRead(POT1);
  int val2 = analogRead(POT2);

  while (setat2 == 0) {
    setat2 = 1;

    if (val2 < 800) {
      stanga2p();
      val2 = analogRead(POT2);
      setat2 = 0;
    }

  }
}

void bifat2p() { //afiseaza informatii despre pozitionarea potentiometrului secundar
  for (int col = 0; col < 4; col++)
    for (int rnd = 0; rnd < 4; rnd++) {
      lc.setLed(0, 7 - col, rnd + 2, bifatMic[rnd][col]);
    }
}

void stanga2p() { //sageata spre stanga, afisata pentru setarea initiala a potentiometrului secundar, varianta 2 playeri
  for (int col = 3; col >= 0; col--) {
    lc.setLed(0, col, 3, 1);
    lc.setLed(0, col - 1, 3, 1);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
    lc.setLed(0, col, 3, 0);
    delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
  }

  lc.setLed(0, 0, 3, 0);
  delay(25); //nu influenteaza jocul; folosit pentru afisarea sagetilor
}

void clipeste(long long & timp, int & stingereLed, int punct, int randStingere) { //se ocupa de palpaitul continuu al led-ului care simbolizeaza jucatorul,
                                                                                  //pentru a se diferentia de obstacolele de pe harta
  if (stingereLed == 1) {
    if (millis() > timp + 40) {
      lc.setLed(0, punct, randStingere, 0);
      stingereLed = 0;
      timp = millis();
    }
  }

  if (stingereLed == 0) {
    if (millis() > timp + 40) {
      lc.setLed(0, punct, randStingere, 1);
      timp = millis();
      stingereLed = 1;
    }
  }

}

void pierdutViata() { //restaurarea hartii cand se pierde o viata ("stergerea obstacolelor")
  predRand = 3;
  randNum = 8;

  for (int rnd = 0; rnd < 8; rnd++)
    for (int col = 0; col < 8; col++)
      harta1p[rnd][col] = 0;

  scrieMatrice(harta1p);
}


void restaurareHarta2p() //"sterge" obstacolele la inceputul fiecarui joc
{
  for (int rnd = 0; rnd < 8; rnd++)
      for (int col = 0; col < 8; col++)
        harta2p[rnd][col] = 0;
}

void jocMultiPlayer() {
  int jocuriCastigateP1 = 0;
  int jocuriCastigateP2 = 0;
  scrieMatrice(matriceDoiPlayeri, 1000);
  lc.clearDisplay(0);
  punct1 = 4; //pozitionarea initiala a jucatorului 1 pe harta
  punct2 = 0; //pozitionarea initiala a jucatorului 2 pe harta
  coboaraServo(); //e delimitata suprafata de joc
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Seteaza");
  lcd.setCursor(0, 1);
  lcd.print("potentiometrul 2");
  seteazaPotentiometre2p();
  lcd.clear();
  castigator(-1, 0, 0); // sunt afisate pe lcd informatiile initiale (fiecare jucator are scorul 0 si nu exista castigator) 
  treiDoiUnuMic(); //numaratoarea inversa

  for (setJocuri = 0; setJocuri < 5; setJocuri++) { //se joaca un set de 5 jocuri care se finalizeaza prin castigarea de catre un jucator (nu sunt luate in calcul remizele)
                                                    //la finalul acestui set, castigatorul final va fi acel jucator care are cel mai mic numar de jocuri pierdute
    lc.clearDisplay(0);
    restaurareHarta2p();
    int vitezaJoc = 1000;
    long long timpStingereP1 = millis();
    long long timpStingereP2 = millis();
    int stinsP1, stinsP2;
    stinsP1 = stinsP2 = 0;
    valPot1 = analogRead(POT1);
    valPot2 = analogRead(POT2);
    long long int timpJucat = millis();
    jocNeterminat = 0;

    while (jocNeterminat == 0) {
      clipeste(timpStingereP1, stinsP1, punct1, 7); //palpaitul punctului
      clipeste(timpStingereP2, stinsP2, punct2, 0); //palpaitul punctului
      miscare2p();

      if (harta2p[7][7 - punct1] == 1 && harta2p[0][7 - punct2] == 1) { //cazul "remiza"
        castigator(0, jocuriCastigateP1, jocuriCastigateP2);
        setJocuri--;
      } else {
        if (harta2p[7][7 - punct1] == 1) { //cazul "castigator: jucator 2"
          jocuriCastigateP2++;
          castigator(2, jocuriCastigateP1, jocuriCastigateP2); 
        }

        if (harta2p[0][7 - punct2] == 1) { //cazul "castigator: jucator 1"
          jocuriCastigateP1++;
          castigator(1, jocuriCastigateP1, jocuriCastigateP2);
        }
      }

      if (jocNeterminat == 0)
        if (millis() > timpJucat + vitezaJoc) {
          coboara2p();

          if (vitezaJoc > 180) //se ocupa de cresterea dificultatii; dificultatea maxima este de 180ms
            vitezaJoc = vitezaJoc - 20;

          lc.setLed(0, punct1, 7, 1);
          lc.setLed(0, punct2, 0, 1);
          timpJucat = millis();
        }

    }
  }

  if (jocuriCastigateP1 > jocuriCastigateP2)
    afiseazaCastigator(1, jocuriCastigateP1, jocuriCastigateP2);
  else
    afiseazaCastigator(2, jocuriCastigateP1, jocuriCastigateP2);

  ridicaServo();
}

void afiseazaCastigator(int jucator, int jocuriCastigateP1, int jocuriCastigateP2) {
  for (int nr = 0; nr < 5; nr++) { //se ocupa de palpaitul fetelor (cea fericita si cea trista) la finalul setului de 5 jocuri
    for (int rnd = 0; rnd < 8; rnd++)
      for (int col = 0; col < 8; col++)
        if (jucator == 1)
          lc.setLed(0, 7 - col, rnd, !castigatorPlayer1[rnd][col]);
        else
          lc.setLed(0, 7 - col, rnd, !castigatorPlayer2[rnd][col]);

    delay(300); //folosit pentru afisarea unei matrice; nu influenteaza jocul

    for (int rnd = 0; rnd < 8; rnd++)
      for (int col = 0; col < 8; col++)
        if (jucator == 1)
          lc.setLed(0, 7 - col, rnd, castigatorPlayer1[rnd][col]);
        else
          lc.setLed(0, 7 - col, rnd, castigatorPlayer2[rnd][col]);

    delay(300);  //folosit pentru afisarea unei matrice; nu influenteaza jocul
  }

  valVechePot1 = analogRead(POT1);
  long long timp = millis();
  int schimbareText = 0;
  jocInDesfasurare = 0;
  jocPierdut = 1;
  nrJoc++;
  nrJucatori = 0;

  while (1) {//schimba texul de pe lcd, la interval de 2 secunde; se iese din while atunci cand e primit input de la jucator
    if (schimbareText == 0) //afiseaza castigatorii
      if (millis() > timp + 2000) {
        castigator(-1, jocuriCastigateP1, jocuriCastigateP2); 
        timp = millis();
        schimbareText = 1;
      }

    if (schimbareText == 1) //afiseaza informatii despre revenirea la meniu
      if (millis() > timp + 2000) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Misca Pot1");
        lcd.setCursor(0, 1);
        lcd.print("pentru a incepe");
        timp = millis();
        schimbareText = 0;
      }

    if (miscare() == 1)
      break;
  }
}

void treiDoiUnuMic() { //numaratoarea inversa initiala inceperii jocului de 2 playeri
  int timp = 1000;
  scrieMatrice(treiMic, timp);
  scrieMatrice(doiMic, timp);
  scrieMatrice(unuMic, timp);
}

void castigator(int i, int jocuriCastigateP1, int jocuriCastigateP2) { //afiseaza informatii despre jocurile castigate de fiecare jucator
  if (i == 1) {
    scrieMatrice(castigatorPlayer1, 1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("castigator: ");
    lcd.setCursor(0, 1);
    lcd.print("jucatorul 1");
  }

  if (i == 2)

  {
    scrieMatrice(castigatorPlayer2, 1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("castigator: ");
    lcd.setCursor(0, 1);
    lcd.print("jucatorul 2");
  }

  if (i == 0)

  {
    scrieMatrice(remiza, 1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("remiza");
  }

  jocNeterminat = 1;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("jucator 1: ");
  lcd.print(jocuriCastigateP1);
  lcd.setCursor(0, 1);
  lcd.print("jucator 2: ");
  lcd.print(jocuriCastigateP2);
}

void coboara2p() { //genenereaza random obstacolele, pentru modul de doi jucatori
  int randVechi = 5;

  for (int rnd = 7; rnd >= 0; rnd--)
    for (int col = 3; col >= 0; col--) {
      harta2p[rnd][col] = harta2p[rnd - 1][col];

      if (rnd == 0)
        harta2p[rnd][col] = 0;
    }

  for (int rnd = 0; rnd < 8; rnd++)
    for (int col = 4; col < 8; col++) {
      harta2p[rnd][col] = harta2p[rnd + 1][col];

      if (rnd == 7)
        harta2p[rnd][col] = 0;
    }

  if (goaleRamase != 0) { //numara cate randuri lasa goale pana la aparitia unui obstacol nou
    scrieMatrice(harta2p);
    goaleRamase--;
    return;
  }

  int nrRand = random(0, 4); //genereaza obstacolul

  while (nrRand == randVechi)
    nrRand = random(0, 4);

  for (int rnd = 0; rnd < 4; rnd++) {
    harta2p[0][rnd] = 1;
    harta2p[7][7 - rnd] = 1;
  }

  randVechi = nrRand;
  harta2p[0][nrRand] = 0;
  harta2p[7][7 - nrRand] = 0;
  goaleRamase = 3;
  scrieMatrice(harta2p);
}

void miscare2p() { //gestioneaza miscarea celor doi jucatori pe harta, in modul de 2 playeri
                   //la fel ca in cazul unui singur jucator, miscarea se realizeaza la modificarea cu 70 de unitati a valorilor primite de la potentiometre 
  
  //gestionare potentiometru 1
  valPot1 = analogRead(POT1);

  if (valPot1 > valVechePot1 + 70) {
    lc.setLed(0, punct1, 7, 0);

    if (punct1 == 7) punct1 = 6;

    punct1++;
    lc.setLed(0, punct1, 7, 1);
    valVechePot1 = valPot1;
  } else if (valPot1 < valVechePot1 - 70) {
    lc.setLed(0, punct1, 7, 0);

    if (punct1 == 4) punct1 = 5;

    punct1--;
    lc.setLed(0, punct1, 7, 1);
    valVechePot1 = valPot1;
  }

  //gestionare potentiometru 2
  valPot2 = analogRead(POT2);

  if (valPot2 < valVechePot2 - 70) {
    lc.setLed(0, punct2, 0, 0);

    if (punct2 == 3) punct2 = 2;

    punct2++;
    lc.setLed(0, punct2, 0, 1);
    valVechePot2 = valPot2;
  } else if (valPot2 > valVechePot2 + 70) {
    lc.setLed(0, punct2, 0, 0);

    if (punct2 == 0) punct2 = 1;

    punct2--;
    lc.setLed(0, punct2, 0, 1);
    valVechePot2 = valPot2;
  }

}

void coboaraServo() { //modifica pozitia elicei si apoi opreste alimentarea servo-ului, pentru a disparea vibratia care "deruteaza" potentiometrele
  servo.attach(SERVO_PIN);
  servo.write(125);
  delay(1000); //e folosit pentru a asteapta terminarea miscarii servo-ului
  servo.detach(); //ii opresc alimentarea, pentru a se opri din vibrat (altfel, apar probleme la potentiometre)
}

void treiDoiUnu() { //numaratoarea inversa, initiala jocului in modul de 2 playeri
  int timpDelay = 1000;
  scrieMatrice(trei, timpDelay);
  scrieMatrice(doi, timpDelay);
  scrieMatrice(unu, timpDelay);
}

void ridicaServo() { //modifica pozitia elicei si apoi opreste alimentarea servo-ului, pentru a disparea vibratia care "deruteaza" potentiometrele
  servo.attach(SERVO_PIN);
  servo.write(0);
  delay(1000); //e folosit pentru a asteapta terminarea miscarii servo-ului
  servo.detach(); //ii opresc alimentarea, pentru a se opri din vibrat (altfel, apar probleme la potentiometre)
}

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 10);
  lc.clearDisplay(0);
  randomSeed(analogRead(0));
  ridicaServo();
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.createChar(0, inima);
  pinMode(PWM, OUTPUT);
  analogWrite(PWM, 60);
  Serial.begin(9600);
  EEPROM.get(0, highscore);

  if (isnan(highscore))
    highscore = 0;
}

void loop() {
  if (jocInDesfasurare == 0) {
    seteazaPotentiometru();
    alegeNrJucatori();
    valPot1 = analogRead(POT1);

    if (valPot1 < 300) {
      jocInDesfasurare = 1;
      nrJucatori = 2;
    }

    if (valPot1 > 800) {
      jocInDesfasurare = 1;
      nrJucatori = 1;
    }

    if (nrJucatori == 1) {
      lcd.clear();
      lcd.print("un jucator");
      delay(1000); //folosit pentru a afisa o matrice; nu influenteaza jocul
      jocSinglePlayer();
    }

    if (nrJucatori == 2) {
      lcd.clear();
      lcd.print("doi jucatori");
      delay(1000);  //folosit pentru a afisa o matrice; nu influenteaza jocul
      jocMultiPlayer();
    }
  }
}
