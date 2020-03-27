#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_get_request(char *host, char *url, char *url_params, char *cookie1, char *cookie2, char *token);
char *compute_post_request(char *host, char *url, char *type, char *cookie1, char *cookie2, char *form_data, char *token);

#endif