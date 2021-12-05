#include <linux/kernel.h>
#include <linux/syscalls.h>
SYSCALL_DEFINE0(hello)
{
        printk("Hello world\n");
        return 0;
}
