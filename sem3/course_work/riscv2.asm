.data
task_count: .word 3
quantum: .word 8

queue: .word -1, -1, -1
queue_size: .word 0

current_task: .word -1
time_remaining: .word 0

newline: .string "\n"
task_msg: .string "Task "
executing_msg: .string " executing (quantum: "
dot: .string ")\n"

.text
.globl main
main:
    la t0, queue # t0 - указатель на очередь
    li t1, 0 # Первая задача с ID 0
    sw t1, 0(t0) # Кидаем в начало очереди
    li t1, 1 # Вторая задача
    sw t1, 4(t0) # Следующая ячейка
    li t1, 2 # Третья задача
    sw t1, 8(t0) # В конец
    
    la t0, queue_size # Смотрим на размер очереди
    li t1, 3 # У нас три задачи
    sw t1, 0(t0) # Запоминаем размер
  
    # Погнали планировщик
    j scheduler 
    
scheduler:
    jal find_next_task # Ищем, какую задачу выполнять next
    li t0, 1 # Проверяем, нашли ли что-то
    bne a0, t0, no_tasks # Если нет задач - всё, конец
  
    # Запускаем то, что нашли
    mv a0, a1 # Перекидываем ID задачи
    jal run_task # Выполняем её
  
    j scheduler # И по новой
    
no_tasks:
    li a7, 10 # Выходим из программы
    ecall 
    
# Функция поиска следующей задачи
# Возвращает: a0=1 если ок, a1=ID задачи
find_next_task:
    addi sp, sp, -4 # Место в стеке
    sw ra, 0(sp) # Сохраняем куда возвращаться
  
    jal get_from_queue # Тянем задачу из очереди
    li t0, -1 # Проверка на пустоту
    bne a1, t0, found_task # Если не -1, значит нашли
  
    # Не нашли ничего
    li a0, 0 # Ставим флаг "не найдено"
    li a1, -1 # ID = -1
    j find_done # Выходим
    
found_task:
    li a0, 1 # Флаг "найдено"
    # В a1 уже лежит ID задачи
    
find_done:
    lw ra, 0(sp) # Восстанавливаем возврат
    addi sp, sp, 4 # Чистим стек
    ret 
    
# Запускаем задачу
# a0 = ID задачи
run_task:
    addi sp, sp, -12 # Место для трёх регистров
    sw ra, 0(sp) # Куда возвращаться
    sw s0, 4(sp) # Сохраняем s0
    sw s1, 8(sp) # И s1
  
    mv s0, a0 # Запоминаем ID задачи в s0
  
    # Запоминаем текущую задачу
    la t0, current_task 
    sw s0, 0(t0) # Пишем что сейчас выполняем
  
    # Ставим квант времени
    la t0, quantum 
    lw t1, 0(t0) # Берем значение кванта
    la t0, time_remaining 
    sw t1, 0(t0) # Записываем как оставшееся время
  
    # Выводим инфу о задаче
    la a0, task_msg # "Task "
    li a7, 4 
    ecall 
  
    mv a0, s0 # Выводим ID задачи
    li a7, 1 
    ecall 
  
    la a0, executing_msg # " executing (quantum: "
    li a7, 4 
    ecall 
  
    la t0, time_remaining # Берем оставшееся время
    lw a0, 0(t0) 
    li a7, 1 
    ecall 
  
    la a0, dot # ")\n"
    li a7, 4 
    ecall 
  
    # Выполняем работу задачи
    jal do_task_work # Делаем что-то полезное
  
    # После выполнения кидаем задачу обратно в очередь
    mv a0, s0 # ID задачи
    jal add_to_queue # Добавляем в конец
  
    lw ra, 0(sp) # Восстанавливаем всё
    lw s0, 4(sp) 
    lw s1, 8(sp) 
    addi sp, sp, 12 
    ret 
    
# Функция работы задачи (простая заглушка)
do_task_work:
    addi sp, sp, -8 # Место для двух регистров
    sw ra, 0(sp) 
    sw s0, 4(sp) 
  
    la s0, time_remaining # Указатель на оставшееся время
    lw t0, 0(s0) # Текущее значение

work_loop:
    # Простая задержка (типа работаем)
    li t1, 0 # Счетчик
    li t2, 300000 # До скольки считать
    
delay_loop:
    addi t1, t1, 1 # +1 к счетчику
    blt t1, t2, delay_loop # Пока не досчитали
  
    # Уменьшаем оставшееся время
    lw t0, 0(s0) # Перезагружаем время
    addi t0, t0, -1 # Минус одна единица
    sw t0, 0(s0) # Сохраняем
  
    # Проверяем, не закончилось ли время
    bgtz t0, work_loop # Если еще есть время - продолжаем
  
    lw ra, 0(sp) # Восстанавливаем
    lw s0, 4(sp) 
    addi sp, sp, 8 
    ret 
    
# Добавляем задачу в конец очереди
# a0 = ID задачи
add_to_queue:
    addi sp, sp, -12 # Место для трёх регистров
    sw ra, 0(sp) 
    sw s0, 4(sp) 
    sw s1, 8(sp) 
  
    mv s0, a0 # Запоминаем ID задачи
  
    la s1, queue # Указатель на очередь
    la t0, queue_size # Указатель на размер
  
    # Добавляем в конец
    lw t1, 0(t0) # Текущий размер
    slli t2, t1, 2 # Умножаем на 4 (смещение)
    add t2, s1, t2 # Адрес куда пишем
    sw s0, 0(t2) # Записываем задачу
  
    # Увеличиваем размер
    addi t1, t1, 1 # +1 к размеру
    sw t1, 0(t0) # Сохраняем новый размер
  
    lw ra, 0(sp) # Восстанавливаем
    lw s0, 4(sp) 
    lw s1, 8(sp) 
    addi sp, sp, 12 
    ret 
    
# Берем задачу из начала очереди
# Возвращает: a1 = ID задачи (-1 если пусто)
get_from_queue:
    addi sp, sp, -12 # Место для трёх регистров
    sw ra, 0(sp) 
    sw s0, 4(sp) 
    sw s1, 8(sp) 
  
    la s0, queue # Указатель на очередь
    la s1, queue_size # Указатель на размер
  
    lw t0, 0(s1) # Смотрим размер
    beqz t0, queue_empty # Если ноль - очередь пуста
  
    # Берем первую задачу
    lw a1, 0(s0) # Первый элемент в a1
  
    # Сдвигаем всю очередь влево
    li t1, 1 # Начинаем со второго элемента
    
shift_loop:
    bge t1, t0, shift_done # Если дошли до конца
    slli t2, t1, 2 # Смещение для текущего
    add t2, s0, t2 # Адрес текущего
    lw t3, 0(t2) # Значение текущего
    addi t4, t1, -1 # Индекс предыдущего
    slli t4, t4, 2 # Смещение предыдущего
    add t4, s0, t4 # Адрес предыдущего
    sw t3, 0(t4) # Сдвигаем влево
    addi t1, t1, 1 # Следующий элемент
    j shift_loop # Продолжаем
  
shift_done:
    # Обнуляем последний элемент
    addi t0, t0, -1 # Новый размер (на 1 меньше)
    slli t1, t0, 2 # Смещение последнего
    add t1, s0, t1 # Адрес последнего
    li t2, -1 # Значение -1
    sw t2, 0(t1) # Обнуляем последний
  
    # Уменьшаем размер
    lw t0, 0(s1) # Текущий размер
    addi t0, t0, -1 # -1
    sw t0, 0(s1) # Сохраняем
    j get_done 
    
queue_empty:
    li a1, -1 # Возвращаем -1 (ничего нет)
    
get_done:
    lw ra, 0(sp) # Восстанавливаем
    lw s0, 4(sp) 
    lw s1, 8(sp) 
    addi sp, sp, 12 
    ret 
