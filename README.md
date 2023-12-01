# EStudent-Projekt

## Opis Projekta

EStudents je aplikacija razvijena u C++ Builder okruženju namijenjena profesorima i učenicima. Klijentska aplikacija omogućuje verifikaciju korisnika putem sigurne komunikacije sa serverskom aplikacijom. Profesori mogu unositi, brisati i pregledavati ocjene svojih predmeta, dok učenici imaju pristup pregledu vlastitih ocjena. Sustav koristi AES algoritam za enkripciju i digitalni potpis kako bi osigurao sigurnu razmjenu podataka između klijenta i servera.

Serverska aplikacija, osim što kontrolira i verificira podatke, dinamički generira tajne ključeve za dodatnu sigurnost. Projekt koristi MS Access bazu podataka za pohranu informacija o korisnicima, predmetima i ocjenama. Unutar projekta ističu se funkcionalnosti poput filtriranja, sortiranja i upravljanja temama. Također, implementirane su sigurnosne mjere poput hashiranja lozinki te primjene digitalnog potpisa za osiguranje integriteta podataka. Sve ove značajke čine projekt EStudents robustnim sustavom za praćenje i upravljanje ocjenama u školskom okruženju.

## Struktura Projekta

- **Klijentska Aplikacija:** Pruža korisničko sučelje za profesore i učenike.
- **Serverska Aplikacija:** Upravlja autentikacijom korisnika, verifikacijom podataka i sigurnom komunikacijom.

## Korištene Tehnologije

- C++ Builder
- MS Access Baza Podataka
- AES i RSA Enkripcija
- Digitalni Potpisi
- Hashing algoritmi (Salt, Peper)
- TCP Protokol (Internetski)

## Ključne Značajke

- Sigurna autentikacija korisnika
- AES enkripcija za sigurnost podataka
- Digitalni potpisi za integritet podataka
- Dinamička generacija tajnih ključeva
- Funkcionalnosti filtriranja i sortiranja
- Upravljanje temama

## Slike

Klijent.exe (Login forma) + Server.exe

![Slika1](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/1c7345b3-a1fc-4fcb-8043-3d16a08f2046)


Klijent.exe (Profesor forma)

![Slika2](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/dc4c0e8b-fe57-4b64-ad86-e295711785df)


Klijent.exe (Student forma)

![Slika3](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/4bbdfb79-c6d4-48f9-817d-6baeb1b0ce52)


Klijent.exe (Autentikacija korisnika)

![image](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/bfd82a67-ab26-4a32-94ca-53d8aba58b08)


Klijent.exe (Promjena teme (svijetlo/tamno))

![image](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/ffaa48aa-ee31-4ddb-b758-27da8080e180)


Klijent.exe (Zapisivanje svake promjene)

![image](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/a74dfc76-68ab-468a-9e0d-f753d8b3fc6f)
![image](https://github.com/AnteDev00/EStudent-Projekt/assets/151842550/2ccc631f-06a6-4513-ab01-aaa683ea9d09)

## I još mnogo toga.. Datoteke se nalaze u zasebnim _branchovima_.

