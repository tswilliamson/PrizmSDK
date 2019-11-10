// Shaun McFall, 2012
// Everything is just assuming ASCII right now...

#include <ctype.h>

int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

int isalpha(int c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int isascii(int c) {
    return c < 128;
}

int isblank(int c) {
    return c == ' ' || c == '\t';
}

int iscntrl(int c) {
    return c >=0 && c < 31;
}

int isdigit(int c) {
    return c >= '0' && c <= '9';
}

int isgraph(int c) {
    return c > ' ' && c <= '~';
}

int islower(int c) {
    return c >= 'a' && c <= 'z';
}

int isprint(int c) {
    return c >= ' ' && c <= '~';
}

int ispunct(int c) {
    return isgraph(c) && !isalnum(c);
}

int isspace(int c) {
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}

int isxdigit(int c) {
    return isdigit(c) || ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
}


int toupper(int c) {
    return islower(c) ? c - 32 : c;
}

int tolower(int c) {
    return isupper(c) ? c + 32 : c;
}
