#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <vector>
#include <thread>
#include <future>
#include <algorithm>
#include <iostream>

using namespace std;



constexpr int RENDER_WIDTH = 640;   // Internal render resolution
constexpr int RENDER_HEIGHT = 360;  

constexpr float Rs = 10.0f;                 // Event Horizon
constexpr float DISK_INNER = 3.0f * Rs;     
constexpr float DISK_OUTER = 15.0f * Rs;    

constexpr int MAX_STEPS = 500;              
constexpr float DT = 0.2f;                  
constexpr float PI = 3.14159265359f;



inline float clampf(float v, float min_v, float max_v) {
    return fmaxf(min_v, fminf(v, max_v));
}

Vector3 calc_acc(Vector3 p, Vector3 v) {
    float r2 = Vector3LengthSqr(p);
    float r = sqrtf(r2);
    if (r < 0.1f) return {0, 0, 0}; 
    
  
    Vector3 cross = Vector3CrossProduct(p, v);
    float h2 = Vector3DotProduct(cross, cross);
    float scale = -1.5f * Rs * h2 / (r2 * r2 * r);
    
    return Vector3Scale(p, scale);
}



Vector3 BlackbodyColor(float K) {
    K = K / 100.0f;
    float r, g, b;
    if (K <= 66) {
        r = 255;
        g = 99.4708025861f * logf(K) - 161.1195681661f;
        b = K <= 19 ? 0 : 138.5177312231f * logf(K - 10) - 305.0447927307f;
    } else {
        r = 329.698727446f * powf(K - 60, -0.1332047592f);
        g = 288.1221695283f * powf(K - 60, -0.0755148492f);
        b = 255;
    }
    return { clampf(r / 255.0f, 0.0f, 1.0f), 
             clampf(g / 255.0f, 0.0f, 1.0f), 
             clampf(b / 255.0f, 0.0f, 1.0f) };
}



Vector3 GetStarColor(Vector3 dir) {
    Vector3 p = Vector3Scale(dir, 300.0f);
    Vector3 i = { floorf(p.x), floorf(p.y), floorf(p.z) };
    Vector3 f = { fmodf(p.x, 1.0f), fmodf(p.y, 1.0f), fmodf(p.z, 1.0f) };
    if (f.x < 0) f.x += 1.0f; if (f.y < 0) f.y += 1.0f; if (f.z < 0) f.z += 1.0f;
    
    float n = sinf(Vector3DotProduct(i, Vector3{ 7.0f, 113.0f, 29.0f })) * 43758.5453f;
    float h = fmodf(n, 1.0f);
    if (h < 0) h += 1.0f;
    
    if (h > 0.985f) {
        float d = Vector3Distance(f, Vector3{0.5f, 0.5f, 0.5f});
        if (d < 0.15f) {
            float brightness = powf(1.0f - d / 0.15f, 3.0f) * (h - 0.985f) * 66.0f;
            float temp = 4000.0f + h * 25000.0f;
            Vector3 col = BlackbodyColor(temp);
            return Vector3Scale(col, brightness * 3.0f);
        }
    }
    
  
    float dust = (sinf(dir.x * 5.0f) * sinf(dir.y * 7.0f) * sinf(dir.z * 6.0f) + 1.0f) * 0.5f;
    dust = powf(dust, 8.0f) * 0.02f;
    return { dust * 0.3f, dust * 0.1f, dust * 0.5f };
}



Vector3 GetDiskColor(Vector3 hit_pos, Vector3 ray_vel) {
    float r = sqrtf(hit_pos.x * hit_pos.x + hit_pos.z * hit_pos.z);
    
   
    float T_rest = powf(DISK_INNER / r, 0.75f); 
    
    
    float v_orb = sqrtf(0.5f * Rs / r);
    Vector3 tangent = { -hit_pos.z, 0.0f, hit_pos.x };
    tangent = Vector3Normalize(tangent);
    Vector3 v_disk = Vector3Scale(tangent, v_orb);
    
    
    float v_dot_ray = Vector3DotProduct(v_disk, ray_vel);
    float gamma = 1.0f / sqrtf(1.0f - v_orb * v_orb);
    float D = 1.0f / (gamma * (1.0f - v_dot_ray));
    
    
    float g_grav = sqrtf(1.0f - Rs / r);
    float g = D * g_grav;
    
    
    float T_obs = T_rest * g;
    Vector3 color = BlackbodyColor(T_obs * 9000.0f); 
    

    float intensity = powf(g, 3.0f) * powf(DISK_INNER / r, 2.5f);
    
    return Vector3Scale(color, intensity);
}



Vector3 trace_ray(Vector3 ro, Vector3 rd) {
    Vector3 pos = ro;
    Vector3 vel = rd;
    float prev_y = pos.y;
    Vector3 prev_pos = pos;
    
    for (int i = 0; i < MAX_STEPS; ++i) {
        float r2 = Vector3LengthSqr(pos);
        float r = sqrtf(r2);
        
        if (r <= Rs) return {0, 0, 0}; 
        if (r > 250.0f * Rs) return GetStarColor(vel); 
        
        
        Vector3 a1 = calc_acc(pos, vel);
        Vector3 vel_mid = Vector3Add(vel, Vector3Scale(a1, DT * 0.5f));
        vel_mid = Vector3Normalize(vel_mid); 
        
        Vector3 a2 = calc_acc(Vector3Add(pos, Vector3Scale(vel, DT * 0.5f)), vel_mid);
        vel = Vector3Add(vel, Vector3Scale(a2, DT));
        vel = Vector3Normalize(vel);
        
        prev_pos = pos;
        prev_y = pos.y;
        pos = Vector3Add(pos, Vector3Scale(vel, DT));
       
        if (prev_y * pos.y <= 0.0f && i > 0) {
            float t_hit = prev_y / (prev_y - pos.y);
            Vector3 hit_pos = Vector3Add(prev_pos, Vector3Scale(Vector3Subtract(pos, prev_pos), t_hit));
            float hit_r = sqrtf(hit_pos.x * hit_pos.x + hit_pos.z * hit_pos.z);
            
            if (hit_r >= DISK_INNER && hit_r <= DISK_OUTER) {
                return GetDiskColor(hit_pos, vel);
            }
        }
    }
    return GetStarColor(vel);
}


Vector3 aces(Vector3 x) {
    float a = 2.51f, b = 0.03f, c = 2.43f, d = 0.59f, e = 0.14f;
    float r = (x.x * (a * x.x + b)) / (x.x * (c * x.x + d) + e);
    float g = (x.y * (a * x.y + b)) / (x.y * (c * x.y + d) + e);
    float bl = (x.z * (a * x.z + b)) / (x.z * (c * x.z + d) + e);
    return { clampf(r, 0.0f, 1.0f), clampf(g, 0.0f, 1.0f), clampf(bl, 0.0f, 1.0f) };
}

Vector3 gamma_correct(Vector3 c) {
    return { powf(c.x, 1.0f/2.2f), powf(c.y, 1.0f/2.2f), powf(c.z, 1.0f/2.2f) };
}



int main() {
    InitWindow(1280, 720, "Interstellar Black Hole - CPU Ray Tracer");
    SetTargetFPS(60);
    
    Image render_image = GenImageColor(RENDER_WIDTH, RENDER_HEIGHT, BLACK);
    Texture2D render_tex = LoadTextureFromImage(render_image);
    SetTextureFilter(render_tex, TEXTURE_FILTER_BILINEAR);
    Color* pixels = (Color*)render_image.data;
    
    // Loading Screen
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("CALCULATING SPACETIME METRIC...", 380, 350, 20, WHITE);
    EndDrawing();
    
    // Camera State
    float cam_yaw = 0.8f;
    float cam_pitch = 0.4f;
    float cam_dist = 140.0f;
    
    while (!WindowShouldClose()) {
        
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 delta = GetMouseDelta();
            cam_yaw -= delta.x * 0.005f;
            cam_pitch += delta.y * 0.005f;
            cam_pitch = clampf(cam_pitch, -1.5f, 1.5f);
        }
        
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            cam_dist -= wheel * 8.0f;
            cam_dist = clampf(cam_dist, 25.0f, 400.0f);
        }
        
       
        Vector3 cam_pos = {
            cam_dist * cosf(cam_pitch) * sinf(cam_yaw),
            cam_dist * sinf(cam_pitch),
            cam_dist * cosf(cam_pitch) * cosf(cam_yaw)
        };
        Vector3 cam_target = { 0, 0, 0 };
        Vector3 forward = Vector3Normalize(Vector3Subtract(cam_target, cam_pos));
        Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, Vector3{0, 1, 0}));
        Vector3 up = Vector3CrossProduct(right, forward);
        
        float aspect = (float)RENDER_WIDTH / (float)RENDER_HEIGHT;
        float half_height = tanf(30.0f * PI / 180.0f);
        float half_width = half_height * aspect;
        
        
        int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 8;
        std::vector<std::future<void>> futures;
        int rows_per_thread = RENDER_HEIGHT / num_threads;

        for (int t = 0; t < num_threads; ++t) {
            int start_y = t * rows_per_thread;
            int end_y = (t == num_threads - 1) ? RENDER_HEIGHT : start_y + rows_per_thread;
            
            futures.push_back(std::async(std::launch::async, [&, start_y, end_y, cam_pos, forward, right, up, half_width, half_height] {
                for (int y = start_y; y < end_y; ++y) {
                    for (int x = 0; x < RENDER_WIDTH; ++x) {
                        float u = ((float)x / RENDER_WIDTH) * 2.0f - 1.0f;
                        float v = 1.0f - ((float)y / RENDER_HEIGHT) * 2.0f;
                        
                        Vector3 rd = Vector3Normalize(Vector3Add(
                            forward, 
                            Vector3Add(
                                Vector3Scale(right, u * half_width),
                                Vector3Scale(up, v * half_height)
                            )
                        ));
                        
                        Vector3 hdr = trace_ray(cam_pos, rd);
                        Vector3 mapped = aces(hdr);
                        Vector3 final_col = gamma_correct(mapped);
                        
                        pixels[y * RENDER_WIDTH + x] = Color{
                            (unsigned char)(final_col.x * 255.0f),
                            (unsigned char)(final_col.y * 255.0f),
                            (unsigned char)(final_col.z * 255.0f),
                            255
                        };
                    }
                }
            }));
        }

        for (auto& f : futures) f.get();
        UpdateTexture(render_tex, pixels);
        
       
        BeginDrawing();
        ClearBackground(BLACK);
        
        
        DrawTexturePro(
            render_tex, 
            { 0, 0, (float)RENDER_WIDTH, (float)RENDER_HEIGHT },
            { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() },
            { 0, 0 }, 0.0f, WHITE
        );
        
      
        DrawText("SCHWARZSCHILD GEODESIC RAY TRACER", 20, 20, 20, WHITE);
        DrawText("MULTI-THREADED CPU PHYSICS", 20, 45, 16, LIGHTGRAY);
        DrawText(TextFormat("FPS: %d", GetFPS()), 20, 70, 16, YELLOW);
        DrawText("DRAG MOUSE: ORBIT", 20, GetScreenHeight() - 40, 16, LIGHTGRAY);
        DrawText("SCROLL: ZOOM", 20, GetScreenHeight() - 20, 16, LIGHTGRAY);
        
        EndDrawing();
    }

    UnloadTexture(render_tex);
    UnloadImage(render_image);
    CloseWindow();
    return 0;
}
