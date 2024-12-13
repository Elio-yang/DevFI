#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/pgtable.h>
#include <linux/kernel.h>
#include <linux/types.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yang Yang");

// use page table walk to translate virtual address to physical address
static int __init phys_addr_module_init(void)
{
    volatile uint64_t test_value = 0xBADDBADDBADDBADD;
    struct mm_struct *mm = current->mm;                // The memory descriptor of the current process.
    volatile uint64_t vaddr = (uint64_t)(&test_value); // The virtual address to be translated

    printk(KERN_INFO "PID: %d\n", current->pid);
    printk(KERN_INFO "Virtual address: %lx\n", vaddr);

    pgd_t *pgd = pgd_offset(mm, vaddr);
    if (!pgd_none(*pgd) && !pgd_bad(*pgd)){
        printk(KERN_INFO "PGD: %lx\n", pgd_val(*pgd));
        p4d_t *p4d = p4d_offset(pgd, vaddr);
        if (!p4d_none(*p4d) && !p4d_bad(*p4d)){
            printk(KERN_INFO "P4D: %lx\n", p4d_val(*p4d));
            pud_t *pud = pud_offset(p4d, vaddr);

            if (!pud_none(*pud) && !pud_bad(*pud)){
                printk(KERN_INFO "PUD: %lx\n", pud_val(*pud));
                pmd_t *pmd = pmd_offset(pud, vaddr);

                if (!pmd_none(*pmd) && !pmd_bad(*pmd)){
                    printk(KERN_INFO "PMD: %lx\n", pmd_val(*pmd));
                    pte_t *pte = pte_offset_map(pmd, vaddr);

                    if (!pte_none(*pte)){
                        unsigned long phys = pte_val(*pte) & PAGE_MASK;
                        printk(KERN_INFO "PTE: %lx\n", pte_val(*pte));
                        printk(KERN_INFO "Physical address: %lx\n", phys);
                    }else{
                        printk(KERN_INFO "PTE ERR\n");
                    }
                    pte_unmap(pte);

                }else{
                    printk(KERN_INFO "PMD ERR\n");
                }
            }else{
                printk(KERN_INFO "PUD ERR\n");
            }
        }else{
            printk(KERN_INFO "P4D ERR\n");
        }
    }else{
        printk(KERN_INFO "PGD ERR\n");
    }
    return 0;
}

static void __exit phys_addr_module_exit(void)
{
    printk(KERN_INFO "Phys Addr Module Exiting\n");
}

module_init(phys_addr_module_init);
module_exit(phys_addr_module_exit);
