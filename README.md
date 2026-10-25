# Pipeline d'acquisition temps réel sur STM32

Projet personnel pour reprendre le développement C embarqué. Lecture d'une centrale inertielle MPU-6050 avec un STM32F446RE, en bare-metal, en essayant d'appliquer les pratiques du logiciel critique (MISRA C, analyse statique, gestion des erreurs).

Le but final est une chaîne complète capteur, filtre d'attitude, sortie, cadencée à fréquence fixe.

## Matériel et branchements

* Carte NUCLEO-F446RE, horloge interne à 16 MHz
* MPU-6050 via I2C1 (PB8/PB9), à 100 kHz
* USART2 (PA2/PA3) vers le PC par le ST-Link, à 115 200 bauds

## Fonctionnel

* Driver USART en émission.
* Driver I2C : écriture d'un registre, lecture d'un registre, et lecture des 14 octets de mesure (accéléromètre, température et gyroscope) en une seule transaction, pour que toutes les valeurs viennent de la même mesure. Validé sur carte : la norme de l'accélération reste autour de 1 g quelle que soit l'orientation.
* Gestion des erreurs I2C : toutes les attentes sont bornées et les fonctions renvoient un code d'erreur. Le cas nominal est testé sur carte, les tests d'injection de fautes restent à faire.
* Un filtre complémentaire en deux versions : une en C, avec vérification des entrées, et une en Ada.

## Choix

Pas d'utilisation de HAL de ST : les périphériques sont configurés directement par leurs registres, à partir du document RM0390. Écrire les drivers moi-même m'oblige à comprendre les étapes et me permet de plus pratiquer le C.

Le code est vérifié avec cppcheck et l'addon MISRA C:2012. Il reste une déviation, la règle 11.4 : chaque accès registre CMSIS (I2C1->SR1, etc.) convertit une adresse en pointeur, ce qu'on ne peut pas éviter pour accéder au matériel. Ces accès restent dans les drivers.

Les dossiers générés par CubeMX à la création du projet contiennent la HAL, mais mon code ne l'utilise pas. Ce sera retiré du projet plus tard.

## Organisation

```
firmware/Core/Inc/   usart.h, i2c.h, MPU6050.h
firmware/Core/Src/   usart.c, i2c.c, main.c
filtre/C/            filtre complémentaire en C
filtre/Ada/          filtre complémentaire en Ada
```

## La suite

* tests d'injection de fautes sur l'I2C
* gestion des erreurs renvoyées par l'I2C
* timer et interruption pour lire le capteur à 100 Hz
* DMA, puis FreeRTOS avec le filtre sur la carte
* Package d'interface Ada avec C
* CAN

Outils : STM32CubeIDE, Git, GNAT pour Ada, cppcheck, VS Code, Windows \& Ubuntu.

