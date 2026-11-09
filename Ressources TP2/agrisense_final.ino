// ======================================================================
// AgriSense - TP final : l'objet qui mesure, publie et decide
// Six trous a completer, reperes par << TROU n >>.
// Tout le reste est fourni : ne le modifiez pas tant que les six trous
// ne sont pas remplis.
// ======================================================================
#include <Arduino.h>
#include <WiFiNINA.h>
#include <ArduinoMqttClient.h>
#include "secrets.h"        // WIFI_SSID, WIFI_PASS, MQTT_USER, MQTT_PASS

// ---- A personnaliser ------------------------------------------------
const char BROKER[] = "192.168.4.1";   // adresse communiquee ulterieurement
const int  PORT = 1883;

// << TROU 1 >> remplacez gXX par le numero de votre groupe
const char SUJET_MESURES[] = "agrisense/gXX/mesures";
const char SUJET_ETAT[]    = "agrisense/gXX/etat";
const char ID_OBJET[]      = "agrisense-gXX";

const int SONDE     = A0;
const int LED_VANNE = 5;
const int SOL_SEC    = 3100;    // VOS releves du TP1
const int SOL_HUMIDE = 1420;

//vous pourrez modifier a votre guise
const uint32_t PERIODE     = 2000;    // 2 s en TP ; 15 min sur le terrain
const uint32_t DUREE_MAX   = 10000;   // 10 s en TP ; 10 min sur le terrain
const uint32_t DUREE_PAUSE = 20000;   // 20 s en TP ; 30 min sur le terrain
const float    SEUIL_BAS   = 35.0;    // on ouvre en dessous
const float    SEUIL_HAUT  = 50.0;    // on ferme au-dessus

// ---- Automate -------------------------------------------------------
enum Etat { REPOS, IRRIGATION, PAUSE, DEFAUT };
const char* NOM_ETAT[] = { "REPOS", "IRRIGATION", "PAUSE", "DEFAUT" };
Etat etat = REPOS;
uint32_t debutEtat = 0;

WiFiClient reseau;
MqttClient mqtt(reseau);
uint32_t derniereMesure = 0;
uint32_t dernierEssai   = 0;

// ---- Acquisition (TP1, deja fait) -----------------------------------
int mediane9(int* v) {
  for (int i = 1; i < 9; i++) {
    int x = v[i], j = i - 1;
    while (j >= 0 && v[j] > x) { v[j + 1] = v[j]; j--; }
    v[j + 1] = x;
  }
  return v[4];
}

float humiditeSol(int brut) {
  float pct = (float)(SOL_SEC - brut) * 100.0f / (float)(SOL_SEC - SOL_HUMIDE);
  return constrain(pct, 0.0f, 100.0f);
}

// ---- Decision -------------------------------------------------------
// SEUL endroit du programme qui commande la vanne.
void changerEtat(Etat nouveau) {
  etat = nouveau;
  debutEtat = millis();
  digitalWrite(LED_VANNE, etat == IRRIGATION ? HIGH : LOW);
}

void decider(float sol, int q) {
  uint32_t duree = millis() - debutEtat;

  if (q != 0) {                       // mesure invalide : securite (EX2)
    if (etat != DEFAUT) changerEtat(DEFAUT);
    return;
  }

  switch (etat) { //cf. a l'automate
    case REPOS:
      // << TROU 4 >>  
      break;

    case IRRIGATION:
      // << TROU 5 >> :
      //  (EX5)
      break;

    case PAUSE:
      // << TROU 6 >> si duree >= DUREE_PAUSE, revenir en REPOS
      break;

    case DEFAUT:
      changerEtat(REPOS);             // la mesure est redevenue valide
      break;
  }
}

// ---- Reseau ---------------------------------------------------------
bool connecter() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi : connexion...");
    if (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) return false;
    Serial.print("adresse IP : ");    Serial.println(WiFi.localIP());
    Serial.print("signal (dBm) : ");  Serial.println(WiFi.RSSI());
  }

  mqtt.setId(ID_OBJET);

  // << TROU 2 >> 
 

  if (!mqtt.connect(BROKER, PORT)) {
    Serial.print("MQTT refuse, code ");
    Serial.println(mqtt.connectError());
    return false;
  }
  mqtt.beginMessage(SUJET_ETAT, true);   // true : message conserve
  mqtt.print("en ligne");
  mqtt.endMessage();
  Serial.println("MQTT : connecte");
  return true;
}

void setup() {
  pinMode(LED_VANNE, OUTPUT);
  digitalWrite(LED_VANNE, LOW);          // etat sur, avant toute autre chose
  Serial.begin(115200);
  while (!Serial && millis() < 10000) {}
  analogReadResolution(12);
  Serial.println("=== AgriSense : TP final ===");
  connecter();
}

void loop() {
  // Reconnexion sans bloquer la mesure (EX9)
  bool enLigne = mqtt.connected();
  if (!enLigne && millis() - dernierEssai >= 5000) {
    dernierEssai = millis();
    enLigne = connecter();
  }
  if (enLigne) mqtt.poll();              // maintient la liaison ouverte (polling)

  if (millis() - derniereMesure < PERIODE) return;
  derniereMesure = millis();

  int v[9], mini = 4095, maxi = 0;
  for (int i = 0; i < 9; i++) {
    v[i] = analogRead(SONDE);
    if (v[i] < mini) mini = v[i];
    if (v[i] > maxi) maxi = v[i];
    delay(2);
  }
  int brut = mediane9(v);
  int q = 0;
  if (brut < SOL_HUMIDE - 200 || brut > SOL_SEC + 200) q = 1;
  if (maxi - mini > 300) q = 1;
  float sol = humiditeSol(brut);

  decider(sol, q);        // decision locale (reflexe vs reflexion) :  pour assurer EX4

  Serial.print("sol=");    Serial.print(sol, 1);
  Serial.print(" q=");     Serial.print(q);
  Serial.print(" etat=");  Serial.print(NOM_ETAT[etat]);
  Serial.println(enLigne ? " publie" : " hors ligne");

  if (enLigne) {
    mqtt.beginMessage(SUJET_MESURES);
    // << TROU 3 >> composer le message, comme on ecrit sur le moniteur
    // serie, pour obtenir exactement :
    //   {"sol":47.3,"q":0,"etat":"REPOS"}
    // Rappel : mqtt.print("texte"); et mqtt.print(sol, 1);
    // Un guillemet a l'interieur d'une chaine s'ecrit \"
    mqtt.endMessage();
  }
}
