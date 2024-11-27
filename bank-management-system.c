#include<stdio.h>
#include<string.h>

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();
  
typedef struct {
    char name[50];
    int acc_no;
    float balance;
} Account; 

int main(){
    int options;
    while(1){
    printf("\n\n*** Bank Management System ***");
    printf("\n1. Create Account");
    printf("\n2. Deposit Money");
    printf("\n3. Withdraw Money");
    printf("\n4. Check Balance");
    printf("\n5. Exit");
    printf("\nEnter your choice: ");
    scanf("%d", &options);

    switch(options){
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

void create_account(){
  Account acc;
  FILE *file = fopen("account.dat","ab+");
  if (file == NULL){
    printf("\nUnable to open file!!!");
    return;
  }

  printf("\nEnter your name: ");
  getchar();
  fgets(acc.name, sizeof(acc.name), stdin);
  int idx = strcspn(acc.name, "\n");
  acc.name[idx] = '\0';
  printf("Enter your account number: ");
  scanf("%d", &acc.acc_no);
  acc.balance = 0;

  fwrite(&acc, sizeof(acc), 1, file);
  fclose(file);
  printf("\nAccount created successfully!");
}

void deposit_money(){
  printf("deposit_money");
}

void withdraw_money(){
  printf("withdraw_money");
}

void check_balance(){
  FILE *file = fopen("account.dat","rb");
  if (file == NULL){
    printf("\nUnable to open file!!!");
    return;
  }
  int account_number;
  Account account_read;
  printf("Enter your account number: ");
  scanf("%d",&account_number);

  while(fread(&account_read, sizeof(account_read), 1, file)){
    if(account_read.acc_no == account_number){
      printf("\nYour current balance is Rs. %.2f", account_read);
      fclose(file);
      return;
    }
  }
  fclose(file);
  printf("\nAccount No:%d was not found.\n",account_number);
}