import numpy
import random
import time
from pprint import pprint
class chunk :
         def __init__(self,index,revers,text,l):
            self.index=index
            self.revers=revers
            self.text=text
            self.len=l
            pass
         def prt(self):
            return "\nindex:{}\trevers:{}\ttext:{:<10}len:{}".format(self.index,self.revers,self.text,self.len)
            pass
         def __str__(self) -> str:
            return self.prt()
            pass
         def __repr__(self):
            return self.prt()

class AlgoCript:
   lit="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
   listPar=['A','C','E','G','I','K','M','O','Q','S','U','W','Y','b','d','f','h','j','l','n','p','r','t','v','x','z']
   listImp=['B','D','F','H','J','L','N','P','R','T','V','X','Z','a','c','e','g','i','k','m','o','q','s','u','w','y']
   def __init__(self) -> None:
      pass
   def keyIntToArr(self,key):
      keyarr=[]
      # print(key)
      while key>0:
         # print((int)(key%10))
         keyarr.append((int)(key%10))
         key=(int)(key/10)
      keyarr=keyarr[::-1]
      return keyarr
      
   def Criptare(self,mesaj,key,ran):

      mc=[]
      key=self.keyIntToArr(key)
      #mc = [
      # [caracterul , car.confirmare]
      # ]
      # print(key)
      j=0
      
      #aici determin literele false . SI II SCRIS PROST
      for i in range(len(ran)):
         if(ran[i]==1):#ii o litera falsa 
            if key[i%len(key)]%2==0:#ii par
               mc.append([
                  self.lit[random.randint(0,len(self.lit)-1)],
                  self.listImp[random.randint(0,len(self.listImp)-1)]
                  ])
            else:
               mc.append([
                  self.lit[random.randint(0,len(self.lit)-1)],
                  self.listPar[random.randint(0,len(self.listPar)-1)]
                  ])
         else:#litere adevarate
            if key[i%len(key)]%2==0:#ii par
               mc.append([
                  mesaj[j],
                  self.listPar[random.randint(0,len(self.listPar)-1)]
                  ])
            else:
               mc.append([
                  mesaj[j],
                  self.listImp[random.randint(0,len(self.listImp)-1)]
                  ])
            j=j+1
      

      
      # print(mc)
      #aici il transform intr-un string
      m=''
      for i in range(len(mc)):
         m+=mc[i][0]+mc[i][1]

      
      if(random.randint(0,1)==1):
         m+=self.lit[random.randint(0,len(self.lit)-1)]


      marimefaze1=len(m)
      # print('mesaj in prima faza de criptare :\n',m+'-----',marimefaze1)
      # return
      sp=[]
      j=0
      while (len(m)>0):
         sp.append(m[:key[j%len(key)]])
         m=m[key[j%len(key)]:]
         # print(key[j%len(key)])
         j+=1
      # print('chunk mai exact :\n',sp,marimefaze1)
      listSwap=[]
      listDeb=[]
      for i in range(len(sp)):
         listDeb.append(chunk(i,False,sp[i],key[i%len(key)]))
      # print('marime sp :',len(sp))
      for i in range(len(sp)):
         #ce ii mai jos ar terbuii mai incolo sa fie implementat ca 
         #inca nu am facut decriptarea si o sa imi fie mai simplu 
         #pentru viitor
         if(i%2==0):
            sp[i]=sp[i][::-1]
            listDeb[i].revers^=True
         aux=sp[i]
         sp[i]=sp[(i+key[i%len(key)])%len(sp)]
         sp[(i+key[i%len(key)])%len(sp)]=aux

         aux=listDeb[i]
         listDeb[i]=listDeb[(i+key[i%len(key)])%len(sp)]
         listDeb[(i+key[i%len(key)])%len(sp)]=aux

         listSwap.append((i,(i+key[i%len(key)])%len(sp)))
      # print('lista mutari chunk:\n',listSwap)
      # print(listDeb)
      # for i in range(len(sp)):
      #    if(i%2==0):
      #       sp[i]=sp[i][::-1]
      # print(sp)
      finalMessaje=''
      j=0
      col=0
      rand=0
      i=0
      while (marimefaze1 > 0):
         if(rand==len(sp)):
            rand=0
            col+=1#s-a trecut la urmatorul rand
         if(col<len(sp[rand])):
            finalMessaje+=sp[rand][col]
            marimefaze1-=1

         rand+=1
      # print('"'+finalMessaje+'"',marimefaze1,len(finalMessaje))
      return finalMessaje
       #REZUMAT:
       # partea a doua din cifru nu ii asa de buna 
       # trebuie vazut pentru ca nu amesteca bine 
       # CAUZA : ii din cauza cheii
       # REZOLVARE :
       #    a)gasesti o alta metoda de amestecare
       #       *as putea sa adaug un numar impar par de litere dupa litere false
       #       *poate as putea sa amestec umpic randul 
       #    b)vezi cum poti sa faci sa nu fie asa deterministic 
       #       pozitiile in cazul unor chei proaste
       #    c)da sift in mesaj , nu mai fa cacatu asta de matrice  
       #    d)uitate sa vezi in codul original 
       # daca vreau sa maresc securitatea as putea sa fac ceva hash 
   def Decriptare(self,mesaj,key):
      count=0
      key=self.keyIntToArr(key)
      j=0
      maxLetM=0
      list=[]
      print('--------------\n','mesaj criptat :\n',mesaj)
      while maxLetM<len(mesaj):
         maxLetM+=key[j%len(key)]
         j+=1
         count+=1
      # print(count,maxLetM, len(mesaj))
      str=""
      

      for i in range(count):
         list.append(chunk(i,False,'',key[i%len(key)]))
      if(maxLetM!=len(mesaj)):
         list[-1].len-=maxLetM-len(mesaj)
      
      listSwap=[]
      for i in range(count):
         #ce ii mai jos ar terbuii mai incolo sa fie implementat ca 
         #inca nu am facut decriptarea si o sa imi fie mai simplu 
         #pentru viitor
         if(i%2==0):
            list[i].revers^=True
         aux=list[i]
         list[i]=list[(i+key[i%len(key)])%count]
         list[(i+key[i%len(key)])%count]=aux
         listSwap.append((i,(i+key[i%len(key)])%count))
      # print('lista mutari chunk:\n',listSwap)
      # print(list)
      
      # for i in range(len(mesaj)):
      #    list[i%count].text+=mesaj[i]
      letAdded=0
      i=0
      while(letAdded<len(mesaj)):
         indexList=i%count
         # print(letAdded)
         if(list[indexList].len>0):
            list[indexList].len-=1
            list[indexList].text+=mesaj[letAdded]
            letAdded+=1
         i+=1
      # print(list)
      #aici ii ceva cu reverse 
      for i in range(len(list)):
         if(list[i].revers):
           list[i].text= list[i].text[::-1]
           list[i].revers=False
      #poate ii bun de ceva asta 
      # for i in range(len(list)):
      #    aux=list[i]
      #    list[i]=list[list[i].index]
      #    list[list[i].index]=aux
      l2=[None] * len(list)
      for i in range(len(list)):
         l2[list[i].index]=list[i]
      list=l2
      # print(list)
      # print(l2)
      textFinal=''
      for i in range(len(list)):
         textFinal+=list[i].text
      # print('mesaj aproape decriptat:\n',textFinal)
      mesajDecriptat=''
      index=0

      for i in range(0,len(textFinal)-(len(textFinal)%2),2):
         idx=index%len(key)
         # print(idx)
         if(key[idx]%2==0):
            if textFinal[i+1] in self.listPar:
               mesajDecriptat+=textFinal[i]
         else:
            if textFinal[i+1] in self.listImp:
               mesajDecriptat+=textFinal[i]
         index+=1

      # lmes=len(textFinal)-(len(textFinal)%2)
      # print(lmes)
      # while lmes>2:
      #    idx=index%len(key)
      #    print(idx)
      #    if(key[idx]%2==0):
      #       if textFinal[lmes+1] in self.listPar:
      #          mesajDecriptat+=textFinal[lmes]
      #          lmes-=2
      #    else:
      #       if textFinal[lmes+1] in self.listImp:
      #          mesajDecriptat+=textFinal[lmes]
      #          lmes-=2
      #    index+=1

      print('mesaj decriptat','"'+mesajDecriptat+'"')
      return

#ar trebuii sa varieze dar acuma ii doar un test

#pana acum ce avem la algorit introdus
# adauga litere false
# schimba positiile



mesaj="acesta este un mesaj de test"

mesaj=#mesajuldinlicenta
a= AlgoCript()
listArrParty=[]
listMesajeCriptate=[]
key=2323
key=2943734825723
key=292929292929
key=1671935488834
key=#ziua de nastere

encmes="XaaLl SPMtwnzmyfypgGFFtNwQiFFiuaMegnampMEhuTrPppMtnTIoUa exhNlJ eWvZlljaLounZHE udentPutideHHQDi"

# print(a.Decriptare(encmes,key))
for i in range(5):
   procentFalse=(int)(len(mesaj)*(random.randint(2,8)/10))
   arrParity=[0]*len(mesaj)+[1]*procentFalse
   random.shuffle(arrParity)
   # print (arrParity)
   
   # key=84582568467
   # key=648489474383


   l=a.Criptare(mesaj,key,arrParity)
   listArrParty.append(arrParity)
   listMesajeCriptate.append(l)
   print('"--'+l+'--"',key)
   time.sleep(0.5)
# print(len(mesaj),l)
# mesajCriptat="CYE7RW9i81unI5pCl64Y0AARModIY 3z2"

for i in range(5):
   a.Decriptare(listMesajeCriptate[i],key)
# key=23
# print("asasdasdasd-----====---")
# for i in range(5):
#    a.Decriptare(listMesajeCriptate[i],key)
# pprint(listArrParty)
