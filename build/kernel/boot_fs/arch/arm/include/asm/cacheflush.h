/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _ASMARM_CACHEFLUSH_H
#define _ASMARM_CACHEFLUSH_H

#include <linux/mm.h>

#include <asm/glue-cache.h>
#include <asm/shmparam.h>
#include <asm/cachetype.h>
#include <asm/outercache.h>

#define CACHE_COLOUR(vaddr)	((vaddr & (SHMLBA - 1)) >> PAGE_SHIFT)


#define PG_dcache_clean PG_arch_1



struct cpu_cache_fns {
	void (*flush_icache_all)(void);
	void (*flush_kern_all)(void);
	void (*flush_kern_louis)(void);
	void (*flush_user_all)(void);
	void (*flush_user_range)(unsigned long, unsigned long, unsigned int);

	void (*coherent_kern_range)(unsigned long, unsigned long);
	int  (*coherent_user_range)(unsigned long, unsigned long);
	void (*flush_kern_dcache_area)(void *, size_t);

	void (*dma_map_area)(const void *, size_t, int);
	void (*dma_unmap_area)(const void *, size_t, int);

	void (*dma_flush_range)(const void *, const void *);
} __no_randomize_layout;


#ifdef MULTI_CACHE

extern struct cpu_cache_fns cpu_cache;

#define __cpuc_flush_icache_all		cpu_cache.flush_icache_all
#define __cpuc_flush_kern_all		cpu_cache.flush_kern_all
#define __cpuc_flush_kern_louis		cpu_cache.flush_kern_louis
#define __cpuc_flush_user_all		cpu_cache.flush_user_all
#define __cpuc_flush_user_range		cpu_cache.flush_user_range
#define __cpuc_coherent_kern_range	cpu_cache.coherent_kern_range
#define __cpuc_coherent_user_range	cpu_cache.coherent_user_range
#define __cpuc_flush_dcache_area	cpu_cache.flush_kern_dcache_area


#define dmac_flush_range		cpu_cache.dma_flush_range

#else

extern void __cpuc_flush_icache_all(void);
extern void __cpuc_flush_kern_all(void);
extern void __cpuc_flush_kern_louis(void);
extern void __cpuc_flush_user_all(void);
extern void __cpuc_flush_user_range(unsigned long, unsigned long, unsigned int);
extern void __cpuc_coherent_kern_range(unsigned long, unsigned long);
extern int  __cpuc_coherent_user_range(unsigned long, unsigned long);
extern void __cpuc_flush_dcache_area(void *, size_t);


extern void dmac_flush_range(const void *, const void *);

#endif


extern void copy_to_user_page(struct vm_area_struct *, struct page *,
	unsigned long, void *, const void *, unsigned long);
#define copy_from_user_page(vma, page, vaddr, dst, src, len) \
	do {							\
		memcpy(dst, src, len);				\
	} while (0)




#define __flush_icache_all_generic()					\
	asm("mcr	p15, 0, %0, c7, c5, 0"				\
	    : : "r" (0));


#define __flush_icache_all_v7_smp()					\
	asm("mcr	p15, 0, %0, c7, c1, 0"				\
	    : : "r" (0));


#if (defined(CONFIG_CPU_V7) && \
     (defined(CONFIG_CPU_V6) || defined(CONFIG_CPU_V6K))) || \
	defined(CONFIG_SMP_ON_UP)
#define __flush_icache_preferred	__cpuc_flush_icache_all
#elif __LINUX_ARM_ARCH__ >= 7 && defined(CONFIG_SMP)
#define __flush_icache_preferred	__flush_icache_all_v7_smp
#elif __LINUX_ARM_ARCH__ == 6 && defined(CONFIG_ARM_ERRATA_411920)
#define __flush_icache_preferred	__cpuc_flush_icache_all
#else
#define __flush_icache_preferred	__flush_icache_all_generic
#endif

static inline void __flush_icache_all(void)
{
	__flush_icache_preferred();
	dsb(ishst);
}


#define flush_cache_louis()		__cpuc_flush_kern_louis()

#define flush_cache_all()		__cpuc_flush_kern_all()

static inline void vivt_flush_cache_mm(struct mm_struct *mm)
{
	if (cpumask_test_cpu(smp_processor_id(), mm_cpumask(mm)))
		__cpuc_flush_user_all();
}

static inline void
vivt_flush_cache_range(struct vm_area_struct *vma, unsigned long start, unsigned long end)
{
	struct mm_struct *mm = vma->vm_mm;

	if (!mm || cpumask_test_cpu(smp_processor_id(), mm_cpumask(mm)))
		__cpuc_flush_user_range(start & PAGE_MASK, PAGE_ALIGN(end),
					vma->vm_flags);
}

static inline void
vivt_flush_cache_page(struct vm_area_struct *vma, unsigned long user_addr, unsigned long pfn)
{
	struct mm_struct *mm = vma->vm_mm;

	if (!mm || cpumask_test_cpu(smp_processor_id(), mm_cpumask(mm))) {
		unsigned long addr = user_addr & PAGE_MASK;
		__cpuc_flush_user_range(addr, addr + PAGE_SIZE, vma->vm_flags);
	}
}

#ifndef CONFIG_CPU_CACHE_VIPT
#define flush_cache_mm(mm) \
		vivt_flush_cache_mm(mm)
#define flush_cache_range(vma,start,end) \
		vivt_flush_cache_range(vma,start,end)
#define flush_cache_page(vma,addr,pfn) \
		vivt_flush_cache_page(vma,addr,pfn)
#else
extern void flush_cache_mm(struct mm_struct *mm);
extern void flush_cache_range(struct vm_area_struct *vma, unsigned long start, unsigned long end);
extern void flush_cache_page(struct vm_area_struct *vma, unsigned long user_addr, unsigned long pfn);
#endif

#define flush_cache_dup_mm(mm) flush_cache_mm(mm)


#define flush_cache_user_range(s,e)	__cpuc_coherent_user_range(s,e)


#define flush_icache_range(s,e)		__cpuc_coherent_kern_range(s,e)


#define clean_dcache_area(start,size)	cpu_dcache_clean_area(start, size)


#define ARCH_IMPLEMENTS_FLUSH_DCACHE_PAGE 1
extern void flush_dcache_page(struct page *);

static inline void flush_kernel_vmap_range(void *addr, int size)
{
	if ((cache_is_vivt() || cache_is_vipt_aliasing()))
	  __cpuc_flush_dcache_area(addr, (size_t)size);
}
static inline void invalidate_kernel_vmap_range(void *addr, int size)
{
	if ((cache_is_vivt() || cache_is_vipt_aliasing()))
	  __cpuc_flush_dcache_area(addr, (size_t)size);
}

#define ARCH_HAS_FLUSH_ANON_PAGE
static inline void flush_anon_page(struct vm_area_struct *vma,
			 struct page *page, unsigned long vmaddr)
{
	extern void __flush_anon_page(struct vm_area_struct *vma,
				struct page *, unsigned long);
	if (PageAnon(page))
		__flush_anon_page(vma, page, vmaddr);
}

#define ARCH_HAS_FLUSH_KERNEL_DCACHE_PAGE
extern void flush_kernel_dcache_page(struct page *);

#define flush_dcache_mmap_lock(mapping)		xa_lock_irq(&mapping->i_pages)
#define flush_dcache_mmap_unlock(mapping)	xa_unlock_irq(&mapping->i_pages)

#define flush_icache_user_range(vma,page,addr,len) \
	flush_dcache_page(page)


#define flush_icache_page(vma,page)	do { } while (0)


static inline void flush_cache_vmap(unsigned long start, unsigned long end)
{
	if (!cache_is_vipt_nonaliasing())
		flush_cache_all();
	else
		
		dsb(ishst);
}

static inline void flush_cache_vunmap(unsigned long start, unsigned long end)
{
	if (!cache_is_vipt_nonaliasing())
		flush_cache_all();
}




#define __CACHE_WRITEBACK_ORDER 6  
#define __CACHE_WRITEBACK_GRANULE (1 << __CACHE_WRITEBACK_ORDER)


#define __cpuc_clean_dcache_area __cpuc_flush_dcache_area


static inline void __sync_cache_range_w(volatile void *p, size_t size)
{
	char *_p = (char *)p;

	__cpuc_clean_dcache_area(_p, size);
	outer_clean_range(__pa(_p), __pa(_p + size));
}


static inline void __sync_cache_range_r(volatile void *p, size_t size)
{
	char *_p = (char *)p;

#ifdef CONFIG_OUTER_CACHE
	if (outer_cache.flush_range) {
		
		__cpuc_clean_dcache_area(_p, size);

		
		outer_flush_range(__pa(_p), __pa(_p + size));
	}
#endif

	
	__cpuc_flush_dcache_area(_p, size);
}

#define sync_cache_w(ptr) __sync_cache_range_w(ptr, sizeof *(ptr))
#define sync_cache_r(ptr) __sync_cache_range_r(ptr, sizeof *(ptr))


#define v7_exit_coherency_flush(level) \
	asm volatile( \
	".arch	armv7-a \n\t" \
	"stmfd	sp!, {fp, ip} \n\t" \
	"mrc	p15, 0, r0, c1, c0, 0	@ get SCTLR \n\t" \
	"bic	r0, r0, #"__stringify(CR_C)" \n\t" \
	"mcr	p15, 0, r0, c1, c0, 0	@ set SCTLR \n\t" \
	"isb	\n\t" \
	"bl	v7_flush_dcache_"__stringify(level)" \n\t" \
	"mrc	p15, 0, r0, c1, c0, 1	@ get ACTLR \n\t" \
	"bic	r0, r0, #(1 << 6)	@ disable local coherency \n\t" \
	"mcr	p15, 0, r0, c1, c0, 1	@ set ACTLR \n\t" \
	"isb	\n\t" \
	"dsb	\n\t" \
	"ldmfd	sp!, {fp, ip}" \
	: : : "r0","r1","r2","r3","r4","r5","r6","r7", \
	      "r9","r10","lr","memory" )

void flush_uprobe_xol_access(struct page *page, unsigned long uaddr,
			     void *kaddr, unsigned long len);


#ifdef CONFIG_CPU_ICACHE_MISMATCH_WORKAROUND
void check_cpu_icache_size(int cpuid);
#else
static inline void check_cpu_icache_size(int cpuid) { }
#endif

#endif
