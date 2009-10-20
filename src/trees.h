#ifndef TREES_H_INCLUDED
#define TREES_H_INCLUDED

#include <iostream>
#include "lists.h"
#include "main.h"

namespace ag
{

template<typename T>
class tree
{
public:
    T data;
    ag::list<tree*> childs;
    tree* parent;
    int iCount;

    tree(T d);
    tree(tree* p,T d);
    ~tree();
    tree* addchild(T d);
    void addchild_ptr(tree* ptr);
    void delchild(tree* c);
    void delallchilds();
    void delchild_r(tree* c);//recursively
    void drawtree_con(std::ostream* o,int depth=0);
};

template<typename T>
tree<T>::tree(T d)
    {
        data=d;
        parent=NULL;
    }

template<typename T>
tree<T>::tree(tree* p,T d)
    {
        std::cout<<"tree::tree()\n";
        p->addchild_ptr(this);
        parent=p;
        data=d;
    }

template<typename T>
    tree<T>::~tree()
    { }

template<typename T>
    tree<T>* tree<T>::addchild(T d)
    {
        tree<T>* c=new tree(this,d);
        return c;
    }

template<typename T>
    void tree<T>::addchild_ptr(tree<T>* ptr)
    {
        childs.add_tail(ptr);
    }

template<typename T>
    void tree<T>::delchild(tree* c)
    {
        for(listmember< tree<T>* >* k=childs.head;k!=NULL;k=k->next)
        {
            if (c==k->data)
            {
                childs.del(k);
                break;
            }
        }
    }

template<typename T>
    void tree<T>::delallchilds()
    {
        listmember< tree<T>* >* p=childs.head;
        listmember< tree<T>* >* k;
        while (p!=childs.tail)
        {
            k=p->next;
            childs.del(p);
            p=k;
        }
    }

template<typename T>
    void tree<T>::delchild_r(tree* c)//recursively
    {
        for(listmember< tree<T>* >* k=childs.head;k!=NULL;k=k->next)
        {
            if (c==k->data)
            {
                k->data->delallchilds();
                childs.del(k);
                break;
            }
        }
    };

template<typename T>
    void tree<T>::drawtree_con(std::ostream* o,int depth)
    {
        for(int i=0;i<depth;i++)
            *o<<"| ";
//		char* s =this->data;
        char ss[255];
        sprintf(ss,"t%d;%s;%s;%s;%s",this->data->tntType,(this->data->text==NULL)?"":this->data->text,
                (this->data->text2==NULL)?"":this->data->text2,(this->data->text3==NULL)?"":this->data->text3,
                (this->data->text4==NULL)?"":this->data->text4);


        (*o) << ss << "\n";
        for(listmember< tree<T>* >* k=childs.head;k!=NULL;k=k->next)
        {
            k->data->drawtree_con(o,depth+1);
        }
    };

};

#endif // TREES_H_INCLUDED
