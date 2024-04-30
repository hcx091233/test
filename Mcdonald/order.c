#include "order.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 获取订单信息
void get_order_info(int target) {
    // buf[0]用来存时间, buf[1]存食物/套餐名, 否则还需要调用一次memset
    char buf[2][MAXINPUTBUFLEN] = {0};
    // 将状态设为待制作
    odqueue[target].flag = 0;

    // 读入下单时间, 并将其转化为秒数
    scanf("%s", buf[0]);
    odqueue[target].order_time = totime(buf[0]);

    // 读入需要的食物/套餐名, 并将其转化为id号
    scanf("%s", buf[1]);
    // 先从单个食物开始搜素
    for(int j = 1; j <= single_food_num; ++j) {
        // 搜到了一样的就直接开一个空间并且存入, 结束
        if(strcmp(sfood[j].food_name, buf[1]) == 0) {
            odqueue[target].order_food_id = (int*)malloc(sizeof(int));
            odqueue[target].order_food_num = 1;
            odqueue[target].order_food_id[0] = j;
            return;
        }
    }
    // 单个食物没搜到就开始搜套餐
    for(int j = 1; j <= type_num; ++j) {
        // 搜到了一样的就直接开tfood[j].food_num对应大小的空间并且存入, 结束
        if(strcmp(tfood[j].type_name, buf[1]) == 0) {
            odqueue[target].order_food_id = (int*)malloc(sizeof(int) * tfood[j].food_num);
            for(int k = 0; k < tfood[j].food_num; ++k)
                odqueue[target].order_food_id[k] = tfood[j].food_id[k];
            odqueue[target].order_food_num = tfood[j].food_num;
            return;
        }
    }
}

// 释放申请的订单空间
void free_memory_order() {
    for(int i = 0; i < n; ++i) {
        if(odqueue[i].order_food_id) free(odqueue[i].order_food_id);
    }
}
