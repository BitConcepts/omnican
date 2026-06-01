/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — SAE J1979 / OBD-II
 * Copyright (c) 2026 BitConcepts, LLC
 */

#include <omnican/obd2.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(omnican_obd2, CONFIG_OMNICAN_LOG_LEVEL);

int omnican_obd2_client_init(struct omnican_obd2_client *client, struct omnican_node *base)
{
	if (!client || !base) {
		return OMNICAN_ERR_INVAL;
	}

	base->protos |= OMNICAN_PROTO_OBD2;
	LOG_INF("OBD-II client init");
	return OMNICAN_OK;
}

int omnican_obd2_request_pid(struct omnican_obd2_client *client, uint8_t mode, uint8_t pid,
			     omnican_obd2_response_cb_t cb, void *user_data, uint32_t timeout_ms)
{
	ARG_UNUSED(mode);
	ARG_UNUSED(pid);
	ARG_UNUSED(cb);
	ARG_UNUSED(user_data);
	ARG_UNUSED(timeout_ms);

	if (!client) {
		return OMNICAN_ERR_INVAL;
	}

	return OMNICAN_ERR_UNSUPPORTED; /* pending */
}
