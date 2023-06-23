#include <iostream>
#include <stdio.h>
#include <stack>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <ctype.h>
using namespace std;

#define MAYOR '>'
#define MAYORIGUAL 258
#define MENOR '<'
#define MENORIGUAL 261
#define IGUALDAD 262
#define DIF 263
#define CARACTER '!'
#define ASIG '='
#define PARI '('
#define PARI2 ')'
#define CORCH '['
#define CORCH2 ']'
#define COMA ','
#define PUNTOYCOMA ';'
#define LLAVE '{'
#define LLAVE2 '}'
#define ID 256
#define NUM 257
#define WHILE 259
#define IF 260
#define INT 264  //->
#define MAIN 265
#define FLOAT 266
#define CHAR 267
#define BOOL 268
#define DOUBLE 269
#define STRING 270
#define CLASS 271
#define FOR 272
#define STRUCT 273
#define SWITCH 274
#define SUM '+'
#define RES '-'
#define MULTI '*'
#define DIVI '/' 
#define COMENT 461
#define BLOCKCOMENT 462


int scanerFile();
int espalres();
void separador(int);
FILE *f;
char _lexema[80];
string linea="";
string variable="";
string identificador="";
int token;

bool esOperador(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Verifica si se ejecuta primero una suma o multiplicacion, etc
int obtenerPrioridad(char operador)
{
    if (operador == '+' || operador == '-')
        return 1;
    else if (operador == '*' || operador == '/')
        return 2;
    else
        return 0;
}

string convertirAPosfija( string& expresionInfija)
{
    stack<char> pila;
    string posfija;

    for (char c : expresionInfija)
    {

        if (c == ' ')
        {
            posfija+=" ";
            continue;
        }

        if (esOperador(c))
        {
            // Verifica si se ejecuta primero una suma o multiplicacion, etc
            
            posfija += " ";
            while (!pila.empty() && pila.top() != '(' && obtenerPrioridad(pila.top()) >= obtenerPrioridad(c))
            {
                posfija += pila.top();
                pila.pop();
            }
            pila.push(c);
        }
        else if (c == '(')
        {
            pila.push(c);
        }
        else if (c == ')')
        {
            posfija += " ";
            while (!pila.empty() && pila.top() != '(')
            {
                posfija += pila.top();
            
                pila.pop();
            }
            
            
            pila.pop();
        }
        else if(c==';')
            break;
        else
        {
            posfija += c;
        }
        
        
    }

    while (!pila.empty())
    {
        posfija += " ";
        posfija += pila.top();
        posfija+=";";
        pila.pop();
    }

    return posfija;
}


int Resultado(string n)
{
    
    stack<float> pila;
    
    string num="";
    for (int i = 0; i < n.size();i++)
    {
        if(isdigit(n[i]))
        
            num+=n[i];
        else
        {
            if(n[i]==' '&& !num.empty())
            {
                pila.push(stof(num));
                num="";
            }else if(esOperador(n[i]))
            {
                int aux = pila.top();
                pila.pop();
                int aux2 = pila.top();
                pila.pop();
                
                switch(n[i])
                {
                    case '+':pila.push(aux2 + aux);break;
                    case '-':pila.push(aux2 - aux);break;
                    case '*':pila.push(aux2 * aux);break;
                    case '/':pila.push(aux2 / aux);break;
                }
                
            }else if(n[i]==';')
                
                return pila.top();
        }
    }
    return 0;
   
}




int scanerFile()
{
    int c;
    int i;
    do c=fgetc(f); while(isspace(c)); //ignora blancos
    if(c==EOF) return EOF;

    if(isalpha(c)) //regla del ID
    {
        i=0;
        do{
            _lexema[i++]=c;
            c=fgetc(f);
        } while(isalnum(c)||c=='_');
        _lexema[i]=0;
        ungetc(c,f); //se devuelve c al flujo de entrada
        i=espalres(); // verifica si es palabra reservada
        // WHILE , IF
        if(i>=0)
            return i;
        return ID; // se trata de un ID
    }


    if(isdigit(c)) //regla del NUM
    {
        i=0;
        do{
            _lexema[i++]=c;
            c=fgetc(f);
        }while(isdigit(c));
        _lexema[i]=0;
        ungetc(c,f);
        return NUM;
    }

    //regla de SIMBOLOS
    if((c=='(') || (c==')')||(c=='[') || (c==']')|| (c==',') || (c==';')||(c=='{') || (c=='}')) return c; //regla del ";" y "("
    

    //regla de los operadores logicos
    if(c=='>') //regla de ">" o ">="
    {
        c=fgetc(f);
        if(c=='=') //return MAYORIGUAL
        { 
            _lexema[0]='>'; _lexema[1]='='; _lexema[2]=0;
            return MAYORIGUAL;
        }
        ungetc(c,f);
        return MAYOR; //return MAYOR
    }
    if(c=='<') //regla de "<" o "<="
    {
        c=fgetc(f);
        if(c=='=') //return MENORIGUAL
        { 
            _lexema[0]='<'; _lexema[1]='='; _lexema[2]=0;
            return MENORIGUAL;
        }
        ungetc(c,f);
        return MENOR; //return MAYOR
    }

    if(c=='=') //regla de "<" o "<="
    {
        c=fgetc(f);
        if(c=='=') //return IGUALDAD
        { 
            _lexema[0]='='; _lexema[1]='='; _lexema[2]=0;
            return IGUALDAD;
        }
        ungetc(c,f);
        return ASIG; //return ASIGNACION
    }

    if(c=='!') //regla de "<" o "<="
    {
        c=fgetc(f);
        if(c=='=') //return DIFERENTE
        { 
            _lexema[0]='!'; _lexema[1]='='; _lexema[2]=0;
            return DIF;
        }
        
        return CARACTER; //return VACIO
    }

    // regla de OPERADORES ARITMETICOS

    if( c=='+') return c;
    if( c=='-') return c;
    if( c=='*') return c;
    if( c=='/') 
    {
        c=fgetc(f);
        if(c=='/') //return COMENTARIO EN LINEA
        { 
            while(c!='\n'){c=fgetc(f);}
            return COMENT;
        }
        if (c=='*') //return COMENTARIO EN BLOQUE
        {
            int aux=c;
            
            while(c!='*' || aux!='/')
            {
                c=fgetc(f);
                aux = fgetc(f);
                ungetc(aux,f);
            }
            c=fgetc(f);
            return BLOCKCOMENT;
        }
        
        ungetc(c,f);
        return DIVI; //return DIVISION
    }
    return -1;
   
}//fin de scanerFile

void separador(int token)
{
    if(token==ID)
    {
        identificador=_lexema;
    }
    if(token==INT ||token==FLOAT ||token==CHAR  || token==BOOL || token==DOUBLE || token==STRING)
    {
        variable=_lexema;
    }
    if(token==NUM )
    {
        //cout<<_lexema<<endl;
        linea+=_lexema;
    }
    if(token==PUNTOYCOMA)
    {
       
        string expresionPosfija = convertirAPosfija(linea);
        cout << "\nExpresión infija ("<<variable<<"): " << linea << endl;
        cout << "Expresión posfija: " << expresionPosfija << endl;
        cout << "Resultado: " <<identificador<<" = "<< Resultado(expresionPosfija)<<endl;
        // linea="";
        linea="";
        
    }
    if(token==PARI || token==PARI2|| token==SUM || token==RES || token==MULTI || token==DIVI) 
    {
        //cout<<char(token)<<endl;
        linea+=char(token);
    }
    
    
}

int espalres()
{
    if(strcmp(_lexema,"while")==0) return WHILE;
    if(strcmp(_lexema,"if")==0) return IF;
    if(strcmp(_lexema,"int")==0) return INT;
    if(strcmp(_lexema,"main")==0) return MAIN;
    if(strcmp(_lexema,"float")==0) return FLOAT;
    if(strcmp(_lexema,"char")==0) return CHAR;
    if(strcmp(_lexema,"bool")==0) return BOOL;
    if(strcmp(_lexema,"double")==0) return DOUBLE;
    if(strcmp(_lexema,"string")==0) return STRING;
    if(strcmp(_lexema,"class")==0) return CLASS;
    if(strcmp(_lexema,"for")==0) return FOR;
    if(strcmp(_lexema,"struct")==0) return STRUCT;
    if(strcmp(_lexema,"switch")==0) return SWITCH;
    
    return -1;
}


void leerArchivo()
{
   
    f=stdin; //entrada estandar del teclado
    if(true) //si se especifico un archivo de lectura
    {
        f=fopen("programa2.txt","rt"); //lectura modo texto
        if(f==NULL)
            f=stdin;
    }
    bool begin=false;
    while (true)
    {
        
        token=scanerFile();
        if(token==EOF) break;
  
        separador(token);

    }
    if(f !=stdin) // si la entrada fue de un archivo
        fclose(f); // entonces cerrar el archivo.
    
}
int main()
{
    //fstream archivo("programa1.txt");
    

    leerArchivo();
   

    return 0;
}
