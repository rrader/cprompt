class CPRApplication;

int argc;
char** argv;
CPRApplication* AppV;
#include <fstream>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory.h>
bool debugmode;
int argnum;

#include "stringlist.h"
#include "main.h"
#include "cprapp.h"
#include "cprtypes.h"

bool po_a,pi_a;

bool FileExists(char* strFilename)
{
    struct stat stFileInfo;
    bool blnReturn;
    int intStat;

    intStat = stat(strFilename,&stFileInfo);
    if(intStat == 0)
    {
        blnReturn = true;
    } else
    {
        blnReturn = false;
    }

    return(blnReturn);
}

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
    if (debugmode) std::cout<<"MakeCPRTreeNode("<<tp<<", ";
    if (debugmode) std::cout<<((text==NULL)?"":text);
    if (debugmode) std::cout<<", ";
    if (debugmode) std::cout<<((text2==NULL)?"":text2);
    if (debugmode) std::cout<<", ";
    if (debugmode) std::cout<<((text3==NULL)?"":text3);
    if (debugmode) std::cout<<", ";
    if (debugmode) std::cout<<((text4==NULL)?"":text4);
    if (debugmode) std::cout<<")\n";

    CPRTreeNode* k=new CPRTreeNode;

    int i1=0,i2=0,i3=0,i4=0;
    k->tntType=tp;
    char *s1,*s2,*s3,*s4;
    if (text4!=NULL)
    {
        //k->text4
        i4=strlen(text4);
        s4=new char[i4+1];
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
        s3=new char[i3+1];
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
        s2=new char[i2+1];
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
        s1=new char[i1+1];
        //int k=0; while(s1[k++]=text[k]);
//        strcpy(s1,text);
    } else
    {
        s1=NULL;
    };
    if (debugmode) std::cout<<"Lendths: "<<i1<<", "<<i2<<", "<<i3<<", "<<i4<<"\n";
    int* q=new int; //лучше не трогать

    if (i1!=0) { strcpy(s1,text); s1[i1]=0;}
    if (i2!=0) { strcpy(s2,text2); s2[i2]=0;}
    if (i3!=0) { strcpy(s3,text3); s3[i3]=0;}
    if (i4!=0) { strcpy(s4,text4); s4[i4]=0;}
    k->text=s1;
    k->text2=s2;
    k->text3=s3;
    k->text4=s4;

    k->r1=rx1;
    k->r2=rx2;
    k->r3=rx3;

//    q=new int; //лучше не трогать
    return k;
}

void FillCPRTreeNode(CPRTreeNode* k,CPRTreeNodeType tp, char* text,char*text2,char*text3,char*text4)
{
    if (text!=NULL)
    {
        k->text=new char[strlen(text)+1];
        strcpy(k->text,text);
        k->text[strlen(text)]=0;
    }
    k->tntType=tp;
    if (text2!=NULL)
    {
        int i=strlen(text2);
        k->text2=new char[i+1];
        strcpy(k->text2,text2);
        k->text2[i]=0;
    }
    if (text3!=NULL)
    {
        k->text3=new char[strlen(text3)+1];
        strcpy(k->text3,text3);
        k->text3[strlen(text3)]=0;
    }
    if (text4!=NULL)
    {
        k->text4=new char[strlen(text4)];
        strcpy(k->text4,text4);
        k->text4[strlen(text4)]=0;
    }
}

void replace(std::string& text, std::string s, std::string d)
{
  for(unsigned index=0; index=text.find(s, index), index!=std::string::npos;)
  {
    text.replace(index, s.length(), d);
    index+=d.length();
  }
}

//int f_argc;
//char** f_argv;
int main(int argc,char* argv[])
{
//    if (debugmode) std::cout<<"Sizeof DTVar: "<<sizeof(DTVar)<<"\n";
debugmode=false;
    ::argc=argc;
	::argv=argv;
	CPRApplication App;
	::AppV=&App;
	argnum=1;
	pi_a=false;
	po_a=false;
    if (argc<=1)
    {
        std::cout<<"Usage: cprompt \"/path/to/your/script.c\"\n";
        return 0;
    }
    while (true)
    {
        if (strcmp(argv[argnum],"--dbg")==0)
        {
            debugmode=true;
            argnum++;
            if (argc<=argnum)
            {
                std::cout<<"Usage: cprompt --dbg \"/path/to/your/script.c\"\n";
                return 0;
            }
        }else

        if (strcmp(argv[argnum],"--pi")==0)
        {
            pi_a=true;
            argnum++;
            if (argc<=argnum)
            {
                std::cout<<"Usage: cprompt --pi \"/path/to/named/pipe\" \"/path/to/your/script.c\"\n";
                return 0;
            }
            freopen(argv[argnum],"r",stdin);
            argnum++;
        }else

        if (strcmp(argv[argnum],"--po")==0)
        {
            po_a=true;
            argnum++;
            if (argc<=argnum)
            {
                std::cout<<"Usage: cprompt --po \"/path/to/named/pipe\" \"/path/to/your/script.c\"\n";
                return 0;
            }
            //po.open(argv[argnum]);
            //std::cout.rdbuf(po.rdbuf());
            freopen(argv[argnum],"w",stdout);
            argnum++;
        }else
        {
            break;
        }
    }

    if (!FileExists(argv[argnum]))
    {
        std::cout<<"(FATAL ERROR) File not found\n";
    }
    if (debugmode) std::cout<<"Start...\n";

    if (debugmode) std::cout << "Hello at the CPrompt's interpreter p_argc=" << argc-1 << "\n";
    if (debugmode) std::cout << "Executing the c-script: " << argv[argnum] << "\n";

    try
    {

        App.SetFile(argv[argnum]);
        App.HashBang();
        if (debugmode) std::cout<<"1. Pre-parsing\n";
        App.ParseIt(&App.aTokens,App.GetCurrentFileText(),true,true,true);
        for(ag::list<CPRTokenInfo>::member p=App.aTokens.head;p!=NULL;p=p->next)
            if (debugmode) std::cout << p->data.sCurrText << ": " << p->data.petCurrType << "; \n";
        //if (debugmode) std::cout<<"\n\n";

        if (debugmode) std::cout<<"2. Preprocessing\n";
        App.Preprocessing(NULL, App.GetCurrentFileText(),App.GetWorkDir());
        if (debugmode) std::cout<<App.GetCurrentFileText()<<"\n";
        if (debugmode) std::cout<<"3. Post-parsing\n";
        App.aTokens.delall();
        App.ParseIt(&App.aTokens,App.GetCurrentFileText(),false,false);

        for(ag::list<CPRTokenInfo>::member p=App.aTokens.head;p!=NULL;p=p->next)
        {
            if (p->data.petCurrType==petQuotedStr)
            {
                std::string tms;
                tms=p->data.sCurrText;
                replace(tms,"\\n","\n");
                replace(tms,"\\t","\t");
                replace(tms,"\\v","\v");
                replace(tms,"\\b","\b");
                replace(tms,"\\r","\r");
                replace(tms,"\\f","\f");
                replace(tms,"\\a","\a");
                replace(tms,"\\\\","\\");
                replace(tms,"\\?","\?");
                replace(tms,"\\\'","\'");
                replace(tms,"\\\"","\"");
                replace(tms,"\\0","\0");
                char* tmsp=new char[tms.size()+1];
                strcpy(tmsp,tms.c_str());
                tmsp[tms.size()]=0;
                p->data.sCurrText=tmsp;
                if (debugmode) std::cout<<tms<<"\n";
            }
        }

        if (debugmode) std::cout<<"4. Building the main tree\n";
        App.BuildTree(App.GetWorkDir(), &App.aTokens, App.GetCurrentFileText(), NULL);
        if (debugmode) std::cout<<"Tree was built:\n";
        if (debugmode) App.aTree->drawtree_con(&std::cout);
    //3. Execute main()
        if (debugmode) std::cout<<"\n";
        if (debugmode) std::cout<<"5. Execute main() \n";
        App.ExecMainTree(App.aTree);
    }catch(...)
    {
        std::cout.flush();
    }
}
