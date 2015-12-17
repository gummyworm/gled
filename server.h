#ifndef SERVER_H
#define SERVER_H

enum {
  MAX_NOTIFICATIONS = 512,
  MAX_RESPONSES = 512,
}

typedef struct {
  char name[31];
  msgpack_object args;
}gled_notification;

typedef struct {
  char str[16];
  
}gled_response;

/* gled_handle_messages flushes the notify and response queues */
void gled_handle_messages();

/* gled_current receives data from neovim (vim_get_current_<prop>) */
void gled_current(const char *prop);

/* gled_request calls the given neovim function with the provided arguments */
uint64_t gled_request(uint64_t id, msgpack_object *args);

gled_notification * gled_server_poll_notification();
gled_response * gled_server_poll_responses();

#endif
