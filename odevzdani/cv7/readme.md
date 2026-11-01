# Cvičení 7

1. Splněno - vizualizováno na stretched tělesech scéna 7
    a. Levá koule - špatný výpočet normál, při neuniformním scale normály nezachovají kolmou normálu
    b. Pravá koule - normály jsou počítány správně
2. Splněno - vizualizováno na scéně 8
    a. Levá koule počítá zrcadlovou složku bez kontroly kolmosti
    b. Pravá koule počítá zrcadlovou složku s kontrolou `dot(N, L) > 0`
    c. Po změně velikosti okna se deformuje obraz
    d. Aplikace využívá observer na aktualizaci viewportu a kamery
3. Splněno
