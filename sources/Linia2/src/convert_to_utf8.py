#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import chardet

def detect_encoding(file_path):
    """Определение кодировки файла"""
    try:
        with open(file_path, 'rb') as f:
            raw_data = f.read()
            # Если файл пустой, возвращаем None
            if not raw_data:
                return None
            # Используем chardet для определения кодировки
            result = chardet.detect(raw_data)
            return result['encoding'].lower() if result['encoding'] else None
    except Exception as e:
        print(f"Ошибка при чтении {file_path}: {e}")
        return None

def convert_to_utf8(file_path, dry_run=False):
    """Конвертация файла в UTF-8"""
    try:
        # Определяем кодировку
        encoding = detect_encoding(file_path)
        
        # Если файл уже в UTF-8 или BOM, пропускаем
        if encoding in ['utf-8', 'utf-8-sig', 'ascii']:
            if dry_run:
                print(f"[SKIP] {file_path} (уже в {encoding})")
            return False
        
        # Если кодировка не определена или не Windows-1251
        if encoding != 'windows-1251':
            if dry_run:
                print(f"[SKIP] {file_path} (кодировка: {encoding}, не Windows-1251)")
            return False
        
        if dry_run:
            print(f"[CONVERT] {file_path} (Windows-1251 -> UTF-8)")
            return True
        
        # Читаем файл в Windows-1251
        with open(file_path, 'r', encoding='windows-1251') as f:
            content = f.read()
        
        # Записываем в UTF-8
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(content)
        
        print(f"[OK] {file_path}")
        return True
        
    except UnicodeDecodeError:
        print(f"[ERROR] Не удалось прочитать {file_path} в Windows-1251")
        return False
    except Exception as e:
        print(f"[ERROR] {file_path}: {e}")
        return False

def process_directory(directory, extensions=None, dry_run=False):
    """Обработка всех файлов в директории"""
    if extensions is None:
        extensions = ['.cpp', '.h', '.c', '.hpp', '.cxx', '.hxx', '.txt']
    
    converted = 0
    total = 0
    
    for root, dirs, files in os.walk(directory):
        # Пропускаем скрытые папки
        dirs[:] = [d for d in dirs if not d.startswith('.')]
        
        for file in files:
            # Проверяем расширение
            if not any(file.endswith(ext) for ext in extensions):
                continue
            
            file_path = os.path.join(root, file)
            total += 1
            
            if convert_to_utf8(file_path, dry_run):
                converted += 1
    
    return converted, total

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='Конвертация файлов из Windows-1251 в UTF-8')
    parser.add_argument('directory', nargs='?', default='.',
                        help='Директория для обработки (по умолчанию: текущая)')
    parser.add_argument('--extensions', '-e', nargs='+',
                        default=['.cpp', '.h', '.c', '.hpp', '.cxx', '.hxx', '.txt'],
                        help='Расширения файлов для обработки')
    parser.add_argument('--dry-run', '-n', action='store_true',
                        help='Только показать, какие файлы будут конвертированы')
    parser.add_argument('--no-backup', action='store_true',
                        help='Не создавать резервные копии')
    
    args = parser.parse_args()
    
    if not os.path.exists(args.directory):
        print(f"Ошибка: Директория {args.directory} не существует")
        sys.exit(1)
    
    print(f"{'ПРОВЕРКА' if args.dry_run else 'КОНВЕРТАЦИЯ'} файлов в {args.directory}")
    print(f"Расширения: {', '.join(args.extensions)}")
    print("-" * 60)
    
    converted, total = process_directory(args.directory, args.extensions, args.dry_run)
    
    print("-" * 60)
    if args.dry_run:
        print(f"Будет конвертировано: {converted} из {total} файлов")
    else:
        print(f"Конвертировано: {converted} из {total} файлов")

if __name__ == "__main__":
    main()
