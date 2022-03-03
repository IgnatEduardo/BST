#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

string nume_fis;


ofstream out("iesire.out");

using namespace std;

struct nod_arbore
{
    int val;
    nod_arbore* fiu_stang;
    nod_arbore* fiu_drept;
};

struct arbore
{
    nod_arbore* radacina;
    arbore()
    {
        radacina = NULL;
    }


    void SRD()
    {
        out<<"SRD: ";
        SRD(radacina);
        out<<endl;
    }
    void SRD(nod_arbore* pointer)
    {
        if (pointer!=NULL)
        {
            SRD(pointer->fiu_stang);
            out<<pointer->val<<" ";
            SRD(pointer->fiu_drept);
        }
    }

    void RSD()
    {
        out<<"RSD: ";
        RSD(radacina);
        out<<endl;
    }
    void RSD(nod_arbore* pointer)
    {
        if (pointer!=NULL){
            out<<pointer->val<<" ";
            RSD(pointer->fiu_stang);
            RSD(pointer->fiu_drept);
        }
    }

    int nr_elem(nod_arbore* nod_arbore) // e nevoie de el in struct ca sa poata fi folosit de k_element
    {
        if (nod_arbore == NULL)
            return 0;
        else
            return(nr_elem(nod_arbore->fiu_stang) + 1 + nr_elem(nod_arbore->fiu_drept));
    }

    int k_element(int k)
    {
        if(nr_elem(radacina) > k)
            return 0;
        return k_element(radacina, k);
    }
    int k_element(nod_arbore* pointer, int k)
    {
        int fiu_stang_nr_elem = nr_elem(pointer->fiu_stang);
        if( k == fiu_stang_nr_elem + 1) //daca numarul de elemente total incepand de la nodul curent in stanga este egal cu k atunci suntem la nodul cautat
            return pointer->val;
        else
        {
            if ( k > fiu_stang_nr_elem + 1) // daca k este mai mare decat numarul de elemente din stanga nodului curent, inclusiv, atunci nu avem ce cauta in stanga si continuam cu fiul din dreapta
                return k_element(pointer->fiu_drept, k - fiu_stang_nr_elem - 1);
            else
                return k_element(pointer->fiu_stang, k);// altfel, cautam in continuare in stanga
        }
    }

};

void insereaza(arbore &arbore_curent, int x) 
{
    nod_arbore* nou_nod = new nod_arbore;
    nou_nod->val = x;
    nou_nod->fiu_stang = NULL;
    nou_nod->fiu_drept = NULL;

    if (arbore_curent.radacina == NULL)
    {
        arbore_curent.radacina = nou_nod;
        return;
    }

    nod_arbore* pointer_prev = NULL;
    nod_arbore* pointer = arbore_curent.radacina;
    int directie = 0;

    while(pointer != NULL)
    {
        pointer_prev = pointer;
        if (x <= pointer->val)
        {
            pointer = pointer->fiu_stang;
            directie = -1;
        }
        else
        {
            pointer = pointer->fiu_drept;
            directie = 1;
        }
    }

    if (directie < 0)
        pointer_prev->fiu_stang = nou_nod;
    else
        pointer_prev->fiu_drept = nou_nod;

}

void sterge(arbore &arbore_curent, int x) 
{
    nod_arbore* pointer_prev = NULL;
    nod_arbore* pointer = arbore_curent.radacina;
    int directie = 0;
    while (pointer != NULL && pointer->val != x)
    {
        pointer_prev = pointer;
        if (x <= pointer->val)
        {
            pointer = pointer->fiu_stang;
            directie = -1;
        }
        else
        {
            pointer = pointer->fiu_drept;
            directie = 1;
        }
    }
    out<<pointer->val<<endl;
    if (pointer == NULL)
    {
        out<<"nu exista "<<x<<endl;
        return;
    }

    if (pointer->fiu_stang == NULL && pointer->fiu_drept == NULL)
    {
        out<<x<<" nu are fii."<<endl;
        if (pointer == arbore_curent.radacina)
        {
            out<<x<<" este radacina."<<endl;
            arbore_curent.radacina = NULL;
            delete pointer;
            return;
        }
        if (directie < 0)
            pointer_prev->fiu_stang = NULL;
        else
            pointer_prev->fiu_drept = NULL;
        delete pointer;
        return;
    }

    if (pointer->fiu_stang != NULL && pointer->fiu_drept == NULL)
    {
        out<<"are doar fiu stang "<<x<<endl;
        if (pointer == arbore_curent.radacina){
            out<<"este radacina "<<x<<endl;
            arbore_curent.radacina = pointer->fiu_stang;
            delete pointer;
            return;
        }
        if (directie < 0)
            pointer_prev->fiu_stang = pointer->fiu_stang;
        else
            pointer_prev->fiu_drept = pointer->fiu_stang;
        delete pointer;
        return;
    }

    if (pointer->fiu_stang == NULL && pointer->fiu_drept != NULL)
    {
        out<<"are doar fiu stang "<<x<<endl;
        if (pointer == arbore_curent.radacina){
            out<<"este radacina "<<x<<endl;
            arbore_curent.radacina = pointer->fiu_drept;
            delete pointer;
            return;
        }
        if (directie < 0)
            pointer_prev->fiu_stang = pointer->fiu_drept;
        else
            pointer_prev->fiu_drept = pointer->fiu_drept;
        delete pointer;
        return;
    }

    out<<"are ambii fii "<<x<<endl;
    nod_arbore* pointer2 = pointer->fiu_stang;
    while (pointer2->fiu_drept != NULL)
        pointer2 = pointer2->fiu_drept;

    int valoare = pointer2->val;
    out<<"nodul "<<valoare<<" il poate inlocui pe "<<x<<endl;
    sterge(arbore_curent, valoare);

    pointer->val = valoare;
}

nod_arbore* cautare(nod_arbore* radacina, int val_de_cautat)
{

    nod_arbore* pointer_prev = NULL;
    nod_arbore* pointer = radacina;
    while (pointer != NULL && pointer->val != val_de_cautat)//merge pe principiul de la inserare, adica in functie de relatia dintre valoarea cautata si cea curenta ( > , < , = ) pointer-ul ia valoarea fiului respectiv
    {
        pointer_prev = pointer;
        if (val_de_cautat <= pointer->val)
            pointer = pointer->fiu_stang;
        else
            pointer = pointer->fiu_drept;
    }

    if (pointer == NULL) // daca pointerul a ramas NULL de la declarare inseamna ca nu exista valoarea cautata in bst
    {
        out<<"Nu exista "<<val_de_cautat<<endl;
        return NULL;
    }
    return pointer;
}

int este_in(arbore arbore_curent, int x) // cauta valoarea cu functia cautare si returneaza 1 sau 0
{
    nod_arbore* pointer_nod = cautare(arbore_curent.radacina, x);
    if(pointer_nod != NULL)
        return 1;
    return 0;
}

nod_arbore* succesor(arbore arbore_curent, int val_succesor)
{
    /*
    cauta mai intai pointer-ul al carui valoare este data, iar functia parcurge
    fiecare fiu din stanga daca exista dupa ce pargurge o singura data fiul din dreapta
    , adica cel mai mic mai mare
    */
    nod_arbore* nod_succesor = cautare(arbore_curent.radacina,val_succesor);

    if(nod_succesor->fiu_drept == NULL)
    {
        out<<"Nu exista succesor ";
        nod_arbore* nod_temp;
        nod_temp->val = 0;
        return nod_temp;
    }
    nod_succesor = nod_succesor->fiu_drept;
    while(nod_succesor->fiu_stang != NULL)
    {
        nod_succesor = nod_succesor->fiu_stang;
    }
    return nod_succesor;
}

nod_arbore* predecesor(arbore arbore_curent, int val_predecesor)
{
    /*
    cauta mai intai pointer-ul al carui valoare este data, iar functia parcurge
    fiecare fiu din dreapta daca exista dupa ce pargurge o singura data fiul din stanga
    , adica cel mai mare mai mic
    */
    nod_arbore* nod_predecesor = cautare(arbore_curent.radacina,val_predecesor);
    if(nod_predecesor->fiu_stang == NULL)
    {
        out<<"Nu exista predecesor ";
        nod_arbore* nod_temp;
        nod_temp->val = 0;
        return nod_temp;
    }
    nod_predecesor = nod_predecesor->fiu_stang;
    while(nod_predecesor->fiu_drept != NULL)
    {
        nod_predecesor = nod_predecesor->fiu_drept;
    }
    return nod_predecesor;
}

int min(arbore arbore_curent)
{
    /*
    cea mai mica valoare este de fapt cea mai din stanga, adica parcurge pana ajunge la
    NULL fiii din stanga
    */
    nod_arbore* nod_min = arbore_curent.radacina;
    if(nod_min == NULL)
        return 0;
    int minim = 0;
    while(nod_min->fiu_stang != NULL)
    {
        minim = nod_min->val;
        nod_min = nod_min->fiu_stang;
    }
    minim = nod_min->val;
    return minim;
}

int max(arbore arbore_curent)
{
    /*
    cea mai mare valoare este de fapt cea mai din dreapta, adica parcurge pana ajunge la
    NULL fiii din dreapta
    */
    nod_arbore* nod_max = arbore_curent.radacina;
    if(nod_max == NULL)
        return 0;
    int maxim = 0;
    while(nod_max->fiu_drept != NULL)
    {
        maxim = nod_max->val;
        nod_max = nod_max->fiu_drept;
    }
    maxim = nod_max->val;
    return maxim;
}

int nr_elem(nod_arbore* nod_arbore)
{
    /*
    functia calculeaza recursiv astfel, daca arborele este gol atunci returneaza 0, altfel
    returneaza numarul de elemente al subarborelui stang + 1 + numarul de elemente al subarborelui drept
    */
    if (nod_arbore == NULL)
        return 0;
    else
        return(nr_elem(nod_arbore->fiu_stang) + 1 + nr_elem(nod_arbore->fiu_drept));
}

int cardinal(arbore arbore_curent) // apeleaza functia nr_elemente de la radacina
{
    return nr_elem(arbore_curent.radacina);
}

int main()
{
    
    int val_introdus;
    arbore arb;
    int comanda = 4;

    /*
    !!!

    Din cauza ca exemplele ocupa foarte mult spatiu, 
    este nevoie ca fiecare exemplu sa fie executat individual.
    
    Pentru asta, este nevoie sa se schimbe valoarea variabilei "comanda"
    in numarul exemplului care se doreste a fi executat. (de la 1 pana la 5)

    !!!
    */

    switch (comanda)
    {
    case 1:
        nume_fis = "exemplu1.in";
        break;
    case 2:
        nume_fis = "exemplu2.in";
        break;
    case 3:
        nume_fis = "exemplu3.in";
        break;
    case 4:
        nume_fis = "exemplu4.in";
        break;
    case 5:
        nume_fis = "exemplu5.in";
        break;
    default:
        break;
    }
    ifstream in(nume_fis);

    
    out<<"Exemplul "<<comanda<<":"<<endl;

    while(in>>val_introdus)
    {
        insereaza(arb,val_introdus);
    }
    

    // arb.SRD();
    // arb.RSD();

    out<<"Elementul minim: ";
    out<<min(arb)<<endl;

    out<<"Elementul maxim: ";
    out<<max(arb)<<endl;

    out<<"Succesorul lui 12: ";

    out<<succesor(arb, 12)->val<<endl;

    out<<"Predecesorul lui 12: ";
    out<<predecesor(arb, 11)->val<<endl;

    out<<"Al 13-lea element: ";
    out<<arb.k_element(0)<<endl;

    out<<"Cardinalul arborelui: ";
    out<<cardinal(arb)<<endl;

    out<<"Elementul ";
    out<<este_in(arb, 11);
    out<<" este in arbore";

    out.close();
    return 0;  
}
