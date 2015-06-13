#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "json-parser.h"

void print_level(int level, const char* str, ...) {
	va_list	args;
	for( int i = 0; i < level; ++i ) {
		printf("    ");
	}

	va_start(args, str);
	vprintf(str, args);
	va_end(args);
}

void
dump(json_value_t* v, int level) {
	switch( v->tag ) {
	case JSON_ARRAY:
		print_level(level, "[");
		for( size_t i = 0; i < v->value.array.count; ++i ) {
			dump(v->value.array.array[i], 0);

			if( i < v->value.array.count - 1) {
				printf(", ");
			}
		}

		print_level(level, "]\n");
		break;

	case JSON_BOOLEAN:
		if( v->value.boolean ) {
			print_level(level, "true");
		} else {
			print_level(level, "false");
		}
		break;

	case JSON_NONE:		print_level(level, "none"); break;

	case JSON_NUMBER:	print_level(level, "%ld", v->value.number); break;

	case JSON_OBJECT:
		print_level(level, "{\n");
		for( size_t i = 0; i < v->value.members.count; ++i ) {
			print_level(0, "\"%s\"", v->value.members.array[i].key);
			printf(" : ");
			dump(v->value.members.array[i].value, 0);


			if( i < v->value.members.count - 1) {
				printf(", ");
			}
		}

		print_level(level, "\n}");
		break;

	case JSON_STRING:	print_level(level, "\"%s\"", v->value.string); break;

	default: break;
	}
}

void
test_simple_object() {
	const char*		json_str	= "{\n\"key\":\"value\"\n}";
	json_value_t*	val			= json_parse(json_str);
	dump(val, 0);
	json_free(val);
}

void
test_simple_object2() {
	const char*		json_str	= "{\n\"key\":\"value\",\n\"key2\":\"value2\"\n}";
	json_value_t*	val			= json_parse(json_str);
	dump(val, 0);
	json_free(val);
}

char*
load_file(const char* filename) {
	FILE*	f = fopen(filename, "rb");
	if( f ) {
		fseek(f, 0, SEEK_END);
		size_t	size	= ftell(f);
		fseek(f, 0, SEEK_SET);
		char*	buffer	= (char*)malloc(size + 1);
		fread(buffer, 1, size, f);
		buffer[size]	= '\0';
		fclose(f);
		return buffer;
	} else {
		return NULL;
	}
}

void
test_twitter() {
	char*	twitter	= load_file("twitter.json");
	json_value_t*	val	= json_parse(twitter);
	dump(val, 0);
	json_free(val);
	free(twitter);
}

int
main(int argc, char* argv[]) {
	test_simple_object();
	test_simple_object2();
	test_twitter();
	return 0;
}
