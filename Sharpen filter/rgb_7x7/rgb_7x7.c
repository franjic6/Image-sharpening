#include <sys\exception.h>
#include <cdefBF533.h>
#include "sysreg.h"
#include "ccblkfn.h"

// Function declarations
void inicijalizacija_EBIU(void);
void postavi_SCLK_54MHz(void);
void Init_SDRAM(void);
void sharpen_filter(void);

#define WIDTH 1920 // image width in pixels
#define HEIGHT 1080  // image height in pixels

unsigned char *rgb_image, *sharpened_rgb_image;

void main(void)
{
    // System initialization
    sysreg_write(reg_SYSCFG, 0x32); // Initialize System Configuration Register
    Init_SDRAM();

    // Setting pointers to memory addresses
    rgb_image = (unsigned char *)0x0;             
    sharpened_rgb_image = (unsigned char *)0x5EEC00;
    sharpen_filter();
}

// Function to apply sharpening filter
void sharpen_filter()
{
    // 7x7 sharpening kernel (strong center focus)
    int sharpen_kernel[7][7] = {
        { 0,  0,  0, -1,  0,  0,  0},
        { 0,  0, -1, -1, -1,  0,  0},
        { 0, -1, -1, -1, -1, -1,  0},
        {-1, -1, -1, 25, -1, -1, -1},
        { 0, -1, -1, -1, -1, -1,  0},
        { 0,  0, -1, -1, -1,  0,  0},
        { 0,  0,  0, -1,  0,  0,  0}
    };
    
    int i, j, k, l;
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            int r_sum = 0, g_sum = 0, b_sum = 0;

            // Process 7x7 neighborhood
            for (k = -3; k <= 3; k++)
            {
                for (l = -3; l <= 3; l++)
                {
                    // Boundary clamping for 7x7 kernel
                    int row = i + k;
                    row = (row < 0) ? 0 : (row >= HEIGHT) ? HEIGHT - 1 : row;
                    int col = j + l;
                    col = (col < 0) ? 0 : (col >= WIDTH) ? WIDTH - 1 : col;

                    int pixel_index = (row * WIDTH + col) * 3;
                    int weight = sharpen_kernel[k + 3][l + 3];  // Offset by 3 for 7x7

                    r_sum += rgb_image[pixel_index] * weight;
                    g_sum += rgb_image[pixel_index + 1] * weight;
                    b_sum += rgb_image[pixel_index + 2] * weight;
                }
            }

            // Clamp values (no normalization needed due to kernel sum = 1)
            r_sum = (r_sum < 0) ? 0 : (r_sum > 255) ? 255 : r_sum;
            g_sum = (g_sum < 0) ? 0 : (g_sum > 255) ? 255 : g_sum;
            b_sum = (b_sum < 0) ? 0 : (b_sum > 255) ? 255 : b_sum;

            int result_index = (i * WIDTH + j) * 3;
            sharpened_rgb_image[result_index] = (unsigned char)r_sum;
            sharpened_rgb_image[result_index + 1] = (unsigned char)g_sum;
            sharpened_rgb_image[result_index + 2] = (unsigned char)b_sum;
        }
    }
}




// SDRAM initialization
void Init_SDRAM(void)
{
    // Check if SDRAM is ready
    if (*pEBIU_SDSTAT & SDRS)
    {
        // Set SDRAM registers
        *pEBIU_SDRRC = 0x0000019c; // Refresh Rate Control Register
        *pEBIU_SDBCTL = 0x00000013; // Memory Bank Control Register
        *pEBIU_SDGCTL = 0x0091998d; // Memory Global Control Register
        ssync(); // System synchronization
    }
}
