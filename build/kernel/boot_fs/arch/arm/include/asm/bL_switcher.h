/* SPDX-License-Identifier: GPL-2.0-only */


#ifndef ASM_BL_SWITCHER_H
#define ASM_BL_SWITCHER_H

#include <linux/compiler.h>
#include <linux/types.h>

typedef void (*bL_switch_completion_handler)(void *cookie);

int bL_switch_request_cb(unsigned int cpu, unsigned int new_cluster_id,
			 bL_switch_completion_handler completer,
			 void *completer_cookie);
static inline int bL_switch_request(unsigned int cpu, unsigned int new_cluster_id)
{
	return bL_switch_request_cb(cpu, new_cluster_id, NULL, NULL);
}


#define BL_NOTIFY_PRE_ENABLE	0
#define BL_NOTIFY_POST_ENABLE	1
#define BL_NOTIFY_PRE_DISABLE	2
#define BL_NOTIFY_POST_DISABLE	3

#ifdef CONFIG_BL_SWITCHER

int bL_switcher_register_notifier(struct notifier_block *nb);
int bL_switcher_unregister_notifier(struct notifier_block *nb);


bool bL_switcher_get_enabled(void);
void bL_switcher_put_enabled(void);

int bL_switcher_trace_trigger(void);
int bL_switcher_get_logical_index(u32 mpidr);

#else
static inline int bL_switcher_register_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int bL_switcher_unregister_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline bool bL_switcher_get_enabled(void) { return false; }
static inline void bL_switcher_put_enabled(void) { }
static inline int bL_switcher_trace_trigger(void) { return 0; }
static inline int bL_switcher_get_logical_index(u32 mpidr) { return -EUNATCH; }
#endif 

#endif
