/**
 * @file RBTree.c
 * @author  Brahan Wassan <brahan>
 * @version 1.0
 * @date 12 Dec 2019
 *
 * @brief Program that build a RedBlack tree
 *
 * @section DESCRIPTION
 * The program builds a generic RB tree
 * Input  : tree nodes
 * Process: checks if the user input is valid, and then build the tree
 * Output : a tree with the desired data types
 */
#include <stdio.h>
#include "RBTree.h"
#include <stdlib.h>

#define SUCCESS 1
#define FAIL 0
#define EQUAL 0

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *tree = NULL;
    tree = (RBTree *) malloc(sizeof(RBTree));
    if (tree == NULL)
    {
        return NULL;
    }
    tree->root = NULL;
    tree->compFunc = compFunc;
    tree->freeFunc = freeFunc;
    tree->size = 0;
    return tree;
}

/**
 * created a new node
 * @param data the data which the node holds
 * @return a new node
 */
Node *newNode(void *data)
{
    Node *node = NULL;
    node = (Node *) malloc(sizeof(Node));

    if (node == NULL)
    {
        return NULL;
    }
    node->data = data;
    node->color = RED;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

/**
 * rotates the tree nodes to the right as we saw in DAST
 * @param node the node which we need to fix its position
 */
void rotateRight(Node *node)
{
    Node *left = node->left;
    Node *parent = node->parent;
    if (left->right != NULL)
    {
        left->right->parent = node;
    }
    node->left = left->right;
    node->parent = left;
    left->right = node;
    left->parent = parent;
    if (parent != NULL)
    {
        if (parent->right == node)
        {
            parent->right = left;
        }
        else
        {
            parent->left = left;
        }
    }
}

/**
 * rotates the tree nodes to the left as we saw in DAST
 * @param node the node which we need to fix its position
 */
void rotateLeft(Node *node)
{
    Node *right = node->right;
    Node *parent = node->parent;
    if (right->left != NULL)
    {
        right->left->parent = node;
    }
    node->right = right->left;
    node->parent = right;
    right->left = node;
    right->parent = parent;

    if (parent != NULL)
    {
        if (parent->left == node)
        {
            parent->left = right;
        }
        else
        {
            parent->right = right;
        }
    }
}

/**
 * a getter
 * @param node the node
 * @return parent
 */
Node *getParent(Node *node)
{
    return node == NULL ? NULL : node->parent;
}

/**
 * a getter
 * @param node the node
 * @return the grandparent
 */
Node *getGrandParent(Node *node)
{
    return getParent(getParent(node));
}

/**
 * a getter
 * @param node the node
 * @return the node sibling
 */
Node *getSibling(Node *node)
{
    Node *parent = getParent(node);

    if (parent == NULL)
    {
        return NULL;
    }
    if (node == parent->left)
    {
        return parent->right;
    }
    else
    {
        return parent->left;
    }
}

/**
 * a getter
 * @param node the node
 * @return the parent sibling
 */
Node *getUncle(Node *n)
{
    Node *p = getParent(n);
    return getSibling(p);
}

/**
 * case 4 second step defined by the pdf
 * @param tree the tree
 * @param node the node we added to the tree
 */
void caseFourSecondStep(RBTree *tree, Node *node)
{
    Node *grandP = getGrandParent(node);
    Node *parent = node->parent;
    parent->color = BLACK;
    grandP->color = RED;
    if (node == parent->left)
    {
        rotateRight(grandP);
        if (getGrandParent(node) == NULL)
        {
            tree->root = node->parent;
        }
    }
    else
    {
        rotateLeft(grandP);
        if (getGrandParent(node) == NULL)
        {
            tree->root = node->parent;
        }
    }
}// need to add it
/**
 * the 4 cases which we fix the tree accordingly
 * @param tree the tree which need to be fixed
 * @param node the node which we added
 */
void treeFix(RBTree *tree, Node *node)
{
    Node *uncle = getUncle(node);
    Node *parent = getParent(node);
    Node *grandP = getGrandParent(node);
    if (parent == NULL)
    {
        node->color = BLACK;
    }
    else if (parent->color == BLACK)
    {
        return;
    }
    else if (uncle != NULL && uncle->color == RED)
    {
        parent->color = BLACK;
        uncle->color = BLACK;
        grandP->color = RED;
        treeFix(tree, grandP);
    }
    else
    {
        if (node == parent->right && parent == grandP->left)
        {
            rotateLeft(parent);
            if (getGrandParent(node) == NULL)
            {
                tree->root = node->parent;
            }
            node = node->left;
        }
        else if (node == parent->left && parent == grandP->right)
        {
            rotateRight(parent);
            if (getGrandParent(node) == NULL)
            {
                tree->root = node->parent;
            }
            node = node->right;
        }
        caseFourSecondStep(tree, node);
    }
}

/**
 * inserting a node to the first null place its finds recurse
 * @param cur current node
 * @param node the node we want to insert into the tree
 * @param compareFunc the tree comp func
 */
void regularBSTInsert(RBTree *tree, Node *cur, Node *node, CompareFunc compareFunc)
{
    if (cur != NULL && cur->data != NULL)
    {
        if (compareFunc(cur->data, node->data) > EQUAL)
        {
            if (cur->left == NULL)
            {
                cur->left = node;
            }
            else
            {
                regularBSTInsert(tree, cur->left, node, compareFunc);
                return;
            }
        }
        else
        {
            if (cur->right == NULL)
            {
                cur->right = node;
            }
            else
            {
                regularBSTInsert(tree, cur->right, node, compareFunc);
                return;
            }
        }
        node->parent = cur;
        node->color = RED;
        ++tree->size;
    }
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int addToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return FAIL;
    }
    else
    {
        Node *node = newNode(data);
        if (tree->root == NULL) // case 1 new node is root
        {
            node->color = BLACK;
            tree->root = node;
            ++tree->size;
            return SUCCESS;
        }
        else if (containsRBTree(tree, data) == SUCCESS)
        {
            free(node);
            return FAIL;
        }
        else
        {
            regularBSTInsert(tree, tree->root, node, tree->compFunc);
            treeFix(tree, node);
            return SUCCESS;
        }
    }
}

/**
 * a helper function which search inorder for the node
 * @param root the current node
 * @param data the data which we looking for in the tree
 * @param compFunc the compare function which we can check if the nodes hold the identical data
 * @return 1 if found 0 if not
 */
int recursiveContains(Node *root, void *data, CompareFunc compFunc)
{
    if (root == NULL)
    {
        return FAIL;
    }
    else
    {
        if (compFunc(root->data, data) == EQUAL)
        {
            return SUCCESS;
        }
        else if (compFunc(root->data, data) < EQUAL)
        {
            return recursiveContains(root->right, data, compFunc);
        }
        return recursiveContains(root->left, data, compFunc);
    }
}

/**
 * check whether the tree contains this item.
 * @param tree: the tree to add an item to.
 * @param data: item to check.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int containsRBTree(RBTree *tree, void *data)
{
    if (tree->root == NULL || data == NULL)
    {
        return FAIL;
    }
    return recursiveContains(tree->root, data, tree->compFunc) ? SUCCESS : FAIL;
}

int inOrder(Node *root, forEachFunc func, void *args)
{
    int isOk = SUCCESS;
    if (root != NULL)
    {
        if (root->left != NULL)
        {
            isOk = inOrder(root->left, func, args);
            if (!isOk)
            {
                return isOk;
            }
        }
        isOk = func(root->data, args);
        if (!isOk)
        {
            return isOk;
        }
        if (root->right != NULL)
        {
            isOk = inOrder(root->right, func, args);
            if (!isOk)
            {
                return isOk;
            }
        }
    }
    return isOk;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(RBTree *tree, forEachFunc func, void *args)
{
    int ret = FAIL;
    if (tree != NULL)
    {
        ret = inOrder(tree->root, func, args);
    }
    return ret;
}

/**
 * helper function which traverse thru the tree and free all the nodes
 * @param root the current node
 * @param freeFunc the tree free function
 */
void freeAll(Node *root, FreeFunc freeFunc)
{
    if (root == NULL)
    {
        return;
    }
    freeAll(root->left, freeFunc);
    freeAll(root->right, freeFunc);
    freeFunc(root->data);
    free(root);
}

/**
 * free all memory of the data structure.
 * @param tree: the tree to free.
 */
void freeRBTree(RBTree *tree)
{
    freeAll(tree->root, tree->freeFunc);
    free(tree);
}