#include <check.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../../src/network/network.h"

START_TEST(test_network_initialization) {
    Network *network = initNetwork();
    NetworkNode *node = initNode(0);


    ck_assert_ptr_nonnull(network);
    printf(" [PASS] Test: network != NULL \n");

    ck_assert_ptr_nonnull(node);
    printf(" [PASS] Test: node != NULL \n");

    destroyNetwork(network);
} END_TEST

START_TEST(test_network_general) {
    Network *network = initNetwork();
    NetworkNode *node0 = initNode(0);
    NetworkNode *node1 = initNode(1);

    addNode(network, node0);
    addNode(network, node1);


    ck_assert_ptr_eq(node0, getNode(network, 0));
    printf(" [PASS] Test: getNode(network, 0) == node0 \n");

    ck_assert_int_eq(true, containsNode(network, 0));
    printf(" [PASS] Test: containsNode(network, 0) == true \n");

    ck_assert_ptr_eq(node1, getNode(network, 1));
    printf(" [PASS] Test: getNode(network, 1) == node1 \n");

    ck_assert_int_eq(true, containsNode(network, 0));
    printf(" [PASS] Test: containsNode(network, 1) == true \n");


    ck_assert_ptr_eq(getNetworkListNode(network, 1), getPrevNetworkListNode(network, 0));
    printf(" [PASS] Test: 1 == getPrevNetworkListNode(network, 0) \n");

    ck_assert_ptr_null(getPrevNetworkListNode(network, 1));
    printf(" [PASS] Test: getPrevNetworkListNode(network, 1) == NULL \n");


    removeNode(network, 0);

    ck_assert_int_eq(false, containsNode(network, 0));
    printf(" [PASS] Test: containsNode(network, 0) == false \n");

    ck_assert_int_eq(true, containsNode(network, 1));
    printf(" [PASS] Test: containsNode(network, 1) == true \n");


    removeNode(network, 1);
    ck_assert_int_eq(false, containsNode(network, 1));
    printf(" [PASS] Test: containsNode(network, 1) == false \n");

    destroyNetwork(network);
} END_TEST

START_TEST(test_network_remove_permutation) {
    Network *network = initNetwork();
    NetworkNode *node0 = initNode(0);
    NetworkNode *node1 = initNode(1);

    addNode(network, node0);
    addNode(network, node1);

    removeNode(network, 1);

    ck_assert_int_eq(true, containsNode(network, 0));
    printf(" [PASS] Test: containsNode(network, 0) == true \n");

    ck_assert_int_eq(false, containsNode(network, 1));
    printf(" [PASS] Test: containsNode(network, 1) == false \n");

    removeNode(network, 0);

    destroyNetwork(network);
} END_TEST

Suite *network_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Network Test");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_network_initialization);
    tcase_add_test(tc_core, test_network_general);
    tcase_add_test(tc_core, test_network_remove_permutation);
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