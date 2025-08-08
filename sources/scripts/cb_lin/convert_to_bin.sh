#!/usr/bin/env bash

# Функция для конвертации всех изображений в один .h-файл
convert_images_to_single_h() {
    local target_dir="$1"
    local output_file="${2:-images.h}"  # По умолчанию: images.h
    
    # Проверка существования каталога
    if [[ ! -d "$target_dir" ]]; then
        echo "Ошибка: каталог '$target_dir' не существует!" >&2
        return 1
    fi

    # Создаем или очищаем выходной файл
    echo "// Автоматически сгенерированный файл с изображениями" > "$output_file"
    echo "#pragma once" >> "$output_file"
    echo "" >> "$output_file"

    # Рекурсивно обрабатываем изображения (BMP/PNG/JPG)
    find "$target_dir" -type f \( -iname "*.bmp" -o -iname "*.png" -o -iname "*.jpg" \) | while read -r file; do
        # Генерируем имя массива (уникальное, с путем)
        local array_name=$(echo "$file" | sed -e "s|^$target_dir/||" -e 's/[^a-zA-Z0-9]/_/g')
        
        echo "// Изображение: $file" >> "$output_file"
        xxd -i "$file" | sed "s/unsigned char .*\[/unsigned char ${array_name}_img[/" >> "$output_file"
        echo "" >> "$output_file"
        
        echo "Добавлено: $file -> $array_name"
    done

    echo "Готово! Все изображения сохранены в $output_file"
}

dir=$PWD

cd ../../VS/Linia2/resources

convert_images_to_single_h "." "images.h"

cd $dir
