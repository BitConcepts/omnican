/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — Unified multi-protocol CAN stack for Zephyr RTOS
 * Copyright (c) 2026 BitConcepts, LLC
 */

#ifndef OMNICAN_VERSION_H
#define OMNICAN_VERSION_H

#define OMNICAN_VERSION_MAJOR 0
#define OMNICAN_VERSION_MINOR 1
#define OMNICAN_VERSION_PATCH 0

#define OMNICAN_VERSION \
	((OMNICAN_VERSION_MAJOR << 16) | (OMNICAN_VERSION_MINOR << 8) | OMNICAN_VERSION_PATCH)

#define OMNICAN_VERSION_STRING "0.1.0"

#endif /* OMNICAN_VERSION_H */
