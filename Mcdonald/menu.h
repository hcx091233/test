#ifndef _MENU_H
#define _MENU_H

#include "solve.h"

#define MAXFOODNUM 101      // 总食物最大数
#define MAXTYPENUM 101      // 总套餐最大数
#define MAXFOOD 6           // 一个套餐最多包含的食物数

typedef struct single_food {// 食物结构体
    char food_name[MAXFOODNAMELEN];        // 食物名称
    int cap;                // 食物最大容量
    int cost_time;          // 制作消耗的时间
}sf;
sf sfood[MAXFOODNUM];       // 食物数组

typedef struct type_food {  // 套餐结构体
    char type_name[MAXTYPENAME];        // 套餐名
    int food_id[MAXFOOD];               // 套餐包含的食物id（下标）
    int food_num;                       // 套餐包含的食物数
}tf;
tf tfood[MAXTYPENUM];       // 套餐数组

int leftfood[MAXFOODNUM];       // 每种id对应的食物剩余数
int madefoodtime[MAXFOODNUM];   // 对应id的食物已经制作了多长时间

void getmenu();
void getfoodid(const char *namebuf, const int id, const int foodidindex);

#endif
