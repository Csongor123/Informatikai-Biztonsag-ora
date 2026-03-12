# Dokumentum titkosítás C nyelven
Ez a projekt egy egyszerű dokumentum titkosító programot valósít meg C programozási nyelven.
A program képes egy szöveges dokumentum tartalmát titkosítani, majd a titkosított adatot vissza is fejteni.

# A program működése

A program három fájllal dolgozik:

# 1. dokumentum.txt
Ez tartalmazza az eredeti szöveget.

# 2. titkositott.bin
Ebbe kerül a titkosított adat.

# 3. visszafejtett.txt
Ide kerül a visszafejtett dokumentum.

A program működési lépései:

1. A program beolvassa a dokumentum.txt fájlt.

2. A dokumentum tartalmát bájtonként feldolgozza.

3. A titkosítás során egy kulcsfolyam jellegű algoritmust alkalmaz.

4. A titkosított adat a titkositott.bin fájlba kerül.

5. A program ezt követően visszafejti a titkosított adatot.

6. A visszafejtett tartalom a visszafejtett.txt fájlba kerül.

# Alkalmazott titkosítási elv

A program egy egyszerű kulcsfolyam alapú titkosítási módszert használ.

A titkosítás során:

- a dokumentum minden bájtja feldolgozásra kerül
- egy álvéletlen kulcsbájt generálódik
- a titkosítás XOR művelettel történik

A titkosítás során figyelembe veszi:

- az aktuális bájt pozícióját
- az előző titkosított bájtot
- egy belső kulcsállapotot

# Fordítás

A program GCC fordítóval fordítható.

# Windows alatt:
gcc titkosito.c -o titkosito.exe

# Linux vagy MacOS alatt:
gcc titkosito.c -o titkosito

# Program futtatása
Amikor futtatni akarjuk a programot, először töltsük le a .ZIP fájl-t, utána nyissuk  meg a titkosito.c fájlt és futassuk a programot, ezek után a dokumentum.txt-ben írt szövegünk egy visszafejtett.txt-ben kapjuk vissza.
A program futtatása után automatikusan létrejönnek a titkosított és visszafejtett fájlok.
