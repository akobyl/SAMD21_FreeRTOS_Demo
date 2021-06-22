/**
 * \file
 *
 * \brief DAC functionality declaration.
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

#ifndef HAL_DAC_OS_H_INCLUDED
#define HAL_DAC_OS_H_INCLUDED

#include <hal_rtos.h>
#include <hpl_dac_async.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_dac_os
 *
 *@{
 */

/**
 * \brief DAC descriptor
 *
 * The DAC descriptor forward declaration.
 */
struct dac_os_descriptor;

/**
 * \brief DAC RTOS channel descriptor.
 */
struct dac_os_channel {
	uint16_t *buffer; /** Pointer to buffer what to be converted */
	uint32_t  length; /** The length of buffer */
};

/**
 * \brief DAC RTOS descriptor
 */
struct dac_os_descriptor {
	struct _dac_async_device device;              /** DAC device */
	struct dac_os_channel    sel_ch[CHANNEL_NUM]; /** DAC channel handlers */
	sem_t                    tx_sem[CHANNEL_NUM]; /** DAC channel write data semaphore */
	int32_t                  error[CHANNEL_NUM];  /** DAC channel convert status */
};

/** \brief Initialize the DAC HAL instance and hardware.
 *
 *  \param[out] descr A DAC descriptor to initialize
 *  \param[in] hw The pointer to hardware instance
 *
 *  \return Operation status.
 *  \retval <0       The passed parameters were invalid or a DAC is already initialized
 *  \retval ERR_NONE The initialization has completed successfully
 */
int32_t dac_os_init(struct dac_os_descriptor *const descr, void *const hw);

/** \brief Deinitialize the DAC HAL instance and hardware.
 *
 *  \param[in] descr Pointer to the HAL DAC descriptor
 *
 *  \return ERR_NONE
 */
int32_t dac_os_deinit(struct dac_os_descriptor *const descr);

/** \brief Enable DAC channel.
 *
 *  \param[in] descr Pointer to the HAL DAC descriptor
 *  \param[in] ch Channel number
 *
 *  \return ERR_NONE
 */
int32_t dac_os_enable_channel(struct dac_os_descriptor *const descr, const uint8_t ch);

/** \brief Disable DAC channel.
 *
 *  \param[in] descr Pointer to the HAL DAC descriptor
 *  \param[in] ch Channel number
 *
 *  \return ERR_NONE
 */
int32_t dac_os_disable_channel(struct dac_os_descriptor *const descr, const uint8_t ch);

/** \brief The DAC convert digital data to analog output.
 *
 *  It blocks the task/thread until the conversation is done.
 *  The buffer sample should be 16-bit wide. 8-bit data will not be supported.
 *  You must convert to 16-bit if there are 8-bit samples.
 *
 *  \param[in] descr   Pointer to the HAL DAC descriptor
 *  \param[in] ch      The channel selected to output
 *  \param[in] buffer  Pointer to digital data or buffer
 *  \param[in] length  The number of elements in buffer
 *
 *  \return Operation status.
 *  \retval <0 Error code
 *  \retval length Convert success
 */
int32_t dac_os_write(struct dac_os_descriptor *const descr, const uint8_t ch, uint16_t *buffer, uint32_t length);

/** \brief Get DAC driver version.
 *
 *  \return Current driver version.
 */
uint32_t dac_os_get_version(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* HAL_DAC_OS_H_INCLUDED */
