#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
#include <linux/string.h>


asmlinkage void sys_print_info(int p, char* s1, char* s2)
{
    char t1[100], t2[100];

    copy_from_user(t1, s1, 100);
    copy_from_user(t2, s2, 100);

    printk("[Project1] %d %s %s\n", p, t1, t2);
    
    return ;
}
