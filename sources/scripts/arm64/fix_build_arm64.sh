#!/usr/bin/env bash
# Скрипт исправления проблем после сборки для Orange Pi 5 Plus (ARM64)
# Исправляет проблемы с линковкой GSpell и пересобирает проект

set -e

echo "Исправление проблем после сборки wxWidgets..."
echo "Архитектура: $(uname -m)"

# Установим недостающие библиотеки
echo "Установка недостающих библиотек..."
sudo apt-get update
sudo apt-get install -y libgspell-1-dev

# Очистим предыдущую сборку основного проекта
echo "Очистка предыдущей сборки..."
cd /Linia/sources
if [ -d "generated" ]; then
    rm -rf generated/*
    echo "Каталог generated очищен"
fi

# Пересборка основного проекта
echo "Пересборка основного проекта Linia..."
cd /Linia/sources/VS
mkdir -p ../generated
cd ../generated

# Запуск CMake с правильными флагами для ARM64
echo "Настройка CMake для ARM64..."
cmake ../VS -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-march=armv8-a -O2" \
    -DCMAKE_C_FLAGS="-march=armv8-a -O2"

# Сборка проекта
echo "Сборка проекта..."
make -j$(nproc)

if [ -f "Linia2/Linia2" ]; then
    echo "✓ Сборка завершена успешно!"
    echo "Исполняемый файл: $(pwd)/Linia2/Linia2"
    ls -la Linia2/Linia2
else
    echo "❌ Ошибка сборки - исполняемый файл не создан"
    exit 1
fi

echo ""
echo "Для запуска приложения:"
echo "cd /Linia/sources/generated/Linia2"
echo "./Linia2"
