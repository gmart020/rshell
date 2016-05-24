using namespace std;

class connector
{
    public:
    connector();
    connector* left;
    connector* right;
    bool successful=false; //did commands execute
    virtual bool evaluate();
};

class ampersand: public connector
{
    public:
    ampersand();

    bool evaluate()
    {
        bool doleft=left->evaluate();
       
        if(doleft)
        {
            bool doright=right->evaluate();
        }

        if(doright)
        {
            successful=true;
        }

        return successful;
    }
};

class pipe: public connector
{
    public:
    pipe();

    bool evaluate()
    {
        bool doleft=left->evaluate();
       
        if(!doleft)
        {
            bool doright=right->evaluate();
        }

        if(doright || doleft)
        {
            successful=true;
        }

        return successful;
    }
};

class always: public connector
{
    public:
    always();

    bool evaluate()
    {
        bool something=left->evaluate();
        if(right->evaluate())
        {
            successful = true;
        }

        return successful;
    }
};

class commands: public connector
{
    public:
    commands();
    string args;

    bool evaluate()
    {
        //execvp goes here
        return successful;
    }
};
