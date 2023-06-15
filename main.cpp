// Copyright (c) 2020 Cesanta Software Limited
// All rights reserved
//
// HTTP server example. This server serves both static and dynamic content.
// It opens two ports: plain HTTP on port 8000 and HTTP on port 8443.
// It implements the following endpoints:
//    /api/stats - respond with free-formatted stats on current connections
//    /api/f2/:id - wildcard example, respond with JSON string {"result": "URI"}
//    any other URI serves static files from s_root_dir
//
// To enable SSL/TLS (using self-signed certificates in PEM files),
//    1. See https://mongoose.ws/tutorials/tls/#how-to-build
//    2. curl -k https://127.0.0.1:8443
// https://towardsdatascience.com/torch-and-torchvision-c-installation-and-debugging-on-linux-263676c38fa2

#include "stereo_camera.h"
#include <unistd.h>
#include "nanoutils.h"
#include <vector>
#include "globals.h"
#include "parson.h"
#include "mongoose.h"

enum CameraId {left, right, stereo};

static const char *s_http_addr = "http://0.0.0.0:8000";    // HTTP port
static const char * JSON_HEADER = (
  "Access-Control-Allow-Origin: *\r\n"
  "Content-Type: application/json\r\n");
static const char * JPEG_HEADER = "Content-Type: image/jpeg\r\n";
static const char * MAIN_HEADER = ("HTTP/1.0 200 OK\r\n"
          "Cache-Control: no-cache\r\n"
          "Pragma: no-cache\r\nExpires: Thu, 01 Dec 1994 16:00:00 GMT\r\n"
          "Access-Control-Allow-Origin: *\r\n");

static void add_default_headers(struct mg_connection *c){
  mg_printf(c, MAIN_HEADER);
} 

static void add_jpeg_header(struct mg_connection *c) {
  mg_printf(c, JPEG_HEADER);
}

static void add_json_header(struct mg_connection *c) {
  mg_printf(c, JSON_HEADER);
}

static void add_size_header(struct mg_connection *c, size_t size) {
  mg_printf(c,"Content-Length: %lu\r\n\r\n", size);
}

          
static void handle_home(struct mg_connection *c) {
  JSON_Value *root_value = json_value_init_object();
  JSON_Object *root_object = json_value_get_object(root_value);
  char *body = NULL;
  json_object_set_string(root_object, "status", "Hello");
  body = json_serialize_to_string(root_value);
  mg_http_reply(c, 200, JSON_HEADER, body);
  json_free_serialized_string(body);
  json_value_free(root_value);

}


static void handle_not_found(struct mg_connection *c) {
  JSON_Value *root_value = json_value_init_object();
  JSON_Object *root_object = json_value_get_object(root_value);
  char *body = NULL;
  json_object_set_string(root_object, "status", "Not Found");
  body = json_serialize_to_string(root_value);
  mg_http_reply(c, 200, JSON_HEADER,
                body);
  json_free_serialized_string(body);
  json_value_free(root_value);
}

static void handle_get_image(struct mg_connection *c, CameraId id) {
  std::vector<uchar> buf;
  switch(id) {
    case CameraId::left:
      nano::bgr8_to_jpeg(camera->value1, buf);
      break;
    case CameraId::right:
      nano::bgr8_to_jpeg(camera->value2, buf);
      break;
    default:
      nano::stereo_to_jpeg(camera->value1, camera->value2, buf);
  }

  size_t size = buf.size();

  add_default_headers(c);
  add_jpeg_header(c);
  add_size_header(c, size);
  mg_send(c, (char*)buf.data(), size);
  mg_send(c, "\r\n", 2);
  
}

static void web_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_HTTP_MSG) {
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    if (mg_http_match_uri(hm, "/")) {
      handle_home(c);
    } else if (mg_http_match_uri(hm, "/api/video1")) {
    c->data[0] = 'S';  // Mark that connection as live streamer
    mg_printf(
        c, "%s",
        "HTTP/1.0 200 OK\r\n"
        "Cache-Control: no-cache\r\n"
        "Pragma: no-cache\r\nExpires: Thu, 01 Dec 1994 16:00:00 GMT\r\n"
        "Content-Type: multipart/x-mixed-replace; boundary=--foo\r\n\r\n");
  } else if(mg_http_match_uri(hm, "/api/images/left"))  {
    handle_get_image(c, CameraId::left);
  } else if(mg_http_match_uri(hm, "/api/images/right"))  {
    handle_get_image(c, CameraId::right);
  } else if(mg_http_match_uri(hm, "/api/images/stereo"))  {
    handle_get_image(c, CameraId::stereo);
  }else {
      handle_not_found(c);
    }
  }
  (void) fn_data;
}

int main(void) {

  camera = new StereoCamera();


  camera->run();


  struct mg_mgr mgr;                            // Event manager
  mg_log_set(MG_LL_DEBUG);              // Set log level
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_listen(&mgr, s_http_addr, web_event_handler, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);                    // Infinite event loop
  mg_mgr_free(&mgr);
  
  camera->stop();
  camera->release();

  return 0;
}
