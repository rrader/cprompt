#ifndef SETS_H_INCLUDED
#define SETS_H_INCLUDED

namespace ag
{

#include <string.h>

class set
{
    public:
        char* _marr;

        set(char* s)
        {
            _marr=new char[256];
            memset(_marr,0,256);
            include_s(s);
        }
        set(set& s)
        {
            _marr=new char[256];
            memcpy(_marr,s._marr,256);
        }
        set()
        {
            _marr=new char[256];
            memset(_marr,0,256);
        }
        ~set()
        {
            delete[] _marr;
        }
        void include_c(char c)
        {
            _marr[c]=1;
        }
        void exclude_c(char c)
        {
            _marr[c]=0;
        }
        void include_s(const char* c)
        {
            int len=strlen(c);
            for(int i=0;i<len;i++)
            {
                _marr[c[i]]=1;
            }
        }
        void exclude_s(const char* c)
        {
            int len=strlen(c);
            for(int i=0;i<len;i++)
                _marr[c[i]]=0;
        }
        void include(const char* s)
        {
            for(int i=0;i<256;i++)
            {
                if (s[i]==1)
                    _marr[i]=1;
            }
        }
        void exclude(const char* s)
        {
            for(int i=0;i<256;i++)
            {
                if (s[i]==1)
                    _marr[i]=0;
            }
        }
        void intersection_s(const char* c)
        {
            char* tmp=new char[256];
            memset(tmp,0,256);
            int len=strlen(c);
            for(int i=0;i<len;i++) tmp[c[i]]=1;

            for(int i=0;i<256;i++)
            {
                if(!(((_marr[i]==1)&&(tmp[i]==1))))
                    tmp[i]=0;
            }
            memcpy(_marr,tmp,256);
            delete[] tmp;
        }
        void intersection(const char* s)
        {
            for(int i=0;i<256;i++)
            {
                if (!((s[i]==1)&&(_marr[i]==1)))
                    _marr[i]=0;
            }
        }
        void symmetricdifference_s(char* c)
        {
            set s1(*this),s2(c);
            _marr=((s1-s2)+(s2-s1))._marr;
        }
        void  symmetricdifference(const char* s)
        {
            for(int i=0;i<256;i++)
            {
                _marr[i]=((s[i]==1)^(_marr[i]==1))?1:0;
            }
        }
        char* getstr()
        {
            char* s=new char[256]; memset(s,0,256);
            int l=0;
            for(int i=0;i<256;i++)
            {
                if (((int)_marr[i])==1)
                {
                    s[l]=i;
                    l++;
                }
            };
            return s;
        }
        bool present(char c)
        {
            return (_marr[c]==1);
        }
        set& operator+(set& __sec)
        {
            set* ret=new set(*this);
            *ret+=__sec;
            return *ret;
        }
        set& operator-(set& __sec)
        {
            set* ret=new set(*this);
            *ret-=__sec;
            return *ret;
        }

        set& operator+(char* __sec)
        {
            set* ret=new set(*this);
            *ret+=__sec;
            return *ret;
        }
        set& operator-(char* __sec)
        {
            set* ret=new set(*this);
            *ret-=__sec;
            return *ret;
        }

        set& operator+(char __sec)
        {
            set* ret=new set(*this);
            *ret+=__sec;
            return *ret;
        }
        set& operator-(char __sec)
        {
            set* ret=new set(*this);
            *ret-=__sec;
            return *ret;
        }

        set& operator*(set& __sec)
        {
            set* ret=new set(*this);
            *ret*=__sec;
            return *ret;
        }
        set& operator/(set& __sec)
        {
            set* ret=new set(*this);
            *ret/=__sec;
            return *ret;
        }
        set& operator+=(set& __sec)
        {
            this->include(__sec._marr);
            return *this;
        }
        set& operator-=(set& __sec)
        {
            this->exclude(__sec._marr);
            return *this;
        }

        set& operator+=(char* __sec)
        {
            this->include_s(__sec);
            return *this;
        }
        set& operator-=(char* __sec)
        {
            this->exclude_s(__sec);
            return *this;
        }

        set& operator+=(char __sec)
        {
            this->include_c(__sec);
            return *this;
        }
        set& operator-=(char __sec)
        {
            this->exclude_c(__sec);
            return *this;
        }

        set& operator*=(set& __sec)
        {
            this->intersection(__sec._marr);
            return *this;
        }
        set& operator/=(set& __sec)
        {
            this->symmetricdifference(__sec._marr);
            return *this;
        }
        void operator=(set& __sec)
        {
            memcpy(_marr,__sec._marr,256);
        }
        void operator=(char* __sec)
        {
            memset(_marr,0,256);
            include_s(__sec);
        }
        bool operator%(char* __sec)
        {
            set* m=&(set(__sec)-(*this));
            bool flag=false;
            for(int i=0;i<256;i++) flag=flag||(m->_marr[i]==1);
            return (!flag);
        }
        bool operator%(char __sec)
        {
            return (_marr[__sec]==1);
        }
};

}

#endif // SETS_H_INCLUDED
