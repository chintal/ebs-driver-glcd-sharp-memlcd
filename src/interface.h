/* 
   Copyright (c)
     (c) 2019 Chintalagiri Shashank, Quazar Technologies Pvt. Ltd.
   
   This file is part of
   Embedded bootstraps : Sharp Memory LCD driver for EBS applications
   
   This library is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef DRIVER_SHARP_MEMLCD_INTERFACE_H
#define DRIVER_SHARP_MEMLCD_INTERFACE_H 

#include "config.h"

#if SHARP_MEMLCD_INTEGRATED_BUILD
    #include "hal/uc/gpio.h"
    #include "hal/uc/spi.h"
#else
    // TODO Work out how to include SPI primitives when 
    //      there is no hal/uc include possible
    #include "hal/uc/spi.h"
#endif

/**
 * @brief Initialize whatever is needed to interface with the hardware.
 * 
 * The following initializations are probably required : 
 * 
 *   - SPI Master initialization
 *   - SPI Slave initialization for sharp_memlcd_slave
 *      - GPIO initilaization for SPI CS
 *   - GPIO initialization for enable (Output, Low)
 *   - GPIO initialization for cominv (Output, Low)
 * 
 * Integrated builds will use a version of this function provided within this
 * library which relies on EBS HAL APIs. The application will be expected
 * to perform SPI Master initialization, and nothing else.
 * 
 * The sharp_memlcd_interface_init() function must exist at application link 
 * time for isolated library builds. It will be called by the driver during 
 * it's own initialization, and should typically contain code for 
 * initialization of most of these elements.
 * 
 * It is upto the application to decide which initialization elements should 
 * be executed in this way and which will be managed by other parts of the 
 * application.
 * 
 * If the application chooses to manage any of these in another way, it 
 * should generally be done before the library's primary initialization 
 * function is called. 
 */
extern void sharp_memlcd_interface_init(void);


/**
 * @brief Enqueue an SPI transaction for the Sharp Memory LCD slave
 *  
 * This function should be able to accept an spi_transaction_t and execute 
 * the SPI transaction. The definition of an SPI transaction is as per the 
 * SPI Transaction Architecture of EBS (as yet not separately documented).
 * 
 * The sharp_memlcd_spi_enqueue_transaction() function must exist at 
 * application link time for isolated library builds.
 * 
 * Integrated builds will use an inlined version of this function provided
 * by this file which rely on EBS HAL APIs.
 */
SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_spi_enqueue_transaction(spi_transaction_t * transaction);

/**
 * @brief Select the Sharp Memory LCD slave
 * 
 * This function should do whatever is needed to select the slave. Typically, 
 * this would pull the appropriate GPIO (SHARP_MEMLCD_SPI_CS) to GND. 
 * 
 * The sharp_memlcd_spi_select() function must exist at application link time 
 * for isolated library builds. This function will be provided by the driver 
 * library for integrated builds by using EBS HAL APIs.
 */
extern void sharp_memlcd_spi_select(void);

/**
 * @brief Deselect the Sharp Memory LCD slave
 * 
 * This function should do whatever is needed to deselect the slave. Typically, 
 * this would pull the appropriate GPIO (SHARP_MEMLCD_SPI_CS) to VCC. 
 * 
 * The sharp_memlcd_spi_deselect() function must exist at application 
 * link time for isolated library builds. This function will be provided 
 * by the driver library for integrated builds by using EBS HAL APIs.
 */
extern void sharp_memlcd_spi_deselect(void);

/**
 * @brief Enable the Sharp Memory LCD
 *  
 * This function should do whatever is needed to enable the display. Typically, 
 * this would pull the appropriate GPIO (SHARP_MEMLCD_ENABLE) to VCC. 
 * 
 * The sharp_memlcd_enable() function must exist at application link time 
 * for isolated library builds. 
 * 
 * Integrated builds will use an inlined version of this function provided
 * by this file which rely on EBS HAL APIs.
 */
SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_enable(void);

/**
 * @brief Disable the Sharp Memory LCD
 *  
 * This function should do whatever is needed to disable the display. Typically, 
 * this would pull the appropriate GPIO (SHARP_MEMLCD_ENABLE) to GND. 
 * 
 * The sharp_memlcd_disable() function must exist at application link time 
 * for isolated library builds. 
 * 
 * Integrated builds will use an inlined version of this function provided
 * by this file which rely on EBS HAL APIs.
 */
SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_disable(void);

/**
 * @brief Set the Sharp Memory LCD VCOM to 1
 *  
 * This function should set the cominv signal high. Typically, this would 
 * pull the appropriate GPIO (SHARP_MEMLCD_COMINV) to VCC. 
 * 
 * The sharp_memlcd_cominv_high() function must exist at application link time 
 * for isolated library builds. 
 * 
 * Integrated builds will use an inlined version of this function provided
 * by this file which rely on EBS HAL APIs.
 */
SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_cominv_high(void);

/**
 * @brief Set the Sharp Memory LCD VCOM to 0
 *  
 * This function should set the cominv signal low. Typically, this would 
 * pull the appropriate GPIO (SHARP_MEMLCD_COMINV) to GND. 
 * 
 * The sharp_memlcd_cominv_low() function must exist at application link time 
 * for isolated library builds. 
 * 
 * Integrated builds will use an inlined version of this function provided
 * by this file which rely on EBS HAL APIs.
 */
SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_cominv_low(void);

/**
 * @brief Sharp Memory LCD SPI Slave Definition.
 *  
 * This struct will be provided by the driver library using the 
 * sharp_memlcd_enable() and sharp_memlcd_disable() functions declared here.
 * 
 * The application may choose to use this if required, though there is 
 * likely not going to be any need to do so. The slave definition will 
 * generally be provided along with each SPI transaction.
 */
extern spi_slave_t sharp_memlcd_slave;


#if SHARP_MEMLCD_INTEGRATED_BUILD

/*
 * For EBS application integrated builds, the application should define the 
 * following constants :
 * 
 *   - SHARP_MEMLCD_SPI_INTFNUM
 *   - SHARP_MEMLCD_SPI_CS_PORT
 *   - SHARP_MEMLCD_SPI_CS_PIN
 *   - SHARP_MEMLCD_ENABLE_PORT
 *   - SHARP_MEMLCD_ENABLE_PIN
 *   - SHARP_MEMLCD_COMINV_PORT
 *   - SHARP_MEMLCD_COMINV_PIN
 * 
 * These constants will typically be defined in `asp/hardware.h` or 
 * `asp/bsp/board.h`, depending on the architecture of the application and 
 * of the hardware.
*/

SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_spi_enqueue_transaction(spi_transaction_t * transaction){
    spi_enqueue_transaction(SHARP_MEMLCD_SPI_INTFNUM, transaction);
}

SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_enable(void){
    gpio_set_output_high(SHARP_MEMLCD_ENABLE_PORT, SHARP_MEMLCD_ENABLE_PIN);
}

SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_disable(void){
    gpio_set_output_low(SHARP_MEMLCD_ENABLE_PORT, SHARP_MEMLCD_ENABLE_PIN);
}

SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_cominv_high(void){
    gpio_set_output_high(SHARP_MEMLCD_COMINV_PORT, SHARP_MEMLCD_COMINV_PIN);
}

SHARP_MEMLCD_INTERFACE_FUNC void sharp_memlcd_cominv_low(void){
    gpio_set_output_low(SHARP_MEMLCD_COMINV_PORT, SHARP_MEMLCD_COMINV_PIN);
}

#endif

#endif
