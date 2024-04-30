#include "solve.h"
#include "order.h"
#include "menu.h"

int main(void) {
    // 从文件中读取菜单信息
    getmenu();
    // 处理订单
    solve();

    return 0;
}

// 已解决问题
/*
    时间还未到达就处理订单     方案: check_order函数中增加time < od[i].order_time条件
    超出cap的食物还会继续制作  方案: 添加madefoodtime数组来表明时间
    making数据错误            方案: 设计循环从order_index开始往下寻找
    check_order函数在处理已做完的食物时od[i].order_food_num会改变导致无法判断该套餐是否制作完毕的条件   方案: 增加cnt变量
    fail的订单fail输出会在还在制作中的订单之前输出      方案: 给订单的flag添加2来表示fail, 而不是采用1表示双重含义（fail和制作完成）, 然后等到order_index走到该订单的时候在输出fail
    order_index++的时机问题   方案: while的循环条件改为flag == 1 || flag == INVALIDNUM（原来是只有flag, 这样会跳过flag == 2的情况）
*/
