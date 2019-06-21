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


#if SHARP_MEMLCD_INTEGRATED_BUILD

void sharp_memlcd_interface_init(void){
    spi_init_slave(SHARP_MEMLCD_SPI_INTFNUM, &sharp_memlcd_slave);
    gpio_conf_output(SHARP_MEMLCD_SPI_CS_PORT, SHARP_MEMLCD_SPI_CS_PIN);
    sharp_memlcd_spi_deselect();
    gpio_conf_output(SHARP_MEMLCD_ENABLE_PORT, SHARP_MEMLCD_ENABLE_PIN);
    sharp_memlcd_disable();
    gpio_conf_output(SHARP_MEMLCD_COMINV_PORT, SHARP_MEMLCD_COMINV_PIN);
    sharp_memlcd_cominv_low();
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
}

void sharp_memlcd_vcom_toggle(void){
    ;
}

void sharp_memlcd_write_row(uint8_t row, uint8_t * data){
    ;
}

void sharp_memlcd_write_region(uint8_t * row, uint8_t nrows, uint8_t * data){
    ;
}

void sharp_memlcd_write_image(image_t * image){
    ;
}

#if SHARP_MEMLCD_FRAMEBUFFER

void sharp_memlcd_write_framebuffer(void){
    ;
}

#endif
