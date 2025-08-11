#!/usr/bin/env bash
# Скрипт установки для Orange Pi 5 Plus (ARM64/aarch64)
# Ubuntu 24.04 с KDE Neon Plasma

set -e    # Stop script when an error occurs

echo "Установка зависимостей для Orange Pi 5 Plus (ARM64)..."
echo "Архитектура: $(uname -m)"

# Обновление системы
sudo apt-get update
sudo apt-get -y upgrade

# Основные инструменты разработки
sudo apt-get -y install nginx htop openssh-server openssh-client iptables-persistent mc links
sudo apt-get -y install cmake gcc g++ make python3 python3-pip python3-venv build-essential gdb

# Библиотеки разработки
sudo apt-get -y install libssl-dev libx11-dev libxrandr-dev mesa-common-dev libgl1-mesa-dev libasound2-dev

# GTK3 и зависимости wxWidgets для ARM64
sudo apt-get install libgtk-3-dev -y
sudo apt-get install curl cmake -y
sudo apt install libcurl4-openssl-dev -y
sudo apt-get install libsecret-1-dev -y
# Добавляем GSpell для проверки орфографии
sudo apt-get install libgspell-1-dev -y

# Установка WebKit с проверкой версии (Ubuntu 24.04 использует 4.1)
echo "Попытка установки WebKit2GTK..."
if apt-cache show libwebkit2gtk-4.1-dev >/dev/null 2>&1; then
    echo "Устанавливаем libwebkit2gtk-4.1-dev (Ubuntu 24.04+)"
    sudo apt-get install libwebkit2gtk-4.1-dev -y
elif apt-cache show libwebkit2gtk-4.0-dev >/dev/null 2>&1; then
    echo "Устанавливаем libwebkit2gtk-4.0-dev (старые версии)"
    sudo apt-get install libwebkit2gtk-4.0-dev -y
else
    echo "Предупреждение: Пакет WebKit2GTK не найден. Продолжаем без него..."
fi

# GStreamer для мультимедиа
sudo apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio -y

# SDL2 для работы с аудио/видео
sudo apt-get install libsdl2-2.0-0 -y
sudo apt-get install libsdl2-dev -y

# Системные уведомления
sudo apt-get install libnotify-dev -y

# OpenGL
sudo apt install libglu1-mesa-dev freeglut3-dev -y

# Инструменты для создания deb-пакетов
sudo apt-get install -y dh-make devscripts dpkg debconf debhelper lintian

# Дополнительные ARM64-специфичные библиотеки
echo "Проверка ARM64 библиотек..."
ls -la /usr/lib/aarch64-linux-gnu/libX11* || echo "Предупреждение: libX11 не найдена в стандартном ARM64 пути"
ls -la /usr/lib/aarch64-linux-gnu/libxkbcommon* || echo "Предупреждение: libxkbcommon не найдена в стандартном ARM64 пути"
ls -la /usr/lib/aarch64-linux-gnu/libpng* || echo "Предупреждение: libpng не найдена в стандартном ARM64 пути"

echo "Установка завершена для архитектуры: $(uname -m)"
echo "Теперь можно запустить сборку wxWidgets и основного проекта"
