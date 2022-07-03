Rozwiązania zostały ocenione na podstawie wyników testów automatycznych. 


## Testy rocznikowe
https://gitlab.com/mimuw-rocznik-2001/so-2022/testy-zad5

## Testy oficjalne
https://github.com/kfernandez31/SO-5-VFS-Listeners/tree/main/src/tests

## Opis wyników i testów
Testy zostały podzielone na kategorie odpowiadające typom monitorowanego zdarzenia:
- `NOTIFY_OPEN`: testy oznaczone `"o*"`,
- `NOTIFY_TRIOPEN`: oznaczone `"t*"`,
- `NOTIFY_CREATE`: oznaczone `"c*"`,
- `NOTIFY_MOVE`: oznaczone `"m*"`.

Za każdą część rozwiązanie otrzymywało liczbę punktów proporcjonalną do liczby zaliczonych testów z tej kategorii, o ile zaliczyło test podstawowy:
* `NOTIFY_OPEN`: test `o3`,
* `NOTIFY_TRIOPEN`: test `t2`,
* `NOTIFY_CREATE`: test `c0`,
* `NOTIFY_MOVE`: test `m3`.

Jeśli rozwiązanie nie zaliczyło podstawowego testu danej części, otrzymywało
0 pkt za tę część. Ocena końcowa jest sumą punktów otrzymanych za każdą część.

Komentarz do oceny w USOSwebie wylicza testy, których rozwiązanie nie zaliczyło.
Użyte w komentarzu skróty odpowiadają załączonym programom testującym, według
poniższej listy:

- `NOTIFY_OPEN`:
    - `o0`: `open_interrupt.c.c`
    - `o1`: `open_badfd.c.c`
    - `o2`: `open_softlink.c.c`
    - `o3`: `open_basic.c.c`
    - `o4`: `open_einval.c.c`
    - `o5`: `open_many.c.c`
    - `o6`: `open_limit.c.c`
    - `o7`: `open_procfs.c.c`
    - `o8`: `open_random.c.c`
    - `o9`: `open_recreate_path.c.c`
    - `o10`: `open_move.c.c`
    - `o11`: `open_fail.c.c`
    - `o12`: `open_dir.c.c`
    - `o13`: `open_hardlink.c.c`

- `NOTIFY_TRIOPEN`:
    - `t0`: `triopen_procfs.c.c`
    - `t1`: `triopen_hardlink.c.c`
    - `t2`: `triopen_once.c.c`
    - `t3`: `triopen_twice.c.c`
    - `t4`: `triopen_thrice.c.c`
    - `t5`: `triopen_softlink.c.c`
    - `t6`: `triopen_random.c.c`
    - `t7`: `triopen_at_same_time.c.c`

- `NOTIFY_CREATE`:
    - `c0`: `create_basic.c.c`
    - `c1`: `create_dev.c.c`
    - `c2`: `create_hardlink.c.c`
    - `c3`: `create_inner.c.c`
    - `c4`: `create_dir.c.c`
    - `c5`: `create_softlink.c.c`
    - `c6`: `create_notdir.c.c`

- `NOTIFY_MOVE`:
    - `m0`: `move_notdir.c.c`
    - `m1`: `move_only_name_changed.c.c`
    - `m2`: `move_softlink.c.c`
    - `m3`: `move_basic.c.c`
    - `m4`: `move_dir.c.c`
    - `m5`: `move_hardlink.c.c`
    - `m6`: `move_dev.c.c`
