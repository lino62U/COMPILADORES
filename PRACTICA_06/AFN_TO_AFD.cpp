#include <iostream>
#include <cstdlib>
#include <stack>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>    // std::find
#include <iterator>
using namespace std;



struct Transition {
    string fromState;
    string symbol;
    string toState;
    Transition(string a, string b, string c)
    {
        fromState = a;
        symbol = b;
        toState = c;
    }
};

void captarDATOS(vector<string> &es, vector<Transition> &tr, vector<string> &alfabeto, vector<string> &acep)
{
    fstream archivo("IN.txt");
    string linea;
    string estado="Estados";
    string est_aceptacion="Estados_de_Aceptacion";
    string alfabetos="Entradas";
    string transiciones="Transiciones_(x,y,z)_epsilon=-1";

    while (getline(archivo, linea))
    {
        linea.erase(linea.length() - 1);
        if(linea==estado)
        {
            getline(archivo,linea);
            getline(archivo,linea);
            istringstream iss(linea);
            string parte;
            while (iss >> parte) {
                es.push_back(parte);
            }
            
        }
        else if(linea== est_aceptacion)
        {
            getline(archivo,linea);
            getline(archivo,linea);
            istringstream iss(linea);
            string parte;
            while (iss >> parte) {
                acep.push_back(parte);
            }
        }
        else if(linea==alfabetos)
        {
            getline(archivo,linea);
            getline(archivo,linea);
            istringstream iss(linea);
            string parte;
            while (iss >> parte) {
                alfabeto.push_back(parte);
            }
        }
        else if(linea==transiciones)
        {
            getline(archivo, linea);
            while (getline(archivo, linea))
            {
                istringstream iss(linea);
                string fromState, symbol, toState;
                iss >> fromState >> symbol >> toState;
                tr.push_back(Transition(fromState, symbol, toState));
            }
        }
    }
}

vector<string>  conjunto_Eclausura (vector<string> &R, vector<Transition> &tranisciones, string symb)
{
    stack<string> pila;
// 1. Apilar todos los estados de R
    for (auto i : R)
        pila.push(i);
    
//2. Inicilizar E_clausura con R
    vector<string> E_Clausura(R);
    
//3. Mientras la pila no es vacia Hacer
    while (!pila.empty())
    {
        //a. Desapilar r de la cima
        string temp = pila.top(); pila.pop();
        //b. Por cada transición (r,ϵ,s) Hacer
        for (auto i : tranisciones){
            if(temp ==  i.fromState && i.symbol == symb){
                
                //i.- Si s no esta en E_clausura(R) entonces
                if(find(E_Clausura.begin(), E_Clausura.end(), i.toState) == E_Clausura.end())
                {
                    //a. Agregar s a E_clausura(R)
                    E_Clausura.push_back(i.toState);
                    //b. Apilar s.
                    pila.push(i.toState);
                }
            }
        }
    }    
    return E_Clausura;
}

vector<string> Mover(const vector<Transition>& transitions, const vector<string>& states, string symbol)
{
    vector<string> result;
    for (string state : states) {
        for (const Transition& transition : transitions) {
            if (transition.fromState == state && transition.symbol == symbol) {
                result.push_back(transition.toState);
            }
        }
    }
    return result;
}

vector< vector<string >> convertidorAFN (vector<string> &estados, vector<string> &simbolos, vector<Transition> &transiones,vector<Transition> &D_tran)
{
//1. Inicializar D_est con E_clausura(s0) y considerarlo no marcado.
    vector<string> estadoInicial; estadoInicial.push_back(estados[0]);

    vector<string> E_clausura=conjunto_Eclausura( estadoInicial,transiones,"-1");
    vector< vector<string> > D_est;  D_est.push_back(E_clausura);

    stack< vector<string> > D_stateStack;
    D_stateStack.push(E_clausura);  // Apilar la E_clausura del estado inicial

//2. Mientras exista un estado no marcado R en D_est Hacer
    while (!D_stateStack.empty())
    {
    //a. Marcar R
        vector<string> R = D_stateStack.top(); D_stateStack.pop();

    //b. Por cada símbolo de entrada x, Hacer
        for (auto x : simbolos)
        {
            //i. U  E-clausura(Mover(R,x))
            vector<string> temp2 = Mover(transiones,R,x);
            vector<string> U = conjunto_Eclausura(temp2,transiones, "-1");
          
            //ii. Si U no pertenece a D_est entonces
            if(find(D_est.begin(), D_est.end(), U)==D_est.end())
            {
                //Agregar U a D_est como un estado no marcado
                D_est.push_back(U);
                D_stateStack.push(U);
            }
            
            //iii. D_tran(R,x) := U
            int posicionStart = std::distance(D_est.begin(), find(D_est.begin(), D_est.end(), R));
            int posicionEnd = std::distance(D_est.begin(), find(D_est.begin(), D_est.end(), U));
            
            D_tran.push_back(Transition(to_string(posicionStart), x, std::to_string(posicionEnd)));
        }
        
       sort(D_tran.begin(), D_tran.end(), [](const Transition& a, const Transition& b) { return a.fromState < b.fromState;});
    }
    return D_est;
}

void findNewEndState (vector< vector<string >> &estados, vector<string> &endState, vector<string> &newEnd)
{
    int q=0;
    for (auto i : estados)
    {
        for(auto o: i)
        {
            for (auto j : endState)
            {
                if(j==o) newEnd.push_back(to_string(q));
            }
        }   
        q++;
    }
}


void generateOut(vector< vector<string >> &AFD, vector<Transition> &tran, vector<string> &alfabeto, vector<string> &NewAceptacion)
{
    ofstream newArchivo("OUT.txt");
    string estado="Estados";
    string est_aceptacion="Estados_de_Aceptacion";
    string alfabetos="Entradas";
    string transiciones="Transiciones_(x,y,z)_epsilon=-1";

    if (newArchivo.is_open()) {
        newArchivo<<estado<<endl;
        newArchivo<<AFD.size()<<endl;
        int q=0;
        for(auto i: AFD)
        {
            newArchivo<<q<<" = {";
            std::sort(i.begin(), i.end());
            for(auto o: i)
                newArchivo<<o<<" ";
            
            newArchivo<<"}"<<endl;
            q++;
        }

        newArchivo<<est_aceptacion<<endl;
        newArchivo<<NewAceptacion.size()<<endl;
        for (auto i : NewAceptacion)
            newArchivo<<i<<" ";
        
        newArchivo<<endl;

        newArchivo<<alfabetos<<endl;
        newArchivo<<alfabeto.size()<<endl;
        for (auto i : alfabeto)
            newArchivo<<i<<" ";
        newArchivo<<endl;

        newArchivo<<transiciones<<endl;
        for(auto a: tran)
            newArchivo<<a.fromState<<" "<<a.symbol<< "  "<<a.toState<<endl;

        
    } else {
        std::cout << "No se pudo crear el archivo." << std::endl;
    }
}

void generateGraphic(vector<Transition> &tran, vector<string> &NewAceptacion)
{
    ofstream newArchivo("grafico.txt");
    
    string est_aceptacion="";
    string transiciones="";
    
    string inicio = "digraph finite_state_machine { fontname=\"Helvetica,Arial,sans-serif\" node [fontname=\"Helvetica,Arial,sans-serif\"]";
    inicio+=" edge [fontname=\"Helvetica,Arial,sans-serif\"] rankdir=LR; node [shape = doublecircle];";
    for (auto i : NewAceptacion)
    {
        est_aceptacion+= i;
        est_aceptacion+=" ";
    }
    est_aceptacion+=";";
    inicio+=est_aceptacion;
    inicio+="node [shape = circle];";

    for (auto t : tran)
    {
        transiciones+=(t.fromState + " -> " + t.toState + " [label = \"");
        transiciones+=(t.symbol + "\"];\n");
        
    }
    inicio+=(transiciones + "\n}");
    newArchivo<<inicio;
}
void generateGraphicAFN(vector<Transition> &tran, vector<string> &NewAceptacion)
{
    ofstream newArchivo("grafico2.txt");
    
    string est_aceptacion="";
    string transiciones="";
    
    string inicio = "digraph finite_state_machine { fontname=\"Helvetica,Arial,sans-serif\" node [fontname=\"Helvetica,Arial,sans-serif\"]";
    inicio+=" edge [fontname=\"Helvetica,Arial,sans-serif\"] rankdir=LR; node [shape = doublecircle];";
    for (auto i : NewAceptacion)
    {
        est_aceptacion+= i;
        est_aceptacion+=" ";
    }
    est_aceptacion+=";";
    inicio+=est_aceptacion;
    inicio+="node [shape = circle];";

    for (auto t : tran)
    {
        transiciones+=(t.fromState + " -> " + t.toState + " [label = \"");
        transiciones+=(t.symbol + "\"];\n");
        
    }
    inicio+=(transiciones + "\n}");
    newArchivo<<inicio;
}
int main(){

    vector<string> estados;
    vector<Transition> transicion;
    vector<Transition> tran;
    vector<string> alfabeto;
    vector<string> aceptacion;
    vector<string> NewAceptacion;
    
    captarDATOS(estados, transicion, alfabeto, aceptacion);
    vector< vector<string >> afd = convertidorAFN(estados,alfabeto,transicion, tran);
    findNewEndState(afd,aceptacion,NewAceptacion );
    
    generateOut(afd,tran,alfabeto,NewAceptacion);
    generateGraphic(tran,NewAceptacion);
    generateGraphicAFN(transicion,aceptacion);

    string comand = "dot -Tpng grafico.txt -o grafico.png";
    string comand2 = "dot -Tpng grafico2.txt -o graficoAFN.png";
       
    system(comand.c_str());
    system(comand2.c_str());
  
    return 0;
}
