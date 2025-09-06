#include "defines.h"
#include "gpiod.h"


#ifndef ARM64

int gpiod_line_event_get_fd(struct gpiod_line * /*line*/)
{
    return 0;
}

struct gpiod_chip *gpiod_chip_open_by_name(const char *name)
{
    LOG_WRITE("Need chip for name %s", name);

    return nullptr;
}

struct gpiod_line *gpiod_chip_get_line(struct gpiod_chip * /*chip*/, unsigned int /*offset*/)
{
    return nullptr;
}

void gpiod_chip_close(struct gpiod_chip * /*chip*/)
{
}

int gpiod_line_request_output(struct gpiod_line * /*line*/, const char * /*consumer*/, int /*default_val*/)
{
    return 0;
}

int gpiod_line_request_input_flags(struct gpiod_line * /*line*/, const char * /*consumer*/, unsigned int /*flags*/)
{
    return 0;
}

int gpiod_line_request_both_edges_events_flags(struct gpiod_line * /*line*/, const char * /*consumer*/, unsigned int /*flags*/)
{
    return 0;
}

int gpiod_line_get_value(struct gpiod_line * /*line*/)
{
    return 0;
}

void gpiod_line_release(struct gpiod_line * /*line*/)
{
}

int gpiod_line_event_read(gpiod_line *, struct gpiod_line_event * /*event*/)
{
    return 0;
}

int gpiod_line_set_value(struct gpiod_line * /*line*/, int /*value*/)
{
    return 0;
}

#endif
