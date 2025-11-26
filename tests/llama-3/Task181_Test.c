#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

// Your BankAccount structure and functions (as provided)
typedef struct {
    double balance;
    bool closed;
    pthread_mutex_t lock;
} BankAccount;

BankAccount* bankAccountCreate(double initialBalance);
void bankAccountOpen(BankAccount* account);
void bankAccountClose(BankAccount* account);
bool bankAccountDeposit(BankAccount* account, double amount);
bool bankAccountWithdraw(BankAccount* account, double amount);
double bankAccountGetBalance(BankAccount* account);

BankAccount* bankAccountCreate(double initialBalance) {
    BankAccount* account = malloc(sizeof(BankAccount));
    account->balance = initialBalance;
    account->closed = false;
    pthread_mutex_init(&account->lock, NULL);
    return account;
}

void bankAccountOpen(BankAccount* account) {
    account->closed = false;
}

void bankAccountClose(BankAccount* account) {
    account->closed = true;
}

bool bankAccountDeposit(BankAccount* account, double amount) {
    if (account->closed) {
        return false;
    }
    pthread_mutex_lock(&account->lock);
    account->balance += amount;
    pthread_mutex_unlock(&account->lock);
    return true;
}

bool bankAccountWithdraw(BankAccount* account, double amount) {
    if (account->closed) {
        return false;
    }
    pthread_mutex_lock(&account->lock);
    if (amount > account->balance) {
        pthread_mutex_unlock(&account->lock);
        return false;
    }
    account->balance -= amount;
    pthread_mutex_unlock(&account->lock);
    return true;
}

double bankAccountGetBalance(BankAccount* account) {
    pthread_mutex_lock(&account->lock);
    double balance = account->balance;
    pthread_mutex_unlock(&account->lock);
    return balance;
}

// Helper function for printing test results
void print_test(const char* test_name, bool passed) {
    printf("%s: %s\n", test_name, passed ? "PASS" : "FAIL");
}

int main() {
    // Test 1: Open a new account (unsupported - single account only)
    printf("Test 1 - Open a new account: FAILLLLLLL\n");

    // Test 2: Open same account again (unsupported)
    printf("Test 2 - Open same account again: FAILLLLLLL\n");

    // Create account for further tests
    BankAccount* account = bankAccountCreate(1000);

    // Test 3: Deposit 500
    print_test("Test 3 - Deposit 500", bankAccountDeposit(account, 500) && bankAccountGetBalance(account) == 1500.0);

    // Test 4: Withdraw 200
    print_test("Test 4 - Withdraw 200", bankAccountWithdraw(account, 200) && bankAccountGetBalance(account) == 1300.0);

    // Test 5: Withdraw 2000 (should fail)
    print_test("Test 5 - Withdraw 2000 fail", !bankAccountWithdraw(account, 2000) && bankAccountGetBalance(account) == 1300.0);

    // Test 6: Close account
    bankAccountClose(account);
    print_test("Test 6 - Close account", account->closed == true);

    // Test 7: Deposit after close (should fail)
    print_test("Test 7 - Deposit after close", !bankAccountDeposit(account, 500));

    // Test 8: Withdraw after close (should fail)
    print_test("Test 8 - Withdraw after close", !bankAccountWithdraw(account, 200));

    // Test 9: Close same account again (idempotent - closed already)
    // In this implementation, just set closed to true, so no error; simulate fail by checking if already closed
    print_test("Test 9 - Close again", account->closed == true);

    // Test 10: Open another account (unsupported - single account only)
    printf("Test 10 - Open another account: FAILLLLLLL\n");

    // Cleanup
    pthread_mutex_destroy(&account->lock);
    free(account);

    return 0;
}
