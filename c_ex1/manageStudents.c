/**
 * @file manageStudents.c
 * @author  Brahan Wassan <brahan>
 * @version 1.0
 * @date 13 Nov 2019
 *
 * @brief System to keep track of all the Students who enrolled in some UNI
 *
 * @section DESCRIPTION
 * The system help the UNI to manage the Students.
 * Input  : students enrolled in the UNI.
 * Process: checks if the user input is valid, and then print the Best student,
 * or an sorted array of all the students according to the user OP input
 * Output : best students, or an array of all the students sorted by name or grade.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define USAGE_ERR "Usage: number of argument must match specified format: <manageStudents> <operation>"
#define START_MSG "Enter student info. To exit press q, then enter"
#define ID_ERR_MSG "ERROR: id must be a 10 digits number that does not start with 0\n"
#define NAME_ERR_MSG "ERROR: name can only contain alphabetic characters, whitespaces or '-'\n"
#define GRADE_ERR_MSG "ERROR: grade must be an integer between 0 and 100\n"
#define AGE_ERR_MSG "ERROR: age must be an integer between 18 and 120\n"
#define COUNTRY_ERR_MSG "ERROR: country can only contain alphabetic characters or '-'\n"
#define CITY_ERR_MSG "ERROR: city can only contain alphabetic characters or '-'\n"
#define GENERAL_ERR_MSG "ERROR: info must match specified format\n"
#define BEST_STUDENT_PRINT_FORMAT "%s%s\t%s\t%d\t%u\t%s\t%s\t\n"
#define STUDENT_PRINT_FORMAT "%s\t%s\t%d\t%u\t%s\t%s\t\n"
#define STUDENT_INPUT_FORMAT "%s %[^\t] %[^\t] %[^\t] %[^\t] %[^\t]"
#define ERR_PRINT_FORMAT "%s%s %d\n"
#define IN_LINE "in line"
#define BEST_STUDENT "best student info is: "
#define  MAX_ARGUMENT 41
#define MAX_ARR_LENGTH 151
#define MAX_INPUT 5001
#define VALID_ID_LEN 10
#define OP_IDX 1
#define BEST "best"
#define QUICK "quick"
#define MERGE "merge"
#define MAX_AGE 120
#define MIN_AGE 18
#define MAX_GRADE 100
#define MIN_GRADE 0
#define LINUX_EXIT "q\n"
#define WIN_EXIT "q\r\n"
#define LOW_NUM_VAL_ASCI 48
#define HI_NUM_VAL_ASCI 57
#define VAL_NUM_FIELD 6
#define DECIMAL_FACTOR 10
#define NO_STUDENTS 0
#define MIN_PROG_ARGS 2
#define NO_INPUT_Q 1
#define FIN_INPUT 2
#define INT_TO_CHAR '0'
#define UPPER_A_ASCII 65
#define LOWER_A_ASCII 97
#define UPPER_Z_ASCII 90
#define LOWER_Z_ASCII 122
#define DASH_ASCII 45
#define SPACE_ASCII 32
#define ID_FORMAT_ERR 2
#define SINGLE_MSG_FORMAT "%s\n"
#define FAIL_GRADE 0
#define MERGE_SORT_DIV_FACTOR 2

/**
 * defines one students , a student is entity that defined by id, name,age,grade,country,city
 */
typedef struct
{
    char name[MAX_ARGUMENT], city[MAX_ARGUMENT],
            country[MAX_ARGUMENT], id[MAX_ARGUMENT];
    int age, grade;
    float studentVal;

} Student;
/**
 * an object that represent the student who achieve the greatset grade in the youngest age
 */
Student gBestStudent;
/**
 * all the valid inputs that the user has entered into the program
 */
int gStudentNumber = 0;
/**
 * an array that holds all the valid students the user has entered to the program
 */
Student gStudentArr[MAX_INPUT];

char getInput();

int checkValidity(char line[], int lineNumber);

void updateBestStudent(char const line[MAX_ARR_LENGTH]);

void addNewStudent(char const line[MAX_ARR_LENGTH]);

void mergeSort(Student arr[], int leftIdx, int rightIdx);

void quickSort(Student arr[], int leftIdx, int rightIdx);

void printStudentArr();

/**
 * main program, manage the manageStudents program
 * @param argc the number of argument the program got
 * @param argv the argument the program got
 * @return 0 if the program finished successfully, 1 otherwise
 */
int main(int argc, char *argv[])
{
    if (argc != MIN_PROG_ARGS)
    {
        printf(SINGLE_MSG_FORMAT, USAGE_ERR);
    }
    else
    {
        int inputSituation = getInput(gStudentArr);

        if (gStudentNumber == NO_STUDENTS)
        {
            return EXIT_FAILURE;
        }
        if (inputSituation == NO_INPUT_Q)
        {
            return EXIT_FAILURE;
        }
        if (strcmp(argv[OP_IDX], BEST) == false)
        {
            if (inputSituation == FIN_INPUT)
            {
                printf(BEST_STUDENT_PRINT_FORMAT, BEST_STUDENT, gBestStudent.id,
                       gBestStudent.name, gBestStudent.grade,
                       gBestStudent.age, gBestStudent.country,
                       gBestStudent.city);
            }
        }
        else
        {
            if (strcmp(argv[OP_IDX], QUICK) == false)
            {
                quickSort(gStudentArr, 0, gStudentNumber - 1);
            }
            if (strcmp(argv[OP_IDX], MERGE) == false)
            {
                mergeSort(gStudentArr, 0, gStudentNumber - 1);
            }
            printStudentArr();
        }
    }
    return EXIT_SUCCESS;
}

/**
 * prints all the students in the studentArray
 */
void printStudentArr()
{
    for (int i = 0; i < gStudentNumber; ++i)
    {
        printf(STUDENT_PRINT_FORMAT, gStudentArr[i].id,
               gStudentArr[i].name, gStudentArr[i].grade,
               gStudentArr[i].age, gStudentArr[i].country,
               gStudentArr[i].city);
    }
}

/**
 * the function gets input from the user according to predefined format
 * @return 0 if the program got input, 1 otherwise
 */
char getInput()
{
    gBestStudent.studentVal = 0;
    int lineCounter = 0;
    char line[MAX_ARR_LENGTH];
    bool isEqual = false;
    while (lineCounter < MAX_INPUT)
    {
        printf(SINGLE_MSG_FORMAT, START_MSG);
        fgets(line, MAX_ARR_LENGTH, stdin);
        if ((strcmp(line, WIN_EXIT) != isEqual) && (strcmp(line, LINUX_EXIT) != isEqual))
        {
            int isValid = checkValidity(line, lineCounter);
            if (isValid == isEqual)
            {
                updateBestStudent(line);
                addNewStudent(line);
            }
            ++lineCounter;
            continue;
        }
        if ((strcmp(line, WIN_EXIT) == isEqual) || (strcmp(line, LINUX_EXIT) == isEqual))
        {
            if (lineCounter == NO_STUDENTS)
            {
                return NO_INPUT_Q;
            }
            return FIN_INPUT;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * evaluate the students by grade/age
 * @param grade the student grade
 * @param age the student age
 * @return the student score
 */
float evaluateStudent(int grade, int age)
{
    if (grade == FAIL_GRADE)
    {
        return FAIL_GRADE;
    }
    return (float) grade / (float) age;
}

/**
 * chec if the id is valid
 * @param id the input id
 * @return 1 if the were problem with the id values, 2 if there were problem with the id format,
 * 0 if there were no problems
 */
int checkId(char const id[])
{
    if (id[0] == INT_TO_CHAR)
    {
        return EXIT_FAILURE;
    }
    unsigned int size = strlen(id);
    if (size != VALID_ID_LEN)
    {
        return EXIT_FAILURE;
    }
    else
    {
        unsigned int i;
        for (i = 0; i < size; ++i)
        {
            if ((id[i] < LOW_NUM_VAL_ASCI || id[i] > HI_NUM_VAL_ASCI))
            {
                return ID_FORMAT_ERR;
            }
        }
        return EXIT_SUCCESS;
    }
}

bool isUpperCase(char const letter)
{
    if (!((letter >= UPPER_A_ASCII && letter <= UPPER_Z_ASCII)) && letter != DASH_ASCII)
    {
        return true;
    }
    return false;
}

bool isLowerCase(char const letter)
{
    if (!(((letter >= LOWER_A_ASCII) && (letter <= LOWER_Z_ASCII)) && letter != DASH_ASCII))
    {
        return true;
    }
    return false;
}

/**
 * check if the given strings (name,country,city) are valid
 * @param string the string we want to check
 * @param isName flag that indicate if the input is the name
 * @return 1 if there is problem with the string,0 otherwise
 */
int checkStrings(char const string[], int isName)
{
    int size = (int) strlen(string);
    for (int i = 0; i < size; ++i)
    {
        if (isName)
        {
            bool isTwoNames = (string[i] != SPACE_ASCII);
            if (isLowerCase(string[i]) && isTwoNames && isUpperCase(string[i]))
            {
                return EXIT_FAILURE;
            }
        }
        else
        {
            if (isLowerCase(string[i]) && isUpperCase(string[i]))
            {
                return EXIT_FAILURE;
            }
        }
    }
    return EXIT_SUCCESS;
}

/**
 * checks if the age is in the given limit 18-120
 * @param age the input age
 * @return 1 if there is problem,0 otherwise
 */
int checkAge(int age)
{
    if (age > MAX_AGE || age < MIN_AGE)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 *  check if the supposed to be integer fields contain only digits
 * @param number the given input
 * @return 1 if there is problem,0 otherwise
 */
int checkDigits(const char number[MAX_ARGUMENT])
{
    int numi, i;
    int size = (int) strlen(number);
    for (i = 0; i < size; ++i)
    {
        numi = (int) (number[i]);
        if (numi < LOW_NUM_VAL_ASCI || numi > HI_NUM_VAL_ASCI)
        {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * converts char digit array to integer
 * @param number the given input
 * @return the integer
 */
int convertCharToInt(char const number[MAX_ARGUMENT])
{
    int i, integer = 0;
    int size = (int) strlen(number);
    for (i = 0; i < size; ++i)
    {
        integer = integer * DECIMAL_FACTOR + (number[i] - INT_TO_CHAR);
    }
    return integer;
}

/**
 * checks if the grade is within the limit 0-100
 * @param grade the given grade
 * @return 1 if there is a problem, 0 otherwise
 */
int checkGrade(int grade)
{
    if (grade > MAX_GRADE || grade < MIN_GRADE)
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

/**
 * add a new student to the student array
 * @param line valid input line
 */
void addNewStudent(char const line[MAX_ARR_LENGTH])
{
    {
        char name[MAX_ARGUMENT], city[MAX_ARGUMENT], country[MAX_ARGUMENT],
                id[MAX_ARGUMENT], age[MAX_ARGUMENT], grade[MAX_ARGUMENT];
        sscanf(line, STUDENT_INPUT_FORMAT, id, name, grade, age, country, city);
        int valGrade = convertCharToInt(grade);
        int valAge = convertCharToInt(age);
        float stuVal = evaluateStudent(valGrade, valAge);

        Student newStudent;
        newStudent.studentVal = stuVal;
        strcpy(newStudent.name, name);
        strcpy(newStudent.id, id);
        strcpy(newStudent.country, country);
        strcpy(newStudent.city, city);
        newStudent.grade = valGrade;
        newStudent.age = valAge;
        gStudentArr[gStudentNumber] = newStudent;
        ++gStudentNumber;
    }
}

/**
 * update the best student global variable
 * @param line valid student input
 */
void updateBestStudent(char const line[MAX_ARR_LENGTH])
{
    char name[MAX_ARGUMENT], city[MAX_ARGUMENT], country[MAX_ARGUMENT],
            id[MAX_ARGUMENT], age[MAX_ARGUMENT], grade[MAX_ARGUMENT];
    sscanf(line, STUDENT_INPUT_FORMAT, id, name, grade, age, country, city);
    int valGrade = convertCharToInt(grade);
    int valAge = convertCharToInt(age);
    float stuVal = evaluateStudent(valGrade, valAge);
    if (stuVal > gBestStudent.studentVal || (stuVal == 0 && gBestStudent.studentVal == 0 && gStudentNumber == 0))
    {
        gBestStudent.studentVal = stuVal;
        strcpy(gBestStudent.name, name);
        strcpy(gBestStudent.id, id);
        strcpy(gBestStudent.country, country);
        strcpy(gBestStudent.city, city);
        gBestStudent.grade = valGrade;
        gBestStudent.age = valAge;
    }
}

/**
 * checks the validity of a single input line
 * @param line the user input
 * @param lineNumber the input line number, will be used to print the error line
 * @return 1 if there is a problem with the user input,0 otherwise
 */
int checkValidity(char line[MAX_ARR_LENGTH], int lineNumber)
{
    char name[MAX_ARGUMENT], city[MAX_ARGUMENT], country[MAX_ARGUMENT],
            id[MAX_ARGUMENT], age[MAX_ARGUMENT], grade[MAX_ARGUMENT];

    int argsNum = sscanf(line, STUDENT_INPUT_FORMAT,
                         id, name, grade, age, country, city);
    bool isEqual = false;
    if (argsNum < VAL_NUM_FIELD)
    {
        printf(ERR_PRINT_FORMAT, GENERAL_ERR_MSG, IN_LINE, lineNumber);
        return EXIT_FAILURE;
    }
    int idSituation = checkId(id);
    if (idSituation == true)
    {
        printf(ERR_PRINT_FORMAT, ID_ERR_MSG, IN_LINE, lineNumber);
        return EXIT_FAILURE;
    }
    if (idSituation == ID_FORMAT_ERR)
    {
        printf(ERR_PRINT_FORMAT, GENERAL_ERR_MSG, IN_LINE, lineNumber);
        return EXIT_FAILURE;
    }
    else if (checkStrings(name, true) != isEqual)
    {
        printf(ERR_PRINT_FORMAT, NAME_ERR_MSG, IN_LINE, lineNumber);
        return EXIT_FAILURE;
    }
    else if ((checkDigits(grade) != isEqual) || checkGrade(convertCharToInt(grade)) != isEqual)
    {
        printf(ERR_PRINT_FORMAT, GRADE_ERR_MSG, IN_LINE, lineNumber);
        return EXIT_FAILURE;
    }
    else if ((checkDigits(age) != isEqual) || (checkAge(convertCharToInt(age)) != isEqual))
    {
        printf(ERR_PRINT_FORMAT, AGE_ERR_MSG, IN_LINE, lineNumber);
        return EXIT_FAILURE;
    }
    else if (checkStrings(city, false) != isEqual)
    {
        printf(ERR_PRINT_FORMAT, CITY_ERR_MSG, IN_LINE, lineNumber);
        return EXIT_FAILURE;
    }
    else if (checkStrings(country, false) != isEqual)
    {
        printf(ERR_PRINT_FORMAT, COUNTRY_ERR_MSG, IN_LINE, lineNumber);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//////////////////////////////////////// SORT///////////////////////////////////////////////
/**
 * merges the subarrays
 * @param arr the array we want to sort
 * @param leftIdx the leftmost index of the block
 * @param divPoint the array divide
 * @param rightIdx
 */
void merge(Student arr[], int leftIdx, int divPoint, int rightIdx)
{
    int rightSubArrLen = rightIdx - divPoint;
    int leftSubArrLen = divPoint - leftIdx + 1;
    int idx1, idx2, idx3;
    Student left[MAX_INPUT], right[MAX_INPUT];
    for (idx2 = 0; idx2 < rightSubArrLen; idx2++)
    {
        right[idx2] = arr[divPoint + 1 + idx2];
    }
    for (idx1 = 0; idx1 < leftSubArrLen; idx1++)
    {
        left[idx1] = arr[leftIdx + idx1];
    }
    idx1 = 0, idx2 = 0, idx3 = leftIdx;
    while (idx1 < leftSubArrLen && idx2 < rightSubArrLen)
    {
        if (left[idx1].grade <= right[idx2].grade)
        {
            arr[idx3] = left[idx1];
            ++idx1;
        }
        else
        {
            arr[idx3] = right[idx2];
            ++idx2;
        }
        ++idx3;
    }

    while (idx1 < leftSubArrLen)
    {
        arr[idx3] = left[idx1];
        ++idx1;
        ++idx3;
    }

    while (idx2 < rightSubArrLen)
    {
        arr[idx3] = right[idx2];
        ++idx2;
        ++idx3;
    }
}

/**
 * merges the two subarrays
 * @param arr the array we want to sort
 * @param leftIdx the leftmost index
 * @param rightIdx the rightmost index
 */
void mergeSort(Student arr[], int leftIdx, int rightIdx)
{
    if (leftIdx < rightIdx)
    {
        int divPoint = leftIdx + (rightIdx - leftIdx) / MERGE_SORT_DIV_FACTOR;
        mergeSort(arr, divPoint + 1, rightIdx);
        mergeSort(arr, leftIdx, divPoint);
        merge(arr, leftIdx, divPoint, rightIdx);
    }
}

/**
 * the swap function we saw in the lecture
 * @param arg1 the first student
 * @param arg2 the second student
 */
void swap(Student *arg1, Student *arg2)
{
    Student temp;
    temp.studentVal = arg1->studentVal;
    strcpy(temp.name, arg1->name);
    strcpy(temp.id, arg1->id);
    strcpy(temp.country, arg1->country);
    strcpy(temp.city, arg1->city);
    temp.grade = arg1->grade;
    temp.age = arg1->age;

    arg1->studentVal = arg2->studentVal;
    strcpy(arg1->name, arg2->name);
    strcpy(arg1->id, arg2->id);
    strcpy(arg1->country, arg2->country);
    strcpy(arg1->city, arg2->city);
    arg1->grade = arg2->grade;
    arg1->age = arg2->age;

    arg2->studentVal = temp.studentVal;
    strcpy(arg2->name, temp.name);
    strcpy(arg2->id, temp.id);
    strcpy(arg2->country, temp.country);
    strcpy(arg2->city, temp.city);
    arg2->grade = temp.grade;
    arg2->age = temp.age;
}

/**
 * helper function for the quickSort function, sort the partition blocs in the array
 * @param arr the array we want to sort
 * @param low the leftmost index in the array - defines the "block" left boarder
 * @param high the rightmost index in the array - defines the "block" right boarder
 * @return sorted block
 */
int partition(Student arr[], int low, int high)
{
    int idx2, idx1 = low - 1;
    char pivot[MAX_ARGUMENT];
    strcpy(pivot, arr[high].name);
    for (idx2 = low; idx2 <= high - 1; ++idx2)
    {
        int isPrior = strcmp(arr[idx2].name, pivot);
        if (isPrior <= 0)
        {
            ++idx1;
            swap(&arr[idx1], &arr[idx2]);
        }
    }
    swap(&arr[idx1 + 1], &arr[high]);
    return (idx1 + 1);
}

/**
 *
 * @param arr the array we want to sort, will be sorted by name
 * @param leftIdx the left boarder of the given array, used to calculate the divide point for the partition
 * @param rightIdx the right boarder of the given array, used to calculate the divide point for the partition
 */
void quickSort(Student arr[], int leftIdx, int rightIdx)
{
    if (leftIdx < rightIdx)
    {
        int divPoint = partition(arr, leftIdx, rightIdx);
        quickSort(arr, leftIdx, divPoint - 1);
        quickSort(arr, divPoint + 1, rightIdx);
    }
}
