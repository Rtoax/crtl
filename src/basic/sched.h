/* Definitions for POSIX 1003.1b-1993 (aka POSIX.4) scheduling interface.
   Copyright (C) 1996-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef	_SCHED_H
#define	_SCHED_H	1

/* Data structure to describe a process' schedulability.  */
struct sched_param
{
  int sched_priority;
};


/* Backward compatibility.  */
#define sched_priority    sched_priority
#define __sched_priority  sched_priority


__BEGIN_DECLS

/* Set scheduling parameters for a process.  */
extern int sched_setparam (__pid_t __pid, const struct sched_param *__param);

/* Retrieve scheduling parameters for a particular process.  */
extern int sched_getparam (__pid_t __pid, struct sched_param *__param) ;

/* Set scheduling algorithm and/or parameters for a process.  */
extern int sched_setscheduler (__pid_t __pid, int __policy,
			       const struct sched_param *__param) ;

/* Retrieve scheduling algorithm for a particular purpose.  */
extern int sched_getscheduler (__pid_t __pid) ;

/* Yield the processor.  */
extern int sched_yield (void) ;

/* Get maximum priority value for a scheduler.  */
extern int sched_get_priority_max (int __algorithm) ;

/* Get minimum priority value for a scheduler.  */
extern int sched_get_priority_min (int __algorithm) ;

/* Get the SCHED_RR interval for the named process.  */
extern int sched_rr_get_interval (__pid_t __pid, struct timespec *__t) ;


#ifdef __USE_GNU
/* Access macros for `cpu_set'.  */
# define CPU_SETSIZE __CPU_SETSIZE
# define CPU_SET(cpu, cpusetp)	 __CPU_SET_S (cpu, sizeof (cpu_set_t), cpusetp)
# define CPU_CLR(cpu, cpusetp)	 __CPU_CLR_S (cpu, sizeof (cpu_set_t), cpusetp)
# define CPU_ISSET(cpu, cpusetp) __CPU_ISSET_S (cpu, sizeof (cpu_set_t), \
						cpusetp)
# define CPU_ZERO(cpusetp)	 __CPU_ZERO_S (sizeof (cpu_set_t), cpusetp)
# define CPU_COUNT(cpusetp)	 __CPU_COUNT_S (sizeof (cpu_set_t), cpusetp)

# define CPU_SET_S(cpu, setsize, cpusetp)   __CPU_SET_S (cpu, setsize, cpusetp)
# define CPU_CLR_S(cpu, setsize, cpusetp)   __CPU_CLR_S (cpu, setsize, cpusetp)
# define CPU_ISSET_S(cpu, setsize, cpusetp) __CPU_ISSET_S (cpu, setsize, \
							   cpusetp)
# define CPU_ZERO_S(setsize, cpusetp)	    __CPU_ZERO_S (setsize, cpusetp)
# define CPU_COUNT_S(setsize, cpusetp)	    __CPU_COUNT_S (setsize, cpusetp)

# define CPU_EQUAL(cpusetp1, cpusetp2) \
  __CPU_EQUAL_S (sizeof (cpu_set_t), cpusetp1, cpusetp2)
# define CPU_EQUAL_S(setsize, cpusetp1, cpusetp2) \
  __CPU_EQUAL_S (setsize, cpusetp1, cpusetp2)

# define CPU_AND(destset, srcset1, srcset2) \
  __CPU_OP_S (sizeof (cpu_set_t), destset, srcset1, srcset2, &)
# define CPU_OR(destset, srcset1, srcset2) \
  __CPU_OP_S (sizeof (cpu_set_t), destset, srcset1, srcset2, |)
# define CPU_XOR(destset, srcset1, srcset2) \
  __CPU_OP_S (sizeof (cpu_set_t), destset, srcset1, srcset2, ^)
# define CPU_AND_S(setsize, destset, srcset1, srcset2) \
  __CPU_OP_S (setsize, destset, srcset1, srcset2, &)
# define CPU_OR_S(setsize, destset, srcset1, srcset2) \
  __CPU_OP_S (setsize, destset, srcset1, srcset2, |)
# define CPU_XOR_S(setsize, destset, srcset1, srcset2) \
  __CPU_OP_S (setsize, destset, srcset1, srcset2, ^)

# define CPU_ALLOC_SIZE(count) __CPU_ALLOC_SIZE (count)
# define CPU_ALLOC(count) __CPU_ALLOC (count)
# define CPU_FREE(cpuset) __CPU_FREE (cpuset)


/* Set the CPU affinity for a task */
extern int sched_setaffinity (__pid_t __pid, size_t __cpusetsize,
			      const cpu_set_t *__cpuset) ;

/* Get the CPU affinity for a task */
extern int sched_getaffinity (__pid_t __pid, size_t __cpusetsize,
			      cpu_set_t *__cpuset) ;
#endif

__END_DECLS

#endif /* sched.h */


/* Definition of the cpu_set_t structure used by the POSIX 1003.1b-1993
   scheduling interface.
   Copyright (C) 1996-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#ifndef _BITS_CPU_SET_H
#define _BITS_CPU_SET_H 1

#ifndef _SCHED_H
# error "Never include <bits/cpu-set.h> directly; use <sched.h> instead."
#endif

/* Size definition for CPU sets.  */
#define __CPU_SETSIZE	1024
#define __NCPUBITS	(8 * sizeof (__cpu_mask))

/* Type for array elements in 'cpu_set_t'.  */
typedef __CPU_MASK_TYPE __cpu_mask;

/* Basic access functions.  */
#define __CPUELT(cpu)	((cpu) / __NCPUBITS)
#define __CPUMASK(cpu)	((__cpu_mask) 1 << ((cpu) % __NCPUBITS))

/* Data structure to describe CPU mask.  */
typedef struct
{
  __cpu_mask __bits[__CPU_SETSIZE / __NCPUBITS];
} cpu_set_t;

/* Access functions for CPU masks.  */
#if __GNUC_PREREQ (2, 91)
# define __CPU_ZERO_S(setsize, cpusetp) \
  do __builtin_memset (cpusetp, '\0', setsize); while (0)
#else
# define __CPU_ZERO_S(setsize, cpusetp) \
  do {									      \
    size_t __i;								      \
    size_t __imax = (setsize) / sizeof (__cpu_mask);			      \
    __cpu_mask *__bits = (cpusetp)->__bits;				      \
    for (__i = 0; __i < __imax; ++__i)					      \
      __bits[__i] = 0;							      \
  } while (0)
#endif
#define __CPU_SET_S(cpu, setsize, cpusetp) \
  (__extension__							      \
   ({ size_t __cpu = (cpu);						      \
      __cpu / 8 < (setsize)						      \
      ? (((__cpu_mask *) ((cpusetp)->__bits))[__CPUELT (__cpu)]		      \
	 |= __CPUMASK (__cpu))						      \
      : 0; }))
#define __CPU_CLR_S(cpu, setsize, cpusetp) \
  (__extension__							      \
   ({ size_t __cpu = (cpu);						      \
      __cpu / 8 < (setsize)						      \
      ? (((__cpu_mask *) ((cpusetp)->__bits))[__CPUELT (__cpu)]		      \
	 &= ~__CPUMASK (__cpu))						      \
      : 0; }))
#define __CPU_ISSET_S(cpu, setsize, cpusetp) \
  (__extension__							      \
   ({ size_t __cpu = (cpu);						      \
      __cpu / 8 < (setsize)						      \
      ? ((((const __cpu_mask *) ((cpusetp)->__bits))[__CPUELT (__cpu)]	      \
	  & __CPUMASK (__cpu))) != 0					      \
      : 0; }))

#define __CPU_COUNT_S(setsize, cpusetp) \
  __sched_cpucount (setsize, cpusetp)

#if __GNUC_PREREQ (2, 91)
# define __CPU_EQUAL_S(setsize, cpusetp1, cpusetp2) \
  (__builtin_memcmp (cpusetp1, cpusetp2, setsize) == 0)
#else
# define __CPU_EQUAL_S(setsize, cpusetp1, cpusetp2) \
  (__extension__							      \
   ({ const __cpu_mask *__arr1 = (cpusetp1)->__bits;			      \
      const __cpu_mask *__arr2 = (cpusetp2)->__bits;			      \
      size_t __imax = (setsize) / sizeof (__cpu_mask);			      \
      size_t __i;							      \
      for (__i = 0; __i < __imax; ++__i)				      \
	if (__arr1[__i] != __arr2[__i])					      \
	  break;							      \
      __i == __imax; }))
#endif

#define __CPU_OP_S(setsize, destset, srcset1, srcset2, op) \
  (__extension__							      \
   ({ cpu_set_t *__dest = (destset);					      \
      const __cpu_mask *__arr1 = (srcset1)->__bits;			      \
      const __cpu_mask *__arr2 = (srcset2)->__bits;			      \
      size_t __imax = (setsize) / sizeof (__cpu_mask);			      \
      size_t __i;							      \
      for (__i = 0; __i < __imax; ++__i)				      \
	((__cpu_mask *) __dest->__bits)[__i] = __arr1[__i] op __arr2[__i];    \
      __dest; }))

#define __CPU_ALLOC_SIZE(count) \
  ((((count) + __NCPUBITS - 1) / __NCPUBITS) * sizeof (__cpu_mask))
#define __CPU_ALLOC(count) __sched_cpualloc (count)
#define __CPU_FREE(cpuset) __sched_cpufree (cpuset)

__BEGIN_DECLS

extern int __sched_cpucount (size_t __setsize, const cpu_set_t *__setp)
     __THROW;
extern cpu_set_t *__sched_cpualloc (size_t __count) __THROW __wur;
extern void __sched_cpufree (cpu_set_t *__set) __THROW;

__END_DECLS

#endif /* bits/cpu-set.h */

