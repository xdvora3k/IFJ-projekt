// Program 1: Vypocet faktorialu (iterativne)
package main

func main() {
  print("Zadejte cislo pro vypocet faktorialu: ###LL")
  a := 1+1
  b :=1+a
  b = 1+a

	a, _ = inputi()

  if a < 0 {
    print("Faktorial nejde spocitat!\n")
  } else {

    for vysl := 1 ; a > 0; a = a - 1 {
      vysl = vysl * a
    }
    print("Vysledek je ",vysl,"\n")
  }
}