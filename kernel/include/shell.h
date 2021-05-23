
#ifndef _SHELL_H
#define _SHELL_H

// executables header
#define OSS_HDR      "OSS"
#define OSS_HDR_LEN   3

void init_shell(void);
int send_tty(char c);

static const char *shell_hdr = "OS S v1.17, x86_64 kernel\n" \
    "\n";

#endif
