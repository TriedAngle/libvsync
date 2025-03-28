/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 * SPDX-License-Identifier: MIT
 */

#ifndef VSYNC_TEST_CASE_H
#define VSYNC_TEST_CASE_H
/* range of keys to preinsert */
#define INIT_K1 0
#define INIT_K2 0

/* Preinsert keys \in [0, 0]
 * goal: SC of add
 */
int msg                 = 0;
const vsize_t g_lst_idx = 0;
void
pre(void)
{
    for (vlistset_key_t k = INIT_K1; k <= INIT_K2; k++) {
        lst_add(MAIN_TID, g_lst_idx, k);
    }
}
void
t0(vsize_t tid)
{
    vbool_t success = false;
    success         = lst_add(tid, g_lst_idx, 2);
    ASSERT(success);
    msg     = 1;
    success = lst_add(tid, g_lst_idx, 3);
    ASSERT(success);
}
void
t1(vsize_t tid)
{
    if (lst_rem(tid, g_lst_idx, 3)) {
        ASSERT(lst_con(tid, g_lst_idx, 2));
        ASSERT(msg);
    } else {
        verification_ignore();
    }
}
void
t2(vsize_t tid)
{
    V_UNUSED(tid);
}
void
post(void)
{
    lst_verify_traces(g_lst_idx);
}
#endif
