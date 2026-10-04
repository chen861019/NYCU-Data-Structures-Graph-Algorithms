//Programming Assignment I 
//Date Data Type
//author : 陳沛安
//date: 2024/09/11
#include <stdio.h>
#include <stdlib.h>

// 定義日期結構
typedef struct {
    int year;  //設為年 
    int month; //設為月 
    int day;   //設為日 
} date;

// 判斷是否為閏年
int leap_year(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);//每四年閏年，但百年不閏，每四百年為閏 
}

// 計算每個月的天數
int month_days(int month, int year) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; //設定每個月的天數 
    return (month == 2 && leap_year(year)) ? 29 : days[month - 1]; //如果為閏年&月為2，天數為29天 
}

// 返回月份的英文名稱
const char* date_month_format(date Date) {
    const char *months[] = {"January", "February", "March", "April", "May", "June",	//設定每個月的英文名稱
                            "July", "August", "September", "October", "November", "December"};
    return months[Date.month - 1];
}

// Zeller's Congruence 算法，用於計算日期是星期幾
const char* Zellercongruence(date Date) {     // 定義一個函數 Zellercongruence，傳入的參數是 date 類型的變數 Date
    if (Date.month == 1 || Date.month == 2) { // 如果月份是1月或2月
        Date.month += 12;                     // 1月和2月在算法中被視為前一年的13月和14月，所以月份加12
        Date.year--;                          // 同時，年份要減去1
    }
    int y = Date.year % 100;                  // 將年份的後兩位取出，這部分稱為「年份」中的 y（year of century）
    int c = Date.year / 100;                  // 將年份的前兩位取出，這部分稱為「世紀」中的 c（century）
    int day_of_week = (Date.day +             // Zeller公式的核心計算，結果是計算出某天的星期幾
        13 * (Date.month + 1) / 5 +           // 計算月份的修正值
        y + y / 4 +                           // 年份修正值，加上年份除以4的結果，用來處理閏年
        c / 4 +                               // 世紀修正值，加上世紀數除以4的結果
        5 * c) % 7;                           // 世紀修正值的另一部分，世紀數乘以5，再加總整個式子，最後對7取模以獲得星期
    const char *days[] = {"Saturday", "Sunday", "Monday", 
						  "Tuesday", "Wednesday", "Thursday", "Friday"};
    return days[day_of_week];                 // 返回計算出的星期幾
}


unsigned int total_days(date d) {        // 定義一個函數 total_days，傳入一個 date 類型的變數 d，返回從公元元年到 d 的總天數
    unsigned int days = d.day;           // 初始值是日期中的「日」，即當前月份中的天數
    int year, month;                     // 定義年份和月份的變數
    for (year = 1; year < d.year; year++) { // 從公元元年開始，循環到指定的年份前一年
        days += leap_year(year) ? 366 : 365; // 如果是閏年，增加366天，否則增加365天
    }
    for (month = 1; month < d.month; month++) { // 從1月開始，循環到指定的月份前一個月份
        days += month_days(month, d.year);      // 累加每個月份的天數，月份天數考慮當年的閏年情況
    }
    return days;                              // 返回計算的總天數
}


// 計算兩個日期之間的差異，總是返回正值
int date_sub(date Date1, date Date2) {
    int diff = total_days(Date2) - total_days(Date1);
    return (diff >= 0) ? diff : -diff;  // 確保返回正值
}

// 加上指定天數
date date_add(date Date, int days_to_add) {  // 定義一個函數 date_add，傳入一個 date 類型的 Date 和一個整數類型的天數 days_to_add
    while (days_to_add != 0) {               // 當還有天數需要加減時，進行迴圈
        if (days_to_add > 0) {               // 如果天數是正的，表示要向未來加天數
            int days_in_current_month = month_days(Date.month, Date.year); // 取得當前月份的天數
            if (Date.day + days_to_add <= days_in_current_month) {         // 檢查當前日期加上天數是否還在當月範圍內
                Date.day += days_to_add;      // 如果還在當月範圍內，直接加上天數
                days_to_add = 0;              // 完成加法，將 days_to_add 設為 0
            } else {                          // 如果加上天數後超出當月天數
                days_to_add -= (days_in_current_month - Date.day + 1); // 減去當月剩下的天數（含當天）
                Date.day = 1;                 // 將日期設為下個月的第一天
                Date.month++;                 // 月份加一
                if (Date.month > 12) {        // 如果月份超過12月，進行進位處理
                    Date.month = 1;           // 重置為1月
                    Date.year++;              // 年份加一
                }
            }
        } else {                              // 如果天數是負的，表示要向過去減天數
            if (Date.day + days_to_add > 0) { // 檢查當前日期減去天數後是否仍大於0（即還在當月內）
                Date.day += days_to_add;      // 如果仍在當月，直接減去天數
                days_to_add = 0;              // 完成減法，將 days_to_add 設為 0
            } else {                          // 如果減去天數後超出當月的天數範圍
                days_to_add += Date.day;      // 計算還剩多少天數需要減去（先把當月剩下的日子扣掉）
                Date.month--;                 // 月份減一
                if (Date.month < 1) {         // 如果月份減到1月之前，進行退位處理
                    Date.month = 12;          // 重置為12月
                    Date.year--;              // 年份減一
                }
                Date.day = month_days(Date.month, Date.year); // 將日期設為前一個月的最後一天
            }
        }
    }
    return Date;                              // 返回最終計算出的日期
}


// 輸入驗證函數
int validate_input(const char *input) {      // 定義一個函數 validate_input，傳入參數為字串 input，返回值為整數（布林值）
    int year, month, day;                    // 定義變數 year、month 和 day，用來存放解析出的年、月、日
    return sscanf(input, "%d/%d/%d", &year, &month, &day) == 3 // 使用 sscanf 解析 input 字串為年/月/日格式，若成功解析到3個整數，則返回值為3
           && year > 0                       // 檢查年份是否大於0（有效年份應大於0）
           && month >= 1 && month <= 12      // 檢查月份是否在1到12之間
           && day >= 1 && day <= month_days(month, year); // 檢查日期是否在當前月份的有效範圍內，這裡依賴 month_days() 函數
}


// 手動尋找 + 或 - 符號位置
char find_operator(const char* input) {  // 定義函數 find_operator，接收字串 input，返回一個字元
    int i;
    for (i = 0; input[i] != '\0'; i++) { // 迴圈遍歷字串，直到遇到字串結束符 '\0'
        if (input[i] == '+' || input[i] == '-') {  // 檢查當前字元是否為 '+' 或 '-'
            return input[i];            // 如果找到，返回該符號
        }
    }
    return '\0';                        // 如果字串未找到運算符，返回空字元 '\0'
}

int main() {
    char input[40];         // 用來存放使用者輸入的字串
    date Date, Date2;        // 日期物件
    char operator;           // 操作符 (+ 或 -)
    int number;              // 加數（若有）
    int days;                // 用來存放日期之間的差異

	// 提示使用者輸入
	printf("Enter a date operation in one of the following formats:\n");
	printf("1. yyyy/mm/dd (e.g., 2019/9/20): to get the day of the week for a specific date.\n");
	printf("2. yyyy/mm/dd - YYYY/MM/DD (e.g., 2018/9/20 - 2019/9/20): to calculate the number of days between two dates.\n");
	printf("3. yyyy/mm/dd + x (e.g., 2019/9/20 + 365 or 2019/9/20 +-65): to add or subtract a\ncertain number of days to/from a specific date.\n");
	printf("Enter a date operation: ");
    // 讀取整行輸入
    if (fgets(input, sizeof(input), stdin) != NULL) {  // 從標準輸入讀取字串，並將其存入 input 中。如果讀取成功，繼續執行
    // 輸入驗證
    if (!validate_input(input)) {                  // 調用 validate_input() 函數驗證輸入的格式是否正確
        printf("Invalid input format.\n");         // 如果格式不正確，打印錯誤信息
        return 1;                                  // 返回 1，結束程序
    }

    // 手動尋找 + 或 - 符號位置
    operator = find_operator(input);               // 使用 find_operator() 函數尋找輸入字串中是否存在 '+' 或 '-' 運算符

    // 如果找到操作符
    if (operator != '\0') {                        // 如果找到 '+' 或 '-'，繼續執行以下操作
        if (operator == '+') {                     // 如果運算符是 '+'
            // 解析日期和數字
            sscanf(input, "%d/%d/%d %*c %d", &Date.year, &Date.month, &Date.day, &number); // 使用 sscanf 解析日期和要加上的天數
            date af_Date = date_add(Date, number); // 調用 date_add() 函數，將 days_to_add 添加到日期 Date 中，返回新日期 af_Date
            const char *relation = (number < 0) ? "before" : "after"; // 確定數字是正數還是負數，決定使用 "before" 還是 "after"
            printf("%d days %s %s %02d, %04d is %s %02d, %04d\n",
                   abs(number), relation, date_month_format(Date), Date.day, Date.year,
                   date_month_format(af_Date), af_Date.day, af_Date.year); // 輸出日期加法結果
        } else if (operator == '-') {              // 如果運算符是 '-'
            // 解析兩個日期
            sscanf(input, "%d/%d/%d %*c %d/%d/%d", &Date.year, &Date.month, &Date.day,
                   &Date2.year, &Date2.month, &Date2.day); // 使用 sscanf 解析兩個日期
            days = date_sub(Date, Date2);          // 調用 date_sub() 函數，計算兩個日期之間的天數
            printf("%d days from %s %02d, %04d to %s %02d, %04d.\n",
                   days, date_month_format(Date2), Date2.day, Date2.year,
                   date_month_format(Date), Date.day, Date.year); // 輸出日期減法結果
        }
    } else {                                       // 如果沒有找到 '+' 或 '-'，則僅處理單一日期
        // 沒有操作符，解析單一日期並輸出結果
        sscanf(input, "%d/%d/%d", &Date.year, &Date.month, &Date.day); // 使用 sscanf 解析單一日期
        printf("%s %02d, %04d is %s.\n",
               date_month_format(Date), Date.day, Date.year, Zellercongruence(Date)); // 使用 Zellercongruence() 計算該日期是星期幾，並輸出結果
    }
} else {
    printf("Failed to read input.\n");             // 如果 fgets 讀取失敗，打印錯誤信息
}

return 0;                                          // 結束程序，返回 0

}

