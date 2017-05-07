#define CONSOLE_TEST_TITLE(message, ...) fprintf(CONSOLE_OUTPUT, "\nTesting " message "...\n", ##__VA_ARGS__);
#define CONSOLE_TEST_RES(number) fprintf(CONSOLE_OUTPUT,"... %d tests succeeded\n\n",(number));
#define CONSOLE_LOG_TEST(message, ...) if(test_verbosity) fprintf(CONSOLE_OUTPUT, "[ "GREENBOLD" OK "RESET" ] \t" message "\n", ##__VA_ARGS__);
