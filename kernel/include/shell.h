
#ifndef _SHELL_H
#define _SHELL_H

// executables header
#define OSS_HDR      "OSS"
#define OSS_HDR_LEN   3

#define SHELL_HDR "OS S v1.17, x86_64 kernel\n\n"

void init_shell(void);
int send_tty(char c);

#endif
