/**
 * 一个简单的模拟器，尝试决定如下问题:
 *  如果每对夫妻都尝试生下儿子，如果生下女儿则继续尝试生女儿
 *  这是否会导致男女比例失调？
 *
 * 具体设定如下：
 * ~ 每个人到20岁开始寻找伴侣，到60生命结束
 * ~ 新生儿男女比例都是1/2
 * ~ 生下女儿则两年后继续尝试，生下儿子则停止尝试
 * ~ 如果生下五个女儿，也停止尝试
 * ~ 假定理想条件，年纪大优先找到另一半，但45岁后不再尝试
 *
 * 如此模拟200年，输出:
 * ~ 终身未娶/未嫁和成功完婚的数量
 * ~ 在世的男女比例
 * ~ 在世未婚的男女比例
 *
 * hard code :
 * 假定开始时间是2014年
 * 开始时刚出生的男女人数相同
 */

#include <stdio.h>
#include "emul_def.h"
#define START_YEAR 2014
#define START_PEOPLE_SIZE 20000


int main(int argc,char * argv[])
{
    int i;
    init_emul(START_YEAR,START_PEOPLE_SIZE/2,START_PEOPLE_SIZE/2);
    show_stat();
    for(i = 0 ; i < 2000 ; i ++ )
    {
        next_year();
        //if(i % 5 == 4)
        {
            show_stat();
        }
    }

    return 0;
}
