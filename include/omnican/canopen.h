/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — CANopen (CiA 301) protocol module
 * Copyright (c) 2026 BitConcepts, LLC
 */

#ifndef OMNICAN_CANOPEN_H
#define OMNICAN_CANOPEN_H

#include <omnican/core.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup omnican_canopen OmniCAN CANopen (CiA 301)
 * @brief CANopen protocol — NMT, SDO, PDO, Emergency, Heartbeat.
 *
 * This module is the CANopen implementation extracted and rearchitected
 * from the BitConcepts/CANopenNode fork, incorporating all fixes from
 * 9 iterative audit rounds. It targets Zephyr-native operation without
 * depending on upstream CANopenNode conventions.
 *
 * @{
 */

/* --- Forward declarations (implementation in src/canopen/) --- */
struct omnican_canopen_ctx;

/**
 * @brief Initialize a CANopen node.
 *
 * @param node     OmniCAN base node context (already initialised).
 * @param node_id  CANopen node ID (1–127).
 *
 * @retval OMNICAN_OK on success.
 */
int omnican_canopen_init(struct omnican_node *node, uint8_t node_id);

/**
 * @brief Start the CANopen stack (enter NMT Pre-Operational → Operational).
 */
int omnican_canopen_start(struct omnican_node *node);

/**
 * @brief Stop the CANopen stack gracefully.
 */
void omnican_canopen_stop(struct omnican_node *node);

/**
 * @brief Process CANopen housekeeping (call periodically from main loop
 *        or a dedicated Zephyr thread).
 *
 * @param node          OmniCAN node.
 * @param elapsed_us    Microseconds elapsed since last call.
 * @param next_us       Output: recommended delay until next call.
 */
void omnican_canopen_process(struct omnican_node *node,
			     uint32_t elapsed_us,
			     uint32_t *next_us);

/** @} */ /* omnican_canopen */

#ifdef __cplusplus
}
#endif

#endif /* OMNICAN_CANOPEN_H */
