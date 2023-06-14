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

#include "src/webserver.h"
#include "src/stereo_camera.h"



int main(void) {

  StereoCamera * camera;

#if __APPLE__
  camera = new StereoCamera();
  camera->capture();
#else
  camera = nullptr;
#endif

  WebServer server(camera);

  struct mg_mgr mgr;                            // Event manager
  mg_log_set(MG_LL_DEBUG);              // Set log level
  mg_mgr_init(&mgr);                            // Initialise event manager
  mg_http_listen(&mgr, s_http_addr, server.web_event_handler, NULL);  // Create HTTP listener
  for (;;) mg_mgr_poll(&mgr, 1000);                    // Infinite event loop
  mg_mgr_free(&mgr);

  // camera->release();
  return 0;
}