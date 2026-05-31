/* SPDX-License-Identifier: Apache-2.0
 * OmniCAN — Core frame router
 * Copyright (c) 2026 BitConcepts, LLC
 */

#include <omnican/core.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(omnican_core, CONFIG_OMNICAN_LOG_LEVEL);

int omnican_node_init(struct omnican_node *node,
		      const struct device *can_dev,
		      uint32_t bitrate,
		      bool fd_mode)
{
	if (!node || !can_dev) {
		return OMNICAN_ERR_INVAL;
	}

	if (!device_is_ready(can_dev)) {
		LOG_ERR("CAN device not ready");
		return OMNICAN_ERR_NODEV;
	}

	node->can_dev = can_dev;
	node->bitrate = bitrate;
	node->fd_mode = fd_mode;
	node->protos  = OMNICAN_PROTO_NONE;

	LOG_INF("OmniCAN node initialised: bitrate=%u kbps, fd=%d",
		bitrate, (int)fd_mode);

	return OMNICAN_OK;
}
