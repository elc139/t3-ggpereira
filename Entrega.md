# T3 - Scheduling com OpenMP
## Disciplina de Programação Paralela
## Gabriel Gomes Pereira - Sistemas de Informação 
### [Programa OpenMPDemoABC.cpp](https://github.com/elc139/t3-ggpereira/blob/master/OpenMPDemoABC.cpp)

### Casos de teste:
O programa implementado é uma modificação do exemplo apresentado usando Threads, nesse caso foram feitas adaptações para o programa
utilizar o OpenMP. São inseridos caracteres em um array compartilhado de 120 posições utilizando 3 threads(t0=A, t1=B, t2=C):

  	1. Schedule Static com especificação do chunk
    É especificado um chunk-size de valor 20, e a saída é: 
            t0                   t1                  t2                t0                  t1                   t2                
    "AAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCC"
    
    Nesse caso o OpenMP divide as iterações em chunks de tamanho 20 e distribui essa carga para cada thread em ordem circular, assim cada thread
    faz 20 inserções por vez(sendo 40 no total) até preencher todo o array.
    
    2. Schedule Static sem especificação do chunk
    A saída é:
                   t0                                 t1                                      t2
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    
    Nesse caso o OpenMP divide os chunks igualmente entre as threads ou seja size/numThreads, cada thread recebe um chunk-size de valor 40,
    assim a execução é feita na ordem t0 = A, t1 = B, t2 = C respectivamente resultando na saída acima.
    
    3. Schedule Dynamic com especificação do chunk
    O chunk-size possui valor 20, e a saída foi:
            t0               t1                      t2                                 t0
    "AAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    
    O OpenMP divide as interações em chunks de 20 e inicialmente distribui entre as threads sem uma ordem específica por isso a cada
    execução o resultado pode não ser o mesmo, e conforme terminam seu trabalho as threads podem solicitar mais chunks até preencher todo
    o vetor. Nesse caso os chunks foram inicialmente distribuidos para t0, t1, t2 respectivamente. A thread 0 acabou finalizando sua carga
    inicial foi executando as cargas restantes.
    
    4. Schedule Dynamic sem especificação do chunk 
    A saída foi:
    
    "ACBACACACCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCBCBBBBBBABABABABABABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACACACACCCCCCCCCCCCCCCCCCCCCCC"
    
    Nesse caso por padrão o chunk é igual 1 vai sendo distribuído sem ordem definida entre as threads, ao fim da execução cada thread teve uma
    carga diferente.
    
    5. Schedule Guided com especificação do chunk
    Com um chunk-size de valor 20 a saída foi:
                      t0                               t2                        t1                       t2
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACCCCCCCCCCCCCCCCCCCCCCCCCCCBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"
    
    A diferença desse modo para o dynamic é que conforme as threads terminam seus chunks, enquanto houver, são atribuídos chunks de menor
    tamanho(número de iterações não atribuídas divido pelo número de threads), nesse caso nunca menor que o chunk que é 20 ,para elas.
    
    6. Schedule Guided sem especificação do chunk 
                  t0                                   t1                    t2              t0
    "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCCCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    
    Esse caso é o mesmo descrito acima com a exceção de que como não foi especificado um valor para o chunk, o padrão é 1. 
    Por isso vemos que a thread B fez apenas 18 iterações.
    
    7. Schedule Auto 
    "ABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABC"
    
    Nesse caso o sistema escolheu o tipo de schedule, estático com chunk = 1
   
    
