#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bw0x00");
MODULE_DESCRIPTION("Dump Syscall Table Addr on x86_64");
MODULE_VERSION("0.01");

void * getSyscallHandler(void);
void * getSyscallTable(void);
const int IA32_LSTAR = 0xc0000082;

void * getSyscallHandler(){
        int low,high;
        asm("rdmsr" : "=a" (low), "=d" (high) : "c" (IA32_LSTAR));
        return (void*)(((long)high<<32)|low);
}

// Parse syscall handler and extract address of syscall table
void * getSyscallTable(){
        unsigned char* p = 0;
        void * system_call = 0;
        int i =0;
        system_call = getSyscallHandler();
        for(p = system_call,i=0;i<500;i++){
                if(p[0]==0xff && p[1]==0x14 && p[2]==0xc5)
                        return (void*)(0xffffffff00000000 | *((unsigned int*)(p+3)));
                p++;
        }
        return 0;
}

static int __init module_initialize(void)
{
    printk(KERN_INFO "Dump Syscall Table Addr...\n");
    printk(KERN_INFO "Handler Addr: %p \n", getSyscallHandler());
    printk(KERN_INFO "Table Addr: %p \n", getSyscallTable());
}

static void __exit module_terminate(void)
{
    printk(KERN_INFO "EXIT\n");
}

module_init(module_initialize);
module_exit(module_terminate);


