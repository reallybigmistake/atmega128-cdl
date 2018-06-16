/*------------------------------------------------------------------------------------------------*/
/*                                                                                                */
/*           Copyright (C) 2016 Brite Semiconductor Co., Ltd. All rights reserved.                */
/*                                                                                                */
/*------------------------------------------------------------------------------------------------*/


#ifndef __TEST_TYPES_H__
#define __TEST_TYPES_H__

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
#define NULL	0

#define __raw_writeb(v,a)	(*(volatile u8 *)(a) = ((u8)v))
#define __raw_writew(v,a)	(*(volatile u16 *)(a) = ((u16)v))
#define __raw_writel(v,a)	(*(volatile u32 *)(a) = ((u32)v))
#define __raw_readb(a)	(*(volatile u8 *)(a))
#define __raw_readw(a)	(*(volatile u16 *)(a))
#define __raw_readl(a)	(*(volatile u32 *)(a))

#define read8	__raw_readb
#define write8	__raw_writeb
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif
