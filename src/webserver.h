#include "mongoose.h"
#include "stereo_camera.h"
#include "parson.h"

static const char *s_http_addr = "http://0.0.0.0:8000";    // HTTP port
static const char *s_https_addr = "https://0.0.0.0:8443";  // HTTPS port
static const char *s_root_dir = ".";

static const char * JSON_HEADER = "Content-Type: application/json\r\n";

class WebServer {
 public:

  StereoCamera * camera;

  WebServer(StereoCamera * camera) : camera(camera) {
    printf("Webserver initiated");
  }

  static void handle_home(struct mg_connection *c) {
    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);
    char *body = NULL;
    json_object_set_string(root_object, "status", "Hello");
    body = json_serialize_to_string(root_value);
    mg_http_reply(c, 200, JSON_HEADER,
                  body);
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

  static void web_event_handler(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
      struct mg_http_message *hm = (struct mg_http_message *) ev_data;
      if (mg_http_match_uri(hm, "/")) {
        handle_home(c);
      } else {
        handle_not_found(c);
      }
    }
    (void) fn_data;
  }
};