# Treicc

**Time Limit:** 0.2s
**Memory Limit:** 512KB
**Input File:** `treicc.in`
**Output File:** `treicc.out`

# Trei cifre consecutive

Fie *A* un numar natural format din *N* cifre pentru care nu exista *3* cifre aflate pe pozitii consecutive care sa aiba aceeasi paritate. Spunem ca *A* are numarul de ordine *M*, daca *A* se afla pe pozitia *M* in multimea numerelor formate din *N* cifre cu aceeasi proprietate.

Avand la dispozitie doua numere naturale *N* si *K* si un sir **ordonat crescator** *A(1), A(2), ..., A(K)* reprezentand numerele de ordine a *K* numere cu proprietatea enuntata mai sus, scrieti un program care afiseaza imaginile acestor *K* numere in ordine.

## Date de intrare

Fisierul de intrare *treicc.in* va contine pe prima linie numerele naturale *N* si *K*, iar pe a doua linie elementele sirului *A(1), A(2), ..., A(K)*, separate printr-un spatiu.

## Date de ieșire

In fisierul de ieșire *treicc.out* vor fi scrise pe randuri separate, numerele cerute in ordinea din fisierul de intrare. Nu va exista niciun spatiu intre cifrele aceluiasi numar.

## Restricții

- *3 ≤ N ≤ 8*
- *1 ≤ K ≤ 5*
- *1 ≤ A(i) ≤ <span class="caps">TOTAL</span>*, unde *1 ≤ i ≤ K* iar *<span class="caps">TOTAL</span>* este numarul total de numere formate din *N* cifre cu proprietatea enuntata

## Exemplu

| treicc.in | treicc.out |
| --- | --- |
| 3 5
</br>
2 6 10 105 197
</br> | 101
</br>
105
</br>
109
</br>
239
</br>
361
</br> |

### Explicație

Numerele 101, 105, 109, 239 si 361 se afla pe pozitiile 2, 6, 10, 105 si 197 in multimea numerelor formate din *3* cifre cu proprietatea enuntata.

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
