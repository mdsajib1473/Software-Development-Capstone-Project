#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MAX_USERS = 10;
int MAX_ITEMS = 10;
int MAX_NAME = 20;
int MAX_DESCRIPTION = 100;

struct User
{
    char username[20];
    char password[20];
    char email[20];
    int rating;
    int isBlocked;
    int loggedIn;
};

struct Item
{
    char name[20];
    char description[50];
    float price;
    int sellerIndex;
    int isSold;
    int auctionActive;
};

struct User users[10];
struct Item items[10];
int numUsers = 0;
int numItems = 0;
int currentUser = -1;

void registration()
{
    if (numUsers >= 10)
    {
        printf("User database is full!\n");
        return;
    }

    printf("Enter username: ");
    scanf("%s", users[numUsers].username);
    printf("Enter password: ");
    scanf("%s", users[numUsers].password);
    printf("Enter email: ");
    scanf("%s", users[numUsers].email);
    users[numUsers].loggedIn = 0;
    users[numUsers].rating = 0;
    users[numUsers].isBlocked = 0;

    printf("\n----------------------->> Registration successful!  <<-----------------------\n");
    numUsers++;
}

void login()
{
    char username[20];
    char password[20];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < numUsers; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            currentUser = i;
            users[i].loggedIn = 1;
            printf("\n----------------------->> Login successful!  <<-----------------------\n");
            return;
        }
    }

    printf("Invalid username or password!\n");
}

void addItem()
{
    if (numItems >= MAX_ITEMS)
    {
        printf("Item database is full! :<\n");
        return;
    }

    if (currentUser == -1 || currentUser >= numUsers)
    {
        printf("You must be logged in to add an item!!! :)\n");
        return;
    }

    printf("Enter item name: ");
    getchar(); // agge input er new line baad dewar jonno
    fgets(items[numItems].name, sizeof(items[numItems].name), stdin);
    items[numItems].name[strcspn(items[numItems].name, "\n")] = '\0'; // new line character baad dewar jonno

    printf("Enter item description: ");
    fgets(items[numItems].description, sizeof(items[numItems].description), stdin);
    items[numItems].description[strcspn(items[numItems].description, "\n")] = '\0'; // same as before

    printf("Enter item price: ");
    scanf("%f", &items[numItems].price);
    items[numItems].sellerIndex = currentUser;
    items[numItems].isSold = 0;
    items[numItems].auctionActive = 0;

    printf("\n----------------------- Item added successfully! -----------------------\n");
    numItems++;
}

void displayItems()
{
    if (numItems == 0)
    {
        printf("No items available!\n");
        return;
    }

    printf("\nItems available for bidding:\n");
    for (int i = 0; i < numItems; i++)
    {
        printf("Name: %s\n", items[i].name);
        printf("Description: %s\n", items[i].description);
        printf("Price: %.2f$\n", items[i].price);
        printf("Seller: %s\n", users[items[i].sellerIndex].username);
        printf("\n-----------------------\n");
    }
}

void launchAuction()
{
    int itemIndex;
    printf("Enter the index of the item to launch auction: ");
    scanf("%d", &itemIndex);

    if (itemIndex < 0 || itemIndex >= numItems)
    {
        printf("Invalid item index!\n");
    }
    else if (items[itemIndex].sellerIndex != currentUser)
    {
        printf("You can only launch an auction for your own items.\n");
    }
    else if (items[itemIndex].auctionActive)
    {
        printf("Auction for item '%s' is already active.\n", items[itemIndex].name);
    }
    else
    {
        items[itemIndex].auctionActive = 1;
        printf("\nAuction for item '%s' launched successfully!\n", items[itemIndex].name);
    }
}

void trackAuction()
{
    int itemIndex;
    printf("Enter the index of the item to track auction: ");
    scanf("%d", &itemIndex);

    if (itemIndex < 0 || itemIndex >= numItems)
    {
        printf("Invalid item index!\n");
    }
    else if (!items[itemIndex].auctionActive)
    {
        printf("Auction for item '%s' is not active.\n", items[itemIndex].name);
    }
    else
    {
        printf("Tracking auction for item: '%s'\n", items[itemIndex].name); // just current obosta bujanor jonno
        printf("Current highest bid: %.2f$\n", items[itemIndex].price);
        printf("Seller: %s\n", users[items[itemIndex].sellerIndex].username);
    }
}

void updateItem()
{
    int itemIndex;
    printf("Enter the index of the item to update: ");
    scanf("%d", &itemIndex);

    if (itemIndex < 0 || itemIndex >= numItems)
    {
        printf("Invalid item index!\n");
        return;
    }

    if (items[itemIndex].sellerIndex != currentUser)
    {
        printf("You can only update your own items.\n");
        return;
    }

    printf("Enter new name: ");
    getchar(); // agger scanf er newline baad dewar jonno
    char newName[20];
    fgets(newName, sizeof(newName), stdin);
    newName[strcspn(newName, "\n")] = '\0'; // newline character baad dewar jonno
    if (strlen(newName) > 0)
    {
        strcpy(items[itemIndex].name, newName);
    }

    printf("Enter new description: ");
    char newDescription[50];
    fgets(newDescription, sizeof(newDescription), stdin);
    newDescription[strcspn(newDescription, "\n")] = '\0'; // same as before :p
    if (strlen(newDescription) > 0)
    {
        strcpy(items[itemIndex].description, newDescription);
    }

    printf("Enter new price: ");
    float newPrice;
    scanf("%f", &newPrice);
    if (newPrice > 0)
    {
        items[itemIndex].price = newPrice;
    }

    printf("Item '%s' updated successfully!\n", items[itemIndex].name);
}

void auctionManagement()
{
    int choice;
    int itemIndex;
    printf("\t\tAuction Management Menu\n");
    printf("\n\n1. Close Auction\n2. Extend Auction\n3. Cancel Auction\n4. Back to Main Menu\n\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        printf("Enter the index of the item to close auction: ");
        scanf("%d", &itemIndex);

        if (itemIndex < 0 || itemIndex >= numItems)
        {
            printf("Invalid item index!\n");
            return;
        }

        if (items[itemIndex].sellerIndex != currentUser)
        {
            printf("You can only close the auction for your own items.\n");
            return;
        }

        if (!items[itemIndex].auctionActive)
        {
            printf("Auction for item '%s' is not active.\n", items[itemIndex].name);
            return;
        }

        items[itemIndex].auctionActive = 0;
        items[itemIndex].isSold = 1; // item sell hoye giye che mone kore...
        printf("Auction for item '%s' closed successfully!\n", items[itemIndex].name);
        break;

    case 2:
        printf("Enter the index of the item to extend auction: ");
        scanf("%d", &itemIndex);

        if (itemIndex < 0 || itemIndex >= numItems)
        {
            printf("Invalid item index!\n");
            return;
        }

        if (items[itemIndex].sellerIndex != currentUser)
        {
            printf("You can only extend the auction for your own items.\n");
            return;
        }

        if (!items[itemIndex].auctionActive)
        {
            printf("Auction for item '%s' is not active.\n", items[itemIndex].name);
            return;
        }

        printf("Auction for item '%s' extended successfully!\n", items[itemIndex].name); // simply bole dewa j extend kora hoye che
        break;

    case 3:
        printf("Enter the index of the item to cancel auction: ");
        scanf("%d", &itemIndex);

        if (itemIndex < 0 || itemIndex >= numItems)
        {
            printf("Invalid item index!\n");
            return;
        }

        if (items[itemIndex].sellerIndex != currentUser)
        {
            printf("You can only cancel the auction for your own items.\n");
            return;
        }

        if (!items[itemIndex].auctionActive)
        {
            printf("Auction for item '%s' is not active.\n", items[itemIndex].name);
            return;
        }

        items[itemIndex].auctionActive = 0;
        printf("Auction for item '%s' canceled successfully!\n", items[itemIndex].name);
        break;

    case 4:
        return; // main menu te chole jawar jonno

    default:
        printf("Invalid choice!\n");
        break;
    }
}

void logout()
{
    if (currentUser == -1 || currentUser >= numUsers)
    {
        printf("No user is currently logged in.\n");
    }
    else
    {
        printf("\n----------------------->> User '%s' logged out successfully! <<-----------------------\n", users[currentUser].username);
        currentUser = -1;
        users[currentUser].loggedIn = 0;
    }
}

int main()
{
    int choice;

    printf("\n\n\t\t\t\t::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("\t\t\t\t::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("\t\t\t\t::::::::::::::::::::::: :::::::::::::::::::::::\n");
    printf("\t\t\t\t::::::::::::::::::::::: Online Bidding System ::::::::::::::::::::::::::\n");
    printf("\t\t\t\t::::::::::::::::::::::: :::::::::::::::::::::::\n");
    printf("\t\t\t\t::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
    printf("\t\t\t\t::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n\n");

    do
    {
        printf("\n1. Registration\n2. Login\n3. Add Item\n4. Display Items\n5. Launch Auction\n6. Track Auction\n7. Update Item\n8. Manage Auction\n9. Logout\n10. Exit\n");
        printf("Enter your choice: \n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            registration();
            break;
        case 2:
            login();
            break;
        case 3:
            addItem();
            break;
        case 4:
            displayItems();
            break;
        case 5:
            launchAuction();
            break;
        case 6:
            trackAuction();
            break;
        case 7:
            updateItem();
            break;
        case 8:
            auctionManagement();
            break;
        case 9:
            logout();
            break;
        case 10:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 10);

    return 0;
}
