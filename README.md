# 🏭 SimplyIOTModbusGen

## 🧩 Introduction

Ce projet génère des données réseau synthétiques simplifiées représentant des systèmes industriels, sous forme de **DataFrame**.  
Il a pour objectif de fournir un jeu de données artificiel de base pour **entraîner et tester des modèles de prédiction de séquences**.  
Les données produites ne visent pas à reproduire fidèlement des systèmes réels, mais à **simuler des comportements plausibles** pour des usages exploratoires.  
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

---

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