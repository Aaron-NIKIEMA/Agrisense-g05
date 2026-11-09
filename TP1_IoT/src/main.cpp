/*
#include <Arduino.h> //bibliothques
// dclaration
void setup() { // une fois, au demarrage
//code d’initialisation
pinMode(LED_BUILTIN, OUTPUT);
}
void loop() { // repetee sans fin
//code principal.
digitalWrite(LED_BUILTIN, HIGH);
delay(100);
digitalWrite(LED_BUILTIN, LOW);
delay(100);
}
*/

/*
#include <Arduino.h>

const int BOUTON = 2;
uint32_t nbAppuis = 0;
bool etatPrecedent = HIGH;

void setup() {
Serial.begin(115200);
while (!Serial && millis() < 3000) {} // attend le moniteur, 3 s au plus
pinMode(BOUTON, INPUT);
Serial.println("=== AgriSense TP1 ===");
}

void loop() {
bool etat = digitalRead(BOUTON);
if (etatPrecedent == HIGH && etat == LOW) { // passage de 1 a 0 : appui
nbAppuis++;
Serial.print("appuis : ");
Serial.println(nbAppuis);
}
etatPrecedent = etat;
delay(2);
}
*/


/*
#include <Arduino.h>
const int BOUTON = 2;
volatile uint32_t nbAppuis = 0;
void surAppui() { nbAppuis++; } // ISR : la plus courte possible
void setup() {
Serial.begin(115200);
while (!Serial && millis() < 3000) {}
pinMode(BOUTON, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(BOUTON), surAppui, FALLING);
}
void loop() {
static uint32_t affiche = 0;
if (nbAppuis != affiche) {
affiche = nbAppuis;
Serial.print("appuis : ");
Serial.println(affiche);
}
delay(200);
}
*/


/*
#include <Arduino.h>
void setup() {
Serial.begin(115200);
while (!Serial && millis() < 3000) {}
analogReadResolution(12);
}
void loop() {
Serial.print("ADC brut : ");
Serial.println(analogRead(A0));
delay(1000);
}
*/

/*
#include <Arduino.h>

const int SOL_SEC = 2755; // VOTRE releve a l’air libre
const int SOL_HUMIDE = 1250; // VOTRE releve dans l’eau
float humiditeSol(int brut) {
float pct = (float)(SOL_SEC - brut) * 100.0f / (float)(SOL_SEC - SOL_HUMIDE);
return constrain(pct, 0.0f, 100.0f);
}

void setup() {
Serial.begin(115200);
while (!Serial && millis() < 3000) {}
analogReadResolution(12);
}

void loop() {
Serial.print("ADC brut : ");
Serial.print(analogRead(A0));
Serial.print(" - humiditeSol : ");
Serial.println(humiditeSol(analogRead(A0)));
delay(1000);
}   
*/



/*
#include <Arduino.h>

int mediane9(int* v) { // trie le tableau, renvoie la valeur du milieu
for (int i = 1; i < 9; i++) {
int x = v[i], j = i - 1;
while (j >= 0 && v[j] > x) { v[j + 1] = v[j]; j--; }
v[j + 1] = x;
}
return v[4];
}

void setup() {
Serial.begin(115200);
while (!Serial && millis() < 3000) {}
analogReadResolution(12);
}

void loop() {
  int mesures[9];

  // 1. Collecte de 9 mesures successives
  for (int i = 0; i < 9; i++) {
    mesures[i] = analogRead(A0);
    delay(10); // Court délai entre les lectures pour stabiliser l'ADC
  }

  // 2. Calcul et affichage de la médiane
  int valeur_filtree = mediane9(mesures);

  Serial.print("Brut (derniere): ");
  Serial.print(mesures[8]);
  Serial.print(" - Mediane9: ");
  Serial.println(valeur_filtree);

  delay(1000);
}
*/


/*
#include <Arduino.h>

// Définition des seuils de qualité (valeurs à adapter à votre capteur)
const int VAL_MIN = 1250;      // Exemple : valeur minimale calibrée
const int VAL_MAX = 3750;     // Exemple : valeur maximale calibrée (max 4095 en 12 bits)
const int ECART_MAX = 500;    // Écart max toléré entre le min et le max de la rafale

int mediane9(int* v) { // trie le tableau, renvoie la valeur du milieu
  for (int i = 1; i < 9; i++) {
    int x = v[i], j = i - 1;
    while (j >= 0 && v[j] > x) { v[j + 1] = v[j]; j--; }
    v[j + 1] = x;
  }
  return v[4];
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  analogReadResolution(12); // Résolution 12 bits (0 à 4095)
}

void loop() {
  int mesures[9];

  // 1. Collecte des 9 mesures
  for (int i = 0; i < 9; i++) {
    mesures[i] = analogRead(A0);
    delay(5); 
  }

  // 2. Calcul du filtre médian (trie aussi le tableau)
  int valeur_filtree = mediane9(mesures);

  // 3. Évaluation de l'indicateur de qualité q
  int q = 0; 

  // Comme le tableau 'mesures' est maintenant trié par la fonction mediane9 :
  int valeur_min_rafale = mesures[0];
  int valeur_max_rafale = mesures[8];
  int dispersion = valeur_max_rafale - valeur_min_rafale;

  // Condition 1 : Hors de la plage calibrée
  if (valeur_filtree < VAL_MIN || valeur_filtree > VAL_MAX) {
    q = 1;
  }
  // Condition 2 : Échantillons trop dispersés (bruit important ou anomalie)
  else if (dispersion > ECART_MAX) {
    q = 1;
  }

  // 4. Affichage des résultats
  Serial.print("Valeur: ");
  Serial.print(valeur_filtree);
  Serial.print(" | Dispersion: ");
  Serial.print(dispersion);
  Serial.print(" | Qualite q: ");
  Serial.println(q);

  delay(1000);
}
*/


/*
#include <Arduino.h>
#include <Wire.h> //biblio. pilotant le bus I2C
void setup() {
Serial.begin(115200);
while (!Serial) {}
Wire.begin();
for (uint8_t adr = 1; adr < 125; adr++) { //Bus SPI est encode sur 7 bits
Wire.beginTransmission(adr);
if (Wire.endTransmission() == 0) { //si l’esclave repond, il retourne 0
Serial.print("peripherique a l’adresse 0x");
Serial.println(adr, HEX);
}
}
}
void loop() {}
*/

/*
#include <Arduino.h>
#include <Arduino_LSM6DS3.h>
void setup() {
Serial.begin(115200);
while (!Serial && millis() < 3000) {}
if (!IMU.begin()) Serial.println("centrale inertielle absente");
}
void loop() {
float x, y, z;
if (IMU.accelerationAvailable()) {
IMU.readAcceleration(x, y, z); // en g
Serial.print(x); Serial.print(" ");
Serial.print(y); Serial.print(" ");
Serial.println(z);
}
delay(200);
}
*/



/*
#include <Arduino.h>
#include <Wire.h>
#include <math.h> // Inclure la bibliothèque mathématique standard

// Remplacer par l'adresse trouvée lors de votre scan (0x6A ou 0x6B)
#define LSM6DS3_ADDRESS 0x6A 

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  Wire.begin();
  
  // Configuration initiale du LSM6DS3 (exemple d'activation de l'accéléromètre)
  // Registre CTRL1_XL (0x10) -> 0x40 pour activer à 104 Hz, plage +/-2g
  Wire.beginTransmission(LSM6DS3_ADDRESS);
  Wire.write(0x10); 
  Wire.write(0x40); 
  Wire.endTransmission();
}

void loop() {
  // Lecture des 6 registres de données de l'accéléromètre (X, Y, Z)
  // Registre de départ : OUTX_L_XL (0x28)
  Wire.beginTransmission(LSM6DS3_ADDRESS);
  Wire.write(0x28);
  Wire.endTransmission(false);
  
  Wire.requestFrom(LSM6DS3_ADDRESS, 6);
  
  if (Wire.available() == 6) {
    int16_t raw_x = Wire.read() | (Wire.read() << 8);
    int16_t raw_y = Wire.read() | (Wire.read() << 8);
    int16_t raw_z = Wire.read() | (Wire.read() << 8);
    
    // Conversion en variables float pour le calcul géométrique
    float x = (float)raw_x;
    float y = (float)raw_y;
    float z = (float)raw_z;
    
    // --- Calcul de l'inclinaison selon la formule de l'énoncé ---
    float denominateur = sqrt(x*x + y*y + z*z);
    
    // Sécurité pour éviter une division par zéro
    if (denominateur > 0.0) {
      // acos retourne une valeur en radians, on multiplie par RAD_TO_DEG pour l'avoir en degrés
      float inclinaison = acos(z / denominateur) * RAD_TO_DEG;
      
      // Affichage de la mesure
      Serial.print("Inclinaison : ");
      Serial.print(inclinaison);
      Serial.println(" °");
      
      // Gestion de l'alerte au-delà de 30°
      if (inclinaison > 30.0) {
        Serial.println(" ALERTE : Inclinaison supérieure à 30° !");
      }
    }
  }
  
  delay(200); // Pause de 200ms entre chaque lecture
}
*/



/*
#include <Arduino.h>

// --- Configuration des constantes ---
const int ANALOG_PIN = A0;          // Broche de mesure analogique
const float V_IN = 3.3;             // Tension d'alimentation de l'Arduino (5.0V ou 3.3V)
const float R1 = 200.0;           // VALEUR DE VOTRE RÉSISTANCE CONNUE (ex: 10 kΩ = 10000.0 ohms)

void setup() {
  Serial.begin(115200);             // Initialisation du moniteur série
  while (!Serial && millis() < 3000) {}
  Serial.println("--- Ohmmètre Arduino Prêt ---");
}

void loop() {
  // 1. Lecture de la valeur brute sur la broche analogique (0 à 1023)
  int raw_value = analogRead(ANALOG_PIN);
  
  if (raw_value > 0 && raw_value < 1023) {
    // 2. Conversion de la valeur brute en tension (V_out)
    float v_out = (raw_value * V_IN) / 1023.0;
    
    // 3. Calcul de la résistance inconnue (Rx) d'après la formule du pont diviseur
    // Rx = R1 * (V_out / (V_in - V_out))
    float rx = R1 * (v_out / (V_IN - v_out));
    
    // 4. Affichage du résultat sur le Moniteur Série
    Serial.print("Valeur brute : ");
    Serial.print(raw_value);
    Serial.print("  |  Tension : ");
    Serial.print(v_out);
    Serial.print(" V  |  Resistance : ");
    
    if (rx >= 1000.0) {
      Serial.print(rx / 1000.0);
      Serial.println(" kOhms");
    } else {
      Serial.print(rx);
      Serial.println(" Ohms");
    }
  } 
  else if (raw_value >= 1023) {
    Serial.println("Circuit ouvert ou résistance infinie (Rx non connectée).");
  } 
  else {
    Serial.println("Court-circuit détecté (Rx = 0 Ohms).");
  }

  delay(1000); // Pause d'une seconde entre chaque mesure
}
*/



#include <Arduino.h>

const int PIN_VANNE_LED = 5;  // LED câblée sur la broche D5 (supporte le PWM)

// Fonction factice pour simuler l'évolution de la sécheresse du sol (0 à 100 %)
float lireHumiditeSol() {
  static float humidite = 100.0;
  humidite -= 2.0; // Le sol s'assèche progressivement
  if (humidite < 0.0) humidite = 100.0; // Réinitialisation (arrosage)
  return humidite;
}

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {}
  
  pinMode(PIN_VANNE_LED, OUTPUT);
}

void loop() {
  // 1. Récupération de la valeur de l'humidité du sol
  float sol = lireHumiditeSol();
  
  // 2. Affichage sur le moniteur série pour contrôle
  Serial.print("Humidite sol : ");
  Serial.print(sol);
  Serial.println(" %");
  
  // 3. Application de la formule : éteinte (0) à 100%, pleine puissance (255) à 0%
  analogWrite(PIN_VANNE_LED, map((int)sol, 0, 100, 255, 0));
  
  delay(100); // Petite pause pour la lisibilité de l'affichage
}



