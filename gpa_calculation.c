/* Johnathan Harrington*/

#include <stdlib.h>
#include <stdio.h>

float gradePoints(char grade)
{
    if (grade == 'A') return 4.0;
    if (grade == 'B') return 3.0;
    if (grade == 'C') return 2.0;
    if (grade == 'D') return 1.0;
    if (grade == 'F') return 0.0;
    return -1.0;  /* invalid*/
}
char upperChar(char i) /* Uppercase conversion */
{
    if (i >= 'a' && i <= 'z') {
        return i - 32;
    }
    return i;
}

int semester(char a[], char b[]) /* Semsester comparison*/
{
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') { /* while not at end of either string  */
        if (a[i] != b[i]) return 0; /* if characters differ then the strings are not equal*/
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}


typedef struct /*Input structure*/
{
    char grade;
    int hours;
    char semester[10];
    int year;
} Course;

typedef struct /*GPA calculation structure*/
{
    char semester[10];
    int year;
    int hours;
    float points;
} Term;


int main() {
    Course *courses = NULL; /*dynamic*/
    Term *terms = NULL; /*dynamic*/
    int courseCount = 0;
    int termCount = 0;
    printf("----> To quit entering grades, enter a blank line.\n");

    while (1) {
    char line[100];
    char grade;
    int hours;
    char sem[10];
    int year;
    
    /*prompt */
    printf("Enter a letter grade and hours for a course with its year and semester (ex. A 3 FALL 2025): ");
     /* Red the whole line input*/   
    if (!fgets(line, sizeof(line), stdin)) {
        break;
    }

    // blank line = exit
    if (line[0] == '\n') {
        break;
    }

    // parse input
    if (sscanf(line, " %c %d %9s %d", &grade, &hours, sem, &year) != 4) {
        printf("Invalid.\n");
        continue;
    }

        if (year < 1990 || year > 2110) {
            printf("Invalid year (Valid: 1990 to 2110).\n");
            continue;
}

        grade = upperChar(grade);

        if (gradePoints(grade) < 0) {
            printf("Invalid grade (A-F).\n");
            continue;
        }

        // uppercase
        for (int i = 0; sem[i] != '\0'; i++) {
            if (sem[i] >= 'a' && sem[i] <= 'z') {
                sem[i] -= 32;
            }
        }

        if (hours <= 0) {
            printf("Invalid hours.\n");
            while (getchar() != '\n');
            continue;
        }

        if (!(semester(sem, "FALL") ||
              semester(sem, "SPRING") ||
              semester(sem, "SUMMER"))) {
            printf("Invalid semester.\n");
            while (getchar() != '\n');
            continue;
        }

        Course *temp = realloc(courses, (courseCount + 1) * sizeof(Course));
        if (!temp) return 1;

        courses = temp;

        courses[courseCount].grade = grade;
        courses[courseCount].hours = hours;
        courses[courseCount].year = year;

        int i = 0;
        while (sem[i] != '\0') {
            courses[courseCount].semester[i] = sem[i];
            i++;
        }
        courses[courseCount].semester[i] = '\0';
        courseCount+= 1;
    }

    if (courseCount == 0) {
        printf("\nNo data (ENTER DATA!!!!).\n");
        return 0;
    }

    float totalPoints = 0;
    int totalHours = 0;

    for (int i = 0; i < courseCount; i++) {
        float pts = gradePoints(courses[i].grade) * courses[i].hours;

        totalPoints += pts;
        totalHours += courses[i].hours;

        int found = -1;

        for (int j = 0; j < termCount; j++) {
            if (terms[j].year == courses[i].year &&
                semester(terms[j].semester, courses[i].semester)) {
                found = j;
                break;
            }
        }

        if (found == -1) {
            Term *temp = realloc(terms, (termCount + 1) * sizeof(Term)); /* Creates new term if not dound*/
            if (!temp) return 1;

            terms = temp;

            int k = 0;
            while (courses[i].semester[k] != '\0') {
                terms[termCount].semester[k] = courses[i].semester[k]; /* copy semester name into term */
                k++;
            }
            terms[termCount].semester[k] = '\0';
            /* Intialize*/
            terms[termCount].year = courses[i].year;
            terms[termCount].hours = 0;
            terms[termCount].points = 0.0;

            found = termCount;
            termCount++;
        }

        terms[found].hours += courses[i].hours;
        terms[found].points += pts;
    }

    printf("\n---------------------------------------------\n");
    printf("Unofficial Transcript\n");

    for (int i = 0; i < termCount; i++) {
        double gpa = terms[i].points / terms[i].hours;
        printf("%s %d (%d hours) %.3f\n",
               terms[i].semester,
               terms[i].year,
               terms[i].hours,
               gpa);
    }
    printf("\n--\n");
    printf("Total credit hours: %d\n", totalHours);
    printf("Overall GPA: %.3f\n", totalPoints / totalHours);
    printf("---------------------------------------------\n");
    /* free memory*/
    free(courses);
    free(terms);

    return 0;
}