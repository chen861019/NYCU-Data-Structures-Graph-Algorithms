#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void insertionSort(int arr[], int n) {
	int i,j,acc;
	// 從第二個元素開始迭代
    for (i = 1; i < n; i++) {
        acc = arr[i];  // 將當前元素儲存為 acc
        j = i - 1;

        // 移動比 acc 大的元素到右邊，為 acc 騰出位置
        while (j >= 0 && arr[j] > acc) {
            arr[j + 1] = arr[j];  // 將元素向右移
            j--;
        }
        arr[j + 1] = acc;  // 將 key 插入正確的位置
    }
}

void selectionsort(int arr[], int n){
	int i,j,minIdx,acc;
	
	for (i = 0; i < n-1; i++) {
        minIdx = i;  // 假設當前元素是最小的

        // 尋找剩餘元素中的最小值
        for (j = i+1; j < n; j++) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j;  // 更新最小值的索引
            }
        }
        //進行交換 
    	acc= arr[i];
		arr[i] = arr[minIdx];
		arr[minIdx]=acc;
	}
}


void quickSort(int arr[], int low, int high) {
    if (low < high) {	
    	// 隨機選擇一個 pivot
        int randomIndex = low + rand() % (high - low);
        // 將 pivot 與最後一個元素交換
        int temp = arr[randomIndex];
        arr[randomIndex] = arr[high];
        arr[high] = temp;
    	
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
        temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        
        int pivotIdx = i + 1;  // pivot 的最終位置
        quickSort(arr, low, pivotIdx - 1);   // 遞歸排序左側
        quickSort(arr, pivotIdx + 1, high);  // 遞歸排序右側
    }
}
 
int main(){

	int arr[] = {12,19,20,1,8,14,9};
	int n = 7;
	//selectionsort(arr, n);
	//insertionSort(arr, n);
	quickSort(arr,0,n-1);
	// 輸出排序後的陣列
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

	
	return(0);
} 
