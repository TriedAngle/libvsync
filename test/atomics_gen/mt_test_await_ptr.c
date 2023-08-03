/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * SPDX-License-Identifier: MIT
 * Warning: File generated by tmpl; DO NOT EDIT.
 */
#include <vsync/atomic.h>
#include <vsync/common/assert.h>
#include <vsync/common/dbg.h>
#include <test/thread_launcher.h>
#include <vsync/utils/math.h>

/* keep number of threads even */
#define MAX_THREADS 10
vatomicptr_t g_shared;
/*****************************************************************************
 * Multi-thread Test: vatomicptr_await_eq
 *****************************************************************************/
static inline void *
mt_atomic_ptr_await_eq_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	(void)vatomicptr_await_eq(&g_shared,
							  (void *)(vuintptr_t)(VUINTPTR_MAX + (void *)tid));
	vatomicptr_write(&g_shared,
					 (void *)(vuintptr_t)(VUINTPTR_MAX + (void *)tid + 1U));
	return NULL;
}
static inline void
mt_atomic_ptr_await_eq(void)
{
	vatomicptr_init(&g_shared, (void *)(vuintptr_t)VUINTPTR_MAX);
	launch_threads(MAX_THREADS, mt_atomic_ptr_await_eq_run);
	void *cur = vatomicptr_read(&g_shared);
	ASSERT(cur == (void *)(vuintptr_t)(VUINTPTR_MAX + MAX_THREADS));
	V_UNUSED(cur);
}
/*****************************************************************************
 * Multi-thread Test: vatomicptr_await_eq_set
 *****************************************************************************/
static inline void *
mt_atomic_ptr_await_eq_set_run(void *args)
{
	vsize_t tid		= (vsize_t)(vuintptr_t)args;
	void *await_val = (void *)(vuintptr_t)(VUINTPTR_MAX + (void *)tid);
	void *new_val	= (void *)(vuintptr_t)((VUINTPTR_MAX + (void *)tid + 1));
	(void)vatomicptr_await_eq_set(&g_shared, await_val, new_val);
	return NULL;
}
static inline void
mt_atomic_ptr_await_eq_set(void)
{
	vatomicptr_init(&g_shared, (void *)(vuintptr_t)VUINTPTR_MAX);
	launch_threads(MAX_THREADS, mt_atomic_ptr_await_eq_set_run);
	void *cur = vatomicptr_read(&g_shared);
	ASSERT(cur == (void *)(vuintptr_t)(VUINTPTR_MAX + MAX_THREADS));
	V_UNUSED(cur);
}
/*****************************************************************************
 * Multi-thread Test: vatomicptr_await_neq
 *****************************************************************************/
static inline void *
mt_atomic_ptr_await_neq_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (tid == 0) {
		vatomicptr_write(&g_shared, (void *)(vuintptr_t)(VUINTPTR_MAX));
	} else {
		(void)vatomicptr_await_neq(&g_shared, NULL);
	}
	return NULL;
}
static inline void
mt_atomic_ptr_await_neq(void)
{
	vatomicptr_init(&g_shared, NULL);
	launch_threads(MAX_THREADS, mt_atomic_ptr_await_neq_run);
	void *cur	   = vatomicptr_read(&g_shared);
	void *expected = (void *)(vuintptr_t)(VUINTPTR_MAX);
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomicptr_await_neq_set
 *****************************************************************************/
static inline void *
mt_atomic_ptr_await_neq_set_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		(void)vatomicptr_await_neq_set(&g_shared,
									   (void *)(vuintptr_t)VUINTPTR_MAX,
									   (void *)(vuintptr_t)VUINTPTR_MAX);
	} else {
		(void)vatomicptr_await_neq_set(&g_shared,
									   (void *)(vuintptr_t)~VUINTPTR_MAX,
									   (void *)(vuintptr_t)~VUINTPTR_MAX);
	}
	return NULL;
}
static inline void
mt_atomic_ptr_await_neq_set(void)
{
	vatomicptr_init(&g_shared, (void *)(vuintptr_t)VUINTPTR_MAX);
	launch_threads(MAX_THREADS, mt_atomic_ptr_await_neq_set_run);
	void *cur	   = vatomicptr_read(&g_shared);
	void *expected = (void *)(vuintptr_t)VUINTPTR_MAX;
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Entry point
 *****************************************************************************/
int
main(void)
{
	mt_atomic_ptr_await_eq();
	mt_atomic_ptr_await_neq();
	mt_atomic_ptr_await_eq_set();
	mt_atomic_ptr_await_neq_set();

	return 0;
}
