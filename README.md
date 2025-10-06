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
  👉 Le détail des options de génération est précisé dans un tableau ci dessous 
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

#### Remarques
- Les fichiers `anomalies` conservent l’identification des esclaves de la base `safe` afin de maintenir la continuité comportementale ; les anomalies sont appliquées selon des probabilités paramétrables (par type d’anomalie).  
- Les fichiers sont des CSV classiques (`,`), encodés en **UTF‑8**, et portent l’extension `.txt` par convention de sortie.  
- Les colonnes et le format exact (noms de colonnes, ordre) suivent la table **Variables générées** présente dans ce README.


### 📝 TODO / Améliorations à venir

- Implémenter et intégrer les **logiques d’établissement de connexion TCP** (handshake) pour tous les protocoles utilisant TCP.  
- Améliorer la **taille des paquets**, en particulier pour les charges utiles, afin de mieux refléter les caractéristiques réelles des systèmes industriels.  
- Ajouter **d’autres protocoles** fréquemment utilisés dans les systèmes industriels fermés.  
- Introduire de **nouvelles anomalies** pour enrichir les scénarios de test et la diversité des comportements anormaux.

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
| 1     | ICMP flood |  connue |
| 2     | Port scan |  connue |
| 3     | RST flood |  connue |
| 4     | Pas de réponse à une requête Modbus |  connue |
| 5     | Modbus esclaves → maître sans requête |  connue |
| 100   | copie le comportement safe |  inconnue |
| 101   | ICMP flood |  inconnue |
| 102   | Port scan |  inconnue |
| 103   | RST flood |  inconnue |
| 104   | Pas de réponse à une requête Modbus |  inconnue |
| 105   | Modbus esclaves → maître sans requête |  inconnue |

### 🎲 Gestion de l’aléatoire

Le générateur utilise des valeurs aléatoires à plusieurs niveaux pour assurer la **diversité des scénarios**.  
- Dans les bases *safe*, les timers, intervalles d’envoi et adresses (IP/MAC) sont générés aléatoirement afin que chaque exécution produise des séquences légèrement différentes même avec les mêmes paramètres.  
- Dans les bases *anomalies*, l’aléatoire est également utilisé pour déterminer **l’apparition, le timing et la répartition des anomalies**, garantissant des scénarios variés à chaque génération.  

Cette approche permet de créer des jeux de données réalistes et reproductibles en terme de format, tout en offrant **une diversité importante pour l’entraînement et le test des modèles**.


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

### 🧩 Logique de construction (basée sur des timers)

La génération des séquences est **entièrement pilotée par des timers** : j’ai volontairement choisi cette approche plutôt que d’affecter des temps manuellement afin de reproduire un comportement temporel réaliste et modulable (accélération, superposition d’échanges, etc.).  

Concrètement :  
- Pour chaque **maître** défini dans le scénario, la génération se fait **esclave par esclave**.  
- Pour un esclave donné, la génération produit **une série de flux distincts par protocole** (Modbus, ICMP, ARP, NTP, SNMP). Chaque flux possède son propre timer et ses propres intervalles d’envoi (déterminés aléatoirement selon la logique de référence ou forcés pour simuler une anomalie).  
- Les timers déterminent les instants d’émission des requêtes/réponses : plutôt que d’attribuer des timestamps fixes, on incrémente un timer local au flux pour obtenir des événements temporellement plausibles.  
- Une fois les flux protocolaires générés pour l’esclave, on **merge** ces séries en respectant l’ordre croissant des timestamps (les valeurs des timers) afin d’obtenir la chronologie réelle des paquets produits par cet esclave.  
- Enfin, on **fusionne** les séries de tous les esclaves (et des machines inconnues/attaquantes) du maître en respectant les timers globaux, ce qui produit la séquence finale triée temporellement. Cette fusion préserve la superposition d’échanges et permet de simuler des collisions temporelles ou des congestions.  

Points clés :
- Les **anomalies** s’insèrent en modifiant soit les timers (ex. rafale de requêtes → timers rapprochés), soit le contenu des paquets (ex. absence de réponse), soit l’apparition de machines nouvelles.  
- L’approche timer‑centrée rend la simulation simple à accélérer/décélérer (`ratio_duration`) et facilite la génération de scénarios avec documents temporels réalistes et reproductibles.  


> **Remarque :** la même logique de merge s’applique à l’échelle du système entier lorsque plusieurs maîtres sont présents — chaque maître produit sa séquence temporelle (en fusionnant ses esclaves/attaquants) et les séquences des différents maîtres sont ensuite fusionnées entre elles en respectant les timers globaux pour obtenir la trace complète du scénario multi‑maître

### 📤 Output — fichiers générés

Le module **générateur** produit des jeux de données au format CSV (séparateur `,`) mais enregistrés avec l’extension `.txt` (encodage UTF-8). Chaque fichier contient les colonnes décrites dans la section *Variables générées* (ex. `id`, `time`, `mac_src`, `ip_src`, `pck_length`, `protocol`, `label`, …).

Deux **familles** de bases sont générées pour chaque maître :

- **`*_safe`** : bases *safe* — aucune anomalie (comportement normal des esclaves).  
- **`*_anomalies`** : bases contenant des anomalies. Pour un maître donné, la base `anomalies` reprend la définition des esclaves de la base `safe` (conservation du même comportement normal) **et** y ajoute la génération d’événements anormaux selon des probabilités d’apparition configurables pour chaque type d’anomalie.

#### Nommage et nombre de fichiers
- L’utilisateur fournit un **préfixe de sortie** `X` (chemin sans extension).  
- Pour chaque maître `i` (indexé à partir de `1`) sont produits **deux** fichiers :
  - `X_safe_i.txt`
  - `X_anomalies_i.txt`

- En plus, le générateur produit **deux fichiers agrégés** (merge de tous les maîtres) :
  - `X_network_safe.txt` (fusion de tous les `*_safe_i.txt`)
  - `X_network_anomalies.txt` (fusion de tous les `*_anomalies_i.txt`)

Le nombre total de fichiers produits est donc : (nombre de master +1) * 2 


## 🔹 2. Module de Construction de symboles

### 🎯 Objectif
Ce module prend en **entrée** les données générées par le module de génération — ou tout autre jeu de données respectant le même format de colonnes.  
Son rôle est d’enrichir ces données en ajoutant une nouvelle colonne : **`symbole`**.

### 🧩 Principe
Chaque **symbole** représente l’**unicité d’un n-uplet** présent dans la base de données.  
Ainsi, **deux paquets identiques** (même combinaison de valeurs sur les colonnes clés) **auront le même symbole**.  
Ce mécanisme permet de convertir une séquence de paquets en une séquence symbolique exploitable par des modèles de prédiction ou de détection de motifs.

Les **symboles** produits appartiennent à \( \mathbb{N}^* \) (séquence de tokens/indices).  
La **relation d'ordre** entre symboles n'est **pas** exploitée : les symboles servent uniquement d'identifiants discrets d'un n‑uplet d'attributs, pas d'une grandeur ordinale.

Les symboles sont construits à partir des variables suivantes :
- `timestamp` (temps du paquet)
- `pck_length` (taille totale du paquet)
- `port_src` (port source)
- `port_dst` (port destination)
- `transport` (protocole de transport, ex. TCP/UDP/ICMP)
- `direction` (sens du paquet : ex. `master->slave` ou `slave->master`)


La variable `timestamp` correspond à **l’écart de temps entre l’arrivée de deux paquets successifs au sein d’une même conversation**.  
Pour chaque conversation (définie par `mac_src`, `mac_dst`, `transport`, `port_src`, `port_dst`), les paquets sont triés par timestamp, puis `timestamp` est calculé comme la différence de temps entre le paquet courant et le paquet précédent.  

Cette approche permet de capturer la **dynamique temporelle locale** de chaque conversation, tout en restant indépendante des autres conversations ou maîtres.  

#### Regroupement par conversation
La construction est effectuée **par conversation** : une conversation est définie par la combinaison
- `mac_src` + `mac_dst` (les deux adresses MAC)  
- le protocole de transport (`transport`)  
- `port_src` et `port_dst`  

Autrement dit, tous les paquets partageant ces valeurs appartiennent à la même conversation.  
Les opérations de discrétisation (par ex. calcul de la moyenne) sont **effectuées au niveau de chaque conversation**, afin de préserver les caractéristiques locales (débit, taille moyenne, etc.).

#### Construction du symbole
1. Pour chaque paquet, on extrait les variables listées ci‑dessus.  
2. Les variables numériques pertinentes sont **discrétisées** selon les règles ci‑dessous (les bornes sont calculées localement par conversation lorsque mentionné).  
3. Chaque vecteur discrétisé (p.ex. `[time_bin, length_bin, port_src_bin, port_dst_bin, transport_token, direction_token]`) est mappé vers un identifiant unique (symbole).  
4. Deux paquets ayant le même n‑uplet discretisé recevront le **même symbole**.

---

### 🔢 Tableau : règles de discrétisation

| Variable | Règle de discrétisation / Description |
|----------|----------------------------------------|
| `timestamp` | Discrétisation relative **par conversation** : `a` si `< moyenne_de_la_conversation`, `b` sinon. (La moyenne est calculée sur les `pck_length` de la conversation.) |
| `pck_length` | Discrétisation relative **par conversation** : `a` si `< moyenne_de_la_conversation`, `b` sinon. (La moyenne est calculée sur les `pck_length` de la conversation.) |
| `payload_size` | 3 classes : `a` si `= 0` (pas de charge utile), `b` si `> 0` **et** `< moyenne_de_la_conversation` (taille < moyenne), `c` sinon (>= moyenne). |
| `port_src` | Classes de ports (standard) : `a` si `< 1023` (ports système), `b` si `>= 1023` et `< 25000` (ports enregistrés), `c` si `>= 25000` et `< 49151` (ports dynamiques/privés basse plage), `d` sinon (`>= 49151`). |
| `port_dst` | **Même** règle que `port_src` (même discrétisation par catégories). |
| `transport` | Catégoriel : mappage direct en token (`TCP` → `tcp`, `UDP` → `udp`, `ICMP` → `icmp`, etc.). |
| `direction` | Catégoriel : tokeniser par sens (`master->slave`, `slave->master`, `unknown`). |
| `protocol` (haut niveau) | Conservé/catégorisé (ex. `Modbus`, `SNMP`, `NTP`, `ARP`) ; peut être inclus dans le n‑uplet si nécessaire. |
| `autres champs` | À discrétiser / catégoriser selon besoin (ex. flags TCP → petites catégories). |

### ▶️ Paramètres demandés au démarrage (mode symbole)

Au lancement du module de construction de symboles, le programme demande quelques paramètres essentiels :  
- `safe_input` : chemin (avec extension) vers la base *safe* (jeu de données sans anomalies) à transformer ;  
- `anomalie_input` : chemin (avec extension) vers la base *anomalies* associée à la base *safe* ;  
- `pathout` : chemin de sortie **sans extension** qui servira de préfixe pour les fichiers générés.  

### ▶️ Fichiers de sortie

Le module produit deux fichiers en sortie basés sur le préfixe `pathout` fourni :  
- `pathout_safe.txt` — version symbolisée de `safe_input`.  
- `pathout_anomalies.txt` — version symbolisée de `anomalie_input`. 


### 📝 TODO / Améliorations à venir (Module de construction de symboles)

- Améliorer les **discrétisations** en utilisant des méthodes statistiques plus avancées (ex. quantiles, clustering, bins dynamiques) pour mieux représenter la distribution réelle des variables numériques.  
- Proposer un **ensemble de choix de discrétisation** : classes plus ou moins grandes selon les besoins, permettant de faire évoluer le **nombre de symboles** générés et d’adapter la granularité de la représentation.


---
## 📄 Licence
Projet distribué sous licence **MIT** 

---