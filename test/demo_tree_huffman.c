#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h> //Google自动化测试

#include <crtl/tree.h>
#include <crtl/log.h>
#include <crtl/assert.h>



// Validate the memory allocation for the histogram structure.
static void
test_histogram_allocation(void **state) //直方图
{
    crtl_huf_histogram_t *histogram = NULL;

    crtl_huf_histogram_init(&histogram, 2, 10);
    assert_non_null(histogram);
    assert_non_null(histogram->frequencies);

    // Validate the initial values of the structure members.
    assert_true(histogram->iota == 2);
    assert_true(histogram->length == 10);
    assert_true(histogram->start == -1);

    // Release resource occupied by the histogram.
    crtl_huf_histogram_free(&histogram);
    assert_null(histogram);
}


// Validate the histogram data population.
static void
test_histogram_populate(void **state)
{
    crtl_huf_histogram_t *histogram = NULL;

    crtl_huf_histogram_init(&histogram, 4, 10);
    assert_non_null(histogram);

    uint32_t array1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    crtl_huf_histogram_populate(histogram, array1, sizeof(array1)); //填充
    assert_true(histogram->start == 0);
    unsigned i;
    
    // Validate the linear distribution of the elements.
    for ( i = 0; i < histogram->length; i++) {
        assert_true(histogram->frequencies[i] == 1);
    }

    uint32_t array2[] = {0, 0, 1, 1, 8, 8, 8, 8};
    uint64_t rates2[] = {3, 3, 1, 1, 1, 1, 1, 1, 5, 1};
    crtl_huf_histogram_populate(histogram, array2, sizeof(array2));
    assert_true(histogram->start == 0);

    // Validate the frequencies updates.
    for ( i = 0; i < histogram->length; i++) {
        assert_true(histogram->frequencies[i] == rates2[i]);
    }

    crtl_huf_histogram_free(&histogram);
    assert_null(histogram);
}


// Validate the histogram start attribute updates.
static void
test_histogram_start(void **state)
{
    crtl_huf_histogram_t *histogram = NULL;

    crtl_huf_histogram_init(&histogram, 4, 10);
    assert_non_null(histogram);

    uint32_t array1[] = {4, 4, 5, 5, 5, 5, 9};
    crtl_huf_histogram_populate(histogram, array1, sizeof(array1));
    assert_true(histogram->start == 4);

    uint32_t array2[] = {1, 1, 1, 8, 8, 8};
    crtl_huf_histogram_populate(histogram, array2, sizeof(array2));
    assert_true(histogram->start == 1);

    crtl_huf_histogram_free(&histogram);
    assert_null(histogram);
}


// Validate the histogram reset.
static void
test_histogram_reset(void **state)
{
    crtl_huf_histogram_t *histogram = NULL;

    crtl_huf_histogram_init(&histogram, 4, 10);
    assert_non_null(histogram);

    uint32_t array1[] = {3, 3, 3, 3, 6, 7, 7, 1, 1, 2, 7, 7};
    uint32_t rates1[] = {0, 2, 1, 4, 0, 0, 1, 4, 0, 0};

    crtl_huf_histogram_populate(histogram, array1, sizeof(array1));
    assert_true(histogram->start == 1);
    unsigned i;
    
    for ( i = 0; i < histogram->length; i++) {
        assert_true(histogram->frequencies[i] == rates1[i]);
    }

    crtl_huf_histogram_reset(histogram);
    assert_true(histogram->start == -1);

    for ( i = 0; i < histogram->length; i++) {
        assert_true(histogram->frequencies[i] == 0);
    }

    crtl_huf_histogram_populate(histogram, array1, sizeof(array1));
    assert_true(histogram->start == 1);

    for ( i = 0; i < histogram->length; i++) {
        assert_true(histogram->frequencies[i] == rates1[i]);
    }

    crtl_huf_histogram_free(&histogram);
    assert_null(histogram);
}


int demo1_test_histogram(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_histogram_allocation),
        cmocka_unit_test(test_histogram_populate),
        cmocka_unit_test(test_histogram_start),
        cmocka_unit_test(test_histogram_reset),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


static void
test_symbol_mapping_allocation(void **state)
{
    crtl_huf_symbol_mapping_t *mapping = NULL;

    crtl_huf_symbol_mapping_init(&mapping, 10);
    assert_non_null(mapping);
    assert_true(mapping->length == 10);

    // Create a new element of the mapping.
    crtl_huf_symbol_mapping_element_t *element1 = NULL;
    crtl_huf_symbol_mapping_element_init(&element1, "1011", 4);

    assert_non_null(element1);
    assert_true(element1->length == 4);
    assert_int_equal(strcmp(element1->coding, "1011"), 0);

    // Insert an element into the mapping.
    crtl_huf_symbol_mapping_insert(mapping, 2, element1);

    crtl_huf_symbol_mapping_element_t *element2 = NULL;
    crtl_huf_symbol_mapping_get(mapping, 2, &element2);

    // Validate the elements are pointing to the same
    // data structure.
    assert_non_null(element2);
    assert_true(element1 == element2);

    crtl_huf_symbol_mapping_free(&mapping);
    assert_null(mapping);
}


static void
test_symbol_mapping_insertion(void **state)
{
    crtl_huf_symbol_mapping_t *mapping = NULL;

    crtl_huf_symbol_mapping_init(&mapping, 10);
    assert_non_null(mapping);

    // Define a few mapping elements.
    crtl_huf_symbol_mapping_element_t *element1 = NULL;
    crtl_huf_symbol_mapping_element_init(&element1, "handsomest", 10);

    crtl_huf_symbol_mapping_element_t *element2 = NULL;
    crtl_huf_symbol_mapping_element_init(&element2, "impedance", 9);

    crtl_huf_symbol_mapping_element_t *element3 = NULL;
    crtl_huf_symbol_mapping_element_init(&element3, "magnanimous", 10);

    crtl_huf_symbol_mapping_element_t *element4 = NULL;
    crtl_huf_symbol_mapping_element_init(&element4, "pitchfork", 9);

    // Insert the elements, validate the overlapping insertion.
    crtl_huf_symbol_mapping_insert(mapping, 1, element1);
    crtl_huf_symbol_mapping_insert(mapping, 1, element2);
    crtl_huf_symbol_mapping_insert(mapping, 3, element3);
    crtl_huf_symbol_mapping_insert(mapping, 4, element4);

    crtl_huf_symbol_mapping_element_t *element = NULL;
    crtl_huf_symbol_mapping_element_t *elements[] = {
        NULL, element2, NULL, element3, element4,
        NULL, NULL, NULL, NULL, NULL};
    unsigned i;
    
    for ( i = 0; i < mapping->length; i++) {
        element = NULL;

        crtl_huf_symbol_mapping_get(mapping, i, &element);
        assert_true(element == elements[i]);
    }

    crtl_huf_symbol_mapping_free(&mapping);
    assert_null(mapping);
}


static void
test_symbol_mapping_reset(void **state)
{
    crtl_huf_symbol_mapping_t *mapping = NULL;

    crtl_huf_symbol_mapping_init(&mapping, 5);
    crtl_huf_symbol_mapping_element_t *element1 = NULL;
    unsigned i;
    
    for ( i = 0; i < mapping->length; i++) {
        crtl_huf_symbol_mapping_element_init(&element1, "value", 5);

        crtl_huf_symbol_mapping_insert(mapping, i, element1);
    }

    // Validate all elements of the mapping.
    crtl_huf_symbol_mapping_element_t *element = NULL;
    for ( i = 0; i < mapping->length; i++) {
        element = NULL;

        crtl_huf_symbol_mapping_get(mapping, i, &element);
        assert_non_null(element);

        assert_int_equal(strcmp(element->coding, "value"), 0);
        assert_true(element->length == 5);
    }

    // Reset the mapping and ensure that all elements are zeroed.
    crtl_huf_symbol_mapping_reset(mapping);
    for ( i = 0; i < mapping->length; i++) {
        element = NULL;

        crtl_huf_symbol_mapping_get(mapping, i, &element);
        assert_null(element);
    }

    crtl_huf_symbol_mapping_element_t *element2 = NULL;

    // Repeat all above steps with reseted mapping.
    for ( i = 0; i < mapping->length; i++) {
        crtl_huf_symbol_mapping_element_init(&element2, "attribute", 9);
        crtl_huf_symbol_mapping_insert(mapping, i, element2);
    }

    for ( i = 0; i < mapping->length; i++) {
        element = NULL;

        crtl_huf_symbol_mapping_get(mapping, i, &element);
        assert_non_null(element);

        assert_int_equal(strcmp(element->coding, "attribute"), 0);
        assert_true(element->length == 9);
    }

    crtl_huf_symbol_mapping_free(&mapping);
    assert_null(mapping);
}


int demo1_test_symbol_mapping(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_symbol_mapping_allocation),
        cmocka_unit_test(test_symbol_mapping_insertion),
        cmocka_unit_test(test_symbol_mapping_reset),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}


int main()
{
    demo1_test_histogram();
//    demo1_test_symbol_mapping();

    return 0;
}
