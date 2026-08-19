#!/bin/bash

# Переменные
MOUNT_POINT="/mnt/usb1"
USER="mnipi"

# Создаём точку монтирования, если её нет
mkdir -p "$MOUNT_POINT"

# Монтируем устройство. Для файловых систем, отличных от ext4, могут понадобиться другие опции.
# Для vfat и exFAT используем uid и gid
mount -o uid=$(id -u $USER),gid=$(id -g $USER),umask=0000 /dev/%k "$MOUNT_POINT"
