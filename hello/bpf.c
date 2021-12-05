#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/filter.h>
#include <asm/uaccess.h>
#include <linux/percpu-defs.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE2(bpf_redirect_info, void*, p, u32, cpu){
	struct bpf_redirect_info *r = SHIFT_PERCPU_PTR(&bpf_redirect_info, per_cpu_offset(cpu));
	copy_to_user(p, r, sizeof(struct bpf_redirect_info));
	return 0;
}
