#include "main_window.h"

static Window *s_window;

Layer *s_time_layer;
Layer *s_health_layer;

static GFont s_plex_sans_regular_20;
static GFont s_plex_sans_regular_48;
static GFont s_plex_sans_bold_48;

static char s_hours_buffer[3];
static char s_minutes_buffer[3];
static char s_date_buffer[32];

static void time_update_proc(Layer *layer, GContext *ctx) {
  const GRect layer_bounds = layer_get_bounds(layer);
  const int margin = 5;
  
  // Black background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, 0, layer_bounds.size.w, layer_bounds.size.h), 0, GCornersAll);
  
  // White display
  graphics_context_set_fill_color(ctx, GColorWhite);
  const GSize size_hour = graphics_text_layout_get_content_size("00", s_plex_sans_bold_48, GRect(0, 0, layer_bounds.size.w/2, layer_bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentRight);
  graphics_fill_rect(ctx, GRect(layer_bounds.size.w/2-size_hour.w, margin, 2*size_hour.w, size_hour.h), margin, GCornersAll);
  
  // Draw time
  graphics_context_set_text_color(ctx, GColorBlack);
  graphics_draw_text(ctx, s_hours_buffer, s_plex_sans_bold_48, GRect(0, 0, layer_bounds.size.w/2, layer_bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentRight, NULL);
  graphics_draw_text(ctx, s_minutes_buffer, s_plex_sans_regular_48, GRect(layer_bounds.size.w/2, 0, layer_bounds.size.w, layer_bounds.size.h), GTextOverflowModeWordWrap, GTextAlignmentLeft, NULL);
  
  // Draw date
  graphics_context_set_text_color(ctx, GColorWhite);
  graphics_draw_text(ctx, s_date_buffer, s_plex_sans_regular_20, GRect(layer_bounds.size.w/2-size_hour.w, size_hour.h + margin, 2*size_hour.w, layer_bounds.size.h/2), GTextOverflowModeTrailingEllipsis, GTextAlignmentRight, NULL);
  
}

static void health_update_proc(Layer *layer, GContext *ctx) {
  const GRect layer_bounds = layer_get_bounds(layer);
  
  // Black background
  graphics_context_set_fill_color(ctx, GColorBlack);
  graphics_fill_rect(ctx, GRect(0, 0, layer_bounds.size.w, layer_bounds.size.h), 0, GCornersAll);
  
}


static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  /* time layer is bottom half */
  s_time_layer = layer_create(
    GRect(
      0, window_bounds.size.h/2,
      window_bounds.size.w, window_bounds.size.h/2
    )
  );
  layer_add_child(window_layer,s_time_layer);
  layer_set_update_proc(s_time_layer, time_update_proc);
  
  /* health layer is top health */
  s_health_layer = layer_create(
    GRect(
      0, 0,
      window_bounds.size.w, window_bounds.size.h/2
    )
  );
  layer_add_child(window_layer,s_health_layer);
  layer_set_update_proc(s_health_layer, health_update_proc);
  
  
  s_plex_sans_regular_20 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PLEX_SANS_REGULAR_20));
  s_plex_sans_regular_48 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PLEX_SANS_REGULAR_54));
  s_plex_sans_bold_48 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PLEX_SANS_BOLD_54));
}

static void window_unload(Window *window) {
  layer_destroy(s_time_layer);
  layer_destroy(s_health_layer);
  
  fonts_unload_custom_font(s_plex_sans_regular_20);
  fonts_unload_custom_font(s_plex_sans_regular_48);
  fonts_unload_custom_font(s_plex_sans_bold_48);
  
  window_destroy(s_window);
}

void main_window_push() {
  s_window = window_create();
  window_set_background_color(s_window, GColorWhite);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_window, true);
}

void main_window_update_time(struct tm* tick_time) {
  strftime(s_hours_buffer, sizeof(s_hours_buffer),
    clock_is_24h_style() ? "%H" : "%l", tick_time);
  strftime(s_minutes_buffer, sizeof(s_minutes_buffer),"%M", tick_time);
  strftime(s_date_buffer, sizeof(s_date_buffer), "%a %e", tick_time);
  layer_mark_dirty(s_time_layer);
  //APP_LOG(APP_LOG_LEVEL_INFO, "time updated\n" );
}

void main_window_redraw() {
  if(s_time_layer) {
    layer_mark_dirty(s_time_layer);
    //layer_mark_dirty(s_text_layer);
  }
}
