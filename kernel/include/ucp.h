//ucp
#ifndef _ucp_h_
#define _ucp_h_

#define IS_FLAG 0x8000000000000000
#define IS_SET(str) ((char *)((u64)str | (u64)IS_FLAG))

int ucp(char *str, char *regex, ...);
char ***ucp_tab(char *str, char *regex, ...);
void ucp_free(char ***tab);

void *is_made1(char *str);
void *is_made2(char *str);
void *is_made3(char *str);
void *is_made4(char *str);
void *is_made5(char *str);

#endif
