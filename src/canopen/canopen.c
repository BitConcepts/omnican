/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — CANopen CiA 301
 * Copyright (c) 2026 BitConcepts, LLC
 */

#include <omnican/canopen.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(omnican_canopen, CONFIG_OMNICAN_LOG_LEVEL);

int omnican_canopen_init(struct omnican_node *node, uint8_t node_id)
{
	if (!node || node_id == 0 || node_id > 127) {
		return OMNICAN_ERR_INVAL;
	}

	node->protos |= OMNICAN_PROTO_CANOPEN;
	LOG_INF("CANopen init: node_id=0x%02x", node_id);
	return OMNICAN_OK;
}

int omnican_canopen_start(struct omnican_node *node)
{
	if (!node) {
		return OMNICAN_ERR_INVAL;
	}
	LOG_INF("CANopen start");
	return OMNICAN_OK;
}

void omnican_canopen_stop(struct omnican_node *node)
{
	if (!node) {
		return;
	}
	LOG_INF("CANopen stop");
}

void omnican_canopen_process(struct omnican_node *node, uint32_t elapsed_us, uint32_t *next_us)
{
	ARG_UNUSED(node);
	ARG_UNUSED(elapsed_us);

	if (next_us) {
		*next_us = 1000U; /* default: call again in 1 ms */
	}
}
