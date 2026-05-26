# Treicc

**Time Limit:** 0.1s
**Memory Limit:** 262144KB
**Input File:** `arbperm.in`
**Output File:** `arbperm.out`

# Arbperm

**Notă: Testele pentru această problemă nu acoperă cazul cel mai dificil.** Există cazuri în care \(\mathcal{O}(N)\) elemente migrează, chiar și pentru *N* mare. Vezi problema <a href="/problema/arbperm2">Arbperm2</a>.

Arborele tuturor permutărilor se generează astfel. În rădăcină așezăm singura permutare cu un element, (1). Aceasta are doi fii, (2, 1) și (1, 2). În general, nodul corespunzător unei permutări *P* cu *N* elemente va avea *N* + 1 fii obținuți prin inserarea valorii *N* + 1 ı̂n *P* pe toate pozițiile de la cea mai din stînga pı̂nă la cea mai din dreapta. Figura de mai jos prezintă primele 4 niveluri ale arborelui de permutări.

<img alt="" src="https://www.nerdarena.ro/problema/arbperm?action=download&file=arbperm-fig.svg" style="width: 1000px;"></img>

Observăm că, pentru un *N* dat, toate permutările de *N* elemente se află pe același nivel ı̂n arbore.

Fiind dată o permutare *P* și un număr *K*, găsiți permutarea *Q* aflată cu *K* poziții la dreapta lui *P*. Se garantează că există cel puțin *K* permutări la dreapta lui *P* pe același nivel.

## Date de intrare

Fișierul de intrare *arbperm.in* conține pe prima linie întregii *N* și *K*, separați printr-un spațiu. A doua linie conține *N* valori întregi, reprezentînd permutarea *P*.

## Date de ieșire

În fișierul de ieșire *arbperm.out* afișați, pe o singură linie, *N* numere separate prin spații reprezentînd permutarea *Q*.

## Restricții

- 3 ≤ N ≤ 100.000
- 1 ≤ K ≤ 1.000.000.000

<table class="subtasks">
<tr>
<th><ol>
</br>
</ol></th>
<th>puncte </th>
<th>restricții </th>
</tr>
<tr>
<td> 1 </td>
<td> 20 </td>
<td> *N* ≤ 20 </td>
</tr>
<tr>
<td> 2 </td>
<td> 20 </td>
<td> *N* ≤ 1.000, *K* ≤ 100.000 </td>
</tr>
<tr>
<td> 3 </td>
<td> 30 </td>
<td> *K* ≤ 5.000.000 </td>
</tr>
<tr>
<td> 4 </td>
<td> 30 </td>
<td> Fără restricții suplimentare. </td>
</tr>
</table>

## Exemplu

| arbperm.in | arbperm.out |
| --- | --- |
| 3 1
</br>
2 1 3
</br> | 3 1 2 |
| 4 7
</br>
2 1 4 3
</br> | 1 4 3 2 |

<script id="MathJax-script" async="None" src="/static/js/third-party/mathjax-3.2.2/tex-chtml.js"></script>

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
