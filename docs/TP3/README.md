# Projet AgriSense — recette et synthèse

Ce TP vérifie l'objet construit pendant le TP1 et le TP2. Les
exercices 1 et 2 ont été réalisés et le tableau de tests de la page 3 a été
rempli dans le notebook `docs/1.ipynb`. La synthèse d'architecture est issue
de `docs/synthese.md`.

## Ce qui a été fait

La recette vérifie la périodicité des messages, la mesure invalide, la
publication, la décision hors ligne, la durée maximale d'irrigation,
l'autonomie théorique, l'affichage sur le tableau de bord et la reconnexion.
L'exigence EX8 reste non testée car l'accéléromètre n'est pas programmé dans
le code d'intégration.

Le système comporte quatre étages :

1. sonde d'humidité ;
2. carte Arduino et automate local ;
3. liaison Wi-Fi/MQTT ;
4. tableau de bord ou abonné.

Les priorités de conception sont EX2/EX5, pour éviter une irrigation dangereuse,
et EX4, pour continuer à fonctionner lorsque le broker est arrêté.

## Tableau de tests réalisé

| Exigence | Résultat observé | Verdict |
|---|---|---|
| EX1 | Messages reçus toutes les 2 s ; exemple d'écart 1,997 s. | Conforme |
| EX2 | À 09:00:20, `q=1`, passage à `DEFAUT`, LED éteinte. | Conforme |
| EX3 | À 09:12:54, humidité à 54,9 %, retour à `REPOS`, vanne coupée. | Conforme |
| EX4 | Broker indisponible (code -2), affichage `hors ligne`, automate actif. | Conforme |
| EX5 | Irrigation observée de 09:13:06 à 09:13:16, soit 10 s. | Conforme |
| EX6 | 2600 mAh / 7 mA = 371,4 h, soit environ 15,5 jours. | Conforme |
| EX7 | Les données du groupe g05 sont visibles sur le tableau de bord. | Conforme |
| EX8 | Aucun effet ; l'IMU n'est pas intégrée au programme final. | Non testé |
| EX9 | Reconnexion tentée toutes les 5 s sans bloquer la lecture. | Conforme |

## Limites et amélioration

Le prototype utilise MQTT sans TLS, ne persiste pas la calibration et ne mesure
pas encore la température ni l'état de la batterie. Une version réelle devrait
ajouter un boîtier étanche, un étage de puissance pour la vanne, un watchdog,
la basse consommation, la sécurité TLS et une validation longue durée.

Voir le [rapport Projet](./rapport.pdf).
