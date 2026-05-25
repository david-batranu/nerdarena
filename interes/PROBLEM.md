# Treicc

**Time Limit:** 0.7s
**Memory Limit:** 524288KB
**Input File:** `interes.in`
**Output File:** `interes.out`

# Interes (clasele 9-12)

Se dă un arbore cu *n* noduri, despre care trebuie să răspundeți la *q* interogări definite astfel. Fiecare interogare constă dintr-o submulțime *S* a nodurilor arborelui. Trebuie să alegeți o submulțime **minimă** *T* de muchii din arbore astfel încît între oricare două noduri din *S* să existe o cale formată doar din muchii din *T*. Răspunsul la interogare este numărul de muchii alese.

## Date de intrare

Fișierul de intrare *interes.in* conține pe prima linie numerele *n* și *q*. Următoarele *n-1* linii conțin perechi de numere *u* *v* cu semnificația că există o muchie între *u* și *v*. Următoarele *q* linii conțin cîte o interogare sub forma *k v<sub>1</sub> v<sub>2</sub> ... v<sub>k</sub>*, unde *k* este numărul de noduri din interogare, iar *v<sub>1</sub>*, *v<sub>2</sub>*, ..., *v<sub>k</sub>* sînt nodurile.

## Date de ieșire

În fișierul de ieșire *interes.out* afișați răspunsurile la interogări, în aceeași ordine ca la intrare.

## Restricții

- *1 ≤ n, q ≤ 200000*
- *C ≤ 500000*, unde *C* este suma valorilor *k* din toate interogările.
- *1 ≤ u, v ≤ n*
- *2 ≤ k ≤ n*
- În fiecare interogare, valorile *v<sub>i</sub>* sînt distincte și cuprinse între *1* și *n*.
- Testele **nu** sînt grupate.

<table style="width: inherit;">
<tr>
<th>subtask </th>
<th>puncte </th>
<th>restricții </th>
</tr>
<tr>
<td> 1 </td>
<td> 32 </td>
<td> *n ≤ 8000*, *C ≤ 20000* </td>
</tr>
<tr>
<td> 2 </td>
<td> 40 </td>
<td> *n ≤ 80000*, *C ≤ 200000* </td>
</tr>
<tr>
<td> 3 </td>
<td> 28 </td>
<td> Fără restricții suplimentare. </td>
</tr>
</table>

## Exemplu

| interes.in | interes.out |
| --- | --- |
| 6 3
</br>
1 2
</br>
2 3
</br>
3 4
</br>
5 2
</br>
6 3
</br>
2 1 6
</br>
3 1 3 5
</br>
4 1 2 4 6
</br> | 3
</br>
3
</br>
4
</br> |

### Explicație

<img alt="" src="https://www.nerdarena.ro/problema/interes?action=download&file=interes-tree.svg"></img>

- Pentru interogarea 1 sînt necesare muchiile *(1,2)*, *(2,3)* și *(3,6)*.
- Pentru interogarea 2 sînt necesare muchiile *(1,2)*, *(2,3)* și *(2,5)*.
- Pentru interogarea 3 sînt necesare muchiile *(1,2)*, *(2,3)*, *(3,4)* și *(3,6)*.

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
