# Селезнев Григорий Ильич БПИ217
## ИДЗ-1 
## Условие варианта 28:
Разработать программу, которая в заданной ASCII-строке определяет корректность вложенности круглых скобок «(» и «)».
Необходимо учесть, что вложенные скобки могут образовывать в
тексте различные группы. Например: ...(...)...(...)....

### Схема на 4
1. Cчитывает данные из файла и передае их в первый канал
2. (Ребенок первого) Cчитывает из первого канала обрабытвает и записывает во второй канал
3. (Ребенок второго) Cчитывает из второго канала записывает в файл

> Запускается программа './prog.out input.txt output.txt'


### Схема на 5
1. Считывате и передает в первых канал ("first_pipe.fifo") данные из файла
2. (Ребенок первого) считывает из первого канала ("first_pipe.fifo") данные обрабатывает и передает во второй канал ("second_pipe.fifo")
3. (Ребенок второго) Считывает из второго канала ("second_pipe.fifo") данные и записывает в файл

> Запускается программа './prog.out input.txt output.txt'

### Схема на 6
1. Считывает и передает в первых канал данные из файла
2. (Ребенок первого) считывает из первого канала данные и обрабатывает и передает во второй канал

1. Считывает из первого канала данные и записываетв файл

### Схема на 7

### Схема на 8

### Схема на 9
