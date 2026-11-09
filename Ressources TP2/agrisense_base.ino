// ======================================================================
// AgriSense - fichier de depart du TP final
// Reprend tout ce qui a ete fait au TP1 : lecture de la sonde, filtre
// median, calibration, indicateur de qualite.
// A FAIRE : remplacer SOL_SEC et SOL_HUMIDE par VOS deux releves du TP1.
// ======================================================================
#include <Arduino.h>

// ---- Broches --------------------------------------------------------
const int SONDE     = A0;   // sortie de la sonde (ou curseur du potentiometre)
const int LED_VANNE = 5;    // LED + resistance 220 ohms vers GND

// ---- VOS valeurs de calibration du TP1 ------------------------------
const int SOL_SEC    = 3100;   // releve a l'air libre
const int SOL_HUMIDE = 1420;   // releve dans l'eau

// ---- Reglages -------------------------------------------------------
const uint32_t PERIODE = 2000;      // 2 s en TP ; 15 min sur le terrain
uint32_t derniereMesure = 0;

// Trie le tableau et renvoie la valeur du milieu (TP1, exercice 2)
int mediane9(int* v) {
  for (int i = 1; i < 9; i++) {
    int x = v[i], j = i - 1;
    while (j >= 0 && v[j] > x) { v[j + 1] = v[j]; j--; }
    v[j + 1] = x;
  }
  return v[4];
}

// Question : Que fait cette fonction ?
float humiditeSol(int brut) {
  float pct = (float)(SOL_SEC - brut) * 100.0f / (float)(SOL_SEC - SOL_HUMIDE);
  return constrain(pct, 0.0f, 100.0f);
}

void setup() {
  pinMode(LED_VANNE, OUTPUT);
  digitalWrite(LED_VANNE, LOW);          // etat sur, avant toute autre chose
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}  // attend le moniteur, 3 s au plus
  analogReadResolution(12);
  Serial.println("=== AgriSense : base du TP1 ===");
}

void loop() {
  if (millis() - derniereMesure < PERIODE) return;
  derniereMesure = millis();

  // Neuf echantillons : on garde la mediane, et on note la dispersion
  int v[9], mini = 4095, maxi = 0;
  for (int i = 0; i < 9; i++) {
    v[i] = analogRead(SONDE);
    if (v[i] < mini) mini = v[i];
    if (v[i] > maxi) maxi = v[i];
    delay(2);
  }
  int brut = mediane9(v);

  // Indicateur de qualite (EX2) : 0 = mesure valide, 1 = mesure suspecte
  int q = 0;
  if (brut < SOL_HUMIDE - 200 || brut > SOL_SEC + 200) q = 1;  // hors plage
  if (maxi - mini > 300) q = 1;                                // trop disperse

  float sol = humiditeSol(brut);

  Serial.print("sol=");  Serial.print(sol, 1);
  Serial.print(" q=");   Serial.println(q);
}
