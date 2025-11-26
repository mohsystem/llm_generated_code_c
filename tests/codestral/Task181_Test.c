#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100

typedef struct {
    char account_number[20];
    double balance;
    bool isOpen;
    pthread_mutex_t lock;
} BankAccount;

typedef struct {
    BankAccount accounts[MAX_ACCOUNTS];
    int account_count;
    pthread_mutex_t bank_lock;
} Bank;

void Bank_init(Bank* bank) {
    bank->account_count = 0;
    pthread_mutex_init(&bank->bank_lock, NULL);
}

BankAccount* Bank_findAccount(Bank* bank, const char* account_number) {
    for (int i = 0; i < bank->account_count; i++) {
        if (strcmp(bank->accounts[i].account_number, account_number) == 0) {
            return &bank->accounts[i];
        }
    }
    return NULL;
}

bool Bank_openAccount(Bank* bank, const char* account_number, double initialBalance) {
    pthread_mutex_lock(&bank->bank_lock);
    if (bank->account_count >= MAX_ACCOUNTS || Bank_findAccount(bank, account_number) != NULL) {
        pthread_mutex_unlock(&bank->bank_lock);
        return false;
    }
    BankAccount* account = &bank->accounts[bank->account_count++];
    strncpy(account->account_number, account_number, sizeof(account->account_number)-1);
    account->account_number[sizeof(account->account_number)-1] = '\0';
    account->balance = initialBalance;
    account->isOpen = true;
    pthread_mutex_init(&account->lock, NULL);
    pthread_mutex_unlock(&bank->bank_lock);
    return true;
}

bool Bank_closeAccount(Bank* bank, const char* account_number) {
    pthread_mutex_lock(&bank->bank_lock);
    BankAccount* account = Bank_findAccount(bank, account_number);
    if (!account) {
        pthread_mutex_unlock(&bank->bank_lock);
        return false;
    }
    pthread_mutex_lock(&account->lock);
    if (!account->isOpen) {
        pthread_mutex_unlock(&account->lock);
        pthread_mutex_unlock(&bank->bank_lock);
        return false;
    }
    account->isOpen = false;
    pthread_mutex_unlock(&account->lock);
    pthread_mutex_unlock(&bank->bank_lock);
    return true;
}

bool Bank_deposit(Bank* bank, const char* account_number, double amount) {
    pthread_mutex_lock(&bank->bank_lock);
    BankAccount* account = Bank_findAccount(bank, account_number);
    if (!account) {
        pthread_mutex_unlock(&bank->bank_lock);
        return false;
    }
    pthread_mutex_lock(&account->lock);
    if (!account->isOpen || amount <= 0) {
        pthread_mutex_unlock(&account->lock);
        pthread_mutex_unlock(&bank->bank_lock);
        return false;
    }
    account->balance += amount;
    pthread_mutex_unlock(&account->lock);
    pthread_mutex_unlock(&bank->bank_lock);
    return true;
}

bool Bank_withdraw(Bank* bank, const char* account_number, double amount) {
    pthread_mutex_lock(&bank->bank_lock);
    BankAccount* account = Bank_findAccount(bank, account_number);
    if (!account) {
        pthread_mutex_unlock(&bank->bank_lock);
        return false;
    }
    pthread_mutex_lock(&account->lock);
    if (!account->isOpen || amount <= 0 || amount > account->balance) {
        pthread_mutex_unlock(&account->lock);
        pthread_mutex_unlock(&bank->bank_lock);
        return false;
    }
    account->balance -= amount;
    pthread_mutex_unlock(&account->lock);
    pthread_mutex_unlock(&bank->bank_lock);
    return true;
}

// Test cases matching your Python tests

void run_tests() {
    Bank bank;
    Bank_init(&bank);

    printf("Test 1: Open account '12345' with 1000: %s\n", Bank_openAccount(&bank, "12345", 1000) ? "PASS" : "FAIL");
    printf("Test 2: Open '12345' again (fail): %s\n", Bank_openAccount(&bank, "12345", 0) ? "FAIL" : "PASS");
    printf("Test 3: Deposit 500 to '12345': %s\n", Bank_deposit(&bank, "12345", 500) ? "PASS" : "FAIL");
    printf("Test 4: Withdraw 200 from '12345': %s\n", Bank_withdraw(&bank, "12345", 200) ? "PASS" : "FAIL");
    printf("Test 5: Withdraw 2000 from '12345' (fail): %s\n", Bank_withdraw(&bank, "12345", 2000) ? "FAIL" : "PASS");
    printf("Test 6: Close '12345': %s\n", Bank_closeAccount(&bank, "12345") ? "PASS" : "FAIL");
    printf("Test 7: Deposit to closed '12345' (fail): %s\n", Bank_deposit(&bank, "12345", 500) ? "FAIL" : "PASS");
    printf("Test 8: Withdraw from closed '12345' (fail): %s\n", Bank_withdraw(&bank, "12345", 200) ? "FAIL" : "PASS");
    printf("Test 9: Close '12345' again (fail): %s\n", Bank_closeAccount(&bank, "12345") ? "FAIL" : "PASS");
    printf("Test 10: Open new account '67890' with 1500: %s\n", Bank_openAccount(&bank, "67890", 1500) ? "PASS" : "FAIL");

    BankAccount* acc = Bank_findAccount(&bank, "67890");
    if (acc && acc->balance == 1500) {
        printf("Test 11: Balance of '67890' == 1500: PASS\n");
    } else {
        printf("Test 11: Balance of '67890' != 1500 or not found: FAIL\n");
    }
}

int main() {
    run_tests();
    return 0;
}
