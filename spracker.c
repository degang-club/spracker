#define _DEFAULT_SOURCE

#include <dirent.h>
#include <float.h>
#include <getopt.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libafbeelding/color_quantization.h>
#include <libafbeelding/format-tga.h>
#include "spr.h"

#define COORDS_TO_INDEX(x, y, width) (y * width + x)

// Define color channels
#define RED 0
#define GREEN 1
#define BLUE 2

const char tga_extension[] = ".tga";

char* strcomb(unsigned int num, ...)
{
	va_list valist;

	char *combined_str;
	unsigned int combined_str_s = 1;

	char *tmp_str;
	unsigned int i;

	va_start(valist, num);
	combined_str = calloc(1, sizeof(char));

	for (i = 0; i < num; i++) {
		tmp_str = va_arg(valist, char*);

		combined_str_s += strlen(tmp_str);
		combined_str = realloc(combined_str, combined_str_s);

		strcat(combined_str, tmp_str);
	}
	va_end(valist);

	return combined_str;
}

int pstrcmp( const struct dirent** a, const struct dirent** b )
{
	int a_size = strlen((*a)->d_name);
	int b_size = strlen((*b)->d_name);
	
	if (a_size == b_size)
		return strcmp( (const char*)(*a)->d_name, (const char*)(*b)->d_name );
	
	if (a_size < b_size)
		return -1;
	else
		return 1;
}

char *substr(char *start, char *end) {
	int length = end - start;
	char *sub = calloc(length + 1, sizeof(char));
	strncpy(sub, start, length);
	sub[length] = '\0';
	return sub;
}

void add_tga_to_spr(SPR *spr, AFB_IMAGE *img)
{
	int image_size = 0;
	AFB_PALETTE pal = afb_quantize_median_cut(*img, 256);
	pal = afb_quantize_median_cut(*img, 256);
	
	if (spr->frame_count == 0) {
		spr->frames = malloc(sizeof(spr->frames));
	} else {
		spr->frames = realloc(spr->frames, sizeof(spr->frames) * (spr->frame_count + 1));
	}
	
	spr->frames[spr->frame_count] = malloc(sizeof(SPR_FRAME));
	
	spr->frames[spr->frame_count]->frame_group = 0;
	spr->frames[spr->frame_count]->origin_x = 0 - img->width / 2;
	spr->frames[spr->frame_count]->origin_y = img->height / 2;
	spr->frames[spr->frame_count]->width = img->width;
	spr->frames[spr->frame_count]->height = img->height;
	
	/* Change max width and height */
	if (spr->frames[spr->frame_count]->width > spr->frame_max_width)
		spr->frame_max_width = spr->frames[spr->frame_count]->width;
	
	if (spr->frames[spr->frame_count]->height > spr->frame_max_height)
		spr->frame_max_height = spr->frames[spr->frame_count]->height;
	
	image_size = spr->frames[spr->frame_count]->width * spr->frames[spr->frame_count]->height;
	spr->frames[spr->frame_count]->image_data = malloc(image_size);

	if (img->image_type != PALETTED) {
		image_to_pal(img, &pal);
	}

	spr->palette.palette_size = img->palette.size;
	spr->palette.palette = malloc(img->palette.size * 3);
	for (unsigned int i = 0; i < img->palette.size; i++) {
		spr->palette.palette[i * 3 + RED] = afb_rgba_get_r(img->palette.colors[i]);
		spr->palette.palette[i * 3 + GREEN] = afb_rgba_get_g(img->palette.colors[i]);
		spr->palette.palette[i * 3 + BLUE] = afb_rgba_get_b(img->palette.colors[i]);
	}

	memcpy(spr->frames[spr->frame_count]->image_data, img->image_data, image_size);

	spr->frame_count++;
}

void convert_tga_to_spr(SPR *spr, char *file_path) {
	AFB_IMAGE img = afb_image_init();
	afb_format_tga_load(&img, file_path);
	add_tga_to_spr(spr, &img);
}

int main(int argc, char *argv[])
{
	int opt;
	int n;
	struct dirent **namelist;
	char *dir_path = "./"; // Use root when nothing supplied
	char *output_file = "./output.spr";
	char *file_path, *extension_start;

	while ((opt = getopt(argc, argv, "d:o:h")) != -1) {
	switch (opt) {
		case 'd':
			dir_path = optarg;
			break;
		case 'o':
			output_file = optarg;
			break;
		case 'h': /* Add help function here */
			exit(0);
			break;
		case '?':
			exit(1);
			break;
		default:
			fprintf(stderr, "No options given\n");
			exit(1);
		}
	}

	// Always add extra '/' to complete path to file
	if (dir_path[strlen(dir_path) - 1] != '/')
		dir_path = strcat(dir_path, "/");
	printf("Dir path: %s\n\n", dir_path);
	
	SPR spr = spr_init(SPR_TYPE_VP_PARALLEL, SPR_TEX_FORMAT_ADDITIVE,
					   SPR_SYNC_TYPE_RANDOM);
	
	// Find all .tga files in dir
	n = scandir(dir_path, &namelist, NULL, pstrcmp);
	if (n < 0)
		perror("scandir");
	else {
		for (int i = 0; i < n; i++) {
			// Find last '.' to get extension 
			extension_start = strrchr(namelist[i]->d_name, '.'); 
			
			if(extension_start == NULL)
				continue;
			// Load tga into wad when it is a tga file
			if(strcmp(extension_start, tga_extension) == 0){
				file_path = strcomb(2, dir_path, namelist[i]->d_name);

				convert_tga_to_spr(&spr, file_path);
				printf("tga file added to spr: %s\n", file_path);
			}
			free(namelist[i]);
		}
	}
	free(namelist);

	spr_update(&spr);
	spr_save_file(&spr, output_file);

	return 0;
}
