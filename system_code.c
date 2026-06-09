#include <stdio.h>
#include <string.h>

/* CONSTANTS*/
#define MAX_USERS 200
#define MAX_ACCOUNTS 500

#define ROLE_ADMIN 1
#define ROLE_CASHIER 2
#define ROLE_CUSTOMER 3

/* File names for text storage */
#define USERS_FILE "users.txt"
#define ACCOUNTS_FILE "accounts.txt"

/* DATA STRUCTURES */
typedef struct {
    int id;
    int role;                 /* 1=Admin, 2=Cashier, 3=Customer*/
    char username[30];
    char password[30];
    char firstName[30];
    char lastName[30];
    char govId[30];
    
    /* Contact details */
    char address[50];
    char phone[20];
    char email[50];
    
    int active;               /* REPRESENTS THE ACCOUNT STATUS */
} User;

typedef struct {
    int accountNo;
    int ownerUserId;          
    char type[16];            /* ACCOUNT TYPE */
    double balance;
    int active;               
} Account;

/* GLOBAL VARIABLES */
User users[MAX_USERS];		  /* Stores all of the system users */	
int userCount = 0;			  /* Tracks the number of valid users */

Account accounts[MAX_ACCOUNTS];		/* Stores all of the bank accounts */
int accountCount = 0;				/* Tracks the number of valid accounts */

/* FUNCTION PROTOTYPES */
void load_data();
void save_data();
void admin_menu();
void cashier_menu();
void customer_menu(int userIdx);


int main(void)				/* THE MAIN FUNCTION */ 
{
    int choice, userIdx;
    char inUser[30], inPass[30];

    load_data();
    check_first_run();

    while(1) {
        printf("\nWelcome to BIG BANK. Choose whether to login or exit this program below.\n");
        printf("1. Login\n2. Exit\nChoice: ");
        scanf("%d", &choice);

        if (choice == 2) break;				/* Exits the program */
        if (choice == 1) 
		{
            printf("Username: "); scanf("%s", inUser);
            printf("Password: "); scanf("%s", inPass);

            userIdx = find_user_index_by_username(inUser);

            if (userIdx != -1 && strcmp(users[userIdx].password, inPass) == 0) 
			{
                int r = users[userIdx].role;
                printf("\nWelcome, %s!\n", users[userIdx].firstName);
                
                if (r == ROLE_ADMIN) admin_menu();
                else if (r == ROLE_CASHIER) cashier_menu();
                else customer_menu(userIdx);
            } 
			else 
			{
                printf("Invalid credentials. Please make sure your enter your credentials correctly\n");
            }
        }
    }
    return 0;
}

/* EXTRA FUNCTIONS */

int next_user_id() 			/* Provides each user with a unique identifier */
{
    int maxId = 0;
    int i;
    for (i = 0; i < userCount; i++) {
        if (users[i].id > maxId) maxId = users[i].id;
    }
    return maxId + 1;
}

int next_account_no() 		/* Provides each account its own account number */
{
    int maxNo = 10000000; 
    int i;
    for (i = 0; i < accountCount; i++) 
	{
        if (accounts[i].accountNo > maxNo) maxNo = accounts[i].accountNo;
    }
    return maxNo + 1;
}

int find_user_index_by_username(char *username) 	 /* Allows the system to locate the user via their username input */ 	
{
    int i;
    for (i = 0; i < userCount; i++) 
	{
        if (users[i].active && strcmp(users[i].username, username) == 0) 
		{
            return i;
        }
    }
    return -1;
}

int find_user_index_by_id(int id) 					  /* Searches for user in the system based on matching ID number */
{
    int i;
    for (i = 0; i < userCount; i++) 
	{
        if (users[i].active && users[i].id == id) 
		{
            return i;
        }
    }
    return -1;
}

int find_account_index_by_no(int accNo) 			/* Searches the filestore for the account with the specified account number */
{
    int i;
    for (i = 0; i < accountCount; i++) 
	{
        if (accounts[i].active && accounts[i].accountNo == accNo) 
		{
            return i;
        }
    }
    return -1;
}


void save_data() 		
{
    FILE *fp;
    int i;

    fp = fopen(USERS_FILE, "w");						/* Allows the file to store prompted variables*/ 
    if (fp != NULL) {
        fprintf(fp, "%d\n", userCount);
        for (i = 0; i < userCount; i++) 
		{
            fprintf(fp, "%d %d %s %s %s %s %s %s %s %s %d\n", users[i].id, users[i].role, users[i].username, users[i].password, users[i].firstName, users[i].lastName, users[i].govId, users[i].address, users[i].phone, users[i].email, users[i].active);
        }
        fclose(fp);
    }


    fp = fopen(ACCOUNTS_FILE, "w");				/* Allows file to save bank account records */
    if (fp != NULL) 
	{
        fprintf(fp, "%d\n", accountCount);
        for (i = 0; i < accountCount; i++) 
		{
            fprintf(fp, "%d %d %s %.2f %d\n", accounts[i].accountNo, accounts[i].ownerUserId, accounts[i].type, accounts[i].balance, accounts[i].active);
        }
        fclose(fp);
    }
}

void load_data() 
{
    FILE *fp;
    int i;

    fp = fopen(USERS_FILE, "r");				/* Calls system to extract, load, and display the variables listed and stored from before */
    if (fp != NULL) 
	{
        fscanf(fp, "%d", &userCount);
        for (i = 0; i < userCount; i++) 
		{
            fscanf(fp, "%d %d %s %s %s %s %s %s %s %s %d", &users[i].id, &users[i].role, users[i].username, users[i].password, &users[i].firstName, &users[i].lastName, &users[i].govId, &users[i].address, &users[i].phone, &users[i].email, &users[i].active);
        }
        fclose(fp);
    }


    fp = fopen(ACCOUNTS_FILE, "r");				/* Calls systems to present account details requested */
    if (fp != NULL) 
	{
        fscanf(fp, "%d", &accountCount);
        for (i = 0; i < accountCount; i++) 
		{
            fscanf(fp, "%d %d %s %lf %d", &accounts[i].accountNo, &accounts[i].ownerUserId, accounts[i].type, &accounts[i].balance, &accounts[i].active);
        }
        fclose(fp);
    }
}

/* ADMIN MODULE */

void admin_list_users() 						/* Lists all users and their details */
{
    int i;
    printf("\n--- SYSTEM USERS ---\n");
    printf("%-5s %-10s %-6s %-15s %-20s %-15s\n", "ID", "Name", "Role", "Phone", "Email", "Address");
    
    for (i = 0; i < userCount; i++) 
	{
        if (users[i].active) 
		{
            printf("%-5d %-10s %-6d %-15s %-20s %s\n",
                   users[i].id, 
                   users[i].firstName, 
                   users[i].role, 
                   users[i].phone, 
                   users[i].email, 
                   users[i].address);
        }
    }
}

void admin_add_user() 					/*Allows admin to create a user */
{
    User u;
    if (userCount >= MAX_USERS) 
	{ 
	printf("User list full.\n");
	return;
	}
    
    u.id = next_user_id();
    u.active = 1;

    printf("\n--- CREATE NEW USER ---\n");
    printf("Role (1=Admin, 2=Cashier, 3=Customer): ");						/* Prompts for user type */
    scanf("%d", &u.role);
    printf("Username: "); scanf("%s", u.username);
    printf("Password: "); scanf("%s", u.password);
    printf("First Name: "); scanf("%s", u.firstName);
    printf("Last Name: "); scanf("%s", u.lastName);
    printf("Gov ID: "); scanf("%s", u.govId);
    
    printf("Address (No spaces, e.g. 123_Main_St): "); 
    scanf("%s", u.address);
    printf("Phone Number: "); 
    scanf("%s", u.phone);
    printf("Email: "); 
    scanf("%s", u.email);

    users[userCount] = u;
    userCount++;
    save_data();
    printf("User added successfully. ID: %d\n", u.id);
}


void admin_remove_user() 					/* Enables admin to remove a user */
{
    int id, idx;
    printf("\n    REMOVE USER    \n");
    printf("Enter the User ID to be removed: ");
    scanf("%d", &id);

    idx = find_user_index_by_id(id);
    if (idx < 0) 
	{
        printf("User not found.\n");
        return;
    }


    users[idx].active = 0;					/* To deactivate an account */
    save_data();
    printf("User ID %d removed/deactivated successfully.\n", id);
}

void admin_reset_password() 				/* Allows admin to reset any password on any user's behalf */
{
    int id, idx;
    printf("\n    RESET PASSWORD    \n");
    printf("Enter User ID: ");
    scanf("%d", &id);

    idx = find_user_index_by_id(id);
    if (idx < 0) 
	{
        printf("User not found.\n");
        return;
    }

    printf("Enter New Password: ");
    scanf("%s", users[idx].password);
    save_data();
    printf("Password updated successfully.\n");
}


void admin_modify_role() 				/* Allows admin to modify an account type */
{
    int id, idx, newRole;
    printf("\n    MODIFY USER PERMISSIONS    \n");
    printf("Enter User ID: ");
    scanf("%d", &id);

    idx = find_user_index_by_id(id);
    if (idx < 0) 
	{
        printf("User not found.\n");
        return;
    }

    printf("Current Role: %d\n", users[idx].role);
    printf("Enter New Role (1=Admin, 2=Cashier, 3=Customer): ");
    scanf("%d", &newRole);

    if (newRole >= 1 && newRole <= 3) {
        users[idx].role = newRole;
        save_data();
        printf("User permissions updated successfully.\n");
    } 
	else 
	{
        printf("Invalid role selection.\n");
    }
}

void admin_menu() 
{
    int choice;
    do {
        printf("\n=== ADMIN MENU ===\n");
        printf("1. List Users\n");
        printf("2. Add User\n");
        printf("3. Remove User\n");         
        printf("4. Reset Password\n");      
        printf("5. Modify User Role\n");    
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) 
		{
            case 1: admin_list_users(); break;
            case 2: admin_add_user(); break;
            case 3: admin_remove_user(); break;
            case 4: admin_reset_password(); break;
            case 5: admin_modify_role(); break;
        }
    } while (choice != 6);
}

/* CASHIER MODULE */

void cashier_open_account() 				/* Cashier can open an account for the user */
{
    Account a;
    int customerId, idx;

    if (accountCount >= MAX_ACCOUNTS) 
	{ 
	printf("Memory full.\n"); 
	return; 
	}

    printf("Enter Customer User ID: ");
    scanf("%d", &customerId);

    idx = find_user_index_by_id(customerId);
    if (idx < 0 || users[idx].role != ROLE_CUSTOMER) 
	{
        printf("Invalid customer ID.\n");
        return;
    }

    a.active = 1;
    a.ownerUserId = customerId;
    a.accountNo = next_account_no();
    a.balance = 0.0;

    printf("Type (SAVINGS/CURRENT): ");
    scanf("%s", a.type);
    
    printf("Initial Deposit: ");
    scanf("%lf", &a.balance);
    if (a.balance < 0) a.balance = 0;

    accounts[accountCount] = a;
    accountCount++;
    save_data();
    printf("Account Opened! Number: %d\n", a.accountNo);
}

void cashier_close_account() 					/* Enables cashier to remove a user */
{
    int id, idx;
    printf("\n    REMOVE USER    \n");
    printf("Enter the User ID to be removed: ");
    scanf("%d", &id);

    idx = find_user_index_by_id(id);
    if (idx < 0) 
	{
        printf("User not found.\n");
        return;
    }


    users[idx].active = 0;					/* To deactivate an account */
    save_data();
    printf("User ID %d removed/deactivated successfully.\n", id);
}

void cashier_update_details() 				/* Allows cashier to update details */
{
    int customerId, idx, choice;
    printf("Enter Customer User ID to update: ");
    scanf("%d", &customerId);

    idx = find_user_index_by_id(customerId);
    if (idx < 0) 
	{ 
	printf("User not found.\n"); 
	return; 
	}

    printf("Update: 1. Address, 2. Phone, 3. Email\nChoice: ");
    scanf("%d", &choice);

    printf("Enter new value (no spaces): ");
    if (choice == 1) scanf("%s", users[idx].address);
    else if (choice == 2) scanf("%s", users[idx].phone);
    else if (choice == 3) scanf("%s", users[idx].email);
    else printf("Invalid option.\n");

    save_data();
    printf("Info updated.\n");
}

void cashier_process_transaction(int isDeposit) 			/* Performs either deposit or withdrawal */
{
    int accNo, idx;
    double amount;

    printf("Account Number: ");
    scanf("%d", &accNo);
    
    idx = find_account_index_by_no(accNo);
    if (idx < 0) 
	{ 
	printf("Account not found.\n"); 
	return; 
	}

    printf("Amount: ");
    scanf("%lf", &amount);

    if (amount <= 0) 
	{ 
	printf("Invalid amount.\n"); 
	return;
	 }

    if (isDeposit) 
	{
        accounts[idx].balance += amount;
        printf("Deposit successful.\n");
    } 
	else 
	{
        if (accounts[idx].balance >= amount) 
		{
            accounts[idx].balance -= amount;
            printf("Withdrawal successful.\n");
        } 
		else 
		{
            printf("Insufficient funds.\n");
            return;
        }
    }
    printf("New Balance: %.2f\n", accounts[idx].balance);
    save_data();
}

void cashier_list_users() 						/* Lists customers and their details */
{
    int i, ownerIdx;
    printf("\n    SYSTEM USERS    \n");
    printf("%-5s %-10s %-6s %-15s %-20s %-15s\n", "ID", "Name", "Role", "Phone", "Email", "Address");
    
    for (i = 0; i < userCount; i++) 
	{
        if (users[i].active&&users[i].role == ROLE_CUSTOMER) 
		{
            printf("%-5d %-10s %-6d %-15s %-20s %s\n",
                   users[i].id, 
                   users[i].firstName, 
                   users[i].role, 
                   users[i].phone, 
                   users[i].email, 
                   users[i].address);
        }
    }
}
void cashier_menu() 
{
    int choice;
    do {
        printf("\n=== CASHIER MENU ===\n");
        printf("1. Open Account\n");
        printf("2. Close Account\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. Update Customer Info\n");
        printf("6. View Customer List\n");
        printf("7. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: cashier_open_account(); break;
            case 2: cashier_close_account(); break;
            case 3: cashier_process_transaction(1); break;
            case 4: cashier_process_transaction(0); break;
            case 5: cashier_update_details(); break;
            case 6: cashier_list_users(); break;
        }
    } while (choice != 7);
}

/* CUSTOMER MODULE */

void customer_view_accounts(int userId) 				/* Allows customer to view their accounts */
{
    int i, found = 0;
    printf("\n    YOUR ACCOUNTS    \n");
    for (i = 0; i < accountCount; i++) 
	{
        if (accounts[i].active && accounts[i].ownerUserId == userId) 
		{
            printf("Acc: %d | Type: %s | Bal: %.2f\n", 
                   accounts[i].accountNo, accounts[i].type, accounts[i].balance);
            found = 1;
        }
    }
    if (!found) printf("No active accounts.\n");
}

void customer_transfer(int userId) 					/* Allows customer to transfer money */		
{
    int srcAccNo, destAccNo, srcIdx, destIdx;
    double amount;

    printf("\n    TRANSFER MONEY    \n");
    printf("From Account Number: ");
    scanf("%d", &srcAccNo);

    srcIdx = find_account_index_by_no(srcAccNo);
    

    if (srcIdx < 0 || accounts[srcIdx].ownerUserId != userId)     			/* Check whether or not account exists and belongs to user */
	{
        printf("Invalid source account.\n");
        return;
    }

    printf("To Account Number: ");
    scanf("%d", &destAccNo);
    destIdx = find_account_index_by_no(destAccNo);

    if (destIdx < 0) 
	{
        printf("Destination account not found.\n");
        return;
    }

    printf("Amount you would like to transfer: ");
    scanf("%lf", &amount);

    if (amount <= 0) 
	{ 
	printf("Invalid amount.\n"); 
	return; 
	}
    
    if (accounts[srcIdx].balance >= amount) 
	{
        accounts[srcIdx].balance -= amount;
        accounts[destIdx].balance += amount;
        save_data();
        printf("Transfer Successful!\n");
        printf("New Balance (Source): %.2f\n", accounts[srcIdx].balance);
    } 
	else 
	{
        printf("Insufficient funds.\n");
    }
}

void customer_update_info(int userIdx) 					/* Allows user to update their info */
{
    int choice;
    printf("\n    UPDATE MY INFO    \n");
    printf("1. Change Password\n2. Update Phone\n3. Update Email\nChoice: ");
    scanf("%d", &choice);

    printf("Enter new value (no spaces): ");
    if (choice == 1) 
	{
        scanf("%s", users[userIdx].password);
    } 
	else if (choice == 2) 
	{
        scanf("%s", users[userIdx].phone);
    } 
	else if (choice == 3) 
	{
        scanf("%s", users[userIdx].email);
    } 
	else 
	{
        printf("Invalid option.\n");
        return;
    }
    save_data();
    printf("Information updated successfully.\n");
}

void customer_menu(int userIdx) 
{
    int choice;
    do 
	{
        printf("\n=== CUSTOMER MENU ===\n");
        printf("1. View Accounts\n");
        printf("2. Transfer Money\n");
        printf("3. Update My Info\n");
        printf("4. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: customer_view_accounts(users[userIdx].id); break;
            case 2: customer_transfer(users[userIdx].id); break;
            case 3: customer_update_info(userIdx); break;
        }
    } while (choice != 4);
}



void check_first_run() 					/* Upon first run, admin first account is here */
{
    if (userCount == 0) 
	{
        User admin;
        admin.id = 1;
        admin.role = ROLE_ADMIN;
        strcpy(admin.username, "admin");
        strcpy(admin.password, "admin123");
        strcpy(admin.firstName, "Super");
        strcpy(admin.lastName, "Admin");
        strcpy(admin.govId, "000");
        strcpy(admin.address, "Bank_HQ");
        strcpy(admin.phone, "000-0000");
        strcpy(admin.email, "admin@bank.com");
        admin.active = 1;
        users[0] = admin;
        userCount = 1;
        save_data(); 
    }
}