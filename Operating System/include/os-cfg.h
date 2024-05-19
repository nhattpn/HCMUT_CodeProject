#ifndef OSCFG_H
#define OSCFG_H

//MLQ_SCHED currently not working
//fix by yourself at read_config in os.c
#define MLQ_SCHED 1 //turn on when input process have 3 props or have input priority (ex: 1 s0 3)
#define MAX_PRIO 140

#define CPU_TLB
#define CPUTLB_FIXED_TLBSZ //turn on when doesn't have input tlbsz (ex: 40000)
#define MM_PAGING
#define MM_FIXED_MEMSZ // turn on when doesn't have input ram and swap space (ex: 2048 16777216 0 0 0)
#define VMDBG 1
#define MMDBG 1
#define IODUMP 1
#define PAGETBL_DUMP 1

#endif
