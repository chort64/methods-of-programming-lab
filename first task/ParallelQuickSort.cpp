#include <ctime>
#include <vector>
#include <thread>
#include <iostream>

using namespace std;


// Классическая реализация быстрой сортировки
void quickSort(int* array, int low, int high) {
    int mid = array[(low + high) / 2];
    int i = low; 
    int j = high;
 
    while(i <= j) {
 
        while (array[i] < mid)
            i++;
        
        while (array[j] > mid)
            j--;
 
        if (i <= j) {
            int tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            ++i;
            --j;
        }
    }


    if (i < high) {
        quickSort(array, i, high);
    }

    if (j > low) {
        quickSort(array, low, j);
    }
}



// Первая реализация многопоточной быстрой сортировки
// где только при первой разделении массива пополам будет создан
// один дополнительный поток для одной из половин массива. 
void firstParallelQuickSort(int* array, int low, int high) {
    int mid = array[(low + high) / 2];
    int i = low; 
    int j = high;
 
    while(i <= j) {
 
        while (array[i] < mid)
            i++;
        
        while (array[j] > mid)
            j--;
 
        if (i <= j) {
            int tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            ++i;
            --j;
        }
    }

    thread newThread;

    // Если массив array разбивается на два подмассива,
    // то создается новый поток для рекурсивного вызова быстрой сортировки левого подмассива,
    // а текущий поток запускает рекурсивный вызов быстрой быстрой сортировки правого подмассива
    if (i < high && j > low) {
        newThread = thread(quickSort, array, low, j);
        quickSort(array, i, high);
        newThread.join();
    }
    else if (i < high) {
        quickSort(array, i, high);
    }
    else if (j > low) {
        quickSort(array, low, j);
    }
}


bool isSorted(int* array, int size) {

    for (int i = 1; i < size; ++i)
        if (array[i - 1] > array[i]) {
            return false;
        }
    return true;
}


// Вторая реализация многопоточной быстрой сортировки
// Здесь вводятся дополнительные глобальные переменные:
//     - количество созданных потоков - NUMBER_OF_THREADS ( данный параметр не отражает действительное кол-во потоков,
//                                                          а лишь с некоторой погрешностью ) 
//     - максимальное число потоков - ограничение для создания потоков.
// Перед рекурсивным вызовом быстрой сортировке, идет проверка, не превышено ли кол-во потоков.
// В случае, если поток можно создать, то он создается и кол-во потоков увеличивается на единицу.
int NUMBER_OF_THREADS;
int MAX_THREADS;
clock_t start;  

void secondParallelQuickSort(int* array, int low, int high) {
    int mid = array[(low + high) / 2];
    int i = low; 
    int j = high;
 
    while(i <= j) {
 
        while (array[i] < mid)
            i++;
        
        while (array[j] > mid)
            j--;
 
        if (i <= j) {
            int tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
            ++i;
            --j;
        }
    }

    if (i < high && j > low) {
        if (NUMBER_OF_THREADS < MAX_THREADS) {
            NUMBER_OF_THREADS += 1;
            thread newThread(secondParallelQuickSort, array, low, j);
            secondParallelQuickSort(array, i, high);
            newThread.join();
        } else {
            quickSort(array, i, high);
            quickSort(array, low, j);
        }
    }
    else if (i < high) {
        quickSort(array, i, high);
    }
    else if (j > low) {
        quickSort(array, low, j);
    }
}


// Функция для создания убывающего динам. массива
int* createArray(int n) {
    int* array = new int[n];
    for (int i = 0; i < n; ++i) 
        array[i] = n - i;
    return array;
}


// Выводит динамический массив array размерности size
void printArray(int* array, int size) {
    for (int i = 0; i < size; ++i)
        cout << array[i] << " ";
}


// Печатает время работы функции сортировки
void printTimeOfWork(int sizeOfArray, void functionOfSort(int*, int, int)) {
    int* array = createArray(sizeOfArray); 

    start = clock();

    functionOfSort(array, 0, sizeOfArray - 1);

    double duration = (double) (clock() - start) / CLOCKS_PER_SEC;
    cout<<"time of work: " << duration <<'\n' << '\n';
}



int main() {

    int SIZE_OF_ARRAY = 5e8;

    // Для Миллиарда элементов выполняется за ~170 секунд
    // cout << "Quick sort in one thread:" << '\n';
    // printTimeOfWork(SIZE_OF_ARRAY, quickSort);

    // Для Миллиарда элементов выполняется за ~75 секунд
    // cout << "Quick sort in first version of multi thread function:" << '\n';
    // printTimeOfWork(SIZE_OF_ARRAY, firstParallelQuickSort);

    // // Для Миллиарда элементов выполняется за ~26 секунд
    // cout << "Quick sort in second version of multi thread function:" << '\n';
    // NUMBER_OF_THREADS = 1;
    // MAX_THREADS = 20;
    // cout << "Max threads: " << MAX_THREADS << '\n';
    // printTimeOfWork(SIZE_OF_ARRAY, secondParallelQuickSort);

    // // Для Миллиарда элементов выполняется за ~22 секунд
    // cout << "Quick sort in second version of multi thread function:" << '\n';
    // NUMBER_OF_THREADS = 1;
    // MAX_THREADS = 30;
    // cout << "Max threads: " << MAX_THREADS << '\n';
    // printTimeOfWork(SIZE_OF_ARRAY, secondParallelQuickSort);

    // // Для Миллиарда элементов выполняется за ~23 секунд
    // cout << "Quick sort in second version of multi thread function:" << '\n';
    // NUMBER_OF_THREADS = 1;
    // MAX_THREADS = 50;
    // cout << "Max threads: " << MAX_THREADS << '\n';
    // printTimeOfWork(SIZE_OF_ARRAY, secondParallelQuickSort);

    // // Для Миллиарда элементов выполняется за ~21 секунд
    cout << "Quick sort in second version of multi thread function:" << '\n';
    NUMBER_OF_THREADS = 1;
    MAX_THREADS = 8;
    cout << "Max threads: " << MAX_THREADS << '\n';
    printTimeOfWork(SIZE_OF_ARRAY, secondParallelQuickSort);

}