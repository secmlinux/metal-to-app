#include <stddef.h>
#include <stdint.h>
#include "print.h"

extern char KERNEL_BASE[1];
#define vga_begin ((char volatile*)((char*)0xB8000 + (uintptr_t)KERNEL_BASE))

int cwrite(ptrdiff_t screen_offset, char* message) {
    char volatile* start = vga_begin + 2 * screen_offset;
    int written = 0;
    while (message[written] != 0) {
        start[written * 2] = message[written];
        start[written * 2 +1] = 0xf;
        written++;
    }
    return written;
}

static ptrdiff_t current_offset = 80;
void print(char message[]) {
    size_t writtenChars = cwrite(current_offset, message);
    current_offset += writtenChars;
    current_offset = current_offset % (80 * 25);
}
void println(char message[]) {
    print(message);
    current_offset = (current_offset + 79)/80 * 80;
}
