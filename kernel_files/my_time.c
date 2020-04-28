#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>


asmlinkage void sys_get_time(struct timespec* t)
{
    struct timespec temp;
    getnstimeofday(&temp);
    copy_to_user(t, &temp, sizeof(struct timespec));
    return ;
}







