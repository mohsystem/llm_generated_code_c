#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    int id;
    int balance;
    pthread_mutex_t lock;
    int closed;
} BankAccount;

void init_account(BankAccount *account, int id, int initial_balance) {
    account->id = id;
    account->balance = initial_balance;
    pthread_mutex_init(&account->lock, NULL);
    account->closed = 0;
}

char* deposit(BankAccount *account, int amount) {
    pthread_mutex_lock(&account->lock);
    if (account->closed) {
        pthread_mutex_unlock(&account->lock);
        return "Account is closed";
    }
    if (amount <= 0) {
        pthread_mutex_unlock(&account->lock);
        return "Invalid deposit amount";
    }
    account->balance += amount;
    pthread_mutex_unlock(&account->lock);
    return "Deposit successful";
}

char* withdraw(BankAccount *account, int amount) {
    pthread_mutex_lock(&account->lock);
    if (account->closed) {
        pthread_mutex_unlock(&account->lock);
        return "Account is closed";
    }
    if (amount <= 0) {
        pthread_mutex_unlock(&account->lock);
        return "Invalid withdraw amount";
    }
    if (account->balance >= amount) {
        account->balance -= amount;
        pthread_mutex_unlock(&account->lock);
        return "Withdrawal successful";
    } else {
        pthread_mutex_unlock(&account->lock);
        return "Insufficient funds";
    }
}

char* close_account(BankAccount *account) {
    pthread_mutex_lock(&account->lock);
    if (account->closed) {
        pthread_mutex_unlock(&account->lock);
        return "Account already closed";
    }
    account->closed = 1;
    pthread_mutex_unlock(&account->lock);
    return "Account closed";
}

int get_balance(BankAccount* account) {
    int balance;
    pthread_mutex_lock(&account->lock);
    balance = account->balance;
    pthread_mutex_unlock(&account->lock);
    return balance;
}

void test_case(const char* test_name, int condition) {
    printf("%s: %s\n", test_name, condition ? "PASS" : "FAIL");
}

int main() {
    BankAccount account;

    // Test 1: Open a new account
    // NOT SUPPORTED: No global bank object or multiple accounts, so print fail
    printf("Test 1 - Open a new account: FAILLLLLLLL\n");

    // Test 2: Open the same account again (should fail)
    printf("Test 2 - Open the same account again: FAILLLLLLLL\n");

    // Initialize account for next tests
    init_account(&account, 1, 1000);

    // Test 3: Deposit money into the account
    test_case("Test 3 - Deposit 500", strcmp(deposit(&account, 500), "Deposit successful") == 0 && get_balance(&account) == 1500);

    // Test 4: Withdraw money from the account
    test_case("Test 4 - Withdraw 200", strcmp(withdraw(&account, 200), "Withdrawal successful") == 0 && get_balance(&account) == 1300);

    // Test 5: Withdraw more money than available (should fail)
    test_case("Test 5 - Withdraw 2000 fail", strcmp(withdraw(&account, 2000), "Insufficient funds") == 0 && get_balance(&account) == 1300);

    // Test 6: Close the account
    test_case("Test 6 - Close account", strcmp(close_account(&account), "Account closed") == 0);

    // Test 7: Try to deposit into a closed account (should fail)
    test_case("Test 7 - Deposit after close", strcmp(deposit(&account, 500), "Account is closed") == 0);

    // Test 8: Try to withdraw from a closed account (should fail)
    test_case("Test 8 - Withdraw after close", strcmp(withdraw(&account, 200), "Account is closed") == 0);

    // Test 9: Close the same account again (should fail)
    test_case("Test 9 - Close again", strcmp(close_account(&account), "Account already closed") == 0);

    // Test 10: Try to open another account
    // NOT SUPPORTED: No multiple accounts, print fail
    printf("Test 10 - Open another account: FAILLLLLLLL\n");

    return 0;
}
