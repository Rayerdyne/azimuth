/*=============================================================================
| Copyright 2012 Matthew D. Steele <mdsteele@alum.mit.edu>                    |
|                                                                             |
| This file is part of Azimuth.                                               |
|                                                                             |
| Azimuth is free software: you can redistribute it and/or modify it under    |
| the terms of the GNU General Public License as published by the Free        |
| Software Foundation, either version 3 of the License, or (at your option)   |
| any later version.                                                          |
|                                                                             |
| Azimuth is distributed in the hope that it will be useful, but WITHOUT      |
| ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       |
| FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for   |
| more details.                                                               |
|                                                                             |
| You should have received a copy of the GNU General Public License along     |
| with Azimuth.  If not, see <http://www.gnu.org/licenses/>.                  |
=============================================================================*/

#pragma once
#ifndef AZIMUTH_SPACE_H_
#define AZIMUTH_SPACE_H_

#include "azimuth/ship.h"
#include "azimuth/vector.h"

/*===========================================================================*/

typedef struct az_space_state_s {
  unsigned long clock;
  az_vector_t camera;
  az_ship_t ship;
} az_space_state_t;

void az_tick_space_state(az_space_state_t *state,
                         const az_controls_t *controls,
                         double time_seconds);

/*===========================================================================*/

#endif // AZIMUTH_SPACE_H_
