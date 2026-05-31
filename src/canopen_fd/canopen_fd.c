/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — CANopen FD CiA 1301
 * Copyright (c) 2026 BitConcepts, LLC
 */

#include <omnican/core.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(omnican_canopen_fd, CONFIG_OMNICAN_LOG_LEVEL);

/* CANopen FD extends the base CANopen module (CiA 1301).
 * Implementation pending — wire in after core CANopen is complete.
 */
