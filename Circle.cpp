#include <iostream>
// potrzebne dla VectorChecker()
#include <cmath> 
// potrzebne do GetValidatedInput()
#include <sstream>
#include <string>
#include <limits>
    
using namespace std;

/*//================================================================================
    Definiowanie zmiennych punktów jako zbioru współrzędnych
    Przyda się również w sytuacji wektorów
*///================================================================================

struct Point {
    float x; 
    float y;
};

/*//================================================================================
    Metody błędów:
        NonNumerErr() - Zła liczba podana jako input
        WrongAngle() - Wartość kąta alpha wykracza poza założenia programu
        VectorVerifFailed() - WektorDE nie spełnia założeń zadania (VectorDE * 5 = r)
*///================================================================================

class ErrorHandlers {
    public:

    float GetValidatedInput(const std::string& prompt) {
        std::string input;
        float value;

        while (true) {
            cout << prompt;
            getline(cin, input);

            // Konwersja tekstu na liczbę
            stringstream stream(input);
            if (stream >> value && stream.eof()) {
                return value;
            } else {
                cout << "!!! To nie jest liczba, albo użyłeś przecinka jako separatora dziesiętnego - Użyj kropki !!!\n\n";
            }
        }
    }

    void SamePoints() {
        cout << "!!! Punkty A i B nie mogą leżeć w tym samym miejscu !!!\n\n";
    }

    void DivZero(){
        cout << "!!! Punkty A i B leżą na osi OX !!!\n";
        cout << "!!! Nie będzie możliwe wyznaczenie punktu F (Dzielenie przez zero) !!!\n\n";
    }

    void WrongAngle() {
        cout << "!!! Kąt ma być w zakresie 10-45 stopni !!!\n\n";
    }

    void WrongRadius() {
        cout << "!!! Podaj liczbę większą niż 0 !!!\n\n";
    }

    void VectorVerifFailed() {
        cout << "!!! Długość wektora DE nie spełnia warunku zadania - błąd w obliczeniach !!!\n\n";
        exit(100);
    }
};

/*//================================================================================
    Wyznaczenie punktu D - operacje:
        VectorAB() - metoda wyznaczająca wektor AB:
            - Współrzędne x, y wektora AB
            - Długość wektora AB
            - Normalizacja wektora AB

        Calculate() - metoda wyznaczająca współrzędne punktu D:
            - Współrzędne x, y punktu D
*///================================================================================

class CalcPointD {
    public:

    Point UvAB;

    void VectorAB(Point* A, Point* B,  Point* UvAB){
        // Współrzędne Wektora AB
        Point vAB;
        vAB.x = B->x - A->x;
        vAB.y = B->y - A->y;
        cout << "-------------------------------------------------------------------\n";
        cout << "== Wektor AB: (" << vAB.x << "),(" << vAB.y << ")\n";

        // Długość Wektora AB
        float length;
        length = sqrt(pow(vAB.x,2) + pow(vAB.y,2));
        cout << "== Długość Wektora AB = " << length << endl;

        // Normalizacja (Unifikacja) Wektora AB
        UvAB->x = vAB.x / length;
        UvAB->y = vAB.y / length;
        cout << "== Znormalizowany Wektor AB: (" << UvAB->x << "),(" << UvAB->y << ")\n";
    };

    void Calculate(Point* D, Point* A, float r, Point* UvAB){
        // Współrzędne punktu D
        D->x = A->x + (r * UvAB->x);
        D->y = A->y + (r * UvAB->y);
        //cout << "== PointD: (" << D->x << "),(" << D->y << ")\n";
        cout << "-------------------------------------------------------------------\n";
    };
};

/*//================================================================================
    Wyznaczenie punktu E - operacje:
        VectorAD() - metoda wyznaczająca wektor AD:
            - Współrzędne x, y wektora AD
            - Długość wektora AD
        
        VectorDE() - metoda wyznaczająca wektor DE:
            - Współrzędne x, y wektora DE (Obrócenie wektora AD o kąt alpha w lewo)
            - Długość wektora DE (Do późniejszej weryfikacji z założeniami zadania)
            - Negacja współrzędnych wektora DE (Aby punkt E znajdował się wewnątrz okręgu o środku A)

        VectorChecker() - metoda sprawdzająca założenie WektorDE * 5 = AD = r
            - Jeśli prawda -> idź dalej (Floating Point error w granicy 0.0001 jest dopuszczalny)
            - Jeśli fałsz -> terminacja programu (kod 100)
        
        Calculate() - wyznaczenie współrzędnych punktu E:
            - Współrzędne x, y punktu E
*///================================================================================

class CalcPointE {
    public:

    Point vAD, vDE;
    float length;

    void VectorAD(Point* D, Point* A, Point* vAD){
        // Współrzędne Wektora AD
        vAD->x = D->x - A->x;
        vAD->y = D->y - A->y;
        cout << "== Wektor AD: (" << vAD->x << "), (" << vAD->y << ")" << endl;

        // Długość Wektora AD
        float lengthAD;
        lengthAD = sqrt(pow(vAD->x,2) + pow(vAD->y,2));
        cout << "== Długość Wektora AD = " << lengthAD << endl;
    };

    void VectorDE(Point* vAD, Point* vDE, float alpha, float& length){
        // Obrócenie Wektora AD o alpha kątów w lewo, pomniejszenie wartości pięciokrotnie
        vDE->x = (vAD->x * cos(alpha) - vAD->y * sin(alpha)) / (5);
        vDE->y = (vAD->x * sin(alpha) + vAD->y * cos(alpha)) / (5);
        
        // Wyliczenie długości wektora w celu weryfikacji
        length = sqrt(pow(vDE->x,2) + pow(vDE->y,2));

        // Negacja Wektora DE, aby punkt E znajdował się wewnątrz okręgu
        vDE->x = -vDE->x;
        vDE->y = -vDE->y;
        cout << "== Wektor DE: (" << vDE->x << "), (" << vDE->y << ")" << endl;
    };

    void VectorChecker(float& length, float r){
        // Floating point error handler
        float epsilon = 0.0001;

        // Jeżeli długość Wektora DE * 5 jest względnie taka sama z promieniem r to przejdź dalej
        if (fabs((length * 5) - r) < epsilon) {
            cout << "== Długość wektora DE spełnia warunek zadania!\n";
        } 
        // W przeciwnym razie, przerwij działanie programu
        else {
            ErrorHandlers ErrHand;
            ErrHand.VectorVerifFailed();
        };
    };

    void Calculate(Point* E, Point* D, Point* vDE){
        // Współrzędne punktu E
        E->x = D->x + vDE->x;
        E->y = D->y + vDE->y;
        //cout << "== PointE: " << E->x << ", " << E->y << endl;
        cout << "-------------------------------------------------------------------\n";
    };
};

/*//================================================================================
    Wyznaczenie punktu F - operacje:
        (Punkt F jest symetrycznym odbiciem punktu E, względem odcinka AB (prosta f).
        Prosta f ma wzór ogólny f: y = mx + b)
        
        (Wzór na współrzędne punktu F został wyznaczony następująco:
            - Wyznaczenie współczynnika kierunkowego na podstawie punktów A i B
                m = (By - Ay) / (Bx - Ax)
            - Wyznaczenie wyrazu wolnego b dla f: Ay = m * Ax + b
                b = Ay - (m * Ax)
            - Równanie prostej przechodzącej przez punkt E, prostopadłej do prostej f
                g: y - Ey = -x/m + Ex/m
            - Wyznaczenie punktu przecięcia prostych f i g
                P: f = g
                P: m * x + b = -x/m + Ey + Ex/m
                P:  {Px = (Ex + m * Ey - m * b) / (m^2 + 1)} // podstawienie Px do wzoru prostej g
                    {Py = m * Px + b}                      // podstawienie Py do wzoru prostej f
            - Odbicie symetryczne punktu E (odległość punktu E od P == odległość punkt F od P)
                F:  {Fx = 2Px - Ex}
                    {Fy = 2Py - Ey}
            - Po podstawieniu wzorów na Px w równaniu punktu F, wychodzą nam wzory w metodzie Calculate())

        - LinearAB() - metoda wyznaczająca współczynnik kierunkowy m i wyraz wolny b:
            - Zmienne m, b
        
        - Calculate() - metoda wyznaczająca współrzęde punktu F:
            - Współrzędne x, y punktu F
*///================================================================================
class CalcPointF {
    public:
    float b, m;

    void LinearAB(Point* A, Point* B, float& m, float& b){
        // Współ. kierunkowy m oraz wyraz wolny b funkcji f: y = mx + b (na której leży odcinek AD)
        m = (B->y - A->y) / (B->x - A->x);
        b = A->y - (m * A->x);
        cout << "== Współczynnik kierunkowy m = " << m << endl; 
        cout << "== Wyraz wolny b = " << b << endl; 
    };

    void Calculate(Point* E, Point* F, float& m, float& b){
        // Współrzędne punktu F
        F->x = ((1 - pow(m,2)) * E->x + (2 * m * E->y) - (2 * m * b)) / (1 + pow(m,2));
        F->y = (((pow(m,2) - 1) * E->y) + (2 * m * E->x) + (2 * b)) / (1 + pow(m,2));
        //cout << "== PointF: " << F->x << ", " << F->y << endl;
    };
};

int main() {
    
// Error Handlery zawarte w klasie ErrorHandlers
    ErrorHandlers ErrHand;

// TEST INPUT ===============================
    // Point A = {2,3};
    // Point B = {4,6};
    // float r = 2;
    // float angle = 30;
// TEST INPUT ===============================

// INPUT ====================================
// Punkty A i B  
    Point A, B; 
    while (true){

        A.x = ErrHand.GetValidatedInput("Podaj współrzędną x dla Punktu A: ");
        A.y = ErrHand.GetValidatedInput("Podaj współrzędną y dla Punktu A: ");
        cout << "Współrzędne punktu A: (" << A.x << ", " << A.y << ")\n";

        B.x = ErrHand.GetValidatedInput("Podaj współrzędną x dla Punktu B: ");
        B.y = ErrHand.GetValidatedInput("Podaj współrzędną y dla Punktu B: ");
        cout << "Współrzędne punktu B: (" << B.x << ", " << B.y << ")\n";


        if(A.x == B.x && A.y == B.y){
            ErrHand.SamePoints();
            continue;
        }

        if(A.x == 0 && B.x == 0){
            ErrHand.DivZero();
            continue;
        }

        break;
    }

// Promień okręgu - nie może być ujemny
    float r; 
    while (true){
        r = ErrHand.GetValidatedInput("Podaj promień okręgu: ");

        if (r <= 0) {
            ErrHand.WrongRadius();
            continue;
        }

        cout << "Promień okręgu to: " << r << endl;
        break;
    }

// Kąt pomiędzy ramieniem strzałki a promieniem - w zakresie angle <10,45>
    float angle;
    while (true){
        angle = ErrHand.GetValidatedInput("Podaj kąt (10-45): ");

        if (angle < 10 || angle > 45){
            ErrHand.WrongAngle();
            continue;
        }

        cout << "Wybrany kąt to: " << angle << endl;
        break;
    }

// ==========================================

// Przekształcenie wartości kątowej na radiany 
    const float pi = 3.14159265358979323846f;
    float alpha = angle * (pi / 180.0f);
// ===========================================

// Wyznaczenie punktu D
    Point D; 
    CalcPointD CalcPointD;

    CalcPointD.VectorAB(&A, &B, &CalcPointD.UvAB);
    CalcPointD.Calculate(&D, &A, r, &CalcPointD.UvAB);

// Wyznaczenie Punktu E
    Point E; 
    CalcPointE CalcPointE;

    CalcPointE.VectorAD(&D, &A, &CalcPointE.vAD);
    CalcPointE.VectorDE(&CalcPointE.vAD, &CalcPointE.vDE, alpha, CalcPointE.length);
    CalcPointE.VectorChecker(CalcPointE.length, r);
    CalcPointE.Calculate(&E, &D, &CalcPointE.vDE);

// Wyznaczenie Punktu F
    Point F;
    CalcPointF CalcPointF;

    CalcPointF.LinearAB(&A, &B, CalcPointF.m, CalcPointF.b);
    CalcPointF.Calculate(&E, &F, CalcPointF.m, CalcPointF.b);

// Output danych wyjściowych
    cout << "===================================================================\n";
    cout << "== Współrzędne punktu D: (" << D.x << ", " << D.y << ")" << endl;
    cout << "== Współrzędne punktu E: (" << E.x << ", " << E.y << ")" << endl;
    cout << "== Współrzędne punktu F: (" << F.x << ", " << F.y << ")" << endl;
    cout << "===================================================================\n";

// KONIEC :D
    return 0;
}       