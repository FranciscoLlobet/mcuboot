/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 * Copyright (c) 2015 Runtime Inc
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __FLASH_MAP_BACKEND_H__
#define __FLASH_MAP_BACKEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * Provides abstraction of flash regions for type of use.
 * I.e. dude where's my image?
 *
 * System will contain a map which contains flash areas. Every
 * region will contain flash identifier, offset within flash and length.
 *
 * 1. This system map could be in a file within filesystem (Initializer
 * must know/figure out where the filesystem is at).
 * 2. Map could be at fixed location for project (compiled to code)
 * 3. Map could be at specific place in flash (put in place at mfg time).
 *
 * Note that the map you use must be valid for BSP it's for,
 * match the linker scripts when platform executes from flash,
 * and match the target offset specified in download script.
 */
#include <inttypes.h>
#include <sys/types.h>

/* Structure describing a flash area. */

struct flash_area
{
  /* MCUboot-API fields */

  uint8_t     fa_id;         /* The slot/scratch ID */
  uint8_t     fa_device_id;  /* The device ID (usually there's only one) */
  uint16_t    pad16;         /* Padding */
  uint32_t    fa_off;        /* The flash offset from the beginning */
  uint32_t    fa_size;       /* The size of this sector */


};

/* Structure describing a sector within a flash area. */

struct flash_sector
{
  /* Offset of this sector, from the start of its flash area (not device). */

  uint32_t fs_off;

  /* Size of this sector, in bytes. */

  uint32_t fs_size;
};


/*
 * Retrieve a memory-mapped flash device's base address.
 *
 * On success, the address will be stored in the value pointed to by
 * ret.
 *
 * Returns 0 on success, or an error code on failure.
 */
int flash_device_base(uint8_t fd_id, uintptr_t *ret);

int flash_area_id_from_image_slot(int slot);
int flash_area_id_from_multi_image_slot(int image_index, int slot);

/**
 * Converts the specified flash area ID and image index (in multi-image setup)
 * to an image slot index.
 *
 * Returns image slot index (0 or 1), or -1 if ID doesn't correspond to an image
 * slot.
 */
int flash_area_id_to_multi_image_slot(int image_index, int area_id);

/* Retrieve the flash sector a given offset belongs to.
 *
 * Returns 0 on success, or an error code on failure.
 */
int flash_area_sector_from_off(off_t off, struct flash_sector *sector);

static inline uint32_t flash_area_get_off(const struct flash_area *fa)
{
	return (uint32_t)fa->fa_off;
}

static inline uint32_t flash_area_get_size(const struct flash_area *fa)
{
	return (uint32_t)fa->fa_size;
}

static inline uint8_t flash_area_get_id(const struct flash_area *fa)
{
	return fa->fa_id;
}

uint8_t flash_area_get_device_id(const struct flash_area *fa);

/*
 * Returns the value expected to be read when accessing any erased
 * flash byte.
 */
uint8_t flash_area_erased_val(const struct flash_area *fap);

static inline uint32_t flash_sector_get_off(const struct flash_sector *fs)
{
	return fs->fs_off;
}

static inline uint32_t flash_sector_get_size(const struct flash_sector *fs)
{
	return fs->fs_size;
}

/* Retrieve the flash sector withing given flash area, at a given offset.
 *
 * @param fa        flash area where the sector is taken from.
 * @param off       offset within flash area.
 * @param sector    structure of sector information.
 * Returns 0 on success, -ERANGE if @p off is beyond flash area size,
 *         other negative errno code on failure.
 */
int flash_area_get_sector(const struct flash_area *fa, off_t off,
                          struct flash_sector *fs);

/**
 * @brief flash_area_open
 * 
 * @param flash_area_id : flash area id
 * @param fap : flash area pointer 
 * @return int 
 */
int flash_area_open(int flash_area_id, const struct flash_area **fap);

int flash_area_close(const struct flash_area *fa);
int flash_area_read(const struct flash_area *fa, uint32_t off, void *dst,
                    uint32_t len);

int flash_area_write(const struct flash_area *fa, uint32_t off, const void *src,
                     uint32_t len);

int flash_area_align(const struct flash_area *fa);

int flash_area_erase(const struct flash_area *fa, uint32_t off, uint32_t len);

int flash_area_to_sectors(int idx, int *cnt, struct flash_sector *ret);

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_MAP_BACKEND_H__ */
