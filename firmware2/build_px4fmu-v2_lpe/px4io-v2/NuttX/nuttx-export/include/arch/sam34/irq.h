/****************************************************************************************
 * arch/arm/include/sam34/irq.h
 *
 *   Copyright (C) 2009-2011, 2013 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************************/

/* This file should never be included directed but, rather, only indirectly through
 * nuttx/irq.h
 */


#ifndef __ARCH_ARM_INCLUDE_SAM34_IRQ_H
#define __ARCH_ARM_INCLUDE_SAM34_IRQ_H

/****************************************************************************************
 * Included Files
 ****************************************************************************************/

#include <nuttx/config.h>
#include <arch/sam34/chip.h>

/****************************************************************************************
 * Pre-processor Definitions
 ****************************************************************************************/

/* IRQ numbers.  The IRQ number corresponds vector number and hence map directly to
 * bits in the NVIC.  This does, however, waste several words of memory in the IRQ
 * to handle mapping tables.
 */

/* Common Processor Exceptions (vectors 0-15) */

#define SAM_IRQ_RESERVED       (0) /* Reserved vector (only used with CONFIG_DEBUG) */
                                   /* Vector  0: Reset stack pointer value */
                                   /* Vector  1: Reset (not handler as an IRQ) */
#define SAM_IRQ_NMI            (2) /* Vector  2: Non-Maskable Interrupt (NMI) */
#define SAM_IRQ_HARDFAULT      (3) /* Vector  3: Hard fault */
#define SAM_IRQ_MEMFAULT       (4) /* Vector  4: Memory management (MPU) */
#define SAM_IRQ_BUSFAULT       (5) /* Vector  5: Bus fault */
#define SAM_IRQ_USAGEFAULT     (6) /* Vector  6: Usage fault */
#define SAM_IRQ_SVCALL        (11) /* Vector 11: SVC call */
#define SAM_IRQ_DBGMONITOR    (12) /* Vector 12: Debug Monitor */
                                   /* Vector 13: Reserved */
#define SAM_IRQ_PENDSV        (14) /* Vector 14: Pendable system service request */
#define SAM_IRQ_SYSTICK       (15) /* Vector 15: System tick */

/* Chip-Specific External interrupts */

#define SAM_IRQ_EXTINT        (16) /* Vector number of the first external interrupt */

#if defined(CONFIG_ARCH_CHIP_SAM3U)
#  include <arch/sam34/sam3u_irq.h>
#elif defined(CONFIG_ARCH_CHIP_SAM4L)
#  include <arch/sam34/sam4l_irq.h>
#elif defined(CONFIG_ARCH_CHIP_SAM4S)
#  include <arch/sam34/sam4s_irq.h>
#else
#  error Unrecognized SAM architecture
#endif

/****************************************************************************************
 * Public Types
 ****************************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************************
 * Public Data
 ****************************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************************
 * Public Function Prototypes
 ****************************************************************************************/

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif

#endif /* __ARCH_ARM_INCLUDE_SAM34_IRQ_H */

