#include <jansson.h>
#include <cstdio>

int main() {
	char *str;
	json_t *root;
	json_error_t error;

	root = json_load_file("error.json", 0, &error);
	if(!root) {
		printf("[ERR] json_load_file\n");
		/* the error variable contains error information */
	}

	if(!json_is_array(root)){
		printf("[ERR] json_is_array\n");
	}

	for (int i=0; i<json_array_size(root); i++) {
		json_t *data, *errnum, *errstr;

		data = json_array_get(root, i);

		errnum = json_object_get(data, "errnum");
		if (!json_is_integer(errnum)) {
			printf("[ERR] json_is_integer\n");
		}

		errstr = json_object_get(data, "errstr");
		if (!json_is_string(errstr)) {
			printf("[ERR] json_is_string\n");
		}

		printf("%d: \"%s\"\n", json_integer_value(errnum), json_string_value(errstr));
	}

	//root = json_pack("{s:s, s:i}", "greeting", "Hello, World!", "number", 42);
	//str = json_dumps(root, JSON_ENCODE_ANY);

	//printf("str = %s\n", str);

	return 0;
}
