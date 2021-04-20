/***********************************************************************
 * nurseProgram.c
 * Program for Nurses to enter patient data
 * 215730
 * version Level 3
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 21

struct users
{
    char name[MAX];
    char username[MAX];
    char password[MAX];
};

void startup(struct users array[MAX], int max);
void getString(char string[MAX]);
void encrypt(char input[MAX]);
int vUser(struct users array[MAX], int max);
int mainMenu();
void addPatient();
int importUsers(FILE *login, struct users array[MAX]);
int vrf(char user[MAX], char pass[MAX], struct users array[MAX], int n);
void printMainMenu(void);
int getNum(int min, int max);
int vNum(int low, int high, int num);
int retakeNum(int low, int high);
void printIntro(void);
void getName(char which[10], char type[MAX]);
void emptyBuffer(void);

int main(void)
{
    int input, nurseNum = 0;
    struct users arrayNurses[MAX];
    FILE *login;
    
    printIntro();
    login = fopen("users.txt", "r");
    if(!login)
    {
        printf("\nunable to open file");
        exit(1);
    }
    nurseNum = importUsers(login, arrayNurses);
    startup(arrayNurses, nurseNum);
    while(1)
    {    
        input = mainMenu();
    
        switch(input)
        {
            case 1:
                addPatient();
                break;
            
            case 2:
                return 0;
        }
    }
}

/*startup and login procedures*/
void startup(struct users array[MAX], int max)
{
    vUser(array, max);
}

/*shift encrypts a string by 8*/ 
void encrypt(char input[MAX])
{
    int i = 0;
    char ch;
    
    for(i = 0; input[i] != '\0'; i++) 
    {
        ch = input[i];
        if(ch >= 'a' && ch <= 'r')
        {
            ch = ch + 8;
        }
        else if(ch >= 's' && ch <= 'z')
        {
            ch = ch - 18;
        }
        
        else if(ch >= 'A' && ch <= 'R')
        {
            ch = ch + 8;
        }
        else if(ch >= 'S' && ch <= 'Z')
        {
            ch = ch - 18;
        }
        else if(ch >= '0' && ch <= '1')
        {
            ch = ch + 8;
        }
        else if(ch >= '2' && ch <= '9')
        {
            ch = ch - 2;
        }
        
        input[i] = ch;
    }
}

/*takes a username and password from the user and returns true if
they are correct*/
int vUser(struct users array[MAX], int max)
{
    while(1)
    {
        char user[MAX];
        char pass[MAX];
        printf("\nusername: ");
        scanf("%15[^\n]", user);
        emptyBuffer();
        encrypt(user);
        printf("Password: ");
        scanf("%15[^\n]", pass);
        emptyBuffer();
        encrypt(pass);
        if(vrf(user, pass, array, max) == 1)
        {
            return 1;
        }
        else
        {
            printf("Username or Password Incorrect\n\n");
        }
    }
}

/*returns the menu choice of the user*/
int mainMenu()
{
    int input;
    printMainMenu();
    input = getNum(0, 3);
    return input;
}

/*allows the user to add a patient which is subsequently saved to
a file*/
void addPatient()
{
    char firstName[MAX], lastName[MAX], fileName[MAX], string[MAX];
    char dateOfBirth[8], comment[50];
    int day, month, year, height, waist, weight;
    FILE *patientData;
    
    system("cls");    
    getchar();
    getName("first name", firstName);
    getName("last name", lastName);
    printf("\npatient Date of Birth\n");
    printf("day:\t");
    day = getNum(0, 32);
    printf("month:\t");
    month = getNum(0, 13);
    printf("year:\t");
    year = getNum(1800, 2020);
    sprintf(fileName, "%s%d%d%d.txt", lastName, day, month, year);
            
    patientData = fopen(fileName, "w");
    
    sprintf(dateOfBirth, "%d:%d:%d", day, month, year);
    encrypt(firstName);
    encrypt(lastName);
    encrypt(dateOfBirth);
    fprintf(patientData, "%s:%s:", firstName, lastName);
    fprintf(patientData, "%s:", dateOfBirth);
    printf("\nPlease enter height in cm:\t\t");
    height = getNum(0, 250);
    sprintf(string, "%d", height);
    encrypt(string);
    fprintf(patientData, "%s:", string);
    printf("\nPlease enter waist measurement in cm:\t");
    waist = getNum(0, 250);
    sprintf(string, "%d", waist);
    encrypt(string);
    fprintf(patientData, "%s:", string);
    printf("\nPlease enter weight in kg:\t\t");
    weight = getNum(0, 300);
    sprintf(string, "%d", weight);
    encrypt(string);
    fprintf(patientData, "%s:", string);
    emptyBuffer();
    printf("\nPlease enter additional comments:\t");
    scanf("%50[^\n]", comment);
    emptyBuffer();
    encrypt(comment);
    fprintf(patientData, "%s:", comment);
    fclose(patientData);
}

/*imports user log in details from a file*/
int importUsers(FILE *login, struct users array[MAX])
{
    
    int i = 0;
    
    while(!feof(login))
    {
        fscanf(login, "%22[^:]%*c%11[^:]%*c%11[^\n]%*c", 
                array[i].name, array[i].username, array[i].password);
        i++;
    }
    return i;
}

/*checks if username and password are correct*/
int vrf(char user[MAX], char pass[MAX], struct users array[MAX], int n)
{
    int i;
    for(i = 0; i < n; i++)
    {
        if(strcmp(user, array[i].username)==0)
        {
            if(strcmp(pass, array[i].password) == 0)
            {
                return 1;
            }
        }
    }
}

/*prints main menu text to the console*/
void printMainMenu(void)
{
    system("cls");
    printf("\nPlease type an option and press ENTER\n");
    printf("\n1.\tEnter patient details\n");
    printf("2.\tExit Program\n\n");
    printf("option:\t ");
}

/*gets a number from the user and verifies that it is in
the required range, asking the user to reinput if not*/
int getNum(int min, int max)
{
    int num;
    
    while(scanf("%d", &num) !=1)
    {
        num = retakeNum(min, max);
        emptyBuffer();
    }
    while(vNum(min, num, max) == 0)
    {
        num = retakeNum(min, max);
        emptyBuffer();
    }
    return num;

}

/*verifies that an integer is within the correct range*/
int vNum(int low, int high, int num)
{
    if(low <= num&&num <= high)
    {
        return 0;
    }    
    else
    {
        return 1;
    }
}

/*retakes the number from the user after it fails initial verification*/
int retakeNum(int low, int high)
{
    int option;
    
    printf("\nERROR, Please only enter ");
    printf("NUMBERS between %d and %d", low, high);
    printf("\n\nInput: ");
    scanf("%d", &option);
    return option;
}

/*prints intro text to the console*/
void printIntro(void)
{
    printf("Action on Weight nurse system\n");
    printf("log in to continue\n");
}

void getName(char which[10], char type[MAX])
{
    printf("patient %s:\t", which);
    scanf("%15[^\n]", type);
    emptyBuffer();
}

/* Empty the keyboard buffer */
void emptyBuffer(void)
{ 
    while(getchar()!= '\n')
	{ 
	    ; 
	} 
}


