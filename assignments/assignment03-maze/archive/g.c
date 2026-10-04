#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 隨機生成迷宮，並控制障礙物的數量
void generate_maze(int rows, int cols, float obstacle_probability) {
    char maze[rows][cols];
    
    // 初始化隨機數生成器
    srand(time(NULL));
    
    // 根據概率隨機填充迷宮
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if ((float)rand() / RAND_MAX < obstacle_probability) {
                maze[i][j] = 'x';  // 設置為障礙物
            } else {
                maze[i][j] = '0';  // 設置為空白
            }
        }
    }
    
    // 隨機設置起點和終點
    int start_row = rand() % rows;
    int start_col = rand() % cols;
    int target_row = rand() % rows;
    int target_col = rand() % cols;
    
    // 保證起點和終點不同
    while (start_row == target_row && start_col == target_col) {
        target_row = rand() % rows;
        target_col = rand() % cols;
    }
    
    maze[start_row][start_col] = 's'; // 設置起點
    maze[target_row][target_col] = 't'; // 設置終點
    
    // 打印迷宮
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%c ", maze[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // 設置障礙物的生成概率
    float obstacle_probability = 0.3;  // 30%的概率生成障礙物
    
    // 生成並打印 3x3, 5x5 和 10x10 迷宮
    printf("3x3 迷宮:\n");
    generate_maze(3, 3, obstacle_probability);
    
    printf("\n5x5 迷宮:\n");
    generate_maze(5, 5, obstacle_probability);
    
    printf("\n10x10 迷宮:\n");
    generate_maze(10, 10, obstacle_probability);
    
    return 0;
}

