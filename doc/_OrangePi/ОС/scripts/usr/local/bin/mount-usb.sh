#!/bin/bash

# Переменные
MOUNT_POINT="/mnt/usb1"
USER="mnipi"
DEVICE="$1"  # Принимаем устройство как параметр

# Если устройство не передано, выходим
if [ -z "$DEVICE" ]; then
    exit 1
fi

# Создаём точку монтирования, если её нет
mkdir -p "$MOUNT_POINT"

# Монтируем устройство
# Для ext4 используем опции uid и gid через командную строку mount
mount -o rw,noatime "$DEVICE" "$MOUNT_POINT"

# После монтирования меняем владельца папки
chown -R "$USER":"$USER" "$MOUNT_POINT"
