#include <iostream>
#include <stdlib.h>
#include <conio.h>
using namespace std;

class node{

 public:

    int key;

    node *left;
    node *right;
    node *parent;

    bool color;

    node(int k, bool c = true) //a node is default created as red
    {
        key = k;
        left = NULL;
        right = NULL;
        color = c; //true=red and false=black
        parent=NULL;
    }
};

class rbtree{

  public:

    node *root; //notice pointer!

    rbtree()
    {
        root = NULL;
    }

    void ins(int); //wrapper
    void ins(int, node*);
    void insfixup(node *);

    void leftrotate(node *);
    void rightrotate(node *);

    node* insuccessor(node *);

    void del(int);
    void del(node *);
    void delfixup(node *, bool flag=false);

    node* searchh(int);
    node* searchh(int, node*);
    node *largest();
    node *largest(node *);
    node *smallest();
    node *smallest(node *);

    int calcblackheight();
    int calcblackheight(node *);

};

void rbtree::ins(int k)
{
    if(root==NULL)
    {
        root = new node(k, false);
        return;
    }
    else
        ins(k, root);
}

void rbtree::ins(int k, node *cur)
{
    if(root==NULL)
    {
        root = new node(k, false);
        return;
    }

    if(cur->key < k)
    {
        if(cur->right == NULL)
        {
            cur->right = new node(k);
            cur->right->parent = cur;

            insfixup(cur->right);
        }
        else
            ins(k, cur->right);
    }
    else if(cur->key > k)
    {
        if(cur->left == NULL)
        {
            cur->left = new node(k);
            cur->left->parent = cur;

            insfixup(cur->left);
        }
        else
            ins(k, cur->left);
    }
}


void rbtree::insfixup(node *z)
{
   node *uncle = NULL;
   node *parent = NULL;
   node *grandparent = NULL;

   if(z->parent!=NULL)
    parent = z->parent;

   if(parent!=NULL)
    grandparent=parent->parent;

   //z always points to a red node. violations exists as long as parent is red also
   while(parent!=NULL && parent->color) //red=true
   {
       if(parent == grandparent->left)
        uncle = grandparent->right;

       else
        uncle = grandparent->left;

       if(uncle!=NULL && uncle->color) //if uncle is red, just recolor
       {
           uncle->color = false;
           parent->color = false;
           grandparent->color = true;

           z = grandparent; //to continue the loop
       }

       else //uncle is black, either because it is NULL or it is actually a node with color black
       {
           if(z == parent->left)
           {
               if(uncle == grandparent->right) //line case
               {
                   rightrotate(grandparent);
                   parent->color = false;
                   grandparent->color = true;
               }
               else //triangle case, which will reduce to line case
               {
                   rightrotate(parent);
                   z = parent;
               }
           }

           else
           {
               if(uncle == grandparent->right) //triangle case
               {
                   leftrotate(parent);
                   z = parent;
               }
               else //line case
               {
                   leftrotate(grandparent);
                   parent->color = false; //or parent->color = !parent->color
                   grandparent->color = true;
               }
           }
       }

       //to continue the loop. by now, the new z for the next iteration has been set
       parent = z->parent;

       if(parent!=NULL)
        grandparent = parent->parent;
   }

    //root case
    if(root->color)
        root->color = false;
}


void rbtree::leftrotate(node *x)
{
    node *y = x->right;

    x->right = y->left;
    if(y->left!=NULL)
        y->left->parent=x;

    y->parent = x->parent;

    if(x->parent==NULL) //that is, x was the root
        root=y;
    else if(x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}


void rbtree::rightrotate(node *x)
{
    node *y = x->left;

    x->left = y->right;
    if(y->right!=NULL)
        y->right->parent = x;

    y->parent = x->parent;

    if(x->parent==NULL)
        root=y;
    else if(x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->right = x;
    x->parent = y;
}


node* rbtree::insuccessor(node *cur)
{
    if(cur==NULL)
        return NULL;
    if(cur->left == NULL) //the node we need
        return cur;
    else
        return insuccessor(cur->left);
}


node* rbtree::searchh(int k)
{
    return(searchh(k, this->root));
}

node* rbtree::searchh(int k, node* cur)
{
    if(cur==NULL)
        return NULL;

    if(cur->key == k)
        return cur;

    else if(cur->key < k)
        return searchh(k, cur->right);

    else return searchh(k, cur->left);
}

node* rbtree::largest()
{
    return(largest(root));
}

node* rbtree::largest(node *cur)
{
    if(cur->right==NULL)
        return cur;
    else
        return (largest(cur->right));
}

node* rbtree::smallest()
{
    return(smallest(root));
}

node* rbtree::smallest(node *cur)
{
    if(cur->left==NULL)
        return cur;
    else
        return (smallest(cur->left));
}



void rbtree::del(int k)
{
    node *cur = searchh(k);
    del(cur);
}


void rbtree::del(node *cur)
{
    node* x; //double-black kid


    if(cur==NULL)
    {
        cout<<"This key is not present in the tree"<<endl;
        return;
    }

    if(cur->left!=NULL && cur->right!=NULL)
    {
        node *succ = insuccessor(cur->right);
        cur->key = succ->key;

        cur = succ; //so succ is to be deleted now
    }

    bool flag = cur->color;
    //above case can also fall into the below cases
    if(cur->left==NULL && cur->right==NULL)
    {

        if(!flag)
            x = NULL;

        if(cur==root)
            root = NULL;
        else
        {
            if(cur == cur->parent->left)
                cur->parent->left = NULL;
            else
                cur->parent->right = NULL;
        }
    }

    else if(cur->left==NULL)
    {
        if(!flag) //doubleblack kid is needed only if a black node is being deleted.
            x = cur->right;

        cur->right->parent = cur->parent;

        if(cur==root)
            root = cur->right;
        else
        {
            if(cur == cur->parent->left)
                cur->parent->left = cur->right;
            else
                cur->parent->right = cur->right;
        }
    }
    else
    {
        if(!flag)
            x = cur->left;

        cur->left->parent = cur->parent;

        if(cur==root)
            root = cur->left;
        else
        {
            if(cur == cur->parent->left)
                cur->parent->left = cur->left;
            else
                cur->parent->right = cur->left;
        }
    }

    node *parent = cur->parent;
    delete(cur);

    if(!flag) //if node deleted was black
    {
        if(x==NULL)
            delfixup(parent, true); //note which parameter is being sent
        else
            delfixup(x);
    }
}


void rbtree::delfixup(node* x, bool flag) //flag to check if doubleblack is a null node or not
{
    //x will always point to a double black node except when flag is true
    //loop runs as long as double-black node exists
    //break out of loop for terminating cases

    node* sibling=NULL;
    node *parent = NULL;

    if(flag)
    {
        parent = x;
        x = NULL;  //update
    }
    else
        parent = x->parent;

    //if the doubleblack node is NULL => flag is true. if doubleblack node is not NULL, usual conventions follow - x is doubleblack and parent is x->parent
    while(x!=root)
    {
        //update parent when x is updated. if x is not being updated, parent doesn't need to either, so no need of a parent update at the beginning of every while loop
        if(x == parent->left)
        {
            sibling = parent->right;

            if(sibling!=NULL && sibling->color) //sibling is red
            {
                parent->color = true;
                sibling->color = false;

                leftrotate(parent);
            }

            else //sibling is black
            {
                if(sibling->right!=NULL && sibling->right->color) //sibling's right child is red
                {
                    leftrotate(parent);

                    //swap colors of parent and sibling
                    bool temp;
                    temp = parent->color;
                    parent->color = sibling->color;
                    sibling->color = temp;

                    sibling->right->color = false;

                    break; //terminating case
                }

                else if(sibling->left!=NULL && sibling->left->color) //sibling's left child is red and right child is black
                {
                    //recolor
                    sibling->color = true;
                    sibling->left->color = false;

                    rightrotate(sibling);
                }

                else //sibling's both kids are black
                {
                    sibling->color = true;

                    if(parent->color)
                    {
                        parent->color = false;
                        break; //terminating case
                    }
                    else
                    {
                        x = parent; //continue to solve violation
                        parent = x->parent;

                        flag = false; //update! x is no longer NULL
                    }
                }
            }
        }

        else
        {
            sibling = parent->left;

            if(sibling!=NULL && sibling->color) //sibling is red
            {
                parent->color = true;
                sibling->color = false;

                rightrotate(parent);
            }

            else //sibling is black
            {
                if(sibling->left!=NULL && sibling->left->color) //sibling's left child is red
                {
                    rightrotate(parent);

                    //swap colors of parent and sibling
                    bool temp;
                    temp = parent->color;
                    parent->color = sibling->color;
                    sibling->color = temp;

                    sibling->left->color = false;

                    break; //terminating case
                }

                else if(sibling->right!=NULL && sibling->right->color) //sibling's right child is red and left child (existing or NULL) is black
                {
                    //recolor
                    sibling->color = true;
                    sibling->right->color = false;

                    leftrotate(sibling);
                }

                else //sibling's both kids are black
                {
                    sibling->color = true;

                    if(parent->color) //if parent is red
                    {
                        parent->color = false;
                        break; //terminating case
                    }
                    else
                    {
                        x = parent; //continue to solve violation; doubleblack is propagated upwards
                        parent = x->parent;

                        flag = false;
                    }
                }
            }
        }
    }//end of while

    //root case
    if(x==root && x!=NULL)
        x->color = false;
}



int rbtree::calcblackheight()
{
    return(calcblackheight(root));
}


int rbtree::calcblackheight(node *cur) //returns blackheight which includes the node itself
{
    if(cur==NULL)
        return 1; //cuz NULL node is black by default
    if(!cur->color) //if black node, add it to blackheight
        return (1 + calcblackheight(cur->left));
    else //red node doesn't add to blackheight
        return(calcblackheight(cur->left));
}


void inorder(node *cur) //a pointer
{
    if(cur==NULL)
        return;

    inorder(cur->left);
    cout<<cur->key<<" ";
    inorder(cur->right);
}


void tester(node *cur)
{
    if(cur==NULL)
        return;
    cout<<endl<<cur->key<<"\t"<<"color:"<<cur->color<<"\t";
    if(cur->left!=NULL)
        cout<<"left:"<<cur->left->key<<"\t";
    if(cur->right!=NULL)
        cout<<"right:"<<cur->right->key;

    tester(cur->left);
    tester(cur->right);
}


node* mergetrees(rbtree T1, rbtree T2)
{
    if(T1.root==NULL && T2.root==NULL)
    {
        cout<<"\n\nBoth trees are empty!\n\n";
        return NULL;
    }
    else if(T1.root==NULL) //empty tree
    {
        cout<<"\n\nTree T1 is empty, so merge results in tree T2 itself\n\n";
        return T2.root;
    }
    else if(T2.root==NULL)
    {
        cout<<"\n\nTree T2 is empty, so merge results in tree T1 itself\n\n";
        return T1.root;
    }


    //canmerge(T1, T2) -- check if all keys in T1 are less than all keys in T2
    node *l = T1.largest();
    node *s = T2.smallest();

    //checking if all keys of T1 are less than all keys of T2
    if(l->key > s->key)
    {
        cout<<"\n\nMerge not possible as some keys in T1 are greater than some keys in T2\n\n";
        return NULL;
    }

    int midkey = l->key;
    T1.del(l);

    node *x = new node(midkey);

    int bh1, bh2;


    bh1 = T1.calcblackheight();
    bh1--; //blackheight isn't supposed to include root
    bh2 = T2.calcblackheight();
    bh2--;

    int r; //rank
    node* temp=NULL;

    cout<<"\n\nMerged tree\n\n";


    if(bh1==0) //T1 has become empty on account of removal of x
    {
        T2.ins(midkey);
        return T2.root;
    }

    else if(bh1==bh2) //x is root of merged tree
    {
        x->color = false;
        x->left = T1.root;
        x->right = T2.root;

        return x;
    }
    else if(bh1 > bh2) //T1 contains merged tree
    {
        //find the first node in T1 with blackheight bh2
        r = bh1;
        temp = T1.root;

        do //this node pakka exists so no need of a NULL check for temp
        {
            temp = temp->right;
            if(!temp->color) //if it is a black node
                r--;
        }while(r!=bh2);

        x->left = temp;
        x->right = T2.root;
        x->parent = temp->parent;
        temp->parent->right = x;

        if(x->parent->color) //violation! parent is also red
            T1.insfixup(x);

        return(T1.root);

    }
    else //T2 contains merged tree
    {
        r = bh2;
        temp = T2.root;

        do
        {
            temp = temp->left;
            if(!temp->color)
                r--;
        }while(r!=bh1);

        x->left = T1.root;
        x->right = temp;
        x->parent = temp->parent;
        temp->parent->left = x;

        if(x->parent->color)
            T2.insfixup(x);

        return(T2.root);
    }
}


int main()
{
    rbtree T1, T2, T3;

    int option1=0, option2=0;

    int val;

    while(option1!=7)
    {
        system("cls");
        cout<<"Pick an option\n\n"<<"1. Build T1\n2. View T1\n3. Build T2\n4. View T2\n5. Merge T1 and T2\n6. View merged tree\n7. Exit\n\nOption: ";
        cin>>option1;

        switch(option1)
        {
            case 1: while(option2!=3)
                    {
                        cout<<"\n\nPick an option\n\n"<<"1. Insert\n2. Delete\n3. Exit\n\nOption: ";
                        cin>>option2;

                        switch(option2)
                        {
                            case 1: cout<<"\n\nEnter key to insert: ";
                                    cin>>val;
                                    T1.ins(val);
                                    tester(T1.root);
                                    break;

                            case 2: cout<<"\n\nEnter key to delete: ";
                                    cin>>val;
                                    T1.del(val);
                                    tester(T1.root);
                                    break;

                            default: break;
                        }
                    }
                    break;

            case 3: while(option2!=3)
                    {
                        cout<<"\n\nPick an option\n\n"<<"1. Insert\n2. Delete\n3. Exit\n\nOption: ";
                        cin>>option2;

                        switch(option2)
                        {
                            case 1: cout<<"\n\nEnter key to insert: ";
                                    cin>>val;
                                    T2.ins(val);
                                    tester(T2.root);
                                    break;

                            case 2: cout<<"\n\nEnter key to delete: ";
                                    cin>>val;
                                    T2.del(val);
                                    tester(T2.root);
                                    break;

                            default: break;
                        }
                    }
                    break;

            case 5: ;
                    T3.root = mergetrees(T1, T2);
                    T1.root=NULL;
                    T2.root=NULL;

                    if(T3.root!=NULL)
                        tester(T3.root);

                    _getch();
                    break;

            case 2: if(T1.root==NULL)
                        cout<<"\n\nTree T1 is empty\n";
                    else
                        tester(T1.root);

                    _getch(); break;

            case 4: if(T2.root==NULL)
                        cout<<"\n\nTree T2 is empty\n";
                    else
                        tester(T2.root);

                    _getch(); break;

            case 6: if(T3.root==NULL)
                        cout<<"\n\nMerged tree is empty\n";
                    else
                        tester(T3.root);

                    _getch(); break;
            default : break;
        }

        option2 = 0; //reset
    }





/*  cout << "Hello world!" << endl;
    rbtree T1;


    T1.ins(2);
    T1.ins(1);
    T1.ins(4);
    T1.ins(5);
    T1.ins(9);
    T1.ins(3);
    T1.ins(6);
    T1.ins(7);


    T1.ins(9);
    T1.ins(8);
    T1.ins(7);
    T1.ins(3);
    T1.ins(5);
    T1.ins(2);

    inorder(T1.root);
    tester(T1.root);

    cout<<endl<<"Black height of tree "<<T1.calcblackheight(T1.root);


    T1.del(4);
    T1.del(2);
    T1.del(1);


    T1.del(2);
    T1.del(5);
    T1.del(3);
    T1.del(7);
    T1.del(8);
    T1.del(9);

    cout<<endl;
    tester(T1.root);


    node *ans = T1.insuccessor(T1.root->right);
    cout<<endl<<ans->key<<endl;



    T1.ins(1);
    T2.ins(2);

    mergetrees(T1, T2);
*/
    return 0;
}
