#include <cstdio>
#include <cstdarg>

#define printx(msg) \
	printf("[%s:%03d] %s", __FILE__, __LINE__, msg);

#define printz(arg...) \
	printf("[%s/%s:%03d] ", __FILE__, __func__, __LINE__); \
	printf(arg);

void redPrintf(const char *format, ...) {
	va_list arg;
	char tmpStr[1024];

	va_start(arg, format);
	//vprintf(format, arg);
	vsnprintf(tmpStr, 1024, format, arg);
	va_end(arg);

	printf("\e[31m%s\e[0m", tmpStr);
}

void printy() {
	printf("[FILE: %s]\n", __FILE__);
	printf("[LINE: %d]\n", __LINE__);
	printf("[FUNC: %s]\n", __func__);
	printf("[FUNC: %s]\n", __FUNCTION__);
	printf("[DATE: %s]\n", __DATE__);
	printf("[TIME: %s]\n", __TIME__);
}

#define NARGS_SEQ(_1,_2,_3,_4,_5,_6,_7,_8,N,...) N
#define NARGS(...) NARGS_SEQ(__VA_ARGS__, 8, 7, 6, 5, 4, 3, 2, 1)
#define fn(...) fnexec(NARGS(__VA_ARGS__) - 1, __VA_ARGS__)

static void fnexec(int n, const char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	printf("%d params received\n", n);
	vprintf(fmt, args);
	va_end(args);
}

int main() {

	printf("\n");
	printx("Printx test\n");
	printf("\n");

	printy();
	printf("\n");

	printz("%d: %s\n", 1, "test");
	printf("\n");

	redPrintf("red msg\n");
	printf("\n");

	fn(">> %s %d %f\n", "Hello", 7, 5.1);

	return 0;
}
