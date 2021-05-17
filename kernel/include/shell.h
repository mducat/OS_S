
#ifndef _SHELL_H
#define _SHELL_H

void init_shell(void);
int send_tty(char c);

static const char *shell_hdr = "OS S v1.17, x86_64 kernel\n" \
    "\n";

#endif
