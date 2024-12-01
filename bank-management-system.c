#include <stdio.h>
#include <string.h>
#include <time.h>

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();

typedef struct
{
  char name[50];
  int acc_no;
  float balance;
} Account;

int main()
{
  int options;
  while (1)
  {
    printf("\n*** Bank Management System ***");
    printf("\n1. Create Account");
    printf("\n2. Deposit Money");
    printf("\n3. Withdraw Money");
    printf("\n4. Check Balance");
    printf("\n5. Exit");
    printf("\nEnter your choice: ");
    scanf("%d", &options);

    switch (options)
    {
    case 1:
      create_account();
      break;
    case 2:
      deposit_money();
      break;
    case 3:
      withdraw_money();
      break;
    case 4:
      check_balance();
      break;
    case 5:
      printf("\nClosing the Bank, Thanks for your visit.\n");
      return 0;
      break;
    default:
      printf("\nInvalid choice!\n");
      break;
    }
  }
  return 0;
}

int count_digits(int number)
{
  int count = 0;
  if (number == 0)
  {
    return 1;
  }
  while (number > 0)
  {
    number /= 10;
    count++;
  }
  return count;
}

void getDate(char *date_str)
{
  time_t now = time(NULL);

  // Convert to local time
  struct tm *local = localtime(&now);

  // Format the date as "DDMonYY"
  strftime(date_str, sizeof(date_str), "%d%b%y", local);
}

void create_account()
{
  Account acc;
  FILE *file = fopen("account.dat", "ab+");
  if (file == NULL)
  {
    printf("\nUnable to open file!!!");
    return;
  }

  printf("\nEnter your name: ");
  getchar();
  fgets(acc.name, sizeof(acc.name), stdin);
  int idx = strcspn(acc.name, "\n");
  acc.name[idx] = '\0';

  do
  {
    printf("Enter your account number: ");
    scanf("%d", &acc.acc_no);

    int digit_count = count_digits(acc.acc_no);
    if (digit_count < 6 || digit_count > 10)
    {
      printf("Account number must have between 6 and 10 digits. Try again.\n");
    }
    else
    {
      break; // Valid account number
    }
  } while (1);

  acc.balance = 0;
  fwrite(&acc, sizeof(acc), 1, file);
  fclose(file);
  printf("\nAccount created successfully!");
}

void deposit_money()
{
  FILE *file = fopen("account.dat", "rb+");
  if (file == NULL)
  {
    printf("Unable to open account file!!!");
    return;
  }
  int acc_no;
  float money;
  Account account_read;
  printf("Enter your account number: ");
  scanf("%d", &acc_no);
  printf("Enter amount to deposit: ");
  scanf("%f", &money);

  while (fread(&account_read, sizeof(account_read), 1, file))
  {
    if (account_read.acc_no == acc_no)
    {
      account_read.balance += money;
      fseek(file, -(long)sizeof(account_read), SEEK_CUR); // to move cursor from current to previous line
      fwrite(&account_read, sizeof(account_read), 1, file);
      fclose(file);
      printf("Successfully deposit Rs.%.2f New balance is Rs.%.2f", money, account_read.balance);
      return;
    }
  }

  fclose(file);
  printf("Money couldn't be deposited as the Account no %d was not found in records.", acc_no);
}

void withdraw_money()
{
  FILE *file = fopen("account.dat", "rb+");
  if (file == NULL)
  {
    printf("\nUnable to open the account file!!!.\n");
    return;
  }

  int acc_no;
  float money;
  Account acc_r;
  printf("Enter your account number: ");
  scanf("%d", &acc_no);
  printf("Enter the amount you wish to withdraw: ");
  scanf("%f", &money);

  while (fread(&acc_r, sizeof(acc_r), 1, file))
  {
    if (acc_r.acc_no == acc_no)
    {
      if (acc_r.balance >= money)
      {
        acc_r.balance -= money;
        fseek(file, -(long)sizeof(acc_r), SEEK_CUR);
        fwrite(&acc_r, sizeof(acc_r), 1, file);
        int hashAccNo = acc_r.acc_no % 10000;
        char date[10];
        getDate(date);
        printf("Dear user, Rs.%.2f withdrawn from A/C XXXX%d on %s. Available balance is Rs.%.2f", money, hashAccNo, date, acc_r.balance);
      }
      else
      {
        printf("Insufficient balance!");
      }
      fclose(file);
      return;
    }
  }

  fclose(file);
  printf("Money could not be withdrawn as the Account no %d was not found in records.", acc_no);
}

void check_balance()
{
  FILE *file = fopen("account.dat", "rb");
  if (file == NULL)
  {
    printf("\nUnable to open file!!!");
    return;
  }
  int account_number;
  Account account_read;
  printf("Enter your account number: ");
  scanf("%d", &account_number);

  while (fread(&account_read, sizeof(account_read), 1, file))
  {
    if (account_read.acc_no == account_number)
    {
      printf("\nYour current balance is Rs.%.2f", account_read.balance);
      fclose(file);
      return;
    }
  }
  fclose(file);
  printf("\nAccount No:%d was not found.\n", account_number);
}
