/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

#pragma once

#include "bt_target.h"
#include "bt_types.h"

static const char GKI_MODULE[] = "gki_module";

/* Error codes */
#define GKI_SUCCESS         0x00
#define GKI_FAILURE         0x01
#define GKI_INVALID_TASK    0xF0
#define GKI_INVALID_POOL    0xFF


/************************************************************************
** Mailbox definitions. Each task has 4 mailboxes that are used to
** send buffers to the task.
*/
#define TASK_MBOX_0    0
#define TASK_MBOX_1    1
#define TASK_MBOX_2    2
#define TASK_MBOX_3    3

#define NUM_TASK_MBOX  4

/************************************************************************
** Event definitions.
**
** There are 4 reserved events used to signal messages rcvd in task mailboxes.
** There are 4 reserved events used to signal timeout events.
** There are 8 general purpose events available for applications.
*/
#define MAX_EVENTS              16

#define TASK_MBOX_0_EVT_MASK   0x0001
#define TASK_MBOX_1_EVT_MASK   0x0002
#define TASK_MBOX_2_EVT_MASK   0x0004
#define TASK_MBOX_3_EVT_MASK   0x0008

#define TIMER_0_EVT_MASK    0x0010
#define TIMER_2_EVT_MASK    0x0040

#define APPL_EVT_0          8
#define APPL_EVT_1          9
#define APPL_EVT_2          10
#define APPL_EVT_3          11
#define APPL_EVT_4          12
#define APPL_EVT_5          13
#define APPL_EVT_6          14
#define APPL_EVT_7          15

#define EVENT_MASK(evt)       ((UINT16)(0x0001 << (evt)))

/* Timer list entry callback type
*/
typedef void (TIMER_CBACK)(void *p_tle);
#ifndef TIMER_PARAM_TYPE
#define TIMER_PARAM_TYPE    UINT32
#endif
/* Define a timer list entry
*/
typedef struct _tle
{
    struct _tle  *p_next;
    struct _tle  *p_prev;
    TIMER_CBACK  *p_cback;
    INT32         ticks;
    INT32         ticks_initial;
    TIMER_PARAM_TYPE   param;
    TIMER_PARAM_TYPE   data;
    UINT16        event;
    UINT8         in_use;
} TIMER_LIST_ENT;

/* Define a timer list queue
*/
typedef struct
{
    TIMER_LIST_ENT   *p_first;
    TIMER_LIST_ENT   *p_last;
} TIMER_LIST_Q;


/***********************************************************************
** This queue is a general purpose buffer queue, for application use.
*/
typedef struct
{
    void    *_p_first;
    void    *_p_last;
    UINT16   _count;
} BUFFER_Q;

#define GKI_IS_QUEUE_EMPTY(p_q) ((p_q)->count == 0)

/* Task constants
*/
typedef void (*TASKPTR)(void);

#define GKI_PUBLIC_POOL         0       /* General pool accessible to GKI_getbuf() */
#define GKI_RESTRICTED_POOL     1       /* Inaccessible pool to GKI_getbuf() */

/***********************************************************************
** Function prototypes
*/

/* Task management
*/
UINT8   GKI_get_taskid(void);

/* To send buffers and events between tasks
*/
UINT8   GKI_send_event (UINT8, UINT16);


/* To get and release buffers, change owner and get size
*/
void    GKI_freebuf (void *);
void   *GKI_getbuf (UINT16);
UINT16  GKI_get_buf_size (void *);
void   *GKI_getpoolbuf (UINT8);
UINT16  GKI_poolcount (UINT8);
UINT16  GKI_poolfreecount (UINT8);
UINT16  GKI_poolutilization (UINT8);


/* User buffer queue management
*/
void   *GKI_dequeue  (BUFFER_Q *);
void    GKI_enqueue (BUFFER_Q *, void *);
void    GKI_enqueue_head (BUFFER_Q *, void *);
void   *GKI_getfirst (BUFFER_Q *);
void   *GKI_getlast (BUFFER_Q *);
void   *GKI_getnext (void *);
void    GKI_init_q (BUFFER_Q *);
UINT16  GKI_queue_length(BUFFER_Q *);
BOOLEAN GKI_queue_is_empty(BUFFER_Q *);
void   *GKI_remove_from_queue (BUFFER_Q *, void *);
UINT16  GKI_get_pool_bufsize (UINT8);

/* Timer management
*/
void    GKI_delay(UINT32);
UINT16  GKI_wait(UINT16, UINT32);
extern INT32 GKI_timer_ticks_getinitial(const TIMER_LIST_ENT *tle);

/* Disable Interrupts, Enable Interrupts
*/
void    GKI_enable(void);
void    GKI_disable(void);

/* os timer operation */
UINT32 GKI_get_os_tick_count(void);

/* Exception handling
*/
void    GKI_exception (UINT16, char *);
