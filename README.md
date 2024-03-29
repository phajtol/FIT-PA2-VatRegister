# FIT-PA2-VatRegister

This is a project created as a homework for subject Programming and algorithmic 2 (BI-PA2 - Programování a algoritmizace 2) at FIT CTU.

Full assignment in Czech language is below.

Summary of the assignment: Create C++ class that will work as a database of invoices.

---

My solution is in file `solution.cpp`, function `main` of this file contains test data for basic scenarios.

My solution has received 5 points out of 5, which is maximum without bonuses.

# Assignment

Úkolem je realizovat třídu `CVATRegister`, která bude implementovat databázi kontrolních hlášení DPH.

Pro plánované důslednější potírání daňových úniků je zaveden systém kontrolních hlášení. V databázi jsou zavedené jednotlivé firmy, a do databáze jsou zaznamenávané jednotlivé vydané faktury, které daná firma vydala. Firmy lze do databáze zadávat a lze je rušit. Firma je identifikována svým jménem, adresou a daňovým identifikátorem (`id`). Daňový identifikátor je unikátní přes celou databázi. Jména a adresy se mohou opakovat, ale dvojice (jméno, adresa) je opět v databázi unikátní. Tedy v databázi může být mnoho firem `ACME`, mnoho firem může mít adresu `Praha`, ale firma `ACME` bydlící sídlící ve městě `Praha` může být v databázi pouze jedna. Při porovnávání daňových identifikátorů rozlišujeme malá a velká písmena, u jmen a adres naopak nerozlišujeme malá a velká písmena.

Veřejné rozhraní je uvedeno níže. Obsahuje následující:

- **Konstruktor bez parametrů**. Tento konstruktor inicializuje instanci třídy tak, že vzniklá instance je zatím prázdná (neobsahuje žádné záznamy).

- **Destruktor**. Uvolňuje prostředky, které instance alokovala.

- **Metoda `NewCompany(name, addr, id)`** přidá do existující databáze další záznam. Parametry name a addr reprezentují jméno a adresu, parametr id udává daňový identifikátor. Metoda vrací hodnotu true, pokud byl záznam přidán, nebo hodnotu false, pokud přidán nebyl (protože již v databázi existoval záznam se stejným jménem a adresou, nebo záznam se stejným číslem účtu).

- **Metody `CancelCompany (name, addr)` / `CancelCompany (id)`** odstraní záznam z databáze. Parametrem je jednoznačná identifikace pomocí jména a adresy (první varianta) nebo pomocí daňového identifikátoru (druhá varianta). Pokud byl záznam skutečně odstraněn, vrátí metoda hodnotu `true`. Pokud záznam neodstraní (protože neexistovala firma s touto identifikací), vrátí metoda hodnotu `false`.

- **Metody `Invoice (name, addres, amount)` / `Invoice (id, amount)`** zaznamenají příjem ve výši `amount`. Varianty jsou dvě - firma je buď identifikována svým jménem a adresou, nebo daňovým identifikátorem. Pokud metoda uspěje, vrací `true`, pro neúspěch vrací `false` (neexistující firma).

- **Metoda `Audit ( name, addr, sum )` / `Audit ( id, sum )`** vyhledá součet příjmů pro firmu se zadaným jménem a adresou nebo firmu zadanou daňovým identifikátorem. Nalezený součet uloží do výstupního parametru `sum`. Metoda vrátí `true` pro úspěch, `false` pro selhání (neexistující firma).

- **Metoda `MedianInvoice ()`** vyhledá medián hodnoty faktury. Do vypočteného mediánu se započtou všechny úspěšně zpracované faktury zadané voláním `Invoice`. Tedy nezapočítávají se faktury, které nešlo přiřadit (volání `Invoice` selhalo), ale započítávají se všechny dosud registrované faktury, tedy při výmazu firmy se neodstraňují její faktury z výpočtu mediánu. Pokud je v systému zadaný sudý počet faktur, vezme se vyšší ze dvou prostředních hodnot. Pokud systém zatím nezpracoval žádnou fakturu, bude vrácena hodnota 0.

Odevzdávejte soubor, který obsahuje implementovanou třídu `CVATRegister`. Třída musí splňovat veřejné rozhraní podle ukázky - pokud Vámi odevzdané řešení nebude obsahovat popsané rozhraní, dojde k chybě při kompilaci. Do třídy si ale můžete doplnit další metody (veřejné nebo i privátní) a členské proměnné. Odevzdávaný soubor musí obsahovat jak deklaraci třídy (popis rozhraní) tak i definice metod, konstruktoru a destruktoru. Je jedno, zda jsou metody implementované inline nebo odděleně. Odevzdávaný soubor nesmí kromě implementace třídy `CVATRegister` obsahovat nic jiného, zejména ne vkládání hlavičkových souborů a funkci `main` (funkce main a vkládání hlavičkových souborů může zůstat, ale pouze obalené direktivami podmíněného překladu). Za základ implementace použijte přiložený zdrojový soubor.

Třída je testovaná v omezeném prostředí, kde je limitovaná dostupná paměť (dostačuje k uložení seznamu) a je omezena dobou běhu. Implementovaná třída se nemusí zabývat kopírujícím konstruktorem ani přetěžováním operátoru =. V této úloze ProgTest neprovádí testy této funkčnosti.

Implementace třídy musí být efektivní z hlediska nároků na čas i nároků na paměť. Jednoduché lineární řešení nestačí (pro testovací data vyžaduje čas přes 5 minut). Předpokládejte, že vytvoření a likvidace firmy jsou řádově méně časté než ostatní operace, tedy zde je lineární složitost akceptovatelná. Častá jsou volání `Invoice` a `Audit`, jejich časová složitost musí být lepší než lineární (např. logaritmická nebo amortizovaná konstantní). Dále, v povinných testech se metoda `MedianInvoice` volá málo často, tedy nemusí být příliš efektivní (pro úspěch v povinných testech stačí složitost lineární nebo `n log n`, pro bonusový test je potřeba složitost lepší než lineární).

Pro uložení hodnot alokujte pole dynamicky případně použijte STL. Pozor Pokud budete pole alokovat ve vlastní režii, zvolte počáteční velikost malou (např. tisíc prvků) a velikost zvětšujte/zmenšujte podle potřeby. Při zaplnění pole není vhodné alokovat nové pole větší pouze o jednu hodnotu, takový postup má obrovskou režii na kopírování obsahu. Je rozumné pole rozšiřovat s krokem řádově tisíců prvků, nebo geometrickou řadou s kvocientem ~1.5 až 2.

Pokud budete používat STL, nemusíte se starat o problémy s alokací. Pozor - k dispozici máte pouze část STL (viz hlavičkové soubory v přiložené ukázce). Tedy například kontejnery `map` / `unordered_map` / `set` / `unordered_set` / ... nejsou k dispozici.

V přiloženém zdrojovém kódu jsou obsažené základní testy. Tyto testy zdaleka nepokrývají všechny situace, pro odladění třídy je budete muset rozšířit. Upozorňujeme, že testy obsažené v odevzdaných zdrojových kódech považujeme za nedílnou součást Vašeho řešení. Pokud v odevzdaném řešení necháte cizí testy, může být práce vyhodnocena jako opsaná.
