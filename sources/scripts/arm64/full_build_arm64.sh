#!/usr/bin/env bash
# Полная сборка для Orange Pi 5 Plus (ARM64)

set -e

echo "Сборка проекта Linia для Orange Pi 5 Plus (ARM64)"
echo "Архитектура: $(uname -m)"

# Проверяем архитектуру
if [ "$(uname -m)" != "aarch64" ]; then
    echo "Предупреждение: Скрипт предназначен для ARM64 (aarch64), текущая архитектура: $(uname -m)"
fi

# Очистка предыдущих сборок
echo "Очистка предыдущих сборок..."
rm -R -f ../../generated
rm -R -f ../../ThirdParty/wxWidgets/generated

# Сборка wxWidgets для ARM64
echo "Сборка wxWidgets для ARM64..."
cd ../../ThirdParty/wxWidgets

# Флаги для подавления предупреждений wxWidgets
WXWIDGETS_CXXFLAGS="-O2 -march=armv8-a"
WXWIDGETS_CXXFLAGS="$WXWIDGETS_CXXFLAGS -Wno-array-bounds"           # Подавляем array bounds warnings
WXWIDGETS_CXXFLAGS="$WXWIDGETS_CXXFLAGS -Wno-maybe-uninitialized"   # Подавляем uninitialized warnings  
WXWIDGETS_CXXFLAGS="$WXWIDGETS_CXXFLAGS -Wno-nonnull"               # Подавляем null pointer warnings
WXWIDGETS_CXXFLAGS="$WXWIDGETS_CXXFLAGS -Wno-unused-variable"       # Подавляем unused variable warnings
WXWIDGETS_CXXFLAGS="$WXWIDGETS_CXXFLAGS -Wno-unused-const-variable" # Подавляем unused const warnings
WXWIDGETS_CXXFLAGS="$WXWIDGETS_CXXFLAGS -Wno-array-parameter"       # Подавляем array parameter warnings

cmake CMakeLists.txt -Bgenerated \
    -DwxBUILD_SAMPLES=ALL \
    -DwxBUILD_SHARED=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="$WXWIDGETS_CXXFLAGS" \
    -DwxUSE_LIBMSPACK=OFF \
    -DwxUSE_LIBLZMA=ON

cd generated
cmake --build . -- -j$(nproc)
cd ../../../scripts/arm64

# Сборка основного проекта
echo "Сборка основного проекта..."

# Флаги для основного проекта (более строгие, но без проблемных предупреждений wxWidgets)
PROJECT_CXXFLAGS="-O2 -march=armv8-a"
PROJECT_CXXFLAGS="$PROJECT_CXXFLAGS -Wall -Wextra"                  # Включаем большинство предупреждений
PROJECT_CXXFLAGS="$PROJECT_CXXFLAGS -Wno-unused-parameter"          # Отключаем для совместимости с wxWidgets
PROJECT_CXXFLAGS="$PROJECT_CXXFLAGS -Wno-deprecated-declarations"   # Отключаем предупреждения об устаревших функциях

cmake ../../VS/CMakeLists.txt -B../../generated \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="$PROJECT_CXXFLAGS"

cd ../../generated

# Попробуем собрать проект
echo "Компиляция основного проекта..."
if cmake --build . -- -j$(nproc); then
    echo "✅ Сборка завершена успешно!"
    cd ../scripts/arm64
else
    echo "❌ Ошибка сборки основного проекта"
    echo ""
    echo "Возможные причины и решения:"
    echo "1. Отсутствие библиотеки GSpell - запустите: ./fix_packages_arm64.sh"
    echo "2. Проблемы с линковкой - запустите: ./fix_build_arm64.sh"
    echo ""
    echo "Для получения подробной информации об ошибке, посмотрите вывод выше"
    cd ../scripts/arm64
    exit 1
fi

echo "Сборка завершена!"
echo "Исполняемый файл: ../../generated/Linia2/Linia2"

# Копирование конфигурационного файла
if [ -f "../../../files/Linia.loc" ]; then
    echo "Копирование конфигурационного файла..."
    cp ../../../files/Linia.loc ../../generated/Linia2/
else
    echo "Предупреждение: Файл конфигурации files/Linia.loc не найден"
fi

echo "Для запуска выполните:"
echo "cd ../../generated/Linia2 && ./Linia2"
echo ""
echo "📋 Важно для GUI приложения:"
echo "   • Приложение требует графическое окружение (X11/Wayland)"
echo "   • Для SSH подключения используйте: ssh -X пользователь@orangepi"
echo "   • Для диагностики GUI: ./diagnose_gui.sh"
echo "   • Для автоматического запуска с настройкой дисплея: ./run_with_display.sh"
