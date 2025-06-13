#ifndef MINIFB_ADAPTER_H
#define MINIFB_ADAPTER_H

#include "dependencies/minifb/include/MiniFB_cpp.h"
#include "dependencies/minifb/include/MiniFB_enums.h"
#include <stdexcept>
#include <cstring>

#define HEIGHT 600
#define WIDTH 800

#define COLOR_RED 0xFFFF0000
#define COLOR_GREEN 0xFF00FF00
#define COLOR_BLUE 0xFF0000FF
#define COLOR_BLACK 0xFF000000
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_YELLOW 0xFFFFFF00
#define COLOR_CYAN 0xFF00FFFF
#define COLOR_MAGENTA 0xFFFF00FF


// 适配器类，用于将 MiniFB 的功能简化
class MiniFB
{
public:
    MiniFB(const char* title, int width, int height)
        : width(width), height(height)
    {
        window = mfb_open(title, width, height);
        if (!window) {
            throw std::runtime_error("Failed to open MiniFB window");
        }
        buffer = new uint32_t[width * height];
        memset(buffer, 0, width * height * sizeof(uint32_t)); // 清屏
    }

    ~MiniFB()
    {
        delete[] buffer;
        mfb_close(window);
    }

    void update()
    {
        mfb_update(window, buffer);
    }

    void clear()
    {
        memset(buffer, 0, width * height * sizeof(uint32_t)); // 清屏
    }

    void setPixel(uint32_t x, uint32_t y, uint32_t color)
    {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            buffer[y * width + x] = color;
        }
    }

    void drawRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
    {
        for (uint32_t i = 0; i < h; ++i) {
            for (uint32_t j = 0; j < w; ++j) {
                setPixel(x + j, y + i, color);
            }
        }
    }
    
    void drawLine(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color)
    {
        if (x1 == x2 && y1 == y2) return;

        int dx = abs(static_cast<int>(x2) - static_cast<int>(x1));
        int dy = abs(static_cast<int>(y2) - static_cast<int>(y1));
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;


        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y1 += sy;
        }
        while (true) {
            if (x1 == x2 && y1 == y2) break;
            setPixel(x1, y1, color);

            int err2 = err * 2;
            if (err2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (err2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
    }

    bool wait()
    {
        return mfb_wait_sync(window);
    }
private:
    struct mfb_window* window;
    uint32_t* buffer;
    uint16_t width;
    uint16_t height;
};

#endif