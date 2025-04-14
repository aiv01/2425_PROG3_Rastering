#include "ScanlineAlgo.h"
#include <array>
#include <algorithm>

void rasterize_row(VGpu& gpu, int y, 
        GpuVertex& left_edge_v1, GpuVertex& left_edge_v2,
        GpuVertex& right_edge_v1, GpuVertex& right_edge_v2,
        Screen& screen, PaintingMode painting_mode) 
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

    float left_z = interpolate(left_edge_v1.z_pos, left_edge_v2.z_pos, left_gradient_y);
    float right_z = interpolate(right_edge_v1.z_pos, right_edge_v2.z_pos, right_gradient_y);

    Color left_color, right_color;
    Vector2f left_uv, right_uv;

    if (painting_mode & PaintingMode::COLOR)
    {
        left_color = interpolate(left_edge_v1.color, left_edge_v2.color, left_gradient_y);
        right_color = interpolate(right_edge_v1.color, right_edge_v2.color, right_gradient_y);
    }

    if (painting_mode & PaintingMode::TEXTURE)
    {
        // Flipped UVs.
        Vector2f left_edge_v1_inverse_uv = { left_edge_v1.uv.x, 1 - left_edge_v1.uv.y };
        Vector2f left_edge_v2_inverse_uv = { left_edge_v2.uv.x, 1 - left_edge_v2.uv.y };
        Vector2f right_edge_v1_inverse_uv = { right_edge_v1.uv.x, 1 - right_edge_v1.uv.y };
        Vector2f right_edge_v2_inverse_uv = { right_edge_v2.uv.x, 1 - right_edge_v2.uv.y };

        left_uv = interpolate(left_edge_v1_inverse_uv, left_edge_v2_inverse_uv, left_gradient_y);
        right_uv = interpolate(right_edge_v1_inverse_uv, right_edge_v2_inverse_uv, right_gradient_y);
    }

    for(int x = left_x; x <= right_x; ++x) 
    {
        float gradient_x = 1.f;
        if (left_x < right_x) 
        {
            gradient_x = static_cast<float>(x - left_x) / static_cast<float>(right_x - left_x);
        }

        float sample_z = interpolate(left_z, right_z, gradient_x);
        Color sampled_color;

        switch (painting_mode)
        {
            case PaintingMode::COLOR:
            {
                sampled_color = interpolate(left_color, right_color, gradient_x);
                break;
            }
            case PaintingMode::TEXTURE:
            {
                Vector2f sample_uv = interpolate(left_uv, right_uv, gradient_x);
                Texture* texture = gpu.texture;
            
                // Out of bounds!
                // int text_x = texture->width * sample_uv.x;
                // int text_y = texture->height * sample_uv.y;
                int text_x = (texture->width - 1) * sample_uv.x;
                int text_y = (texture->height - 1) * sample_uv.y;

                int text_index = (text_y * texture->width + text_x) * texture->pixel_size;

                sampled_color.r = texture->pixels[text_index + 0];
                sampled_color.g = texture->pixels[text_index + 1];
                sampled_color.b = texture->pixels[text_index + 2];
                sampled_color.a = texture->pixels[text_index + 3];
                break;
            }
            case PaintingMode::BLEND:
                Color sampled_color_to_blend = interpolate(left_color, right_color, gradient_x);

                Vector2f sample_uv = interpolate(left_uv, right_uv, gradient_x);
                Texture* texture = gpu.texture;

                // Out of bounds!
                // int text_x = texture->width * sample_uv.x;
                // int text_y = texture->height * sample_uv.y;
                int text_x = (texture->width - 1) * sample_uv.x;
                int text_y = (texture->height - 1) * sample_uv.y;
            
                int text_index = (text_y * texture->width + text_x) * texture->pixel_size;

                // Lerping.
                sampled_color.r = texture->pixels[text_index + 0] + 0.5f * (sampled_color_to_blend.r - texture->pixels[text_index + 0]);
                sampled_color.g = texture->pixels[text_index + 1] + 0.5f * (sampled_color_to_blend.g - texture->pixels[text_index + 1]);
                sampled_color.b = texture->pixels[text_index + 2] + 0.5f * (sampled_color_to_blend.b - texture->pixels[text_index + 2]);
                sampled_color.a = texture->pixels[text_index + 3] + 0.5f * (sampled_color_to_blend.a - texture->pixels[text_index + 3]);

                break;
        }

        screen.put_pixel(x, y, sample_z, sampled_color);
    }
}

void ScanlineAlgo::rasterize(VGpu& gpu, GpuVertex& v1, GpuVertex& v2, GpuVertex& v3, Screen& screen, PaintingMode painting_mode)
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
                rasterize_row(gpu, y, v1s,v2s, v1s,v3s, screen, painting_mode);
            } 
            else {
                rasterize_row(gpu, y, v2s,v3s, v1s,v3s, screen, painting_mode);
            }
        }
    } else { // |>
        for (int y = p1s.y; y <= p3s.y; ++y) 
        {
            if (y < p2s.y) {
                rasterize_row(gpu, y, v1s,v3s, v1s,v2s, screen, painting_mode);
            } 
            else {
                rasterize_row(gpu, y, v1s,v3s, v2s,v3s, screen, painting_mode);
            }
        }
    }
}
