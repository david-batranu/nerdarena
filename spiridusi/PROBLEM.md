# Treicc

**Time Limit:** 0.3s
**Memory Limit:** 524288KB
**Input File:** `spiridusi.in`
**Output File:** `spiridusi.out`

# Spiriduși (clasele 9-12)

Spiridușii lui Moș Crăciun tocmai au terminat de asamblat o instalație luminoasă. Ea are *n* beculețe dispuse în lanț, numerotate de la *1* la *n*. Acum, spiridușii testează instalația. Mai întîi ei aprind o parte dintre beculețe, apoi fac *q* operații de două tipuri:

- *1 x*: Schimbă starea beculețului *x* (aprinde beculețul dacă era stins și invers).
- *2 x y*: Află cel mai mare număr de beculețe consecutive aprinse între pozițiile *x* și *y* inclusiv.

Ajutați-i pe spiriduși să afle răspunsurile la operațiile de tipul *2*!

## Date de intrare

Fișierul de intrare *spiridusi.in* conține pe prima linie numerele *n* și *q*. Pe următoarea linie apar *n* caractere 0 sau 1, fără spații, indicînd starea inițială a beculețelor (*0* = stins, *1* = aprins). Pe următoarele *q* linii se află cîte o operație, sub una dintre formele *1 x* (cu *1 ≤ x ≤ n*) sau *2 x y* (cu *1 ≤ x ≤ y ≤ n*).

## Date de ieșire

În fișierul de ieșire *spiridusi.out* afișați răspunsul la operațiile de tip *2*, cîte unul pe linie.

## Restricții

- *1 ≤ n, q ≤ 200.000*
- Testele **nu** sînt grupate.

<table style="width: inherit;">
<tr>
<th>subtask </th>
<th>puncte </th>
<th>restricții </th>
</tr>
<tr>
<td> 1 </td>
<td> 30 </td>
<td> *n, q ≤ 20.000* </td>
</tr>
<tr>
<td> 2 </td>
<td> 40 </td>
<td> *n q ≤ 60.000* </td>
</tr>
<tr>
<td> 3 </td>
<td> 30 </td>
<td> Fără restricții suplimentare. </td>
</tr>
</table>

## Exemplu

| spiridusi.in | spiridusi.out |
| --- | --- |
| 9 4
</br>
100110111
</br>
2 4 8
</br>
2 2 3
</br>
1 6
</br>
2 4 8
</br> | 2
</br>
0
</br>
5
</br> |

### Explicație

Pe pozițiile 4-8 se află beculețele *11011* și există două beculețe consecutive aprinse.
</br>
Pe pozițiile 2-3 se află beculețele *00*. Nu există beculețe consecutive aprinse.
</br>
După aprinderea beculețului 6, instalația devine *100111111*.
</br>
Acum pe pozițiile 4-8 există 5 beculețe consecutive aprinse.

<li class="copyright">
©
2012-2026            NerdArena        </li>
<li class="separate"><a href="/">Prima pagină</a></li>
- <a href="/despre-nerdarena">Despre NerdArena</a>
- <a href="/termeni-si-conditii">Termeni și condiții</a>
- <a href="/contact">Contact</a>
<li class="top"><a href="#header">Sari la începutul paginii ↑</a></li>

<a class="badge" rel="license" href="https://creativecommons.org/licenses/by-nc-sa/4.0/">
<img alt="Creative Commons License" src="/static/images/creative-commons.png">
</a>
Cu excepția cazurilor în care se specifică altfel, conținutul
site-ului NerdArena
este publicat sub licența
<a rel="license" href="https://creativecommons.org/licenses/by-nc-sa/4.0/">Creative
Commons Attribution-NonCommercial-ShareAlike 4.0 International</a>.

</body>
</html>
