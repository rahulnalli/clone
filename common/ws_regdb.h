/*
 * License: GPLv2
 * Created: 2022-04-21 16:40:04
 * Copyright 2022, Silicon Labs
 * Main authors:
 *    - Jérôme Pouiller <jerome.pouiller@silabs.com>
 */
#ifndef WS_PHY_H
#define WS_PHY_H

#include <stdint.h>
#include <stdbool.h>
#include "stack/mac/channel_list.h"
#include "stack/ws_management_api.h"

struct phy_params {
    uint8_t rail_phy_mode_id;
    uint8_t phy_mode_id;
    uint8_t modulation;
    uint32_t datarate;
    uint8_t op_mode;
    uint8_t fsk_modulation_index;
    uint8_t ofdm_mcs;
    uint8_t ofdm_option;
    bool fec;
};

struct chan_params {
    uint8_t reg_domain;
    uint8_t op_class;           // 0 if not specified by FAN 1.0
    uint8_t regional_reg;
    uint8_t chan_plan_id;       // 0 if not specified by FAN 1.1
    uint32_t chan0_freq;
    uint32_t chan_spacing;
    uint16_t chan_count;
    uint16_t chan_count_valid;  // number of channels after chan_mask applied
    uint8_t valid_phy_modes[8];
    const char *chan_allowed;
};

extern const struct phy_params phy_params_table[];
extern const struct chan_params chan_params_table[];

bool check_phy_chan_compat(const struct phy_params *phy_params, const struct chan_params *chan_params);
const struct phy_params *phy_params_from_mode(int operating_mode);
const struct phy_params *phy_params_from_id(int phy_mode_id);
const struct chan_params *chan_params_fan1_0(int domain, int class);
const struct chan_params *chan_params_fan1_1(int domain, int chan_plan_id);
const struct chan_params *chan_params_universal(int chan0_freq, int chan_spacing, int chan_count_valid);

int chan_spacing_id(int val);
int chan_spacing_value(int id);

#endif