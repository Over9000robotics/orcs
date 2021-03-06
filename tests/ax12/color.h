/**
 * @file color.h
 * @brief Defines all of the ANSI terminal escape codes that modify the color of text.
 */

#ifndef COLOR_H
#define COLOR_H

#define BLACK_TEXT 	 "\033[30;1m"
#define RED_TEXT 	 "\033[31;1m"
#define GREEN_TEXT 	 "\033[32;1m"
#define YELLOW_TEXT  "\033[33;1m"
#define BLUE_TEXT 	 "\033[34;1m"
#define MAGENTA_TEXT "\033[35;1m"
#define CYAN_TEXT 	 "\033[36;1m"
#define WHITE_TEXT 	 "\033[37;1m"
#define ORANGE_TEXT	 "\033[40;1m"

#define BOLD_BLACK_TEXT 	"\033[1m\033[30m;1m"
#define BOLD_RED_TEXT 		"\033[1m\033[31m;1m"
#define BOLD_GREEN_TEXT 	"\033[1m\033[32m;1m"
#define BOLD_YELLOW_TEXT 	"\033[1m\033[33m;1m"
#define BOLD_BLUE_TEXT 		"\033[1m\033[34m;1m"
#define BOLD_MAGENTA_TEXT 	"\033[1m\033[35m;1m"
#define BOLD_CYAN_TEXT 		"\033[1m\033[36m;1m"
#define BOLD_WHITE_TEXT 	"\033[1m\033[37m;1m"

void print_red(void);
void print_yellow(void);
void print_blue(void);
void print_green(void);
void print_orange(void);
void print_cyan(void);
void print_reset(void);

void print_yellow_bold(void);
void print_green_bold(void);
void print_blue_bold(void);
void print_red_bold(void);

#endif // COLOR_H
