##       Краткое описание утилиты 
###   Назначение 
 Утилита предназначена для чтения файлов в том числе и свыше 4 гб. Файл разбивается на блоки, каждый блок читается в отдельном потоке и для него вычисляется хэш сумма MD5. После того как чтение завершится в выходной файл записываются хэши для каждого блога начиная с начала в hex формате. Если последний блок меньше указанного размера блока, то недостающаи данные дополняются нулями.
### Используемые библиотеки 
1. Boost.
2. MD5 файл из openssl библиотеки.
### Формат командной строки для утилиты 
 Необходимо передать не менее одного параметра командной строки. Формат следующий 
 --in [full_path_to_input_file] --out [full_path_to_output_file] --[size block_size]
 
 Размер блока по умолчанию 1000
 Имя выходного файла по умолчанию file_hash.txt
 Имя входного файла по умолчанию big_file.txt (должен находиться в папке, где находиться исполняемый файл)
 
### Операционная система под которой тестировалась
    - ОС Linus debian
### Как собрать (выполнять в директории где CmakeList) 
    - cmake .
    - make -j4
### Как скачать с гит хаба 
    git clone https://github.com/AksenovAr/bigfile_hashcalc.git
