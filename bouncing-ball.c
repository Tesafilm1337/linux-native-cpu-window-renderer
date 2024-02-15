#include <stdint.h>

#include "x-native-window.c"
#include "olive.c"
#include "geez.c"

#define FACTOR  100
#define WIDTH   16 * FACTOR
#define HEIGHT   9 * FACTOR

#define BACKGROUND_COLOR 0xFF181818
#define BALL_RADIUS 100
#define GRAVITY 1000.0

typedef struct {
    float x, y;
} Vector2;

static Vector2 ball_position = {BALL_RADIUS, BALL_RADIUS};
static Vector2 ball_velocity = {200, 200};

Olivec_Canvas oc;

static int width = WIDTH, height = HEIGHT;

Olivec_Canvas game_render(float dt)
{
    olivec_fill(oc, BACKGROUND_COLOR);

    // Circle
    ball_velocity.y += GRAVITY*dt;
    float x = ball_position.x + ball_velocity.x*dt;
    if (x - BALL_RADIUS < 0.0 || x + BALL_RADIUS >= width) {
        ball_velocity.x *= -1.0f;
    } else {
        ball_position.x = x;
    }
    float y = ball_position.y + ball_velocity.y*dt;
    if (y - BALL_RADIUS < 0.0 || y + BALL_RADIUS >= height) {
        ball_velocity.y *= -1.0f;
    } else {
        ball_position.y = y;
    }
    olivec_circle(oc, ball_position.x, ball_position.y, BALL_RADIUS, 0xFFFF0000);

    return oc;
}

int main(void) {
    int window = create_window(WIDTH, HEIGHT, "Simple, CPU rendered Game");

    geez_set_render_target(window, width, height);

    uint64_t target = 1000/60;
    uint64_t current_time = get_time()/1000000;
    uint64_t last_redraw =  current_time - 1;

    bool should_close = false;
    while (!should_close) {
        uint64_t frametime = last_redraw - current_time;
        int timeout = (target - frametime);
        event_loop_poll(timeout);
        current_time = get_time()/1000000;
        uint64_t deltatime = current_time - last_redraw;

        for (Event e = begin_event(); next_event(&e);)
            switch ((int)e.type) {
                case E_CLOSE:
                    if (e.window == window)
                        should_close = true; 
                    break;
                case E_RESIZE: if (e.window == window) {
                    width = e.new_width;
                    height = e.new_height;
                    geez_update_target_dimensions(
                        window,
                        width, height
                    );
                } break;
            }
        
        oc = geez_get_canvas();
        game_render((float)deltatime/1000);
        geez_blit();

        last_redraw = get_time()/1000000;
    }

    close_window(window);
}


#define STB_DS_IMPLEMENTATION
#define X_NATIVE_WINDOW_IMPLEMENTATION
#include "x-native-window.c"

#define OLIVEC_IMPLEMENTATION
#include "olive.c"


#define GEEZ_IMPLEMENTATION
#include "geez.c"