#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL || value == NULL) 
        return;
    if (tree->root == NULL){
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
    }
    else{
        TreeNode * aux = tree->root;
        
        while(aux != NULL){
            if(is_equal(tree,key,aux->pair->key))
                return;
                
            else{
                if(tree->lower_than(key,aux->pair->key) == 1){
                    if(aux->left == NULL){
                        TreeNode *nuevoNodo = createTreeNode(key, value);
                        nuevoNodo->parent = aux;
                        aux->left = nuevoNodo;
                        tree->current = nuevoNodo;
                        return;
                    }
                    else
                        aux = aux->left;
                }
                else{
                    if(aux->right == NULL){
                        TreeNode *nuevoNodo = createTreeNode(key, value);
                        nuevoNodo->parent = aux;
                        aux->right = nuevoNodo;
                        tree->current = nuevoNodo;
                        return;
                    }
                    else
                        aux = aux->right;
                }
            }
        }
    }
    return;
        
}

TreeNode * minimum(TreeNode * x){
    while(x->left != NULL)
        x = x->left;
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (tree == NULL || node == NULL)
        return;
    if (node->left == NULL && node->right == NULL){
        if(node->parent->left == node)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;
    }
    else{
        if(node->left != NULL && node->right == NULL){
            if(node->parent->left == node){
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
            else{
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }
        if(node->right != NULL && node->left == NULL){
            if(node->parent->left == node){
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
            else{
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
        }
        if(node->left != NULL && node->right != NULL){
            TreeNode *minimo = minimum(node->right);
            node->pair->key = minimo->pair->key;
            node->pair->value = minimo->pair->value;
            removeNode(tree,minimo);
        }
    }
    return;
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL || key == NULL)
        return NULL;
    TreeNode * aux = tree->root;
    while(aux != NULL){
        if(is_equal(tree,key,aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        }
        else{
            if(tree->lower_than(key,aux->pair->key) == 1){
                if(aux->left == NULL)
                    return NULL;
                else
                    aux = aux->left;
            }
            else{
                if(aux->right == NULL){
                    return NULL;
                }
                else{
                    aux = aux->right;
                }
            }
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL || key == NULL){
        return NULL;
    }
    TreeNode *auxiliar = tree->root;
    TreeNode *resultado = NULL;
    while(auxiliar != NULL){
        if(tree->lower_than(key, auxiliar->pair->key)){
            resultado = auxiliar;
            auxiliar = auxiliar->left;
        }
        else if(is_equal(tree, key, auxiliar->pair->key)){
            resultado = auxiliar;
            break;
        }
        else{
            auxiliar = auxiliar->right;
        }
    }
    if(resultado == NULL)
        return NULL;
    else
        return resultado->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = minimum(tree->root);
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {

    if (tree == NULL || tree->current == NULL) return NULL;
    TreeNode * aux = tree->current;
    if(aux->right != NULL){
        tree->current = minimum(aux->right);
        return tree->current->pair;
    }
    else{
        while(aux->parent != NULL && aux->parent->right == aux){
            aux = aux->parent;
        }
        tree->current = aux->parent;
        if(tree->current != NULL)
            return tree->current->pair;
        else
            return NULL;
    }
    return NULL;
}
