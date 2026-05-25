# Treicc

**Time Limit:** 0.3s
**Memory Limit:** 131072KB
**Input File:** `ssdj.in`
**Output File:** `ssdj.out`

# Ssdj (lot liceu)

Pentru că nu au luat toți nota 10 la simulare, conducerea liceului a hotărât să pedepsească elevii într-un mod inuman: aceștia nu au mai avut voie să meargă la teatru și nici să mai citească din marii clasici ai literaturii. Singura lor mângâiere era o matrice cu *N* linii și *N* coloane care conține numai litere mici ale alfabetului englez, pentru care trebuiau să identifice submatricele valabile. O submatrice este considerată valabilă dacă îndeplinește simultan următoarele condiții:

- are cel puțin două linii și cel puțin două coloane;
- literele aflate în colțurile stânga-sus și dreapta-jos ale submatricei sunt strict mai mari lexicografic decât toate celelalte litere din submatrice.

Ajutați elevii liceului să afle numărul submatricelor valabile care există în matrice și să scape astfel de pedeapsa îngrozitoare.

## Date de intrare

Fișierul *ssdj.in* conține pe prima linie numărul natural *N*, iar pe următoarele *N* linii se află câte *N* litere mici, neseparate prin spații.

## Date de ieșire

Fișierul *ssdj.out* conține un singur număr natural reprezentând numărul de submatrice valabile.

## Restricții

- *1 ≤ N ≤ 1000*
- Pentru teste valorând *10* puncte, *N ≤ 50*.
- Pentru teste valorând *20* puncte, matricea va conține numai literele *a* și *b*.

## Exemplu

| ssdj.in | ssdj.out |
| --- | --- |
| 4
</br>
maea
</br>
bcda
</br>
aaae
</br>
aaaa
</br> | 3
</br> |

### Explicație

Submatricele valabile sunt:

1)
</br>
ma
</br>
bc

2)
</br>
ea
</br>
da
</br>
ae

3)
</br>
da
</br>
ae

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
