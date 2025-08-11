#!/bin/bash

# Расширенная диагностика для Orange Pi 5 Plus (ARM64)

echo "=== ДИАГНОСТИКА СИСТЕМЫ СБОРКИ ==="
echo "Дата: $(date)"
echo "Архитектура: $(uname -m)"
echo "Система: $(cat /etc/os-release | grep PRETTY_NAME | cut -d'"' -f2 2>/dev/null || uname -a)"
echo

echo "=== ПРОВЕРКА CMAKE ==="
if command -v cmake >/dev/null 2>&1; then
    echo "CMake версия: $(cmake --version | head -n1)"
else
    echo "❌ CMake не установлен"
fi
echo

echo "=== ПРОВЕРКА КОМПИЛЯТОРА ==="
if command -v gcc >/dev/null 2>&1; then
    echo "GCC версия: $(gcc --version | head -n1)"
else
    echo "❌ GCC не установлен"
fi

if command -v g++ >/dev/null 2>&1; then
    echo "G++ версия: $(g++ --version | head -n1)"
else
    echo "❌ G++ не установлен"
fi
echo

echo "=== ПРОВЕРКА PKG-CONFIG ==="
if command -v pkg-config >/dev/null 2>&1; then
    echo "pkg-config версия: $(pkg-config --version)"
    echo
    
    echo "Проверка основных библиотек:"
    for lib in gtk+-3.0 gdk-3.0 x11 xkbcommon gspell-1 libpng; do
        if pkg-config --exists "$lib" 2>/dev/null; then
            version=$(pkg-config --modversion "$lib" 2>/dev/null || echo "неизвестно")
            echo "✅ $lib (версия: $version)"
        else
            echo "❌ $lib не найдена"
        fi
    done
else
    echo "❌ pkg-config не установлен"
fi
echo

echo "=== ПРОВЕРКА БИБЛИОТЕК ARM64 ==="
echo "Путь к ARM64 библиотекам: /usr/lib/aarch64-linux-gnu/"
if [ -d "/usr/lib/aarch64-linux-gnu/" ]; then
    echo "✅ Каталог ARM64 библиотек существует"
    
    echo "Проверка ключевых библиотек:"
    for lib in libX11.so libxkbcommon.so libpng16.so libgtk-3.so; do
        if [ -f "/usr/lib/aarch64-linux-gnu/$lib" ]; then
            echo "✅ $lib найдена"
        else
            echo "❌ $lib не найдена"
        fi
    done
else
    echo "❌ Каталог ARM64 библиотек не найден"
fi
echo

echo "=== ПРОВЕРКА WXWIDGETS ==="
wxwidgets_path="../../ThirdParty/wxWidgets/generated"
if [ -d "$wxwidgets_path" ]; then
    echo "✅ Каталог wxWidgets найден: $wxwidgets_path"
    
    if [ -f "$wxwidgets_path/lib/libwx_gtk3u_core-3.2.a" ]; then
        echo "✅ wxWidgets библиотека собрана"
        lib_size=$(stat -f%z "$wxwidgets_path/lib/libwx_gtk3u_core-3.2.a" 2>/dev/null || stat -c%s "$wxwidgets_path/lib/libwx_gtk3u_core-3.2.a" 2>/dev/null || echo "неизвестно")
        echo "   Размер библиотеки: $lib_size байт"
    else
        echo "❌ wxWidgets библиотека не собрана"
    fi
else
    echo "❌ Каталог wxWidgets не найден"
fi
echo
        echo "❌ X11 forwarding не активен. Используйте: ssh -X или ssh -Y"
    fi
fi

# Проверяем установленные GUI библиотеки
echo "📚 Проверка GUI библиотек:"
ldconfig -p | grep -E "(gtk|qt|wx)" | head -10

# Проверяем исполняемый файл
EXEC_PATH="../../generated/Linia2/Linia2"
if [ -f "$EXEC_PATH" ]; then
    echo "✅ Исполняемый файл найден: $EXEC_PATH"
    echo "🔍 Зависимости исполняемого файла (графические):"
    ldd "$EXEC_PATH" | grep -E "(gtk|gdk|wx|X11)" | head -10
else
    echo "❌ Исполняемый файл не найден: $EXEC_PATH"
fi

echo ""
echo "=== Рекомендации ==="

if [ -z "$DISPLAY" ]; then
    echo "1. 🖥️  Для локального запуска: войдите в графическую сессию KDE"
    echo "2. 🌐 Для удаленного доступа через SSH:"
    echo "   ssh -X пользователь@orangepi_ip"
    echo "   ssh -Y пользователь@orangepi_ip  # для trusted connections"
    echo "3. 🖼️  Для VNC: установите и настройте VNC сервер"
    echo "4. 🖼️  Для виртуального дисплея:"
    echo "   sudo apt install xvfb"
    echo "   export DISPLAY=:99"
    echo "   Xvfb :99 -screen 0 1024x768x16 &"
fi

if [ "$(whoami)" = "root" ]; then
    echo "⚠️  Вы запускаете от root. Для GUI приложений лучше использовать обычного пользователя"
    echo "   Если нужен root доступ к дисплею: xhost +local:root"
fi
