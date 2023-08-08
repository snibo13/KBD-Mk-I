#include "messages/status.pb.h"

#include "fcntl.h"
#include "unistd.h"

#define BUFFER_SIZE 256

static uint8_t buffer[BUFFER_SIZE];

static KBD_Test received_proto;

static int update = 0;
static const char *no_data_received = "No protobuf received";
static const char *data_received = "Data received";
static char *interface_string;

void initialise_interface();
void interface_task();