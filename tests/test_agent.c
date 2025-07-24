#include <stdio.h>
#include <jansson.h>
#include "unity.h"
#include "serialize.h"
#include "agent.h"

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

void test_agent_chat(void) {
    json_t *root = json_string("Hey there, please give me a long term strategy that maximizes science and trade output in my cities.");

    char *response_text = chat(root);
    TEST_ASSERT_NOT_NULL(response_text);
    printf(response_text);

    json_decref(root);
}

void test_agent_generate(void) {

    TEST_FAIL_MESSAGE("Test not implemented yet.");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_agent_chat);
    RUN_TEST(test_agent_generate);
    return UNITY_END();
}