#ifndef RICKARD_NVIC_UTILS_H
#define RICKARD_NVIC_UTILS_H

#define NVIC_ISER_BASE	           0xE000E100
#define NVIC_ISER_ADDR(x)          ((int32_t*)(NVIC_ISER_BASE + 0x100 + ((x>>5)<<2)))
#define NVIC_ICER_ADDR(x)          ((int32_t*)(NVIC_ISER_BASE + 0x180 + ((x>>5)<<2)))
#define NVIC_ISPR_ADDR(x)          ((int32_t*)(NVIC_ISER_BASE + 0x200 + ((x>>5)<<2)))
#define NVIC_ICPR_ADDR(x)          ((int32_t*)(NVIC_ISER_BASE + 0x280 + ((x>>5)<<2)))
#define NVIC_IABR_ADDR(x)          ((int32_t*)(NVIC_ISER_BASE + 0x300 + ((x>>5)<<2)))
#define NVIC_BIT(x)                BIT(x % 32)

#define NVIC_SET_PRIO(c,x)          do {\
char prio = (char)x;\
int32_t code = (int32_t) c;\
int32_t *ispx = (int32_t*) + NVIC_ISER_BASE + 0x400 + ((code >> 2)<<2); \
*ispx |= (prio<<(8*(code & 0x000002)));\
} while (0);

#define NVIC_TRIGGER_INT(n) do {\
int32_t *stir = (int32_t*) NVIC_ISER_BASE + 0xE00; *stir = n & 0x000000FF; } while(0);

#endif