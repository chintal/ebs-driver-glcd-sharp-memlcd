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


#ifndef DRIVER_SHARP_MEMLCD_PANELS_H
#define DRIVER_SHARP_MEMLCD_PANELS_H

#include "config.h"

#define SHARP_MEMLCD_LS013B4DN04        0

#if SHARP_MEMLCD_PANEL == SHARP_MEMLCD_LS013B4DN04
    #define SHARP_MEMLCD_NROWS          96
    #define SHARP_MEMLCD_NCOLS          96
#endif

#endif

