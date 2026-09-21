#!/bin/bash
# Сборка SPbPoly под Linux
set -e
if ! pkg-config --exists gtkmm-3.0; then
    echo "Не найдена gtkmm-3.0. Установите: sudo apt install libgtkmm-3.0-dev"
    exit 1
fi
g++ -std=c++17 -O2 $(pkg-config --cflags gtkmm-3.0) "Upgrade Original.cpp" $(pkg-config --libs gtkmm-3.0) -o SPbPoly
echo "Готово: ./SPbPoly"
