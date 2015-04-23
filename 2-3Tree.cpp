/*
 Author: Chris McDonald
 Course: CS452
 2-3 Tree Implementation
 */

#include <fstream>
#include <iostream>
#include <queue>
#include <string>

using namespace std;

// 2-3 Tree
class two_three_tree {
    
    // Only accessible within class: two_three_tree
private:
    
    // Node
    class two_three_node {
    public:
        int pos, size, value[3];
        two_three_node * parent, * child[3];
        
        // Constructor
        two_three_node(){
            size = pos =  0;
            value[0] = value[1] = value[2] = -1;
            child[0] = child[1] = child[2] = parent = 0;
        };
        
        // Creates a root
        static two_three_node * Root(int a){
            two_three_node * root = new two_three_node();
            root->parent = root;
            two_three_node * leaf = Leaf(a);
            root->add(leaf);
            
            return root;
        }
        
        // Creates a leaf
        static two_three_node * Leaf(int a){
            two_three_node * leaf = new two_three_node();
            leaf->size = 1;
            leaf->value[2] = a;
            
            return leaf;
        }
        
        // Prints a node - (0,0,0)
        void print_node(){
            cout << "(" << value[0] << ", " << value[1] << ", " << value[2] << ")     ";
        }
        
        // Is the node a leaf?
        bool is_leaf(){
            if(value[0] == -1 && value[1] == -1){
                return true;
            }
            else{
                return false;
            }
        }
        
        // Add a new child node to a current parent node
        void add(two_three_node * a){
            int a_value = a->value[2];
            if(size == 0){
                return_value(3) = a_value;
                return_child(3) = a;
                return_child(3)->parent = this;
                return_child(3)->pos = 3;
                
                if(pos != 0){
                    parent->return_value(pos) = return_value(3);
                }
            }
            else if(size == 1){
                if(a_value < value[2]){
                    return_value(1) = a_value;
                    return_child(1) = a;
                    return_child(1)->parent = this;
                    return_child(1)->pos = 1;
                }
                else{
                    move(3,1);
                    return_value(3) = a_value;
                    return_child(3) = a;
                    return_child(3)->parent = this;
                    return_child(3)->pos = 3;
                    
                    if(pos != 0){
                        parent->return_value(pos) = return_value(3);
                    }
                }
            }
            else{
                if(a_value < value[0]){
                    move(1,2);
                    return_value(1) = a_value;
                    return_child(1) = a;
                    return_child(1)->parent = this;
                    return_child(1)->pos = 1;
                }
                else if(a_value < value[2]){
                    return_value(2) = a_value;
                    return_child(2) = a;
                    return_child(2)->parent = this;
                    return_child(2)->pos = 2;
                }
                else{
                    move(3,2);
                    return_value(3) = a_value;
                    return_child(3) = a;
                    return_child(3)->parent = this;
                    return_child(3)->pos = 3;
                    
                    if(pos != 0){
                        parent->return_value(pos) = return_value(3);
                    }
                }
            }
            if(pos!=0 && parent->pos == 3){
                parent->parent->value[2] = parent->value[2];
            }
            
            size++;
        }
        
        // Return the correct child node
        two_three_node * & return_child(int child_pos){
            if(child_pos == 1){
                return child[0];
            }
            if(child_pos == 2){
                return child[1];
            }
            if(child_pos == 3){
                return child[2];
            }
        }
        
        // Return the correct value of the child
        int & return_value(int child_pos){
            if(child_pos == 1){
                return value[0];
            }
            if(child_pos == 2){
                return value[1];
            }
            if(child_pos == 3){
                return value[2];
            }
        }
        
        // Removes a child and returns the location of the removed child
        two_three_node * remove(int child_value){
            two_three_node * temp_node;
            
            if(child_value == 1){
                temp_node = child[0];
            }
            if(child_value == 2){
                temp_node = child[1];
            }
            if(child_value == 3){
                temp_node = child[2];
            }
            if(size > 1){
                if(child_value == 1){
                    if(size == 3){
                        move(2,1);
                        empty(2);
                    }
                    else{
                        empty(1);
                    }
                }
                if(child_value == 2){
                    empty(2);
                }
                if(child_value == 3){
                    if(size == 3){
                        move(2,3);
                        empty(2);
                    }
                    else{
                        move(1,3);
                        empty(1);
                    }
                }
            }
            else{
                empty(1);
                empty(2);
                empty(3);
            }
            size--;
            return temp_node;
        }
        
        // Empty the node
        void empty(int child_value){
            if(child_value == 0){
                child[0] = 0;
                value[0] = -1;
                child[1] = 0;
                value[1] = -1;
                child[2] = 0;
                value[2] = -1;
                if(pos != 0){
                    parent->return_value(pos) = -1;
                    parent->return_child(pos) = 0;
                }
            }
            if(child_value == 1){
                child[0] = 0;
                value[0] = -1;
            }
            if(child_value == 2){
                child[1] = 0;
                value[1] = -1;
            }
            if(child_value == 3){
                child[2] = 0;
                value[2] = -1;
            }
        }
        
        // Move the child from the original position to the new one
        void move(int origin, int new_loc){
            return_child(new_loc) = return_child(origin);
            return_value(new_loc) = return_value(origin);
            return_child(new_loc)->pos = new_loc;
            if(origin == 3){
                child[2] = 0;
                value[2] = -1;
            }
            else{
                empty(origin);
            }
            if(pos != 0 && new_loc == 3){
                parent->return_value(pos) = value[2];
            }
            if(pos == 3 && parent->pos != 0){
                parent->parent->return_value(parent->pos) = parent->value[2];
            }
        }
    };
    
public:
    two_three_node * root;
    
    // Constructor
    two_three_tree(){
        root = new two_three_node();
    };
    
    // Print the 2-3 Tree
    void print_tree(two_three_node * node){
        queue<two_three_node *> q;
        cout << "Breadth First Traversal of the 2-3 Tree:" << endl;
        int count = 0; // Number of nodes removed from the queue
        while (root != 0) {
            count++;
            if(root->is_leaf()){
                break;
            }
            if(root->child[0] != 0){
                q.push(root->child[0]);
            }
            if(root->child[1] != 0){
                q.push(root->child[1]);
            }
            if(root->child[2] != 0){
                q.push(root->child[2]);
            }
            // INCOMPLETE!!!
            // After each level is iterated through, go to the next line
            //if((count % 3 == 0 && count % 9 == 0 && count % 6 != 0)){
            //  cout << "\n";
            //}
            // Print the node
            root->print_node();
            if(!q.empty()){
                root = q.front();
                q.pop();
            }
            else{
                root = 0;
            }
        }
        cout << "\nEnd of the 2-3 Tree." << endl;
    }
    
    // Absorb two nodes in the tree
    void absorb(two_three_node * a, two_three_node * b){
        two_three_node * parent = b->parent;
        if(b == parent){
            root = new two_three_node();
            root->parent=root;
            root->add(b);
            root->add(a);
        }
        else if(parent->size <= 2){
            parent->add(a);
        }
        else if(parent->pos != 0){
            two_three_node * c_left = new two_three_node();
            two_three_node * c = new two_three_node();
            two_three_node * temp = parent->parent->remove(parent->pos);
            if(a->value[2] < temp->value[1]){
                c_left->add(a);
                c_left->add(temp->child[0]);
                c->add(temp->child[1]);
                c->add(temp->child[2]);
            }
            else{
                c_left->add(temp->child[0]);
                c_left->add(temp->child[1]);
                c->add(a);
                c->add(temp->child[2]);
            }
            parent->parent->add(c);
            absorb(c_left, c);
        }
        else{
            two_three_node * c_left = new two_three_node();
            two_three_node * c = new two_three_node();
            if(a->value[2] < parent->value[1]){
                c_left->add(a);
                c_left->add(parent->child[0]);
                c->add(parent->child[1]);
                c->add(parent->child[2]);
            }
            else{
                c_left->add(parent->child[0]);
                c_left->add(parent->child[1]);
                c->add(a);
                c->add(parent->child[2]);
            }
            root = new two_three_node();
            root->parent = root;
            root->add(c_left);
            root->add(c);
        }
        return;
    }
    
    // Search for a value in the tree
    int search(int a){
        two_three_node * node = root;
        if(node->size > 0){
            while(node->size > 1){
                if(a <= node->value[0]){
                    node = node->child[0];
                }
                else if(node->value[1] != -1 && a <= node->value[1]){
                    node = node->child[1];
                }
                else{
                    node = node->child[2];
                }
            }
            return node->value[2];
        }
        else{
            cout << "The tree is empty." << endl;
            return -1;
        }
    }
    
    // Insert a value into the tree
    void insert(int a){
        int search_value = search(a);
        if(search_value == a){
            cout << "The value " << a << " is already in the tree." << endl;
            return;
        }
        if(search_value == -1){
            root = two_three_node::Root(a);
            return;
        }
        two_three_node * node = root;
        while(node->size > 1){
            if(a <= node->value[0]){
                node = node->child[0];
            }
            else if(node->value[1] != -1 && a <= node->value[1]){
                node = node->child[1];
            }
            else{
                node = node->child[2];
            }
        }
        two_three_node * leaf = two_three_node::Leaf(a);
        absorb(leaf, node);
    }
    
};

void load_default();
void load_from_file();

int main ()
{
    cout << "(0) - (Use the default values)\n(1) - (Load values from a file)\n: ";
    int load_file = 0;
    cin >> load_file;
    if(load_file){
        load_from_file();
    }
    else{
        load_default();
    }
    return 0;
}

// Use the default test values
void load_default(){
    two_three_tree Tree;
    Tree.insert(45);
    Tree.insert(2);
    Tree.insert(5);
    Tree.insert(12);
    Tree.insert(8);
    Tree.insert(4);
    Tree.insert(13);
    Tree.insert(3);
    Tree.insert(20);
    Tree.insert(6);
    Tree.insert(17);
    Tree.insert(14);
    Tree.insert(24);
    Tree.insert(41);
    Tree.insert(34);
    Tree.insert(39);
    Tree.insert(16);
    Tree.insert(15);
    Tree.insert(29);
    Tree.print_tree(Tree.root);
}

// Use values from a file
void load_from_file(){
    string file_name;
    int number_of_values;
    cout << "Enter the name of the file you wish to use: ";
    cin >> file_name;
    cout << "\n";
    ifstream file(file_name.c_str());
    file >> number_of_values;
    int data;
    two_three_tree Tree;
    for(int x = 0; x < number_of_values; x++){
        file >> data;
        Tree.insert(data);
    }
    Tree.print_tree(Tree.root);
}
