#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    double balance;
    pthread_mutex_t mutex;
    int is_open;
} BankAccount;

BankAccount* createBankAccount(double initial_balance) {
    BankAccount* account = (BankAccount*)malloc(sizeof(BankAccount));
    if (account == NULL) return NULL;
    account->balance = initial_balance;
    pthread_mutex_init(&account->mutex, NULL);
    account->is_open = 1;
    return account;
}

void destroyBankAccount(BankAccount* account) {
    if (account == NULL) return;
    pthread_mutex_destroy(&account->mutex);
    free(account);
}

int openAccount(BankAccount* account) {
    if (!account) return 0;
    pthread_mutex_lock(&account->mutex);
    if (account->is_open) {
        pthread_mutex_unlock(&account->mutex);
        return 0; // already open
    }
    account->is_open = 1;
    pthread_mutex_unlock(&account->mutex);
    return 1;
}

int closeAccount(BankAccount* account) {
    if (!account) return 0;
    pthread_mutex_lock(&account->mutex);
    if (!account->is_open) {
        pthread_mutex_unlock(&account->mutex);
        return 0; // already closed
    }
    account->is_open = 0;
    pthread_mutex_unlock(&account->mutex);
    return 1;
}

int deposit(BankAccount* account, double amount) {
    if (!account) return 0;
    pthread_mutex_lock(&account->mutex);
    if (!account->is_open || amount <= 0) {
        pthread_mutex_unlock(&account->mutex);
        return 0;
    }
    account->balance += amount;
    pthread_mutex_unlock(&account->mutex);
    return 1;
}

int withdraw(BankAccount* account, double amount) {
    if (!account) return 0;
    pthread_mutex_lock(&account->mutex);
    if (!account->is_open || amount <= 0 || amount > account->balance) {
        pthread_mutex_unlock(&account->mutex);
        return 0;
    }
    account->balance -= amount;
    pthread_mutex_unlock(&account->mutex);
    return 1;
}

double getBalance(BankAccount* account) {
    if (!account) return 0;
    pthread_mutex_lock(&account->mutex);
    double bal = account->is_open ? account->balance : 0;
    pthread_mutex_unlock(&account->mutex);
    return bal;
}

// --- دوال الاختبار ---

void test_case(int test_num, int condition) {
    printf("Test %d: %s\n", test_num, condition ? "PASS" : "FAIL");
}

void test_bank() {
    BankAccount* acc1 = createBankAccount(1000);

    // Test 1: open same account again (should fail)
    test_case(1, openAccount(acc1) == 0);

    // Test 2: deposit 500 (success)
    test_case(2, deposit(acc1, 500) == 1 && getBalance(acc1) == 1500);

    // Test 3: withdraw 200 (success)
    test_case(3, withdraw(acc1, 200) == 1 && getBalance(acc1) == 1300);

    // Test 4: withdraw 2000 (fail)
    test_case(4, withdraw(acc1, 2000) == 0 && getBalance(acc1) == 1300);

    // Test 5: close account (success)
    test_case(5, closeAccount(acc1) == 1 && getBalance(acc1) == 0);

    // Test 6: deposit after close (fail)
    test_case(6, deposit(acc1, 500) == 0);

    // Test 7: withdraw after close (fail)
    test_case(7, withdraw(acc1, 200) == 0);

    // Test 8: close again (fail)
    test_case(8, closeAccount(acc1) == 0);

    // Test 9: open again (success)
    test_case(9, openAccount(acc1) == 1);

    // Test 10: check balance restored after reopen
    test_case(10, getBalance(acc1) == 1300);

    destroyBankAccount(acc1);
}

int main() {
    test_bank();
    return 0;
}
