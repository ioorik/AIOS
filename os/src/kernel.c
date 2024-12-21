#include <stdint.h>

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Define color codes
#define BLACK 0
#define BLUE 1
#define GREEN 2
#define CYAN 3
#define RED 4
#define MAGENTA 5
#define BROWN 6
#define LIGHT_GRAY 7
#define DARK_GRAY 8
#define LIGHT_BLUE 9
#define LIGHT_GREEN 10
#define LIGHT_CYAN 11 
#define LIGHT_RED 12
#define LIGHT_MAGENTA 13
#define LIGHT_YELLOW 14
#define WHITE 15

// VGA memory pointer
volatile uint16_t *vga_buffer = (uint16_t *)VGA_ADDRESS;

#define KEYBOARD_PORT 0x60  // The I/O port for keyboard data
#define KEYBOARD_CTRL_PORT 0x64  // The I/O port for keyboard control

int x = 0;
int y = 0;
int shift = 0;

// Declare an interrupt handler for IRQ1 (keyboard interrupt)
extern void keyboard_interrupt_handler();

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    asm volatile("inb %1, %0" : "=a"(result) : "d"(port));
    return result;
}

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" : : "a"(value), "d"(port));
}

char* itoa(int val, int base) {
    static char buf[32] = {0};
    int i = 30;
    for (; val && i; --i, val /= base)
        buf[i] = "0123456789abcdefhijklmnopqrstuvwxyz"[val % base];
    return &buf[i + 1];
}

void putchar(char c, int x, int y, int bgcolor, int fgcolor) {
    uint16_t color = (bgcolor << 12) | (fgcolor << 8);
    vga_buffer[y * VGA_WIDTH + x] = color | c; // Write character at (x, y)
}

char scancode_to_char(uint8_t scancode) {
    // Handle keys as numbers or letters (add more as needed)
    if (scancode == 2) return '1';
    if (scancode == 3) return '2';
    if (scancode == 4) return '3';
    if (scancode == 5) return '4';
    if (scancode == 6) return '5';
    if (scancode == 7) return '6';
    if (scancode == 8) return '7';
    if (scancode == 9) return '8';
    if (scancode == 10) return '9';
    if (scancode == 11) return '0';

    if (scancode == 12) return '-';
    if (scancode == 13) return '=';

    if (scancode == 14) return '\x08';
    if (scancode == 15) return '\t';

    if (scancode == 30) return 'a';
    if (scancode == 48) return 'b';
    if (scancode == 46) return 'c';
    if (scancode == 32) return 'd';
    if (scancode == 18) return 'e';
    if (scancode == 33) return 'f';
    if (scancode == 34) return 'g';
    if (scancode == 35) return 'h';
    if (scancode == 23) return 'i';
    if (scancode == 36) return 'j';
    if (scancode == 37) return 'k';
    if (scancode == 38) return 'l';
    if (scancode == 50) return 'm';
    if (scancode == 49) return 'n';
    if (scancode == 24) return 'o';
    if (scancode == 25) return 'p';
    if (scancode == 16) return 'q';
    if (scancode == 19) return 'r';
    if (scancode == 31) return 's';
    if (scancode == 20) return 't';
    if (scancode == 22) return 'u';
    if (scancode == 47) return 'v';
    if (scancode == 17) return 'w';
    if (scancode == 45) return 'x';
    if (scancode == 21) return 'y';
    if (scancode == 44) return 'z';

    if (scancode == 58) return '\xF0';

    if (scancode == 28) return '\n';

    return '\0';  // Return null character if no match
}

void poll_keyboard() {
    if (inb(KEYBOARD_CTRL_PORT) & 0x01) {  // Check if data is available
        uint8_t scancode = inb(KEYBOARD_PORT);
        if (scancode) process_key(scancode);
        else return 0;
    }
}

char uppercase(char c)
{
    return c -('a'-'A');
}

void process_key(uint8_t scancode) {
    char c = scancode_to_char(scancode);

    if (c == 'w') {
        put_pixel(x, y, BLACK);
        y--;
        putchar('\x07', x, y, BLACK, WHITE);
    }
    else if (c == 's') {
        put_pixel(x, y, BLACK);
        y++;
        putchar('\x07', x, y, BLACK, WHITE);
    }
    else if (c == 'a') {
        put_pixel(x, y, BLACK);
        x--;
        putchar('\x07', x, y, BLACK, WHITE);
    }
    else if (c == 'd') {
        put_pixel(x, y, BLACK);
        x++;
        putchar('\x07', x, y, BLACK, WHITE);
    }
    putchar('\033[?25l', 0, 0, BLACK, BLACK);
}

void init_keyboard() {
    // Enable the keyboard port (0x60) for reading data
    outb(KEYBOARD_CTRL_PORT, 0xAE);  // Enable the keyboard (0xAE is the "enable scanning" command)

    // Enable interrupts for the keyboard (IRQ1)
    outb(KEYBOARD_CTRL_PORT, 0x20);  // Get the current status
    uint8_t status = inb(KEYBOARD_CTRL_PORT);
    outb(KEYBOARD_CTRL_PORT, status | 0x01);  // Enable the keyboard IRQ (IRQ1)
}

void fill_screen(int color) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = (color << 12) | (WHITE << 8) | ' ';
    }
}

void put_pixel(int x, int y, int color) {
    // Clear the pixel by writing a space character (' ') with a background color
    // You can use any color you want for the background. I'll use black (0) and white (15) for the foreground.
    vga_buffer[y * VGA_WIDTH + x] = (color << 12) | (WHITE << 8) | ' ';  // Clear the pixel (x, y)
}

void kernel_main() {
    init_keyboard();
    fill_screen(BLACK);
    x = 0;
    y = 0;
    putchar('\033[?25l', 0, 0, BLACK, BLACK);
    while (1) {
        poll_keyboard();

    }
}