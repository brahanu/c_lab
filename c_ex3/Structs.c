/**
 * @file Structs.c
 * @author  Brahan Wassan <brahan>
 * @version 1.0
 * @date 12 Dec 2019
 *
 * @brief Program that define Vectors which can be nodes in the RBTree
 *
 * @section DESCRIPTION
 * The program builds a generic Vector
 * Input  : Vector data
 * Process: checks if the user input is valid, and then define a node
 * Output : a valid node
 */
#include <string.h>
#include <malloc.h>
#include "Structs.h"

#define LESS (-1)
#define EQUAL (0)
#define GREATER (1)
#define SUCCESS (1)
#define TRUE (1)
#define FAIL (0)
#define SQUARE(a) (a)*(a)
#define UNDEFINED_SIZE (-1)

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    char *v = (char *) a;
    char *u = (char *) b;
    return strcmp(v, u);
}

/**
 * ForEach function that concatenates the given word to pConcatenated. pConcatenated is already allocated with
 * enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    const char *cWord = (char *) word;
    char *cP = (char *) pConcatenated;
    size_t firstLen = strlen(cWord);
    size_t secLen = strlen(cP);
    if (strcat(cP, cWord))
    {
        strcat(cP, "\n");
    }
    if (strlen(cP) < firstLen + secLen)
    {
        return FAIL;
    }
    return SUCCESS;

}

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    Vector *v = (Vector *) a;
    Vector *u = (Vector *) b;
    int isEq = 0;
    if (v->len == u->len)
    {
        isEq = TRUE;
    }
    int minLen = v->len;
    if (minLen > u->len)
    {
        minLen = u->len;
    }
    int i = 0;
    while (i < minLen)
    {
        if (v->vector[i] != u->vector[i])
        {
            if (v->vector[i] > u->vector[i])
            {
                return GREATER;
            }
            return LESS;
        }
        ++i;
    }
    if (isEq == TRUE)
    {
        return EQUAL;
    }
    else
    {
        if (minLen == v->len)
        {
            return LESS;
        }
        return GREATER;
    }
}
/**
 * calculate vector norm
 * @param v the vector
 * @return the vector norm
 */
double calcNorm(const Vector *v)
{
    if (v == NULL || v->vector == NULL)
    {
        return UNDEFINED_SIZE;
    }
    int len = v->len;
    double norm = 0;
    double cur = 0;
    for (int i = 0; i < len; ++i)
    {
        cur = SQUARE(v->vector[i]);
        norm += cur;
    }
    return norm;
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    if (pVector == NULL)
    {
        return FAIL;
    }
    const Vector *toCopy = (Vector *) pVector;
    if (toCopy->vector == NULL)
    {
        return FAIL;
    }
    Vector *maxVec = (Vector *) pMaxVector;
    if (maxVec->vector == NULL || calcNorm(maxVec) < calcNorm(toCopy))
    {
        if (maxVec->vector != NULL)
        {
            free(maxVec->vector);
        }
        maxVec->vector = (double *) malloc(sizeof(double) * toCopy->len);
        if (maxVec->vector == NULL)
        {
            free(maxVec);
            return FAIL;
        }
        for (int i = 0; i < toCopy->len; ++i)
        {
            maxVec->vector[i] = toCopy->vector[i];
        }
        maxVec->len = toCopy->len;
    }
    return SUCCESS;
}

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 * // implement it in Structs.c You must use copyIfNormIsLarger in the implementation!
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    Vector *cur = (Vector *) malloc(sizeof(Vector));
    if (cur == NULL)
    {
        return NULL;
    }
    cur->len = 0;
    cur->vector = NULL;
    Vector *pMaxNorm = (Vector *) cur;
    int flag = forEachRBTree(tree, copyIfNormIsLarger, pMaxNorm);
    if (flag == FAIL)
    {
        return NULL;
    }
    return pMaxNorm;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    if (s == NULL)
    {
        return;
    }
    free(s);
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    Vector *node = (Vector *) pVector;
    if (node != NULL)
    {
        if (node->vector != NULL)
        {
            free(node->vector);
        }
        free(pVector);
    }
}
