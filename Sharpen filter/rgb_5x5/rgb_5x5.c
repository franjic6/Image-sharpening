#include <sys\exception.h>
#include <cdefBF533.h>
#include "sysreg.h"
#include "ccblkfn.h"

// Function declarations
void Init_SDRAM(void);
void sharpen_filter(void);

#define WIDTH 1920
#define HEIGHT 1080

unsigned char *rgb_image, *sharpened_rgb_image;

void main(void)
{
    // System initialization
    sysreg_write(reg_SYSCFG, 0x32);
    Init_SDRAM();

    // Memory pointers (adjust if needed for your memory map)
    rgb_image = (unsigned char *)0x0;
    sharpened_rgb_image = (unsigned char *)0x5EEC00;
    sharpen_filter();
}

// Optimized 5x5 sharpening filter
void sharpen_filter()
{
    // 5x5 sharpening kernel with center emphasis
    const int sharpen_kernel[5][5] = {
        { 0, -1, -1, -1,  0},
        {-1, -1, -1, -1, -1},
        {-1, -1, 21, -1, -1},
        {-1, -1, -1, -1, -1},
        { 0, -1, -1, -1,  0}
    };
    
    // Loop variables
    int i, j, k, l;
    
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            int r_acc = 0, g_acc = 0, b_acc = 0;

            // Process 5x5 neighborhood
            for (k = -2; k <= 2; k++) {
                for (l = -2; l <= 2; l++) {
                    // Boundary clamping
                    int row = i + k;
                    row = (row < 0) ? 0 : (row >= HEIGHT) ? HEIGHT - 1 : row;
                    int col = j + l;
                    col = (col < 0) ? 0 : (col >= WIDTH) ? WIDTH - 1 : col;

                    // Pixel data access
                    int pixel_index = (row * WIDTH + col) * 3;
                    int weight = sharpen_kernel[k + 2][l + 2];  // Kernel offset

                    // Multiply-accumulate operations
                    r_acc += rgb_image[pixel_index] * weight;
                    g_acc += rgb_image[pixel_index + 1] * weight;
                    b_acc += rgb_image[pixel_index + 2] * weight;
                }
            }

            // Clamping with ternary operators
            r_acc = (r_acc < 0) ? 0 : (r_acc > 255) ? 255 : r_acc;
            g_acc = (g_acc < 0) ? 0 : (g_acc > 255) ? 255 : g_acc;
            b_acc = (b_acc < 0) ? 0 : (b_acc > 255) ? 255 : b_acc;

            // Store results
            int result_index = (i * WIDTH + j) * 3;
            sharpened_rgb_image[result_index]     = (unsigned char)r_acc;
            sharpened_rgb_image[result_index + 1] = (unsigned char)g_acc;
            sharpened_rgb_image[result_index + 2] = (unsigned char)b_acc;
        }
    }
}

// SDRAM initialization remains unchanged
void Init_SDRAM(void)
{
    if (*pEBIU_SDSTAT & SDRS) {
        *pEBIU_SDRRC = 0x0000019c;
        *pEBIU_SDBCTL = 0x00000013;
        *pEBIU_SDGCTL = 0x0091998d;
        ssync();
    }
}
