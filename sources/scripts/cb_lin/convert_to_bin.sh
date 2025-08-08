#!/usr/bin/env bash

# Функция для конвертации файлов в C-массивы
convert_files_to_c_arrays() {
    local target_dir="$1"
    
    # Проверка существования каталога
    if [[ ! -d "$target_dir" ]]; then
        echo "Ошибка: каталог '$target_dir' не существует!" >&2
        return 1
    fi

    # Рекурсивная обработка файлов
    find "$target_dir" -type f | while read -r file; do
        # Пропускаем .h-файлы и бинарники
        if [[ "$file" == *.h || "$file" == *.rc ]]; then
            continue
        fi

        # Генерируем имя массива (заменяем /, . и - на _)
        local array_name=$(basename "$file" | sed 's/[.-]/_/g')
        local output_file="${file}.h"

        echo "Конвертируем: $file -> $output_file"
        xxd -i "$file" > "$output_file"
    done
}

dir=$PWD

cd ../../VS/Linia2/resources

# Пример вызова функции для разных каталогов
convert_files_to_c_arrays "."
convert_files_to_c_arrays "buttons"
convert_files_to_c_arrays "grid"
convert_files_to_c_arrays "icons"
convert_files_to_c_arrays "pics"
convert_files_to_c_arrays "sch"
convert_files_to_c_arrays "sch/lines"

cd $dir
