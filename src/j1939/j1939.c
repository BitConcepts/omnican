/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — SAE J1939
 * Copyright (c) 2026 BitConcepts, LLC
 */

#include <omnican/j1939.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(omnican_j1939, CONFIG_OMNICAN_LOG_LEVEL);

int omnican_j1939_init(struct omnican_j1939_node *j1939, struct omnican_node *base,
		       const uint8_t name[8])
{
	if (!j1939 || !base || !name) {
		return OMNICAN_ERR_INVAL;
	}

	j1939->base = base;
	j1939->addr = OMNICAN_J1939_ADDR_NULL;
	memcpy(j1939->name, name, 8);

	base->protos |= OMNICAN_PROTO_J1939;
	LOG_INF("J1939 init");
	return OMNICAN_OK;
}

int omnican_j1939_claim_address(struct omnican_j1939_node *j1939, omnican_j1939_addr_t desired_addr,
				uint32_t timeout_ms)
{
	ARG_UNUSED(timeout_ms);

	if (!j1939 || desired_addr > 0xFD) {
		return OMNICAN_ERR_INVAL;
	}

	/* Stub: claim succeeds unconditionally */
	j1939->addr = desired_addr;
	LOG_INF("J1939 address claimed: 0x%02x", desired_addr);
	return OMNICAN_OK;
}

int omnican_j1939_send(struct omnican_j1939_node *j1939, omnican_j1939_pgn_t pgn,
		       omnican_j1939_addr_t dst, const uint8_t *data, size_t len)
{
	ARG_UNUSED(pgn);
	ARG_UNUSED(dst);
	ARG_UNUSED(data);
	ARG_UNUSED(len);

	if (!j1939 || j1939->addr == OMNICAN_J1939_ADDR_NULL) {
		return OMNICAN_ERR_INVAL;
	}

	return OMNICAN_ERR_UNSUPPORTED; /* pending */
}

int omnican_j1939_register_pgn(struct omnican_j1939_node *j1939, omnican_j1939_pgn_t pgn,
			       omnican_j1939_rx_cb_t cb, void *user_data)
{
	ARG_UNUSED(pgn);
	ARG_UNUSED(cb);
	ARG_UNUSED(user_data);

	if (!j1939) {
		return OMNICAN_ERR_INVAL;
	}

	return OMNICAN_ERR_UNSUPPORTED; /* pending */
}
