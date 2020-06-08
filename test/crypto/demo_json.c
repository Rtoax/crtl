#include <crypto/json/json.h>
#include <crtl/log.h>
#include <crtl/assert.h>
#include <crypto/limits.h>
#include <time.h>
#include <locale.h>
#include <xlocale.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static int sort_fn(const void *j1, const void *j2)
{
	json_object * const *jso1, * const *jso2;
	int i1, i2;

	jso1 = (json_object* const*)j1;
	jso2 = (json_object* const*)j2;
	if (!*jso1 && !*jso2)
		return 0;
	if (!*jso1)
		return -1;
	if (!*jso2)
		return 1;

	i1 = json_object_get_int(*jso1);
	i2 = json_object_get_int(*jso2);

	return i1 - i2;
}

json_object *make_array(void);
json_object *make_array()
{
	json_object *my_array;

	my_array = json_object_new_array();
	json_object_array_add(my_array, json_object_new_int(1));
	json_object_array_add(my_array, json_object_new_int(2));
	json_object_array_add(my_array, json_object_new_int(3));
	json_object_array_put_idx(my_array, 4, json_object_new_int(5));
	json_object_array_put_idx(my_array, 3, json_object_new_int(4));
	json_object_array_put_idx(my_array, 6, json_object_new_int(7));

	return my_array;
}

void test_array_del_idx(void);
void test_array_del_idx()
{
	int rc;
	size_t ii;
	size_t orig_array_len;
	json_object *my_array;


	my_array = make_array();
	orig_array_len = json_object_array_length(my_array);

	printf("my_array=\n");
	for(ii = 0; ii < json_object_array_length(my_array); ii++)
	{
		json_object *obj = json_object_array_get_idx(my_array, ii);
		printf("\t[%d]=%s\n", (int)ii, json_object_to_json_string(obj));
	}
	printf("my_array.to_string()=%s\n", json_object_to_json_string(my_array));

	for (ii = 0; ii < orig_array_len; ii++)
	{
		rc = json_object_array_del_idx(my_array, 0, 1);
		printf("after del_idx(0,1)=%d, my_array.to_string()=%s\n",
		       rc, json_object_to_json_string(my_array));
	}

	/* One more time, with the empty array: */
	rc = json_object_array_del_idx(my_array, 0, 1);
	printf("after del_idx(0,1)=%d, my_array.to_string()=%s\n",
	       rc, json_object_to_json_string(my_array));

	json_object_put(my_array);

	/* Delete all array indexes at once */
	my_array = make_array();
	rc = json_object_array_del_idx(my_array, 0, orig_array_len);
	printf("after del_idx(0,%d)=%d, my_array.to_string()=%s\n",
	       (int)orig_array_len, rc, json_object_to_json_string(my_array));

	json_object_put(my_array);

	/* Delete *more* than all array indexes at once */
	my_array = make_array();
	rc = json_object_array_del_idx(my_array, 0, orig_array_len + 1);
	printf("after del_idx(0,%d)=%d, my_array.to_string()=%s\n",
	       (int)(orig_array_len + 1), rc, json_object_to_json_string(my_array));

	json_object_put(my_array);
	
	/* Delete some array indexes, then add more */
	my_array = make_array();
	rc = json_object_array_del_idx(my_array, 0, orig_array_len - 1);
	printf("after del_idx(0,%d)=%d, my_array.to_string()=%s\n",
	       (int)(orig_array_len - 1), rc, json_object_to_json_string(my_array));
	json_object_array_add(my_array, json_object_new_string("s1"));
	json_object_array_add(my_array, json_object_new_string("s2"));
	json_object_array_add(my_array, json_object_new_string("s3"));

	printf("after adding more entries, my_array.to_string()=%s\n",
	       json_object_to_json_string(my_array));
	json_object_put(my_array);
}

void demo_crypto_json_test1()
{
	json_object *my_string, *my_int, *my_null, *my_object, *my_array;
	size_t i;    

	my_string = json_object_new_string("\t");
	printf("my_string=%s\n", json_object_get_string(my_string));
	printf("my_string.to_string()=%s\n", json_object_to_json_string(my_string));
	json_object_put(my_string);

	my_string = json_object_new_string("\\");
	printf("my_string=%s\n", json_object_get_string(my_string));
	printf("my_string.to_string()=%s\n", json_object_to_json_string(my_string));
	json_object_put(my_string);

	my_string = json_object_new_string("/");
	printf("my_string=%s\n", json_object_get_string(my_string));
	printf("my_string.to_string()=%s\n", json_object_to_json_string(my_string));
	printf("my_string.to_string(NOSLASHESCAPE)=%s\n", json_object_to_json_string_ext(my_string, JSON_C_TO_STRING_NOSLASHESCAPE));
	json_object_put(my_string);

	my_string = json_object_new_string("/foo/bar/baz");
	printf("my_string=%s\n", json_object_get_string(my_string));
	printf("my_string.to_string()=%s\n", json_object_to_json_string(my_string));
	printf("my_string.to_string(NOSLASHESCAPE)=%s\n", json_object_to_json_string_ext(my_string, JSON_C_TO_STRING_NOSLASHESCAPE));
	json_object_put(my_string);

	my_string = json_object_new_string("foo");
	printf("my_string=%s\n", json_object_get_string(my_string));
	printf("my_string.to_string()=%s\n", json_object_to_json_string(my_string));

	my_int = json_object_new_int(9);
	printf("my_int=%d\n", json_object_get_int(my_int));
	printf("my_int.to_string()=%s\n", json_object_to_json_string(my_int));

	my_null = json_object_new_null();
	printf("my_null.to_string()=%s\n", json_object_to_json_string(my_null));

	my_array = json_object_new_array();
	json_object_array_add(my_array, json_object_new_int(1));
	json_object_array_add(my_array, json_object_new_int(2));
	json_object_array_add(my_array, json_object_new_int(3));
	json_object_array_put_idx(my_array, 4, json_object_new_int(5));
	printf("my_array=\n");
	for(i=0; i < json_object_array_length(my_array); i++)
	{
		json_object *obj = json_object_array_get_idx(my_array, i);
		printf("\t[%d]=%s\n", (int)i, json_object_to_json_string(obj));
	}
	printf("my_array.to_string()=%s\n", json_object_to_json_string(my_array));

	json_object_put(my_array);

	test_array_del_idx();

	my_array = json_object_new_array();
	json_object_array_add(my_array, json_object_new_int(3));
	json_object_array_add(my_array, json_object_new_int(1));
	json_object_array_add(my_array, json_object_new_int(2));
	json_object_array_put_idx(my_array, 4, json_object_new_int(0));
	printf("my_array=\n");
	for(i=0; i < json_object_array_length(my_array); i++)
	{
		json_object *obj = json_object_array_get_idx(my_array, i);
		printf("\t[%d]=%s\n", (int)i, json_object_to_json_string(obj));
	}
	printf("my_array.to_string()=%s\n", json_object_to_json_string(my_array));
	json_object_array_sort(my_array, sort_fn);
	printf("my_array=\n");
	for(i=0; i < json_object_array_length(my_array); i++)
	{
		json_object *obj = json_object_array_get_idx(my_array, i);
		printf("\t[%d]=%s\n", (int)i, json_object_to_json_string(obj));
	}
	printf("my_array.to_string()=%s\n", json_object_to_json_string(my_array));

	my_object = json_object_new_object();
	int rc = json_object_object_add(my_object, "abc", my_object);
	if (rc != -1)
	{
		printf("ERROR: able to successfully add object to itself!\n");
		fflush(stdout);
	}
	json_object_object_add(my_object, "abc", json_object_new_int(12));
	json_object_object_add(my_object, "foo", json_object_new_string("bar"));
	json_object_object_add(my_object, "bool0", json_object_new_boolean(0));
	json_object_object_add(my_object, "bool1", json_object_new_boolean(1));
	json_object_object_add(my_object, "baz", json_object_new_string("bang"));

	json_object *baz_obj = json_object_new_string("fark");
	json_object_get(baz_obj);
	json_object_object_add(my_object, "baz", baz_obj);
	json_object_object_del(my_object, "baz");

	/* baz_obj should still be valid */
	printf("baz_obj.to_string()=%s\n", json_object_to_json_string(baz_obj));
	json_object_put(baz_obj);

	/*json_object_object_add(my_object, "arr", my_array);*/
	printf("my_object=\n");
	json_object_object_foreach(my_object, __key, val)
	{
		printf("\t%s: %s\n", __key, json_object_to_json_string(val));
	}
	printf("my_object.to_string()=%s\n", json_object_to_json_string(my_object));

	json_object_put(my_string);
	json_object_put(my_int);
	json_object_put(my_null);
	json_object_put(my_object);
	json_object_put(my_array);

	return ;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void demo_crypto_json_test2()
{
    json_object *new_obj;

	new_obj = json_tokener_parse("/* more difficult test case */"
				     "{ \"glossary\": { \"title\": \"example glossary\", \"GlossDiv\": { \"title\": \"S\", \"GlossList\": [ { \"ID\": \"SGML\", \"SortAs\": \"SGML\", \"GlossTerm\": \"Standard Generalized Markup Language\", \"Acronym\": \"SGML\", \"Abbrev\": \"ISO 8879:1986\", \"GlossDef\": \"A meta-markup language, used to create markup languages such as DocBook.\", \"GlossSeeAlso\": [\"GML\", \"XML\", \"markup\"] } ] } } }");
	printf("new_obj.to_string()=%s\n", json_object_to_json_string(new_obj));
	json_object_put(new_obj);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void print_hex(const char* s)
{
	const char *iter = s;
	unsigned char ch;
	while ((ch = *iter++) != 0)
	{
		if( ',' != ch)
			printf("%x ", ch);
		else
			printf( ",");
	}
	putchar('\n');
}

int demo_crypto_json_test3(void)
{
	const char *input = "\"\\ud840\\udd26,\\ud840\\udd27,\\ud800\\udd26,\\ud800\\udd27\"";
	const char *expected = "\xF0\xA0\x84\xA6,\xF0\xA0\x84\xA7,\xF0\x90\x84\xA6,\xF0\x90\x84\xA7";
	struct json_object *parse_result = json_tokener_parse(input);
	const char *unjson = json_object_get_string(parse_result);

	printf("input: %s\n", input);

	int strings_match = !strcmp( expected, unjson);
	int retval = 0;
	if (strings_match)
	{
		printf("JSON parse result is correct: %s\n", unjson);
		puts("PASS");
	} else {
		printf("JSON parse result doesn't match expected string\n");
		printf("expected string bytes: ");
		print_hex(expected);
		printf("parsed string bytes:   ");
		print_hex(unjson);
		puts("FAIL");
		retval = 1;
	}
	json_object_put(parse_result);
	return retval;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



static void getit(struct json_object *new_obj, const char *field)
{
	struct json_object *o = NULL;
	if (!json_object_object_get_ex(new_obj, field, &o))
		printf("Field %s does not exist\n", field);

	enum json_type o_type = json_object_get_type(o);
	printf("new_obj.%s json_object_get_type()=%s\n", field,
	       json_type_to_name(o_type));
	printf("new_obj.%s json_object_get_int()=%d\n", field,
	       json_object_get_int(o));
	printf("new_obj.%s json_object_get_int64()=%" PRId64 "\n", field,
	       json_object_get_int64(o));
	printf("new_obj.%s json_object_get_uint64()=%" PRIu64 "\n", field,
	       json_object_get_uint64(o));
	printf("new_obj.%s json_object_get_boolean()=%d\n", field,
	       json_object_get_boolean(o));
	printf("new_obj.%s json_object_get_double()=%f\n", field,
	       json_object_get_double(o));
}

static void checktype_header()
{
	printf("json_object_is_type: %s,%s,%s,%s,%s,%s,%s\n",
		json_type_to_name(json_type_null),
		json_type_to_name(json_type_boolean),
		json_type_to_name(json_type_double),
		json_type_to_name(json_type_int),
		json_type_to_name(json_type_object),
		json_type_to_name(json_type_array),
		json_type_to_name(json_type_string));
}
static void checktype(struct json_object *new_obj, const char *field)
{
	struct json_object *o = new_obj;
	if (field && !json_object_object_get_ex(new_obj, field, &o))
		printf("Field %s does not exist\n", field);
			
	printf("new_obj%s%-18s: %d,%d,%d,%d,%d,%d,%d\n",
		field ? "." : " ", field ? field : "",
		json_object_is_type(o, json_type_null),
		json_object_is_type(o, json_type_boolean),
		json_object_is_type(o, json_type_double),
		json_object_is_type(o, json_type_int),
		json_object_is_type(o, json_type_object),
		json_object_is_type(o, json_type_array),
		json_object_is_type(o, json_type_string));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int demo_crypto_json_test4()
{
	const char *input = "{\n\
		\"string_of_digits\": \"123\",\n\
		\"regular_number\": 222,\n\
		\"decimal_number\": 99.55,\n\
		\"boolean_true\": true,\n\
		\"boolean_false\": false,\n\
		\"int64_number\": 2147483649,\n\
		\"negative_number\": -321321321,\n\
		\"a_null\": null,\n\
	}";
	/* Note: 2147483649 = INT_MAX + 2 */
	/* Note: 9223372036854775809 = INT64_MAX + 2 */
	/* Note: 18446744073709551617 = UINT64_MAX + 2 */

	struct json_object *new_obj;

	new_obj = json_tokener_parse(input);
	printf("Parsed input: %s\n", input);
	printf("Result is %s\n", (new_obj == NULL) ? "NULL (error!)" : "not NULL");
	if (!new_obj)
		return 1; // oops, we failed.

	getit(new_obj, "string_of_digits");
	getit(new_obj, "regular_number");
	getit(new_obj, "decimal_number");
	getit(new_obj, "boolean_true");
	getit(new_obj, "boolean_false");
	getit(new_obj, "int64_number");
	getit(new_obj, "negative_number");
	getit(new_obj, "a_null");

	// Now check the behaviour of the json_object_is_type() function.
	printf("\n================================\n");
	checktype_header();
	checktype(new_obj, NULL);
	checktype(new_obj, "string_of_digits");
	checktype(new_obj, "regular_number");
	checktype(new_obj, "decimal_number");
	checktype(new_obj, "boolean_true");
	checktype(new_obj, "boolean_false");
	checktype(new_obj, "int64_number");
	checktype(new_obj, "negative_number");
	checktype(new_obj, "a_null");

	json_object_put(new_obj);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int demo_crypto_json_test5()
{
	/*
	 * Check that replacing an existing object keeps the key valid,
	 * and that it keeps the order the same.
	 */
	json_object *my_object = json_object_new_object();
	json_object_object_add(my_object, "foo1", json_object_new_string("bar1"));
	json_object_object_add(my_object, "foo2", json_object_new_string("bar2"));
	json_object_object_add(my_object, "deleteme", json_object_new_string("bar2"));
	json_object_object_add(my_object, "foo3", json_object_new_string("bar3"));

	printf("==== delete-in-loop test starting ====\n");

	int orig_count = 0;
	json_object_object_foreach(my_object, key0, val0)
	{
		printf("Key at index %d is [%s] %d", orig_count, key0, (val0 == NULL));
		if (strcmp(key0, "deleteme") == 0)
		{
			json_object_object_del(my_object, key0);
			printf(" (deleted)\n");
		}
		else
			printf(" (kept)\n");
		orig_count++;
	}

	printf("==== replace-value first loop starting ====\n");

	const char *original_key = NULL;
	orig_count = 0;
	json_object_object_foreach(my_object, key, val)
	{
		printf("Key at index %d is [%s] %d\n", orig_count, key, (val == NULL));
		orig_count++;
		if (strcmp(key, "foo2") != 0)
			continue;
		printf("replacing value for key [%s]\n", key);
		original_key = key;
		json_object_object_add(my_object, key, json_object_new_string("zzz"));
	}

	printf("==== second loop starting ====\n");

	int new_count = 0;
	int retval = 0;
	json_object_object_foreach(my_object, key2, val2)
	{
		printf("Key at index %d is [%s] %d\n", new_count, key2, (val2 == NULL));
		new_count++;
		if (strcmp(key2, "foo2") != 0)
			continue;
		printf("pointer for key [%s] does %smatch\n", key2,
		       (key2 == original_key) ? "" : "NOT ");
		if (key2 != original_key)
			retval = 1;
	}
	if (new_count != orig_count)
	{
		printf("mismatch between original count (%d) and new count (%d)\n",
		       orig_count, new_count);
		retval = 1;
	}

	json_object_put( my_object );

	return retval;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* make sure only lowercase forms are parsed in strict mode */
void demo_crypto_json_test6()
{
	struct json_tokener *tok;
	json_object *new_obj;

	tok = json_tokener_new();
	json_tokener_set_flags(tok, JSON_TOKENER_STRICT);

	new_obj = json_tokener_parse_ex(tok, "True", 4);
	crtl_assert (new_obj == NULL);

	new_obj = json_tokener_parse_ex(tok, "False", 5);
	crtl_assert (new_obj == NULL);

	new_obj = json_tokener_parse_ex(tok, "Null", 4);
	crtl_assert (new_obj == NULL);

	printf("OK\n");

	json_tokener_free(tok);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int demo_crypto_json_test7()
{
	/* integer tests */
	struct json_object *int1 = json_object_new_int(0);
	struct json_object *int2 = json_object_new_int(1);
	struct json_object *int3 = json_object_new_int(1);
	struct json_object *int4 = json_object_new_int(-1);
	struct json_object *uint1 = json_object_new_uint64(0);
	struct json_object *uint2 = json_object_new_uint64(1);
	struct json_object *uint3 = json_object_new_uint64(1);
	struct json_object *uint4 = json_object_new_uint64((uint64_t)INT64_MAX + 100);

	if (!json_object_equal(int1, int2))
		printf("JSON integer comparison is correct\n");
	else
		printf("JSON integer comparison failed\n");

	if (json_object_equal(int1, int1))
		printf("JSON same object comparison is correct\n");
	else
		printf("JSON same object comparison failed\n");

	if (json_object_equal(int2, int3))
		printf("JSON same integer comparison is correct\n");
	else
		printf("JSON same integer comparison failed\n");

	if (!json_object_equal(uint1, uint2))
		printf("JSON usigned integer comparison is correct\n");
	else
		printf("JSON usigned integer comparison failed\n");

	if (json_object_equal(uint1, uint1))
		printf("JSON same usigned object comparison is correct\n");
	else
		printf("JSON same usigned object comparison failed\n");

	if (json_object_equal(uint2, uint3))
		printf("JSON same usigned integer comparison is correct\n");
	else
		printf("JSON same usigned integer comparison failed\n");

	if (json_object_equal(int2, uint2))
		printf("JSON integer & usigned integer comparison is correct\n");
	else
		printf("JSON integer & usigned integer comparison failed\n");

	if (!json_object_equal(int2, uint4))
		printf("JSON integer & usigned integer comparison is correct\n");
	else
		printf("JSON integer & usigned integer comparison failed\n");

	if (!json_object_equal(int4, uint2))
		printf("JSON integer & usigned integer comparison is correct\n");
	else
		printf("JSON integer & usigned integer comparison failed\n");

	if (!json_object_equal(int4, uint4))
		printf("JSON integer & usigned integer comparison is correct\n");
	else
		printf("JSON integer & usigned integer comparison failed\n");

	if (json_object_equal(uint2, int2))
		printf("JSON usigned integer & integer comparison is correct\n");
	else
		printf("JSON usigned integer & integer comparison failed\n");

	if (!json_object_equal(uint2, int4))
		printf("JSON usigned integer & integer comparison is correct\n");
	else
		printf("JSON usigned integer & integer comparison failed\n");

	if (!json_object_equal(uint4, int2))
		printf("JSON usigned integer & integer comparison is correct\n");
	else
		printf("JSON usigned integer & integer comparison failed\n");

	if (!json_object_equal(uint4, int4))
		printf("JSON usigned integer & integer comparison is correct\n");
	else
		printf("JSON usigned integer & integer comparison failed\n");

	json_object_put(int1);
	json_object_put(int2);
	json_object_put(int3);
	json_object_put(int4);
	json_object_put(uint1);
	json_object_put(uint2);
	json_object_put(uint3);
	json_object_put(uint4);

	/* string tests */
	struct json_object *str1 = json_object_new_string("TESTSTRING");
	struct json_object *str2 = json_object_new_string("TESTSTRING");
	struct json_object *str3 = json_object_new_string("DIFFERENT");

	if (json_object_equal(str1, str2))
		printf("Comparing equal strings is correct\n");
	else
		printf("Comparing equal strings failed\n");

	if (!json_object_equal(str1, str3))
		printf("Comparing different strings is correct\n");
	else
		printf("Comparing different strings failed\n");

	json_object_put(str1);
	json_object_put(str2);
	json_object_put(str3);

	/* double tests */
	struct json_object *dbl1 = json_object_new_double(3.14159);
	struct json_object *dbl2 = json_object_new_double(3.14159);
	struct json_object *dbl3 = json_object_new_double(3.0);

	if (json_object_equal(dbl1, dbl2))
		printf("Comparing equal doubles is correct\n");
	else
		printf("Comparing equal doubles failed\n");

	if (!json_object_equal(dbl1, dbl3))
		printf("Comparing different doubles is correct\n");
	else
		printf("Comparing different doubles failed\n");

	json_object_put(dbl1);
	json_object_put(dbl2);
	json_object_put(dbl3);

	/* array tests */
	struct json_object *ar1 = json_object_new_array();
	struct json_object *ar2 = json_object_new_array();
	struct json_object *ar3 = json_object_new_array();
	struct json_object *ar4 = json_object_new_array();

	json_object_array_add(ar1, json_object_new_int(1));
	json_object_array_add(ar1, json_object_new_int(2));

	json_object_array_add(ar2, json_object_new_int(1));
	json_object_array_add(ar2, json_object_new_int(2));

	json_object_array_add(ar3, json_object_new_int(1));
	json_object_array_add(ar3, json_object_new_int(1));

	if (json_object_equal(ar1, ar2))
		printf("Comparing equal arrays is correct\n");
	else
		printf("Comparing equal arrays failed\n");

	json_object_array_add(ar2, json_object_new_int(1));
	if (!json_object_equal(ar1, ar2))
		printf("Comparing arrays of different len is correct\n");
	else
		printf("Comparing arrays of different len failed\n");

	if (!json_object_equal(ar1, ar3))
		printf("Comparing different arrays is correct\n");
	else
		printf("Comparing different arrays failed\n");

	if (!json_object_equal(ar1, ar4))
		printf("Comparing different arrays (one empty) is correct\n");
	else
		printf("Comparing different arrays (one empty) failed\n");

	json_object_put(ar1);
	json_object_put(ar2);
	json_object_put(ar3);
	json_object_put(ar4);

	/* object tests */
	struct json_object *obj1 = json_object_new_object();
	struct json_object *obj2 = json_object_new_object();

	json_object_object_add(obj1, "test1", json_object_new_int(123));
	json_object_object_add(obj1, "test2", json_object_new_int(321));
	json_object_object_add(obj1, "test3", json_object_new_int(320));
	json_object_object_add(obj1, "test4", json_object_new_int(319));
	json_object_object_add(obj1, "test5", json_object_new_int(318));

	json_object_object_add(obj2, "test5", json_object_new_int(318));
	json_object_object_add(obj2, "test4", json_object_new_int(319));
	json_object_object_add(obj2, "test3", json_object_new_int(320));
	json_object_object_add(obj2, "test2", json_object_new_int(321));
	json_object_object_add(obj2, "test1", json_object_new_int(123));

	/* key-order is different between obj1 and obj2, should still be equal */
	if (json_object_equal(obj1, obj2))
		printf("Comparing JSON object with different key order is correct\n");
	else
		printf("Comparing JSON object with different key order is incorrect\n");

	/* make obj2 look different to obj1 */
	json_object_object_add(obj2, "test3", json_object_new_int(234));
	if (!json_object_equal(obj1, obj2))
		printf("Comparing different objects is correct\n");
	else
		printf("Comparing different objects is incorrect\n");

	json_object_put(obj1);
	json_object_put(obj2);

	return 0;
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static void do_benchmark(json_object *src2)
{
	json_object *dst2 = NULL;

	int ii;
	/**
	 * The numbers that I got are:
	 * BENCHMARK - 1000000 iterations of 'dst2 = json_tokener_parse(json_object_get_string(src2))' took 71 seconds
	 * BENCHMARK - 1000000 iterations of 'json_object_deep_copy(src2, &dst2, NULL)' took 29 seconds
	 */

	int iterations = 1000000;
	time_t start = time(NULL);

	start = time(NULL);
	for (ii = 0; ii < iterations; ii++) {
		dst2 = json_tokener_parse(json_object_get_string(src2));
		json_object_put(dst2);
	}
	printf("BENCHMARK - %d iterations of 'dst2 = json_tokener_parse(json_object_get_string(src2))' took %d seconds\n", iterations, (int)(time(NULL) - start));

	start = time(NULL);
	dst2 = NULL;
	for (ii = 0; ii < iterations; ii++) {
		json_object_deep_copy(src2, &dst2, NULL);
		json_object_put(dst2);
		dst2 = NULL;
	}
	printf("BENCHMARK - %d iterations of 'json_object_deep_copy(src2, &dst2, NULL)' took %d seconds\n", iterations, (int)(time(NULL) - start));
}


static const char *json_str1 = 
"{"
"    \"glossary\": {"
"        \"title\": \"example glossary\","
"        \"GlossDiv\": {"
"            \"title\": \"S\","
"            \"null_obj\": null, "
"            \"exixt\": false,"
"            \"quantity\":20,"
"            \"univalent\":19.8,"
"            \"GlossList\": {"
"                \"GlossEntry\": {"
"                    \"ID\": \"SGML\","
"                    \"SortAs\": \"SGML\","
"                    \"GlossTerm\": \"Standard Generalized Markup Language\","
"                    \"Acronym\": \"SGML\","
"                    \"Abbrev\": \"ISO 8879:1986\","
"                    \"GlossDef\": {"
"                        \"para\": \"A meta-markup language, used to create markup languages such as DocBook.\","
"                        \"GlossSeeAlso\": [\"GML\", \"XML\"]"
"                    },"
"                    \"GlossSee\": \"markup\""
"                }"
"            }"
"        }"
"    }"
"}";

static const char *json_str2 =
"{\"menu\": {"
"    \"header\": \"SVG Viewer\","
"    \"items\": ["
"        {\"id\": \"Open\"},"
"        {\"id\": \"OpenNew\", \"label\": \"Open New\"},"
"        null,"
"        {\"id\": \"ZoomIn\", \"label\": \"Zoom In\"},"
"        {\"id\": \"ZoomOut\", \"label\": \"Zoom Out\"},"
"        {\"id\": \"OriginalView\", \"label\": \"Original View\"},"
"        null,"
"        {\"id\": \"Quality\", \"another_null\": null},"
"        {\"id\": \"Pause\"},"
"        {\"id\": \"Mute\"},"
"        null,"
"        {\"id\": \"Find\", \"label\": \"Find...\"},"
"        {\"id\": \"FindAgain\", \"label\": \"Find Again\"},"
"        {\"id\": \"Copy\"},"
"        {\"id\": \"CopyAgain\", \"label\": \"Copy Again\"},"
"        {\"id\": \"CopySVG\", \"label\": \"Copy SVG\"},"
"        {\"id\": \"ViewSVG\", \"label\": \"View SVG\"},"
"        {\"id\": \"ViewSource\", \"label\": \"View Source\"},"
"        {\"id\": \"SaveAs\", \"label\": \"Save As\"},"
"        null,"
"        {\"id\": \"Help\"},"
"        {\"id\": \"About\", \"label\": \"About Adobe CVG Viewer...\"}"
"    ]"
"}}";

static const char *json_str3 =
"{\"menu\": {"
"  \"id\": \"file\","
"  \"value\": \"File\","
"  \"popup\": {"
"    \"menuitem\": ["
"      {\"value\": \"New\", \"onclick\": \"CreateNewDoc()\"},"
"      {\"value\": \"Open\", \"onclick\": \"OpenDoc()\"},"
"      {\"value\": \"Close\", \"onclick\": \"CloseDoc()\"}"
"    ]"
"  }"
"}}";

json_object_to_json_string_fn my_custom_serializer;
int my_custom_serializer(struct json_object *jso, struct printbuf *pb, int level, int flags)
{
	sprintbuf(pb, "OTHER");
	return 0;
}

json_c_shallow_copy_fn my_shallow_copy;
int my_shallow_copy(json_object *src, json_object *parent, const char *key, size_t index, json_object **dst)
{
	int rc;
	rc = json_c_shallow_copy_default(src, parent, key, index, dst);
	if (rc < 0)
		return rc;
	if (key != NULL && strcmp(key, "with_serializer") == 0)
	{
		printf("CALLED: my_shallow_copy on with_serializer object\n");
		void *userdata = json_object_get_userdata(src);
		json_object_set_serializer(*dst, my_custom_serializer, userdata, NULL);
		return 2;
	}
	return rc;
}


int demo_crypto_json_test8(int __benchmark)
{
	struct json_object *src1, *src2, *src3;
	struct json_object *dst1 = NULL, *dst2 = NULL, *dst3 = NULL;
	int benchmark = 0;

	if (__benchmark)
	{
		benchmark = 1;
	}

	src1 = json_tokener_parse(json_str1);
	src2 = json_tokener_parse(json_str2);
	src3 = json_tokener_parse(json_str3);

	crtl_assert(src1 != NULL);
	crtl_assert(src1 != NULL);
	crtl_assert(src3 != NULL);

	printf("PASSED - loaded input data\n");

	/* do this 3 times to make sure overwriting it works */
	crtl_assert(0 == json_object_deep_copy(src1, &dst1, NULL));
	crtl_assert(0 == json_object_deep_copy(src2, &dst2, NULL));
	crtl_assert(0 == json_object_deep_copy(src3, &dst3, NULL));

	printf("PASSED - all json_object_deep_copy() returned succesful\n");

	crtl_assert(-1 == json_object_deep_copy(src1, &dst1, NULL));
	crtl_assert(errno == EINVAL);
	crtl_assert(-1 == json_object_deep_copy(src2, &dst2, NULL));
	crtl_assert(errno == EINVAL);
	crtl_assert(-1 == json_object_deep_copy(src3, &dst3, NULL));
	crtl_assert(errno == EINVAL);

	printf("PASSED - all json_object_deep_copy() returned EINVAL for non-null pointer\n");

	crtl_assert(1 == json_object_equal(src1, dst1));
	crtl_assert(1 == json_object_equal(src2, dst2));
	crtl_assert(1 == json_object_equal(src3, dst3));

	printf("PASSED - all json_object_equal() tests returned succesful\n");

	crtl_assert(0 == strcmp(json_object_to_json_string_ext(src1, JSON_C_TO_STRING_PRETTY),
	                   json_object_to_json_string_ext(dst1, JSON_C_TO_STRING_PRETTY)));
	crtl_assert(0 == strcmp(json_object_to_json_string_ext(src2, JSON_C_TO_STRING_PRETTY),
	                   json_object_to_json_string_ext(dst2, JSON_C_TO_STRING_PRETTY)));
	crtl_assert(0 == strcmp(json_object_to_json_string_ext(src3, JSON_C_TO_STRING_PRETTY),
	                   json_object_to_json_string_ext(dst3, JSON_C_TO_STRING_PRETTY)));

	printf("PASSED - comparison of string output\n");

	json_object_get(dst1);
	crtl_assert(-1 == json_object_deep_copy(src1, &dst1, NULL));
	crtl_assert(errno == EINVAL);
	json_object_put(dst1);

	printf("PASSED - trying to overrwrite an object that has refcount > 1");

	printf("\nPrinting JSON objects for visual inspection\n");
	printf("------------------------------------------------\n");
	printf(" JSON1\n");
	printf("%s\n", json_object_to_json_string_ext(dst1, JSON_C_TO_STRING_PRETTY));
	printf("------------------------------------------------\n");

	printf("------------------------------------------------\n");
	printf(" JSON2\n");
	printf("%s\n", json_object_to_json_string_ext(dst2, JSON_C_TO_STRING_PRETTY));
	printf("------------------------------------------------\n");

	printf("------------------------------------------------\n");
	printf(" JSON3\n");
	printf("------------------------------------------------\n");
	printf("%s\n", json_object_to_json_string_ext(dst3, JSON_C_TO_STRING_PRETTY));
	printf("------------------------------------------------\n");

	json_object_put(dst1);
	json_object_put(dst2);
	json_object_put(dst3);

	printf("\nTesting deep_copy with a custom serializer set\n");
	json_object *with_serializer = json_object_new_string("notemitted");

	char udata[] = "dummy userdata";
	json_object_set_serializer(with_serializer, my_custom_serializer, udata, NULL);
	json_object_object_add(src1, "with_serializer", with_serializer);
	dst1 = NULL;
	/* With a custom serializer in use, a custom shallow_copy function must also be used */
	crtl_assert(-1 == json_object_deep_copy(src1, &dst1, NULL));
	crtl_assert(0 == json_object_deep_copy(src1, &dst1, my_shallow_copy));

	json_object *dest_with_serializer = json_object_object_get(dst1, "with_serializer");
	crtl_assert(dest_with_serializer != NULL);
	char *dst_userdata = json_object_get_userdata(dest_with_serializer);
	crtl_assert(strcmp(dst_userdata, "dummy userdata") == 0);

	const char *special_output = json_object_to_json_string(dest_with_serializer);
	crtl_assert(strcmp(special_output, "OTHER") == 0);
	printf("\ndeep_copy with custom serializer worked OK.\n");
	json_object_put(dst1);

	if (benchmark)
	{
		do_benchmark(src2);
	}

	json_object_put(src1);
	json_object_put(src2);
	json_object_put(src3);

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Avoid compiler warnings about diving by constant zero */
double zero_dot_zero = 0.0;

int demo_crypto_json_test9()
{
	struct json_object *obj = json_object_new_double(0.5);
	char udata[] = "test";

	printf("Test default serializer:\n");
	printf("obj.to_string(standard)=%s\n", json_object_to_json_string(obj));

	printf("Test default serializer with custom userdata:\n");
//	obj->_userdata = udata;
    json_object_set_userdata(obj, udata, NULL);
	printf("obj.to_string(userdata)=%s\n", json_object_to_json_string(obj));

	printf("Test explicit serializer with custom userdata:\n");
	json_object_set_serializer(obj, json_object_double_to_json_string, udata, NULL);
	printf("obj.to_string(custom)=%s\n", json_object_to_json_string(obj));

	printf("Test reset serializer:\n");
	json_object_set_serializer(obj, NULL, NULL, NULL);
	printf("obj.to_string(reset)=%s\n", json_object_to_json_string(obj));

	json_object_put(obj);
	printf("Test no zero reset serializer:\n");
	obj = json_object_new_double(3.1415000);
	char data[] = "%.17g";
	json_object_set_serializer(obj, json_object_double_to_json_string, data, NULL);
	printf("obj.to_string(reset)=%s\n", json_object_to_json_string_ext(obj, 4));

	json_object_put(obj);
	obj = json_object_new_double(0.52381);

	printf("obj.to_string(default format)=%s\n", json_object_to_json_string(obj));
	if (json_c_set_serialization_double_format("x%0.3fy", JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(with global format)=%s\n", json_object_to_json_string(obj));
//#ifdef HAVE___THREAD
#if 1
	if (json_c_set_serialization_double_format("T%0.2fX", JSON_C_OPTION_THREAD) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(with thread format)=%s\n", json_object_to_json_string(obj));
	if (json_c_set_serialization_double_format("Ttttttttttttt%0.2fxxxxxxxxxxxxxxxxxxX", JSON_C_OPTION_THREAD) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(long thread format)=%s\n", json_object_to_json_string(obj));
	if (json_c_set_serialization_double_format(NULL, JSON_C_OPTION_THREAD) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(back to global format)=%s\n", json_object_to_json_string(obj));
#else
	// Just fake it up, so the output matches.
	printf("obj.to_string(with thread format)=%s\n", "T0.52X");
	printf("obj.to_string(back to global format)=%s\n", "x0.524y");
#endif
	if (json_c_set_serialization_double_format(NULL, JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj.to_string(back to default format)=%s\n", json_object_to_json_string(obj));

	json_object_put(obj);

	obj = json_object_new_double(12.0);
	printf("obj(12.0).to_string(default format)=%s\n", json_object_to_json_string(obj));
	if (json_c_set_serialization_double_format("%.0f", JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj(12.0).to_string(%%.0f)=%s\n", json_object_to_json_string(obj));

	if (json_c_set_serialization_double_format("%.0g", JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj(12.0).to_string(%%.0g)=%s\n", json_object_to_json_string(obj));

	if (json_c_set_serialization_double_format("%.2g", JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");
	printf("obj(12.0).to_string(%%.1g)=%s\n", json_object_to_json_string(obj));

	// Reset to default to free memory
	if (json_c_set_serialization_double_format(NULL, JSON_C_OPTION_GLOBAL) < 0)
		printf("ERROR: json_c_set_serialization_double_format() failed");

	json_object_put(obj);

	obj = json_object_new_double(-12.0);
	printf("obj(-12.0).to_string(default format)=%s\n", json_object_to_json_string(obj));
	json_object_put(obj);

	/* Test NaN handling */
	obj = json_object_new_double(zero_dot_zero / zero_dot_zero);
	printf("obj(0.0/0.0)=%s\n", json_object_to_json_string(obj));
	json_object_put(obj);

	/* Test Infinity and -Infinity handling */
	obj = json_object_new_double(1.0/zero_dot_zero);
	printf("obj(1.0/0.0)=%s\n", json_object_to_json_string(obj));
	json_object_put(obj);

	obj = json_object_new_double(-1.0/zero_dot_zero);
	printf("obj(-1.0/0.0)=%s\n", json_object_to_json_string(obj));
	json_object_put(obj);

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int demo_crypto_json_test10(void)
{
  json_object *json;

  json = json_object_new_double(1.0);
  printf("json = %s\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY));
  json_object_put(json);

  json = json_object_new_double(-1.0);
  printf("json = %s\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY));
  json_object_put(json);
  json = json_object_new_double(1.23);
  printf("json = %s\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY));
  json_object_put(json);
  json = json_object_new_double(123456789.0);
  printf("json = %s\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY));
  json_object_put(json);
  json = json_object_new_double(123456789.123);
  printf("json = %s\n", json_object_to_json_string_ext(json, JSON_C_TO_STRING_PRETTY));
  json_object_put(json);
  return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void demo_crypto_json_test11()
{
	json_object *tmp = json_object_new_int(123);
	json_object_int_inc(tmp, 123);
	crtl_assert(json_object_get_int(tmp) == 246);
	json_object_put(tmp);
	printf("INT ADD PASSED\n");
	tmp = json_object_new_int(INT32_MAX);
	json_object_int_inc(tmp, 100);
	crtl_assert(json_object_get_int(tmp) == INT32_MAX);
	crtl_assert(json_object_get_int64(tmp) == (int64_t)INT32_MAX + 100L);
	json_object_put(tmp);
	printf("INT ADD OVERFLOW PASSED\n");
	tmp = json_object_new_int(INT32_MIN);
	json_object_int_inc(tmp, -100);
	crtl_assert(json_object_get_int(tmp) == INT32_MIN);
	crtl_assert(json_object_get_int64(tmp) == (int64_t)INT32_MIN - 100L);
	json_object_put(tmp);
	printf("INT ADD UNDERFLOW PASSED\n");
	tmp = json_object_new_int64(321321321);
	json_object_int_inc(tmp, 321321321);
	crtl_assert(json_object_get_int(tmp) == 642642642);
	json_object_put(tmp);
	printf("INT64 ADD PASSED\n");
	tmp = json_object_new_int64(INT64_MAX);
	json_object_int_inc(tmp, 100);
	crtl_assert(json_object_get_int64(tmp) == INT64_MAX);
	crtl_assert(json_object_get_uint64(tmp) == (uint64_t)INT64_MAX + 100U);
	json_object_int_inc(tmp, -100);
	crtl_assert(json_object_get_int64(tmp) == INT64_MAX);
	crtl_assert(json_object_get_uint64(tmp) == (uint64_t)INT64_MAX);
	json_object_put(tmp);
	printf("INT64 ADD OVERFLOW PASSED\n");
	tmp = json_object_new_int64(INT64_MIN);
	json_object_int_inc(tmp, -100);
	crtl_assert(json_object_get_int64(tmp) == INT64_MIN);
	json_object_int_inc(tmp, 100);
	crtl_assert(json_object_get_int64(tmp) != INT64_MIN);
	json_object_put(tmp);
	printf("INT64 ADD UNDERFLOW PASSED\n");
  // uint64 + negative int64--> negative int64
	tmp = json_object_new_uint64(400);
	json_object_int_inc(tmp, -200);
	crtl_assert(json_object_get_int64(tmp) == 200);
	crtl_assert(json_object_get_uint64(tmp) == 200);
	json_object_put(tmp);
	printf("UINT64 ADD PASSED\n");
	tmp = json_object_new_uint64(100);
	json_object_int_inc(tmp, -200);
	crtl_assert(json_object_get_int64(tmp) == -100);
	crtl_assert(json_object_get_uint64(tmp) == 0);
	json_object_put(tmp);
	printf("UINT64 ADD UNDERFLOW PASSED\n");

	printf("PASSED\n");
	return ;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void test_example_int(struct json_object *jo1, const char *json_pointer, int expected_int)
{
	struct json_object *jo2 = NULL;
	crtl_assert(0 == json_pointer_get(jo1, json_pointer, NULL));
	crtl_assert(0 == json_pointer_get(jo1, json_pointer, &jo2));
	crtl_assert(json_object_is_type(jo2, json_type_int));
	crtl_assert(expected_int == json_object_get_int(jo2));
	printf("PASSED - GET -  %s == %d\n", json_pointer, expected_int);
}

static const char *input_json_str = "{ "
	"'foo': ['bar', 'baz'], "
	"'': 0, "
	"'a/b': 1, "
	"'c%d': 2, "
	"'e^f': 3, "
	"'g|h': 4, "
	"'i\\\\j': 5, "
	"'k\\\"l': 6, "
	"' ': 7, "
	"'m~n': 8 "
"}";


static const char *rec_input_json_str = "{"
	"'arr' : ["
		"{"
			"'obj': ["
				"{},{},"
				"{"
					"'obj1': 0,"
					"'obj2': \"1\""
				"}"
			"]"
		"}"
	"],"
	"'obj' : {"
		"'obj': {"
			"'obj': ["
				"{"
					"'obj1': 0,"
					"'obj2': \"1\""
				"}"
			"]"
		"}"
	"}"
"}";

/* Example from RFC */
static void test_example_get()
{
	int i;
	struct json_object *jo1, *jo2, *jo3;
	struct json_pointer_map_s_i {
		const char *s;
		int i;
	};
	/* Create a map to iterate over for the ints */
	struct json_pointer_map_s_i json_pointers[] = {
		{ "/", 0 },
		{ "/a~1b", 1 },
		{"/c%d", 2 },
		{"/e^f", 3 },
		{ "/g|h", 4 },
		{ "/i\\j", 5 },
		{ "/k\"l", 6 },
		{ "/ ", 7 },
		{ "/m~0n", 8 },
		{ NULL, 0}
	};

	jo1 = json_tokener_parse(input_json_str);
	crtl_assert(NULL != jo1);
	printf("PASSED - GET - LOADED TEST JSON\n");
	printf("%s\n", json_object_get_string(jo1));

	/* Test empty string returns entire object */
	jo2 = NULL;
	/* For each test, we're trying to see that NULL **value works (does no segfault) */
	crtl_assert(0 == json_pointer_get(jo1, "", NULL));
	crtl_assert(0 == json_pointer_get(jo1, "", &jo2));
	crtl_assert(json_object_equal(jo2, jo1));
	printf("PASSED - GET - ENTIRE OBJECT WORKED\n");

	/* Test /foo == ['bar', 'baz']  */
	jo3 = json_object_new_array();
	json_object_array_add(jo3, json_object_new_string("bar"));
	json_object_array_add(jo3, json_object_new_string("baz"));

	jo2 = NULL;
	crtl_assert(0 == json_pointer_get(jo1, "/foo", NULL));
	crtl_assert(0 == json_pointer_get(jo1, "/foo", &jo2));
	crtl_assert(NULL != jo2);
	crtl_assert(json_object_equal(jo2, jo3));
	json_object_put(jo3);
	printf("PASSED - GET - /foo == ['bar', 'baz']\n");

	/* Test /foo/0 == 'bar' */
	jo2 = NULL;
	crtl_assert(0 == json_pointer_get(jo1, "/foo/0", NULL));
	crtl_assert(0 == json_pointer_get(jo1, "/foo/0", &jo2));
	crtl_assert(NULL != jo2);
	crtl_assert(0 == strcmp("bar", json_object_get_string(jo2)));
	printf("PASSED - GET - /foo/0 == 'bar'\n");

	for (i = 0 ; json_pointers[i].s; i++)
		test_example_int(jo1, json_pointers[i].s, json_pointers[i].i);

	json_object_put(jo1);
}

/* I'm not too happy with the RFC example to test the recusion of the json_pointer_get() function */
static void test_recursion_get()
{
	struct json_object *jo2, *jo1 = json_tokener_parse(rec_input_json_str);

	jo2 = NULL;
	crtl_assert(jo1 != NULL);
	printf("%s\n", json_object_get_string(jo1));
	crtl_assert(0 == json_pointer_get(jo1, "/arr/0/obj/2/obj1", &jo2));
	crtl_assert(json_object_is_type(jo2, json_type_int));
	crtl_assert(0 == json_object_get_int(jo2));

	crtl_assert(0 == json_pointer_get(jo1, "/arr/0/obj/2/obj2", &jo2));
	crtl_assert(json_object_is_type(jo2, json_type_string));
	crtl_assert(0 == strcmp("1", json_object_get_string(jo2)));

	crtl_assert(0 == json_pointer_getf(jo1, &jo2, "/%s/%d/%s/%d/%s", "arr", 0, "obj", 2, "obj2"));
	crtl_assert(json_object_is_type(jo2, json_type_string));
	crtl_assert(0 == strcmp("1", json_object_get_string(jo2)));

	crtl_assert(jo1 != NULL);
	crtl_assert(0 == json_pointer_get(jo1, "/obj/obj/obj/0/obj1", &jo2));
	crtl_assert(json_object_is_type(jo2, json_type_int));
	crtl_assert(0 == json_object_get_int(jo2));

	crtl_assert(0 == json_pointer_get(jo1, "/obj/obj/obj/0/obj2", &jo2));
	crtl_assert(json_object_is_type(jo2, json_type_string));
	crtl_assert(0 == strcmp("1", json_object_get_string(jo2)));

	crtl_assert(0 == json_pointer_getf(jo1, &jo2, "%s", "\0"));

	printf("PASSED - GET - RECURSION TEST\n");

	json_object_put(jo1);
}

static void test_wrong_inputs_get()
{
	struct json_object *jo2, *jo1 = json_tokener_parse(input_json_str);

	crtl_assert(NULL != jo1);
	printf("PASSED - GET - LOADED TEST JSON\n");
	printf("%s\n", json_object_get_string(jo1));

	/* Test leading '/' missing */
	jo2 = NULL;
	errno = 0;
	crtl_assert(0 != json_pointer_get(jo1, "foo/bar", NULL));
	crtl_assert(0 != json_pointer_get(jo1, "foo/bar", &jo2));
	crtl_assert(errno == EINVAL);
	crtl_assert(jo2 == NULL);
	printf("PASSED - GET - MISSING /\n");

	/* Test combinations of NULL params for input json & path */
	errno = 0;
	crtl_assert(0 != json_pointer_get(NULL, "foo/bar", NULL));
	crtl_assert(errno == EINVAL);
	errno = 0;
	crtl_assert(0 != json_pointer_get(NULL, NULL, NULL));
	crtl_assert(errno == EINVAL);
	errno = 0;
	crtl_assert(0 != json_pointer_getf(NULL, NULL, NULL));
	crtl_assert(errno == EINVAL);
	errno = 0;
	crtl_assert(0 != json_pointer_get(jo1, NULL, NULL));
	crtl_assert(errno == EINVAL);
	errno = 0;
	crtl_assert(0 != json_pointer_getf(jo1, NULL, NULL));
	crtl_assert(errno == EINVAL);
	printf("PASSED - GET - NULL INPUTS\n");

	/* Test invalid indexes for array */
	errno = 0;
	crtl_assert(0 != json_pointer_get(jo1, "/foo/a", NULL));
	crtl_assert(errno == EINVAL);
	errno = 0;
	crtl_assert(0 != json_pointer_get(jo1, "/foo/01", NULL));
	crtl_assert(errno == EINVAL);
	errno = 0;
	crtl_assert(0 != json_pointer_getf(jo1, NULL, "/%s/a", "foo"));
	crtl_assert(errno == EINVAL);
	errno = 0;
	crtl_assert(0 != json_pointer_get(jo1, "/foo/-", NULL));
	crtl_assert(errno == EINVAL);
	errno = 0;
	/* Test optimized array path */
	crtl_assert(0 != json_pointer_get(jo1, "/foo/4", NULL));
	crtl_assert(errno == ENOENT);
	errno = 0;
	/* Test non-optimized array path */
	crtl_assert(0 != json_pointer_getf(jo1, NULL, "%s", "/foo/22"));
	crtl_assert(errno == ENOENT);
	errno = 0;
	crtl_assert(0 != json_pointer_getf(jo1, NULL, "/%s/%d", "foo", 22));
	crtl_assert(errno == ENOENT);
	errno = 0;
	crtl_assert(0 != json_pointer_get(jo1, "/foo/-1", NULL));
	crtl_assert(errno == EINVAL);
	errno = 0;
	crtl_assert(0 != json_pointer_get(jo1, "/foo/10", NULL));
	crtl_assert(errno == ENOENT);
	printf("PASSED - GET - INVALID INDEXES\n");

	json_object_put(jo1);
}

static void test_example_set()
{
	struct json_object *jo2, *jo1 = json_tokener_parse(input_json_str);

	crtl_assert(jo1 != NULL);
	printf("PASSED - SET - LOADED TEST JSON\n");
	printf("%s\n", json_object_get_string(jo1));

	crtl_assert(0 == json_pointer_set(&jo1, "/foo/1", json_object_new_string("cod")));
	crtl_assert(0 == strcmp("cod", json_object_get_string(json_object_array_get_idx(json_object_object_get(jo1, "foo"), 1))));
	printf("PASSED - SET - 'cod' in /foo/1\n");
	crtl_assert(0 != json_pointer_set(&jo1, "/fud/gaw", (jo2 = json_tokener_parse("[1,2,3]"))));
	crtl_assert(errno == ENOENT);
	printf("PASSED - SET - non-existing /fud/gaw\n");
	crtl_assert(0 == json_pointer_set(&jo1, "/fud", json_object_new_object()));
	printf("PASSED - SET - /fud == {}\n");
	crtl_assert(0 == json_pointer_set(&jo1, "/fud/gaw", jo2)); /* re-using jo2 from above */
	printf("PASSED - SET - /fug/gaw == [1,2,3]\n");
	crtl_assert(0 == json_pointer_set(&jo1, "/fud/gaw/0", json_object_new_int(0)));
	crtl_assert(0 == json_pointer_setf(&jo1, json_object_new_int(0), "%s%s/%d", "/fud", "/gaw", 0));
	printf("PASSED - SET - /fug/gaw == [0,2,3]\n");
	crtl_assert(0 == json_pointer_set(&jo1, "/fud/gaw/-", json_object_new_int(4)));
	printf("PASSED - SET - /fug/gaw == [0,2,3,4]\n");
	crtl_assert(0 == json_pointer_set(&jo1, "/", json_object_new_int(9)));
	printf("PASSED - SET - / == 9\n");

	jo2 = json_tokener_parse("{ 'foo': [ 'bar', 'cod' ], '': 9, 'a/b': 1, 'c%d': 2, 'e^f': 3, 'g|h': 4, 'i\\\\j': 5, 'k\\\"l': 6, ' ': 7, 'm~n': 8, 'fud': { 'gaw': [ 0, 2, 3, 4 ] } }");
	crtl_assert(json_object_equal(jo2, jo1));
	printf("PASSED - SET - Final JSON is: %s\n", json_object_get_string(jo1));
	json_object_put(jo2);

	crtl_assert(0 == json_pointer_set(&jo1, "", json_object_new_int(10)));
	crtl_assert(10 == json_object_get_int(jo1));
	printf("%s\n", json_object_get_string(jo1));

	json_object_put(jo1);
}

static void test_wrong_inputs_set()
{
	struct json_object *jo2, *jo1 = json_tokener_parse(input_json_str);

	crtl_assert(jo1 != NULL);
	printf("PASSED - SET - LOADED TEST JSON\n");
	printf("%s\n", json_object_get_string(jo1));

	crtl_assert(0 != json_pointer_set(NULL, NULL, NULL));
	crtl_assert(0 != json_pointer_setf(NULL, NULL, NULL));
	crtl_assert(0 != json_pointer_set(&jo1, NULL, NULL));
	crtl_assert(0 != json_pointer_setf(&jo1, NULL, NULL));
	printf("PASSED - SET - failed with NULL params for input json & path\n");

	crtl_assert(0 != json_pointer_set(&jo1, "foo/bar", (jo2 = json_object_new_string("cod"))));
	printf("PASSED - SET - failed 'cod' with path 'foo/bar'\n");
	json_object_put(jo2);

	crtl_assert(0 != json_pointer_setf(&jo1, (jo2 = json_object_new_string("cod")), "%s", "foo/bar"));
	printf("PASSED - SET - failed 'cod' with path 'foo/bar'\n");
	json_object_put(jo2);

	crtl_assert(0 != json_pointer_set(&jo1, "0", (jo2 = json_object_new_string("cod"))));
	printf("PASSED - SET - failed with invalid array index'\n");
	json_object_put(jo2);

	jo2 = json_object_new_string("whatever");
	crtl_assert(0 != json_pointer_set(&jo1, "/fud/gaw", jo2));
	crtl_assert(0 == json_pointer_set(&jo1, "/fud", json_object_new_object()));
	crtl_assert(0 == json_pointer_set(&jo1, "/fud/gaw", jo2)); /* re-using jo2 from above */
	// ownership of jo2 transferred into jo1

	jo2 = json_object_new_int(0);
	crtl_assert(0 != json_pointer_set(&jo1, "/fud/gaw/0", jo2));
	json_object_put(jo2);
	jo2 = json_object_new_int(0);
	crtl_assert(0 != json_pointer_set(&jo1, "/fud/gaw/", jo2));
	json_object_put(jo2);
	printf("PASSED - SET - failed to set index to non-array\n");

	crtl_assert(0 == json_pointer_setf(&jo1, json_object_new_string("cod"), "%s", "\0"));

	json_object_put(jo1);
}

void demo_crypto_json_test12()
{
//	_json_c_strerror_enable = 1;

	test_example_get();
	test_recursion_get();
	test_wrong_inputs_get();
	test_example_set();
	test_wrong_inputs_set();
	return ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int demo_crypto_json_test13()
{
	json_object *new_obj;
	setlocale(LC_NUMERIC, "de_DE");


	char buf1[10], buf2[10];
	// Should result in "0,1", if the locale is installed.
	// Regardless of what it generates, we check that it's
	// consistent below.
	(void)snprintf(buf1, sizeof(buf1), "%f", 0.1);
    
	new_obj = json_tokener_parse("[1.2,3.4,123456.78,5.0,2.3e10]");

	(void)snprintf(buf2, sizeof(buf2), "%f", 0.1);
	if (strcmp(buf1, buf2) != 0)
		printf("ERROR: Original locale not restored \"%s\" != \"%s\"",
		       buf1, buf2);
    
	setlocale(LC_NUMERIC, "C");

	// Explicitly print each value, to avoid having the "special"
	// serialization done for parsed doubles simply re-emit the original
	// string that was parsed.  (see json_object_new_double_s())
	printf("new_obj.to_string()=[");
	unsigned int ii;
	for (ii = 0 ; ii < json_object_array_length(new_obj); ii++)
	{
		json_object *val = json_object_array_get_idx(new_obj, ii);
		printf("%s%.2lf", (ii > 0) ? "," : "", json_object_get_double(val));
	}
	printf("]\n");

	printf("new_obj.to_string()=%s\n", json_object_to_json_string_ext(new_obj,JSON_C_TO_STRING_NOZERO));
	json_object_put(new_obj);

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static json_c_visit_userfunc clear_serializer;
static void do_clear_serializer(json_object *jso);

static void single_basic_parse(const char *test_string, int clear_serializer)
{
	json_object *new_obj;

	new_obj = json_tokener_parse(test_string);
	if (clear_serializer)
		do_clear_serializer(new_obj);
	printf("new_obj.to_string(%s)=%s\n", test_string, json_object_to_json_string(new_obj));
	json_object_put(new_obj);
}


int demo_crypto_json_test14_null(void)
{
	/* this test has a space after the null character. check that it's still included */
	const char *input = " \0 ";
	const char *expected = "\" \\u0000 \"";
	struct json_object *string = json_object_new_string_len(input, 3);
	const char *json = json_object_to_json_string(string);

	int strings_match =  !strcmp( expected, json);
	int retval = 0;
	if (strings_match)
	{
		printf("JSON write result is correct: %s\n", json);
		puts("PASS");
	} else {
		puts("JSON write result doesn't match expected string");
		printf("expected string: ");
		puts(expected);
		printf("parsed string:   ");
		puts(json);
		puts("FAIL");
		retval=1;
	}
	json_object_put(string);

	struct json_object *parsed_str = json_tokener_parse(expected);
	if (parsed_str)
	{
		int parsed_len = json_object_get_string_len(parsed_str);
		const char *parsed_cstr = json_object_get_string(parsed_str);
		int ii;
		printf("Re-parsed object string len=%d, chars=[", parsed_len);
		for (ii = 0; ii < parsed_len ; ii++)
		{
			printf("%s%d", (ii ? ", " : ""), (int)parsed_cstr[ii]);
		}
		puts("]");
		json_object_put(parsed_str);
	}
	else
	{
		puts("ERROR: failed to parse");
	}
	return retval;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static void test_basic_parse()
{
	single_basic_parse("\"\003\"", 0);
	single_basic_parse("/* hello */\"foo\"", 0);
	single_basic_parse("// hello\n\"foo\"", 0);
	single_basic_parse("\"foo\"blue", 0);
	single_basic_parse("\'foo\'", 0);
	single_basic_parse("\"\\u0041\\u0042\\u0043\"", 0);
	single_basic_parse("\"\\u4e16\\u754c\\u00df\"", 0);
	single_basic_parse("\"\\u4E16\"", 0);
	single_basic_parse("\"\\u4e1\"", 0);
	single_basic_parse("\"\\u4e1@\"", 0);
	single_basic_parse("\"\\ud840\\u4e16\"", 0);
	single_basic_parse("\"\\ud840\"", 0);
	single_basic_parse("\"\\udd27\"", 0);
	// Test with a "short" high surrogate
	single_basic_parse("[9,'\\uDAD", 0);
	single_basic_parse("null", 0);
	single_basic_parse("NaN", 0);
	single_basic_parse("-NaN", 0); /* non-sensical, returns null */

	single_basic_parse("Inf", 0); /* must use full string, returns null */
	single_basic_parse("inf", 0); /* must use full string, returns null */
	single_basic_parse("Infinity", 0);
	single_basic_parse("infinity", 0);
	single_basic_parse("-Infinity", 0);
	single_basic_parse("-infinity", 0);
	single_basic_parse("{ \"min\": Infinity, \"max\": -Infinity}", 0);

	single_basic_parse("Infinity!", 0);
	single_basic_parse("Infinitynull", 0);
	single_basic_parse("InfinityXXXX", 0);
	single_basic_parse("-Infinitynull", 0);
	single_basic_parse("-InfinityXXXX", 0);
	single_basic_parse("Infinoodle", 0);
	single_basic_parse("InfinAAA", 0);
	single_basic_parse("-Infinoodle", 0);
	single_basic_parse("-InfinAAA", 0);

	single_basic_parse("True", 0);
	single_basic_parse("False", 0);

	/* not case sensitive */
	single_basic_parse("tRue", 0);
	single_basic_parse("fAlse", 0);
	single_basic_parse("nAn", 0);
	single_basic_parse("iNfinity", 0);

	single_basic_parse("12", 0);
	single_basic_parse("12.3", 0);
	single_basic_parse("12.3.4", 0); /* non-sensical, returns null */
	/* was returning (int)2015 before patch, should return null */
	single_basic_parse("2015-01-15", 0);

	/* ...but this works.  It's rather inconsistent, and a future major release
	 * should change the behavior so it either always returns null when extra
	 * bytes are present (preferred), or always return object created from as much
	 * as was able to be parsed.
	 */
	single_basic_parse("12.3xxx", 0);

	single_basic_parse("{\"FoO\"  :   -12.3E512}", 0);
	single_basic_parse("{\"FoO\"  :   -12.3e512}", 0);
	single_basic_parse("{\"FoO\"  :   -12.3E51.2}", 0); /* non-sensical, returns null */
	single_basic_parse("{\"FoO\"  :   -12.3E512E12}", 0); /* non-sensical, returns null */
	single_basic_parse("[\"\\n\"]", 0);
	single_basic_parse("[\"\\nabc\\n\"]", 0);
	single_basic_parse("[null]", 0);
	single_basic_parse("[]", 0);
	single_basic_parse("[false]", 0);
	single_basic_parse("[\"abc\",null,\"def\",12]", 0);
	single_basic_parse("{}", 0);
	single_basic_parse("{ \"foo\": \"bar\" }", 0);
	single_basic_parse("{ \'foo\': \'bar\' }", 0);
	single_basic_parse("{ \"foo\": \"bar\", \"baz\": null, \"bool0\": true }", 0);
	single_basic_parse("{ \"foo\": [null, \"foo\"] }", 0);
	single_basic_parse("{ \"abc\": 12, \"foo\": \"bar\", \"bool0\": false, \"bool1\": true, \"arr\": [ 1, 2, 3, null, 5 ] }", 0);
	single_basic_parse("{ \"abc\": \"blue\nred\\ngreen\" }", 0);

	// Clear serializer for these tests so we see the actual parsed value.
	single_basic_parse("null", 1);
	single_basic_parse("false", 1);
	single_basic_parse("[0e]", 1);
	single_basic_parse("[0e+]", 1);
	single_basic_parse("[0e+-1]", 1);
	single_basic_parse("\"hello world!\"", 1);

  // uint64/int64 range test
	single_basic_parse("[9223372036854775806]", 1);
	single_basic_parse("[9223372036854775807]", 1);
	single_basic_parse("[9223372036854775808]", 1);
	single_basic_parse("[-9223372036854775807]", 1);
	single_basic_parse("[-9223372036854775808]", 1);
	single_basic_parse("[-9223372036854775809]", 1);
	single_basic_parse("[18446744073709551614]", 1);
	single_basic_parse("[18446744073709551615]", 1);
	single_basic_parse("[18446744073709551616]", 1);
}

static void test_utf8_parse()
{
	// json_tokener_parse doesn't support checking for byte order marks.
	// It's the responsibility of the caller to detect and skip a BOM.
	// Both of these checks return null.
	char utf8_bom[] = { 0xEF, 0xBB, 0xBF, 0x00 };
	char utf8_bom_and_chars[] = { 0xEF, 0xBB, 0xBF, '{', '}', 0x00 };
	single_basic_parse(utf8_bom, 0);
	single_basic_parse(utf8_bom_and_chars, 0);
}

// Clear the re-serialization information that the tokener
// saves to ensure that the output reflects the actual
// values we parsed, rather than just the original input.
static void do_clear_serializer(json_object *jso)
{
	json_c_visit(jso, 0, clear_serializer, NULL);
}

static int clear_serializer(json_object *jso, int flags,
                     json_object *parent_jso,
                     const char *jso_key,
                     size_t *jso_index, void *userarg)
{
	if (jso)
		json_object_set_serializer(jso, NULL, NULL, NULL);
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static void test_verbose_parse()
{
	json_object *new_obj;
	enum json_tokener_error error = json_tokener_success;

	new_obj = json_tokener_parse_verbose("{ foo }", &error);
	crtl_assert (error == json_tokener_error_parse_object_key_name);
	crtl_assert (new_obj == NULL);

	new_obj = json_tokener_parse("{ foo }");
	crtl_assert (new_obj == NULL);

	new_obj = json_tokener_parse("foo");
	crtl_assert (new_obj == NULL);
	new_obj = json_tokener_parse_verbose("foo", &error);
	crtl_assert (new_obj == NULL);

	/* b/c the string starts with 'f' parsing return a boolean error */
	crtl_assert (error == json_tokener_error_parse_boolean);

	puts("json_tokener_parse_verbose() OK");
}

struct incremental_step {
	const char *string_to_parse;
	int length;
	int char_offset;
	enum json_tokener_error expected_error;
	int reset_tokener;
} incremental_steps[] = {

	/* Check that full json messages can be parsed, both w/ and w/o a reset */
	{ "{ \"foo\": 123 }", -1, -1, json_tokener_success,  0 },
	{ "{ \"foo\": 456 }", -1, -1, json_tokener_success,  1 },
	{ "{ \"foo\": 789 }", -1, -1, json_tokener_success,  1 },

	/* Check the comment parse*/
	{ "/* hello */{ \"foo\"", -1, -1, json_tokener_continue,  0 },
	{ "/* hello */:/* hello */", -1, -1, json_tokener_continue,  0 },
	{ "\"bar\"/* hello */", -1, -1, json_tokener_continue,  0 },
	{ "}/* hello */", -1, -1, json_tokener_success,  1 },
	{ "/ hello ", -1, 1, json_tokener_error_parse_comment,  1 },
	{ "/* hello\"foo\"", -1, -1, json_tokener_continue,  1 },
	{ "/* hello*\"foo\"", -1, -1, json_tokener_continue,  1 },
	{ "// hello\"foo\"", -1, -1, json_tokener_continue,  1 },

	/*  Check a basic incremental parse */
	{ "{ \"foo",          -1, -1, json_tokener_continue, 0 },
	{ "\": {\"bar",       -1, -1, json_tokener_continue, 0 },
	{ "\":13}}",          -1, -1, json_tokener_success,  1 },

	/* Check that json_tokener_reset actually resets */
	{ "{ \"foo",          -1, -1, json_tokener_continue, 1 },
	{ ": \"bar\"}",       -1, 0, json_tokener_error_parse_unexpected, 1 },

	/* Check incremental parsing with trailing characters */
	{ "{ \"foo",          -1, -1, json_tokener_continue, 0 },
	{ "\": {\"bar",       -1, -1, json_tokener_continue, 0 },
	{ "\":13}}XXXX",      10, 6, json_tokener_success,  0 },
	{ "XXXX",              4, 0, json_tokener_error_parse_unexpected, 1 },

	/* Check that trailing characters can change w/o a reset */
	{ "{\"x\": 123 }\"X\"", -1, 11, json_tokener_success, 0 },
	{ "\"Y\"",            -1, -1, json_tokener_success, 1 },

	/* To stop parsing a number we need to reach a non-digit, e.g. a \0 */
	{ "1",                 1, 1, json_tokener_continue, 0 },
	/* This should parse as the number 12, since it continues the "1" */
	{ "2",                 2, 1, json_tokener_success, 0 },
	{ "12{",               3, 2, json_tokener_success, 1 },
	/* Parse number in strict model */
	{ "[02]",             -1, 3, json_tokener_error_parse_number, 3 },

	/* Similar tests for other kinds of objects: */
	/* These could all return success immediately, since regardless of
	   what follows the false/true/null token we *will* return a json object,
       but it currently doesn't work that way.  hmm... */
	{ "false",             5, 5, json_tokener_continue, 1 },
	{ "false",             6, 5, json_tokener_success, 1 },
	{ "true",              4, 4, json_tokener_continue, 1 },
	{ "true",              5, 4, json_tokener_success, 1 },
	{ "null",              4, 4, json_tokener_continue, 1 },
	{ "null",              5, 4, json_tokener_success, 1 },

	{ "Infinity",          9, 8, json_tokener_success, 1 },
	{ "infinity",          9, 8, json_tokener_success, 1 },
	{ "-infinity",        10, 9, json_tokener_success, 1 },
	{ "infinity",          9, 0, json_tokener_error_parse_unexpected, 3 },
	{ "-infinity",        10, 1, json_tokener_error_parse_unexpected, 3 },

	{ "inf",               3, 3, json_tokener_continue, 0 },
	{ "inity",             6, 5, json_tokener_success, 1 },
	{ "-inf",              4, 4, json_tokener_continue, 0 },
	{ "inity",             6, 5, json_tokener_success, 1 },

	{ "i",                 1, 1, json_tokener_continue, 0 },
	{ "n",                 1, 1, json_tokener_continue, 0 },
	{ "f",                 1, 1, json_tokener_continue, 0 },
	{ "i",                 1, 1, json_tokener_continue, 0 },
	{ "n",                 1, 1, json_tokener_continue, 0 },
	{ "i",                 1, 1, json_tokener_continue, 0 },
	{ "t",                 1, 1, json_tokener_continue, 0 },
	{ "y",                 1, 1, json_tokener_continue, 0 },
	{ "",                  1, 0, json_tokener_success, 1 },

	{ "-",                 1, 1, json_tokener_continue, 0 },
	{ "inf",               3, 3, json_tokener_continue, 0 },
	{ "ini",               3, 3, json_tokener_continue, 0 },
	{ "ty",                3, 2, json_tokener_success, 1 },

	{ "-",                 1, 1, json_tokener_continue, 0 },
	{ "i",                 1, 1, json_tokener_continue, 0 },
	{ "nfini",             5, 5, json_tokener_continue, 0 },
	{ "ty",                3, 2, json_tokener_success, 1 },

	{ "-i",                2, 2, json_tokener_continue, 0 },
	{ "nfinity",           8, 7, json_tokener_success, 1 },

	{ "InfinityX",        10, 8, json_tokener_success, 0 },
	{ "X",                 1, 0, json_tokener_error_parse_unexpected, 1 },

	{ "Infinity1234",     13, 8, json_tokener_success, 0 },
	{ "1234",              5, 4, json_tokener_success, 1 },

	{ "Infinity9999",      8, 8, json_tokener_continue, 0 },

	/* returns the Infinity loaded up by the previous call: */
	{ "1234",              5, 0, json_tokener_success, 0 },
	{ "1234",              5, 4, json_tokener_success, 1 },

	/* offset=1 because "n" is the start of "null".  hmm... */
	{ "noodle",            7, 1, json_tokener_error_parse_null, 1 },
	/* offset=2 because "na" is the start of "nan".  hmm... */
	{ "naodle",            7, 2, json_tokener_error_parse_null, 1 },
	/* offset=2 because "tr" is the start of "true".  hmm... */
	{ "track",             6, 2, json_tokener_error_parse_boolean, 1 },
	{ "fail",              5, 2, json_tokener_error_parse_boolean, 1 },

	/* Although they may initially look like they should fail,
	   the next few tests check that parsing multiple sequential
       json objects in the input works as expected */
	{ "null123",           9, 4, json_tokener_success, 0 },
	{ &"null123"[4],       4, 3, json_tokener_success, 1 },
	{ "nullx",             5, 4, json_tokener_success, 0 },
	{ &"nullx"[4],         2, 0, json_tokener_error_parse_unexpected, 1 },
	{ "{\"a\":1}{\"b\":2}",15, 7, json_tokener_success, 0 },
	{ &"{\"a\":1}{\"b\":2}"[7],
	                       8, 7, json_tokener_success, 1 },

	/* Some bad formatting. Check we get the correct error status */
	{ "2015-01-15",       10, 4, json_tokener_error_parse_number, 1 },

	/* Strings have a well defined end point, so we can stop at the quote */
	{ "\"blue\"",         -1, -1, json_tokener_success, 0 },

	/* Check each of the escape sequences defined by the spec */
	{ "\"\\\"\"",         -1, -1, json_tokener_success, 0 },
	{ "\"\\\\\"",         -1, -1, json_tokener_success, 0 },
	{ "\"\\b\"",         -1, -1, json_tokener_success, 0 },
	{ "\"\\f\"",         -1, -1, json_tokener_success, 0 },
	{ "\"\\n\"",         -1, -1, json_tokener_success, 0 },
	{ "\"\\r\"",         -1, -1, json_tokener_success, 0 },
	{ "\"\\t\"",         -1, -1, json_tokener_success, 0 },
	{ "\"\\/\"",         -1, -1, json_tokener_success, 0 },
	// Escaping a forward slash is optional
	{ "\"/\"",           -1, -1, json_tokener_success, 0 },
	/* Check wrong escape sequences */
	{ "\"\\a\"",         -1, 2, json_tokener_error_parse_string, 1 },

	/* Check '\'' in strict model */
	{ "\'foo\'",         -1, 0, json_tokener_error_parse_unexpected, 3 },

	/* Parse array/object */
	{ "[1,2,3]",          -1, -1, json_tokener_success, 0 },
	{ "[1,2,3}",         -1, 6, json_tokener_error_parse_array, 1 },
	{ "{\"a\"}",         -1, 4, json_tokener_error_parse_object_key_sep, 1 },
	{ "{\"a\":1]",       -1, 6, json_tokener_error_parse_object_value_sep, 1 },
	{ "{\"a\"::1}",      -1, 5, json_tokener_error_parse_unexpected, 1 },
	{ "{\"a\":}",        -1, 5, json_tokener_error_parse_unexpected, 1 },
	{ "{\"a\":1,\"a\":2}",-1, -1, json_tokener_success, 1 },
	{ "\"a\":1}",        -1, 3, json_tokener_success, 1 },
	{ "{\"a\":1",        -1, -1, json_tokener_continue, 1 },
	{ "[,]",             -1, 1, json_tokener_error_parse_unexpected, 1 },
	{ "[,1]",             -1, 1, json_tokener_error_parse_unexpected, 1 },

	/* This behaviour doesn't entirely follow the json spec, but until we have
	   a way to specify how strict to be we follow Postel's Law and be liberal
	   in what we accept (up to a point). */
	{ "[1,2,3,]",         -1, -1, json_tokener_success, 0 },
	{ "[1,2,,3,]",        -1, 5, json_tokener_error_parse_unexpected, 0 },

	{ "[1,2,3,]",         -1, 7, json_tokener_error_parse_unexpected, 3 },
	{ "{\"a\":1,}",         -1, 7, json_tokener_error_parse_unexpected, 3 },

  // utf-8 test
  // acsll encoding
	{ "\x22\x31\x32\x33\x61\x73\x63\x24\x25\x26\x22",-1, -1, json_tokener_success, 5 },
	{ "\x22\x31\x32\x33\x61\x73\x63\x24\x25\x26\x22",-1, -1, json_tokener_success, 1 },
  // utf-8 encoding
	{ "\x22\xe4\xb8\x96\xe7\x95\x8c\x22",-1, -1, json_tokener_success, 5 },
	{ "\x22\xe4\xb8",-1, 3, json_tokener_error_parse_utf8_string, 4 },
	{ "\x96\xe7\x95\x8c\x22",-1, 0, json_tokener_error_parse_utf8_string, 5 },
	{ "\x22\xe4\xb8\x96\xe7\x95\x8c\x22",-1, -1, json_tokener_success, 1 },
	{ "\x22\xcf\x80\xcf\x86\x22",-1, -1, json_tokener_success, 5 },
	{ "\x22\xf0\xa5\x91\x95\x22",-1, -1, json_tokener_success, 5 },
  // wrong utf-8 encoding
	{ "\x22\xe6\x9d\x4e\x22",-1, 3, json_tokener_error_parse_utf8_string, 5 },
	{ "\x22\xe6\x9d\x4e\x22",-1, 5, json_tokener_success, 1 },
  // GBK encoding
	{ "\x22\xc0\xee\xc5\xf4\x22",-1, 2, json_tokener_error_parse_utf8_string, 5 },
	{ "\x22\xc0\xee\xc5\xf4\x22",-1, 6, json_tokener_success, 1 },
  // char after space
	{ "\x20\x20\x22\xe4\xb8\x96\x22",-1, -1, json_tokener_success, 5 },
	{ "\x20\x20\x81\x22\xe4\xb8\x96\x22",-1, 2, json_tokener_error_parse_utf8_string, 5 },
	{ "\x5b\x20\x81\x31\x5d",-1, 2, json_tokener_error_parse_utf8_string, 5 },
  // char in state inf
	{ "\x49\x6e\x66\x69\x6e\x69\x74\x79",9, 8, json_tokener_success, 1 },
	{ "\x49\x6e\x66\x81\x6e\x69\x74\x79",-1, 3, json_tokener_error_parse_utf8_string, 5 },
  // char in escape unicode
	{ "\x22\x5c\x75\x64\x38\x35\x35\x5c\x75\x64\x63\x35\x35\x22",15, 14, json_tokener_success, 5 },
	{ "\x22\x5c\x75\x64\x38\x35\x35\xc0\x75\x64\x63\x35\x35\x22",-1, 8, json_tokener_error_parse_utf8_string, 5 },
	{ "\x22\x5c\x75\x64\x30\x30\x33\x31\xc0\x22",-1, 9, json_tokener_error_parse_utf8_string, 5 },
  // char in number
	{ "\x31\x31\x81\x31\x31",-1, 2, json_tokener_error_parse_utf8_string, 5 },
  // char in object
	{ "\x7b\x22\x31\x81\x22\x3a\x31\x7d",-1, 3, json_tokener_error_parse_utf8_string, 5 },

	{ NULL, -1, -1, json_tokener_success, 0 },
};

static void test_incremental_parse()
{
	json_object *new_obj;
	enum json_tokener_error jerr;
	struct json_tokener *tok;
	const char *string_to_parse;
	int ii;
	int num_ok, num_error;

	num_ok = 0;
	num_error = 0;

	printf("Starting incremental tests.\n");
	printf("Note: quotes and backslashes seen in the output here are literal values passed\n");
	printf("     to the parse functions.  e.g. this is 4 characters: \"\\f\"\n");

	string_to_parse = "{ \"foo"; /* } */
	printf("json_tokener_parse(%s) ... ", string_to_parse);
	new_obj = json_tokener_parse(string_to_parse);
	if (new_obj == NULL) puts("got error as expected");

	/* test incremental parsing in various forms */
	tok = json_tokener_new();
	for (ii = 0; incremental_steps[ii].string_to_parse != NULL; ii++)
	{
		int this_step_ok = 0;
		struct incremental_step *step = &incremental_steps[ii];
		int length = step->length;
		size_t expected_char_offset;

		if (step->reset_tokener & 2)
			{
				if (step->reset_tokener & 4)
					json_tokener_set_flags(tok, 3);
				else
					json_tokener_set_flags(tok, JSON_TOKENER_STRICT);
			}
		else
			{
				if (step->reset_tokener & 4)
					json_tokener_set_flags(tok, JSON_TOKENER_VALIDATE_UTF8);
				else
					json_tokener_set_flags(tok, 0);
			}

		if (length == -1)
			length = strlen(step->string_to_parse);
		if (step->char_offset == -1)
			expected_char_offset = length;
		else
			expected_char_offset = step->char_offset;

		printf("json_tokener_parse_ex(tok, %-12s, %3d) ... ",
			step->string_to_parse, length);
		new_obj = json_tokener_parse_ex(tok, step->string_to_parse, length);

		jerr = json_tokener_get_error(tok);
		if (step->expected_error != json_tokener_success)
		{
			if (new_obj != NULL)
				printf("ERROR: invalid object returned: %s\n",
				       json_object_to_json_string(new_obj));
			else if (jerr != step->expected_error)
				printf("ERROR: got wrong error: %s\n",
				       json_tokener_error_desc(jerr));
			else if (json_tokener_get_parse_end(tok) != expected_char_offset)
				printf("ERROR: wrong char_offset %zu != expected %zu\n",
				       json_tokener_get_parse_end(tok),
				       expected_char_offset);
			else
			{
				printf("OK: got correct error: %s\n",
				       json_tokener_error_desc(jerr));
				this_step_ok = 1;
			}
		}
		else
		{
			if (new_obj == NULL &&
			    !(step->length >= 4 &&
			      strncmp(step->string_to_parse, "null", 4) == 0))
				printf("ERROR: expected valid object, instead: %s\n",
				       json_tokener_error_desc(jerr));
			else if (json_tokener_get_parse_end(tok) != expected_char_offset)
				printf("ERROR: wrong char_offset %zu != expected %zu\n",
				       json_tokener_get_parse_end(tok),
				       expected_char_offset);
			else
			{
				printf("OK: got object of type [%s]: %s\n",
					json_type_to_name(json_object_get_type(new_obj)),
					json_object_to_json_string(new_obj));
				this_step_ok = 1;
			}
		}

		if (new_obj)
			json_object_put(new_obj);

		if (step->reset_tokener & 1)
			json_tokener_reset(tok);

		if (this_step_ok)
			num_ok++;
		else
			num_error++;
	}

	json_tokener_free(tok);

	printf("End Incremental Tests OK=%d ERROR=%d\n", num_ok, num_error);
}




int demo_crypto_json_test15(void)
{

	static const char separator[] = "==================================";
	test_basic_parse();
	puts(separator);
	test_utf8_parse();
	puts(separator);
	test_verbose_parse();
	puts(separator);
	test_incremental_parse();
	puts(separator);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void checkit(const char *buf)
{
	int64_t cint64 = -666;

	int retval = json_parse_int64(buf, &cint64);
	printf("buf=%s parseit=%d, value=%" PRId64 " \n", buf, retval, cint64);
}

void checkit_uint(const char *buf)
{
	uint64_t cuint64 = 666;

	int retval = json_parse_uint64(buf, &cuint64);
	printf("buf=%s parseit=%d, value=%" PRIu64 " \n", buf, retval, cuint64);
}

/**
 * This test calls json_parse_int64 and json_parse_int64 with a variety
 * of different strings. It's purpose is to ensure that the results are
 * consistent across all different environments that it might be executed in.
 *
 * This always exits with a 0 exit value.  The output should be compared
 * against previously saved expected output.
 */
int demo_crypto_json_test16()
{
	char buf[100];

  printf("==========json_parse_int64() test===========\n");
	checkit("x");

	checkit("0");
	checkit("-0");

	checkit("00000000");
	checkit("-00000000");

	checkit("1");

	strcpy(buf, "2147483647"); // aka INT32_MAX
	checkit(buf);

	strcpy(buf, "-1");
	checkit(buf);

	strcpy(buf, "   -1");
	checkit(buf);

	strcpy(buf, "00001234");
	checkit(buf);

	strcpy(buf, "0001234x");
	checkit(buf);

	strcpy(buf, "-00001234");
	checkit(buf);

	strcpy(buf, "-00001234x");
	checkit(buf);

	strcpy(buf, "4294967295"); // aka UINT32_MAX
	checkit(buf);

	strcpy(buf, "4294967296");  // aka UINT32_MAX + 1
	checkit(buf);

	strcpy(buf, "21474836470"); // INT32_MAX * 10
	checkit(buf);

	strcpy(buf, "31474836470"); // INT32_MAX * 10 + a bunch
	checkit(buf);

	strcpy(buf, "-2147483647"); // INT32_MIN + 1
	checkit(buf);

	strcpy(buf, "-2147483648"); // INT32_MIN
	checkit(buf);

	strcpy(buf, "-2147483649"); // INT32_MIN - 1
	checkit(buf);

	strcpy(buf, "-21474836480"); // INT32_MIN * 10
	checkit(buf);

	strcpy(buf, "9223372036854775806"); // INT64_MAX - 1
	checkit(buf);

	strcpy(buf, "9223372036854775807"); // INT64_MAX
	checkit(buf);

	strcpy(buf, "9223372036854775808"); // INT64_MAX + 1
	checkit(buf);

	strcpy(buf, "-9223372036854775808"); // INT64_MIN
	checkit(buf);

	strcpy(buf, "-9223372036854775809"); // INT64_MIN - 1
	checkit(buf);

	strcpy(buf, "18446744073709551614"); // UINT64_MAX - 1
	checkit(buf);

	strcpy(buf, "18446744073709551615"); // UINT64_MAX
	checkit(buf);

	strcpy(buf, "18446744073709551616"); // UINT64_MAX + 1
	checkit(buf);

	strcpy(buf, "-18446744073709551616"); // -UINT64_MAX
	checkit(buf);

	// Ensure we can still parse valid numbers after parsing out of range ones.
	strcpy(buf, "123");
	checkit(buf);

  printf("\n==========json_parse_uint64() test===========\n");
	checkit_uint("x");

	checkit_uint("0");
	checkit_uint("-0");

	checkit_uint("00000000");
	checkit_uint("-00000000");

	checkit_uint("1");

	strcpy(buf, "2147483647"); // aka INT32_MAX
	checkit_uint(buf);

	strcpy(buf, "-1");
	checkit_uint(buf);

	strcpy(buf, "-9223372036854775808");
	checkit_uint(buf);

	strcpy(buf, "   1");
	checkit_uint(buf);

	strcpy(buf, "00001234");
	checkit_uint(buf);

	strcpy(buf, "0001234x");
	checkit_uint(buf);

	strcpy(buf, "4294967295"); // aka UINT32_MAX
	checkit_uint(buf);

	strcpy(buf, "4294967296");  // aka UINT32_MAX + 1
	checkit_uint(buf);

	strcpy(buf, "21474836470"); // INT32_MAX * 10
	checkit_uint(buf);

	strcpy(buf, "31474836470"); // INT32_MAX * 10 + a bunch
	checkit_uint(buf);

	strcpy(buf, "9223372036854775806"); // INT64_MAX - 1
	checkit_uint(buf);

	strcpy(buf, "9223372036854775807"); // INT64_MAX
	checkit_uint(buf);

	strcpy(buf, "9223372036854775808"); // INT64_MAX + 1
	checkit_uint(buf);

	strcpy(buf, "18446744073709551614"); // UINT64_MAX - 1
	checkit_uint(buf);

	strcpy(buf, "18446744073709551615"); // UINT64_MAX
	checkit_uint(buf);

	strcpy(buf, "18446744073709551616"); // UINT64_MAX + 1
	checkit_uint(buf);

	// Ensure we can still parse valid numbers after parsing out of range ones.
	strcpy(buf, "123");
	checkit_uint(buf);

	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



static void test_basic_printbuf_memset()
{
	struct printbuf *pb;

	printf("%s: starting test\n", __func__);
	pb = printbuf_new();
	sprintbuf(pb, "blue:%d", 1);
	printbuf_memset(pb, -1, 'x', 52);
	printf("Buffer contents:%.*s\n", printbuf_length(pb), pb->buf);
	printbuf_free(pb);
	printf("%s: end test\n", __func__);
}

static void test_printbuf_memset_length()
{
	struct printbuf *pb;

	printf("%s: starting test\n", __func__);
	pb = printbuf_new();
	printbuf_memset(pb, -1, ' ', 0);
	printbuf_memset(pb, -1, ' ', 0);
	printbuf_memset(pb, -1, ' ', 0);
	printbuf_memset(pb, -1, ' ', 0);
	printbuf_memset(pb, -1, ' ', 0);
	printf("Buffer length: %d\n", printbuf_length(pb));
	printbuf_memset(pb, -1, ' ', 2);
	printbuf_memset(pb, -1, ' ', 4);
	printbuf_memset(pb, -1, ' ', 6);
	printf("Buffer length: %d\n", printbuf_length(pb));
	printbuf_memset(pb, -1, ' ', 6);
	printf("Buffer length: %d\n", printbuf_length(pb));
	printbuf_memset(pb, -1, ' ', 8);
	printbuf_memset(pb, -1, ' ', 10);
	printbuf_memset(pb, -1, ' ', 10);
	printbuf_memset(pb, -1, ' ', 10);
	printbuf_memset(pb, -1, ' ', 20);
	printf("Buffer length: %d\n", printbuf_length(pb));

	// No length change should occur
	printbuf_memset(pb, 0, 'x', 30);
	printf("Buffer length: %d\n", printbuf_length(pb));

	// This should extend it by one.
	printbuf_memset(pb, 0, 'x', printbuf_length(pb) + 1);
	printf("Buffer length: %d\n", printbuf_length(pb));

	printbuf_free(pb);
	printf("%s: end test\n", __func__);
}

static void test_printbuf_memappend(int *before_resize)
{
	struct printbuf *pb;
	int initial_size;

	printf("%s: starting test\n", __func__);
	pb = printbuf_new();
	printf("Buffer length: %d\n", printbuf_length(pb));

	initial_size = pb->size;

	while(pb->size == initial_size)
	{
		printbuf_memappend_fast(pb, "x", 1);
	}
	*before_resize = printbuf_length(pb) - 1;
	printf("Appended %d bytes for resize: [%s]\n", *before_resize + 1, pb->buf);

	printbuf_reset(pb);
	printbuf_memappend_fast(pb, "bluexyz123", 3);
	printf("Partial append: %d, [%s]\n", printbuf_length(pb), pb->buf);

	char with_nulls[] = { 'a', 'b', '\0', 'c' };
	printbuf_reset(pb);
	printbuf_memappend_fast(pb, with_nulls, (int)sizeof(with_nulls));
	printf("With embedded \\0 character: %d, [%s]\n", printbuf_length(pb), pb->buf);

	printbuf_free(pb);
	pb = printbuf_new();
	char *data = malloc(*before_resize);
	memset(data, 'X', *before_resize);
	printbuf_memappend_fast(pb, data, *before_resize);
	printf("Append to just before resize: %d, [%s]\n", printbuf_length(pb), pb->buf);

	free(data);
	printbuf_free(pb);

	pb = printbuf_new();
	data = malloc(*before_resize + 1);
	memset(data, 'X', *before_resize + 1);
	printbuf_memappend_fast(pb, data, *before_resize + 1);
	printf("Append to just after resize: %d, [%s]\n", printbuf_length(pb), pb->buf);

	free(data);
	printbuf_free(pb);

#define SA_TEST_STR "XXXXXXXXXXXXXXXX"
	pb = printbuf_new();
	printbuf_strappend(pb, SA_TEST_STR);
	printf("Buffer size after printbuf_strappend(): %d, [%s]\n", printbuf_length(pb), pb->buf);
	printbuf_free(pb);
#undef  SA_TEST_STR

	printf("%s: end test\n", __func__);
}

static void test_sprintbuf(int before_resize)
{
	struct printbuf *pb;
	const char  *max_char = "if string is greater than stack buffer, then use dynamic string"
		" with vasprintf.  Note: some implementation of vsnprintf return -1 "
		" if output is truncated whereas some return the number of bytes that "
		" would have been written - this code handles both cases.";

	printf("%s: starting test\n", __func__);
	pb = printbuf_new();
	printf("Buffer length: %d\n", printbuf_length(pb));

	char *data = malloc(before_resize + 1 + 1);
	memset(data, 'X', before_resize + 1 + 1);
	data[before_resize + 1] = '\0';
	sprintbuf(pb, "%s", data);
	free(data);
	printf("sprintbuf to just after resize(%d+1): %d, [%s], strlen(buf)=%d\n", before_resize, printbuf_length(pb), pb->buf, (int)strlen(pb->buf));

	printbuf_reset(pb);
	sprintbuf(pb, "plain");
	printf("%d, [%s]\n", printbuf_length(pb), pb->buf);

	sprintbuf(pb, "%d", 1);
	printf("%d, [%s]\n", printbuf_length(pb), pb->buf);

	sprintbuf(pb, "%d", INT_MAX);
	printf("%d, [%s]\n", printbuf_length(pb), pb->buf);

	sprintbuf(pb, "%d", INT_MIN);
	printf("%d, [%s]\n", printbuf_length(pb), pb->buf);

	sprintbuf(pb, "%s", "%s");
	printf("%d, [%s]\n", printbuf_length(pb), pb->buf);

	sprintbuf(pb, max_char);
	printf("%d, [%s]\n", printbuf_length(pb), pb->buf);
	printbuf_free(pb);
	printf("%s: end test\n", __func__);
}

int demo_crypto_json_test17()
{
	int before_resize = 0;
    
	test_basic_printbuf_memset();
	printf("========================================\n");
	test_printbuf_memset_length();
	printf("========================================\n");
	test_printbuf_memappend(&before_resize);
	printf("========================================\n");
	test_sprintbuf(before_resize);
	printf("========================================\n");

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


struct myinfo {
	int value;
};

static int freeit_was_called = 0;
static void freeit(json_object *jso, void *userdata)
{
	struct myinfo *info = userdata;
	printf("freeit, value=%d\n", info->value);
	// Don't actually free anything here, the userdata is stack allocated.
	freeit_was_called = 1;
}
static int custom_serializer(struct json_object *o,
					struct printbuf *pb,
					int level,
					int flags)
{
	sprintbuf(pb, "Custom Output");
	return 0;
}

int demo_crypto_json_test18()
{
	json_object *my_object;

	printf("Test setting, then resetting a custom serializer:\n");
	my_object = json_object_new_object();
	json_object_object_add(my_object, "abc", json_object_new_int(12));
	json_object_object_add(my_object, "foo", json_object_new_string("bar"));

	printf("my_object.to_string(standard)=%s\n", json_object_to_json_string(my_object));

	struct myinfo userdata = { .value = 123 };
	json_object_set_serializer(my_object, custom_serializer, &userdata, freeit);

	printf("my_object.to_string(custom serializer)=%s\n", json_object_to_json_string(my_object));

	printf("Next line of output should be from the custom freeit function:\n");
	freeit_was_called = 0;
	json_object_set_serializer(my_object, NULL, NULL, NULL);
	crtl_assert(freeit_was_called);

	printf("my_object.to_string(standard)=%s\n", json_object_to_json_string(my_object));

	json_object_put(my_object);

	// ============================================

	my_object = json_object_new_object();
	printf("Check that the custom serializer isn't free'd until the last json_object_put:\n");
	json_object_set_serializer(my_object, custom_serializer, &userdata, freeit);
	json_object_get(my_object);
	json_object_put(my_object);
	printf("my_object.to_string(custom serializer)=%s\n", json_object_to_json_string(my_object));
	printf("Next line of output should be from the custom freeit function:\n");

	freeit_was_called = 0;
	json_object_put(my_object);
	crtl_assert(freeit_was_called);

	return 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int demo_crypto_json_test19()
{
	json_object *tmp=json_object_new_int(123);
	crtl_assert (json_object_get_int(tmp)==123);
	json_object_set_int(tmp,321);
	crtl_assert (json_object_get_int(tmp)==321); 
	printf("INT PASSED\n");
	json_object_set_int64(tmp,(int64_t)321321321);
	crtl_assert (json_object_get_int64(tmp)==321321321); 
	json_object_put(tmp);
	printf("INT64 PASSED\n");
	tmp=json_object_new_uint64(123);
	crtl_assert (json_object_get_int(tmp)==123);
	crtl_assert (json_object_get_int64(tmp)==123);
	crtl_assert (json_object_get_uint64(tmp)==123);
	json_object_set_uint64(tmp,(uint64_t)321321321);
	crtl_assert (json_object_get_uint64(tmp)==321321321);
	json_object_set_uint64(tmp,9223372036854775808U);
	crtl_assert (json_object_get_uint64(tmp)==9223372036854775808U);
	json_object_put(tmp);
	printf("UINT64 PASSED\n");
	tmp=json_object_new_boolean(1);
	crtl_assert (json_object_get_boolean(tmp)==1); 
	json_object_set_boolean(tmp,0);
	crtl_assert (json_object_get_boolean(tmp)==0); 
	json_object_set_boolean(tmp,1);
	crtl_assert (json_object_get_boolean(tmp)==1); 
	json_object_put(tmp);
	printf("BOOL PASSED\n");
	tmp=json_object_new_double(12.34);
	crtl_assert (json_object_get_double(tmp)==12.34); 
	json_object_set_double(tmp,34.56);
	crtl_assert (json_object_get_double(tmp)==34.56); 
	json_object_set_double(tmp,6435.34);
	crtl_assert (json_object_get_double(tmp)==6435.34); 
	json_object_set_double(tmp,2e21);
	crtl_assert (json_object_get_int64(tmp)==INT64_MAX);
	crtl_assert (json_object_get_uint64(tmp)==UINT64_MAX);
	json_object_set_double(tmp,-2e21);
	crtl_assert (json_object_get_int64(tmp)==INT64_MIN);
	crtl_assert (json_object_get_uint64(tmp)==0);
	json_object_put(tmp);
	printf("DOUBLE PASSED\n");
#define SHORT "SHORT"
#define MID   "A MID STRING"
//             12345678901234567890123456789012....
#define HUGE  "A string longer than 32 chars as to check non local buf codepath"
	tmp=json_object_new_string(SHORT);
	crtl_assert (strcmp(json_object_get_string(tmp),SHORT)==0); 
	json_object_set_string(tmp,MID);
	crtl_assert (strcmp(json_object_get_string(tmp),MID)==0); 
	json_object_set_string(tmp,HUGE);
	crtl_assert (strcmp(json_object_get_string(tmp),HUGE)==0); 
	json_object_set_string(tmp,SHORT);
	crtl_assert (strcmp(json_object_get_string(tmp),SHORT)==0); 
	json_object_put(tmp);
	printf("STRING PASSED\n");

	tmp = json_tokener_parse("1.234");
	json_object_set_double(tmp, 12.3);
	const char *serialized = json_object_to_json_string(tmp);
	fprintf(stderr, "%s\n", serialized);
	crtl_assert(strncmp(serialized, "12.3", 4)==0);
	json_object_put(tmp);
	printf("PARSE AND SET PASSED\n");

	printf("PASSED\n");
	return 0;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static void test_read_valid_with_fd(const char *testdir);
static void test_read_valid_nested_with_fd(const char *testdir);
static void test_read_nonexistant();
static void test_read_closed(void);

static void test_write_to_file();
static void stat_and_cat(const char *file);

#ifndef PATH_MAX
#define PATH_MAX 256
#endif

static void test_write_to_file()
{
	json_object *jso;

	jso = json_tokener_parse("{"
		"\"foo\":1234,"
		"\"foo1\":\"abcdefghijklmnopqrstuvwxyz\","
		"\"foo2\":\"abcdefghijklmnopqrstuvwxyz\","
		"\"foo3\":\"abcdefghijklmnopqrstuvwxyz\","
		"\"foo4\":\"abcdefghijklmnopqrstuvwxyz\","
		"\"foo5\":\"abcdefghijklmnopqrstuvwxyz\","
		"\"foo6\":\"abcdefghijklmnopqrstuvwxyz\","
		"\"foo7\":\"abcdefghijklmnopqrstuvwxyz\","
		"\"foo8\":\"abcdefghijklmnopqrstuvwxyz\","
		"\"foo9\":\"abcdefghijklmnopqrstuvwxyz\""
		"}");
	const char *outfile = "json.out";
	int rv = json_object_to_file(outfile, jso);
	printf("%s: json_object_to_file(%s, jso)=%d\n",
		(rv == 0) ? "OK" : "FAIL", outfile, rv);
	if (rv == 0)
		stat_and_cat(outfile);

	putchar('\n');

	const char *outfile2 = "json2.out";
	rv = json_object_to_file_ext(outfile2, jso, JSON_C_TO_STRING_PRETTY);
	printf("%s: json_object_to_file_ext(%s, jso, JSON_C_TO_STRING_PRETTY)=%d\n",
	       (rv == 0) ? "OK" : "FAIL", outfile2, rv);
	if (rv == 0)
		stat_and_cat(outfile2);

	const char *outfile3 = "json3.out";
	int d = open(outfile3, O_WRONLY|O_CREAT, 0600);
	if (d < 0)
	{
		printf("FAIL: unable to open %s %s\n", outfile3, strerror(errno));
		return;
	}
	rv = json_object_to_fd(d, jso, JSON_C_TO_STRING_PRETTY);
	printf("%s: json_object_to_fd(%s, jso, JSON_C_TO_STRING_PRETTY)=%d\n",
	       (rv == 0) ? "OK" : "FAIL", outfile3, rv);
	// Write the same object twice
	rv = json_object_to_fd(d, jso, JSON_C_TO_STRING_PLAIN);
	printf("%s: json_object_to_fd(%s, jso, JSON_C_TO_STRING_PLAIN)=%d\n",
	       (rv == 0) ? "OK" : "FAIL", outfile3, rv);
	close(d);
	if (rv == 0)
		stat_and_cat(outfile3);

	json_object_put(jso);
}

static void stat_and_cat(const char *file)
{
	struct stat sb;
	int d = open(file, O_RDONLY, 0600);
	if (d < 0)
	{
		printf("FAIL: unable to open %s: %s\n",
		       file, strerror(errno));
		return;
	}
	if (fstat(d, &sb) < 0)
	{
		printf("FAIL: unable to stat %s: %s\n",
		       file, strerror(errno));
		close(d);
		return;
	}
	char *buf = malloc(sb.st_size + 1);
	if(!buf)
	{
		printf("FAIL: unable to allocate memory\n");
		close(d);
		return;
	}
	if (read(d, buf, sb.st_size) < sb.st_size)
	{
		printf("FAIL: unable to read all of %s: %s\n",
		       file, strerror(errno));
		free(buf);
		close(d);
		return;
	}
	buf[sb.st_size] = '\0';
	printf("file[%s], size=%d, contents=%s\n", file, (int)sb.st_size, buf);
	free(buf);
	close(d);
}

int demo_crypto_json_test20(int argc, char **argv)
{
//	json_object_to_file(file, obj);
//	json_object_to_file_ext(file, obj, flags);


	const char *testdir;
	if (argc < 2)
	{
		fprintf(stderr,
			"Usage: %s <testdir>\n"
			"  <testdir> is the location of input files\n",
			argv[0]);
		return EXIT_FAILURE;
	}
	testdir = argv[1];

	test_read_valid_with_fd(testdir);
	test_read_valid_nested_with_fd(testdir);
	test_read_nonexistant();
	test_read_closed();
	test_write_to_file();
	return EXIT_SUCCESS;
}

static void test_read_valid_with_fd(const char *testdir)
{
	char filename[PATH_MAX];
	(void)snprintf(filename, sizeof(filename), "%s/valid.json", testdir);

	int d = open(filename, O_RDONLY, 0);
	if (d < 0)
	{
		fprintf(stderr,
			"FAIL: unable to open %s: %s\n",
			filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
	json_object *jso = json_object_from_fd(d);
	if (jso != NULL)
	{
		printf("OK: json_object_from_fd(valid.json)=%s\n",
		       json_object_to_json_string(jso));
		json_object_put(jso);
	}
	else
	{
		fprintf(stderr,
		        "FAIL: unable to parse contents of %s: %s\n",
		        filename, json_util_get_last_err());
	}
	close(d);
}

static void test_read_valid_nested_with_fd(const char *testdir)
{
	char filename[PATH_MAX];
	(void)snprintf(filename, sizeof(filename), "%s/valid_nested.json", testdir);

	int d = open(filename, O_RDONLY, 0);
	if (d < 0)
	{
		fprintf(stderr,
			"FAIL: unable to open %s: %s\n",
			filename, strerror(errno));
		exit(EXIT_FAILURE);
	}
	json_object *jso = json_object_from_fd_ex(d, 20);
	if (jso != NULL)
	{
		printf("OK: json_object_from_fd_ex(valid_nested.json, 20)=%s\n",
		       json_object_to_json_string(jso));
		json_object_put(jso);
	}
	else
	{
		fprintf(stderr,
		        "FAIL: unable to parse contents of %s: %s\n",
		        filename, json_util_get_last_err());
	}

	(void)lseek(d, SEEK_SET, 0);

	jso = json_object_from_fd_ex(d, 3);
	if (jso != NULL)
	{
		printf("FAIL: json_object_from_fd_ex(%s, 3)=%s\n",
		       filename, json_object_to_json_string(jso));
		json_object_put(jso);
	}
	else
	{
		printf("OK: correctly unable to parse contents of valid_nested.json with low max depth: %s\n",
		        json_util_get_last_err());
	}
	close(d);
}

static void test_read_nonexistant()
{
	const char *filename = "./not_present.json";

	json_object *jso = json_object_from_file(filename);
	if (jso != NULL)
	{
		printf("FAIL: json_object_from_file(%s) returned %p when NULL expected\n",
		       filename, (void *)jso);
		json_object_put(jso);
	}
	else
	{
		printf("OK: json_object_from_file(%s) correctly returned NULL: %s\n",
		       filename, json_util_get_last_err());
	}
}

static void test_read_closed()
{
	// Test reading from a closed fd
	int d = open("/dev/null", O_RDONLY, 0);
	if(d < 0)
	{
		puts("FAIL: unable to open");
	}
	// Copy over to a fixed fd number so test output is consistent.
	int fixed_d = 10;
	if (dup2(d, fixed_d) < 0)
	{
		printf("FAIL: unable to dup to fd %d", fixed_d);
	}
	close(d);
	close(fixed_d);

	json_object *jso = json_object_from_fd(fixed_d);
	if (jso != NULL)
	{
		printf("FAIL: read from closed fd returning non-NULL: %p\n",
		       (void *)jso);
		fflush(stdout);
		printf("  jso=%s\n", json_object_to_json_string(jso));
		json_object_put(jso);
		return;
	}
	printf("OK: json_object_from_fd(closed_fd), "
	       "expecting NULL, EBADF, got:NULL, %s\n",
	       json_util_get_last_err());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


static json_c_visit_userfunc emit_object;
static json_c_visit_userfunc skip_arrays;
static json_c_visit_userfunc pop_and_stop;
static json_c_visit_userfunc err_on_subobj2;
static json_c_visit_userfunc pop_array;
static json_c_visit_userfunc stop_array;
static json_c_visit_userfunc err_return;

int demo_crypto_json_test21(void)
{
	const char *input = "{\
		\"obj1\": 123,\
		\"obj2\": {\
			\"subobj1\": \"aaa\",\
			\"subobj2\": \"bbb\",\
			\"subobj3\": [ \"elem1\", \"elem2\", true ],\
		},\
		\"obj3\": 1.234,\
		\"obj4\": [ true, false, null ]\
	}";

	json_object *jso = json_tokener_parse(input);
	printf("jso.to_string()=%s\n", json_object_to_json_string(jso));

	int rv;
	rv = json_c_visit(jso, 0, emit_object, NULL);
	printf("json_c_visit(emit_object)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, skip_arrays, NULL);
	printf("json_c_visit(skip_arrays)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, pop_and_stop, NULL);
	printf("json_c_visit(pop_and_stop)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, err_on_subobj2, NULL);
	printf("json_c_visit(err_on_subobj2)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, pop_array, NULL);
	printf("json_c_visit(pop_array)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, stop_array, NULL);
	printf("json_c_visit(stop_array)=%d\n", rv);
	printf("================================\n\n");

	rv = json_c_visit(jso, 0, err_return, NULL);
	printf("json_c_visit(err_return)=%d\n", rv);
	printf("================================\n\n");

	json_object_put(jso);

	return 0;
}


static int emit_object(json_object *jso, int flags,
                     json_object *parent_jso,
                     const char *jso_key,
                     size_t *jso_index, void *userarg)
{
	printf("flags: 0x%x, key: %s, index: %ld, value: %s\n",
	       flags,
	       (jso_key ? jso_key : "(null)"),
	       (jso_index ? (long)*jso_index : -1L),
	       json_object_to_json_string(jso));
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int skip_arrays(json_object *jso, int flags,
                     json_object *parent_jso,
                     const char *jso_key,
                     size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (json_object_get_type(jso) == json_type_array)
		return JSON_C_VISIT_RETURN_SKIP;
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int pop_and_stop(json_object *jso, int flags,
                     json_object *parent_jso,
                     const char *jso_key,
                     size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (jso_key != NULL && strcmp(jso_key, "subobj1") == 0)
	{
		printf("POP after handling subobj1\n");
		return JSON_C_VISIT_RETURN_POP;
	}
	if (jso_key != NULL && strcmp(jso_key, "obj3") == 0)
	{
		printf("STOP after handling obj3\n");
		return JSON_C_VISIT_RETURN_STOP;
	}
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int err_on_subobj2(json_object *jso, int flags,
                     json_object *parent_jso,
                     const char *jso_key,
                     size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (jso_key != NULL && strcmp(jso_key, "subobj2") == 0)
	{
		printf("ERROR after handling subobj1\n");
		return JSON_C_VISIT_RETURN_ERROR;
	}
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int pop_array(json_object *jso, int flags,
                     json_object *parent_jso,
                     const char *jso_key,
                     size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (jso_index != NULL && (*jso_index == 0))
	{
		printf("POP after handling array[0]\n");
		return JSON_C_VISIT_RETURN_POP;
	}
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int stop_array(json_object *jso, int flags,
                     json_object *parent_jso,
                     const char *jso_key,
                     size_t *jso_index, void *userarg)
{
	(void)emit_object(jso, flags, parent_jso, jso_key, jso_index, userarg);
	if (jso_index != NULL && (*jso_index == 0))
	{
		printf("STOP after handling array[1]\n");
		return JSON_C_VISIT_RETURN_STOP;
	}
	return JSON_C_VISIT_RETURN_CONTINUE;
}

static int err_return(json_object *jso, int flags,
                     json_object *parent_jso,
                     const char *jso_key,
                     size_t *jso_index, void *userarg)
{
	printf("flags: 0x%x, key: %s, index: %ld, value: %s\n",
	       flags,
	       (jso_key ? jso_key : "(null)"),
	       (jso_index ? (long)*jso_index : -1L),
	       json_object_to_json_string(jso));
	return 100;
}


int main(int argc, char *argv[])
{
    crtl_print_info("\n");
    demo_crypto_json_test1();
    
    demo_crypto_json_test2();
    
    demo_crypto_json_test3();
    
    demo_crypto_json_test4();
    
    demo_crypto_json_test5();
    
    demo_crypto_json_test6();

    demo_crypto_json_test7();

    demo_crypto_json_test8(0);

    demo_crypto_json_test9();

    demo_crypto_json_test10();

    demo_crypto_json_test11();

    demo_crypto_json_test12();

    demo_crypto_json_test13();

    demo_crypto_json_test14_null();

    demo_crypto_json_test15();

    demo_crypto_json_test16();

    demo_crypto_json_test17();

    demo_crypto_json_test18();

    demo_crypto_json_test19();

    demo_crypto_json_test20(argc, argv);

    crtl_print_info("\n\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    demo_crypto_json_test21();

    return 0;
}
