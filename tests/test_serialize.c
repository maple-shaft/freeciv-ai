#include <stdio.h>
#include <jansson.h>
#include "unity.h"
#include "serialize.h"

void setUp(void) {
    // Setup stuff
}

void tearDown(void) {
    // Tear down stuff
}

static inline void dump(const json_t *data, const char *filename) {
    int rc = json_dump_file(data, filename, JSON_INDENT(2));
    if (rc == 0) {
        printf("Json successfully dumped.\n");
    } else {
        printf("Problem in dumping the file.\n");
    }
}

void test_cm_parameter(void) {
    printf("Entering test_cm_parameter\n");
    struct cm_parameter cobj;
    cm_init_parameter(&cobj);
    struct cm_parameter *pcobj = &cobj;
    pcobj->allow_disorder = true;
    pcobj->allow_specialists = false;
    pcobj->happy_factor = 3;
    pcobj->max_growth = false;
    pcobj->require_happy = true;

    json_t *actual = serialize_cm_parameter(&cobj);
    TEST_ASSERT_NOT_NULL(actual);

    dump(actual, "cmparameter.json");

    json_decref(actual);

    actual = serialize_cm_parameter(NULL);
    TEST_ASSERT_EQUAL_PTR(json_null(), actual);
}

void test_name_translation(void) {
    printf("Entering test_name_translation\n");
    struct name_translation nt;
    names_set(&nt, "domain", "vernacular_name", "rule_name");
    TEST_ASSERT_NOT_NULL(&nt);

    json_t *parent = json_object();
    serialize_name(&nt, parent, "name");
    dump(parent, "name_translation_parent.json");
    json_decref(parent);

}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_cm_parameter);
    RUN_TEST(test_name_translation);
    return UNITY_END();
}

