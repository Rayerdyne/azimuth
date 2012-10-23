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

#include "azimuth/state/baddie.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h> // for NULL

#include "azimuth/util/misc.h"
#include "azimuth/util/polygon.h"
#include "azimuth/util/vector.h"

/*===========================================================================*/

#define DECL_COMPONENTS(c) .num_components=AZ_ARRAY_SIZE(c), .components=(c)
#define INIT_POLYGON(v) { .num_vertices=AZ_ARRAY_SIZE(v), .vertices=(v) }

static const az_vector_t lump_vertices[] = {
  {20, 0}, {15, 15}, {-15, 15}, {-15, -15}, {15, -15}
};
static const az_vector_t turret_vertices[] = {
  {20, 0}, {10, 17.320508075688775}, {-10, 17.320508075688775},
  {-20, 0}, {-10, -17.320508075688775}, {10, -17.320508075688775}
};
static const az_vector_t turret_cannon_vertices[] = {
  {30, 5}, {0, 5}, {0, -5}, {30, -5}
};
static const az_component_data_t turret_components[] = {
  { .bounding_radius = 30.5, .polygon = INIT_POLYGON(turret_cannon_vertices) }
};

static const az_baddie_data_t baddie_data[] = {
  [AZ_BAD_LUMP] = {
    .bounding_radius = 20.0,
    .max_health = 10.0,
    .polygon = INIT_POLYGON(lump_vertices)
  },
  [AZ_BAD_TURRET] = {
    .bounding_radius = 30.5,
    .max_health = 15.0,
    DECL_COMPONENTS(turret_components),
    .polygon = INIT_POLYGON(turret_vertices)
  }
};

/*===========================================================================*/

const az_baddie_data_t *az_get_baddie_data(az_baddie_kind_t kind) {
  assert(kind != AZ_BAD_NOTHING);
  const int data_index = (int)kind;
  assert(0 <= data_index && data_index < AZ_ARRAY_SIZE(baddie_data));
  return &baddie_data[data_index];
}

void az_init_baddie(az_baddie_t *baddie, az_baddie_kind_t kind,
                    az_vector_t position, double angle) {
  assert(kind != AZ_BAD_NOTHING);
  baddie->kind = kind;
  baddie->data = az_get_baddie_data(kind);
  baddie->position = position;
  baddie->velocity = AZ_VZERO;
  baddie->angle = angle;
  baddie->health = baddie->data->max_health;
  baddie->cooldown = 0.0;
  for (int i = 0; i < baddie->data->num_components; ++i) {
    assert(i < AZ_ARRAY_SIZE(baddie->components));
    baddie->components[i].position = AZ_VZERO;
    baddie->components[i].angle = 0.0;
  }
}

bool az_ray_hits_baddie(const az_baddie_t *baddie, az_vector_t start,
                        az_vector_t delta, az_vector_t *point_out,
                        az_vector_t *normal_out) {
  assert(baddie->kind != AZ_BAD_NOTHING);
  const az_baddie_data_t *data = baddie->data;

  // Common case: if ray definitely misses baddie, return early.
  if (!az_ray_hits_circle(start, delta, baddie->position,
                          data->bounding_radius)) {
    return false;
  }

  // Calculate start and delta relative to the positioning of the baddie.
  const az_vector_t rel_start = az_vrotate(az_vsub(start, baddie->position),
                                           -baddie->angle);
  az_vector_t rel_delta = az_vrotate(delta, -baddie->angle);
  bool did_hit = false;

  // Check if we hit the main body of the baddie.
  if (az_ray_hits_polygon(data->polygon, rel_start, rel_delta,
                          point_out, normal_out)) {
    did_hit = true;
    rel_delta = az_vsub(*point_out, rel_start);
  }

  // Now check if we hit any of the baddie's components.
  for (int i = 0; i < data->num_components; ++i) {
    assert(i < AZ_ARRAY_SIZE(baddie->components));
    if (az_ray_hits_polygon_trans(data->components[i].polygon,
                                  baddie->components[i].position,
                                  baddie->components[i].angle,
                                  rel_start, rel_delta,
                                  point_out, normal_out)) {
      did_hit = true;
      rel_delta = az_vsub(*point_out, rel_start);
    }
  }

  // Fix up *point_out and *normal_out and return.
  if (did_hit) {
    if (point_out != NULL) {
      *point_out = az_vadd(az_vrotate(*point_out, baddie->angle),
                           baddie->position);
    }
    if (normal_out != NULL) {
      *normal_out = az_vrotate(*normal_out, baddie->angle);
    }
  }
  return did_hit;
}

/*===========================================================================*/
