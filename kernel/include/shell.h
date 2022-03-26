
#ifndef _SHELL_H
#define _SHELL_H

#include <fs.h>
#include <kboot.h>

// executables header
#define OSS_HDR      "OSS"
#define OSS_HDR_LEN   3

#define	EOSS_HDR		"\177ELF"
#define	EOSS_HDR_LEN	4

#define SHELL_HDR "OS S v1.17, x86_64 kernel\n\n"

void init_shell(void);
int send_tty(char c);

int exec(file_t *, int ac, char **av);

void do_reset(reset_t reset, reset_type_t type);

#endif
