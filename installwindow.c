#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <panel.h>

int startx = 0;
int starty = 0;

int newwinheight = 20;
int newwinwidth = 70;

char *choices[] = {
	"Guided - use entire disk",
	"Guided - use entire disk and set up LVM",
	"Guided - use entire disk and set up encrypted LVM",
	"Manual",
	"<Go Back>"
	};
	
char title[] = {
	"| [!!] Partition Disks |"
};

char testmsg[] = {
	"The installer can guide you through partitioning a disk (using different standard schemes), or if you prefer, you can do it manually. With guided partitioning you will still have a chance later to review and customise the results. If you choose guided partitioning for an entire disk, you will be asked which disk should be used. Partitioning method:"
};

int n_choices = sizeof(choices) / sizeof(char *);
void print_menu(WINDOW *menu_win, int highlight);
int print_margin(WINDOW *win, int starty, int startx, int lmargin, int rmargin, char message[]);

int main(int argc, char* argv[]){
	
	WINDOW *menu_win;
	int choice = 0;
	int highlight = 1;
	
	initscr();
	clear();
	noecho();
	cbreak();
	
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
	init_pair(3, COLOR_RED, COLOR_WHITE);
	wbkgd(stdscr, COLOR_PAIR(1));
	
	mvprintw(0, 0, "My terminal supports %d colours. \n", COLORS);
	
	menu_win = newwin(newwinheight, newwinwidth, 2, 5);
	keypad(menu_win, TRUE);
	refresh();
	print_menu(menu_win, highlight);
	
	while(1){
	
		int c = wgetch(menu_win);
		
		switch(c){
			
			case KEY_UP:
				if(highlight == 1)
					highlight = n_choices;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == n_choices)
					highlight = 1;
				else
					++highlight;	
				break;
			case 10:
				choice = highlight;
				mvprintw(23, 0, "You chose choice %d which means %s", choice, choices[choice - 1]);
				break;

			
		}
	
		print_menu(menu_win, highlight);
		refresh();
	
	}
	
	clrtoeol();
	getch();
	endwin();
	
}

void print_menu(WINDOW *menu_win, int highlight){
	int x, y, i;
	int h, w;
	getmaxyx(menu_win, h, w);
	
	x = 2;
	y = 2; 

	box(menu_win, 0, 0);
	wbkgd(menu_win, COLOR_PAIR(2));
	
	wattron(menu_win, COLOR_PAIR(3));
	mvwprintw(menu_win, 0, ((newwinwidth - strlen(title)) / 2), "%s", title);
	wattroff(menu_win, COLOR_PAIR(3));
	
	y = print_margin(menu_win, 2, 2, 2, 2, testmsg) + 2;
	mvprintw(2, 0, "%d", strlen(choices[2]));
	refresh();
	
	for(int i = 0; i < n_choices - 1; i++){
		
		if(highlight == i + 1){
			
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y,  ((newwinwidth - 49) / 2), "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
			
		} else {
		
			mvwprintw(menu_win, y, ((newwinwidth - 49) / 2), "%s", choices[i]);
		
		}
		
		y++;
		
	}
	
	y = y + 2;
	
	if(highlight == n_choices){
		
		wattron(menu_win, A_REVERSE);
		mvwprintw(menu_win, y,  5, "%s", choices[n_choices - 1]);
		wattroff(menu_win, A_REVERSE);
		
	} else {
		
		mvwprintw(menu_win, y,  5, "%s", choices[n_choices - 1]);
		
	}
	
	wrefresh(menu_win);

}

int print_margin(WINDOW *win, int starty, int startx, int lmargin, int rmargin, char message[]){

	int x = startx;
	int y = starty;
	int h, w;
	getmaxyx(win, h, w);
	int marginwidth = w - lmargin - rmargin;
	
	char buffer[marginwidth];
	
	int j = 0;
	
	for(int i = 0; i < strlen(message); i){

		if(message[i] != ' ' || message[i] != '\0'){
			buffer[j] = message[i];
			j++;
			i++;
		}
		
		if(x + j > marginwidth){
			y++;
			x = lmargin;
		}
		
		if(message[i] == '\n'){
			buffer[j+1] = ' ';
			buffer[j+2] = '\0';
			i++;
			y++;
			x = lmargin;
			mvwprintw(win, y, x, buffer);
			j = 0;
			memset(buffer, 0, sizeof buffer);
			
		}
		
		if(message[i] == ' ' || message[i] == '\0'){
			buffer[j+1] = ' ';
			buffer[j+2] = '\0';
			i++;
			mvwprintw(win, y, x, buffer);
			x = x + j + 1;
			j = 0;
			memset(buffer, 0, sizeof buffer);
			
		}
		
	}
	
	mvprintw(1, 0, "%d", y);
	
	return y;

}
