#ifndef SERVER_H
#define SERVER_H

enum {
  MAX_NOTIFICATIONS = 512,
  MAX_RESPONSES = 512,
  MAX_NEOFUNC_ARGUMENTS = 16,
  MAX_NEOFUNCTIONS = 512
}

/* gled_notification is a struct containing the data returned by a NOTIFY */
typedef struct {
  char name[31];
  msgpack_object args;
}gled_notification;

/* gled_response is a struct containing the data returned by a RESPONSE */
typedef struct {
  char str[16];
}gled_response;

/* gled_neofunc_param contains the data of one neovim function call argument */
typedef struct {
  char type[32];
  char name[32];
}gled_neofunc_param;

/* gled_neofunc is a struct containing the data for a neovim function call */
typedef struct {
  char name[32];
  char res_type[32];
  uint64_t id;
  gled_neofunc_param args[MAX_NEOFUNC_ARGUMENTS];
}gled_neofunc;


/* gled_handle_messages flushes the notify and response queues */
void gled_handle_messages();

/* gled_current receives data from neovim (vim_get_current_<prop>) */
void gled_current(const char *prop);

/* gled_request calls the given neovim function with the provided arguments */
uint64_t gled_request(uint64_t id, msgpack_object *args);

gled_notification * gled_server_poll_notification();
gled_response * gled_server_poll_responses();

#endif
