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

spi_slave_t sharp_memlcd_slave = {
    #if SPI_SUPPORT_SCLK_CTL
        (0x00),
    #endif
    SPI_SELECTOR_FUNC,
    {{
        &sharp_memlcd_spi_select, 
        &sharp_memlcd_spi_deselect
    }},
};


#if SHARP_MEMLCD_INTEGRATED_BUILD

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
