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
    Definiowanie zmiennych punktów jako zbioru współrzędnych
*///================================================================================

struct Point {
    float x; 
    float y;
};

/*//================================================================================
    Metody błędów:
        NonNumerErr() - Zła liczba podana jako input
        WrongAngle() - Wartość kąta alpha wykracza poza założenia programu
        VectorVerifFailed() - WektorDE nie spełnia założeń zadania, 
            nie jest pięciokrotnie mniejszy od promienia
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

    float x, y, length;

    void VectorAB(
        Point A, Point B, float& x, float& y, float length){
        x = B.x - A.x;
        y = B.y - A.y;
        length = sqrt(pow(x,2) + pow(y,2));
        cout << "BasicAB: (" << x << "),(" << y << ")\n";
        cout << "BasicAB.length: " << length << endl;

        x = x / length;
        y = y / length;
        cout << "UnifiedAB: (" << x << "),(" << y << ")\n";
    };

    void Calculate(Point D, Point A, float r, float x, float y){
        D.x = A.x + (r * x);
        D.y = A.y + (r * y);
        cout << "PointD: (" << D.x << "),(" << D.y << ")\n";
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

        float x, y, length;

        void VectorAD(float& x, float& y, Point D, Point A, float& length){
            x = D.x - A.x;
            y = D.y - A.y;
            length = sqrt(pow(x,2) + pow(y,2));

            cout << "VectorAD: " << x << ", " << y << endl;
            cout << "VectorAD.length: " << length << endl;
        };

        void VectorDE(float& x, float& y, float& alpha, float& length){
            x = (x * cos(alpha) - y * sin(alpha)) / (5);
            y = (x * sin(alpha) + y * cos(alpha)) / (5);
            length = sqrt(pow(x,2) + pow(y,2));

            cout << "BasicDE: " << x << ", " << y << endl;
            cout << "BasicDE.length: " << length << endl;

            x = -x;
            y = -y;

            cout << "VectorDE: " << x << ", " << y << endl;
        };

        void VectorChecker(float& length, float& r){
            float epsilon = 0.00001;
            if ((length * 5) == r) {
                cout << "Vectors verified, DE is 5 times less than AB\n";
            } else {
                ErrorHandlers ErrHand;
                ErrHand.VectorVerifFailed();
            };
        };

        void Calculate(Point E, const Point D, float& x, float& y){
            E.x = D.x + x;
            E.y = D.y + y;

            cout << "PointE: " << E.x << ", " << E.y << endl;
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
        float b, m;

        void LinearAB(Point A, Point B, float& m, float& b){
            m = (B.y - A.y) / (B.x - A.x);
            b = A.y - (m * A.x);

            cout << "LinearAB m,b: " << m << ", " << b << endl;
        };

        void Calculate(Point E, Point F, float& m, float& b){
            F.x = 
            ((1 - pow(m,2)) * E.x + (2 * m * E.y) - (2 * m * b)) 
            / (1 + pow(m,2));

            F.y = 
            (((pow(m,2) - 1) * E.y) + (2 * m * E.x) + (2 * b))
            /(1 + pow(m,2));

            cout << "PointF: " << F.x << ", " << F.y << endl;
        };
};

int main() {
    
// Error Handlery zawarte w klasie ErrorHandlers
// Przydatne do loopów inputowych, aby użytkownik podał poprawne wartości
    ErrorHandlers ErrHand;
   // ErrHand.NonNumberErr();

// INPUT ====================================
// Do przepisania na cin
    Point A;
    cout << "Podaj współrzędne x i y dla Punktu A: ";
    cin >> A.x >> A.y;
    
    Point B;
    cout << "Podaj współrzędne x i y dla Punktu B: ";
    cin >> B.x >> B.y;
    
    float r; // (nie może być ujemne)
    cout << "Podaj dłuość r: ";
    cin >> r;
        while (r < 0){
            cout << "Podaj liczbę większą niż 0\n";
            cin >> r;
        }
    
    float angle; // (angle <10,45>)
    cout << "Podaj kąt: ";
    cin >> angle;
        while (angle < 10 || angle > 45){
            ErrHand.WrongAngle();
            cin >> angle;
        }
// ==========================================

// Przekształcenie wartości kątowej na radiany 
    const float pi = 3.14159265358979323846f;
    float alpha = angle * (pi / 180.0f);
// ===========================================

// Wyznaczenie punktu D
    Point D; 
    CalcPointD CalcPointD;

    CalcPointD.VectorAB(A, B, CalcPointD.x, CalcPointD.y, CalcPointD.length);
    CalcPointD.Calculate(D, A, r, CalcPointD.x, CalcPointD.y);

// Wyznaczenie Punktu E
    Point E; 
    CalcPointE CalcPointE;

    CalcPointE.VectorAD(CalcPointE.x, CalcPointE.y, D, A, CalcPointE.length);
    CalcPointE.VectorDE(CalcPointE.x, CalcPointE.y, alpha, CalcPointE.length);
    CalcPointE.VectorChecker(CalcPointE.length, r);
    CalcPointE.Calculate(E, D, CalcPointE.x, CalcPointE.y);

// Wyznaczenie Punktu F
    Point F;
    CalcPointF CalcPointF;

    CalcPointF.LinearAB(A, B, CalcPointF.m, CalcPointF.b);
    CalcPointF.Calculate(E, F, CalcPointF.m, CalcPointF.b);

// Output danych wyjściowych
    cout << "PointD: (" << D.x << ", " << D.y << ")" << endl;
    cout << "PointE: (" << E.x << ", " << E.y << ")" << endl;
    cout << "PointF: (" << F.x << ", " << F.y << ")" << endl;

// KONIEC :DDDDDDD
    return 0;
}       

// yay line 300
