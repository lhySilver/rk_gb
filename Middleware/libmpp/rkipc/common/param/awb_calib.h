// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "iniparser.h"

extern dictionary *g_awb_ini_d_;

int rk_awb_calib_get_int(const char *entry, int default_val);
int rk_awb_calib_set_int(const char *entry, int val);
const char *rk_awb_calib_get_string(const char *entry, const char *default_val);
int rk_awb_calib_set_string(const char *entry, const char *val);
int rk_awb_calib_save();
int rk_awb_calib_init(char *ini_path);
int rk_awb_calib_deinit();
int rk_awb_calib_reload();
