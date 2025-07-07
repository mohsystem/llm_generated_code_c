#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#define MAX_ACCOUNTS 100

typedef struct {
    char account_number[20];
    double balance;
    bool is_open;
    pthread_mutex_t mutex;
} BankAccount;

typedef struct {
    BankAccount accounts[MAX_ACCOUNTS];
    int account_count;
    pthread_mutex_t mutex;
} Bank;

BankAccount* find_account(Bank* bank, const char* account_number) {
    for (int i = 0; i < bank->account_count; i++) {
        if (strcmp(bank->accounts[i].account_number, account_number) == 0) {
            return &bank->accounts[i];
        }
    }
    return NULL;
}

bool open_account(Bank* bank, const char* account_number, double initial_balance) {
    pthread_mutex_lock(&bank->mutex);
    if (bank->account_count >= MAX_ACCOUNTS || find_account(bank, account_number) != NULL) {
        pthread_mutex_unlock(&bank->mutex);
        return false;
    }

    BankAccount* account = &bank->accounts[bank->account_count++];
    strncpy(account->account_number, account_number, sizeof(account->account_number) - 1);
    account->account_number[sizeof(account->account_number) - 1] = '\0';
    account->balance = initial_balance;
    account->is_open = true;
    pthread_mutex_init(&account->mutex, NULL);

    pthread_mutex_unlock(&bank->mutex);
    return true;
}

bool close_account(Bank* bank, const char* account_number) {
    pthread_mutex_lock(&bank->mutex);
    BankAccount* account = find_account(bank, account_number);
    if (!account) {
        pthread_mutex_unlock(&bank->mutex);
        return false;
    }

    pthread_mutex_lock(&account->mutex);
    if (!account->is_open) {
        pthread_mutex_unlock(&account->mutex);
        pthread_mutex_unlock(&bank->mutex);
        return false;
    }
    account->is_open = false;
    pthread_mutex_unlock(&account->mutex);

    pthread_mutex_unlock(&bank->mutex);
    return true;
}

bool deposit(Bank* bank, const char* account_number, double amount) {
    pthread_mutex_lock(&bank->mutex);
    BankAccount* account = find_account(bank, account_number);
    if (!account) {
        pthread_mutex_unlock(&bank->mutex);
        return false;
    }

    pthread_mutex_lock(&account->mutex);
    if (!account->is_open || amount <= 0) {
        pthread_mutex_unlock(&account->mutex);
        pthread_mutex_unlock(&bank->mutex);
        return false;
    }
    account->balance += amount;
    pthread_mutex_unlock(&account->mutex);

    pthread_mutex_unlock(&bank->mutex);
    return true;
}

bool withdraw(Bank* bank, const char* account_number, double amount) {
    pthread_mutex_lock(&bank->mutex);
    BankAccount* account = find_account(bank, account_number);
    if (!account) {
        pthread_mutex_unlock(&bank->mutex);
        return false;
    }

    pthread_mutex_lock(&account->mutex);
    if (!account->is_open || amount <= 0 || amount > account->balance) {
        pthread_mutex_unlock(&account->mutex);
        pthread_mutex_unlock(&bank->mutex);
        return false;
    }
    account->balance -= amount;
    pthread_mutex_unlock(&account->mutex);

    pthread_mutex_unlock(&bank->mutex);
    return true;
}

// Test functions without concurrency for strict testing

void test_bank() {
    Bank bank = {0};
    pthread_mutex_init(&bank.mutex, NULL);

    printf("Test 1: Open a new account: %s\n", open_account(&bank, "12345", 1000) ? "PASS" : "FAIL");
    printf("Test 2: Open the same account again: %s\n", open_account(&bank, "12345", 0) ? "FAIL" : "PASS");
    printf("Test 3: Deposit 500 into 12345: %s\n", deposit(&bank, "12345", 500) ? "PASS" : "FAIL");
    printf("Test 4: Withdraw 200 from 12345: %s\n", withdraw(&bank, "12345", 200) ? "PASS" : "FAIL");
    printf("Test 5: Withdraw 2000 from 12345 (fail): %s\n", withdraw(&bank, "12345", 2000) ? "FAIL" : "PASS");
    printf("Test 6: Close account 12345: %s\n", close_account(&bank, "12345") ? "PASS" : "FAIL");
    printf("Test 7: Deposit into closed account 12345: %s\n", deposit(&bank, "12345", 500) ? "FAIL" : "PASS");
    printf("Test 8: Withdraw from closed account 12345: %s\n", withdraw(&bank, "12345", 200) ? "FAIL" : "PASS");
    printf("Test 9: Close the same account again: %s\n", close_account(&bank, "12345") ? "FAIL" : "PASS");
    printf("Test 10: Open another account 67890 with 1500: %s\n", open_account(&bank, "67890", 1500) ? "PASS" : "FAIL");

    BankAccount* acc = find_account(&bank, "67890");
    if (acc) {
        printf("Test 11: Balance of 67890 == 1500: %s\n", (acc->balance == 1500) ? "PASS" : "FAIL");
    } else {
        printf("Test 11: Account 67890 not found: FAIL\n");
    }
}

int main() {
    test_bank();
    return 0;
}
