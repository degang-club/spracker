#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spr.h"

SPR spr_init(uint16_t sprite_type, uint16_t texture_format, uint16_t synchronization_type)
{
	return (SPR){
		.sprite_version = 0x2,
		.sprite_type = sprite_type,
		.texture_format = texture_format,
		.bounding_radius = 0,
		.frame_max_width = 0,
		.frame_max_height = 0,
		.frame_count = 0,
		.beam_length = 0,
		.synchronization_type = synchronization_type
	};
}

int spr_update(SPR *spr)
{
	// int i;
	// uint64_t image_size;

	
	return 0;
}

/* NOTE This code will probably create bad SPR files on big-endian systems */
int spr_save_file(SPR *spr, char *path) {
	int i, x, image_size;
	FILE *f = fopen(path, "w");

	/* Write the header */
	fwrite(SPR_MAGIC, 4, 1, f);
	fwrite(&spr->sprite_version, sizeof(spr->sprite_version), 1, f);
	fwrite(&spr->sprite_type, sizeof(spr->sprite_type), 1, f);
	fwrite(&spr->texture_format, sizeof(spr->texture_format), 1, f);
	fwrite(&spr->bounding_radius, sizeof(spr->bounding_radius), 1, f);
	fwrite(&spr->frame_max_width, sizeof(spr->frame_max_width), 1, f);
	fwrite(&spr->frame_max_height, sizeof(spr->frame_max_height), 1, f);
	fwrite(&spr->frame_count, sizeof(spr->frame_count), 1, f);
	fwrite(&spr->beam_length, sizeof(spr->beam_length), 1, f);
	fwrite(&spr->synchronization_type, sizeof(spr->synchronization_type), 1, f);
	
	/* Write the palette */
	fwrite(&spr->palette.palette_size, sizeof(spr->palette.palette_size), 1, f);
	for (i=0; i < spr->palette.palette_size; i++) {
		fwrite(&spr->palette.palette[i * 3], sizeof(*spr->palette.palette), 1, f);
		fwrite(&spr->palette.palette[i * 3 + 1], sizeof(*spr->palette.palette), 1, f);
		fwrite(&spr->palette.palette[i * 3 + 2], sizeof(*spr->palette.palette), 1, f);
	}
	
	/* Write the frames */
	for (unsigned i=0; i < spr->frame_count; i++) {
		fwrite(&spr->frames[i]->frame_group, sizeof(spr->frames[i]->frame_group), 1, f);
		fwrite(&spr->frames[i]->origin_x, sizeof(spr->frames[i]->origin_x), 1, f);
		fwrite(&spr->frames[i]->origin_y, sizeof(spr->frames[i]->origin_y), 1, f);
		fwrite(&spr->frames[i]->width, sizeof(spr->frames[i]->width), 1, f);
		fwrite(&spr->frames[i]->height, sizeof(spr->frames[i]->height), 1, f);
		
		image_size = spr->frames[i]->width * spr->frames[i]->height;
		for (x=0; x < image_size; x++)
			fwrite(&spr->frames[i]->image_data[x], sizeof(*spr->frames[i]->image_data), 1, f);
	}

	return 0;
}

void spr_free(SPR *spr)
{
}
