/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef __ASM_ARM_CCI_H
#define __ASM_ARM_CCI_H

#ifdef CONFIG_MCPM
#include <asm/mcpm.h>


static inline bool platform_has_secure_cci_access(void)
{
	return mcpm_is_available();
}

#else
static inline bool platform_has_secure_cci_access(void)
{
	return false;
}
#endif

#endif
