/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — Unified multi-protocol CAN stack for Zephyr RTOS
 * Copyright (c) 2026 BitConcepts, LLC
 */

#ifndef OMNICAN_H
#define OMNICAN_H

/**
 * @file omnican.h
 * @brief OmniCAN top-level include — pulls in all enabled protocol headers.
 *
 * Including this header provides access to all OmniCAN APIs enabled via
 * Kconfig (CONFIG_OMNICAN_CANOPEN, CONFIG_OMNICAN_J1939, etc.).
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/can.h>

#include <omnican/version.h>
#include <omnican/core.h>

#if IS_ENABLED(CONFIG_OMNICAN_CANOPEN)
#include <omnican/canopen.h>
#endif

#if IS_ENABLED(CONFIG_OMNICAN_CANOPEN_FD)
#include <omnican/canopen_fd.h>
#endif

#if IS_ENABLED(CONFIG_OMNICAN_J1939)
#include <omnican/j1939.h>
#endif

#if IS_ENABLED(CONFIG_OMNICAN_UDS)
#include <omnican/uds.h>
#endif

#if IS_ENABLED(CONFIG_OMNICAN_OBD2)
#include <omnican/obd2.h>
#endif

#endif /* OMNICAN_H */
