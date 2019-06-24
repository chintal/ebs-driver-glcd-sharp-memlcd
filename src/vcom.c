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
#include <time/cron.h>
#if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_TIMER)
    #include "hal/uc/timer.h"
#endif


#if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_CRON) || (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_SW)
// TODO This should be made const. That will need some tweaks to libtime.
tm_sdelta_t _sharp_memlcd_vcom_period = {100, 0, 0}; 
cron_job_t _sharp_memlcd_vcom_service;
#endif


void sharp_memlcd_vcom_init(void){
    #if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_CRON)
    gpio_conf_output(SHARP_MEMLCD_COMINV_PORT, SHARP_MEMLCD_COMINV_PIN);
    sharp_memlcd_cominv_low();
    #endif
    #if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_CRON) || (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_SW)
    sharp_memlcd_sm.vcom = 0;
    tm_cron_create_job_rel(
        &_sharp_memlcd_vcom_service, &sharp_memlcd_vcom_toggle, 
        &_sharp_memlcd_vcom_period, &_sharp_memlcd_vcom_period
    );
    #endif
    #if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_TIMER)
    timer_init(SHARP_MEMLCD_COMINV_INTFNUM);
    timer_set_prescaler(SHARP_MEMLCD_COMINV_INTFNUM, SHARP_MEMLCD_COMINV_CLK_PRESCALER_B);
    timer_set_top(SHARP_MEMLCD_COMINV_INTFNUM, 
                  SHARP_MEMLCD_COMINV_CLKSOURCE_FREQ / SHARP_MEMLCD_COMINV_CLK_PRESCALER_B / 50);
    timer_set_outmode_ch(SHARP_MEMLCD_COMINV_INTFNUM, SHARP_MEMLCD_COMINV_INTFCHN, TIMER_OUTMODE_TOGGLE);
    timer_set_mode(SHARP_MEMLCD_COMINV_INTFNUM, TIMER_MODE_PERIODIC);
    gpio_conf_periph(SHARP_MEMLCD_COMINV_PORT, SHARP_MEMLCD_COMINV_PIN, 0x80);
    #endif
}

#if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_CRON) || (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_SW)
void sharp_memlcd_vcom_toggle(void){
    if (sharp_memlcd_sm.vcom){
        #if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_CRON)
        sharp_memlcd_cominv_low();
        #endif
        sharp_memlcd_sm.vcom = 0;
    }
    else{
        #if (SHARP_MEMLCD_COMINV == SHARP_MEMLCD_COMINV_HW_CRON)
        sharp_memlcd_cominv_high();
        #endif
        sharp_memlcd_sm.vcom = 1;
    }
}
#endif
