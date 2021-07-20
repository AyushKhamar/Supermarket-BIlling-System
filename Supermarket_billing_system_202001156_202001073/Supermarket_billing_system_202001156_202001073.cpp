#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <bits/stdc++.h>
#define l long long int
using namespace std;
static l new_id = 9400000101;
class manager;
class item
{
private:
    l item_id;
    string name;
    int rate;
    int quantity;
    item *next = NULL; //This is the pointer to the next item

public:
    //contructors
    item()
    {
        this->item_id = 0;
        this->name = "none";
        this->rate = 0;
        this->quantity = 0;
    }
    //second constructor with input values
    item(l id, string name, int rate, int quantity)
    {
        this->item_id = id;
        this->name = name;
        this->rate = rate;
        this->quantity = quantity;
    }
    //these functions are made so that we can use its private variables in other classes that we will inherit it to
    l getid()
    {
        return this->item_id;
    }
    string getname()
    {
        return this->name;
    }
    int getrate()
    {
        return this->rate;
    }
    int getquantity()
    {
        return this->quantity;
    }
    //this function will return the pointer to the next item
    item *getnext()
    {
        return this->next;
    }
    //we keep this set quantity as public so that the customer is able to set the quanitity after his purchase but cannot change any other parameters of the item he chooses to buy
    void setquantity(int quantity)
    {
        this->quantity = quantity;
    }

private:
    //these are the parameters that the customer cannot change
    void setid(l id)
    {
        this->item_id = id;
    }
    void setname(string name)
    {
        this->name = name;
    }
    void setrate(int rate)
    {
        this->rate = rate;
    }
    void setnext(item *next)
    {
        this->next = next;
    }
    friend class item_linkedlist_base;
};
// this is the linklist that will be accessed using the hash table at every point
class item_linkedlist_base : public item
{
    item *head = NULL; //this head is used to iterate all the items in the list
public:
    void insert_item(l i, string name, int rate, int quantity)
    {
        // this is the new item to be added
        item *temp = new item;
        //this is the behind pointer incase we have to add the element in betweeen
        item *temp_behind = new item;
        //this is the head pointer used for traversing and also the ahead pointer incase we have to enter the element in between
        item *temp_ahead = head;
        temp->setid(i);
        temp->setname(name);
        temp->setrate(rate);
        temp->setquantity(quantity);
        if (temp_ahead == NULL) //linklist is empty
        {
            head = temp;
        }
        else
        {
            if (temp_ahead->getid() > i) //if the id of head is bigger then element will come before head
            {
                temp->setnext(temp_ahead);
                //to reset the head
                head = temp;
            }
            else
            {
                //this is to find the suitable position for the element in entire list
                while ((temp_ahead->getid() < temp->getid()) && (temp_ahead->getnext() != NULL))
                {
                    temp_behind = temp_ahead;
                    temp_ahead = temp_ahead->getnext();
                }
                //traverse the entire list and if we find no suitable position
                if ((temp_ahead->getnext() == NULL) && (temp_ahead->getid() < temp->getid()))
                {
                    //now we set this item as the last node of the list
                    temp_ahead->setnext(temp);
                }
                else
                {
                    //this adds the node in between 2 nodes and revises their links
                    temp_behind->setnext(temp);
                    temp->setnext(temp_ahead);
                }
            }
        }
    }
    //this function will return the pointer to data for a particular item in the inventory using the hash tables and using Id to locate it
    item *get_info_inventory(l id)
    {
        item *temp = head;
        //this while loop runs till we get the correct id
        while (temp->getid() != id && temp->getnext() != NULL)
        {
            temp = temp->getnext();
        }
        //if we dont find the element
        if (temp->getnext() == NULL && temp->getid() != id)
        {
            cout << " The item was not found" << endl;
            return NULL;
        }
        return temp;
    }

    // now in order to delete an item by the manager, we just use the logic of deletions in the case of link lists
    void delete_item(l id)
    {
        item *temp = head;
        //behind pointer is one item behind the temp pointer
        item *behind = new item;

        // if the item to be deleted is the first itself
        if (temp != NULL && temp->getid() == id)
        {
            head = temp->getnext();
            free(temp);
            return;
        }
        // the second case where we find the element to be deleted in between somewhere
        while (temp != NULL && temp->getid() != id)
        {
            behind = temp;
            temp = temp->getnext();
        }
        // if the loop breaks because id was not found
        if (temp == NULL)
        {
            cout << " The id is not found in the list" << endl;
            return;
        }
        //if we find the id in the list
        behind->setnext(temp->getnext());
        //free(temp);
    }

    //this display function is to show the entire inventory
    void display()
    {
        item *temp = head;
        //we iterate the entire list till the last element and we print each of them
        while (temp != NULL)
        {
            cout << temp->getid() << " ";
            cout << temp->getname() << " ";
            cout << temp->getrate() << " ";
            cout << temp->getquantity() << "-->";
            temp = temp->getnext();
        }
    }

    //this function is valid only for the manager and is used to change attributes of item
    void update_item(l i, string name, int rate, int quantity)
    {
        item *temp = head;
        //till we find the element or we end the list
        while (temp->getid() != i && temp->getnext() != NULL)
        {
            temp = temp->getnext();
        }
        // if(temp->getid()==i)
        // {
        // temp->setid(i);
        // temp->setname(name);
        // temp->setrate(rate);
        // temp->setquantity(quantity);
        // }
        temp->setid(i);
        temp->setname(name);
        temp->setrate(rate);
        temp->setquantity(quantity);
    }
    friend class inventory;
};
//this is the class where we make the entire bucket for hashing
class inventory : public item_linkedlist_base
{

    item_linkedlist_base *a = new item_linkedlist_base[100];
    //this makes a bucket of 100 elements and each element has a linked list in it.
    //now we can access each of the links with elements
    // a[0].function to access the functions of that id only

    //these are the functions which only manager can use
private:
    //function overloading
    //this updates the element
    void update_item(l i, string name, int rate, int quantity)
    {
        a[i % 100].update_item(i, name, rate, quantity);
    }
    //this overloaded function calls the upper function where it inserts this element into the link list at the postion a[i%100] of the bucket
    void insert_item(l i, string name, int rate, int quantity)
    {
        a[i % 100].insert_item(i, name, rate, quantity);
    }
    //this is same as above but for deleting an object
    void delete_item(l id)
    {
        a[id % 100].delete_item(id);
    }

    //these are the utility functions if we want to know any one parameter based on the id
    int search_rate(l id)
    {
        int item_hash = id % 100;
        //get info of inventory function returns a pointer to the object
        item *searchelement = a[item_hash].get_info_inventory(id);
        return searchelement->getrate();
    }
    string search_name(l id)
    {
        item *searchelement = a[id % 100].get_info_inventory(id);
        return searchelement->getname();
    }
    // int search_quantity(l id)
    // {
    //     item * searchelement = a[id%100].get_info_inventory(id);
    //     return searchelement->getquantity();

    // }

public:
    //overloaded function and it returns the item for the particular id of the element
    item *get_info_inventory(l i)
    {
        item *temp = a[i % 100].get_info_inventory(i);
        return temp;
    }
    //when one purchases an item then its quantity changes
    void update_stock(l id, int sold)
    {
        item *temp = a[id % 100].get_info_inventory(id);
        temp->setquantity(temp->getquantity() - sold);
        //this basically sets the difference of the stock and sold as the new quantity in the data base
    }
    void update_stock_item_deletion(l id, int sold)
    {
        item *temp = a[id % 100].get_info_inventory(id);
        temp->setquantity(temp->getquantity() + sold);
    }

    void display()
    {
        int i;
        for (i = 0; i < 100; i++)
        {
            a[i].display();
            cout << endl;
        }
    }
    friend class manager;
};
class customer
{

private:
    l customer_id;
    string name;
    int rewards;
    customer *next = NULL;

public:
    customer()
    {
        this->customer_id = 0;
        this->name = "none";
        this->rewards = 0;
    }
    customer(l id, string name, int rewards)
    {
        this->customer_id = id;
        this->name = name;
        this->rewards = rewards;
    }
    l getidcustomer()
    {
        return this->customer_id;
    }
    string getnamecustomer()
    {
        return this->name;
    }
    int getrewardscustomer()
    {
        return this->rewards;
    }
    customer *getnextcustomer()
    {
        return this->next;
    }

private:
    //in case we want to set the name and the id again
    //and these can be accessed only by the customer and not anyone else
    //an element of customer can only access these functions and the member of any other class cannot access these functions and cannot cahnge the variables the same logic goes above in the case of manager who can change the values of the objects
    void setidcustomer(l id)
    {
        this->customer_id = id;
    }
    void setnamecustomer(string name)
    {
        this->name = name;
    }
    void setnextcustomer(customer *next)
    {
        this->next = next;
    }

public:
    void setrewardscustomer(int rewards)
    {
        this->rewards = rewards;
    }

    friend class customer_linkedlist_base;
};
// this is the linked list in orde to store customers data like we did it in the case of the items
class customer_linkedlist_base : public customer
{

    customer *head = NULL;
    //this is the same code that we used for doing operations in the case of items
    void insert_customer(l i, string name, int rewards)
    {
        customer *temp = new customer;
        customer *temp_behind = new customer;
        customer *temp_ahead = head;
        temp->setidcustomer(i);
        temp->setnamecustomer(name);
        temp->setrewardscustomer(rewards);
        if (temp_ahead == NULL)
        {
            //no elements in the customer list
            head = temp;
        }
        else
        {
            //if customer is to be added before head
            if (temp_ahead->getidcustomer() > i)
            {
                temp->setnextcustomer(temp_ahead);
                head = temp;
            }
            else
            {
                //if customer is to be added at the last or inbetween somewhere based on id
                while ((temp_ahead->getidcustomer() < temp->getidcustomer()) && (temp_ahead->getnextcustomer() != NULL))
                {
                    temp_behind = temp_ahead;
                    temp_ahead = temp_ahead->getnextcustomer();
                }
                if ((temp_ahead->getnextcustomer() == NULL) && (temp_ahead->getidcustomer() < temp->getidcustomer()))
                {
                    temp_ahead->setnextcustomer(temp);
                }
                else
                {
                    temp_behind->setnextcustomer(temp);
                    temp->setnextcustomer(temp_ahead);
                }
            }
        }
    }
    //we get the entire pointer for the info of the customer
    customer *get_customer_info(l id)
    {
        customer *temp = head;
        while (temp->getidcustomer() != id && temp->getnextcustomer() != NULL)
        {
            temp = temp->getnextcustomer();
        }
        if (temp->getnextcustomer() == NULL && temp->getidcustomer() != id)
        {
            cout << "The customer was not found " << endl;
        }
        return temp;
    }
    void displayc()
    {
        //displays all the details of a customer
        customer *temp = head;
        while (temp != NULL)
        {

            cout << temp->getidcustomer() << " ";
            cout << temp->getnamecustomer() << " ";
            cout << temp->getrewardscustomer() << " "
                 << "-->";
            temp = temp->getnextcustomer();
        }
    }
    void update_customer(l i, string name, int rewards)
    {
        customer *temp = head;
        while (temp->getidcustomer() != i && temp->getnextcustomer() != NULL)
            temp = temp->getnextcustomer();

        temp->setidcustomer(i);
        temp->setnamecustomer(name);
        temp->setrewardscustomer(rewards);
    }
    void update_points(l id, int points)
    {
        customer *temp = head;
        while (temp->getidcustomer() != id && temp->getnextcustomer() != NULL)
        {
            temp = temp->getnextcustomer();
        }
        temp->setrewardscustomer(points);
    }
    void delete_customer(l id)
    {
        customer *temp = head;
        customer *behind = new customer;
        //if first customer is to be deleted
        if (temp != NULL && temp->getidcustomer() == id)
        {
            head = temp->getnextcustomer();
            free(temp);
            return;
        }
        while (temp != NULL && temp->getidcustomer() != id)
        {
            behind = temp;
            temp = temp->getnextcustomer();
        }
        if (temp = NULL)
        {
            cout << " The id was not found of the customer to be deleted" << endl;
            return;
        }
        behind->setnextcustomer(temp->getnextcustomer());
        // free(temp);
    }
    friend class customer_linkedlist;
    friend class manager;
};
class customer_linkedlist : public customer_linkedlist_base
{
    friend class manager;

private:
    //the definitions of these functions is kept private to avoid any changes
    customer_linkedlist_base *b = new customer_linkedlist_base[100];

    void displayc()
    {
        int i;
        for (int i = 0; i < 100; i++)
        {
            b[i].displayc();
            cout << endl;
        }
    }
    void insert_customer(l i, string name, int rewards)
    {
        b[i % 100].insert_customer(i, name, rewards);
    }
    void update_customer(l i, string name, int rewards)
    {
        b[i % 100].update_customer(i, name, rewards);
    }
    void delete_customer(l id)
    {
        b[id % 100].delete_customer(id);
    }

public:
    //these are utility functions to add customers and update points
    void add_customer(l id, string name, int rewards)
    {
        this->insert_customer(id, name, rewards);
    }
    //these are utility functions to add customers and update points
    void update_points(l id, int tm)
    {
        b[id % 100].update_points(id, tm);
    }
    //these are the functions that give the information of a customer
    // customer info returns a pointer for the info of a customer
    customer *get_customer_info(l i)
    {
        customer *temp = b[i % 100].get_customer_info(i);
        return temp;
    }
    //this function returns the points of a customer based on his id
    int get_points(l id)
    {
        customer *temp = b[id % 100].get_customer_info(id);

        return temp->getrewardscustomer();
    }
};

//Billing list starts from here
struct node
{
    l id;
    string name;
    int rate;
    int quantity;
    int amount;
    node *next;
};
struct node *head_item_1;
//the data of each bill is stored in the form of a linked list and here is the implementation for the same
//each item is a node of the linklist
//for the bill
class item_1_list
{
    node *head, *tail;

public:
    item_1_list()
    {
        head = NULL;
        tail = NULL;
    }
    //simple linked list
    //this function adds the details of the item onto the node
    void add_node(l id, string name, int rate, int quantity, int amount)
    {
        node *temp = new node;
        temp->id = id;
        temp->name = name;
        temp->rate = rate;
        temp->quantity = quantity;
        temp->amount = amount;
        temp->next = NULL;
        //prinary condition
        if (head == NULL)
        {
            head = temp;
            tail = temp;
        }
        else
        {
            tail->next = temp;
            tail = tail->next;
        }
    }
    node *gethead()
    {
        return head;
    }
    //this is the function to display the entire list of the items bought
    static void display_item_1(node *head)
    {
        if (head == NULL)
        {
            return;
        }
        else
        {
            cout << head->id << "    ";
            cout << head->name << "    ";
            cout << head->rate << "    ";
            cout << "\t" << head->quantity << "   \t";
            cout << "   " << head->amount << "    " << endl;
            display_item_1(head->next);
        }
    }
    void delete_node(l id)
    {
        node *temp = head;
        //if the first item is the one is to be deleted
        if (head->id == id)
        {
            head_item_1 = head->next;
        }
        node *prev = new node;
        //any item in betweeen is to be deleted
        while (temp->id != id)
        {
            prev = temp;
            temp = temp->next;
        }
        prev->next = temp->next;
    }

    //these are the functions that relate the buying of the items with the updation of the stocks in the above inventory
    int getq(l pr)
    {
        node *temp = head;
        while (temp->id != pr)
        {
            temp = temp->next;
        }
        return temp->quantity;
    }
    int geta(l pr)
    {
        node *temp = head;
        while (temp->id != pr)
        {
            temp = temp->next;
        }
        return temp->amount;
    }
    //in case we want the entire amount of the bill
    int gettm(node *head)
    {
        int sum = 0;
        node *temp = head;
        while (temp)
        {
            sum = sum + temp->amount;
            temp = temp->next;
        }
        return sum;
    }
};
class manager
{
    l id;
    string name;

public:
    void insert_item(inventory i, l id, string name, int rate, int quantity)
    {
        i.insert_item(id, name, rate, quantity);
    }
    void insert_customer(customer_linkedlist c, l id, string name, int rewards)
    {
        c.insert_customer(id, name, rewards);
    }
    item *get_info_inventory(inventory i, l id)
    {
        item *temp = i.get_info_inventory(id);
        return temp;
    }
    customer *get_customer_info(l id)
    {
        customer *temp = this->get_customer_info(id);
        return temp;
    }
    void disp_node(l id, inventory i)
    {
        item *temp = i.get_info_inventory(id);
        cout << temp->getid() << "\t";
        cout << temp->getname() << "\t";

        cout << temp->getrate() << "\t";
        cout << temp->getquantity() << "\t";
    }
    void disp_c_node(l id, customer_linkedlist c)
    {
        customer *temp = c.get_customer_info(id);
        cout << "-----------------------------------------------------------------------------" << endl;

        cout << "Name\t\t"
             << "ID\t"
             << "Rewards\t" << endl;
        cout << temp->getidcustomer() << "\t";
        cout << temp->getnamecustomer() << "\t";
        cout << temp->getrewardscustomer() << "\t";
    }
    //display function is for items and the displayc function is used for the customer

    //the following functions are used only by the manager to actually invoke the original functions such that anyone else cannot use them
    void display(inventory i)
    {
        i.display();
    }
    void delete_item(inventory i, l id)
    {
        i.delete_item(id);
    }
    void displayc(customer_linkedlist c)
    {
        c.displayc();
    }
    void update_item(l id, string name, int rate, int quantity, inventory i)
    {
        i.update_item(id, name, rate, quantity);
    }
    void update_customer(l id, string name, int rew, customer_linkedlist c)
    {
        c.update_customer(id, name, rew);
    }
    void delete_customer(customer_linkedlist c, l id)
    {
        c.delete_customer(id);
    }
    void update_stock(inventory i, l id, int q)
    {
        i.update_stock(id, q);
    }
    void update_points(customer_linkedlist c, int tm, l id)
    {
        c.update_points(id, tm);
    }
};
item_1_list accessfordel;
//we need this for deletion of the objects
class billing : public inventory, public customer_linkedlist
{
    item_1_list p;
    l transaction_id;
    l customer_id;
    int ch;
    int t_amount;

public:
    billing()
    {
        transaction_id = 0;
        customer_id = 0;
        t_amount = 0;
    }
    billing(l tr_id, l c_id)
    {
        this->transaction_id = tr_id;
        this->customer_id = c_id;
    }

    //this function us for the utility if we want to operate from main and for the operation by the user we have made another function
    void buy_item_testcase(inventory a, customer_linkedlist c, l product_id, int quantity)
    {
        //we take the info of the particular product with the help of his id
        item *temp = a.get_info_inventory(product_id);
        //if it is a valid product id
        if (temp != NULL)
        {
            string nama = temp->getname();
            int ratea = temp->getrate();

            //now we add the product into the list
            p.add_node(product_id, nama, ratea, quantity, ratea * quantity);
            a.update_stock(product_id, quantity);
            head_item_1 = p.gethead();
            accessfordel = p;
        }
    }
    l buyitem(inventory a, customer_linkedlist c)
    {
        if (customer_id == 99)
        {
            //this is for adding the customer onto the list because he has bought something
            string naim;
            cout << "Please enter your name:";
            cin >> naim;
            //this new_id is the static id that we had created in the beginning for the adding
            c.add_customer(new_id, naim, 0);
            cout << "Your new id is :-";
            cout << new_id;
            new_id++;
        }
        int no_of_item;
        cout << "Enter number of items you want to purchase ";
        cin >> no_of_item;
        while (no_of_item--)
        {
            cout << "Please add product id you want to purchase:-";
            //this takes in the id and then grabs the element so that we can update the stock as per its quantity
            l prid;
            cin >> prid;
            item *temp = a.get_info_inventory(prid);
            if (temp != NULL)
            {
                string nama = temp->getname();
                int ratea = temp->getrate();
                int q;
                cout << "Enter quantity:-";
                cin >> q;
                p.add_node(prid, nama, ratea, q, ratea * q);
                a.update_stock(prid, q);
                p.display_item_1(p.gethead());
                head_item_1 = p.gethead();
                accessfordel = p;
            }
        }
        return new_id--;
    }
    void cancelitem(inventory a)
    {

        cout << "HERE IS LIST OF ITEMS PURCHASE :-" << endl;
        p.display_item_1(head_item_1);
        cout << "please enter the product id of item you dont want now:--" << endl;
        l prif;
        cin >> prif;
        int x = accessfordel.getq(prif);
        //this is just for updating the stock
        a.update_stock_item_deletion(prif, x);
        //this is to delete the node from the billing list of that particular item
        accessfordel.delete_node(prif);
        accessfordel.display_item_1(head_item_1);
    }
    void Make_Bill(l cid, customer_linkedlist cl, manager c)
    {
        cout << "----------------------------------------------------------" << endl;

        cout << "Your Transaction ID: " << ++transaction_id << "\t\t";
        cout << "Your Customer ID is: " << cid << endl;
        cout << "----------------------------------------------------------" << endl;

        cout << "Product ID   "
             << "  Name  "
             << "  Rate  "
             << "  Quantity  "
             << "  Amount   " << endl;

        cout << "----------------------------------------------------------" << endl;
        //cout << "Here is list of items you have bought:--" << endl;
        accessfordel.display_item_1(head_item_1);
        cout << "----------------------------------------------------------" << endl;

        int m = accessfordel.gettm(head_item_1);
        cout << "Your total amount is:--"
             << " " << m << endl;
        cout << "----------------------------------------------------------" << endl;

        c.update_points(cl, m, cid);

        cout << endl
             << endl
             << endl
             << "************Thanks for Visiting**************" << endl
             << endl
             << endl;
    }
};
int main()
{
    inventory inv;
    customer_linkedlist c;
    manager a;
    //this is the entire stash of the items that we have
    //their rates and their quantities
    item hashitem[100] = {
        {111100000001, "1", 100, 20},
        {111100000002, "2", 110, 20},
        {111100000003, "3", 120, 20},
        {111100000004, "4", 130, 20},
        {111100000005, "5", 140, 20},
        {111100000006, "6", 150, 20},
        {111100000007, "7", 160, 20},
        {111100000008, "8", 170, 20},
        {111100000009, "9", 180, 20},
        {111100000010, "10", 190, 20},
        {111100000011, "11", 200, 20},
        {111100000012, "12", 210, 20},
        {111100000013, "13", 220, 20},
        {111100000014, "14", 230, 20},
        {111100000015, "15", 240, 20},
        {111100000016, "16", 250, 20},
        {111100000017, "17", 260, 20},
        {111100000018, "18", 270, 20},
        {111100000019, "19", 280, 20},
        {111100000020, "20", 290, 20},
        {111100000021, "21", 300, 20},
        {111100000022, "22", 310, 20},
        {111100000023, "23", 320, 20},
        {111100000024, "24", 330, 20},
        {111100000025, "25", 340, 20},
        {111100000026, "26", 350, 20},
        {111100000027, "27", 360, 20},
        {111100000028, "28", 370, 20},
        {111100000029, "29", 380, 20},
        {111100000030, "30", 390, 20},
        {111100000031, "31", 400, 20},
        {111100000032, "32", 410, 20},
        {111100000033, "33", 420, 20},
        {111100000034, "34", 430, 20},
        {111100000035, "35", 440, 20},
        {111100000036, "36", 450, 20},
        {111100000037, "37", 460, 20},
        {111100000038, "38", 470, 20},
        {111100000039, "39", 480, 20},
        {111100000040, "40", 490, 20},
        {111100000041, "41", 500, 20},
        {111100000042, "42", 510, 20},
        {111100000043, "43", 520, 20},
        {111100000044, "44", 530, 20},
        {111100000045, "45", 540, 20},
        {111100000046, "46", 550, 20},
        {111100000047, "47", 560, 20},
        {111100000048, "48", 570, 20},
        {111100000049, "49", 580, 20},
        {222200001111, "50", 590, 20},
        {222200001114, "51", 600, 20},
        {222200001117, "52", 610, 20},
        {222200001120, "53", 620, 20},
        {222200001123, "54", 630, 20},
        {222200001126, "55", 640, 20},
        {222200001129, "56", 650, 20},
        {222200001132, "57", 660, 20},
        {222200001135, "58", 670, 20},
        {222200001138, "59", 680, 20},
        {222200001141, "60", 690, 20},
        {222200001144, "61", 700, 20},
        {222200001147, "62", 710, 20},
        {222200001150, "63", 720, 20},
        {222200001153, "64", 730, 20},
        {222200001156, "65", 740, 20},
        {222200001159, "66", 750, 20},
        {222200001162, "67", 760, 20},
        {222200001165, "68", 770, 20},
        {222200001168, "69", 780, 20},
        {222200001171, "70", 790, 20},
        {222200001174, "71", 800, 20},
        {222200001177, "72", 810, 20},
        {222200001180, "73", 820, 20},
        {222200001183, "74", 830, 20},
        {222200001186, "75", 840, 20},
        {222200001189, "76", 850, 20},
        {222200001192, "77", 860, 20},
        {222200001195, "78", 870, 20},
        {222200001198, "79", 880, 20},
        {222200001201, "80", 890, 20},
        {222200001204, "81", 900, 20},
        {222200001207, "82", 910, 20},
        {222200001210, "83", 920, 20},
        {222200001213, "84", 930, 20},
        {222200001216, "85", 940, 20},
        {222200001219, "86", 950, 20},
        {222200001222, "87", 960, 20},
        {222200001225, "88", 970, 20},
        {222200001228, "89", 980, 20},
        {222200001231, "90", 990, 20},
        {222200001234, "91", 1000, 20},
        {222200001237, "92", 1010, 20},
        {222200001240, "93", 1020, 20},
        {222200001243, "94", 1030, 20},
        {222200001246, "95", 1040, 20},
        {222200001249, "96", 1050, 20},
        {222200001252, "97", 1060, 20},
        {222200001255, "98", 1070, 20},
        {222200001258, "99", 1080, 20},
        {222200001261, "100", 1090, 20},
    };

    //this is the entire stash of the customers that have various id s and names that are taken main function traversal and not for user input traversal

    customer hashcustomer[100] = {
        {9400000001, "1", 0},
        {9400000002, "2", 0},
        {9400000003, "3", 0},
        {9400000004, "4", 0},
        {9400000005, "5", 0},
        {9400000006, "6", 0},
        {9400000007, "7", 0},
        {9400000008, "8", 0},
        {9400000009, "9", 0},
        {9400000010, "10", 0},
        {9400000011, "11", 0},
        {9400000012, "12", 0},
        {9400000013, "13", 0},
        {9400000014, "14", 0},
        {9400000015, "15", 0},
        {9400000016, "16", 0},
        {9400000017, "17", 0},
        {9400000018, "18", 0},
        {9400000019, "19", 0},
        {9400000020, "20", 0},
        {9400000021, "21", 0},
        {9400000022, "22", 0},
        {9400000023, "23", 0},
        {9400000024, "24", 0},
        {9400000025, "25", 0},
        {9400000026, "26", 0},
        {9400000027, "27", 0},
        {9400000028, "28", 0},
        {9400000029, "29", 0},
        {9400000030, "30", 0},
        {9400000031, "31", 0},
        {9400000032, "32", 0},
        {9400000033, "33", 0},
        {9400000034, "34", 0},
        {9400000035, "35", 0},
        {9400000036, "36", 0},
        {9400000037, "37", 0},
        {9400000038, "38", 0},
        {9400000039, "39", 0},
        {9400000040, "40", 0},
        {9400000041, "41", 0},
        {9400000042, "42", 0},
        {9400000043, "43", 0},
        {9400000044, "44", 0},
        {9400000045, "45", 0},
        {9400000046, "46", 0},
        {9400000047, "47", 0},
        {9400000048, "48", 0},
        {9400000049, "49", 0},
        {9400000050, "50", 0},
        {9400000051, "51", 0},
        {9400000052, "52", 0},
        {9400000053, "53", 0},
        {9400000054, "54", 0},
        {9400000055, "55", 0},
        {9400000056, "56", 0},
        {9400000057, "57", 0},
        {9400000058, "58", 0},
        {9400000059, "59", 0},
        {9400000060, "60", 0},
        {9400000061, "61", 0},
        {9400000062, "62", 0},
        {9400000063, "63", 0},
        {9400000064, "64", 0},
        {9400000065, "65", 0},
        {9400000066, "66", 0},
        {9400000067, "67", 0},
        {9400000068, "68", 0},
        {9400000069, "69", 0},
        {9400000070, "70", 0},
        {9400000071, "71", 0},
        {9400000072, "72", 0},
        {9400000073, "73", 0},
        {9400000074, "74", 0},
        {9400000075, "75", 0},
        {9400000076, "76", 0},
        {9400000077, "77", 0},
        {9400000078, "78", 0},
        {9400000079, "79", 0},
        {9400000080, "80", 0},
        {9400000081, "81", 0},
        {9400000082, "82", 0},
        {9400000083, "83", 0},
        {9400000084, "84", 0},
        {9400000085, "85", 0},
        {9400000086, "86", 0},
        {9400000087, "87", 0},
        {9400000088, "88", 0},
        {9400000089, "89", 0},
        {9400000090, "90", 0},
        {9400000091, "91", 0},
        {9400000092, "92", 0},
        {9400000093, "93", 0},
        {9400000094, "94", 0},
        {9400000095, "95", 0},
        {9400000096, "96", 0},
        {9400000097, "97", 0},
        {9400000098, "98", 0},
        {9400000099, "99", 0},
        {9400000100, "100", 0},
    };
    manager b;
    for (int i = 0; i < 100; i++)
    {
        b.insert_customer(c, hashcustomer[i].getidcustomer(), hashcustomer[i].getnamecustomer(), hashcustomer[i].getrewardscustomer());
    }
    for (int i = 0; i < 100; i++)
    {
        a.insert_item(inv, hashitem[i].getid(), hashitem[i].getname(), hashitem[i].getrate(), hashitem[i].getquantity());
    }
    static l transactio_id = 900;
    //here are the testcases for main input driven program

    cout << endl
         << endl
         << endl
         << " Welcome to our Supermarket" << endl
         << endl
         << endl;
    billing h(transactio_id, 9400000012);
    h.buy_item_testcase(inv, c, 111100000011, 5);
    h.buy_item_testcase(inv, c, 111100000038, 15);
    h.buy_item_testcase(inv, c, 222200001126, 17);
    h.Make_Bill(9400000012, c, b);
    cout << "AFter Billing is callled" << endl;
    cout << endl
         << endl;
    cout << "Customer info:-" << endl;
    b.disp_c_node(9400000012, c);
    cout << endl;
    cout << endl
         << endl;
    cout << "Updated Inventory" << endl;
    cout << "Product_ID\t"
         << "Name\t"
         << "Rate\t"
         << "Quantity\t" << endl;
    a.disp_node(111100000011, inv);
    cout << endl;
    a.disp_node(111100000038, inv);
    cout << endl;
    a.disp_node(222200001126, inv);
    cout << endl;
    cout << endl
         << endl
         << endl
         << "************Thanks for Visiting**************" << endl
         << endl
         << endl;
    cout << endl
         << endl
         << endl
         << " Welcome to our Supermarket" << endl
         << endl
         << endl;

    billing h2(transactio_id, 9400000017);
    h2.buy_item_testcase(inv, c, 111100000017, 5);
    h2.buy_item_testcase(inv, c, 111100000018, 12);
    h2.buy_item_testcase(inv, c, 111100000019, 13);
    h.Make_Bill(9400000017, c, b);
    cout << "AFter Billing is callled" << endl;
    cout << endl
         << endl;
    cout << "Customer info:-" << endl;
    b.disp_c_node(9400000017, c);
    cout << endl;
    cout << endl
         << endl;
    cout << "Updated Inventory" << endl;
    cout << "Product_ID\t"
         << "Name\t"
         << "Rate\t"
         << "Quantity\t" << endl;
    a.disp_node(111100000017, inv);
    cout << endl;
    a.disp_node(111100000018, inv);
    cout << endl;
    a.disp_node(111100000019, inv);
    cout << endl;
    cout << endl
         << endl
         << endl
         << "************Thanks for Visiting**************" << endl
         << endl
         << endl;

    l cid, mid, x_id, f_id, d_id;
    //These are the elements to be used in the switch case
    int in;
    int check = 0;
    cout << "If you want menu driven program press 5" << endl;
    cin >> check;
    if (check == 5)
    {
    come_here:
        cout << "Press 1 for manager and 2 for custoemer" << endl;
        cin >> in;
        if (in == 2)
        {
        go_there:
            while (1)
            {
                int ch = 0;
                cout << "Please enter your customer ID, in case you are a new customer then press 99" << endl;
                cin >> cid;
                while (1)
                {
                    cout << endl;
                    cout << "Enter 1 to buy item" << endl;
                    cout << "Enter 2 to delete item" << endl;
                    cout << "Enter 3 to make bill" << endl;
                    cout << "Enter 0 to exit" << endl;
                    cin >> ch;
                    billing u(++transactio_id, cid);
                    switch (ch)
                    {
                    case 1:
                    {

                        u.buyitem(inv, c);
                        break;
                    }
                    case 2:
                    {
                        u.cancelitem(inv);

                        break;
                    }
                    case 3:
                    {
                        if (cid == 99)
                        {
                            u.Make_Bill(new_id--, c, b);
                        }
                        else
                            u.Make_Bill(cid, c, b);
                        goto go_there; //after making the new id he is again asked what operation he wants to do whether buy or delete or make bill
                        break;
                    }
                    case 0:
                    {
                        goto come_here;
                    }

                    default:
                        break;
                    }
                }
            }
        }
    }
    l correct_id = 202001156;
    string correct_name = "Ayush";
    string m_name;
    if (in == 1)
    {
        int k;
        cout << "The correct Id and Name for the manager are 202001156 and Ayush" << endl;
        cout << "Please enter the correct ID and then Name for the manager" << endl;
        cin >> mid;
        cin >> m_name;
        l temp_id;
        string temp_name = "0";
        int temp_rate;
        int temp_quantity;
        l h_id = 0;
        string h_name = "0";
        int h_rew = 0;
        if (mid == correct_id && m_name == correct_name)
        {
            cout << "Enter 1 to update item" << endl
                 << "Enter 2 to update customer" << endl
                 << "Enter 3 to delete customer" << endl
                 << "Enter 4 to delete item" << endl
                 << "Enter 5 to display item node" << endl
                 << "Enter 6 to display customer" << endl
                 << "Enter 7 to exit";
            cin >> k;
            switch (k)
            {
            case 1:
            {
                cout << "Enter the item id" << endl;
                cin >> temp_id;
                cout << "Enter the product name" << endl;
                cin >> temp_name;
                cout << "Enter rate" << endl;
                cin >> temp_rate;
                cout << "Enter the quantity" << endl;
                cin >> temp_quantity;
                a.update_item(temp_id, temp_name, temp_rate, temp_quantity, inv);
                break;
            }
            case 2:
            {
                cout << "Enter the id of customer you want to update" << endl;
                cin >> h_id;
                cout << "Enter the name of the customer" << endl;
                cin >> h_name;
                cout << "Enter the rewards" << endl;
                cin >> h_rew;
                cout << endl;
                b.update_customer(h_id, h_name, h_rew, c);
                //here we see that the manager b is changing the data while in the case 1 manager a was changing the data but all the  managers are having the change done by one simultaneously
                break;
            }
            case 3:
            {
                cout << "ENter the customer ID you want to delete" << endl;
                cin >> x_id; //this element is taken for safety, it is not needed we can navigate from temp_id too
                b.delete_customer(c, x_id);

                break;
            }
            case 4:
            {
                cout << "Enter the product ID you want to delete" << endl;
                cin >> f_id;
                a.delete_item(inv, f_id);
                break;
            }
            case 5:
            {
                cout << "Enter the product id you want to display" << endl;
                cin >> f_id;
                cout << "Product_ID\t"
                     << "Name\t"
                     << "Rate\t"
                     << "Quantity\t" << endl;
                a.disp_node(f_id, inv);
                break;
            }
            case 6:
            {
                cout << "Enter the customer id you want to display" << endl;
                cin >> d_id;
                b.disp_c_node(d_id, c);
                cout << endl;
                break;
            }
            case 7:
                goto go_there;
            default:
                break;
            }
        }
    }
    return 0;
}