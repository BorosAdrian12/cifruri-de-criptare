#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random> // For random number generation
#include <ctime>  // For seeding the random number generator

using namespace std;
// Function to print the binary representation of any data type

struct ArrayTransport{ // i needed for transporting dynamic array
int* arr;
int size;
};
struct Str{
unsigned char *arr;
int size;
};
void printBinary(const void *data, size_t size) {
    const unsigned char *byte = (const unsigned char *)data;

    for (size_t i = 0; i < size; i++) {
        for (int bit = 7; bit >= 0; bit--) {
            unsigned char mask = 1 << bit;
            printf("%d", (byte[i] & mask) ? 1 : 0);
        }
        printf(" ");
    }
}
void printBinary2Row(const void *data, size_t size) {
    const unsigned char *byte = (const unsigned char *)data;

    for (size_t i = 0; i < size; i++) {
        if(i%8==0){
            printf("\n");
        }
        for (int bit = 7; bit >= 0; bit--) {
            unsigned char mask = 1 << bit;
            printf("%d", (byte[i] & mask) ? 1 : 0);
        }

        printf(" ");
    }
}
void setBit(char *c, int position) {
    *c |= (1 << position);
}

void clearBit(char *c, int position) {
    //*c &= ~(1 << position);
    *c= *c & ~(1<<position);
}
// o sa trebuiasca si la dreapta
bool getBit(unsigned char* key1 , int size , int poz){
//ar trebuii sa faci ceva in caz ca ii over dar nu stiu ce
//conventie , poz o sa fie intre 0-7 , precum ar fi index la un array
unsigned char aux=key1[poz/8];
return (aux>>(7-(poz%8)))%2;
}

void shiftArrayLeft(unsigned char *array,int size,short shift){
    unsigned char start , end;
    //memorez capetele ca sa nu se incurce
    start=array[0];
    end=array[size-1];

    for(int i=0;i<size-1;i++){
        array[i]=((array[i]<<shift)|(array[i+1]>>(8-shift)));
    }
    array[size-1]=((array[size-1]<<shift)|(start>>(8-shift)));
}
void shiftArrayRight(unsigned char *array,int size,short shift){
    unsigned char start , end;
    //memorez capetele ca sa nu se incurce
    start=array[0];
    end=array[size-1];

    for(int i=size;i>0;i--){
        array[i]=((array[i]>>shift)|(array[i-1]<<(8-shift)));
    }
    array[0]=((array[0]>>shift)|(end<<(8-shift)));
}
ArrayTransport generateFalsePosition(int realPosition){
    /*
    IMPORTANT!!!
    i need minimum of 32 characters , aka real position
    */

    //realPosition - number of letter in message
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
     std::random_device rd; // Obtain a random seed from the hardware
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr))); // Use the Mersenne Twister engine
    int min=20,max=80;
    std::uniform_int_distribution<int> distribution(min, max);//generate a number between min an max , aka percentage
    int NR_MINIM_POZITI=16;// ii pentru a avea 32 de charuri pt tabel
    int percentage =distribution(rng);
    int falsePosition=(realPosition*(percentage/100.0));

    int size=realPosition+falsePosition, index=0;
    if(size<NR_MINIM_POZITI){
        size=NR_MINIM_POZITI;
        falsePosition=size-realPosition;
    }

    int *positions = new int[size];
    //prepare for shuffle
    for(index;index<realPosition;index++)
        positions[index]=0;
    //cout<<index<<endl;
    for(index;index<size;index++)
        positions[index]=1;
    //cout<<index<<endl;

    //std::mt19937 rng(std::rand());
    //cout<<sizeof(positions)<<endl<<size<<endl;
    std::shuffle(positions, positions + size, rng);
    //<<sizeof(positions)<<endl<<size<<endl;
    //for(int i=0;i<size;i++)
    //    cout<<positions[i]<<' ';
    ArrayTransport aux;
    aux.arr=positions;
    aux.size=size;
    return aux;
}
Str generateTable(int size){
/*
    IMPORTANT!!!
    i need minimum of 32 characters , aka real position
    */

    //realPosition - number of letter in message
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
     std::random_device rd; // Obtain a random seed from the hardware
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr))); // Use the Mersenne Twister engine
    int min=20,max=80;

    unsigned char *positions = new unsigned char[size];
    //prepare for shuffle
    for(int i=0 ;i<size;i++)
        positions[i]=i+1;


    std::shuffle(positions, positions + size, rng);
    //<<sizeof(positions)<<endl<<size<<endl;
    //for(int i=0;i<size;i++)
    //    cout<<positions[i]<<' ';
    Str aux;
    aux.arr=positions;
    aux.size=size;
    return aux;
}
void letSetBit(unsigned char* x,bool state,int position){
//conventie , lucram de la 0-7 , stanga la dreapta ca si in array , numaratoarea incepe de la 0
//asta ar trebuii sa fie pentru a seta bitul cheita in pozitia respectiva
unsigned char aux ;
aux= x[0];
aux=(aux<<(position+1));
aux=aux>>(position+1);
//printBinary(&aux,1);
x[0]=x[0]>>(7-position+1);
x[0]=x[0]<<1;
x[0]+=state;
x[0]=x[0]<<(7-position);
x[0]=x[0]|aux;
}

Str implementFalsePosition(unsigned char* mesaj,int marimeMesaj,ArrayTransport vec,unsigned char *key1 , int marimeKey1){
/*
===conventie===
-cand se face XOR cu cheita se face cu key1 , nu cu copyKey1
-copyKey1 determina pozitia bitului
===============
ce trebuie sa faci :
-implementeaza random
-baga bitul
--aici te folosesti de o copie key1 pentru pozitia bitului
--o sa faci cu %8
-faci si chestia cu xor tot odata

*/

unsigned char* pozImp,*copyKey1;
copyKey1=new unsigned char[marimeKey1];
for(int i=0;i<marimeKey1;i++){
    copyKey1[i]=key1[i];
}

pozImp=new unsigned char[vec.size*2];
int indexMesaj=0;
//printBinary(key1,marimeKey1);cout<<endl;
//se implementeaza pozitile false
    for(int i=0;i<vec.size;i++){
        //cu asta ar trebuii sa fie dar inca testez si nu stiu daca ii bine sau nu
        pozImp[i*2+1]=rand() % 256;
        int poz=copyKey1[0]%8;
        bool state=getBit(key1,marimeKey1,i%marimeKey1);;
        if(vec.arr[i]==0&&indexMesaj<marimeMesaj/*maby add marimeMesaj*/){//adica ca ii litara din mesaj , aka e 0 pozitia , aka adevarata
            pozImp[i*2]=mesaj[indexMesaj];
            indexMesaj++;
            pozImp[i*2+1]=0b00000000;//provizoriu , trebuie adaugat de la cheie
            letSetBit(&pozImp[i*2+1],getBit(copyKey1,marimeKey1,0),poz);
        }else{//NU ESTE LITERA DIN MESAJ aka falsa
            pozImp[i*2]=rand() % 256;
            pozImp[i*2+1]=0b11111111;//provizoriu , trebuie adaugat de la cheie
            letSetBit(&pozImp[i*2+1],(getBit(copyKey1,marimeKey1,0)^1),poz);
        }
        //pozImp[i*2+1]=
        /*pentru acum am sa setez pe ultimul bit , dar in viitor am sa fac sa ia aleator
        if(key1[i%marimeKey1]){//ii 1
            pozImp[i*2+1]=
        }else{//ii 0
            pozImp[i*2+1]=
        }*/
        //xor pe caracter
        //xor pe cheita
        //cout<<poz<<' ';
        pozImp[i*2]=pozImp[i*2]^pozImp[i*2+1];
        pozImp[i*2+1]=pozImp[i*2+1]^key1[i%marimeKey1];
        shiftArrayLeft(copyKey1,marimeKey1,1);

    }
    //cout<<endl;

Str aux;
aux.arr=pozImp;
aux.size=vec.size*2;
return aux;
}
Str deimplementatFalsePosition(unsigned char* mesajCriptat,int marimeMesajCriptat,unsigned char *key1, int marimeKey1){
unsigned char* mesaj,*copyKey1;
copyKey1=new unsigned char[marimeKey1];
int index=0;
    for(int i=0;i<marimeKey1;i++){
        copyKey1[i]=key1[i];
    }
    //printBinary2Row(mesajCriptat,marimeMesajCriptat);cout<<endl;

    for(int i=0;i<marimeMesajCriptat/2;i++){
        mesajCriptat[i*2+1]=mesajCriptat[i*2+1]^key1[i%marimeKey1];
        mesajCriptat[i*2]=mesajCriptat[i*2]^mesajCriptat[i*2+1];
        }

    //cout<<"am scos masca"<<endl;
    //cout<<marimeMesajCriptat<<endl;
    //printBinary2Row(mesajCriptat,marimeMesajCriptat);cout<<endl;


mesaj=new unsigned char[marimeMesajCriptat];
    int marime=marimeMesajCriptat/2;
    for(int i=0;i<marime;i++){
        int poz=copyKey1[0]%8;
        //mesajCriptat[i*2+1]=mesajCriptat[i*2+1]^key1[i%marimeKey1];
        //mesajCriptat[i*2]=mesajCriptat[i*2]^mesajCriptat[i*2+1];

        if(getBit(&mesajCriptat[i*2+1],1,poz)==getBit(copyKey1,marimeKey1,0)){
            //aici o sa adaugi in mesaj

            //aici ar trebuii sa faci chestia asta pt optimizare
            //mesajCriptat[i*2]=mesajCriptat[i*2]^mesajCriptat[i*2+1];
            mesaj[index]=mesajCriptat[i*2];
            //cout<<' '<<mesaj[index]<<'|'<<(char)mesajCriptat[i*2];
            index++;
        }
        //cout<<"["<<getBit(&mesajCriptat[i*2+1],1,poz)<<getBit(copyKey1,marimeKey1,0)<<"] ";
        //cout<<poz<<' ';
        shiftArrayLeft(copyKey1,marimeKey1,1);

    }
    //cout<<endl;

Str aux;
aux.arr=mesaj;
aux.size=index;
aux.arr[index]='\n';
//cout<<"aux:"<<aux.arr<<endl;
return aux;


/*
cout<<"index:"<<index<<endl;
cout<<"mesaj:"<<mesaj<<endl;
printBinary2Row(mesajCriptat,marimeMesajCriptat);cout<<endl;
/**/
}
void amestecare(unsigned char* mesaj , int marimeMesaj , unsigned char* tabel , int marimeTabel){
    //cout<<endl;
    for(int i=0;i<marimeMesaj;i++){
      //  cout<<(i+tabel[i%marimeTabel])%marimeMesaj<<' ';
        swap(mesaj[i],mesaj[(i+tabel[i%marimeTabel])%marimeMesaj]);
    }
    //cout<<endl;
}
void deAmestecare(unsigned char* mesaj , int marimeMesaj , unsigned char* tabel , int marimeTabel){
    //cout<<endl;
    for(int i=marimeMesaj-1;i>=0;i--){
        //cout<<(i+tabel[i%marimeTabel])%marimeMesaj<<' ';
        swap(mesaj[i],mesaj[(i+tabel[i%marimeTabel])%marimeMesaj]);
    }
    //cout<<endl;
}
Str criptare(unsigned char* mesaj , int marimeMesaj , unsigned char *key1 , int marimeKey1,unsigned char *tabel , int tableSize){
cout<<"suntem in CRIPTARE"<<endl;
for(int i=0;i<tableSize;i++)
    cout<<(int)tabel[i]<<' ';
cout<<endl;

ArrayTransport vecPoz;
vecPoz=generateFalsePosition(marimeMesaj);
unsigned char* mesajCriptat;
int marimeMesajCriptat;
Str test;
/*
cout<<"debug:\n";
cout<<"marime mesaj:"<<marimeMesaj<<endl;
cout<<"marime vec:"<<vecPoz.size<<endl;
cout<<"vec poz:";
for(int i=0;i<vecPoz.size;i++)
    cout<<vecPoz.arr[i];
cout<<endl;
*/

test=implementFalsePosition(mesaj,marimeMesaj,vecPoz,key1,marimeKey1);

mesajCriptat=test.arr;
marimeMesajCriptat=test.size;
//shiftArrayLeft(mesajCriptat,marimeMesajCriptat,4);
//amestecare(mesajCriptat,marimeMesajCriptat,tabel,tableSize);

//printBinary2Row(mesajCriptat,marimeMesajCriptat);cout<<endl;
//ii pentru testare comanda de mai jos
//cout<<"debug";
//deimplementatFalsePosition(mesajCriptat,marimeMesajCriptat,key1,marimeKey1);
Str aux;
//aux=test;
aux.arr=mesajCriptat;
aux.size=marimeMesajCriptat;
return aux;
}
void deCriptare(unsigned char* mesaj , int marimeMesaj , unsigned char *key1 , int marimeKey1,unsigned char *tabel , int tableSize){
cout<<"suntem in decriptare"<<endl;
for(int i=0;i<tableSize;i++)
    cout<<(int)tabel[i]<<' ';
cout<<endl;
//deAmestecare(mesaj,marimeMesaj,tabel,tableSize);
//shiftArrayRight(mesaj,marimeMesaj,4);
Str aux;

aux=deimplementatFalsePosition(mesaj,marimeMesaj,key1,marimeKey1);
cout<<"din decriptare mesaj returnat:";
for(int i=0;i<aux.size;i++)
    cout<<aux.arr[i];
cout<<endl;
}

void regenerateTable(unsigned char *tabel,int marimeTabel){
    unsigned char *newTabe=new unsigned char[marimeTabel];
    //-1 pt ca ultima o ia pe prima
    for(int i=0;i<marimeTabel-1;i++){
        //-1 ii pentru rectificarea in memorie
        newTabe[tabel[i]-1]=tabel[i+1];
    }

    newTabe[tabel[marimeTabel-1]-1]=tabel[0];
    for(int i=0;i<marimeTabel;i++){
        bool flag=1;
        for(int j=0;j<marimeTabel;j++)
            if(tabel[j]==newTabe[i]){
                flag=0;
                break;
            }
        if(flag){
            cout<<"EROARE"<<endl;
        }
    }
    for(int i=0;i<marimeTabel;i++){
        tabel[i]=newTabe[i];
    }
}

Str Criptare(unsigned char* mesaj , int marimeMesaj , unsigned char *key1 , int marimeKey1,unsigned char *tabel , int marimeTabel){
ArrayTransport vecPoz;
vecPoz=generateFalsePosition(marimeMesaj);

Str mesajCuPozitiFalse;
mesajCuPozitiFalse=implementFalsePosition(mesaj,marimeMesaj,vecPoz,key1,marimeKey1);
int iteratii=6;
for(int i=0;i<iteratii;i++){
    //shiftArrayLeft(mesajCuPozitiFalse.arr,mesajCuPozitiFalse.size,4);
    amestecare(mesajCuPozitiFalse.arr,mesajCuPozitiFalse.size,tabel,marimeTabel);
    regenerateTable(tabel,marimeTabel);
    for(int i=0;i<marimeTabel;i++){
            cout<<setw(2)<<(int)tabel[i]<<' ';
        }
        cout<<endl;
}
return mesajCuPozitiFalse;
}
Str Decriptare(unsigned char* mesaj , int marimeMesaj , unsigned char *key1 , int marimeKey1,unsigned char *tabel , int marimeTabel){
unsigned char **listaTabele;int iteratii=6;

listaTabele=new unsigned char*[iteratii];

for(int i=0;i<iteratii;i++){
    listaTabele[i]=new unsigned char[marimeTabel];
    for(int j=0;j<marimeTabel;j++){
        listaTabele[i][j]=tabel[j];
    }
    regenerateTable(tabel,marimeTabel);
}





    for(int i=0;i<iteratii;i++){
        deAmestecare(mesaj,marimeMesaj,listaTabele[iteratii-i-1],marimeTabel);
       // shiftArrayRight(mesaj,marimeMesaj,4);
       for(int j=0;j<marimeTabel;j++){
            cout<<setw(2)<<(int)listaTabele[i][j]<<' ';
        }
        cout<<endl;
    }

    Str aux = deimplementatFalsePosition(mesaj,marimeMesaj,key1,marimeKey1);

    return aux;
}
void test(){
unsigned char *key1,tabel[]={1 ,9 ,21 ,20 ,29 ,25 ,3 ,2 ,30 ,28 ,18 ,23 ,5 ,8 ,12 ,4 ,32 ,15 ,27 ,7 ,24 ,19 ,11 ,16 ,17 ,31 ,6 ,10 ,14 ,26 ,13 ,22 };
    int sizeKey1=8;
    unsigned char mesaj[]="A fost odata ca nici odata!AAAAAAAA";
    int marimeTabel=32;
    for(int i=0;i<10;i++){
        regenerateTable(tabel,marimeTabel);
        for(int i=0;i<marimeTabel;i++){
            cout<<setw(2)<<(int)tabel[i]<<' ';
        }
        cout<<endl;
    }

}
void program(){
unsigned char *key1,tabel[]={1 ,9 ,21 ,20 ,29 ,25 ,3 ,2 ,30 ,28 ,18 ,23 ,5 ,8 ,12 ,4 ,32 ,15 ,27 ,7 ,24 ,19 ,11 ,16 ,17 ,31 ,6 ,10 ,14 ,26 ,13 ,22 };
unsigned char copytabel[]={1 ,9 ,21 ,20 ,29 ,25 ,3 ,2 ,30 ,28 ,18 ,23 ,5 ,8 ,12 ,4 ,32 ,15 ,27 ,7 ,24 ,19 ,11 ,16 ,17 ,31 ,6 ,10 ,14 ,26 ,13 ,22 };
    int sizeKey1=8;
    unsigned char mesaj[]="A fost odata ca nici odata! acesta este o alta bucata de text si vreau sa testez daca merge !";
    int marimeMesaj=sizeof(mesaj)/sizeof(mesaj[0]),marimeTabel=32;
    /*
    ,TABLE[]={5 ,25 ,6 ,32 ,16 ,19 ,15 ,17 ,11 ,29 ,3 ,7 ,4 ,27 ,23 ,20 ,24 ,9 ,26 ,2 ,10 ,28 ,18 ,31 ,8 ,1 ,12 ,21 ,14 ,22 ,30 ,13}

    Str tablegen;
    tablegen=generateTable(32);
    tabel=tablegen.arr;
    marimeTabel=tablegen.size;
    */
    key1=new unsigned char[sizeKey1];
    key1[0]='A';
    key1[1]='B';
    key1[2]='c';
    key1[3]='D';
    key1[4]='E';
    key1[5]='F';
    key1[6]='G';
    key1[7]='H';
    cout<<"mesaj:"<<mesaj<<endl;
    Str mesajCriptat;
    mesajCriptat=Criptare(mesaj,marimeMesaj,key1,sizeKey1,tabel,marimeTabel);
    cout<<"-=-=-=- IN PROGRAM | MARIME MESAJ CRIPTAT :"<<mesajCriptat.size<<endl;
    //printBinary2Row(mesajCriptat.arr,mesajCriptat.size);cout<<endl;
    //key1[1]='a';
    //tabel[0]=9;
    //tabel[1]=4;
    //tabel[5]=1;
    //tabel[7]=2;
    mesajCriptat=Decriptare(mesajCriptat.arr,mesajCriptat.size,key1,sizeKey1,copytabel,marimeTabel);
    cout<<"Final : mesaj decriptat:"<<mesajCriptat.arr<<endl;
}

int main() {



//test();
program();

    return 0;
    }


