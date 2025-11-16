.data
# Массивы указателей
units:      .word unit0, unit1, unit2, unit3, unit4, unit5, unit6, unit7, unit8, unit9
tens:       .word ten0, ten1, ten2, ten3, ten4, ten5, ten6, ten7, ten8, ten9
hundreds:   .word hundred0, hundred1, hundred2

# Единицы (0-9)
unit0:      .asciz ""           # 0
unit1:      .asciz "один"       # 1
unit2:      .asciz "два"        # 2
unit3:      .asciz "три"        # 3
unit4:      .asciz "четыре"     # 4
unit5:      .asciz "пять"       # 5
unit6:      .asciz "шесть"      # 6
unit7:      .asciz "семь"       # 7
unit8:      .asciz "восемь"     # 8
unit9:      .asciz "девять"     # 9

# Десятки (0-90)
ten0:       .asciz ""           # 0
ten1:       .asciz "десять"     # 10
ten2:       .asciz "двадцать"   # 20
ten3:       .asciz "тридцать"   # 30
ten4:       .asciz "сорок"      # 40
ten5:       .asciz "пятьдесят"  # 50
ten6:       .asciz "шестьдесят" # 60
ten7:       .asciz "семьдесят"  # 70
ten8:       .asciz "восемьдесят"# 80
ten9:       .asciz "девяносто"  # 90

# Сотни (0-200)
hundred0:   .asciz ""           # 0
hundred1:   .asciz "сто"        # 100
hundred2:   .asciz "двести"     # 200

# Числа 10-19
special10:  .asciz "десять"     # 10
special11:  .asciz "одиннадцать"# 11
special12:  .asciz "двенадцать" # 12
special13:  .asciz "тринадцать" # 13
special14:  .asciz "четырнадцать"# 14
special15:  .asciz "пятнадцать" # 15
special16:  .asciz "шестнадцать"# 16
special17:  .asciz "семнадцать" # 17
special18:  .asciz "восемнадцать"# 18
special19:  .asciz "девятнадцать"# 19

# Массив особых случаев
special:    .word special10, special11, special12, special13, special14, special15, special16, special17, special18, special19

# Константы
max_value:  .word 255           # Максимальное число
space:      .asciz " "          # Пробел
newline:    .asciz "\n"         # Новая строка
prompt:     .asciz "Введите число от 0 до 255: " # Приглашение
zero_text:  .asciz "ноль"       # Текст для нуля
error_msg:  .asciz "Ошибка: число должно быть от 0 до 255!\n" # Ошибка

.text

# InputModule: ввод и проверка числа
.macro InputModule
    la a0, prompt              # Загрузить адрес приглашения
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести приглашение

    li a7, 5                   # Код ввода числа
    ecall                      # Ввести число
    mv s0, a0                  # Сохранить число в s0

    blt s0, x0, input_error    # Если < 0 - ошибка
    lw t0, max_value           # Загрузить максимум (255)
    bgt s0, t0, input_error    # Если > 255 - ошибка
.end_macro

# HundredsModule: вывод сотен
.macro HundredsModule
    beqz s1, .HundredsDone     # Пропустить если сотен нет
    la t0, hundreds            # Адрес массива сотен
    slli t1, s1, 2             # Индекс ? 4 (размер слова)
    add t0, t0, t1             # Адрес элемента массива
    lw a0, 0(t0)               # Загрузить строку сотен
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести сотни

    la a0, space               # Загрузить пробел
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести пробел
.HundredsDone:
.end_macro

# TensModule: вывод десятков
.macro TensModule
    li t0, 1                   # Число 1 для сравнения
    beq s3, t0, handle_special_case # Если 10-19 - особый случай
    beqz s3, .TensDone         # Пропустить если десятков нет

    la t0, tens                # Адрес массива десятков
    slli t1, s3, 2             # Индекс ? 4 (размер слова)
    add t0, t0, t1             # Адрес элемента массива
    lw a0, 0(t0)               # Загрузить строку десятков
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести десятки

    la a0, space               # Загрузить пробел
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести пробел
.TensDone:
.end_macro

# UnitsModule: вывод единиц
.macro UnitsModule
    beqz s4, .UnitsDone        # Пропустить если единиц нет
    la t0, units               # Адрес массива единиц
    slli t1, s4, 2             # Индекс ? 4 (размер слова)
    add t0, t0, t1             # Адрес элемента массива
    lw a0, 0(t0)               # Загрузить строку единиц
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести единицы
.UnitsDone:
.end_macro

# SpecialModule: вывод чисел 10-19
.macro SpecialModule
    la t0, special             # Адрес массива особых случаев
    slli t1, s4, 2             # Индекс ? 4 (размер слова)
    add t0, t0, t1             # Адрес элемента массива
    lw a0, 0(t0)               # Загрузить строку числа
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести число 10-19
.end_macro

# EndModule: завершение программы
.macro EndModule
    la a0, newline             # Загрузить перевод строки
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести перевод строки

    li a7, 10                  # Код завершения программы
    ecall                      # Завершить программу
.end_macro

# Основная программа
main:
    InputModule                # Ввести и проверить число

    beqz s0, print_zero        # Если 0 - вывести "ноль"

    li t0, 100                 # Загрузить 100
    div s1, s0, t0             # s1 = сотни (число / 100)
    rem s2, s0, t0             # s2 = остаток (число % 100)

    li t0, 10                  # Загрузить 10
    div s3, s2, t0             # s3 = десятки (остаток / 10)
    rem s4, s2, t0             # s4 = единицы (остаток % 10)

    HundredsModule             # Вывести сотни
    TensModule                 # Вывести десятки
    UnitsModule                # Вывести единицы
    j end_program              # Перейти к завершению

# Обработка чисел 10-19
handle_special_case:
    SpecialModule              # Вывести число 10-19
    j end_program              # Перейти к завершению

# Вывод нуля
print_zero:
    la a0, zero_text           # Загрузить "ноль"
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести "ноль"
    j end_program              # Перейти к завершению

# Обработка ошибки ввода
input_error:
    la a0, error_msg           # Загрузить сообщение об ошибке
    li a7, 4                   # Код вывода строки
    ecall                      # Вывести ошибку
    j main                     # Повторить ввод

# Завершение программы
end_program:
    EndModule                  # Завершить программу