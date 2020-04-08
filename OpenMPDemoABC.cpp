#include<algorithm>
#include<iostream>
#include<string>
#include<omp.h>


enum ScheduleMode{ 
    DYNAMIC_WITHOUT_CHUNK, 
    DYNAMIC_WITH_CHUNK, 
    STATIC_WITH_CHUNK,
    STATIC_WITHOUT_CHUNK,
    GUIDED_WITH_CHUNK,
    GUIDED_WITHOUT_CHUNK,
    AUTO,
    INCORRECT
};

class SharedArray{
    private:
        char *array;
        int size;
        bool usecritical;
    public:
        SharedArray(int n, bool use) : size(n), usecritical(use){
            array = new char[size];
            std::fill(array, array+size, '-');
        }

        ~SharedArray(){
            delete[] array;
        }
        
        void addChar(char c, int index){
            #pragma omp critical
            {
                array[index] = c;
                spendSomeTime();
            }
            
        }

        int countOcurrences(char c){
            return std::count(array, array+size, c);
        }

        std::string toString(){
            return std::string(array, size);
        }
    private:
        void spendSomeTime(){
            for(int i = 0; i < 10000; i++){
                for(int j = 0; j< 100; j++){
                    // These loops shouldn`t be removed by the compiler
                }
            }
        }

};

class ArrayFiller{
    private:
        static const int nThreads = 3;
        static const int nTimes = 20;
        SharedArray* array;
        ScheduleMode scheduleMode;
    public:
        ArrayFiller(bool usecritical, ScheduleMode sch): scheduleMode(sch){
            array = new SharedArray(nThreads * nTimes, usecritical);
        }

        void fillArrayConcurrently(){
            int i;

            omp_set_num_threads(nThreads);

            switch(scheduleMode){
                case DYNAMIC_WITH_CHUNK:
                    omp_set_schedule(omp_sched_dynamic, nTimes);
                    break;
                case DYNAMIC_WITHOUT_CHUNK:
                    /* chunksize < 1 uses default value */
                    omp_set_schedule(omp_sched_dynamic, 0);
                    break;
                case STATIC_WITH_CHUNK:
                    omp_set_schedule(omp_sched_static, nTimes);
                    break;
                case STATIC_WITHOUT_CHUNK:
                    omp_set_schedule(omp_sched_static, 0);
                    break;
                case GUIDED_WITH_CHUNK:
                    omp_set_schedule(omp_sched_guided, nTimes);
                    break;
                case GUIDED_WITHOUT_CHUNK:
                    omp_set_schedule(omp_sched_guided, 0);
                    break;
                case AUTO:
                    omp_set_schedule(omp_sched_auto, 0);
                    break;
            }
    
            #pragma omp parallel shared(array) private(i)
            {
                #pragma omp for schedule(runtime)
                for(i = 0; i < nTimes * nThreads; i++){
                    array->addChar('A'+omp_get_thread_num(), i);
                }
            }
        }

        void printStats(){
            std::cout << array->toString() << std::endl;
            for(int i = 0; i < nThreads; i++)
                std::cout << (char) ('A'+i) << "="
                          << array->countOcurrences('A'+i) << " ";
            std::cout << std::endl;
        }

        ~ArrayFiller(){
            delete array;
        }
};

int main(){
    std::cout << "Exemplo anterior mas substituindo POSIX por OpenMP" << std::endl;
    std::cout << "Pra cada thread adiciona um char no array 20 vezes "<< std::endl;

    std::cout << "Caso 1 schedule static com especificacao do chunk" << std::endl;
    ArrayFiller m1(true, STATIC_WITH_CHUNK);
    m1.fillArrayConcurrently();
    m1.printStats();

    std::cout << "Caso 2 schedule static sem especificacao do chunk" << std::endl;
    ArrayFiller m2(true, STATIC_WITHOUT_CHUNK);
    m2.fillArrayConcurrently();
    m2.printStats();

    std::cout << "Caso 3 schedule dynamic com especificacao do chunk" << std::endl;
    ArrayFiller m3(true, DYNAMIC_WITH_CHUNK);
    m3.fillArrayConcurrently();
    m3.printStats();

    std::cout << "Caso 4 schedule dynamic sem especificacao do chunk" << std::endl;
    ArrayFiller m4(true, DYNAMIC_WITHOUT_CHUNK);
    m4.fillArrayConcurrently();
    m4.printStats();

    std::cout << "Caso 5 schedule guided com especificacao do chunk" << std::endl;
    ArrayFiller m5(true, GUIDED_WITH_CHUNK);
    m5.fillArrayConcurrently();
    m5.printStats();

    std::cout << "Caso 6 schedule guided sem especificacao do chunk" << std::endl;
    ArrayFiller m6(true, GUIDED_WITHOUT_CHUNK);
    m6.fillArrayConcurrently();
    m6.printStats();

    std::cout << "Caso 7 schedule auto" << std::endl;
    ArrayFiller m7(true, AUTO);
    m7.fillArrayConcurrently();
    m7.printStats();

    return 0;
}