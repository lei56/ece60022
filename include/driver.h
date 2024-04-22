#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <dirent.h>
#include <errno.h>
#include "arguments.h"
#include "buffer.h"
#include "cdn.h"
#include "edge.h"
#include "flow.h"
#include "link.h"
#include "links.h"
#include "memory.h"
#include "packet.h"
#include "params.h"
#include "transmitter.h"
#include "user.h"

arguments_t * process_args(int argc, char ** argv);
void read_tracefile(char * filename);
void initialize_network(arguments_t * arguments);
void free_network();

#endif