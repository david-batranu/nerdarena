# Bonus galactic (clasele 11-12)

Parlamentul Federației Galactice, în scădere de popularitate, a hotarît să mai arunce un os ființelor inteligente din galaxie. Mai exact, a hotărît să dea o primă tuturor galaxienilor care împlinesc o sumă rotundă de ani. Propunerea de lege a fost atacată însă de diverși oportuniști, pe motivul că sistemele solare au ani de lungime diferită. După un an de parlamentări federația a decis să ia în considerare vîrsta în Ani Galactici Standard, ceea ce a dus la un val de nemulțumiri din partea celor ce nu se descurcau cu conversiile, căci picasera bacul galactic. Așa încît ei au atacat din nou legea, pe motiv că fiecare specie folosește propria bază de numerație, deci nu se poate defini un număr rotund. Dupa încă un an de deliberări parlamentarii au decis că un număr de ani **N** va fi considerat rotund dacă el se poate scrie sub forma 100...100...0 într-o bază oarecare **B** (exact două cifre 1, și zero sau mai multe cifre 0).

Cîrcotasii nemaiavînd ce spune, legea a fost trecută și a apărut impasul: numărul de cetățeni galactici fiind foarte mare, parlamentarii nu știu să calculeze care vîrste sînt rotunde și care nu, suficient de repede ca cetățenii să primească primele înainte de obștescul lor sfîrșit.

## Cerință

Parlamentul vă roagă pe voi să îl ajutați. Vîrstele cetățenilor variază între $3$ și $2^{64}-1$ ani, iar numărul aproximat al cetățenilor galactici este între $1$ și $100\ 000$. Dîndu-se numărul **N** al cetățenilor precum și fiecare vîrstă **$X_i$**, voi trebuie să calculați pentru fiecare **$X_i$** baza minimă în care ea este număr rotund. Dacă nu exista nici o bază, veți afișa 0.

## Date de intrare

Fișierul de intrare `bonus.in` conține pe prima linie numărul **N**, reprezentînd numărul de cetățeni ai galaxiei. Următoarele **N** linii conțin întregii **$X_1$**, **$X_2$**, ..., **$X_N$**, cîte unul pe linie.

## Date de ieșire

Fișierul de ieșire `bonus.out` va conține **N** linii. Linia **i** va conține un singur număr, cea mai mică bază în care **$X_i$** este număr rotund, sau zero dacă acea bază nu există.

## Restricții

* $1 \le N \le 100,000$
* pentru 20% din teste $1 \le N \le 10,000$
* $3 \le X_i \le 2^{64}-1$ pentru orice $1 \le i \le N$

## Exemplu

| `bonus.in` | `bonus.out` | Explicație |
| :--- | :--- | :--- |
| 5<br>36<br>26<br>80<br>56<br>125 | 2<br>5<br>2<br>7<br>124 | 36 = $100100_2$<br>26 = $101_5$<br>80 = $1010000_2$<br>56 = $110_7$<br>125 = $11_{124}$<br><br>Unele numere sînt rotunde în multiple baze. De exemplu 36 este rotund și în baza 3: 36 = $1100_3$. Însă răspunsul trebuie să fie baza cea mai mică. |
