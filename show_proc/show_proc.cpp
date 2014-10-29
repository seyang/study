#include <stdio.h>
#include <string.h>
#include <map>
#include <iostream>

#define  BUFF_SIZE   1024

using namespace std;

typedef struct user_info {
	char pts[16];
	char ip[16];
	char name[16];
} USER_INFO;

USER_INFO uInfo[64];
int userCnt = 0;

int main( void)
{
	char  buff[BUFF_SIZE];
	FILE *fp;
	char *tmpStr;
	char tmpPts[16];
	char tmpIP[16];
	int tmpCnt;

	// 1. tty 
	fp = popen( "tty", "r");
	if ( NULL == fp) {
		perror( "popen() fail");
		return -1;
	}

	while( fgets( buff, BUFF_SIZE, fp) ) {
		printf("\n * Current TTY(TeleTYpewriter): [%.*s]\n\n", (strlen(buff)-1), buff);
	}

	pclose( fp);

	// 2. print menu
	printf("%-10s%-18s%-10s%s\n", "TTY", "IP", "USER", "CMD");

	// 3. who
	fp = popen( "who", "r");
	if ( NULL == fp) {
		perror( "popen() fail");
		return -1;
	}

	while( fgets( buff, BUFF_SIZE, fp) ) {
		char *c;
		memset(tmpPts, 0x00, 16);

		tmpStr = strstr(buff, "pts");
		tmpCnt = 0;

		for (c = tmpStr; *c != ' '; c++) {
			tmpCnt++;
		}
		memcpy(tmpPts, tmpStr, tmpCnt);

		snprintf(tmpIP, 16, "%.*s"
				, (int)(strchr(buff, ')')-strchr(buff, '(')-1), strchr(buff, '(')+1);

		strcpy(uInfo[userCnt].pts, tmpPts);
		strcpy(uInfo[userCnt].ip, tmpIP);
		if(strcmp(tmpIP, "100.100.100.36") == 0) {
			strcpy(uInfo[userCnt].name, "HEH");
		} else if(strcmp(tmpIP, "100.100.100.40") == 0) {
			strcpy(uInfo[userCnt].name, "YSE");
		} else {
			strcpy(uInfo[userCnt].name, "Unkown");
		}

		userCnt++;
	}

	pclose( fp);


	// 4. ps -f -u fmln
	int ptsCheck;

	fp = popen( "ps -f -u fmln", "r");
	if ( NULL == fp)
	{
		perror( "popen() fail");
		return -1;
	}

	while( fgets( buff, BUFF_SIZE, fp) ) {
		char *c;
		memset(tmpPts, 0x00, 16);

		if(strstr(buff, "bash") == NULL
				&& strstr(buff, "sshd") == NULL
				&& strstr(buff, "show_proc") == NULL
				&& strstr(buff, "ps -f -u fmln") == NULL
				&& strstr(buff, "UID") == NULL) {
			tmpStr = strstr(buff, "pts");
			tmpCnt = 0;

			for (c = tmpStr; *c != ' '; c++) {
				tmpCnt++;
			}
			memcpy(tmpPts, tmpStr, tmpCnt);
			for(ptsCheck=0; ptsCheck<userCnt; ptsCheck++) {
				if(strcmp(uInfo[ptsCheck].pts, tmpPts) == 0) {
					if(strcmp(uInfo[ptsCheck].name, "YSE") == 0) {
						printf("%-10s%-18s\e[32m%-10s\e[m"
								, uInfo[ptsCheck].pts, uInfo[ptsCheck].ip
								, uInfo[ptsCheck].name);
					} else {
						printf("%-10s%-18s\e[33m%-10s\e[m"
								, uInfo[ptsCheck].pts, uInfo[ptsCheck].ip
								, uInfo[ptsCheck].name);
					}
					break;
				}
			}
			if(ptsCheck == userCnt) {
				continue;
			}
			printf("%s", buff+48);
		}
	}

	pclose( fp);

	printf("\n");

	return 0;
}
