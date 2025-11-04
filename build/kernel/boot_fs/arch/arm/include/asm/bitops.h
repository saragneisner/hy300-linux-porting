/* SPDX-License-Identifier: GPL-2.0 */


#ifndef __ASM_ARM_BITOPS_H
#define __ASM_ARM_BITOPS_H

#ifdef __KERNEL__

#ifndef _LINUX_BITOPS_H
#error only <linux/bitops.h> can be included directly
#endif

#include <linux/compiler.h>
#include <linux/irqflags.h>
#include <asm/barrier.h>


static inline void ____atomic_set_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned long mask = BIT_MASK(bit);

	p += BIT_WORD(bit);

	raw_local_irq_save(flags);
	*p |= mask;
	raw_local_irq_restore(flags);
}

static inline void ____atomic_clear_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned long mask = BIT_MASK(bit);

	p += BIT_WORD(bit);

	raw_local_irq_save(flags);
	*p &= ~mask;
	raw_local_irq_restore(flags);
}

static inline void ____atomic_change_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned long mask = BIT_MASK(bit);

	p += BIT_WORD(bit);

	raw_local_irq_save(flags);
	*p ^= mask;
	raw_local_irq_restore(flags);
}

static inline int
____atomic_test_and_set_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned int res;
	unsigned long mask = BIT_MASK(bit);

	p += BIT_WORD(bit);

	raw_local_irq_save(flags);
	res = *p;
	*p = res | mask;
	raw_local_irq_restore(flags);

	return (res & mask) != 0;
}

static inline int
____atomic_test_and_clear_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned int res;
	unsigned long mask = BIT_MASK(bit);

	p += BIT_WORD(bit);

	raw_local_irq_save(flags);
	res = *p;
	*p = res & ~mask;
	raw_local_irq_restore(flags);

	return (res & mask) != 0;
}

static inline int
____atomic_test_and_change_bit(unsigned int bit, volatile unsigned long *p)
{
	unsigned long flags;
	unsigned int res;
	unsigned long mask = BIT_MASK(bit);

	p += BIT_WORD(bit);

	raw_local_irq_save(flags);
	res = *p;
	*p = res ^ mask;
	raw_local_irq_restore(flags);

	return (res & mask) != 0;
}

#include <asm-generic/bitops/non-atomic.h>




extern void _set_bit(int nr, volatile unsigned long * p);
extern void _clear_bit(int nr, volatile unsigned long * p);
extern void _change_bit(int nr, volatile unsigned long * p);
extern int _test_and_set_bit(int nr, volatile unsigned long * p);
extern int _test_and_clear_bit(int nr, volatile unsigned long * p);
extern int _test_and_change_bit(int nr, volatile unsigned long * p);


extern int _find_first_zero_bit_le(const unsigned long *p, unsigned size);
extern int _find_next_zero_bit_le(const unsigned long *p, int size, int offset);
extern int _find_first_bit_le(const unsigned long *p, unsigned size);
extern int _find_next_bit_le(const unsigned long *p, int size, int offset);


extern int _find_first_zero_bit_be(const unsigned long *p, unsigned size);
extern int _find_next_zero_bit_be(const unsigned long *p, int size, int offset);
extern int _find_first_bit_be(const unsigned long *p, unsigned size);
extern int _find_next_bit_be(const unsigned long *p, int size, int offset);

#ifndef CONFIG_SMP

#define ATOMIC_BITOP(name,nr,p)			\
	(__builtin_constant_p(nr) ? ____atomic_##name(nr, p) : _##name(nr,p))
#else
#define ATOMIC_BITOP(name,nr,p)		_##name(nr,p)
#endif


#define set_bit(nr,p)			ATOMIC_BITOP(set_bit,nr,p)
#define clear_bit(nr,p)			ATOMIC_BITOP(clear_bit,nr,p)
#define change_bit(nr,p)		ATOMIC_BITOP(change_bit,nr,p)
#define test_and_set_bit(nr,p)		ATOMIC_BITOP(test_and_set_bit,nr,p)
#define test_and_clear_bit(nr,p)	ATOMIC_BITOP(test_and_clear_bit,nr,p)
#define test_and_change_bit(nr,p)	ATOMIC_BITOP(test_and_change_bit,nr,p)

#ifndef __ARMEB__

#define find_first_zero_bit(p,sz)	_find_first_zero_bit_le(p,sz)
#define find_next_zero_bit(p,sz,off)	_find_next_zero_bit_le(p,sz,off)
#define find_first_bit(p,sz)		_find_first_bit_le(p,sz)
#define find_next_bit(p,sz,off)		_find_next_bit_le(p,sz,off)

#else

#define find_first_zero_bit(p,sz)	_find_first_zero_bit_be(p,sz)
#define find_next_zero_bit(p,sz,off)	_find_next_zero_bit_be(p,sz,off)
#define find_first_bit(p,sz)		_find_first_bit_be(p,sz)
#define find_next_bit(p,sz,off)		_find_next_bit_be(p,sz,off)

#endif

#if __LINUX_ARM_ARCH__ < 5

#include <asm-generic/bitops/__fls.h>
#include <asm-generic/bitops/__ffs.h>
#include <asm-generic/bitops/fls.h>
#include <asm-generic/bitops/ffs.h>

#else


#include <asm-generic/bitops/builtin-__fls.h>
#include <asm-generic/bitops/builtin-__ffs.h>
#include <asm-generic/bitops/builtin-fls.h>
#include <asm-generic/bitops/builtin-ffs.h>

#endif

#include <asm-generic/bitops/ffz.h>

#include <asm-generic/bitops/fls64.h>

#include <asm-generic/bitops/sched.h>
#include <asm-generic/bitops/hweight.h>
#include <asm-generic/bitops/lock.h>

#ifdef __ARMEB__

static inline int find_first_zero_bit_le(const void *p, unsigned size)
{
	return _find_first_zero_bit_le(p, size);
}
#define find_first_zero_bit_le find_first_zero_bit_le

static inline int find_next_zero_bit_le(const void *p, int size, int offset)
{
	return _find_next_zero_bit_le(p, size, offset);
}
#define find_next_zero_bit_le find_next_zero_bit_le

static inline int find_next_bit_le(const void *p, int size, int offset)
{
	return _find_next_bit_le(p, size, offset);
}
#define find_next_bit_le find_next_bit_le

#endif

#include <asm-generic/bitops/find.h>
#include <asm-generic/bitops/le.h>


#include <asm-generic/bitops/ext2-atomic-setbit.h>

#endif 

#endif 
