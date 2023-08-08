#include "interface.h"

void interface_task()
{

    // Check if data is available on stdin

    ssize_t n = read(STDIN_FILENO, buffer, sizeof(buffer));
    if (n > 0)
    {
        update = n;
        interface_string = const_cast<char *>(data_received);
    }
    else
    {
        update = 0;
    }
}

void initialise_interface()
{
    // STDIN to non-blocking mode
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    received_proto = KBD_Test_init_zero;
}