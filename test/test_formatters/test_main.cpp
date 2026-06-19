#include <unity.h>
#include "formatters.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_formatter_seconds(void) {
    TEST_ASSERT_EQUAL_STRING("0s", formatDuration(0).c_str());
    TEST_ASSERT_EQUAL_STRING("59s", formatDuration(59).c_str());
}

void test_formatter_minutes(void) {
    TEST_ASSERT_EQUAL_STRING("1m", formatDuration(60).c_str());
    TEST_ASSERT_EQUAL_STRING("59m", formatDuration(3599).c_str());
}

void test_formatter_hours(void) {
    TEST_ASSERT_EQUAL_STRING("1h", formatDuration(3600).c_str());
    TEST_ASSERT_EQUAL_STRING("23h", formatDuration(86399).c_str());
}

void test_formatter_days(void) {
    TEST_ASSERT_EQUAL_STRING("1d", formatDuration(86400).c_str());
    TEST_ASSERT_EQUAL_STRING("10d", formatDuration(864000).c_str());
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_formatter_seconds);
    RUN_TEST(test_formatter_minutes);
    RUN_TEST(test_formatter_hours);
    RUN_TEST(test_formatter_days);
    return UNITY_END();
}
