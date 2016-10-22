#include <pebble.h>
#include "main.h"

static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_date_layer;

static BitmapLayer * s_background_layer;
static GBitmap * s_background_bitmap;

static void main_window_load(Window * window)
{

  Layer * window_layer = window_get_root_layer(window); //Get the information about window
  GRect bounds = layer_get_bounds(window_layer);
  //get bounds

 //new text layer
  s_time_layer = text_layer_create(GRect (15, PBL_IF_ROUND_ELSE(64,128), bounds.size.w, 50));
  s_date_layer = text_layer_create(GRect (105, PBL_IF_ROUND_ELSE(64,128), bounds.size.w, 50));
  s_background_bitmap =  gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);


  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_date_layer, GColorClear);

  text_layer_set_text(s_date_layer,"date");


  s_background_layer = bitmap_layer_create(bounds);

  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));


  //add the text as a child layer to the root window
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));


}

static void update_time()
{
    time_t temp = time(NULL);

    struct tm * tick_time = localtime(&temp);

    static char s_buffer[8];
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M": "%I:%M", tick_time);

    text_layer_set_text(s_time_layer, s_buffer);
}

static void tick_handler (struct tm * tick_time, TimeUnits units_changed)
{
    update_time();
    
}


static void main_window_unload(Window * window)
{
  text_layer_destroy(s_time_layer);  
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
  window_destroy(s_main_window);
  
}


static void init()
{
  s_main_window = window_create(); // create a main window and return a pointer
  
  window_set_window_handlers(s_main_window, 
                             (WindowHandlers) 
                             { .load = main_window_load, .unload = main_window_unload});
 
 window_set_background_color(s_main_window, GColorBlack);
 window_stack_push(s_main_window, true);

 tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
 update_time();

  
} 


static void deinit()
{


}
int main (void)
{
  init();
  app_event_loop();
  deinit();
}
