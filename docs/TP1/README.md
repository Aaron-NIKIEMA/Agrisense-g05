# TP1 — Acquisition, qualification et I2C

Ce README rassemble les travaux réalisés dans les exercices 1, 2 et 4 du TP1.
Ils constituent la base technique du projet AgriSense.

## Exercice 1 — GPIO et interruptions

Le bouton est câblé entre D2 et GND et utilise `INPUT_PULLUP`. Il est donc à
`HIGH` au repos et à `LOW` lors d'un appui. Deux méthodes ont été étudiées :

- la scrutation, qui lit régulièrement `digitalRead()` et peut manquer un
  appui bref lorsque la boucle est ralentie ;
- l'interruption `FALLING`, qui déclenche une ISR même pendant `delay(200)`.

La variable du compteur est `volatile`, l'ISR reste volontairement courte et
l'affichage est effectué dans `loop()`. Le test avec `INPUT` montre qu'une
entrée flottante devient sensible au bruit électrique. Les rebonds mécaniques
peuvent produire plusieurs fronts pour un seul appui ; un anti-rebond temporel
de 20 à 50 ms est donc nécessaire.

## Exercice 2 — ADC, calibration et qualité

La sonde capacitive est reliée à A0, avec VCC sur 3V3 et GND sur GND. La
résolution ADC est configurée à 12 bits (`0..4095`, environ `0,806 mV` par
niveau). Les relevés documentés sont :

| État | Valeur brute |
|---|---:|
| Sonde à l'air libre | 2755 |
| Sonde dans l'eau | 1250 |

La conversion utilisée est :

```text
H(%) = (2755 - valeur_brute) / (2755 - 1250) × 100
```

Le résultat est limité à 0–100 %. Neuf valeurs sont filtrées par médiane. Ce
filtre résiste mieux à un pic isolé que la moyenne. `q=1` si la valeur filtrée
sort de la plage calibrée ou si la dispersion `max-min` dépasse `500`; sinon
`q=0`. Un fil de signal débranché rend A0 flottante et produit des mesures
instables, généralement détectées par `q`.

## Exercice 4 — Datasheet et bus I2C

La centrale inertielle LSM6DS3 est intégrée à la Nano 33 IoT : aucun câblage
externe n'est nécessaire. Sa tension admissible est d'environ 1,71 à 3,6 V,
ses adresses I2C possibles sont `0x6A` et `0x6B`, et ses plages
accélérométriques sont `±2 g`, `±4 g`, `±8 g` et `±16 g`.

Le scanner `Wire` détecte l'adresse du composant. La bibliothèque
`Arduino_LSM6DS3` lit `x`, `y`, `z` en g ; à plat, `x` et `y` sont proches de
zéro et `z` proche de 1 g, car la gravité est mesurée. L'accès direct par
registres permet de lire les six octets d'accélération et de calculer :

```text
inclinaison = acos(z / sqrt(x² + y² + z²))
```

Une alerte peut être déclenchée au-delà de 30 degrés.

## Mise en œuvre

Les programmes sont téléversés sur une Arduino Nano 33 IoT avec le moniteur
série à `115200 bauds`. Les bibliothèques nécessaires sont `Wire` et
`Arduino_LSM6DS3`. La carte fonctionne en 3,3 V : aucune broche ne doit
recevoir 5 V.

Voir le [rapport TP1](./rapport.pdf) pour la synthèse des manipulations et des
exigences couvertes.
