#include <pebble.h>
#include "main.h"

static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static TextLayer *s_family_one_layer;
static TextLayer *s_family_two_layer;
static TextLayer *s_family_three_layer;
static TextLayer *s_family_four_layer;

static BitmapLayer * s_background_layer;
static GBitmap * s_background_bitmap;
static Layer * window_layer;
static GRect bounds;
static time_t start;


//SEND
static bool send_to_phone_multi() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "send to phone");
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  // Some other request with no string data
  const int dummy_val = 1;
  dict_write_int(iter, MESSAGE_KEY_YourKey, &dummy_val, sizeof(int), true);


  dict_write_end(iter);
  app_message_outbox_send();
  return true;
}

// RECEIVE
// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "got data");

  // Background Color
  Tuple *yourkey_t = dict_find(iter, MESSAGE_KEY_YourKey);
  if (yourkey_t) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "got: %d", (int)yourkey_t->value->int32);
  }


}


static void main_window_load(Window * window)
{

 window_layer  = window_get_root_layer(window); //Get the information about window
  bounds = layer_get_bounds(window_layer);
  //get bounds

 //new text layer
  s_time_layer = text_layer_create(GRect (15, PBL_IF_ROUND_ELSE(64,128)+25, bounds.size.w, 50));
  s_date_layer = text_layer_create(GRect (105, PBL_IF_ROUND_ELSE(64,128)+25, bounds.size.w, 50));

  s_family_one_layer = text_layer_create(GRect (68, PBL_IF_ROUND_ELSE(64,128)-30, bounds.size.w, 50));
  s_family_two_layer = text_layer_create(GRect (68, PBL_IF_ROUND_ELSE(64,128)-30, bounds.size.w, 50));
  s_family_three_layer = text_layer_create(GRect (68, PBL_IF_ROUND_ELSE(64,128)-30, bounds.size.w, 50));
  s_family_four_layer = text_layer_create(GRect (68, PBL_IF_ROUND_ELSE(64,128)-30, bounds.size.w, 50));
  s_background_bitmap =  gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND);


  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_background_color(s_date_layer, GColorClear);
  text_layer_set_background_color(s_family_one_layer, GColorClear);
  text_layer_set_background_color(s_family_two_layer, GColorClear);
  text_layer_set_background_color(s_family_three_layer, GColorClear);
  text_layer_set_background_color(s_family_four_layer, GColorClear);

  text_layer_set_text(s_family_one_layer,"😂");
  text_layer_set_text(s_family_two_layer,"😍");
  text_layer_set_text(s_family_three_layer,"😇");
  text_layer_set_text(s_family_four_layer," ");

  s_background_layer = bitmap_layer_create(bounds);

  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));


  //add the text as a child layer to the root window
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_family_one_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_family_two_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_family_three_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_family_four_layer));
  
  time_t start = time(NULL);
}

static void update_time() {
    time_t temp = time(NULL);

    struct tm * tick_time = localtime(&temp);

    static char s_buffer[8];
    static char s_buffer_date[8];
    strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M": "%I:%M", tick_time);
    strftime(s_buffer_date, sizeof(s_buffer), "%e.%m", tick_time);

    text_layer_set_text(s_time_layer, s_buffer);
    text_layer_set_text(s_date_layer, s_buffer_date);
    layer_mark_dirty(text_layer_get_layer(s_time_layer));
    layer_mark_dirty(text_layer_get_layer(s_date_layer));
}

static void check_overlap() {
  GRect one = layer_get_frame(text_layer_get_layer(s_family_one_layer));
  GRect two = layer_get_frame(text_layer_get_layer(s_family_two_layer));
  GRect three = layer_get_frame(text_layer_get_layer(s_family_three_layer));

  if (grect_equal(&one, &two) && grect_equal(&two, &three)) {
    layer_set_frame(text_layer_get_layer(s_family_one_layer), GRect (one.origin.x - 15, one.origin.y, bounds.size.w, 50));
    layer_set_frame(text_layer_get_layer(s_family_three_layer), GRect (one.origin.x + 15, one.origin.y, bounds.size.w, 50));    
    
  } else {
    if (grect_equal(&one, &two)) {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Overlap true %d", 1);
      layer_set_frame(text_layer_get_layer(s_family_one_layer), GRect (one.origin.x - 7, one.origin.y, bounds.size.w, 50));
      layer_set_frame(text_layer_get_layer(s_family_two_layer), GRect (two.origin.x + 7, two.origin.y, bounds.size.w, 50));
    } else {
      APP_LOG(APP_LOG_LEVEL_DEBUG, "Overlap false %d", 1);    
    }
  
      if (grect_equal(&two, &three)) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Overlap true %d", 1);
        layer_set_frame(text_layer_get_layer(s_family_two_layer), GRect (two.origin.x - 7, two.origin.y, bounds.size.w, 50));
        layer_set_frame(text_layer_get_layer(s_family_three_layer), GRect (three.origin.x + 7, three.origin.y, bounds.size.w, 50));
      } else {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Overlap false %d", 1);    
      }
    
    if (grect_equal(&one, &three)) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Overlap true %d", 1);
        layer_set_frame(text_layer_get_layer(s_family_one_layer), GRect (one.origin.x - 7, one.origin.y, bounds.size.w, 50));
        layer_set_frame(text_layer_get_layer(s_family_three_layer), GRect (three.origin.x + 7, three.origin.y, bounds.size.w, 50));
      } else {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Overlap false %d", 1);    
      }

  }
  
    layer_mark_dirty(text_layer_get_layer(s_family_one_layer));
    layer_mark_dirty(text_layer_get_layer(s_family_two_layer));
    layer_mark_dirty(text_layer_get_layer(s_family_three_layer));
}

static void update_family_member(int family_member, int position) {
    if (family_member == 1) {
      if (position == 0) {
        text_layer_set_text(s_family_one_layer,"😂");
        layer_set_frame(text_layer_get_layer(s_family_one_layer), GRect (68, 35, bounds.size.w, 50));
      } 
      if (position == 1) {
        text_layer_set_text(s_family_one_layer,"😂");
        layer_set_frame(text_layer_get_layer(s_family_one_layer), GRect (110, PBL_IF_ROUND_ELSE(64,128)/2-15, bounds.size.w, 50));
      } 
      if (position == 2) {
        text_layer_set_text(s_family_one_layer,"🙁");
        layer_set_frame(text_layer_get_layer(s_family_one_layer), GRect (110, PBL_IF_ROUND_ELSE(64,128)/2+10, bounds.size.w, 50));
      } 
      if (position == 3) {
        text_layer_set_text(s_family_one_layer,"😂");
        layer_set_frame(text_layer_get_layer(s_family_one_layer), GRect (68, PBL_IF_ROUND_ELSE(64,128)-30, bounds.size.w, 50));
      } 
      if (position == 4) {
        text_layer_set_text(s_family_one_layer,"😂");
        layer_set_frame(text_layer_get_layer(s_family_one_layer), GRect (27, PBL_IF_ROUND_ELSE(64,128)/2+10, bounds.size.w, 50));
      } 
      if (position == 5) {
        text_layer_set_text(s_family_one_layer,"😂");
        layer_set_frame(text_layer_get_layer(s_family_one_layer), GRect (27, PBL_IF_ROUND_ELSE(64,128)/2-15, bounds.size.w, 50));
      } 
    }
    if (family_member == 2) {
      if (position == 0) {
        layer_set_frame(text_layer_get_layer(s_family_two_layer), GRect (68, 35, bounds.size.w, 50));
      } 
      if (position == 1) {
        layer_set_frame(text_layer_get_layer(s_family_two_layer), GRect (110, PBL_IF_ROUND_ELSE(64,128)/2-15, bounds.size.w, 50));
      } 
      if (position == 2) {
        layer_set_frame(text_layer_get_layer(s_family_two_layer), GRect (110, PBL_IF_ROUND_ELSE(64,128)/2+10, bounds.size.w, 50));
      } 
      if (position == 3) {
        layer_set_frame(text_layer_get_layer(s_family_two_layer), GRect (68, PBL_IF_ROUND_ELSE(64,128)-30, bounds.size.w, 50));
      } 
      if (position == 4) {
        layer_set_frame(text_layer_get_layer(s_family_two_layer), GRect (27, PBL_IF_ROUND_ELSE(64,128)/2+10, bounds.size.w, 50));
      } 
      if (position == 5) {
        layer_set_frame(text_layer_get_layer(s_family_two_layer), GRect (27, PBL_IF_ROUND_ELSE(64,128)/2-15, bounds.size.w, 50));
      } 
    }
    if (family_member == 3) {
      if (position == 0) {
        layer_set_frame(text_layer_get_layer(s_family_three_layer), GRect (68, 35, bounds.size.w, 50));
      } 
      if (position == 1) {
        layer_set_frame(text_layer_get_layer(s_family_three_layer), GRect (110, PBL_IF_ROUND_ELSE(64,128)/2-15, bounds.size.w, 50));
      } 
      if (position == 2) {
        layer_set_frame(text_layer_get_layer(s_family_three_layer), GRect (110, PBL_IF_ROUND_ELSE(64,128)/2+10, bounds.size.w, 50));
      } 
      if (position == 3) {
        layer_set_frame(text_layer_get_layer(s_family_three_layer), GRect (68, PBL_IF_ROUND_ELSE(64,128)-30, bounds.size.w, 50));
      } 
      if (position == 4) {
        layer_set_frame(text_layer_get_layer(s_family_three_layer), GRect (27, PBL_IF_ROUND_ELSE(64,128)/2+10, bounds.size.w, 50));
      } 
      if (position == 5) {
        layer_set_frame(text_layer_get_layer(s_family_three_layer), GRect (27, PBL_IF_ROUND_ELSE(64,128)/2-15, bounds.size.w, 50));
      } 
    }
}

static void redraw_all() {
    layer_mark_dirty(text_layer_get_layer(s_family_one_layer));
    layer_mark_dirty(text_layer_get_layer(s_family_two_layer));
    layer_mark_dirty(text_layer_get_layer(s_family_three_layer));
}


static void tick_handler (struct tm * tick_time, TimeUnits units_changed) {
    update_time();
  // IF tick_time->tm_min % 30 send_to_phone_multi
    
}

static void tick_handler_seconds (struct tm * tick_time, TimeUnits units_changed) {
    update_family_member(1, tick_time->tm_sec%6);   
    update_family_member(2, (2*tick_time->tm_sec)%6);    
    update_family_member(3, (3*tick_time->tm_sec)%6); 
    check_overlap();
    redraw_all();
  // IF tick_time->tm_min % 30 send_to_phone_multi
    update_time();
}


static void main_window_unload(Window * window)
{
  text_layer_destroy(s_time_layer);  
  text_layer_destroy(s_date_layer);  
  text_layer_destroy(s_family_one_layer);  
  text_layer_destroy(s_family_two_layer);  
  text_layer_destroy(s_family_three_layer);  
  text_layer_destroy(s_family_four_layer);  
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
  
  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);


 tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
 tick_timer_service_subscribe(SECOND_UNIT, tick_handler_seconds);
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
