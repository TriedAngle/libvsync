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
#define IT			  10
#define V_DOUBLE(_v_) ((_v_)*2)
#define MAX_THREADS	  10
#define VUINT32_VAL	  ((((vuint32_t)0xF) << 16U) | ((vuint32_t)VUINT16_MAX))
vatomic32_t g_shared;
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_eq
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_eq_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	(void)vatomic32_await_eq(&g_shared, (VUINT32_VAL + (vuint32_t)tid));
	vatomic32_write(&g_shared, (VUINT32_VAL + (vuint32_t)tid + 1U));
	return NULL;
}
static inline void
mt_atomic_u32_await_eq(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_eq_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	ASSERT(cur == (VUINT32_VAL + MAX_THREADS));
	V_UNUSED(cur);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_eq_add
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_eq_add_run(void *args)
{
	vsize_t tid			= (vsize_t)(vuintptr_t)args;
	vuint32_t await_val = (VUINT32_VAL + (vuint32_t)tid);
	vuint32_t new_val	= (1);
	(void)vatomic32_await_eq_add(&g_shared, await_val, new_val);
	return NULL;
}
static inline void
mt_atomic_u32_await_eq_add(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_eq_add_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	ASSERT(cur == (VUINT32_VAL + MAX_THREADS));
	V_UNUSED(cur);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_eq_set
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_eq_set_run(void *args)
{
	vsize_t tid			= (vsize_t)(vuintptr_t)args;
	vuint32_t await_val = (VUINT32_VAL + (vuint32_t)tid);
	vuint32_t new_val	= ((VUINT32_VAL + (vuint32_t)tid + 1));
	(void)vatomic32_await_eq_set(&g_shared, await_val, new_val);
	return NULL;
}
static inline void
mt_atomic_u32_await_eq_set(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_eq_set_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	ASSERT(cur == (VUINT32_VAL + MAX_THREADS));
	V_UNUSED(cur);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_eq_sub
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_eq_sub_run(void *args)
{
	vsize_t tid		   = (vsize_t)(vuintptr_t)args;
	vuint32_t wait_val = (VUINT32_VAL + MAX_THREADS) - (vuint32_t)tid;
	(void)vatomic32_await_eq_sub(&g_shared, wait_val, 1);
	return NULL;
}
static inline void
mt_atomic_u32_await_eq_sub(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL + MAX_THREADS);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_eq_sub_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	ASSERT(cur == VUINT32_VAL);
	V_UNUSED(cur);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_gt
 *****************************************************************************/
static inline void
mt_atomic_u32_await_gt_waiter(void)
{
	for (vuint32_t i = 0; i < IT; i++) {
		(void)vatomic32_await_gt(&g_shared, (VUINT32_VAL + i));
	}
}
static inline void
mt_atomic_u32_await_gt_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_inc(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_gt_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_gt_waiter();
	} else {
		mt_atomic_u32_await_gt_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_gt(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_gt_run);
	vuint32_t cur	   = vatomic32_read(&g_shared);
	vuint32_t expected = (VUINT32_VAL + (MAX_THREADS * IT));
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_ge
 *****************************************************************************/
static inline void
mt_atomic_u32_await_ge_waiter(void)
{
	for (vuint32_t i = 0; i < IT; i++) {
		(void)vatomic32_await_ge(&g_shared, (VUINT32_VAL + i));
	}
}
static inline void
mt_atomic_u32_await_ge_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_inc(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_ge_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_ge_waiter();
	} else {
		mt_atomic_u32_await_ge_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_ge(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_ge_run);
	vuint32_t cur	   = vatomic32_read(&g_shared);
	vuint32_t expected = (VUINT32_VAL + (MAX_THREADS * IT));
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_neq
 *****************************************************************************/
static inline void
mt_atomic_u32_await_neq_waiter(void)
{
	for (vuint32_t i = 0; i < IT; i++) {
		(void)vatomic32_await_neq(&g_shared, (VUINT32_VAL + i));
	}
}
static inline void
mt_atomic_u32_await_neq_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_inc(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_neq_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_neq_waiter();
	} else {
		mt_atomic_u32_await_neq_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_neq(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_neq_run);
	vuint32_t cur	   = vatomic32_read(&g_shared);
	vuint32_t expected = (VUINT32_VAL + (MAX_THREADS * IT));
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_lt
 *****************************************************************************/
static inline void
mt_atomic_u32_await_lt_waiter(void)
{
	for (vuint32_t i = 0; i < IT; i++) {
		(void)vatomic32_await_lt(&g_shared, (VUINT32_VAL - i));
	}
}
static inline void
mt_atomic_u32_await_lt_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_dec(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_lt_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_lt_waiter();
	} else {
		mt_atomic_u32_await_lt_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_lt(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_lt_run);
	vuint32_t cur	   = vatomic32_read(&g_shared);
	vuint32_t expected = (VUINT32_VAL - (MAX_THREADS * IT));
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_gt_add
 *****************************************************************************/
static inline void
mt_atomic_u32_await_gt_add_waiter(void)
{
	vuint32_t await_val = 0;
	for (vuint32_t i = 0; i < IT; i++) {
		await_val = (VUINT32_VAL + i) + 1;
		(void)vatomic32_await_gt_add(&g_shared, await_val, 1);
	}
}
static inline void
mt_atomic_u32_await_gt_add_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_inc(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_gt_add_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_gt_add_waiter();
	} else {
		mt_atomic_u32_await_gt_add_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_gt_add(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_gt_add_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	vuint32_t expected =
		(VUINT32_VAL + (MAX_THREADS * IT)) + ((MAX_THREADS / 2) * IT);
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_ge_add
 *****************************************************************************/
static inline void
mt_atomic_u32_await_ge_add_waiter(void)
{
	vuint32_t await_val = 0;
	for (vuint32_t i = 0; i < IT; i++) {
		await_val = (VUINT32_VAL + i) + 1;
		(void)vatomic32_await_ge_add(&g_shared, await_val, 1);
	}
}
static inline void
mt_atomic_u32_await_ge_add_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_inc(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_ge_add_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_ge_add_waiter();
	} else {
		mt_atomic_u32_await_ge_add_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_ge_add(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_ge_add_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	vuint32_t expected =
		(VUINT32_VAL + (MAX_THREADS * IT)) + ((MAX_THREADS / 2) * IT);
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_neq_add
 *****************************************************************************/
static inline void
mt_atomic_u32_await_neq_add_waiter(void)
{
	vuint32_t await_val = 0;
	for (vuint32_t i = 0; i < IT; i++) {
		await_val = (VUINT32_VAL + i) + 1;
		(void)vatomic32_await_neq_add(&g_shared, await_val, 1);
	}
}
static inline void
mt_atomic_u32_await_neq_add_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_inc(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_neq_add_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_neq_add_waiter();
	} else {
		mt_atomic_u32_await_neq_add_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_neq_add(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_neq_add_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	vuint32_t expected =
		(VUINT32_VAL + (MAX_THREADS * IT)) + ((MAX_THREADS / 2) * IT);
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_lt_add
 *****************************************************************************/
static inline void
mt_atomic_u32_await_lt_add_waiter(void)
{
	vuint32_t await_val = 0;
	for (vuint32_t i = 0; i < IT; i++) {
		await_val = (VUINT32_VAL - i) - 1;
		(void)vatomic32_await_lt_add(&g_shared, await_val, 1);
	}
}
static inline void
mt_atomic_u32_await_lt_add_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_dec(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_lt_add_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_lt_add_waiter();
	} else {
		mt_atomic_u32_await_lt_add_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_lt_add(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_lt_add_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	vuint32_t expected =
		(VUINT32_VAL - (MAX_THREADS * IT)) + ((MAX_THREADS / 2) * IT);
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_gt_sub
 *****************************************************************************/
static inline void
mt_atomic_u32_await_gt_sub_waiter(void)
{
	for (vuint32_t i = 0; i < IT; i++) {
		(void)vatomic32_await_gt_sub(&g_shared, VUINT32_VAL + i + 1, 1);
	}
}
static inline void
mt_atomic_u32_await_gt_sub_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_inc(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_gt_sub_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_gt_sub_waiter();
	} else {
		mt_atomic_u32_await_gt_sub_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_gt_sub(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_gt_sub_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	vuint32_t expected =
		(VUINT32_VAL + (MAX_THREADS * IT)) - ((MAX_THREADS / 2) * IT);
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_ge_sub
 *****************************************************************************/
static inline void
mt_atomic_u32_await_ge_sub_waiter(void)
{
	for (vuint32_t i = 0; i < IT; i++) {
		(void)vatomic32_await_ge_sub(&g_shared, VUINT32_VAL + i + 1, 1);
	}
}
static inline void
mt_atomic_u32_await_ge_sub_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_inc(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_ge_sub_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_ge_sub_waiter();
	} else {
		mt_atomic_u32_await_ge_sub_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_ge_sub(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_ge_sub_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	vuint32_t expected =
		(VUINT32_VAL + (MAX_THREADS * IT)) - ((MAX_THREADS / 2) * IT);
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_neq_sub
 *****************************************************************************/
static inline void
mt_atomic_u32_await_neq_sub_waiter(void)
{
	for (vuint32_t i = 0; i < IT; i++) {
		(void)vatomic32_await_neq_sub(&g_shared, VUINT32_VAL + i + 1, 1);
	}
}
static inline void
mt_atomic_u32_await_neq_sub_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_inc(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_neq_sub_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_neq_sub_waiter();
	} else {
		mt_atomic_u32_await_neq_sub_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_neq_sub(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_neq_sub_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	vuint32_t expected =
		(VUINT32_VAL + (MAX_THREADS * IT)) - ((MAX_THREADS / 2) * IT);
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_lt_sub
 *****************************************************************************/
static inline void
mt_atomic_u32_await_lt_sub_waiter(void)
{
	for (vuint32_t i = 0; i < IT; i++) {
		(void)vatomic32_await_lt_sub(&g_shared, VUINT32_VAL - (i - 1), 1);
	}
}
static inline void
mt_atomic_u32_await_lt_sub_writer(void)
{
	for (vuint32_t i = 0; i < V_DOUBLE(IT); i++) {
		vatomic32_dec(&g_shared);
	}
}
static inline void *
mt_atomic_u32_await_lt_sub_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		mt_atomic_u32_await_lt_sub_waiter();
	} else {
		mt_atomic_u32_await_lt_sub_writer();
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_lt_sub(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_lt_sub_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	vuint32_t expected =
		(VUINT32_VAL - (MAX_THREADS * IT)) - ((MAX_THREADS / 2) * IT);
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_gt_set
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_gt_set_run(void *args)
{
	vsize_t tid		   = (vsize_t)(vuintptr_t)args;
	vuint32_t wait_val = VUINT32_VAL + (vuint32_t)tid;
	vuint32_t set_val  = wait_val + 2;
	(void)vatomic32_await_gt_set(&g_shared, wait_val, set_val);
	return NULL;
}
static inline void
mt_atomic_u32_await_gt_set(void)
{
	vuint32_t init_val = VUINT32_VAL + 1;
	vatomic32_init(&g_shared, init_val);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_gt_set_run);
	vuint32_t cur	   = vatomic32_read(&g_shared);
	vuint32_t expected = init_val + (vuint32_t)MAX_THREADS;
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_ge_set
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_ge_set_run(void *args)
{
	vsize_t tid		   = (vsize_t)(vuintptr_t)args;
	vuint32_t wait_val = VUINT32_VAL + (vuint32_t)tid;
	vuint32_t set_val  = wait_val + 1;
	(void)vatomic32_await_ge_set(&g_shared, wait_val, set_val);
	return NULL;
}
static inline void
mt_atomic_u32_await_ge_set(void)
{
	vuint32_t init_val = VUINT32_VAL;
	vatomic32_init(&g_shared, init_val);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_ge_set_run);
	vuint32_t cur	   = vatomic32_read(&g_shared);
	vuint32_t expected = init_val + (vuint32_t)MAX_THREADS;
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_lt_set
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_lt_set_run(void *args)
{
	vsize_t tid		   = (vsize_t)(vuintptr_t)args;
	vuint32_t wait_val = VUINT32_VAL - (vuint32_t)tid;
	vuint32_t set_val  = wait_val - 2;
	(void)vatomic32_await_lt_set(&g_shared, wait_val, set_val);
	return NULL;
}
static inline void
mt_atomic_u32_await_lt_set(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL - 1);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_lt_set_run);
	vuint32_t cur	   = vatomic32_read(&g_shared);
	vuint32_t expected = (VUINT32_VAL - 1) - (vuint32_t)MAX_THREADS;
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_neq_set
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_neq_set_run(void *args)
{
	vsize_t tid = (vsize_t)(vuintptr_t)args;
	if (VIS_EVEN(tid)) {
		(void)vatomic32_await_neq_set(&g_shared, VUINT32_VAL, VUINT32_VAL);
	} else {
		(void)vatomic32_await_neq_set(&g_shared, ~VUINT32_VAL, ~VUINT32_VAL);
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_neq_set(void)
{
	vatomic32_init(&g_shared, VUINT32_VAL);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_neq_set_run);
	vuint32_t cur	   = vatomic32_read(&g_shared);
	vuint32_t expected = VUINT32_VAL;
	ASSERT(cur == expected);
	V_UNUSED(cur, expected);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_mask_eq
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_mask_eq_run(void *args)
{
	vsize_t tid		  = (vsize_t)(vuintptr_t)args;
	vuint32_t mask	  = (vuint32_t)tid;
	vuint32_t new_val = (vuint32_t)(tid + 1);

	vuint32_t cur = vatomic32_await_mask_eq(&g_shared, mask, mask);
	ASSERT(cur == mask);
	vatomic32_write(&g_shared, new_val);
	V_UNUSED(cur);
	return NULL;
}
static inline void
mt_atomic_u32_await_mask_eq(void)
{
	vatomic32_init(&g_shared, 0);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_mask_eq_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	ASSERT(cur == MAX_THREADS);
	V_UNUSED(cur);
}
/*****************************************************************************
 * Multi-thread Test: vatomic32_await_mask_neq
 *****************************************************************************/
static inline void *
mt_atomic_u32_await_mask_neq_run(void *args)
{
	vsize_t tid	   = (vsize_t)(vuintptr_t)args;
	vuint32_t mask = (vuint32_t)tid;

	if (tid == 0) {
		vatomic32_write(&g_shared, 0);
	} else {
		vuint32_t cur = vatomic32_await_mask_neq(&g_shared, mask, mask);
		ASSERT(cur == 0);
		V_UNUSED(cur);
	}
	return NULL;
}
static inline void
mt_atomic_u32_await_mask_neq(void)
{
	vatomic32_init(&g_shared, VUINT32_MAX);
	launch_threads(MAX_THREADS, mt_atomic_u32_await_mask_neq_run);
	vuint32_t cur = vatomic32_read(&g_shared);
	ASSERT(cur == 0);
	V_UNUSED(cur);
}
/*****************************************************************************
 * Entry point
 *****************************************************************************/
int
main(void)
{
	mt_atomic_u32_await_gt();
	mt_atomic_u32_await_ge();
	mt_atomic_u32_await_lt();
	mt_atomic_u32_await_eq();
	mt_atomic_u32_await_neq();

	mt_atomic_u32_await_gt_add();
	mt_atomic_u32_await_ge_add();
	mt_atomic_u32_await_lt_add();
	mt_atomic_u32_await_eq_add();
	mt_atomic_u32_await_neq_add();

	mt_atomic_u32_await_gt_sub();
	mt_atomic_u32_await_ge_sub();
	mt_atomic_u32_await_lt_sub();
	mt_atomic_u32_await_eq_sub();
	mt_atomic_u32_await_neq_sub();

	mt_atomic_u32_await_gt_set();
	mt_atomic_u32_await_ge_set();
	mt_atomic_u32_await_lt_set();
	mt_atomic_u32_await_eq_set();
	mt_atomic_u32_await_neq_set();

	mt_atomic_u32_await_mask_eq();
	mt_atomic_u32_await_mask_neq();

	return 0;
}
