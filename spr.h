#ifndef SPR_H_INCLUDED
#define SPR_H_INCLUDED

#include <stdint.h>

#define SPR_MAGIC "IDSP"

#define SPR_TYPE_VP_PARALLEL_UPRIGHT 0x0
#define SPR_TYPE_FACING_UPRIGHT 0x1
#define SPR_TYPE_VP_PARALLEL 0x2
#define SPR_TYPE_ORIENTED 0x3
#define SPR_TYPE_VP_PARALLEL_ORIENTED 0x4

#define SPR_TEX_FORMAT_NORMAL 0x0
#define SPR_TEX_FORMAT_ADDITIVE 0x1
#define SPR_TEX_FORMAT_INDEXALPHA 0x2
#define SPR_TEX_FORMAT_ALPHTEST 0x3

#define SPR_SYNC_TYPE_SYNCHRONIZED 0x0
#define SPR_SYNC_TYPE_RANDOM 0x1

typedef struct {
	uint16_t palette_size;
    uint8_t *palette;
} SPR_COLOR_PALETTE;

typedef struct {
    uint32_t frame_group;
    int32_t origin_x;
    int32_t origin_y;
    uint32_t width;
    uint32_t height;
    uint8_t *image_data;
} SPR_FRAME;

typedef struct {
    uint32_t sprite_version;
    uint32_t sprite_type;
    uint32_t texture_format;
    float bounding_radius;
    uint32_t frame_max_width;
    uint32_t frame_max_height;
    uint32_t frame_count;
    float beam_length;
    uint32_t synchronization_type;
	SPR_COLOR_PALETTE palette;
	SPR_FRAME **frames;
} SPR;

SPR spr_init(uint16_t sprite_type, uint16_t texture_format, uint16_t synchronization_type);

int spr_update(SPR *spr);

int spr_save_file(SPR *spr, char *path);

void spr_free(SPR *spr);

#endif // SPR_H_INCLUDED
