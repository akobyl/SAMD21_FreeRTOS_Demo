/**
 * \file
 *
 * \brief DAC functionality implementation.
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include <utils_assert.h>
#include <utils.h>
#include <hal_atomic.h>
#include "hal_dac_os.h"

/**
 * \brief Driver version
 */
#define DAC_RTOS_DRIVER_VERSION 0x00000001u

static void dac_tx_ready(struct _dac_async_device *device, const uint8_t ch);
static void dac_tx_error(struct _dac_async_device *device, const uint8_t ch);

/**
 * \brief Initialize the DAC HAL instance and hardware.
 */
int32_t dac_os_init(struct dac_os_descriptor *const descr, void *const hw)
{
	uint8_t i;
	int32_t rc;

	ASSERT(descr && hw);

	rc = _dac_async_init(&descr->device, hw);
	if (rc) {
		return rc;
	}

	descr->device.dac_cb.tx_ready_cb = dac_tx_ready;
	descr->device.dac_cb.tx_error_cb = dac_tx_error;

	for (i = 0; i < CHANNEL_NUM; i++) {
		descr->sel_ch[i].buffer = NULL;
		descr->sel_ch[i].length = 0;
		descr->error[i]         = ERR_NONE;

		rc = sem_init(&descr->tx_sem[i], 0);
		if (rc < 0) {
			while (i > 0) {
				sem_deinit(&descr->tx_sem[--i]);
			}
			_dac_async_deinit(&descr->device);
			return rc;
		}
	}

	return ERR_NONE;
}

/**
 * \brief Deinitialize the DAC HAL instance and hardware.
 */
int32_t dac_os_deinit(struct dac_os_descriptor *const descr)
{
	uint8_t i;

	ASSERT(descr);

	for (i = 0; i < CHANNEL_NUM; i++) {
		sem_deinit(&descr->tx_sem[i]);
	}

	_dac_async_deinit(&descr->device);

	return ERR_NONE;
}

/**
 * \brief Enable DAC channel.
 */
int32_t dac_os_enable_channel(struct dac_os_descriptor *const descr, const uint8_t ch)
{
	ASSERT(descr && (ch < CHANNEL_NUM));

	_dac_async_enable_channel(&descr->device, ch);
	_dac_async_set_irq_state(&descr->device, DAC_DEVICE_CONVERSION_DONE_CB, true);
	_dac_async_set_irq_state(&descr->device, DAC_DEVICE_ERROR_CB, true);

	return ERR_NONE;
}

/**
 * \brief Disable DAC channel.
 */
int32_t dac_os_disable_channel(struct dac_os_descriptor *const descr, const uint8_t ch)
{
	ASSERT(descr && (ch < CHANNEL_NUM));

	_dac_async_disable_channel(&descr->device, ch);
	_dac_async_set_irq_state(&descr->device, DAC_DEVICE_CONVERSION_DONE_CB, false);
	_dac_async_set_irq_state(&descr->device, DAC_DEVICE_ERROR_CB, false);

	return ERR_NONE;
}

/**
 * \brief DAC convert digital data to analog output.
 */
int32_t dac_os_write(struct dac_os_descriptor *const descr, const uint8_t ch, uint16_t *buffer, uint32_t length)
{
	ASSERT(descr && (ch < CHANNEL_NUM) && buffer && length);

	/* check whether channel is enable */
	if (!_dac_async_is_channel_enable(&descr->device, ch)) {
		return ERR_ABORTED;
	}

	descr->sel_ch[ch].buffer = buffer;
	descr->sel_ch[ch].length = length;

	_dac_async_write_data(&descr->device, *(descr->sel_ch[ch].buffer), ch);

	if (sem_down(&descr->tx_sem[ch], ~0) < 0) {
		return ERR_TIMEOUT;
	}

	if (descr->error[ch]) {
		return descr->error[ch];
	}

	return length;
}

/**
 * \brief Get DAC driver version.
 */
uint32_t dac_os_get_version(void)
{
	return DAC_RTOS_DRIVER_VERSION;
}

/**
 * \internal Process transfer completion
 *
 * \param[in] device The pointer to DAC device instance
 */
static void dac_tx_ready(struct _dac_async_device *device, const uint8_t ch)
{
	struct dac_os_descriptor *const descr = CONTAINER_OF(device, struct dac_os_descriptor, device);

	if (descr->sel_ch[ch].length) {
		descr->sel_ch[ch].length--;
		if (descr->sel_ch[ch].length) {
			descr->sel_ch[ch].buffer++;
			_dac_async_write_data(&descr->device, *(descr->sel_ch[ch].buffer), ch);
		} else {
			sem_up(&descr->tx_sem[ch]);
		}
	}
}

/**
 * \internal Error occurs in transfer process
 *
 * \param[in] device The pointer to DAC device instance
 */
static void dac_tx_error(struct _dac_async_device *device, const uint8_t ch)
{
	struct dac_os_descriptor *const descr = CONTAINER_OF(device, struct dac_os_descriptor, device);

	descr->error[ch] = ERR_ABORTED;
	sem_up(&descr->tx_sem[ch]);
}
