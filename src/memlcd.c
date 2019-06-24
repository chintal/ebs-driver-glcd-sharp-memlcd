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


#include "memlcd.h"
#include "vcom.h"
#include <stdlib.h>


const spi_slave_t sharp_memlcd_slave = {
    #if APP_SUPPORT_SPI_CTL
    {{
        SPI_CLKPHA_CHG_CAP, SPI_CLKPOL_AL, SPI_DATA_LSBFIRST, SPI_DATA_8BIT,
        (uint8_t)(SHARP_MEMLCD_SPI_BASE_FREQ / (SHARP_MEMLCD_SPI_CLK_FREQ * 2))
    }},
    #endif
    SPI_SELECTOR_FUNC,
    {{
        &sharp_memlcd_spi_select, 
        &sharp_memlcd_spi_deselect
    }},
};


#if SHARP_MEMLCD_MANAGER
fifoq_t sharp_memlcd_action_queue = {NULL, NULL};
#else
sharp_memlcd_action_t current_action;
#endif

sharp_memlcd_sm_t sharp_memlcd_sm = {
    SHARP_MEMLCD_STATE_PREINIT, 
    #if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_SW) || (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_CRON)
    0,
    #endif
    #if SHARP_MEMLCD_MANAGER
    NULL,
    &sharp_memlcd_action_queue
    #else
    &current_action
    #endif
};

uint8_t _sharp_memlcd_cmd_buffer[2];
spi_transaction_t _sharp_memlcd_spi_tr;


#if SHARP_MEMLCD_INTEGRATED_BUILD

void sharp_memlcd_interface_init(void){
    spi_init_slave(SHARP_MEMLCD_SPI_INTFNUM, &sharp_memlcd_slave);
    gpio_conf_output(SHARP_MEMLCD_SPI_CS_PORT, SHARP_MEMLCD_SPI_CS_PIN);
    sharp_memlcd_spi_deselect();
    gpio_conf_output(SHARP_MEMLCD_ENABLE_PORT, SHARP_MEMLCD_ENABLE_PIN);
    sharp_memlcd_disable();
}

void sharp_memlcd_spi_select(void){
    gpio_set_output_low(SHARP_MEMLCD_SPI_CS_PORT, SHARP_MEMLCD_SPI_CS_PIN);
}

void sharp_memlcd_spi_deselect(void){
    gpio_set_output_high(SHARP_MEMLCD_SPI_CS_PORT, SHARP_MEMLCD_SPI_CS_PIN);
}

#endif

void sharp_memlcd_init(void){
    sharp_memlcd_interface_init();
    _sharp_memlcd_spi_tr.slave = NULL;
    _sharp_memlcd_spi_tr.rxlen = 0;
    _sharp_memlcd_spi_tr.rxdata = 0;
    _sharp_memlcd_spi_tr.callback = &sharp_memlcd_state_machine;
    sharp_memlcd_vcom_init();
    sharp_memlcd_sm.state = SHARP_MEMLCD_STATE_IDLE;
    sharp_memlcd_enable();
}

static void _sharp_memlcd_trigger_action(void){
    sharp_memlcd_spi_select();
    // Send Row ID
    _sharp_memlcd_spi_tr.txlen = 0x02;
    #if SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_SW
    if (sharp_memlcd_sm.vcom){
        _sharp_memlcd_cmd_buffer[0] = 0x03;
    }
    else{
        _sharp_memlcd_cmd_buffer[0] = 0x01;
    }
    #else
        _sharp_memlcd_cmd_buffer[0] = 0x01;
    #endif
    _sharp_memlcd_cmd_buffer[1] = sharp_memlcd_sm.current_action->row;
    sharp_memlcd_sm.current_action->row ++;
    _sharp_memlcd_spi_tr.txdata = (uint8_t *)(&_sharp_memlcd_cmd_buffer);
    sharp_memlcd_spi_enqueue_transaction(&_sharp_memlcd_spi_tr);
    sharp_memlcd_sm.state = SHARP_MEMLCD_STATE_SENT_ROWID;
}

void sharp_memlcd_state_machine(__attribute__((unused)) spi_transaction_t * transaction){
    switch(sharp_memlcd_sm.state){
        case SHARP_MEMLCD_STATE_PREINIT:
            return;
        case SHARP_MEMLCD_STATE_IDLE:
            #if SHARP_MEMLCD_MANAGER
            if (!fifoq_empty(sharp_memlcd_sm.action_queue)){
                sharp_memlcd_sm.current_action = (sharp_memlcd_action_t * )(fifoq_pop_next(sharp_memlcd_sm.action_queue));
                _sharp_memlcd_trigger_action();
            }
            #endif
            break;
        case SHARP_MEMLCD_STATE_SENT_ROWID:
            _sharp_memlcd_spi_tr.txlen = SHARP_MEMLCD_NCOLS / 8;
            
            #if SHARP_MEMLCD_IMAGE_WRITER
            switch(sharp_memlcd_sm.current_action->type){
                case SHARP_MEMLCD_ACTION_WRITE_REGION:
            #endif
                    _sharp_memlcd_spi_tr.txdata = sharp_memlcd_sm.current_action->row_data;
                    sharp_memlcd_sm.current_action->row_data += SHARP_MEMLCD_NCOLS / 8;
            #if SHARP_MEMLCD_IMAGE_WRITER
                    break;
                case SHARP_MEMLCD_ACTION_WRITE_IMAGE:
                    break;
            }
            #endif
            sharp_memlcd_spi_enqueue_transaction(&_sharp_memlcd_spi_tr);
            sharp_memlcd_sm.state = SHARP_MEMLCD_STATE_SENT_ROWDATA;
            break;
        case SHARP_MEMLCD_STATE_SENT_ROWDATA:
            sharp_memlcd_spi_deselect();
            #if SHARP_MEMLCD_IMAGE_WRITER
            switch(sharp_memlcd_sm.current_action->type){
                case SHARP_MEMLCD_ACTION_WRITE_REGION:
            #endif
                    if(sharp_memlcd_sm.current_action->end_condition > sharp_memlcd_sm.current_action->row){
                        _sharp_memlcd_trigger_action();
                    }
                    else{
                        #if SHARP_MEMLCD_MANAGER
                        free(sharp_memlcd_sm.current_action);
                        #endif
                        sharp_memlcd_sm.state = SHARP_MEMLCD_STATE_IDLE;
                    }
            #if SHARP_MEMLCD_IMAGE_WRITER
                    break;
                case SHARP_MEMLCD_ACTION_WRITE_IMAGE:
                    // Close out Image Write
                    break;
            }
            #endif
            break;
    };
}

void sharp_memlcd_write_region(uint8_t row, uint8_t nrows, uint8_t * data){
    #if SHARP_MEMLCD_MANAGER
        sharp_memlcd_action_t * action;
        action = (sharp_memlcd_action_t *) malloc(sizeof(sharp_memlcd_action_t));
        action->type = SHARP_MEMLCD_ACTION_WRITE_REGION;
        action->row = row;
        action->row_data = data;
        action->end_condition = row + nrows - 1;
        fifoq_enqueue(sharp_memlcd_sm.action_queue, (_fifoq_item_stub_t *)(action));
        if (sharp_memlcd_sm.state == SHARP_MEMLCD_STATE_IDLE){
            sharp_memlcd_state_machine(NULL);
        }
    #else
        sharp_memlcd_sm.current_action->type = SHARP_MEMLCD_ACTION_WRITE_REGION;
        sharp_memlcd_sm.current_action->row = row;
        sharp_memlcd_sm.current_action->row_data = data;
        sharp_memlcd_sm.current_action->end_condition = row + nrows - 1;
        sharp_memlcd_state_machine(NULL);
    #endif
}

#if SHARP_MEMLCD_IMAGE_WRITER
void sharp_memlcd_write_image(image_t * image){
    ;
}
#endif

#if SHARP_MEMLCD_FRAMEBUFFER

void sharp_memlcd_write_framebuffer(void){
    ;
}

#endif
