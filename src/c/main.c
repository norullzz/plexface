/*
 *    ES, Nov 2017
 */

#include <pebble.h>

#include "util.h"

#include "main_window.h"

static void tick_handler(struct tm *tick_time, TimeUnits changed) {
  main_window_update_time(tick_time);
  main_window_redraw();
}

void init() {

  main_window_push();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  main_window_update_time(util_get_tm());
}

void deinit() { }

int main() {
  //APP_LOG(APP_LOG_LEVEL_INFO, "the time is: %d", (int32_t) util_get_tm());
  init();
  app_event_loop();
  deinit();
}
