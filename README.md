# 🏭 SimplyIOTModbusGen

## 🧩 Introduction

Dans le cadre de mon projet professionnel, j’ai besoin de travailler sur des données provenant de **systèmes industriels réels** pour entraîner des IDS (systèmes de détection d’intrusion).  
Or, le manque de jeux de données adaptés constitue un problème majeur : les bases publiques existantes, comme **UNSW‑NB15** ou **CIC datasets**, ne reflètent que partiellement le comportement réel des systèmes industriels et n’intègrent pas la logique séquentielle spécifique à ce type d’infrastructure.  

Pour pallier ce manque, je participe entre autre à développer en interne un **système intelligent de génération de données industrielles**. 
Cependant, pour pouvoir commencer à entraîner et tester les modèles de manière rapide, il m’a semblé utile de disposer d’un générateur **personnel, open/privé**, capable de produire des données synthétiques **respectant une logique séquentielle proche de celle des systèmes industriels réels**.  Même si celles-ci demeurent très simplistes.

Ce projet personnel permet donc de générer **des séquences de symboles labellisées**, avec un niveau de détail suffisant pour expérimenter des modèles de prédiction de séquences et de détection d’anomalies.  
Il offre une base flexible pour prototyper rapidement des algorithmes IDS, tout en restant totalement indépendant de données sensibles ou confidentielles.  

Ce projet génère des données réseau synthétiques simplifiées représentant des systèmes industriels, sous forme de **DataFrame**.  
Il a pour objectif de fournir un jeu de données artificiel de base pour **entraîner et tester des modèles de prédiction de séquences**.  
Les données produites ne visent pas à reproduire fidèlement des systèmes réels, mais à **simuler des comportements plausibles** pour des usages exploratoires.  du moins d'un point de vue séquentielle.
Ce générateur facilite le **prototypage rapide d’algorithmes** sans dépendre de données industrielles confidentielles.  
⚠️ **Attention :** ces données ne remplacent pas des données réelles dans un contexte de production.

---

## ⚙️ Structure du projet

Le projet est divisé en **deux modules principaux** :
1. **Génération** (`Generation`)
2. **Construction de symboles** (`SymbolConstruction`) 

---

## 🔹 1. Module de Génération

### 🎯 Objectif
Ce module a pour but de **simuler le trafic réseau d’un système industriel miniature** fondé sur une logique **Modbus**.  
L’inspiration provient d’une **architecture industrielle réduite** où les échanges suivent une logique maître–esclave.

### 🧠 Principes de fonctionnement
- **Protocoles pris en charge :**  
  Cinq protocoles majeurs sont actuellement utilisés : `Modbus`, `ICMP`, `ARP`, `NTP`, et `SNMP`.  
- **Logique maître–esclave :**  
  Le maître agit comme un **client unique** communiquant avec un ensemble de **serveurs/esclaves**.  
- **Fréquences d’échanges :**  
  Chaque communication entre le maître et un esclave définit **un intervalle d’envoi de requêtes** pour chacun des cinq protocoles.  
  Ces intervalles sont **définis aléatoirement** selon une logique de référence inspirée des systèmes industriels réels.  
- **Données générées :**  
  L’algorithme peut produire :
  - des **bases de données “saines”** (sans anomalies)  
  - des **bases de données contenant des anomalies simulées**  
  👉 Le détail des options de génération est précisé dans le **Makefile** du dossier correspondant.  
- **Adressage réseau :**  
  Les **adresses IP et MAC** sont générées aléatoirement.  
  Une future mise à jour permettra de **définir manuellement** ces adresses pour un meilleur contrôle de la simulation.

### 📊 Variables générées

| Variable | Description |
|----------|-------------|
| `id` | Numéro unique du paquet dans la séquence |
| `time` | Temps écoulé depuis la réception du paquet précédent (en secondes ou millisecondes) |
| `mac_src` | Adresse MAC source du paquet (format `XX:XX:XX:XX:XX:XX`) |
| `mac_dst` | Adresse MAC de destination (format `XX:XX:XX:XX:XX:XX`) |
| `ip_src` | Adresse IP source (format `XXX.XXX.XXX.XXX`) |
| `ip_dst` | Adresse IP de destination (format `XXX.XXX.XXX.XXX`) |
| `pck_length` | Taille totale du paquet en octets |
| `port_src` | Port source du protocole transport (si applicable) |
| `port_dst` | Port de destination du protocole transport (si applicable) |
| `transport` | Protocole de transport utilisé (`TCP`, `UDP`, etc.) |
| `direction` | 1 si Master --> Esclave, 0 Si Eslave --> Master |
| `payload` | Contenu ou charge utile du paquet, si disponible |
| `protocol` | Protocole réseau utilisé (`Modbus`, `ICMP`, `ARP`, `NTP`, `SNMP`) |
| `label` | Label d’anomalie correspondant au comportement du paquet (voir tableau des anomalies) |


---

### ⚠️ Familles d’anomalies
Les anomalies sont classées en **deux familles** :  

1. **Famille 1 – Machines prévues (présentes dans la base “safe”)**  
   Ces machines adoptent un **comportement anormal** ponctuel tout en restant connues du système.  
   Les anomalies de cette famille portent les labels **1 à 5**.

2. **Famille 2 – Machines inconnues**  
   Machines **non présentes dans la base “safe”**.  
   Elles peuvent soit **reproduire un comportement normal**, soit être **attaquantes**.  
   Les anomalies de cette famille portent les labels **100 à 105**.

### 📝 Tableau des labels d’anomalies

| Label | Description | Type de machine |
|-------|------------|----------------|
| 0     | Pas d’anomalie |  connue |
| 1     | Petit flood ICMP |  connue |
| 2     | Petit port scan |  connue |
| 3     | RST flood |  connue |
| 4     | Pas de réponse à une requête Modbus |  connue |
| 5     | Modbus esclaves → maître sans requête |  connue |
| 100   | copie le comportement safe |  inconnue |
| 101   | ICMP flood |  inconnue |
| 102   | Port scan |  inconnue |
| 103   | RST flood |  inconnue |
| 104   | Pas de réponse à une requête Modbus |  inconnue |
| 105   | Modbus esclaves → maître sans requête |  inconnue |

### ▶️ Mode Générateur — paramètres demandés

Lorsque le logiciel est lancé en **mode générateur**, il demande un ensemble de paramètres pour construire le scénario. Ces paramètres pilotent la durée, la vitesse d'exécution (accélération), le nombre et la répartition des machines (maîtres, esclaves, attaquants) ainsi que le chemin de sortie pour les fichiers générés.

| Valeur | Description |
|--------|-------------|
| `duration_scenario` | Durée totale du scénario en **secondes** (durée simulée). |
| `ratio_duration` | Ratio d'accélération du scénario (ex. `0.01` → le scénario s'exécute **100× plus vite** que le temps réel). |
| `path` | Chemin (sans extension) vers le fichier de sortie où seront écrites les bases de données générées. |
| `nb_msf` | **Nombre de maîtres** (master devices / clients) dans la topologie. |

> ⚠️ **Remarque importante sur la dépendance maître / esclaves / attaquants**  
> Après la définition du nombre de maîtres (`nb_msf`), les autres paramètres de topologie (nombre d'esclaves, nombre de machines “inconnues”/attaquantes) sont interprétés **par rapport aux maîtres** : chaque maître possède son propre ensemble d'esclaves et d'éventuels attaquants. 

| Valeur | Description |
|--------|-------------|
| `nb_slaves` | **Nombre total d'esclaves** (devices connus attendus dans la base *safe*). Ces esclaves adoptent par défaut un comportement normal pour la base *safe* ; ils peuvent recevoir des anomalies de la **famille 1** (machines prévues). La valeur est répartie entre les `nb_msf` maîtres (répartition uniforme si non précisée). |
| `nb_attackers` | **Nombre total de machines inconnues / attaquantes** (machines non présentes dans la base *safe*). Elles sont réparties entre les maîtres et correspondent aux machines de la **famille 2** (anomalies 100–105) ou à des machines inconnues se comportant normalement (label 100). |

#### Exemple d'interprétation
- Si `nb_msf = 1`, `nb_slaves = 5`, `nb_attackers = 2` → le maître unique aura 5 esclaves connus et 2 machines inconnues/attaquantes.  


## 🔹 2. Module de Construction de symboles

### 🎯 Objectif
Ce module prend en **entrée** les données générées par le module de génération — ou tout autre jeu de données respectant le même format de colonnes.  
Son rôle est d’enrichir ces données en ajoutant une nouvelle colonne : **`symbole`**.

### 🧩 Principe
Chaque **symbole** représente l’**unicité d’un n-uplet** présent dans la base de données.  
Ainsi, **deux paquets identiques** (même combinaison de valeurs sur les colonnes clés) **auront le même symbole**.  
Ce mécanisme permet de convertir une séquence de paquets en une séquence symbolique exploitable par des modèles de prédiction ou de détection de motifs.

### ⚙️ Détails de traitement
- Les **variables numériques** (telles que :
  `packet_length`, `payload_size`, `src_port`, `dst_port`)  
  sont **discrétisées** afin de **réduire le nombre total de symboles** et de simplifier la représentation.  
- La logique de discrétisation est conçue pour **préserver les comportements caractéristiques** tout en limitant la granularité inutile.  
- Le module retourne un **DataFrame enrichi** contenant l’ensemble des colonnes d’origine + la colonne `symbole`.

---
## 📄 Licence
Projet distribué sous licence **MIT** 

---