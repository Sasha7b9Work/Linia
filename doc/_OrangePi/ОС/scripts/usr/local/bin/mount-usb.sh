#!/bin/bash

MOUNT_POINT="/mnt/usb1"
USER="mnipi"
LOG="/tmp/mount-usb.log"

echo "=== $(date) ===" >> $LOG

# Находим устройство
DEVICE=$(lsblk -o NAME,TYPE,MOUNTPOINT | grep -E "sd[a-z][0-9]" | grep -v "part" | head -1)

echo "Found device: $DEVICE" >> $LOG

if [ -z "$DEVICE" ]; then
    echo "No device found" >> $LOG
    exit 1
fi

DEVICE="/dev/$DEVICE"

# Создаём точку монтирования
mkdir -p "$MOUNT_POINT"

# Монтируем
echo "Mounting $DEVICE to $MOUNT_POINT" >> $LOG
mount -o rw,noatime "$DEVICE" "$MOUNT_POINT" 2>> $LOG

# Меняем владельца
chown -R "$USER":"$USER" "$MOUNT_POINT" 2>> $LOG

echo "Done" >> $LOG
