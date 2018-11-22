#include "test.h"

class Father
{
public:
    char name[32];

    Father(char name[])
    {
        strncpy(this->name,name,10);
    }
    void fun1()
    {
        cout<<"this is Father.fun1"<<endl;
    }

    void fun2()
    {
        cout<<"this is Father.fun2"<<endl;
    }

    virtual void fun3(int a)
    {
        cout<<"this is Father.fun3"<<endl;
    }
};

class Mather
{
public:
    char name[32];

    Mather(char name[])
    {
        strncpy(this->name,name,10);
    }
    void fun1()
    {
        cout<<"this is Mather.fun1"<<endl;
    }

    void fun2()
    {
        cout<<"this is Mather.fun2"<<endl;
    }

    virtual void fun3(int a)
    {
        cout<<"this is Mather.fun3"<<endl;
    }
};

class Child:public Father,public Mather
{
public:
    char name[32];

    Child (char fname[],char mname[],char name[]):
        Father(fname),
        Mather(mname)
    {
        strncpy(this->name,name,10);
    }
    void fun0()
    {
        cout<<"this is Child.fun1"<<endl;
    }

    void fun2()
    {
        cout<<"this is Child.fun2"<<endl;
    }

    virtual void fun3(int a)
    {
        cout<<"this is Child.fun31"<<endl;
    } 

};


void test()
{
   
    Child *c= new Child("wzq","bhm","whf");
    c->fun0(); //this is Child.fun1   --子类的
    //c->fun1(); //this is Father.fun1  --继承的 不知道继承那个的
    c->fun2(); //this is Child.fun2   --子类重写的
    c->fun3(1); //this is Child.fun31   --子类重写的

    cout<<"====================================="<<endl;
    Father *f = c;
    //f->fun0(); //'class Father' has no member named 'fun0'
    f->fun1(); //this is Father.fun1  --父类的
    f->fun2(); //this is Father.fun2   --父类的
    f->fun3(1); //this is Child.fun31   --多态，子类的
    
}
