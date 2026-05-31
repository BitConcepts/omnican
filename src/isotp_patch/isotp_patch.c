/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — Zephyr ISOTP issue #86025 workaround
 * Copyright (c) 2026 BitConcepts, LLC
 *
 * Workaround for Zephyr issue #86025:
 * The Zephyr ISO-TP layer does not allow binding and transmitting on the
 * same CAN ID within the same context. This is a required pattern for
 * standard UDS physical addressing (e.g. tester at 0x7E0, ECU at 0x7E8).
 *
 * Patch: use separate send/recv CAN sockets with a forwarding shim.
 */

#include <omnican/core.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(omnican_isotp_patch, CONFIG_OMNICAN_LOG_LEVEL);

/* Patch implementation pending — depends on Zephyr upstream resolution. */
