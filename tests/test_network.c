#include <check.h>

#include <stdlib.h>
#include <stdio.h>

#include "../src/network.h"

START_TEST(test_network_initialization) {
    Network *network = initNetwork();
    ck_assert_ptr_nonnull(network);
    printf(" [PASS] Test: network != NULL \n");

    destroyNetwork(network);
} END_TEST

Suite *network_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Network Test");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_network_initialization);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    Suite *s = network_suite();
    SRunner *runner = srunner_create(s);

    srunner_run_all(runner, CK_NORMAL);
    int no_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}