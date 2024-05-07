#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 将套餐和食物信息全部读入
void getmenu() {
    // 读取单个食品的数量, 套餐数量, 存入single_food_num, type_num两个变量中
    char buf[MAXFIRSTLINE] = {0}; // 读入文件的第一行的缓冲区
    FILE *fp = fopen("dict.dic", "r");
    if(fp == NULL) {
        printf("菜单读取失败!\n");
        exit(0);
    }
    fgets(buf, MAXFIRSTLINE - 1, fp);
    sscanf(buf, "%d %d", &single_food_num, &type_num);

    // 将每个单独的食物读入到single_food数组中, 下标从1开始
    for(int i = 1; i <= single_food_num; ++i) {
        fscanf(fp, "%s", &sfood[i].food_name);
    }

    // 开始读入每个菜单套餐的制作时间和最大库存
    for(int i = 1; i <= single_food_num; ++i)
        fscanf(fp, "%d", &sfood[i].cost_time);
    for(int i = 1; i <= single_food_num; ++i)
        fscanf(fp, "%d", &sfood[i].cap);

    // 读入w1, w2
    fscanf(fp, "%d %d\n", &w1, &w2);

    // 开始读入套餐
    char namebuf[MAXFOODNAMELEN] = {0}; // 套餐名缓冲区
    char ch;
    for(int i = 1; i <= type_num; ++i) {
        // 开始读入
        ch = fgetc(fp);
        while(ch == ' ') ch = fgetc(fp);    // 这里是防止套餐的开头会包含空格导致bug
        int index = 0;                      // 此处的index作为套餐名的下标
        while(ch != ' ') {
            tfood[i].type_name[index++] = ch;
            ch = fgetc(fp);
        }
        tfood[i].type_name[index] = '\0';
        index = 0;                  // 此处的index作为读入的食物名的下标
        ch = fgetc(fp);
        int foodidindex = 0;        // 每个套餐包含的单个食物的id（下标）
        while(1) {
            if(ch == ' ') {         // 读到空格就开始处理一个套餐包含的食物id
                ch = fgetc(fp);     // 这里先fgetc是为了防止下面出现死循环
                if(!index) continue;// 如果当前index为空说明有多个空格, 直接跳过
                namebuf[index] = '\0';
                getfoodid(namebuf, i, foodidindex++);
                index = 0;          // index置0, 刷新namebuf
                continue;           // 继续读入字符
            }
            if(ch == '\n') {        // 读到'\n'就处理这个食物并退出循环
                namebuf[index] = '\0';
                getfoodid(namebuf, i, foodidindex++);
                break;              // 该套餐结束, 退出循环
            }
            namebuf[index++] = ch;
            ch = fgetc(fp);
        }
    }
    // 文件读入结束, 关闭文件
    fclose(fp);

    // /*
    // 以下是用于测试读入文件的代码, 输出id对应的食物名, 以及套餐包含的食物名及其id
    for(int i = 1; i <= single_food_num; ++i) {
        printf("%d: %s\n", i, sfood[i].food_name);
    }

    for(int i = 1; i <= type_num; ++i) {
        printf("%d: %s: ", i, tfood[i].type_name);
        for(int j = 0; tfood[i].food_id[j] <= single_food_num && tfood[i].food_id[j] >= 1; ++j) {
            printf("%d: %s ", tfood[i].food_id[j], sfood[tfood[i].food_id[j]].food_name);
        }
        putchar('\n');
    }
    // */
}

// 获取食物id
void getfoodid(const char *namebuf, const int id, const int foodidindex) {
    for(int j = 1; j <= single_food_num; ++j) {
        if(!strcmp(sfood[j].food_name, namebuf)) {
            tfood[id].food_id[foodidindex] = j;
            ++tfood[id].food_num;
            break;
        }
    }
}
