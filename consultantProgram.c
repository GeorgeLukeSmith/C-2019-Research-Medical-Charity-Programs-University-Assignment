/***********************************************************************
 * consultantProgram.c
 * Program for consultant to view and alter files
 * 215730
 * Version 3-10
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 21
#define PASS_LEN 9

struct users
{
    char name[MAX];
    char username[MAX];
    char password[PASS_LEN];
};

struct patient
{
    char firstName[MAX];
    char lastName[MAX];
    char dayOfBirth[3];
    char monthOfBirth[3];
    char yearOfBirth[3];
    char height[4];
    char waist[4];
    char weight[4];
    char comment[50];
};

void startup(struct users array[MAX], int max);
void getName(char name[MAX]);
void getString(char string[MAX]);
void encrypt(char input[MAX]);
int vUser(struct users array[MAX], int max);
int mainMenu(void);
void loadPatient(void);
int importUsers(FILE *login, struct users array[MAX]);
int vrf(char user[MAX], char pass[MAX], struct users array[MAX], int n);
void printMainMenu(void);
int getNum(int min, int max);
int vNum(int low, int high, int num);
int retakeNum(int low, int high);
void printIntro(void);
void decrypt(char input[MAX]);
int updateNurseInfo(int numUsers, struct users array[MAX]);
int addUser(int numUser, struct users array[MAX]);
void printNurseMenu(void);
void changeUser(int numUsers, struct users array[MAX]);
void getFileName(char fileName[MAX]);
void printPatientInfo(struct patient patient);
void decryptAndPrint(struct patient patient, char type[MAX]);
void alterNurseDetails(int i, struct users array[MAX]);
void emptyBuffer(void);
int removeUser(int i, int num, struct users array[MAX]);
int removeNurse(int numUsers, struct users array[MAX]);

int main(void)
{
    int input, userNum = 0, nurseNum = 0;
    struct users arrayUsers[MAX], arrayNurses[MAX];
    FILE *login;
    
    login = fopen("consultantLogin.txt", "r");
    if(!login)
    {
        printf("\nunable to open file");
        exit(1);
    }
    userNum = importUsers(login, arrayUsers) -1;
    startup(arrayUsers, userNum);
    while(1)
    {    
        input = mainMenu();
    
        switch(input)
        {
            case 1:
                loadPatient();
                break;
            
            case 2:
                nurseNum = updateNurseInfo(nurseNum, arrayNurses);
                break;
                        
            case 3:
                return 0;
        }
    
    }
}
    
/*startup and login procedures*/
void startup(struct users array[MAX], int max)
{
    printIntro();
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
            printf("\nUsername or Password Incorrect\n\n");
        }
    }
}

/*returns the menu choice of the user*/
int mainMenu()
{
    int input;
    printMainMenu();
    input = getNum(0, 4);
    return input;
}

/*allows the user to add a patient which is subsequently saved to
a file*/
void loadPatient()
{
    char firstName[MAX], fileName[MAX], string[MAX];
    char dateOfBirth[8], comment[50];
    int height, waist, weight, input;
    struct patient patient;
    FILE *data;
    
    getFileName(fileName);
    data = fopen(fileName, "a+");
    while(!feof(data))
    {
        fscanf(data, "%11[^:]:%11[^:]:%11[^:]:%11[^:]:%11[^:]:%11[^:]:"
                "%11[^:]:%11[^:]:%50[^:]:", patient.firstName,
                patient.lastName, patient.dayOfBirth,
                patient.monthOfBirth, patient.yearOfBirth, 
                patient.height, patient.waist, patient.weight,
                patient.comment);
    }
    
    printPatientInfo(patient);
    
    
    input = getNum(0, 3);
    switch(input)
    {
        case 1:
            printf("\nPlease enter additional comments:\t");
            scanf("%50[^\n]", comment);
            emptyBuffer();
            encrypt(comment);
            fprintf(data, "%s:", comment);
        
        case 2:
            break;
    }
    fclose(data);
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
    printf("\n1.\tView patient details\n");
    printf("2.\tUpdate Nurse Logins\n");
    printf("3.\tExit Program\n\n");
    
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
    printf("Action on Weight Consultant system\n");
    printf("log in to continue\n");
}

/*decrypt strings*/
void decrypt(char input[MAX])
{
    int i = 0;
    char ch;
    
    for(i = 0; input[i] != '\0'; i++) 
    {
        ch = input[i];
        if(ch >= 'i' && ch <= 'z')
        {
            ch = ch - 8;
        }
        else if(ch >= 'a' && ch <= 'h')
        {
            ch = ch + 18;
        }
        
        else if(ch >= 'I' && ch <= 'Z')
        {
            ch = ch - 8;
        }
        else if(ch >= 'A' && ch <= 'H')
        {
            ch = ch + 18;
        }
        else if(ch >= '8' && ch <= '9')
        {
            ch = ch - 8;
        }
        else if(ch >= '0' && ch <= '7')
        {
            ch = ch + 2;
        }
        
        input[i] = ch;
    }
}

/*update nurse menu option*/
int updateNurseInfo(int numUsers, struct users array[MAX])
{
    FILE *nurses;
    int input, i;
    
    system("cls");
    printf("\n");
    nurses = fopen("users.txt", "r");
    if(!nurses)
    {
        printf("\nunable to open file");
        exit(1);
    }
    numUsers = importUsers(nurses, array) -1;
    fclose(nurses);
    printNurseMenu();
    input = getNum(0, 5);
    
    switch(input)
    {
        case 1:
            numUsers = addUser(numUsers, array);
            return numUsers;
            
        case 2:
            changeUser(numUsers, array);
            break;
        
		case 3: 
			removeNurse(numUsers, array);
			break;
		
		case 4:
			break;
    }
    return numUsers;
}

/*add a user to the user file*/
addUser(int numUsers, struct users array[MAX])
{
    FILE *nurses;
    
    system("cls");
    numUsers++;
    getchar();
    printf("Please Enter New Nurse Details:\n");
    printf("\nNurse Name:\t");
    scanf("%50[^\n]", array[numUsers].name);
    emptyBuffer();
    printf("Nurse Username:\t");
    scanf("%50[^\n]", array[numUsers].username);
    emptyBuffer();
    printf("Nurse Password:\t");
    scanf("%9[^\n]", array[numUsers].password);
    emptyBuffer();
    nurses = fopen("users.txt", "a");
    if(!nurses)
    {
        printf("\nunable to open file");
        exit(1);
    }
    encrypt(array[numUsers].name);
    encrypt(array[numUsers].username);
    encrypt(array[numUsers].password);
    fprintf(nurses, "%s:%s:%s\n", array[numUsers].name,
            array[numUsers].username, array[numUsers].password);
    fclose(nurses);
    return numUsers;
}

/*alter the details of a user*/
void changeUser(int numUsers, struct users array[MAX])
{
    int i;
    FILE *nurses;
    
    system("cls");
    for(i = 0; i < numUsers; i++)
    {
        decrypt(array[i].name);
        printf("%.2d|\t%s\n", i + 1, array[i].name);
    }
    printf("\nSelect the number of the nurse you wish to alter\n");
    printf("option: ");
    i = getNum(0, numUsers+1) - 1;
    alterNurseDetails(i, array);
    
    nurses = fopen("users.txt", "w");
    if(!nurses)
    {
        printf("\nunable to open file");
        exit(1);
    }
    for(i = 0; i < numUsers; i++)
    {
        encrypt(array[i].name);
        encrypt(array[i].username);
        encrypt(array[i].password);
        fprintf(nurses, "%s:%s:%s\n", array[i].name, array[i].username, 
                array[i].password);
    }
    fclose(nurses);
}

/*print the nurse menu to the console*/
void printNurseMenu(void)
{
    printf("Please Select an Option\n");
    printf("\n1.\tAdd Nurse\n2.\tChange Nurse Details");
    printf("\n3.\tDelete Nurse");
    printf("\n4.\tReturn to Main Menu\n\nOption: ");
}

/*gets the name of the patient file to open from user input*/
void getFileName(char fileName[MAX])
{
    int day, month, year;
    char lastName[MAX];
    
    system("cls"); 
	getchar();   
    printf("patient last name:\t");
    scanf("%15[^\n]", lastName);
    emptyBuffer();
    printf("\npatient Date of Birth\n");
    printf("day:\t");
    day = getNum(0, 32);
    printf("month:\t");
    month = getNum(0, 13);
    printf("year:\t");
    year = getNum(1800, 2020);
    sprintf(fileName, "%s%d%.2d%d.aow", strlwr(lastName), day, month, 
            year % 100);
}

/*prints patient info to the console*/
void printPatientInfo(struct patient patient)
{
    decrypt(patient.firstName);
    printf("\nFirst Name:\t\t%s", patient.firstName);
    decrypt(patient.lastName);
    printf("\nLast Name:\t\t%s", patient.lastName);
    decrypt(patient.dayOfBirth);
    decrypt(patient.monthOfBirth);
    decrypt(patient.yearOfBirth);
    printf("\nDate of Birth:\t\t%s/%s/%s", patient.dayOfBirth,
            patient.monthOfBirth, patient.yearOfBirth);
    decrypt(patient.height);
    printf("\nheight:\t\t\t%s cm", patient.height);
    decrypt(patient.waist);
    printf("\nWaist Measurement:\t%s cm", patient.waist);
    decrypt(patient.weight);
    printf("\nWeight:\t\t\t%s kg", patient.weight);
    decrypt(patient.comment);
    printf("\nComment:\t\t%s\n", patient.comment);
    printf("\nAdd Additional Comment?\n\n1.\tyes\n2.\tno\n\nOption: ");
}

/*change the name, username and password of a nurse*/
void alterNurseDetails(int i, struct users array[MAX])
{
    getchar();
	printf("\nNew Nurse Name:\t\t");
    scanf("%50[^\n]", array[i].name);
    emptyBuffer();
    printf("\nNew Nurse Username:\t");
    scanf("%50[^\n]", array[i].username);
    emptyBuffer();
    printf("\nNew Nurse Password:\t");
    scanf("%8[^\n]", array[i].password);
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

/*remove a nurse from the system*/
int removeNurse(int numUsers, struct users array[MAX])
{
	
    FILE *nurses;
    int i;
    system("cls");
    for(i = 0; i < numUsers; i++)
    {
        decrypt(array[i].name);
        printf("%.2d|\t%s\n", i + 1, array[i].name);
    }
    printf("\nSelect the number of the nurse you wish to Remove\n");
    printf("option: ");
    i = getNum(0, numUsers+1) - 1;
    numUsers = removeUser(i, numUsers, array);
    
    nurses = fopen("users.txt", "w");
    if(!nurses)
    {
        printf("\nunable to open file");
        exit(1);
    }
    for(i = 0; i < numUsers; i++)
    {
        encrypt(array[i].name);
        encrypt(array[i].username);
        encrypt(array[i].password);
        fprintf(nurses, "%s:%s:%s\n", array[i].name, array[i].username, 
                array[i].password);
    }
    fclose(nurses);
    return numUsers;
}	

/*removes selected user and returns new number of users*/
int removeUser(int i, int num, struct users array[MAX])
{
	for(i; i < num - 1; i++)
	{
		array[i] = array[i + 1];
	}
	num = num - 1;
	return num;
}
