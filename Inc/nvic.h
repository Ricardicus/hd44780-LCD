#ifndef RICKARD_NVIC_UTILS_H
#define RICKARD_NVIC_UTILS_H

#define SCS_BASE_ADDR              0xE000E000UL

#define ENABLE_IRQ()               do {asm volatile ("cpsie i" : : : "memory");} while(0);

#define NVIC_ISER_ADDR(x)          ((uint32_t*)(SCS_BASE_ADDR + 0x0100UL + ((x>>5UL)<<2UL)))
#define NVIC_ICER_ADDR(x)          ((uint32_t*)(SCS_BASE_ADDR + 0x0180UL + ((x>>5UL)<<2UL)))
#define NVIC_ISPR_ADDR(x)          ((uint32_t*)(SCS_BASE_ADDR + 0x0200UL + ((x>>5UL)<<2UL)))
#define NVIC_ICPR_ADDR(x)          ((uint32_t*)(SCS_BASE_ADDR + 0x0280UL + ((x>>5UL)<<2UL)))
#define NVIC_IABR_ADDR(x)          ((uint32_t*)(SCS_BASE_ADDR + 0x0300UL + ((x>>5UL)<<2UL)))
#define NVIC_BIT(x)                BIT(x & 0x1FUL)

#define NVIC_ENABLE(x) do {\
uint32_t *addr = NVIC_ISER_ADDR(x); *addr = (uint32_t) 1UL << ((uint32_t)x & 0x1FUL);\
} while ( 0 );

#define NVIC_SET_PRIO(c,x)          do {\
char prio = (char)x;\
int32_t code = (int32_t) c;\
int32_t *ispx = (int32_t*) + SCS_BASE_ADDR + 0x400 + ((code >> 2)<<2); \
*ispx = (*ispx & ~(0xFFUL<<(8*(code & 0x03UL)))) + (prio<<(8*(code & 0x03UL)));\
} while (0);

#define NVIC_TRIGGER_INT(n) do {\
int32_t *stir = (int32_t*) SCS_BASE_ADDR + 0xE00; *stir = n & 0x000000FF; } while(0);

#endif