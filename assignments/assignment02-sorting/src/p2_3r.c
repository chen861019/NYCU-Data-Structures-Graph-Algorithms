#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void selectionSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++)
            if (arr[j] < arr[min_idx])
                min_idx = j;
        int acc = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = acc;
    }
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];  // 使用高位作為 pivot
        int i = (low - 1);

        for (int j = low; j < high; j++) {  // 使用 j < high
            if (arr[j] < pivot) {
                i++;
                // 交換 arr[i] 和 arr[j]
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        
        // 將 pivot 放到正確的位置
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        
        int pi = i + 1;  // pivot 的最終位置
        quickSort(arr, low, pi - 1);   // 遞歸排序左側
        quickSort(arr, pi + 1, high);  // 遞歸排序右側
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // 遞歸排序左半部分
        mergeSort(arr, l, m);
        // 遞歸排序右半部分
        mergeSort(arr, m + 1, r);

        // 合併已排序的部分
        int n1 = m - l + 1; // 左半部分的大小
        int n2 = r - m;     // 右半部分的大小
        int L[n1], R[n2];  // 創建輔助陣列

        // 填充左半部分
        for (int i = 0; i < n1; i++)
            L[i] = arr[l + i];
        // 填充右半部分
        for (int j = 0; j < n2; j++)
            R[j] = arr[m + 1 + j];

        int i = 0, j = 0, k = l;

        // 合併兩個已排序的輔助陣列
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

        // 複製剩餘的左半部分（如果有的話）
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        // 複製剩餘的右半部分（如果有的話）
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }
}

void heapSort(int arr[], int n) {
    // 定義 heapify 作為 heapSort 的內部函數
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

            heapify(arr, n, largest); // 遞歸調用 heapify
        }
    }

    // 建立最大堆
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // 一個一個提取元素
    for (int i = n - 1; i > 0; i--) {
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        heapify(arr, i, 0); // 重建堆
    }
}

// 從檔案讀取數列
int* readArrayFromFile(const char *filename, int *n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("無法打開檔案 %s\n", filename);
        exit(1);
    }

    // 預設分配空間，稍後調整
    int capacity = 1024;
    int *arr = (int *)malloc(capacity * sizeof(int));
    *n = 0;

    // 逐行讀取數字
    while (fscanf(file, "%d", &arr[*n]) == 1) {
        (*n)++;
        if (*n >= capacity) {
            capacity *= 2;
            arr = (int *)realloc(arr, capacity * sizeof(int));
        }
    }

    fclose(file);
    return arr;
}

struct timespec diff(struct timespec start, struct timespec end) {
    struct timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

int main() {
    const char *filename = "random_numbers.txt";
    int n;
    struct timespec start, end, time_diff;

    int *arr = readArrayFromFile(filename, &n);

#ifdef DEBUG
    printf("原始數據:\n");
    printArray(arr, n);
#endif

    // 插入排序
    clock_gettime(CLOCK_MONOTONIC, &start);
    insertionSort(arr, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_diff = diff(start, end);
    printf("插入排序時間: %ld.%09ld 秒\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
    printf("插入排序後的數據:\n");
    printArray(arr, n);
#endif

    // 選擇排序
    arr = readArrayFromFile(filename, &n);  // 重新讀取數據
    clock_gettime(CLOCK_MONOTONIC, &start);
    selectionSort(arr, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_diff = diff(start, end);
    printf("選擇排序時間: %ld.%09ld 秒\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
    printf("選擇排序後的數據:\n");
    printArray(arr, n);
#endif

    // 快速排序
    arr = readArrayFromFile(filename, &n);  // 重新讀取數據
    clock_gettime(CLOCK_MONOTONIC, &start);
    quickSort(arr, 0, n - 1);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_diff = diff(start, end);
    printf("快速排序時間: %ld.%09ld 秒\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
    printf("快速排序後的數據:\n");
    printArray(arr, n);
#endif

    // 合併排序
    arr = readArrayFromFile(filename, &n);  // 重新讀取數據
    clock_gettime(CLOCK_MONOTONIC, &start);
    mergeSort(arr, 0, n - 1);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_diff = diff(start, end);
    printf("合併排序時間: %ld.%09ld 秒\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
    printf("合併排序後的數據:\n");
    printArray(arr, n);
#endif

    // 堆排序
    arr = readArrayFromFile(filename, &n);  // 重新讀取數據
    clock_gettime(CLOCK_MONOTONIC, &start);
    heapSort(arr, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    time_diff = diff(start, end);
    printf("堆排序時間: %ld.%09ld 秒\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
    printf("堆排序後的數據:\n");
    printArray(arr, n);
#endif

    free(arr);
    return 0;
}

