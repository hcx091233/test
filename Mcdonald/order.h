#ifndef _ORDER_H
#define _ORDER_H

#include "solve.h"

#define MAXORDERNUM 7000    // 订单队列最大空间

typedef struct o {          // 订单结构体
    int order_time;         // 存储订单时间的数组
    int *order_food_id;     // 存储订单的食物id, 这一要开数组因为可能是套餐需要存储许多食物类型
    int order_food_num;     // 订单要求的食物总数
    int flag;               // 用于表示订单状态: 0表示还未制作成功 2表示fail 其余数值表示完成时间
}order;
order odqueue[MAXORDERNUM];     // 订单数组

int read_num;                   // 已经读入的订单数
int completed_num;              // 已经制作完成的订单数
int making;                     // 已下单但未完成的订单的数量
int left_food_num;              // 剩余食物的数量, 即leftfood数组中不为0的个数, 这个主要是用于减少不必要的循环: 如果目前没有库存食物, 说明不用遍历一遍订单来查看是否有订单可以满足
int front, rear;                // 队列的头指针和尾指针

void get_order_info(int target);
void free_memory_order();

#endif
