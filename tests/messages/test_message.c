#include <check.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../../src/messages/message.h"

START_TEST(test_int_serialization) {
    unsigned char buffer[4] = { 0 };

    int before = 42;
    int after;

    serialize_int(buffer, before);
    deserialize_int(buffer, &after);
    ck_assert_int_eq(before, after);
    printf(" [PASS] Test: serialize & deserialze 42 \n");

    before = 0;

    serialize_int(buffer, before);
    deserialize_int(buffer, &after);
    ck_assert_int_eq(before, after);
    printf(" [PASS] Test: serialize & deserialze 0 \n");

    before = -1;

    serialize_int(buffer, before);
    deserialize_int(buffer, &after);
    ck_assert_int_eq(before, after);
    printf(" [PASS] Test: serialize & deserialze -1 \n");

    before = -42;

    serialize_int(buffer, before);
    deserialize_int(buffer, &after);
    ck_assert_int_eq(before, after);
    printf(" [PASS] Test: serialize & deserialze -42 \n");
} END_TEST

START_TEST(test_message_serialization) {
    unsigned char buffer[sizeof(Message)] = { 0 };

    Message before = {1, 2, 3, 4};
    Message after;

    serialize_message(buffer, &before);
    deserialize_message(buffer, &after);
    ck_assert_int_eq(before.sender, after.sender);
    ck_assert_int_eq(before.command, after.command);
    ck_assert_int_eq(before.ticket, after.ticket);
    ck_assert_int_eq(before.value, after.value);
    printf(" [PASS] Test: serialize & deserialze message {1, 2, 3, 4} \n");
} END_TEST

Suite *message_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Queue Test");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_int_serialization);
    tcase_add_test(tc_core, test_message_serialization);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s = message_suite();
    SRunner *runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    int no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}