#include <stdio.h>
#include "polyshade.h"

int main() {
  al_init();
  al_init_primitives_addon();

  ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
  ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
  ALLEGRO_DISPLAY* disp = al_create_display(512, 512);

  al_register_event_source(queue, al_get_display_event_source(disp));
  al_register_event_source(queue, al_get_timer_event_source(timer));

  bool redraw = true;
  bool done = false;
  ALLEGRO_EVENT event;

  struct scene testScene = {
    .camPos = {0, 0, -120},
    .camRot = {0},
    .dispOff = {0, 0, 100},
    .bg_color = al_map_rgb(0, 0, 0),
    .numModels = 1,
    .models = loadModel("cube.bin")
  };

  testScene.models[0].scale = 100;

  al_start_timer(timer);
  while (1) {
    al_wait_for_event(queue, &event);

    switch (event.type) {
      case ALLEGRO_EVENT_TIMER:
        redraw = true;
        break;
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
    }

    if (done)
      break;

    if (redraw && al_is_event_queue_empty(queue)) {
      ALLEGRO_BITMAP* target = al_clone_bitmap(al_get_backbuffer(disp));
      struct drawScene_opts opts = {
        .flags = WIREFRAME
      };

      drawScene(&testScene, &opts, target);
      al_draw_bitmap(target, 0, 0, 0);
      al_draw_filled_rectangle(50, 50, 100, 100, al_map_rgb(255, 0, 0));
      al_flip_display();

      redraw = false;
    }
  }

  al_destroy_display(disp);
  al_destroy_timer(timer);
  al_destroy_event_queue(queue);

  return 0;
}
