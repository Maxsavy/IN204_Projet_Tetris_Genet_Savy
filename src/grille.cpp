#include <iostream>
using namespace std;


class Case
{
    private:
        int m_coordX;
        int m_coordY;
        int state;


    public:

        Case() {}
        ~Case() 
        {
            cout << "La case a été détruite" << endl;
        }

};


