#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFF_SIZE 1024
#define LINE_START 3

#define PAIR_CYAN		1
#define PAIR_RED		2
#define PAIR_YELLOW		3
#define PAIR_BLACK		4

typedef struct user_info {
	char	tty[16];	// Terminal line
	char	ip[16];		// User IP
	char	name[16];	// User name
} USER_INFO;

USER_INFO	g_user_info[64];
int			g_user_cnt;

void cprint(int _x, int _y, char *_str, int _str_len, short _color) {
	short pair;
	switch (_color) {
		case COLOR_CYAN:
			pair = PAIR_CYAN;
			break;
		case COLOR_RED:
			pair = PAIR_RED;
			break;
		case COLOR_YELLOW:
			pair = PAIR_YELLOW;
			break;
		case COLOR_BLACK:
			pair = PAIR_BLACK;
			break;
		default:
			break;
	}

	attron(COLOR_PAIR(pair));
	mvprintw(_x,_y,"%-.*s", _str_len, _str);
	attroff(COLOR_PAIR(pair));
}
int main() {
	int		i;
	FILE*	fp;
	char*	login_name;
	char	cmd[128];
	char	buff[BUFF_SIZE];
	char	tty_info[128];
	char	tmp_tty[16];
	char	tmp_ip[16];
	char*	tmp_str_ptr;
	char	tmp_str[128];
	int		tmp_cnt;

	login_name = getenv("LOGNAME");

	// 1. tty
	fp = popen("tty", "r");
	if (fp == NULL) {
		perror("popen(tty) fail");
		return EXIT_FAILURE;
	}
	while (fgets(buff, BUFF_SIZE, fp)) {
		snprintf(tty_info, sizeof(tty_info), "%.*s", (int)(strlen(buff)-1), buff);
	}
	pclose(fp);

	// 2. Who
	fp = popen( "who", "r");
    if ( NULL == fp) {
        perror( "popen(who) fail");
        return EXIT_FAILURE;
    }

	while( fgets( buff, BUFF_SIZE, fp) ) {                                            
        char *c; 
        memset(tmp_tty, 0x00, sizeof(tmp_tty));

        tmp_str_ptr = strstr(buff, "pts");
        tmp_cnt = 0;

        for (c = tmp_str_ptr; *c != ' '; c++) {
            tmp_cnt++;
        }   
        memcpy(tmp_tty, tmp_str_ptr, tmp_cnt);

        snprintf(tmp_ip, sizeof(tmp_ip), "%.*s"
                , (int)(strchr(buff, ')')-strchr(buff, '(')-1), strchr(buff, '(')+1);

        strcpy(g_user_info[g_user_cnt].tty, tmp_tty);
        strcpy(g_user_info[g_user_cnt].ip, tmp_ip);
        if(strcmp(tmp_ip, "100.100.100.36") == 0) {
            strcpy(g_user_info[g_user_cnt].name, "HEH");
        } else if(strcmp(tmp_ip, "100.100.100.39") == 0) {
            strcpy(g_user_info[g_user_cnt].name, "YTS");
        } else if(strcmp(tmp_ip, "100.100.100.40") == 0) {
            strcpy(g_user_info[g_user_cnt].name, "YSE");
        } else if(strcmp(tmp_ip, "100.100.100.77") == 0) {
            strcpy(g_user_info[g_user_cnt].name, "JCW");
        } else {
            strcpy(g_user_info[g_user_cnt].name, "Unkown");
        }       

        g_user_cnt++;
    }       
	pclose(fp);
	
	// 3. ps -f -u fmln
	int ttyCheck;
	int line = 0;

	initscr();
	start_color();
	init_pair(PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);
	init_pair(PAIR_RED, COLOR_RED, COLOR_BLACK);
	init_pair(PAIR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
	init_pair(PAIR_BLACK, COLOR_BLACK, COLOR_BLACK);
	for (i=0; ; i++) {
		clear();
		snprintf(tmp_str, sizeof(tmp_str), "* User[%s], TTY[%s]", login_name, tty_info);
		cprint(1,LINE_START,tmp_str, strlen(tmp_str), COLOR_CYAN);
		snprintf(tmp_str, sizeof(tmp_str), "%-10s%-18s%-10s%s", "TTY", "IP", "USER", "CMD");
		cprint(3,LINE_START,tmp_str, strlen(tmp_str), COLOR_CYAN);
		line = 0;
		snprintf(cmd, sizeof(cmd), "ps -f -u %s", login_name);
		fp = popen(cmd, "r");
		if ( NULL == fp) {
			perror( "popen() fail");
			return -1;
		}
		while( fgets( buff, BUFF_SIZE, fp) ) {
			char *c;
			memset(tmp_tty, 0x00, 16);

			if(strstr(buff, "bash") == NULL
					&& strstr(buff, "sshd") == NULL
					&& strstr(buff, "show_proc") == NULL
					&& strstr(buff, cmd) == NULL
					&& strstr(buff, "UID") == NULL) {
				if ((tmp_str_ptr = strstr(buff, "pts")) == NULL) {
					continue;
				}
				tmp_cnt = 0;

				for (c = tmp_str_ptr; *c != ' '; c++) {
					tmp_cnt++;
				}
				memcpy(tmp_tty, tmp_str_ptr, tmp_cnt);
				for(ttyCheck=0; ttyCheck<g_user_cnt; ttyCheck++) {
					if(strcmp(g_user_info[ttyCheck].tty, tmp_tty) == 0) {
						cprint(5+line,LINE_START,g_user_info[ttyCheck].tty,10,COLOR_YELLOW);
						cprint(5+line,LINE_START+10,g_user_info[ttyCheck].ip,18,COLOR_YELLOW);
						cprint(5+line,LINE_START+28,g_user_info[ttyCheck].name,10,COLOR_YELLOW);
						break;
					}
				}
				if(ttyCheck == g_user_cnt) {
					continue;
				}
				cprint(5+line,LINE_START+38,buff+48,30,COLOR_YELLOW);
				line++;
			}
		}
		refresh();
		pclose(fp);
		/*getch();*/
		sleep(1);
	}
	endwin();

	return 0;
}

