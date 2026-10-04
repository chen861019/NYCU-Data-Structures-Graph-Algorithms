#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define QUEUE_SIZE 10000

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point point;
    Point path[QUEUE_SIZE];
    int path_length;
} QueueNode;

int maze[MAX][MAX];
int visited[MAX][MAX];
int m, n;
Point start, end;
Point directions[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

// 初始化佇列
QueueNode queue[QUEUE_SIZE];
int front = 0, rear = 0;

void enqueue(QueueNode node) {
    if (rear < QUEUE_SIZE) {
        queue[rear++] = node;
    }
}

QueueNode dequeue() {
    return queue[front++];
}

int is_empty() {
    return front == rear;
}

// 讀取迷宮輸入
void parse_maze(char input_data[][50], int lines) {
    for (int i = 0; i < lines; i++) {
        int row;
        char line[50];
        strcpy(line, input_data[i]);
        char *token = strtok(line, " ");
        row = atoi(token) - 1;
        while ((token = strtok(NULL, " ")) != NULL) {
            int col = atoi(token) - 1;
            token = strtok(NULL, " ");
            if (strcmp(token, "x") == 0) {
                maze[row][col] = -1;
            } else if (strcmp(token, "s") == 0) {
                maze[row][col] = 0;
                start.x = row;
                start.y = col;
            } else if (strcmp(token, "t") == 0) {
                maze[row][col] = 0;
                end.x = row;
                end.y = col;
            } else {
                maze[row][col] = 0;
            }
        }
    }
}

int bfs() {
    // 初始化佇列
    QueueNode start_node;
    start_node.point = start;
    start_node.path[0] = start;
    start_node.path_length = 1;

    enqueue(start_node);
    visited[start.x][start.y] = 1;

    while (!is_empty()) {
        QueueNode current = dequeue();

        // 如果到達終點
        if (current.point.x == end.x && current.point.y == end.y) {
            printf("Shortest path: ");
            for (int i = 0; i < current.path_length; i++) {
                printf("(%d, %d) ", current.path[i].x + 1, current.path[i].y + 1);
            }
            printf("\n");
            return 1;
        }

        // 探索四個方向
        for (int i = 0; i < 4; i++) {
            int nx = current.point.x + directions[i].x;
            int ny = current.point.y + directions[i].y;

            if (nx >= 0 && nx < m && ny >= 0 && ny < n && !visited[nx][ny] && maze[nx][ny] != -1) {
                visited[nx][ny] = 1;
                QueueNode next;
                next.point.x = nx;
                next.point.y = ny;
                memcpy(next.path, current.path, sizeof(Point) * current.path_length);
                next.path[current.path_length] = next.point;
                next.path_length = current.path_length + 1;
                enqueue(next);
            }
        }
    }

    return 0;
}

void display_maze() {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (maze[i][j] == -1) {
                printf("x ");
            } else if (i == start.x && j == start.y) {
                printf("s ");
            } else if (i == end.x && j == end.y) {
                printf("t ");
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

int main() {
    // 迷宮輸入數據，可以替換成實際數據
    char input_data[][50] = {
        "2 x 0",
        "2 s 4 x 0",
        "3 x 0",
        "2 x 4 x 5 t 0"
    };

    m = 4; // 迷宮行數
    n = 5; // 迷宮列數

    parse_maze(input_data, 4);

    printf("Maze layout:\n");
    display_maze();

    if (bfs()) {
        printf("Path found!\n");
    } else {
        printf("No path found.\n");
    }

    return 0;
}

