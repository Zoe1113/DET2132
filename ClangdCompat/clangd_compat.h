#ifndef CLANGD_COMPAT_H
#define CLANGD_COMPAT_H
/* clangd 索引兼容头：把 Sonix 非标准关键字映射为标准 C，不参与固件编译 */
#define sfr     extern volatile unsigned char
#define sbit    extern volatile unsigned char
#define bit     unsigned char
#define __asm
#define __interrupt
#define interrupt
#define data
#define idata
#define xdata
#define pdata
#define code
#endif