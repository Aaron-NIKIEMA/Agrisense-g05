# TP final — Mesurer, publier et décider

Ce TP reprend les acquis du TP1 et les transforme en objet connecté complet :
mesure de l'humidité, publication MQTT et décision locale lorsque le réseau est
indisponible.

## Réalisation

- Sonde capacitive sur `A1` et LED représentant la vanne sur `D5`.
- Calibration utilisée : `SOL_SEC=3100`, `SOL_HUMIDE=0`.
- Filtre médian sur neuf lectures et indicateur `q`.
- Wi-Fi/MQTT avec broker `172.20.10.2:1883`, client `agrisense-g05`.
- Identifiants dans `secrets.h`, non versionnés.

Topics utilisés :

| Usage | Topic | Retain |
|---|---|---|
| État de connexion | `agrisense/g05/etat` | Oui |
| Mesures et automate | `agrisense/g05/mesures` | Non |

Payload :

```json
{"sol":47.3,"q":0,"etat":"REPOS"}
```

## Automate

`REPOS` ouvre sous 35 %, `IRRIGATION` s'arrête à 50 % ou après 10 secondes,
`PAUSE` impose 20 secondes et `DEFAUT` coupe immédiatement la vanne lorsque
`q=1`. `mqtt.poll()` maintient la connexion et les tentatives de reconnexion
ont lieu toutes les cinq secondes sans bloquer la mesure.

Les essais ont confirmé la mesure périodique, la sécurité capteur, la régulation,
le fonctionnement hors ligne, l'anti-noyade et la reconnexion.
