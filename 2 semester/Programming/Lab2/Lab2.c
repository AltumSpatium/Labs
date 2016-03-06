/* Variant 5 
 * eVEgetables. Commercial enterprise, realizing food,
 * plans to open the Internet-shop selling vegetables.
 * That company books you develop a module in C,
 * which should perform the following functions:
 * Order potatoes
 * Order carrots
 * Order beet
 * Shopping cart
 * Cost
 * About
 * Exit
 */

#include <stdio.h>
#include <stdlib.h>

const int potato_price = 500;
const int carrot_price = 1000;
const int beet_price = 700;
const int shipping_price = 15000;

int getch(void) { /* Считывание символа */
    int ch;
    while ((ch = getchar()) == 10);
    return ch;
}

void clrscr(void) { /* Очистка экрана */
    puts("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void About(void) {
    clrscr();
    puts("Shop: eVegetables Inc.");
    puts("License number: 635/385TUT/BY");
    puts("Contacts: Belarus, Minsk, Yakubova st., 56, tel. +375(17) 123-45-67");
    printf("\nPress any key to continue...");
    getch();
}

int OrderPotatoes(void) {
    int i;
    char buf[256]; /* Буфер введенных символов */

    clrscr();
    while (1) {
        printf("Enter the number of potatoes (kg): ");
        scanf("%s", buf);
        for (i = 0; buf[i] != 0; i++) {
            if (buf[i] < '0' || buf[i] > '9') 
                break; /* Является ли неотрицательным числом */
        }        
        if (buf[i] != 0) { /* Обработка ошибки */
            clrscr();
            puts("Incorrect input, please repeat ...\n");
        }
        else
            break;
    }
    return atoi(buf); /* Перевод строки в число */
}

int OrderCarrots(void) {
    int i;
    char buf[256];

    clrscr();
    while (1) {
        printf("Enter the number of carrots (kg): ");
        scanf("%s", buf);
        for (i = 0; buf[i] != 0; i++) {
            if (buf[i] < '0' || buf[i] > '9') 
                break;
        }        
        if (buf[i] != 0) {
            clrscr();
            puts("Incorrect input, please repeat ...\n");
        }
        else
            break;
    }
    return atoi(buf);
}

int OrderBeet(void) {
    int i;
    char buf[256];
    
    clrscr();
    while (1) {
        printf("Enter the number of beet (kg): ");
        scanf("%s", buf);
        for (i = 0; buf[i] != 0; i++) {
            if (buf[i] < '0' || buf[i] > '9') 
                break;
        }        
        if (buf[i] != 0) {
            clrscr();
            puts("Incorrect input, please repeat ...\n");
        }
        else
            break;
    }
    return atoi(buf);
}

void ShoppingCart(int potato, int carrot, int beet) {
    clrscr();
    printf("You ordered:\r\n");
    printf("Potatoes: %d kg, price - %d r.\r\nCarrots: %d kg, price - %d r.\r\nBeet: %d kg, price - %d r.\r\n\nPress any key to continue...\r\n", potato, potato * potato_price, carrot, carrot * carrot_price, beet, beet * beet_price);
    getch();
}

void Cost(int potato, int carrot, int beet) {
    double full_cost = potato * potato_price + carrot * carrot_price + beet * beet_price + shipping_price;
    char* discount = "0%";

    clrscr();
    if ((potato + carrot + beet) > 50) {
        discount = "30%";
        full_cost = full_cost * 0.7;
    }
    else if ((potato + carrot + beet) > 25) {
        discount = "20%";
        full_cost = full_cost * 0.8;
    }
    else if ((potato + carrot + beet) > 10) {
        discount = "10%";
        full_cost = full_cost * 0.9;
    }
    printf("You ordered:\r\n");
    printf("Potatoes: %d kg, price per kg - %d r.\r\nCarrots: %d kg, price per kg - %d r.\r\nBeet: %d kg, price per kg - %d r.\r\nDelivery costs - %d r.\r\nDiscount - %s \r\nFull cost - %.3f r.\r\n\nPress any key to continue...\r\n", potato, potato_price, carrot, carrot_price, beet, beet_price, shipping_price, discount, full_cost);
    getch();
}

void NewOrder(void) {
    int correct_order = 0;
    int chr = 0;
    int potato = 0;
    int carrot = 0;
    int beet = 0;

    while (1) {
        clrscr();
        if ((potato > 0) || (carrot > 0) || (beet > 0)) {
            correct_order = 1;
            printf("1 - Order potatoes\r\n2 - Order carrot\r\n3 - Order beet\r\n4 - Shopping cart\r\n5 - Cost\r\n0 - Exit\r\n\nEnter the number of the desired menu bar: ");
        }
        else {
            correct_order = 0;
            printf("1 - Order potatoes\r\n2 - Order carrot\r\n3 - Order beet\r\n4 - Shopping cart\r\n0 - Exit\r\n\nEnter the number of the desired menu bar: ");
        }
        chr = getch();
        if (chr == '1')
            potato = OrderPotatoes();
        else if (chr == '2')
            carrot = OrderCarrots();
        else if (chr == '3')
            beet = OrderBeet();
        else if (chr == '4')
            ShoppingCart(potato, carrot, beet);
        else if ((correct_order == 1) && (chr == '5'))
            Cost(potato, carrot, beet);
        else if (chr == '0')
            break;
    }
}

int main() {
    int chr = 0;

    while (1) {
     
        clrscr();
        printf("1 - New order\r\n2 - About shop\r\n0 - Exit\r\n\nEnter the number of the desired menu bar: ");
        chr = getch();
        
        if (chr == '1')
            NewOrder();
        else if (chr == '2')
            About();
        else if (chr == '0') {
            clrscr();
            return 0;
        }
    }
}
