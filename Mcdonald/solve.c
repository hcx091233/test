#include "menu.h"
#include "order.h"
#include "solve.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 将字符串时间转化为类似时间戳（从07:00:00开始）的秒数
int totime(const char *src) {
    int cur_h = 0, cur_m = 0, cur_s = 0;        // 小时, 分钟, 秒
    int cur_time;                               // 当下时间
    sscanf(src, "%d:%d:%d", &cur_h, &cur_m, &cur_s);
    cur_time = cur_h * 3600 + cur_m * 60 + cur_s;
    return cur_time;
}

// 将秒数转化为时间字符串并存入res
void tostringtime(char *dest, int stamp) {
    int h = stamp / 3600;
    stamp %= 3600;
    int m = stamp / 60;
    int s = stamp % 60;
    sprintf(dest, "%02d:%02d:%02d", h, m, s);
}

// 从头指针开始输出函数
void output() {
    // 如果front的flag等于2才说明要输出fail, 否则存储的就是时间
    while(odqueue[front].flag) {
        if(odqueue[front].flag == 2)
            puts("Fail");
        else {
            char buf[MAXTIMELEN] = {0};         // 格式化时间存储的位置
            tostringtime(buf, odqueue[front].flag);    // 格式化时间并输出
            puts(buf);
        }
        // front向后移一位, 完成数 + 1
        front = (front + 1) % MAXORDERNUM;
    }
}

// 处理订单函数
void solve() {
    // 状态
    int state = q0;
    // 输入订单总数
    scanf("%d", &n);
    get_order_info(rear);
    for(int i = start_time; completed_num < n; ++i) {
        // 有限状态自动机
        // 这里的每个make_food放在最后是因为该秒结束后才制作食物
        switch(state) {
            case q0:
                if(i == odqueue[rear].order_time) {
                    ++read_num;
                    ++making;
                    if(read_num < n) {
                        rear = (rear + 1) % MAXORDERNUM;
                        get_order_info(rear);
                    }
                }
                // 状态转移
                if(making > w1) state = fail;
                else state = accepting_order;
                // 这秒结束制作食物
                make_food();
                break;
            case accepting_order:
                if(i == odqueue[rear].order_time) {
                    ++read_num;
                    ++making;
                    if(read_num < n) {
                        rear = (rear + 1) % MAXORDERNUM;
                        get_order_info(rear);
                    }
                }
                // 如果有订单被完成就检查订单的front是否可以输出
                if(check_order(i)) output();
                // 如果制作中的数量大于w1或者22:00:00后的订单, state变为fail
                if(making > w1 || odqueue[front].order_time > end_time) state = fail;
                // 制作食物
                make_food();
                break;
            case fail:
                // fail状态下的订单直接将其flag标记为2处理
                // 这里要先处理fail, 因为就算making < w2也要下一秒才能重启系统
                if(i == odqueue[rear].order_time) {
                    ++read_num;
                    ++completed_num;        // 失败的订单视作完成
                    odqueue[rear].flag = 2;
                    if(read_num < n) {
                        rear = (rear + 1) % MAXORDERNUM;
                        get_order_info(rear);
                    }
                }
                // 如果订单时间大于22:00:00, 直接输出fail
                while(odqueue[front].order_time > end_time) {
                    front = (front + 1) % MAXORDERNUM;
                    ++completed_num;
                    puts("Fail");
                }
                if(check_order(i)) output();
                if(making < w2) state = accepting_order;                 // 如果制作中的数量小于w2, 改变state
                make_food();
                break;
            default:            // 正常来说不会出现default的情况, 但为了程序完整性, 此处添加了default
                break;
        }
    }

    free_memory_order();
}

// 每秒都在制作食物
void make_food() {
    for(int i = 1; i <= single_food_num; ++i) {
        // 如果食物库存等于最大容量就直接跳过该食物
        if(leftfood[i] == sfood[i].cap) {
            madefoodtime[i] = 0; // 制作时间归0
            continue;
        }
        // 如果时间到达制作时间, 说明又做出了一个食物
        if(++madefoodtime[i] % sfood[i].cost_time == 0) {
            // 如果该食物原库存为0并且cap > 0, left_food_num就++
            if(leftfood[i] == 0 && sfood[i].cap > 0) ++left_food_num;
            ++leftfood[i];       // 增加食物库存
            madefoodtime[i] = 0; // 制作时间从0开始重新计数
        }
    }
}

// 检测队列头指针处的订单是否制作完成, 完成返回1, 不能完成返回0
int check_order(const int time) {
    // flag作为返回值: 如果有订单做完了, flag就更新为1
    int end = (rear - front + 1 + MAXORDERNUM) % MAXORDERNUM;
    // 循环从front开始, 以end结尾
    for(int i = front, len = 0; len < end; i = (i + 1) % MAXORDERNUM, ++len) {
        // 如果当前时间还没到达当前订餐时间就直接返回0, 防止提前处理还没到时间的订单
        if(time < odqueue[i].order_time) return odqueue[front].flag ? 1 : 0;;
        // 如果此时一个库存都没有直接返回0
        if(left_food_num == 0) return odqueue[front].flag ? 1 : 0;;
        // 如果当前的odqueue[i].flag不为0说明该订单已被满足或fail无需处理
        if(odqueue[i].flag) continue;
        // cnt用于记录已经满足了当前订单多少个食物
        int cnt = 0;
        // 记录减少前的食物总数
        int init_num = odqueue[i].order_food_num;
        // 检查当下能分配的食物
        for(int j = 0; j < odqueue[i].order_food_num; ++j) {
            // 如果当下有有且仅有一个库存, 还要减少库存食物的总数
            // 注意这里不能else break, 因为要保证队列先入先出的性质, 有食物优先给先到的库存
            if(leftfood[odqueue[i].order_food_id[j]]) {
                if(--leftfood[odqueue[i].order_food_id[j]] == 0) --left_food_num;
                // 这里将order_food_id数组中的最后一个元素放到第j个位置上, 并且将订单需求的食物数--, 表示该订单这个食物已经有了
                odqueue[i].order_food_id[j] = odqueue[i].order_food_id[--odqueue[i].order_food_num];
                --j;      // 这里--j是为了继续检查第j个位置上的食物是否有库存
                ++cnt;    // cnt满足一个食物就++
            }
        }
        if(cnt == init_num) {
            odqueue[i].flag = time;     // 该订单标记为做完了
            ++completed_num;            // 订单完成数 + 1
            --making;                   // 减少一个制作中的订单
        }
    }
    return odqueue[front].flag ? 1 : 0;
}
