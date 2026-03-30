.section .text
.global functional

functional:
    # --- ПРОЛОГ ---
    # В этой версии мы не трогаем s-регистры, 
    # поэтому нам не нужно сохранять их в стек.
    # Достаточно сохранить только адрес возврата ra.
    addi sp, sp, -16
    sw   ra, 0(sp)

    # Инициализация (используем t4 для res, t5 для i)
    li   t4, 0           # res = 0
    li   t5, 0           # i = 0

DAG1:
    # Условие цикла: i < n (a1 - это n)
    slt  t0, t5, a1      
    beq  t0, zero, DAG5  # Если i >= n, выходим
    
    # Проверка на четность: i % 2
    andi t0, t5, 1      
    beq  t0, zero, DAG3  # Если i % 2 == 0, идем в DAG3

DAG4: # Нечетные: res -= (a[i] + 3)
    slli t0, t5, 2       # offset = i * 4
    add  t0, a0, t0      # addr = a + offset
    lw   t1, 0(t0)       # t1 = a[i]
    addi t1, t1, 3       # t1 = val + 3
    sub  t4, t4, t1      # res -= t1
    j    DAG_INC

DAG3: # Четные: res += a[i] * 2
    slli t0, t5, 2       # offset = i * 4
    add  t0, a0, t0      # addr = a + offset
    lw   t1, 0(t0)       # t1 = a[i]
    slli t1, t1, 1       # t1 = val * 2
    add  t4, t4, t1      # res += t1

DAG_INC:
    addi t5, t5, 1       # i++
    j    DAG1

DAG5:
    # --- ЭПИЛОГ ---
    mv   a0, t4          # Результат в a0 (согласно ABI)
    lw   ra, 0(sp)       # Восстанавливаем адрес возврата
    addi sp, sp, 16      # Чистим стек
    ret
    