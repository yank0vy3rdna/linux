#include <linux/kernel.h>
#include <linux/filter.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#define num_bpf 549
#define num_dm 550
long bpf_syscall(void* p, int cpu_num)
{
    return syscall(num_bpf, p, cpu_num);
}
long dm_syscall(void* p, const char * path, int target_num, const char * log_type, char * region_size)
{
    return syscall(num_dm, p, path, target_num, log_type, region_size);
}
int main(int argc, char *argv[])

{
    for (int i = 0; i < 1000; i++){
    unsigned char *bpf_data = malloc(1024);
    void* dm_data = malloc(1024);
    bpf_syscall(bpf_data, i);
    int bpf_flags = (bpf_data[0]) << 24 | (bpf_data[1]) << 16 | (bpf_data[2]) << 8 | (bpf_data[3]);
    printf("%d\n", bpf_flags);
    }
/*
    printf("%s\n", strerror(errno));
    printf("%d\n",dm_syscall(dm_data, "/dev/sda2/", 0, "core", "1024"));    
    printf("%s\n", strerror(errno));*/
    return 0;
}
