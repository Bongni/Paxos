#include <check.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../../src/network/queue.h"

START_TEST(test_queue_init) {
    Queue *queue = initQueue();


    ck_assert_ptr_nonnull(queue);
    printf(" [PASS] Test: network != NULL \n");

    destroyQueue(queue);
} END_TEST

START_TEST(test_queue_push_pop) {
    Queue *queue = initQueue();

    Message m1 = {1, 1};
    Message m2 = {2, 2};

    push(queue, m1);
    push(queue, m2);

    Message m1_ret = pop(queue);
    Message m2_ret = pop(queue);


    ck_assert_int_eq(m1_ret.sender, m1.sender);
    ck_assert_int_eq(m1_ret.value, m1.value);
    printf(" [PASS] Test: m1_ret == m1 \n");

    ck_assert_int_eq(m2_ret.sender, m2.sender);
    ck_assert_int_eq(m2_ret.value, m2.value);
    printf(" [PASS] Test: m2_ret == m2 \n");
    
    destroyQueue(queue);
} END_TEST

Suite *queue_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Queue Test");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_queue_init);
    tcase_add_test(tc_core, test_queue_push_pop);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s = queue_suite();
    SRunner *runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    int no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}