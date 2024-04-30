#ifndef _SOLVE_H
#define _SOLVE_H

// 定义各种常量
#define MAXFIRSTLINE 20     // 文件第一行最大长度
#define MAXFOODNAMELEN 50   // 食物名字最长数
#define MAXTYPENAME 50      // 套餐名称最长数
#define MAXINPUTBUFLEN 50   // 读入缓冲区长度, 用于读入时间和食物名
#define MAXTIMELEN 10       // 格式化时间最长缓冲区

#define start_time 7 * 3600 // 开业时间07:00:00对应的秒数
#define end_time 22 * 3600  // 关门时间22:00:00对应的秒数

// 自动机状态, 这里不设置end状态是因为可以用循环控制结束, 并且结束状态也不需要什么操作
#define q0 0                // 起始状态
#define accepting_order 1   // 可以接受订单的状态
#define fail 2              // 订单已满的状态

// 定义全局变量
int single_food_num, type_num;  // 食物总数, 套餐总数
int n;                          // 订单数
int w1, w2;                     // 订单大于w1 state就变为fail; fail状态下, 订单小于w2 state就恢复到accepting_order

int totime(const char *str);
void tostringtime(char *res, int stamp);
void solve();
void make_food();
int check_order(const int time);
void output();

#endif
