#include <string>
#include <vector>
#include <fstream>
using namespace std;

class errorMags
{
public:
    enum ERRCODE
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
    };

private:
    class erritem
    {
    private:
        int line;
        ERRCODE code;

    public:
        erritem(int line, ERRCODE code);
        int getLine();
        string getCodeStr();
    };

private:
    errorMags();
    errorMags(const errorMags &) = delete;
    errorMags &operator&(const errorMags &) = delete;

public:
    static errorMags &get_instance()
    {
        static errorMags instance;
        return instance;
    }

private:
    vector<erritem> errlist;

public:
    void errPut(int line, ERRCODE code);
    bool hasErrors();
    string getErrorsStr();
};
