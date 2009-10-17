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

CPRTreeNode* MakeCPRTreeNode(CPRTreeNodeType tp, char* text,char*text2,char*text3,char*text4)
{
    CPRTreeNode* k=new CPRTreeNode;
    if (text!=NULL)
    {
        k->text=new char[strlen(text)];
        strcpy(k->text,text);
    } else
    {
        k->text=NULL;
    }
    k->tntType=tp;
    if (text2!=NULL)
    {
        k->text2=new char[strlen(text2)];
        strcpy(k->text2,text2);
    } else
    {
        k->text2=NULL;
    }
    if (text3!=NULL)
    {
        k->text3=new char[strlen(text3)];
        strcpy(k->text3,text3);
    } else
    {
        k->text3=NULL;
    }
    if (text4!=NULL)
    {
        k->text4=new char[strlen(text4)];
        strcpy(k->text4,text4);
    } else
    {
        k->text4=NULL;
    }
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
        k->text2=new char[strlen(text2)];
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
//    MakePostfixFromInfix("a=sin(8+2*9+bn)");
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
    for(ag::list<CPRTokenInfo>::member p=App.aTokens.head;p!=App.aTokens.tail;p=p->next)
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
