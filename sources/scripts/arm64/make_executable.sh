#!/bin/bash

# Делаем все скрипты исполняемыми

echo "🔧 Установка прав на выполнение для скриптов Orange Pi..."

chmod +x install_soft_arm64.sh
chmod +x full_build_arm64.sh
chmod +x fix_packages_arm64.sh
chmod +x fix_build_arm64.sh
chmod +x run_with_display.sh
chmod +x diagnose_gui.sh

echo "✅ Права установлены для всех скриптов"

# Проверяем права
echo "📋 Текущие права доступа:"
ls -la *.sh
