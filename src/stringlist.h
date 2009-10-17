#ifndef STRINGLIST_H_INCLUDED
#define STRINGLIST_H_INCLUDED

#include "lists.h"
#include "string.h"

namespace ag
{

class stringlist : public ag::list<char*>
{
    public:
        stringlist():list<char*>() {};

        stringlist(char*ar[],int count)
        {
            for(int i=0;i<count;i++) addstr(ar[i]);
        };

        void addstr(char* s)
        {
            char* c=new char[strlen(s)];
            strcpy(c,s);
            add_tail(c);
        };
        member findstr(char* s)
        {
            member p=head;
            while (p!=NULL)
            {
                if (strcmp(p->data,s)==0)
                    return p;
                p=p->next;
            }
            return NULL;
        };
        char* makestrfromelements()
        {
            int iSz=0;
            for(member sm=head;sm!=NULL;sm=sm->next)
                iSz+=strlen(sm->data);
            char* s=new char[iSz+1];
            int iSz2=0;
            for(member sm=head;sm!=NULL;sm=sm->next)
            {
                for(int i=0;i<strlen(sm->data);i++)
                    s[i+iSz2]=sm->data[i];
                iSz2+=strlen(sm->data);
            }
            s[iSz]=0;
            return s;
        };
    private:
};


}

#endif // STRINGLIST_H_INCLUDED
