#ifndef __CRTL_CRYPTO_LINUX_GFP_H
#define __CRTL_CRYPTO_LINUX_GFP_H 1


typedef  unsigned int gfp_t;

#define __GFP_BITS_SHIFT (26)


/* Plain integer GFP bitmasks. Do not use this directly. */
#define ___GFP_DMA		0x01u
#define ___GFP_HIGHMEM		0x02u
#define ___GFP_DMA32		0x04u
#define ___GFP_MOVABLE		0x08u
#define ___GFP_RECLAIMABLE	0x10u
#define ___GFP_HIGH		0x20u
#define ___GFP_IO		0x40u
#define ___GFP_FS		0x80u
#define ___GFP_ZERO		0x100u
#define ___GFP_ATOMIC		0x200u
#define ___GFP_DIRECT_RECLAIM	0x400u
#define ___GFP_KSWAPD_RECLAIM	0x800u
#define ___GFP_WRITE		0x1000u
#define ___GFP_NOWARN		0x2000u
#define ___GFP_RETRY_MAYFAIL	0x4000u
#define ___GFP_NOFAIL		0x8000u
#define ___GFP_NORETRY		0x10000u
#define ___GFP_MEMALLOC		0x20000u
#define ___GFP_COMP		0x40000u
#define ___GFP_NOMEMALLOC	0x80000u
#define ___GFP_HARDWALL		0x100000u
#define ___GFP_THISNODE		0x200000u
#define ___GFP_ACCOUNT		0x400000u
#ifdef CONFIG_LOCKDEP
#define ___GFP_NOLOCKDEP	0x800000u
#else
#define ___GFP_NOLOCKDEP	0
#endif

/*
 * Physical address zone modifiers (see linux/mmzone.h - low four bits)
 *
 * Do not put any conditional on these. If necessary modify the definitions
 * without the underscores and use them consistently. The definitions here may
 * be used in bit comparisons.
 */
#define __GFP_DMA	(( gfp_t)___GFP_DMA)
#define __GFP_HIGHMEM	(( gfp_t)___GFP_HIGHMEM)
#define __GFP_DMA32	(( gfp_t)___GFP_DMA32)
#define __GFP_MOVABLE	(( gfp_t)___GFP_MOVABLE)  /* ZONE_MOVABLE allowed */
#define GFP_ZONEMASK	(__GFP_DMA|__GFP_HIGHMEM|__GFP_DMA32|__GFP_MOVABLE)

/**
 * DOC: Page mobility and placement hints
 *
 * Page mobility and placement hints
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * These flags provide hints about how mobile the page is. Pages with similar
 * mobility are placed within the same pageblocks to minimise problems due
 * to external fragmentation.
 *
 * %__GFP_MOVABLE (also a zone modifier) indicates that the page can be
 * moved by page migration during memory compaction or can be reclaimed.
 *
 * %__GFP_RECLAIMABLE is used for slab allocations that specify
 * SLAB_RECLAIM_ACCOUNT and whose pages can be freed via shrinkers.
 *
 * %__GFP_WRITE indicates the caller intends to dirty the page. Where possible,
 * these pages will be spread between local zones to avoid all the dirty
 * pages being in one zone (fair zone allocation policy).
 *
 * %__GFP_HARDWALL enforces the cpuset memory allocation policy.
 *
 * %__GFP_THISNODE forces the allocation to be satisfied from the requested
 * node with no fallbacks or placement policy enforcements.
 *
 * %__GFP_ACCOUNT causes the allocation to be accounted to kmemcg.
 */
#define __GFP_RECLAIMABLE (( gfp_t)___GFP_RECLAIMABLE)
#define __GFP_WRITE	(( gfp_t)___GFP_WRITE)
#define __GFP_HARDWALL   (( gfp_t)___GFP_HARDWALL)
#define __GFP_THISNODE	(( gfp_t)___GFP_THISNODE)
#define __GFP_ACCOUNT	(( gfp_t)___GFP_ACCOUNT)


#endif /*<__CRTL_CRYPTO_LINUX_GFP_H>*/
