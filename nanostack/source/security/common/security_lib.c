/*
 * Copyright (c) 2013-2019, 2021, Pelion and affiliates.
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
#include "ns_types.h"
#include "ns_trace.h"
#include "string.h"
#include "eventOS_event.h"
#include "nsdynmemLIB.h"
#include "core/include/ns_socket.h"
#include "nwk_interface/protocol.h"
#include "shalib.h"
#include "randLIB.h"
#ifdef ECC
#include "libX509_V3.h"
#include "ecc.h"
#endif
#include "security/tls/tls_lib.h"
#include "security/tls/tls_ccm_crypt.h"
#include "security/common/sec_lib.h"
#include "net_nvm_api.h"
#include "security/pana/pana_nvm.h"
#include "security/pana/pana.h"
#include "security/pana/eap_protocol.h"
#include "security/pana/pana_internal_api.h"
#include "common_functions.h"



//************************ECC Certificates end
