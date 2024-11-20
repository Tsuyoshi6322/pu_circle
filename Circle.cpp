/*
    TODO:
    - User's input
    - ErrorHandlers
        - Loops on user's input to not break the rules of the program
        - Try,Catch 's to e.g. not divide by 0

    TODO (later):
    - Class optimalisation, generalisation
        - Repetitive instructions, fix it
        - Usless nesting of class' constructors, which could've been just methods
    - Maybe make a separate version of the file, based on headers
    - Some fancy output delay
        - Like, print the lines character by character :D
*/ 

#include <iostream>
#include <cmath>
using namespace std;

/*//================================================================================
    Definiowanie punktów A,B oraz D,E,F
*///================================================================================

struct PointA {
    float x; 
    float y;
};
struct PointB {
    float x; 
    float y;
};

struct PointD {
    float x; 
    float y;
};
struct PointE {
    float x; 
    float y;
};
struct PointF {
    float x; 
    float y;
};

/*//================================================================================
    Metody błędów:
        NonNumerErr() - Zła liczba podana jako input
        WrongAngle() - Wartość kąta alpha wykracza poza założenia programu
*///================================================================================

class ErrorHandlers {
    public:
    void NonNumberErr() {   
        cout << "To nie jest liczba!\n";
    }; 

    void WrongAngle() {
        cout << "Kąt ma być w zakresie 10-45 stopni!\n";
    }

    void VectorVerifFailed() {
        cout << "Vectors could not be verified, there is something wrong with the calculations\n";
        exit(100);
    }
};

/*//================================================================================
    Wyznaczenie punktu D - operacje:
        VectorAB::Basic - konstruktor wektora AB:
            - Współrzędne x, y wektora AB
            - Długość wektora AB

        VectorAB::Unified - kontruktor znormalizowanego wektora AB:
            - Współrzędne x, y znormalizowanego wektora AB

        CalcPointD - wyznaczenie współrzędnych punktu D:
            - Współrzędne x, y punktu D
*///================================================================================

class CalcPointD {
    public:
    class VectorAB {
        public:
        class Basic {
            public:
            float x, y, length;

            Basic(const PointA& PointA, const PointB& PointB){
                x = PointB.x - PointA.x;
                y = PointB.y - PointA.y;
                length = sqrt(pow(x,2) + pow(y,2));

                cout << "BasicAB: " << x << ", " << y << endl;
                cout << "BasicAB.length: " << length << endl;
            };

        };

        class Unified {
            public: 
            float x, y;

            Unified(Basic& Basic){
                x = Basic.x / Basic.length;
                y = Basic.y / Basic.length;

                cout << "UnifiedAB: " << x << ", " << y << endl;
            };
        };
    };

    CalcPointD(PointD& PointD, const PointA& PointA, float r, VectorAB::Unified& UnifiedVectorAB){
        PointD.x = PointA.x + (r * UnifiedVectorAB.x);
        PointD.y = PointA.y + (r * UnifiedVectorAB.y);

        cout << "PointD: " << PointD.x << ", " << PointD.y << endl;
    };
};

/*//================================================================================
    Wyznaczenie punktu E - operacje:
        VectorAD - konstruktor wektora AD:
            - Współrzędne x, y wektora AD
            - Długość wektora AD

        (Wektor AD należy obrócić (DE_1) i zastosować jego negację (DE_2), 
        aby punkt E znajdował się wewnątrz okręgu o środku A)
        
        VectorDE::Basic - konstruktor obróconego wektora AD:
            - Współrzędne x, y (Obrócenie wektora AD o kąt alpha w lewo)
            - Długość wektora DE_1 (Pięciokrotnie mniejszy od długości wektora AD)

        VectorDE::Basic.VectorChecker() - sprawdzenie założenia DE*5 = AD
            - Jeśli prawda -> idź dalej
            - Jeśli fałsz -> terminacja programu

        VectorDE - konstruktor wektora DE (DE_2):
            - Współrzędne x, y wektora DE (Negacja obróconego wektora AD)
        
        CalcPointE - wyznaczenie współrzędnych punktu E:
            - Współrzędne x, y punktu E
*///================================================================================

class CalcPointE {
    public:
    class VectorAD {
        public:
        float x, y, length;

        VectorAD(PointD& PointD, const PointA& PointA){
            x = PointD.x - PointA.x;
            y = PointD.y - PointA.y;
            length = sqrt(pow(x,2) + pow(y,2));

            cout << "VectorAD: " << x << ", " << y << endl;
            cout << "VectorAD.length: " << length << endl;
        };
    };

    class VectorDE {
        public:
        class Basic {
            public:
            float x, y, length;
            Basic(VectorAD& VectorAD, float alpha){
                x = (VectorAD.x * cos(alpha) - VectorAD.y * sin(alpha)) / (5);
                y = (VectorAD.x * sin(alpha) + VectorAD.y * cos(alpha)) / (5);
                length = sqrt(pow(x,2) + pow(y,2));

                cout << "BasicDE: " << x << ", " << y << endl;
                cout << "BasicDE.length: " << length << endl;
            };
        };

        float x,y;
        VectorDE(Basic& Basic){
            x = -Basic.x;
            y = -Basic.y;

            cout << "VectorDE: " << x << ", " << y << endl;
        };

        void VectorChecker(Basic& VectorDE, VectorAD& VectorAD){
            float epsilon = 0.00001;
            if (fabs((VectorDE.length * 5) - VectorAD.length) < epsilon) {
                cout << "Vectors verified, DE is 5 times less than AB\n";
            } else {
                ErrorHandlers ErrHand;
                ErrHand.VectorVerifFailed();
            };
        };
    };

    CalcPointE(PointE& PointE, const PointD& PointD, VectorDE& VectorDE){
        PointE.x = PointD.x + VectorDE.x;
        PointE.y = PointD.y + VectorDE.y;

        cout << "PointE: " << PointE.x << ", " << PointE.y << endl;
    };
};

/*//================================================================================
    Wyznaczenie punktu F - operacje:
        (Punkt F jest symetrycznym odbiciem punktu E, względem odcinka AB (prosta f).
        Prosta f ma wzór ogólny f: y = mx + b)
        
        (Wzór na współrzędne punktu F został wyznaczony następująco:
            - Równanie prostej prostopadłej do f: y = mx + b
            - Wyznaczenie wyrazu wolnego b = y + x/m
            - Równanie prostej przechodzącej przez punkt E, prostopadłej do prostej f
                g: y = -x/m + yE + xE/m
            - Wyznaczenie punktu przecięcia prostych f i g
                P: f = g
                P: mx + b = -x/m + yE + xE/m
                P:  {xP = (xE + myE - mb) / (m^2 + 1)}
                    {yP = mxP + b}                      // podstawienie xP do wzoru prostej f
            - Odbicie symetryczne punktu E (odległość punktu E od P == odległość punkt F od P
                F:  {xF = 2xP - xE}
                    {yF = 2yP - yE}
            - Po podstawieniu wzorów na xP w równaniu punktu F, wychodzą nam wzory w CalcPointF())

        - LinearAB - konstruktor współczynnika kierunkowego m i wyrazu wolnego b:
            - Zmienne m, b
        
        - CalcPointF - wyznaczenie współrzędnych punktu F:
            - Współrzędne x, y punktu F
*///================================================================================
class CalcPointF {
    public:
    class LinearAB {
        public:
        float b, m;
        LinearAB(const PointA& PointA, const PointB& PointB){
            m = (PointB.y - PointA.y) / (PointB.x - PointA.x);
            b = PointA.y - (m * PointA.x);

            cout << "LinearAB m,b: " << m << ", " << b << endl;
        };
    };

    CalcPointF(PointF& PointF, LinearAB& LinearAB, const PointE& PointE){
        PointF.x = 
        ((1 - pow(LinearAB.m,2)) * PointE.x + (2 * LinearAB.m * PointE.y) - (2 * LinearAB.m * LinearAB.b)) 
        / (1 + pow(LinearAB.m,2));

        PointF.y = 
        (((pow(LinearAB.m,2) - 1) * PointE.y) + (2 * LinearAB.m * PointE.x) + (2 * LinearAB.b))
        /(1 + pow(LinearAB.m,2));

        cout << "PointF: " << PointF.x << ", " << PointF.y << endl;
    };
};

int main() {
    
// Error Handlery zawarte w klasie ErrorHandlers
// Przydatne do loopów inputowych, aby użytkownik podał poprawne wartości
    ErrorHandlers ErrHand;
    ErrHand.NonNumberErr();
    ErrHand.WrongAngle();

// INPUT ====================================
// Do przepisania na cin
    PointA PointA = {2.0f,3.0f};
    PointB PointB = {4.0f,6.0f};
    float r = 2.0f; // (nie może być ujemne)
    float angle = 30.0f; // (angle <10,45>)
// ==========================================

// Przekształcenie wartości kątowej na radiany 
    const float pi = 3.14159265358979323846f;
    float alpha = angle * (pi / 180.0f);
// ===========================================

// Wyznaczenie punktu D
    PointD PointD; 

    CalcPointD::VectorAB::Basic BasicVectorAB(PointA, PointB);
    CalcPointD::VectorAB::Unified UnifiedVectorAB(BasicVectorAB);
    CalcPointD calcPointD(PointD, PointA, r, UnifiedVectorAB);

// Wyznaczenie Punktu E
    PointE PointE; 

    CalcPointE::VectorAD VectorAD(PointD, PointA);
    CalcPointE::VectorDE::Basic BasicVectorDE(VectorAD, alpha);
    CalcPointE::VectorDE ActualVectorDE(BasicVectorDE);
    ActualVectorDE.VectorChecker(BasicVectorDE, VectorAD);
    CalcPointE calcPointE(PointE, PointD, ActualVectorDE);

// Wyznaczenie Punktu F
    PointF PointF;

    CalcPointF::LinearAB LinearAB(PointA, PointB);
    CalcPointF calcPointF(PointF, LinearAB, PointE);

// Output danych wyjściowych
    cout << "PointD: (" << PointD.x << ", " << PointD.y << ")" << endl;
    cout << "PointE: (" << PointE.x << ", " << PointE.y << ")" << endl;
    cout << "PointF: (" << PointF.x << ", " << PointF.y << ")" << endl;

// KONIEC :DDDDDDD
    return 0;
}       

// yay line 300