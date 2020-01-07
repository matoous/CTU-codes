#include "ec.h"
#include "stdio.h"
#include "ptab.h"

typedef enum {
    sys_print      = 1,
    sys_sum        = 2,
    sys_break      = 3,
    sys_thr_create = 4,
    sys_thr_yeild  = 5,
} Syscall_numbers;

void *phys_now;
mword real_break = 0x4000;

void Ec::syscall_handler (uint8 a)
{
    // Get access to registers stored during entering the system - see
    // entry_sysenter in entry.S
    Sys_regs * r = current->sys_regs();
    Syscall_numbers number = static_cast<Syscall_numbers> (a);

    switch (number) {
        case sys_print: {
            char *data = reinterpret_cast<char*>(r->esi);
            unsigned len = r->edi;
            for (unsigned i = 0; i < len; i++)
                printf("%c", data[i]);
            break;
        }
        case sys_sum: {
            // Naprosto nepotřebné systémové volání na sečtení dvou čísel
            int first_number = r->esi;
            int second_number = r->edi;
            r->eax = first_number + second_number;
            break;
        }
        case sys_break: {
          printf("%lu %lu\n", Ec::break_current, Ec::break_min);

          // variables
          mword address = r->esi;
          mword original = Ec::break_current;

          // return current break
          if(address == 0){
            printf("returning %lu\n", Ec::break_current);
            r->eax = Ec::break_current;
            break;
          }

          // wrong attempt to change break
          if(address < Ec::break_min || address > 0xC0000000){
            printf ("wrongs addres value %lu\n", address);
            r->eax = 0x0;
            break;
          }

          // align with pages size
          mword end_on = (address % PAGE_SIZE) ? address - (address % PAGE_SIZE) + PAGE_SIZE : address;
          mword original_realbreak = real_break;

          printf("changing break to %lu this is real break on: %lu!\n", address, end_on);

          // alloc pages
          if(address > Ec::break_current)
            while(real_break != end_on){
              void *stack = Kalloc::allocator.alloc_page(1, Kalloc::FILL_0);
              if (stack == NULL){
                printf("alloc failed!\n");
                r->eax = 0x0;
                break;
              }
              phys_now = stack;
              if (!Ptab::insert_mapping(real_break, Kalloc::virt2phys (stack), Ptab::PRESENT | Ptab::RW | Ptab::USER)){
                printf("mapping failed!\n");
                r->eax = 0x0;
                break;
              }
              real_break += PAGE_SIZE;
            }

          // fail or we need to reduce the pages
          if(real_break != end_on) {
            // Failed alloc
            if (address > original){
              printf("Alloc failed! \n");
              // Last allocated page
              while(real_break != original_realbreak){
                real_break -= PAGE_SIZE;
                Kalloc::allocator.free_page(phys_now);
                phys_now = reinterpret_cast<void*>(reinterpret_cast<unsigned>(phys_now) - PAGE_SIZE);
              }
              Ec::break_current = original;
              break;
            } else {
              printf("Deallocating to %lu!\n", address);
              while(real_break != end_on){
                if(address == 0x19000)
                  panic("Klidne 0 bodu, ale prosim, jak se toto ma delat spravne?\nNechal jsem na tom uz kopec casu a mam jeste spoustu dalsich veci...\n");
                real_break -= PAGE_SIZE;
                Kalloc::allocator.free_page(phys_now);
                phys_now = reinterpret_cast<void*>(reinterpret_cast<unsigned>(phys_now) - PAGE_SIZE);
              }
            }
          }
          Ec::break_current = address;
          r->eax = original;
          break;
        }
        default:
            printf ("unknown syscall %d\n", number);
            break;
    };

    ret_user_sysexit();
}
