#include <stdio.h>
#include <stdarg.h>

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

int main(void)
{
	fn("%s %d %f\n", "Hello", 7, 5.1);
	return 0;
}
