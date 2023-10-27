#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <cmath>
using namespace std;

void Merge_(int* arr, int begin, int mid, int end)
{
    int const leftArrayLen = mid - begin + 1;
    int const rightArrayLen = end - mid;

    //создание вспомогательных массивов
    int* leftArray = new int[leftArrayLen];
    int* rightArray = new int[rightArrayLen];

    //заполнение массивов

    for (int i = 0; i < leftArrayLen; i++)
    {
        leftArray[i] = arr[begin + i];
    }
    for (int i = 0; i < rightArrayLen; i++)
    {
        rightArray[i] = arr[mid + 1 + i];
    }

    //индексы для вспомогательных и и основного массива
    int a, b, c;
    a = b = 0;
    c = begin;

    while (a < leftArrayLen && b < rightArrayLen)
    {
        if (leftArray[a] <= rightArray[b])
        {
            arr[c] = leftArray[a];
            a++;
        }
        else
        {
            arr[c] = rightArray[b];
            b++;
        }
        c++;
    }

    //копирование оставшихся элементов
    while (a < leftArrayLen)
    {
        arr[c] = leftArray[a];
        a++;
        c++;
    }
    while (b < rightArrayLen)
    {
        arr[c] = rightArray[b];
        c++;
        b++;
    }

    delete[] leftArray;
    delete[] rightArray;
}
int* InsertionSort_(int* arr, int left, int right)
{
    for (int i = left + 1; i <= right; i++)
    {
        for (int j = i; j > left; j--)
        {
            if (arr[j] < arr[j - 1])
            {
                swap(arr[j], arr[j - 1]);
            }
            else
            {
                break;
            }
        }
    }
    return arr;
}

class TimSortClass
{
    int minrun;



    struct Stack
    {
        int Start;
        int Lenght;
        Stack* next;
    };
    Stack* StackHead = nullptr;
    int Run = 0;


    void MinRun(int len)
    {
        while (len >= 64)
        {
            len /= 2;
        }
        minrun = len;

    }


    void AppendStack(int start, int lenght)
    {
        Stack* elem = new Stack;
        elem->Lenght = lenght;
        elem->Start = start;

        if (StackHead == nullptr)
        {
            elem->next = nullptr;
        }
        else
        {
            elem->next = StackHead;

        }
        StackHead = elem;
        Run++;
    }

    Stack* GetByIndex(int index)
    {
        Stack* curr = StackHead;
        for (int i = 0; i < index; i++)
        {
            curr = curr->next;
        }
        return curr;

    }

    void DisplayStack()
    {
        Stack* curr = StackHead;
        while (curr != nullptr)
        {
            std::cout << curr->Start << ' ' << curr->Lenght << '\n';
            curr = curr->next;
        }
    }

    void ReverseRun(int* arr, int start, int end)
    {
        for (int i = start; i < start + (end - start) / 2 + 1; i++)
        {
            swap(arr[i], arr[end - i]);
        }
    }


    void findRuns(int* arr, int len)
    {

        int i = 0;
        int startRun = 0;
        int runLen = 1;

        while (i < len)
        {
            if (i == len - 1)
            {

            }
            else
            {
                bool increasing = true;
                if (arr[i] < arr[i + 1]) //Возрастающая последовательность
                {
                    i++;
                    runLen++;
                    while (arr[i] < arr[i + 1] and i < len - 1)
                    {
                        i++;
                        runLen++;
                    }

                }
                else
                {
                    //убывающая последовательность
                    increasing = false;
                    i++;
                    runLen++;
                    while (arr[i] > arr[i + 1] and i < len - 1)
                    {
                        i++;
                        runLen++;
                    }
                }

                if (runLen < minrun)
                {
                    if (startRun + minrun <= len)
                    {
                        InsertionSort_(arr, startRun, startRun + minrun - 1);
                        AppendStack(startRun, minrun);
                        i = startRun + minrun;
                    }
                    else
                    {
                        InsertionSort_(arr, startRun, len - 1);
                        AppendStack(startRun, len - 1 - startRun);
                        break;
                    }
                }
                else
                {
                    if (!increasing)
                    {
                        ReverseRun(arr, startRun, startRun + runLen - 1);
                    }
                    AppendStack(startRun, runLen);
                    i += runLen;
                }
                Merging(arr);

                startRun = i;
                runLen = 1;
            }
        }

    }
    void StackMerge(Stack* firstElem, Stack* secondElem)
    {
        Stack* prev = StackHead;
        while (prev->next != firstElem and prev != firstElem)
        {
            prev = prev->next;
        }
        prev->next = secondElem;
        secondElem->Lenght += firstElem->Lenght;
        if (prev == firstElem)
        {
            StackHead = secondElem;
        }
        delete firstElem;
    }
    void ForceMerge(int* arr)
    {
        while (Run > 1)
        {
            if (Run >= 3)
            {
                if (GetByIndex(0)->Lenght > GetByIndex(2)->Lenght)
                {
                    int begin = GetByIndex(2)->Start;
                    int end = GetByIndex(1)->Lenght + GetByIndex(1)->Start - 1;
                    int mid = begin + GetByIndex(2)->Lenght - 1;
                    StackMerge(GetByIndex(1), GetByIndex(2));
                    Merge_(arr, begin, mid, end);
                    Run--;
                }
                else
                {
                    int begin = GetByIndex(1)->Start;
                    int end = GetByIndex(0)->Lenght + GetByIndex(0)->Start - 1;
                    int mid = begin + GetByIndex(1)->Lenght - 1;
                    StackMerge(GetByIndex(0), GetByIndex(1));
                    Merge_(arr, begin, mid, end);
                    Run--;
                }
            }
            if (Run == 2)
            {
                int begin = GetByIndex(1)->Start;
                int end = GetByIndex(0)->Lenght + GetByIndex(0)->Start - 1;
                int mid = begin + GetByIndex(1)->Lenght - 1;
                StackMerge(GetByIndex(0), GetByIndex(1));
                Merge_(arr, begin, mid, end);
                Run--;


            }
        }
        StackHead->next = nullptr;
        StackHead->Lenght = NULL;
        StackHead->Start = NULL;
        delete StackHead;
    }

    void Merging(int* arr)
    {
        while (true)
        {
            bool three = true; bool two = true;
            if (Run >= 3)
            {

                if (GetByIndex(0)->Lenght + GetByIndex(1)->Lenght > GetByIndex(2)->Lenght)
                {
                    three = false;
                    if (GetByIndex(0)->Lenght > GetByIndex(2)->Lenght)
                    {

                        int begin = GetByIndex(2)->Start;
                        int end = GetByIndex(1)->Lenght + GetByIndex(1)->Start - 1;
                        int mid = begin + GetByIndex(2)->Lenght - 1;
                        StackMerge(GetByIndex(1), GetByIndex(2));
                        Merge_(arr, begin, mid, end);
                        Run--;
                    }
                    else
                    {
                        int begin = GetByIndex(1)->Start;
                        int end = GetByIndex(0)->Lenght + GetByIndex(0)->Start - 1;
                        int mid = begin + GetByIndex(1)->Lenght - 1;
                        StackMerge(GetByIndex(0), GetByIndex(1));
                        Merge_(arr, begin, mid, end);
                        Run--;
                    }
                }
            }
            if (Run >= 2)
            {

                if (GetByIndex(0)->Lenght > GetByIndex(1)->Lenght)
                {
                    two = false;

                    int begin = GetByIndex(1)->Start;
                    int end = GetByIndex(0)->Lenght + GetByIndex(0)->Start - 1;
                    int mid = begin + GetByIndex(1)->Lenght - 1;
                    StackMerge(GetByIndex(0), GetByIndex(1));
                    Merge_(arr, begin, mid, end);
                    Run--;
                }

            }
            if (two and three || Run == 1)
            {
                break;
            }
        }
    }

public:
    void TimSort(int* arr, int len)
    {
        MinRun(len);
        findRuns(arr, len);

        ForceMerge(arr);
        Run = 0;

    }




};

// Function to print an array
void printArray(int* arr, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        cout << arr[i] << " ";
        cout << endl;
    }
}

void selectionSort(int* arr, int n)
{
    int i, j, min_idx;
    for (i = 0; i < n - 1; i++) {
        min_idx = i;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx])
                min_idx = j;
        }

        if (min_idx != i)
            swap(arr[min_idx], arr[i]);
    }
}

void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}
void bubbleSort(int* arr, int n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
            break;
    }
}
// Merge sort 
void Merge(int* arr, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* left_arr = new int[n1], * right_arr = new int[n2];
    for (i = 0; i < n1; ++i)
        left_arr[i] = arr[left + i];
    for (j = 0; j < n2; ++j)
        right_arr[j] = arr[mid + 1 + j];
    i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            ++i;
        }
        else {
            arr[k] = right_arr[j];
            ++j;
        }
        ++k;
    }
    while (i < n1) {
        arr[k] = left_arr[i];
        ++i; ++k;
    }
    while (j < n2) {
        arr[k] = right_arr[j];
        ++j; ++k;
    }
    delete[] left_arr;
    delete[] right_arr;
}
void MergeSort(int* arr, int begin, int end) {
    if (begin < end) {
        int mid = begin + (end - begin) / 2;
        MergeSort(arr, begin, mid);
        MergeSort(arr, mid + 1, end);
        Merge(arr, begin, mid, end);
    }
}



//Быстрая

int Partition(int* arr, int begin, int end)
{
    int baseElem = arr[end]; //опорный элемент 
    int PartIndex = begin;


    for (int i = begin; i < end; i++)
    {
        if (arr[i] < baseElem)
        {
            swap(arr[i], arr[PartIndex]); //перенос в левый "массив" элементов, меньших опорного
            PartIndex++;
        }
    }
    swap(arr[end], arr[PartIndex]);//разграничение двух "массивов" 

    return PartIndex;
}
void quickSort(int* arr, int begin, int end)
{
    if (begin >= end) return;

    int baseElem = Partition(arr, begin, end);
    quickSort(arr, begin, baseElem - 1);
    quickSort(arr, baseElem + 1, end);

}


void shellSort(int* arr, int len, int num) {
    int i, j, k, temp, gap;
    int gaps[100];
    int gapSize = 0;
    switch (num) {
    case 0:
        temp = len / 2;
        while (temp > 0) {
            gaps[gapSize++] = temp;
            temp /= 2;
        }
        break;
    case 1: // (A000225)
        i = 1;
        temp = pow(2, i) - 1;
        while (temp < len) {
            gaps[gapSize++] = temp;
            ++i; temp = pow(2, i) - 1;
        }
        std::reverse(gaps, gaps + gapSize);
        break;
    case 2: //  (A003462)
        i = 1;
        gaps[gapSize++] = 1;
        temp = (pow(3, i) - 1) / 2;
        while (temp < len / 3) {
            gaps[gapSize++] = temp;
            ++i; temp = (pow(3, i) - 1) / 2;
        }
        std::reverse(gaps, gaps + gapSize);
        break;
    }
    for (i = 0; i < gapSize; ++i) {
        gap = gaps[i];
        for (j = gap; j < len; ++j) {
            temp = arr[j];
            for (k = j; k >= gap && arr[k - gap] > temp; k -= gap)
                arr[k] = arr[k - gap];
            arr[k] = temp;
        }
    }
}
//Пирамидальная
void heapify(int* arr, int n, int i)
{
    int largest = i;
    // Инициализируем наибольший элемент как корень
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    // Если левый дочерний элемент больше корня
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // Если правый дочерний элемент больше, чем самый большой элемент на данный момент
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // Если самый большой элемент не корень
    if (largest != i)
    {
        swap(arr[i], arr[largest]);

        // Рекурсивно преобразуем в двоичную кучу затронутое поддерево
        heapify(arr, n, largest);
    }
}

// Основная функция, выполняющая пирамидальную сортировку
void heapSort(int* arr, int n)
{
    // Построение кучи 
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i >= 0; i--)
    {
        // Перемещаем текущий корень в конец
        swap(arr[0], arr[i]);

        // вызываем процедуру heapify на уменьшенной куче
        heapify(arr, i, 0);
    }
}

void insertionsort(int a[], int low, int high)
{

    for (int i = low + 1; i <= high; i++)
    {
        int value = a[i];
        int j = i;
        while (j > low && a[j - 1] > value)
        {
            a[j] = a[j - 1];
            j--;
        }

        a[j] = value;
    }
}

// Function to partition the array using Lomuto partition scheme
int partition(int a[], int low, int high)
{
    int pivot = a[high];

    int pIndex = low;

    for (int i = low; i < high; i++)
    {
        if (a[i] <= pivot)
        {
            swap(a[i], a[pIndex]);
            pIndex++;
        }
    }

    swap(a[pIndex], a[high]);
    return pIndex;
}

// Quicksort randomized partition to rearrange elements across pivot
int randPartition(int a[], int low, int high)
{
    int pivotIndex = rand() % (high - low + 1) + low;

    swap(a[pivotIndex], a[high]);

    return partition(a, low, high);
}

// Function to perform heapsort on the given range of elements
void heapsort(int* begin, int* end)
{
    make_heap(begin, end);
    sort_heap(begin, end);
}



void Swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}
int ChoosePivot(int* arr, int left, int right) {
    int mid = left + (right - left) / 2;
    if (arr[mid] < arr[left])
        Swap(arr[left], arr[mid]);
    if (arr[right] < arr[left])
        Swap(arr[left], arr[right]);
    if (arr[right] < arr[mid])
        Swap(arr[mid], arr[right]);
    return mid;
}
int PartitionIntro(int* arr, int left, int right) {
    int pivotIndex = ChoosePivot(arr, left, right);
    int pivotValue = arr[pivotIndex];
    Swap(arr[pivotIndex], arr[right]);
    int storeIndex = left;
    for (int i = left; i < right; i++) {
        if (arr[i] < pivotValue) {
            Swap(arr[i], arr[storeIndex]);
            storeIndex++;
        }
    }
    Swap(arr[storeIndex], arr[right]);
    return storeIndex;
}
void QuickSortIntro(int* arr, int begin, int end, int depthLimit) {
    while (end - begin > 16) {
        if (depthLimit == 0) {
            // If depth limit is reached, switch to heapsort
            std::make_heap(arr + begin, arr + end + 1);
            std::sort_heap(arr + begin, arr + end + 1);
            return;
        }
        --depthLimit;
        int pivotIndex = PartitionIntro(arr, begin, end);
        QuickSortIntro(arr, pivotIndex + 1, end, depthLimit);
        end = pivotIndex - 1;
    }
    insertionsort(arr, begin, end);
}

void IntroSort(int* arr, int length) {
    int depthLimit = 2 * log(length);
    QuickSortIntro(arr, 0, length - 1, depthLimit);
}


int main()
{
    fstream f;

    setlocale(LC_ALL, "rus");
    int size = 20000;
    int numb;
    auto begin = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();


    TimSortClass ts;

    int count = 0;
    /*srand((unsigned int)time(NULL));
    for (int i = 0; i < size; i++) {
        //arr[j] = i;
        arr[i] = rand() % 20 + 1;
    }*/
    f.open("revmerge.txt", ios::out);
    // for (int size = 100; size < 21000; size += 1000) {
        // f << size << "\t";
    int* arr = new int[size];
    do {

        cout << "Выберите сортировку для массива:" << endl
            << "1.Сортировка выбором" << endl
            << "2.Сортировка вставками" << endl
            << "3.Сортировка пузырьком" << endl
            << "4.Сортировка слиянием" << endl
            << "5.Быстрая сортировка" << endl
            << "6.Сортировка Шелла" << endl
            << "7.Пирамидальная сортировка" << endl
            << "8.TimSort" << endl
            << "9.IntroSort" << endl;
        cin >> numb;
        int temp = size;
        /*for (int i = 0; i < size; i++) {
         arr[i] = i;
     }*/
        srand((unsigned int)time(NULL));
        for (int i = 0; i < size; i++) {

            arr[i] = rand() % 25000 + 0.85 * size;
        }
        /*for (int i = 0; i < size * 0.8; i++) {
           arr[i] = i;
        }
        for (int i = size * 0.8; i < size; i++) {

            arr[i] = rand() % 25000 + 0.85*size;
        }*/
        /*for (int i = 0; i < size; i++)
        {
            arr[i] = temp;
            temp--;
        }*/
        switch (numb) {
        case 1:


            begin = std::chrono::steady_clock::now();
            selectionSort(arr, size);
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << ' ';
            // search_time = ((float)t) / CLOCKS_PER_SEC;
            f << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t";
            cout << "Sorted array: \n";
            break;


        case 2:

            begin = std::chrono::steady_clock::now();
            insertionSort(arr, size);
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << ' ';
            // search_time = ((float)t) / CLOCKS_PER_SEC;
            f << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t";
            cout << "Sorted array: \n";
            //printArray(arr, size);
            break;
        case 3:

            begin = std::chrono::steady_clock::now();
            bubbleSort(arr, size);
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << ' ';
            // search_time = ((float)t) / CLOCKS_PER_SEC;
            f << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t";
            cout << "Sorted array: \n";
            //printArray(arr, n);
            break;
        case 4:

            begin = std::chrono::steady_clock::now();
            MergeSort(arr, 0, 0 + size - 1);
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << ' ';
            // search_time = ((float)t) / CLOCKS_PER_SEC;
            f << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t";
            //printArray(arr, size);
            break;
        case 5:

            begin = std::chrono::steady_clock::now();
            quickSort(arr, 0, size - 1);
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << ' ';
            // search_time = ((float)t) / CLOCKS_PER_SEC;
            f << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t";
            if (size == 100) printArray(arr, size);
            //printArray(arr, size);
            break;
        case 6:

            begin = std::chrono::steady_clock::now();
            shellSort(arr, size, 2);
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << ' ';
            // search_time = ((float)t) / CLOCKS_PER_SEC;
            f << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t";
            cout << "Sorted array: \n";
            //printArray(arr, size);
            break;
        case 7:
            cout << "array: \n";
            //printArray(arr, size);

            begin = std::chrono::steady_clock::now();
            heapSort(arr, size);
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << ' ';
            // search_time = ((float)t) / CLOCKS_PER_SEC;
            f << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t";
            cout << "Sorted array: \n";
            // printArray(arr, size);
            break;
        case 8:

            begin = std::chrono::steady_clock::now();
            ts.TimSort(arr, size);
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << ' ';
            // search_time = ((float)t) / CLOCKS_PER_SEC;
            f << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t";
            //cout << "Time: " << elapsed_ms.count();
            cout << "Sorted array: \n";
            //printArray(arr, size);
            break;
        case 9:
            begin = std::chrono::steady_clock::now();
            IntroSort(arr, size);
            end = std::chrono::steady_clock::now();
            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << ' ';
            // search_time = ((float)t) / CLOCKS_PER_SEC;
            f << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "\t";
            //cout << "Time: " << elapsed_ms.count();
            cout << "Sorted array: \n";
            //printArray(arr, size);
            break;

        }

        count += 1;
    } while (numb != 0);
    f << endl;
    delete[] arr;
    //   }

    f.close();
}
