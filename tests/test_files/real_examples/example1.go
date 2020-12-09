// Program 1: Vypocet faktorialu (iterativne)
package main

func main() {
  print("Zadejte cislo pro vypocet faktorialu: ###LL")
  a := 122
  b := 1.5
  c := "s"



  if a < 0 {
    print("Faktorial nejde spocitat!\n",a)
  } else {
    vysl := 1
    for ; a > 0; a = a - 1 {
      vysl = vysl * a
    }
    print("Vysledek je ",vysl,"\n")
  }
}