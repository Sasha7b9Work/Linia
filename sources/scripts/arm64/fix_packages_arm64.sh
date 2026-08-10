#!/usr/bin/env bash
# Скрипт исправления проблем с пакетами для Orange Pi 5 Plus (ARM64)
# Ubuntu 24.04 с KDE Neon Plasma

set -e

echo "Исправление проблем с пакетами для ARM64..."
echo "Архитектура: $(uname -m)"

# Проверяем и устанавливаем WebKit2GTK
echo "Проверка и установка WebKit2GTK..."
if apt-cache show libwebkit2gtk-4.1-dev >/dev/null 2>&1; then
    echo "Найден libwebkit2gtk-4.1-dev (Ubuntu 24.04+)"
    sudo apt-get install libwebkit2gtk-4.1-dev -y
    echo "✓ libwebkit2gtk-4.1-dev установлен"
elif apt-cache show libwebkit2gtk-4.0-dev >/dev/null 2>&1; then
    echo "Найден libwebkit2gtk-4.0-dev (старые версии)"
    sudo apt-get install libwebkit2gtk-4.0-dev -y
    echo "✓ libwebkit2gtk-4.0-dev установлен"
else
    echo "❌ WebKit2GTK не найден в репозиториях"
    echo "Доступные варианты WebKit:"
    apt-cache search libwebkit2gtk | grep -i dev || echo "Нет доступных пакетов WebKit"
fi

# Проверяем другие критически важные пакеты
echo ""
echo "Проверка других критически важных пакетов..."

CRITICAL_PACKAGES=(
    "libgtk-3-dev"
    "libssl-dev"
    "libx11-dev"
    "cmake"
    "build-essential"
    "libgspell-1-dev"
)

for package in "${CRITICAL_PACKAGES[@]}"; do
    if dpkg -l | grep -q "^ii  $package "; then
        echo "✓ $package уже установлен"
    else
        echo "⚠ $package не установлен, попытка установки..."
        sudo apt-get install "$package" -y && echo "✓ $package установлен" || echo "❌ Ошибка установки $package"
    fi
done

# Очистка неиспользуемых пакетов
echo ""
echo "Очистка неиспользуемых пакетов..."
sudo apt autoremove -y

echo ""
echo "Проверка завершена для архитектуры: $(uname -m)"
echo "Если есть проблемы с WebKit, можно попробовать установить альтернативные пакеты:"
echo "  sudo apt install webkit2gtk-4.0 webkit2gtk-4.1"
