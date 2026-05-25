# Treicc

**Time Limit:** 0.3s
**Memory Limit:** 131072KB
**Input File:** `switch5.in`
**Output File:** `switch5.out`

# Switch5 (lot liceu)

A fost odată un om așa de sărman, că singura lui avere era un tablou cu *N* linii (numerotate de la *1* la *N*) și *N* coloane (numerotate de la *1* la *N*), iar în fiecare poziție a acestui tablou era câte un bec și câte un comutator. Inițial toate becurile sunt stinse. Pentru orice acționare a unui comutator, atunci, în mod simultan, stările becurilor din poziția respectivă și din cele cel mult patru poziții vecine se modifică, adică becurile stinse se aprind, iar cele aprinse se sting.

## Cerință

Ajutați-l pe bietul om să găsească o posibilitate de acționare a comutatoarelor astfel încât toate becurile să devină aprinse.

## Date de intrare

Fișierul de intrare *switch5.in* conține pe prima linie un singur număr natural *N*.

## Date de ieșire

Fișierul de ieșire *switch5.out* conține o matrice dată prin *N* linii, pe fiecare linie aflându-se exact *N* cifre *0* sau *1*, fără spații. Matricea aceasta va avea valoarea *0* la o poziție *(i, j)* dacă la această poziție comutatorul nu a fost acționat, sau va avea valoarea *1* dacă la această poziție comutatorul a fost acționat.

## Restricții

- *1 ≤ N ≤ 1000*
- Fișierul de ieșire nu va conține nici un caracter spațiu.
- În concurs, limita de memorie a fost de *256MB*.

## Exemplu

| switch5.in | switch5.out |
| --- | --- |
| 4
</br> | 0100
</br>
0001
</br>
1000
</br>
0010
</br> |

### Explicație

Au fost acționate comutatoarele de la pozițiile *(1, 2)*, *(2, 4)*, *(3, 1)* și *(4, 3)*.
</br>
Un alt rezultat corect ar fi fost:
</br>
1111
</br>
1001
</br>
1111
</br>
0000

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
