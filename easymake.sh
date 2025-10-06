#!/bin/bash

# ==============================
# 💻 SimplyIOTModbusGen - Build Script
# ==============================

# Variables
CXX=g++
CXXFLAGS="-Wall -Wextra -std=c++17 -Iinclude"
TARGET="simg"

# Répertoires
SRC_DIR="src"
BUILD_DIR="build"

# Création du dossier build s’il n’existe pas
mkdir -p $BUILD_DIR

# Compilation étape par étape
echo "Compilation des .o ..."

$CXX -c $SRC_DIR/Affiche_Debog.cpp -o $BUILD_DIR/Affiche_Debog.o $CXXFLAGS
echo "Affiche_Debog.o done"

$CXX -c $SRC_DIR/Get_And_Transforme_Data.cpp -o $BUILD_DIR/Get_And_Transforme_Data.o $CXXFLAGS
echo "Get_And_Transforme_Data.o done"

$CXX -c $SRC_DIR/Traffic_Creator.cpp -o $BUILD_DIR/Traffic_Creator.o $CXXFLAGS
echo "Traffic_Creator.o done"

$CXX -c $SRC_DIR/Anomalies.cpp -o $BUILD_DIR/Anomalies.o $CXXFLAGS
echo "Anomalies.o done"

$CXX -c $SRC_DIR/Generateur_Donnees.cpp -o $BUILD_DIR/Generateur_Donnees.o $CXXFLAGS
echo "Generateur_Donnees.o done"

$CXX -c $SRC_DIR/main.cpp -o $BUILD_DIR/main.o $CXXFLAGS
echo "main.o done"

# Linking
echo "Édition de liens ..."
$CXX $BUILD_DIR/Affiche_Debog.o \
     $BUILD_DIR/Get_And_Transforme_Data.o \
     $BUILD_DIR/Traffic_Creator.o \
     $BUILD_DIR/Anomalies.o \
     $BUILD_DIR/Generateur_Donnees.o \
     $BUILD_DIR/main.o \
     -o $BUILD_DIR/$TARGET

echo "✅ Build terminé : $BUILD_DIR/$TARGET"
