
#include <oss.h>

#include "imshow.h"

void rmemcpy(void *p_dest, const void *p_src, int size)
{
    char *dest = (char *) p_dest;
    const char *src = (char *) p_src;

    for (int i = 0; i < size; i++)
        *dest++ = *src++;
}

int show(int ac, char **av)
{
    if (ac != 2) {
        printf("%s [image_path]", av[0]);
        goto out;
    }

    file_t *img_file = open(av[1]);

    if (!img_file) {
        printf("%s: No such file.\n", av[1]);
        return 1;
    }

    bmp_header_t *hdr = (bmp_header_t *) img_file->content;
    printf("%d, size:%d\n", hdr->magic, hdr->size);

    if (hdr->magic != BMP_MAGIC) {
        printf("%s: format unrecognized.\n", av[1]);
        return 1;
    }

    bmp_info_header_t *inf =
        (bmp_info_header_t *) (img_file->content + sizeof(bmp_header_t));

    uint32_t *fb = malloc(hdr->size * sizeof(uint32_t));
    uint32_t *im_data = (uint32_t *) (img_file->content + hdr->offset);

    rect_t *dim = malloc(sizeof(rect_t));

    dim->x = 0;
    dim->y = 0;
    dim->dx = inf->width;
    dim->dy = inf->height;

    for (unsigned int i = 0; i < dim->dy; i++) {
        for (unsigned int j = 0; j < dim->dx; j++) {
            fb[(dim->dy - i) * dim->dx + j] = im_data[i * dim->dx + j];
        }
    }
    draw_fb(fb, dim);

    close(img_file);
    free(dim);
    
out:
    refresh();
    return 0;

}
