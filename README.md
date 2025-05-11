# Dumb Thieves

### 6. Tępi złodzieje. 

W roku 2070 przeprowadzono na grupie więźniów ochotników eksperyment mający skutecznie
zniechęcić ich do popełniania przestępstw. Niestety, eksperyment okazał się nie do końca
skuteczny. W jego efekcie inteligencja uczestników została znacznie obniżona, bez redukcji
skłonności przestępczych. W celu zatuszowania pomyłki, rząd w tajemnicy wyznaczył grupę
specjalną, która delikatnie sugeruje złodziejom dobór odpowiednich domów do okradania. Domy
te są specjalnie przygotowane, by nawet skończony debil był w stanie się do nich włamać.
Grupa specjalna następnie udaje paserów, którzy odkupują "ukradzione" rzeczy od złodziei i
wstawia je z powrotem do domów.

Procesy symbolizują opiekunów złodziei, udających ich kumpli, oraz paserów. 

Procesy; N opiekunów
Zasoby: D unikalnych domów, P paserów. Po dostępie do domu jest on niedostępny póki opiekun nie uzyska dostępu
do pasera, który odstawi rzeczy na miejsce.

Procesy działają z różną prędkością, mogą wręcz przez jakiś czas odpoczywać. Nie powinno to blokować pracy innych procesów.

Projekt na trójkę: brak paserów, domy można okradać bezustannie

## How To Run
```
mkdir build
cd build
cmake ..
make
mpirun --hostfile ../hosts -np <number_of_processes> ./dumb_thieves <number_of_houses> <number_of_fences>
```
