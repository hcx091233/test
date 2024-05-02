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
