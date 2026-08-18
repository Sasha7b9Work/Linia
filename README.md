# Измеритель параметров полупроводниковых приборов

** Tools

Windows:
	Visual Studio 2026

Linux:
	Ubuntu 24.04.2 LTS
	gcc version 13.3.0
    
*** После устанвоки ***

sources/Linia2/src/convert_to_utf8.py - преобразование исходных файлов в utf-8. В Visual Studio по умолчанию кодировка Windows-1251,
                                        и строки могут неправильно отображаться в Linux

*** Сборка под Linux.

1. Установить Linux.

2. Зайти под root :
    sudo su

3. Обновиться :
    apt-get update
	apt-get upgrade -y

4. Установить git и файловый менеджер mc:
    apt install git
	apt install mc

5. Клонировать репозиторий :
    git clone github.com/Sasha7b9Work/Linia

6. Установить инструменты сборки:
   cd Linia/sources/scripts/cb_lin
   ./install_soft.sh

7. Установить зависимости для wxWidgets :
   cd ../../ThirdParty/linux
   ./install_software.sh

   8. Собрать wxWidgets :
  ./full_build.sh

9. Собрать проект :
    cd ../../scripts/cb_lin
	./full_build.sh

10. Скопировать файл files/CIC.loc в каталог sources/genereated/Linia2

11. Запустить файл sources/generated/Linia2/Linia2 :
    ./Linia2


*** vcpkg ***

** Установка под Linux **
cd /путь/к/проекту
vcpkg install --triplet x64-linux


*** Содержимое каталогов ***
sources/Device - прошивка st32 платы контроллера
sources/Linia  - старая версия ПО Orange Pi, для макетной платы контроллера
sources/Linia2 - актуальная версия ПО Orange Pi для опытных образцов
