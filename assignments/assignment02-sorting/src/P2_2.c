//Programming Assignment II 
//Sorting Algorithms
//author : 陳沛安
//date: 2024/09/25
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//插入排序演算法：將數列中的每個元素插入到已排序的部分
void insertionSort(int arr[], int n) {
	//從第二個元素開始
    for (int i = 1; i < n; i++) {
        int acc = arr[i];  // 將當前元素儲存為acc
        int j = i - 1;
        //移動比acc大的元素到右邊，為acc騰出位置
        while (j >= 0 && arr[j] > acc) {
            arr[j + 1] = arr[j];  //將元素向右移
            j--;
        }
        arr[j + 1] = acc;  //將acc插入正確的位置
    }
}
//選擇排序演算法：從數列中選出最小的元素，並與當前元素交換
void selectionSort(int arr[], int n){
	for (int i = 0; i < n-1; i++) {
        int minIdx = i;  //假設當前元素是最小的

        //尋找剩餘元素中的最小值
        for (int j = i+1; j < n; j++) {
            if (arr[j] < arr[minIdx]) { //找到比當前元素更小的值
                minIdx = j;  // 更新最小值的索引
            }
        }
        //將最小值與當前元素交換
    	int acc = arr[i];
		arr[i] = arr[minIdx];
		arr[minIdx] = acc;
	}
}
// 快速排序演算法：選擇一個pivot，將數列分為兩部分並遞歸排序
void quickSort(int arr[], int low, int high) {
    if (low < high) {	
    	// 隨機選擇一個 pivot
        int randomIndex = low + rand() % (high - low);
        // 將 pivot 與最後一個元素交換
        int acc = arr[randomIndex];
        arr[randomIndex] = arr[high];
        arr[high] = acc;
    	
        int pivot = arr[high];  //使用數列的最後一個元素作為pivot
        int i = (low - 1); //利用i用來分割較小元素和較大元素
		//遍歷數列並根據pivot值將元素劃分
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                // 交換 arr[i] 和 arr[j]
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        // 將 pivot 放到正確的位置
        acc = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = acc;
        int pivotIdx = i + 1;  // pivot 的最終位置
        quickSort(arr, low, pivotIdx - 1);   // 遞歸排序左側
        quickSort(arr, pivotIdx + 1, high);  // 遞歸排序右側
    }
}
//合併排序演算法：將數列拆分為左右兩部分，分別遞歸排序後合併
void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2; //找到中間點
        //遞歸排序左半部分
        mergeSort(arr, l, m);
        //遞歸排序右半部分
        mergeSort(arr, m + 1, r);
        //合併已排序的部分
        int n1 = m - l + 1; //左半部分的大小
        int n2 = r - m;     //右半部分的大小
        int L[n1], R[n2];  //創建輔助陣列
        //將左半部分數據複製到L
        for (int i = 0; i < n1; i++)
            L[i] = arr[l + i];
        // 將右半部分數據複製到R
        for (int j = 0; j < n2; j++)
            R[j] = arr[m + 1 + j];
        int i = 0, j = 0, k = l;
        //將L和R中的元素比較後合併到arr中
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
        //複製剩餘的左半部分（如果有的話）
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
        //複製剩餘的右半部分（如果有的話）
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }
}
// 堆排序演算法：使用堆結構進行排序
void heapSort(int arr[], int n) {
    // 定義heapify函數，確保子樹滿足最大堆的性質
    void heapify(int arr[], int n, int i) {
        int largest = i;  // 假設父節點為最大值
        int left = 2 * i + 1;  // 左子節點
        int right = 2 * i + 2; // 右子節點
		// 如果左子節點比父節點大，則更新最大值索引
        if (left < n && arr[left] > arr[largest])
            largest = left;
		// 如果右子節點比父節點大，則更新最大值索引
        if (right < n && arr[right] > arr[largest])
            largest = right;
		// 如果最大值不是父節點，則交換並遞歸調整子樹
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
    // 每次提取堆頂（最大值）並將其與最後一個元素交換
    for (int i = n - 1; i > 0; i--) {
        int acc = arr[0];
        arr[0] = arr[i];
        arr[i] = acc;
        heapify(arr, i, 0); // 對剩餘部分重新建堆
    }
}
// 生成隨機數列
void generateRandomArray(int arr[], int n, int seed) {
    srand(seed); // 使用指定的隨機種子
    for (int i = 0; i < n; i++) {
        arr[i] = rand(); // 生成隨機數
    }
}
// 檢查數列是否已排序
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
// 檢查數列是否已經從小到大排序
int isSorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;  // 如果有任意一個元素比後一個大，則數列未排序
        }
    }
    return 1;  // 如果整個數列都符合，則已排序
}
// 計算兩個時間點的差異
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
    int n ;
#ifdef DEBUG
    n = 20; // 小型數據集用於除錯模式 
#else
	n = 10000;  // 大型數據集
#endif
	int seed = 7;  // 固定隨機種子
    int arr[n]; // 數列 
    struct timespec start, end, time_diff; // 計時變數 
    generateRandomArray(arr, n, seed); // 生成隨機數列 
 
#ifdef DEBUG   
    printf("Original data:\n");
	printArray(arr, n);
#endif
    // 插入排序
    generateRandomArray(arr, n, seed);  // 重新生成隨機數據
    clock_gettime(CLOCK_MONOTONIC, &start);  // 開始計時
    insertionSort(arr, n);  // 執行插入排序
    clock_gettime(CLOCK_MONOTONIC, &end);  // 結束計時
    time_diff = diff(start, end);  // 計算時間差
    printf("1.Insertion Sort time: %ld.%09ld seconds\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
    printf("Data after Insertion Sort:\n");
    printArray(arr, n);
	if (isSorted(arr, n)) {
    	printf("Array sorted correctly!\n");
	} else {
    	printf("Array sorting error!\n");
	}
#endif

    // 選擇排序
    generateRandomArray(arr, n, seed);  // 重新生成隨機數據
    clock_gettime(CLOCK_MONOTONIC, &start);  // 開始計時
    selectionSort(arr, n); // 執行選擇排序 
    clock_gettime(CLOCK_MONOTONIC, &end);  // 結束計時
    time_diff = diff(start, end);  // 計算時間差
    printf("2.Selection Sort time: %ld.%09ld seconds\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
	printf("Data after Selection Sort:\n");
    printArray(arr, n);
    if (isSorted(arr, n)) {
    	printf("Array sorted correctly!\n");
	} else {
    	printf("Array sorting error!\n");
	}
#endif

    // 快速排序
    generateRandomArray(arr, n, seed);  // 重新生成隨機數據
    clock_gettime(CLOCK_MONOTONIC, &start);  // 開始計時
    quickSort(arr, 0, n - 1); // 執行快速排序 
    clock_gettime(CLOCK_MONOTONIC, &end);  // 結束計時
    time_diff = diff(start, end);  // 計算時間差
    printf("3.Quick Sort time: %ld.%09ld seconds\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
    printf("Data after Quick Sort:\n");
    printArray(arr, n);
    if (isSorted(arr, n)) {
    	printf("Array sorted correctly!\n");
	} else {
    	printf("Array sorting error!\n");
	}
#endif

    // 合併排序
    generateRandomArray(arr, n, seed);  // 重新生成隨機數據
    clock_gettime(CLOCK_MONOTONIC, &start);  // 開始計時
    mergeSort(arr, 0, n - 1); // 執行合併排序  
    clock_gettime(CLOCK_MONOTONIC, &end);  // 結束計時
    time_diff = diff(start, end);  // 計算時間差
    printf("4.Merge Sort time: %ld.%09ld seconds\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
    printf("Data after Merge Sort:\n");
    printArray(arr, n);
    if (isSorted(arr, n)) {
    	printf("Array sorted correctly!\n");
	} else {
    	printf("Array sorting error!\n");
	}
#endif

    // 堆排序
    generateRandomArray(arr, n, seed);  // 重新生成隨機數據
    clock_gettime(CLOCK_MONOTONIC, &start);  // 開始計時
    heapSort(arr, n); // 執行堆排序   
    clock_gettime(CLOCK_MONOTONIC, &end);  // 結束計時
    time_diff = diff(start, end);  // 計算時間差
    printf("5.Heap Sort time: %ld.%09ld seconds\n", time_diff.tv_sec, time_diff.tv_nsec);

#ifdef DEBUG
    printf("Data after Heap Sort:\n");
    printArray(arr, n);
    if (isSorted(arr, n)) {
    	printf("Array sorted correctly!\n");
	} else {
    	printf("Array sorting error!\n");
	}
#endif
	
    return 0;
}

