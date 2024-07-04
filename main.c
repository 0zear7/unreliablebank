#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

struct Customer
{
    char username[32];
    double balance;
};

struct Customer sessionUser;

void create_customer()
{
    struct Customer customer;

    printf("username: ");
    scanf("%s", customer.username);
    customer.balance = 1000;

    FILE *pFile = fopen("data.bin", "ab+");
    fwrite(&customer, sizeof(struct Customer), 1, pFile);
    fclose(pFile);

    printf("customer username is %s current balance is %lf\n", customer.username, customer.balance);

}

void get_customers()
{
    struct Customer customer;

    FILE *pFile = fopen("data.bin", "rb");
    fseek(pFile, sizeof(struct Customer), SEEK_END);
    long unsigned length = ftell(pFile) / sizeof(struct Customer) - 1;
    for (int i = 0; i < length; i++)
    {
        fseek(pFile, sizeof(struct Customer) * i, SEEK_SET);
        fread(&customer, sizeof(struct Customer), 1, pFile);
        printf("customer username is %s current balance is %lf\n", customer.username, customer.balance);
    }
    fclose(pFile);
}

void login()
{
    FILE *pFile = fopen("data.bin", "rb");
    struct Customer existingUser;
    char username[32];
    printf("username: ");
    scanf("%s", username);
    struct Customer customer;

    fseek(pFile, sizeof(struct Customer), SEEK_END);
    long unsigned length = ftell(pFile) / sizeof(struct Customer) - 1;
    for (int i = 0; i < length; i++)
    {
        fseek(pFile, sizeof(struct Customer) * i, SEEK_SET);
        fread(&customer, sizeof(struct Customer), 1, pFile);
        if (strcmp(username, customer.username) == 0)
        {
            printf("customer username is %s current balance is %lf\n", customer.username, customer.balance);
            sessionUser = customer;
            printf("Signed in as %s\n", sessionUser.username);
        }
    }
    fclose(pFile);
}

void whoami()
{
    if (strlen(sessionUser.username) > 0)
    {
        printf("You're signed in as %s, your balance is %lf\n", sessionUser.username, sessionUser.balance);
        return;
    }
    printf("You're not signed in!");
}

void transfer_money()
{
    FILE *pFile = fopen("data.bin", "rb");
    struct Customer existingUser;
    char username[32];
    double amount;
    printf("amount: ");
    scanf("%lf", &amount);
    printf("username: ");
    scanf("%s", username);
    struct Customer customer;

    fseek(pFile, sizeof(struct Customer), SEEK_END);
    long unsigned length = ftell(pFile) / sizeof(struct Customer) - 1;
    for (int i = 0; i < length; i++)
    {
        fseek(pFile, sizeof(struct Customer) * i, SEEK_SET);
        fread(&customer, sizeof(struct Customer), 1, pFile);
        if (strcmp(username, customer.username) == 0)
        {
            sessionUser.balance = sessionUser.balance - amount;
            customer.balance = customer.balance + amount;
            return;
        }
    }
    fclose(pFile);
}

int main()
{
    while (1)
    {
        int choice;

        printf("1.Login\n");
        printf("2.register as a new customer\n");
        printf("3.Who Am I\n");

        printf("choice:");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                login();
                break;
            case 2:
                create_customer();
                break;
            case 3:
                whoami();
                break;
        }
    }
    return EXIT_SUCCESS;
}
