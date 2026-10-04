#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 插入排序
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 選擇排序
void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        int temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

// 快速排序輔助函數：分區
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

// 快速排序
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// 合併排序輔助函數：合併
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// 合併排序
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// 堆排序輔助函數：建立堆
void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;

    if (right < n && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

// 堆排序
void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0);
    }
}

// 生成隨機數據集
void generateRandomArray(int arr[], int n, int seed) {
    srand(seed);
    for (int i = 0; i < n; i++) {
        arr[i] = rand();
    }
}

// 驗證排序正確性
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n = 10000;  // 初始數據集大小
    int seed = 42;  // 固定種子
    int arr[n];

    generateRandomArray(arr, n, seed);

#ifdef DEBUG
    printf("原始數據:\n");
    printArray(arr, n);
#endif

    // 插入排序
    generateRandomArray(arr, n, seed);
    clock_t start = clock();
    insertionSort(arr, n);
    clock_t end = clock();
    printf("插入排序時間: %lf 秒\n", (double)(end - start) / CLOCKS_PER_SEC);
    
#ifdef DEBUG
    printf("插入排序後的數據:\n");
    printArray(arr, n);
#endif

    // 選擇排序
    generateRandomArray(arr, n, seed);
    start = clock();
    selectionSort(arr, n);
    end = clock();
    printf("選擇排序時間: %lf 秒\n", (double)(end - start) / CLOCKS_PER_SEC);

#ifdef DEBUG
    printf("選擇排序後的數據:\n");
    printArray(arr, n);
#endif

    // 快速排序
    generateRandomArray(arr, n, seed);
    start = clock();
    quickSort(arr, 0, n - 1);
    end = clock();
    printf("快速排序時間: %lf 秒\n", (double)(end - start) / CLOCKS_PER_SEC);

#ifdef DEBUG
    printf("快速排序後的數據:\n");
    printArray(arr, n);
#endif

    // 合併排序
    generateRandomArray(arr, n, seed);
    start = clock();
    mergeSort(arr, 0, n - 1);
    end = clock();
    printf("合併排序時間: %lf 秒\n", (double)(end - start) / CLOCKS_PER_SEC);

#ifdef DEBUG
    printf("合併排序後的數據:\n");
    printArray(arr, n);
#endif

    // 堆排序
    generateRandomArray(arr, n, seed);
    start = clock();
    heapSort(arr, n);
    end = clock();
    printf("堆排序時間: %lf 秒\n", (double)(end - start) / CLOCKS_PER_SEC);

#ifdef DEBUG
    printf("堆排序後的數據:\n");
    printArray(arr, n);
#endif

    return 0;
}

