/*
 * Copyright (c) 2014-2019, Pelion and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "nsconfig.h"
#include <stdint.h>
#include "common/log.h"
#include "common/hal_interrupt.h"
#include "nwk_interface/protocol_timer.h"

#define PROTOCOL_TIMER_PERIOD_MS 50

protocol_timer_t protocol_timer[PROTOCOL_TIMER_MAX];

// time is in milliseconds
void protocol_timer_start(protocol_timer_id_e id, void (*passed_fptr)(uint16_t), uint32_t time)
{
    BUG_ON(!passed_fptr);
    BUG_ON(time % PROTOCOL_TIMER_PERIOD_MS);

    time /= PROTOCOL_TIMER_PERIOD_MS;
    platform_enter_critical();
    protocol_timer[id].ticks = (uint16_t)time;
    protocol_timer[id].orderedTime = (uint16_t)time;
    if (time > 1 && protocol_timer[id].time_drifts >= 50) {
        protocol_timer[id].ticks--;
        protocol_timer[id].time_drifts -= 50;
    }
    protocol_timer[id].fptr = passed_fptr;
    platform_exit_critical();
}

void protocol_timer_stop(protocol_timer_id_e id)
{
    platform_enter_critical();
    protocol_timer[id].ticks = 0;
    protocol_timer[id].orderedTime = 0;
    platform_exit_critical();
}

void protocol_timer_cb(uint16_t ticks)
{
    uint8_t i;
    uint16_t tick_update, tempTimer;
    for (i = 0; i < PROTOCOL_TIMER_MAX; i++) {
        if (protocol_timer[i].ticks) {
            if (protocol_timer[i].ticks <= ticks) {
                tempTimer = (ticks - protocol_timer[i].ticks);

                tick_update = 1;
                if (protocol_timer[i].time_drifts >= 50) {
                    tempTimer++;
                    protocol_timer[i].time_drifts -= 50;
                }

                if (tempTimer >= protocol_timer[i].orderedTime) {
                    tick_update += (tempTimer / protocol_timer[i].orderedTime);
                    protocol_timer[i].time_drifts += ((tempTimer % protocol_timer[i].orderedTime) * 50);
                }

                protocol_timer[i].ticks = 0;
                protocol_timer[i].orderedTime = 0;
                protocol_timer[i].fptr(tick_update);

            } else {
                protocol_timer[i].ticks -= ticks;
            }
        }
    }
}
