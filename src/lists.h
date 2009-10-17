#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

#include <cstdlib>

namespace ag
{

template<typename T>
struct listmember
{
    listmember* next;
    listmember* prev;
    T data;
};

template<typename T>
class list
{
    public:
        list();
        ~list();

        void delall();

        listmember<T>* add_head(T data);
        listmember<T>* add_tail(T data);
        T& del(listmember<T>* e);
        listmember<T>* create_first(T data);
        listmember<T>* find(T d);
        typedef listmember< T >* member;
        listmember< T >* operator[](int i);
        member head;
        member tail;
        int count();
        bool empty(){ return this->tail==NULL; }
};

template<typename T>
  list<T>::list()
        {
            head=0;
            tail=0;
        };

template<typename T>
  list<T>::~list()
        {
            member p=head;
            member k;
            while (p!=0)
            {
                k=p->next;
                del(p);
                p=k;
            }
        };

template<typename T>
  void list<T>::delall()
        {
            member p=head;
            member k;
            while (p!=0)
            {
                k=p->next;
                del(p);
                p=k;
            }
        };

template<typename T>
  listmember<T>* list<T>::add_head(T data)
        {
            if (head==0)
                return create_first(data);
            else
            {
                listmember<T>* e=new listmember<T>;
                e->data=data;
                e->prev=0;
                e->next=head;
                head->prev=e;
                head=e;
                return e;
            }
        };

template<typename T>
  listmember<T>* list<T>::add_tail(T data)
        {
            if (tail==0)
                return create_first(data);
            else
            {
                listmember<T>* e=new listmember<T>;
                e->data=data;
                e->next=0;
                e->prev=tail;
                tail->next=e;
                tail=e;
                return e;
            }
        };

template<typename T>
  T& list<T>::del(listmember<T>* e)
        {
            if (!e) throw -1;

            T* k=new T;
            *k=e->data;
            if (e->prev)
                e->prev->next=e->next;
            else
                head=e->next;
            if (e->next)
               e->next->prev=e->prev;
            else
                tail=e->prev;
            delete e;
            return *k;
        };

template<typename T>
  listmember<T>* list<T>::create_first(T data)
        {
            listmember<T>* e=0;
            if ((head==0)&&(tail==0))
            {
                e=new listmember<T>;
                e->data=data;
                e->next=0;
                e->prev=0;
                head=tail=e;
            }
            return e;
        };

template<typename T>
  listmember<T>* list<T>::find(T d)
        {
            member p=head;
            while (p!=NULL)
            {
                if (p->data==d)
                    return p;
                p=p->next;
            }
            return 0;
        };

template<typename T>
  int list<T>::count()
        {
            member p=head;
            int i;
            while (p!=NULL)
            {
                i++;
                p=p->next;
            }
            return i-1;
        };

template<typename T>
  listmember<T>* list<T>::operator[](int i)
        {
            member p=head;
            int k=0;
            while (p!=0)
            {
                if (k==i)
                    return p;
                p=p->next;
                k++;
            }
            return 0;
        }

};

#endif // LISTS_H_INCLUDED
