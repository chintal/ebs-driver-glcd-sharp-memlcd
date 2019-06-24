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


#ifndef DRIVER_SHARP_MEMLCD_H
#define DRIVER_SHARP_MEMLCD_H

#include "config.h"
#include "panels.h"
#include "interface.h"

#include <graphics/image.h>


/**
 * @name Sharp Memory LCD Initialization Functions
 */
/**@{*/ 

/**
 * @brief Initialize the Sharp Memory LCD 
 */
void sharp_memlcd_init(void);
/**@}*/


/**
 * @name Sharp Memory LCD State Machine and associated types
 */
/**@{*/ 

typedef enum SHARP_MEMLCD_STATE_t{
    SHARP_MEMLCD_STATE_PREINIT,
    SHARP_MEMLCD_STATE_IDLE,
    SHARP_MEMLCD_STATE_SENT_ROWID,
    SHARP_MEMLCD_STATE_SENT_ROWDATA,
}sharp_memlcd_state_t;

typedef enum SHARP_MEMLCD_ACTION_TYPE_t{
    SHARP_MEMLCD_ACTION_WRITE_REGION,
    #if SHARP_MEMLCD_IMAGE_WRITER
        SHARP_MEMLCD_ACTION_WRITE_IMAGE,
    #endif
}sharp_memlcd_action_type_t;

typedef struct SHARP_MEMLCD_ACTION_t{
    struct SHARP_MEMLCD_ACTION_t * next;
    struct{
        sharp_memlcd_action_type_t type : 1;
        uint8_t reserved: 7;
    };
    uint8_t row;
    uint8_t * row_data;
    union{
        #if SHARP_MEMLCD_IMAGE_WRITER
            image_t * image;
        #endif
        uint8_t end_condition;
    };
}sharp_memlcd_action_t;

#if SHARP_MEMLCD_MANAGER
extern fifoq_t sharp_memlcd_action_queue;
#endif

typedef struct SHARP_MEMLCD_SM_t{
    sharp_memlcd_state_t state;
    #if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_SW) || (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_CRON)
    _Bool vcom;
    #endif
    sharp_memlcd_action_t * current_action;
    #if SHARP_MEMLCD_MANAGER
    fifoq_t *const action_queue;
    #endif
}sharp_memlcd_sm_t;

extern sharp_memlcd_sm_t sharp_memlcd_sm;


/**
 * @brief Sharp Memory LCD State Machine
 * 
 * This state machine function should get called when there is something to 
 * do. The application would typically not call this directly. The driver
 * library functions will call this function as needed, and it will setup
 * subsequent calls to itself using appropriate callbacks as needed.
 * 
 * The transaction parameter is not used, and exists for signature 
 * compatibility with SPI transaction callbacks.
 * 
 */
void sharp_memlcd_state_machine(spi_transaction_t * transaction);

/**@}*/


/**
 * @name Sharp Memory LCD Low Level Direct Write Functions
 */
/**@{*/ 

/**
 * @brief Write a region to the Sharp Memory LCD
 * 
 * This function can be used to directly write to the Sharp Memory LCD. This 
 * function bypasses the framebuffer entirely and therefore would typically 
 * not be used directly an application which uses the framebuffer and the 
 * graphics primitives from EBS libgraphics.
 * 
 * This function is internally used by the driver to perform the actual LCD
 * write.
 * 
 * If the memlcd transaction manager is not enabled, the caller of this 
 * function must ensure the LCD is in an idle state before calling this 
 * functions.
 */
void sharp_memlcd_write_region(uint8_t row, uint8_t nrows, uint8_t * data);
/**@}*/


#if SHARP_MEMLCD_IMAGE_WRITER
/**
 * @name Sharp Memory LCD Special Direct Write Functions
 */
/**@{*/ 

/**
 * @brief Write an image to the Sharp Memory LCD
 * 
 * This is a convenience function which writes a libgraphics image to the LCD.
 * Since this bypasses the framebuffer, it can only write full screen images.
 * Most applications will probably use the framebuffer's image write functions
 * instead.
 * 
 * This function can be used to directly write to the Sharp Memory LCD. This 
 * function bypasses the framebuffer entirely and therefore would typically 
 * not be used directly an application which uses the framebuffer and the 
 * graphics primitives from EBS libgraphics.
 * 
 * If the memlcd transaction manager is not enabled, the caller of this 
 * function must ensure the LCD is in an idle state before calling this 
 * function.
 */
void sharp_memlcd_write_image(image_t * image);
/**@}*/
#endif


#if SHARP_MEMLCD_FRAMEBUFFER


/**
 * @name Sharp Memory LCD Framebuffer Support Interface
 */
/**@{*/ 


// extern framebuffer_t sharp_memlcd_framebuffer;

/**
 * @brief Write the framebuffer to the Sharp Memory LCD
 * 
 * The framebuffer will generally be manipulated by functions and primitives
 * provided by EBS libgraphics
 */
void sharp_memlcd_write_framebuffer(void);
/**@}*/

#endif


#endif
