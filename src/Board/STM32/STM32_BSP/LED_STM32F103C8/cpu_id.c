#include <includes.h>

struct cpu_id {
    u32 id[3];
};

struct cpu_id GetLockCode(void)
{
    struct cpu_id CpuID;

    CpuID.id[0]    =*(vu32*)(0x1ffff7e8);
    CpuID.id[1]    =*(vu32*)(0x1ffff7ec);
    CpuID.id[2]    =*(vu32*)(0x1ffff7f0);
    
    return  CpuID;
}

