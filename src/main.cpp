class CPRApplication;

int argc;
char** argv;
CPRApplication* AppV;
#include <iostream>
#include <fstream>
#include <string.h>
#include <fstream>
#include "stringlist.h"
#include "main.h"
#include "cprapp.h"
#include "cprtypes.h"

char* concat(char* s1,char* s2)
{
    int j=strlen(s1);
    int o=strlen(s2);
    char* S=new char[j+o+1];
    for(int i=0;i<strlen(s1);i++)
        S[i]=s1[i];
    for(int i=0;i<strlen(s2);i++)
        S[i+strlen(s1)]=s2[i];
    S[strlen(s1)+strlen(s2)]=0;
    return S;
}

CPRTreeNode::operator char*()
{
    char* ss=new char[255];
    sprintf(ss,"t%d;%s;%s;%s;%s",tntType,(text==NULL)?"":text,(text2==NULL)?"":text2,(text3==NULL)?"":text3,(text4==NULL)?"":text4);
    return ss;
}

CPRTreeNode* MakeCPRTreeNode(CPRTreeNodeType tp, char* text,char*text2,char*text3,char*text4,void* rx1,void* rx2,void* rx3)
{
    std::cout<<"MakeCPRTreeNode("<<tp<<", ";
    std::cout<<((text==NULL)?"":text);
    std::cout<<", ";
    std::cout<<((text2==NULL)?"":text2);
    std::cout<<", ";
    std::cout<<((text3==NULL)?"":text3);
    std::cout<<", ";
    std::cout<<((text4==NULL)?"":text4);
    std::cout<<")\n";

    CPRTreeNode* k=new CPRTreeNode;

    int i1=0,i2=0,i3=0,i4=0;
    k->tntType=tp;
    char *s1,*s2,*s3,*s4;
    if (text4!=NULL)
    {
        //k->text4
        i4=strlen(text4);
        s4=new char[i4];
        //int k=0; while(s4[k++]=text4[k]);
//        strcpy(s4,text4);
    } else
    {
        s4=NULL;
    };
    if (text3!=NULL)
    {
        //k->text3
        i3=strlen(text3);
        s3=new char[i3];
        //int k=0; while(s3[k++]=text3[k]);
//        strcpy(s3,text3);
    } else
    {
        s3=NULL;
    };
    if (text2!=NULL)
    {
        //k->text2
        i2=strlen(text2);
        s2=new char[i2];
        //int k=0; while(s2[k++]=text2[k]);
//        strcpy(s2,text2);
    } else
    {
        s2=NULL;
    };
    if (text!=NULL)
    {
        //k->text
        i1=strlen(text);
        s1=new char[i1];
        //int k=0; while(s1[k++]=text[k]);
//        strcpy(s1,text);
    } else
    {
        s1=NULL;
    };
    std::cout<<"Lendths: "<<i1<<", "<<i2<<", "<<i3<<", "<<i4<<"\n";
    int* q=new int;

    if (i1!=0) strcpy(s1,text);
    if (i2!=0) strcpy(s2,text2);
    if (i3!=0) strcpy(s3,text3);
    if (i4!=0) strcpy(s4,text4);
    k->text=s1;
    k->text2=s2;
    k->text3=s3;
    k->text4=s4;

    k->r1=rx1;
    k->r2=rx2;
    k->r3=rx3;

    q=new int;
    return k;
}


void FillCPRTreeNode(CPRTreeNode* k,CPRTreeNodeType tp, char* text,char*text2,char*text3,char*text4)
{
    if (text!=NULL)
    {
        k->text=new char[strlen(text)];
        strcpy(k->text,text);
    }
    k->tntType=tp;
    if (text2!=NULL)
    {
        int i=strlen(text2);
        k->text2=new char[i];
        strcpy(k->text2,text2);
    }
    if (text3!=NULL)
    {
        k->text3=new char[strlen(text3)];
        strcpy(k->text3,text3);
    }
    if (text4!=NULL)
    {
        k->text4=new char[strlen(text4)];
        strcpy(k->text4,text4);
    }
}

//int f_argc;
//char** f_argv;
int main(int argc,char* argv[])
{
    std::cout<<"\n";
    std::cout<<"Sizeof DTVar: "<<sizeof(DTVar)<<"\n";
    ::argc=argc;
	::argv=argv;
	CPRApplication App;
	::AppV=&App;
    std::cout << "Hello at the CPrompt's interpreter p_argc=" << argc-1 << "\n";
    std::cout << "Executing the c-script: " << argv[1] << "\n";

	if (argc<=1)
	{
		std::cout<<"Usage: cprompt \"/path/to/your/cprompt/script.cp\"\n";
		return 0;
	}
    App.SetFile(argv[1]);
	std::cout<<"1. Parsing\n";
    App.ParseIt();
    for(ag::list<CPRTokenInfo>::member p=App.aTokens.head;p!=NULL;p=p->next)
        std::cout << p->data.sCurrText << ": " << p->data.petCurrType << "; ";
	std::cout<<"\n\n";
	std::cout<<"2. Building the main tree\n";
    App.BuildTree();
	std::cout<<"Tree was built:\n";
    App.aTree->drawtree_con(&std::cout);
//3. Execute main()
	std::cout<<"\n";
	std::cout<<"3. Execute main() \n";
	App.ExecMainTree(App.aTree);
}
