#include "ScanlineAlgo.h"
#include <array>
#include <algorithm>


void rasterize_row(int y, 
        GpuVertex& left_edge_v1, GpuVertex& left_edge_v2,
        GpuVertex& right_edge_v1, GpuVertex& right_edge_v2,
        Screen& screen) 
{
    auto& left_edge_sp1 = left_edge_v1.screen_pos;
    auto& left_edge_sp2 = left_edge_v2.screen_pos;

    auto& right_edge_sp1 = right_edge_v1.screen_pos;
    auto& right_edge_sp2 = right_edge_v2.screen_pos;

    float left_gradient_y =  1.f;
    if (left_edge_sp1.y != left_edge_sp2.y) 
    {
        left_gradient_y =  (y - left_edge_sp1.y) / (float)(left_edge_sp2.y - left_edge_sp1.y);
    }

    float right_gradient_y =  1.f;
    if (right_edge_sp1.y != right_edge_sp2.y) 
    {
        right_gradient_y =  (y - right_edge_sp1.y) / (float)(right_edge_sp2.y - right_edge_sp1.y);
    }

    int left_x = (int) ((float)left_edge_sp1.x + left_gradient_y * (float)(left_edge_sp2.x - left_edge_sp1.x) ) ;
    int right_x = (int) ((float)right_edge_sp1.x + right_gradient_y * (float)(right_edge_sp2.x - right_edge_sp1.x) ) ;

    for(int x = left_x; x <= right_x; ++x) 
    {
        Color red {255, 0, 0, 255};
        screen.put_pixel(x, y, red);
    }
}

void ScanlineAlgo::rasterize(GpuVertex& v1, GpuVertex& v2, GpuVertex& v3, Screen& screen)
{
    std::array<std::reference_wrapper<GpuVertex>, 3> points = {v1, v2, v3};
    std::sort(points.begin(), points.end(), [](const GpuVertex& v1, const GpuVertex& v2) {
        return v1.screen_pos.y < v2.screen_pos.y;
    });

    auto& v1s = points[0].get();
    auto& v2s = points[1].get();
    auto& v3s = points[2].get();

    auto& p1s = v1s.screen_pos;
    auto& p2s = v2s.screen_pos;
    auto& p3s = v3s.screen_pos;

    float inv_slope_p1p2 = (float)(p2s.x - p1s.x) / (float)(p2s.y - p1s.y);
    float inv_slope_p1p3 = (float)(p3s.x - p1s.x) / (float)(p3s.y - p1s.y);

    // <|
    if (inv_slope_p1p2 < inv_slope_p1p3) {
        for (int y = p1s.y; y <= p3s.y; ++y) 
        {
            if (y < p2s.y) {
                rasterize_row(y, v1s,v2s, v1s,v3s, screen);
            } 
            else {
                rasterize_row(y, v2s,v3s, v1s,v3s, screen);
            }
        }
    } else { // |>
        for (int y = p1s.y; y <= p3s.y; ++y) 
        {
            if (y < p2s.y) {
                rasterize_row(y, v1s,v3s, v1s,v2s, screen);
            } 
            else {
                rasterize_row(y, v1s,v3s, v2s,v3s, screen);
            }
        }
    }
}
