import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Scanner;

public class SimPa {
    public static void main(String[] args) {
        ArrayList<Stanje> listaStanja = new ArrayList<>();
        ArrayList<UlazniZnak> ulazniZnakovi = new ArrayList<>();
        ArrayList<ZnakStoga> znakoviStoga = new ArrayList<>();
        String[] prihvatStanja;
        Stanje pocStanje = new Stanje();
        Scanner sc = new Scanner(System.in);

        // ulazni nizovi

        String line = sc.nextLine();
        String[] nizovi = line.split("\\|");

        line = sc.nextLine();
        String[] stanjaString = line.split(",");

        for (int i = 0; i < stanjaString.length; i++) {
            Stanje pom = new Stanje(stanjaString[i], new HashMap<Ulaz, Izlaz>());
            listaStanja.add(pom);

        }

        line = sc.nextLine();
        String[] simboliAbecede = line.split(",");

        for (int i = 0; i < simboliAbecede.length; i++) {
            UlazniZnak pom = new UlazniZnak(simboliAbecede[i]);
            ulazniZnakovi.add(pom);

        }
        ulazniZnakovi.add(new UlazniZnak("$"));

        line = sc.nextLine();
        String[] znakoviStogaString = line.split(",");

        for (int i = 0; i < znakoviStogaString.length; i++) {
            ZnakStoga pom = new ZnakStoga(znakoviStogaString[i]);
            znakoviStoga.add(pom);

        }
        znakoviStoga.add(new ZnakStoga("$"));

        line = sc.nextLine();
        prihvatStanja = line.split(",");

        listaStanja.forEach((s) -> {
            for (int i = 0; i < prihvatStanja.length; i++) {
                if (s.stanje.equals(prihvatStanja[i])) {
                    s.prihvatljivo = true;
                }
            }
        });

        line = sc.nextLine();
        pocStanje = getStanje(listaStanja, line.trim());

        line = sc.nextLine();
        ZnakStoga pocZnak = getZnakStoga(znakoviStoga, line.trim());

        while (sc.hasNextLine()) {
            line = sc.nextLine();
            String[] prijelaz = line.split("->");
            String[] stanjeIZnakovi = prijelaz[0].split(",");

            Stanje currS = listaStanja.get(listaStanja.indexOf(new Stanje(stanjeIZnakovi[0])));
            UlazniZnak currUl = getUlazniZnak(ulazniZnakovi, stanjeIZnakovi[1]);
            ZnakStoga currSt = getZnakStoga(znakoviStoga, stanjeIZnakovi[2]);
            Ulaz ulaz = new Ulaz(currUl, currSt);

            stanjeIZnakovi = prijelaz[1].split(",");
            Stanje nextS = getStanje(listaStanja, stanjeIZnakovi[0]);
            ArrayDeque<ZnakStoga> stack = new ArrayDeque<>();
            String s = stanjeIZnakovi[1];
            while (!s.isEmpty()) {
                ZnakStoga znakSt = getZnakStoga(znakoviStoga, s.substring(0, 1));
                s = s.substring(1);
                stack.addFirst(znakSt);
            }
            Izlaz izlaz = new Izlaz(nextS, stack);

            currS.mapaPrijelaza.put(ulaz, izlaz);
        }

        // for (int i = 0; i < nizovi.length; i++) {
        // System.out.println(nizovi[i]);
        // }
        // ulazniZnakovi.forEach((uz) -> {
        // System.out.print(uz.znak + " & ");
        // });

        // System.out.println("___________________________");
        // listaStanja.forEach((s) -> {
        // System.out.println("stanje: " + s.stanje + " : " + s.prihvatljivo);
        // s.mapaPrijelaza.forEach((a, b) -> {
        // System.out.print(a.ulZnak.znak + " " + a.znakSt.znak);
        // System.out.print(" ide u ");
        // System.out.println(b.s.stanje);
        // System.out.print("Stanje stoga: ");
        // b.znakovi.forEach((z) -> {
        // System.out.print(z.znak + "|");
        // });
        // System.out.println();
        // });

        // });

        // System.out.println("__________________");
        // znakoviStoga.forEach((zs) -> {
        // System.out.print(zs.znak + " i ");
        // });

        sc.close();

        for (int i = 0; i < nizovi.length; i++) {
            ispis(nizovi[i], listaStanja, pocStanje, pocZnak, ulazniZnakovi);
        }
    }

    public static Stanje getStanje(ArrayList<Stanje> lista, String ime) {
        for (int i = 0; i < lista.size(); i++) {
            if (lista.get(i).stanje.equals(ime)) {
                return lista.get(i);
            }
        }

        return null;
    }

    public static ZnakStoga getZnakStoga(ArrayList<ZnakStoga> lista, String ime) {
        for (int i = 0; i < lista.size(); i++) {
            if (lista.get(i).znak.equals(ime)) {
                return lista.get(i);
            }
        }

        return null;
    }

    public static UlazniZnak getUlazniZnak(ArrayList<UlazniZnak> lista, String ime) {
        for (int i = 0; i < lista.size(); i++) {
            if (lista.get(i).znak.equals(ime)) {
                return lista.get(i);
            }
        }

        return null;
    }

    public static void ispis(String niz, ArrayList<Stanje> lista, Stanje pocS, ZnakStoga pocZS,
            ArrayList<UlazniZnak> listaUlaznih) {
        ArrayDeque<ZnakStoga> deck = new ArrayDeque<>();
        String[] simboli = niz.split(",");

        deck.addFirst(pocZS);
        Stanje currS = pocS;

        System.out.print(currS.stanje + "#");
        printDeck(deck);
        System.out.print("|");
        int i;

        ZnakStoga pomZS = deck.peekLast();
        UlazniZnak pomUL = getUlazniZnak(listaUlaznih, "$");
        if (pomZS != null) {
            Ulaz pomUlaz = new Ulaz(pomUL, pomZS);
            Izlaz pomIzlaz = currS.mapaPrijelaza.get(pomUlaz);
            if (pomIzlaz != null) {
                deck.removeLast();
                Iterator<ZnakStoga> it = pomIzlaz.znakovi.iterator();
                while (it.hasNext()) {
                    ZnakStoga pomZnak = it.next();
                    if (!pomZnak.znak.equals("$")) {
                        deck.addLast(pomZnak);
                    }
                }

                currS = pomIzlaz.s;
                System.out.print(currS.stanje + "#");
                printDeck(deck);
                System.out.print("|");

            }
        }

        for (i = 0; i < simboli.length; i++) {
            pomZS = deck.peekLast();
            pomUL = getUlazniZnak(listaUlaznih, "$");
            if (pomZS != null) {
                // System.out.println("PEEK: " + pomZS.znak);
                Ulaz pomUlaz = new Ulaz(pomUL, pomZS);
                Izlaz pomIzlaz = currS.mapaPrijelaza.get(pomUlaz);
                if (pomIzlaz != null) {
                    deck.removeLast();
                    Iterator<ZnakStoga> it = pomIzlaz.znakovi.iterator();
                    while (it.hasNext()) {
                        ZnakStoga pomZnak = it.next();
                        if (!pomZnak.znak.equals("$")) {
                            deck.addLast(pomZnak);
                        }
                    }

                    currS = pomIzlaz.s;
                    System.out.print(currS.stanje + "#");
                    printDeck(deck);
                    System.out.print("|");
                    i--;

                } else {
                    UlazniZnak currUZ = getUlazniZnak(listaUlaznih, simboli[i]);
                    ZnakStoga currZS = deck.peekLast();
                    if (currZS == null) {
                        // System.out.print("/nTu sam?/n");
                        break;
                    }
                    Ulaz pom = new Ulaz(currUZ, currZS);
                    // System.out.println(pom.ulZnak.znak + " " + pom.znakSt.znak);
                    Izlaz iz = currS.mapaPrijelaza.get(pom);
                    if (iz == null) {
                        // System.out.println("A tu???");
                        break;
                    }

                    deck.removeLast();
                    Iterator<ZnakStoga> it = iz.znakovi.iterator();
                    while (it.hasNext()) {
                        ZnakStoga pomZnak = it.next();
                        // System.out.println(pomZnak.znak);
                        if (!pomZnak.znak.equals("$")) {
                            deck.addLast(pomZnak);
                        }
                    }

                    currS = iz.s;
                    System.out.print(currS.stanje + "#");
                    printDeck(deck);
                    System.out.print("|");
                }
            } else {
                break;
            }
            // System.out.print("__" + i + "__");

        }
        // System.out.print("__" + i + "__");
        if (i != simboli.length) {
            System.out.println("fail|0");
        } else {
            Stanje pomS = currS;
            String pomStog = "";
            String currStog = "";
            Iterator<ZnakStoga> it = deck.descendingIterator();

            while (it.hasNext()) {
                ZnakStoga zs = it.next();
                pomStog = pomStog.concat(zs.znak);
            }

            do {
                if (currS.prihvatljivo) {
                    break;
                } else {
                    ZnakStoga pomStoga = deck.peekLast();
                    UlazniZnak pomUlazni = getUlazniZnak(listaUlaznih, "$");
                    if (pomStoga != null) {
                        Ulaz pomUlaz = new Ulaz(pomUlazni, pomStoga);
                        Izlaz pomIzlaz = currS.mapaPrijelaza.get(pomUlaz);
                        if (pomIzlaz != null) {
                            deck.removeLast();
                            Iterator<ZnakStoga> it2 = pomIzlaz.znakovi.iterator();
                            while (it2.hasNext()) {
                                ZnakStoga pomZnak = it2.next();
                                if (!pomZnak.znak.equals("$")) {
                                    deck.addLast(pomZnak);
                                }
                            }

                            currS = pomIzlaz.s;
                            System.out.print(currS.stanje + "#");
                            printDeck(deck);
                            System.out.print("|");

                        } else {
                            break;
                        }
                    }
                }
                it = deck.descendingIterator();

                while (it.hasNext()) {
                    currStog = currStog.concat(it.next().znak);
                }
                // System.out.println(currS.stanje + " " + pomS.stanje);
                // System.out.println(pomStog + " " + currStog);
            } while (!currS.equals(pomS) || !currStog.equals(pomStog));

            if (currS.prihvatljivo) {
                System.out.println("1");
            } else {
                System.out.println("0");
            }
        }

    }

    public static void printDeck(ArrayDeque<ZnakStoga> deq) {
        Iterator<ZnakStoga> it = deq.descendingIterator();
        int brojac = 0;

        while (it.hasNext()) {
            brojac++;
            System.out.print(it.next().znak);
        }
        if (brojac == 0) {
            System.out.print("$");
        }
    }
}

class Stanje implements Comparable<Stanje> {
    String stanje;
    HashMap<Ulaz, Izlaz> mapaPrijelaza;
    Boolean prihvatljivo = false;

    Stanje(String s, HashMap<Ulaz, Izlaz> mapa) {
        stanje = s;
        mapaPrijelaza = mapa;
    }

    Stanje(String s) {
        stanje = s;
    }

    Stanje() {

    }

    @Override
    public int compareTo(Stanje o) {
        return stanje.compareTo(o.stanje);
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Stanje) {
            return stanje.equals(((Stanje) o).stanje);
        }
        return false;
    }

    @Override
    public int hashCode() {
        return stanje.hashCode();
    }

}

class ZnakStoga implements Comparable<ZnakStoga> {
    String znak;

    ZnakStoga(String z) {
        this.znak = z;
    }

    ZnakStoga() {
    }

    @Override
    public int compareTo(ZnakStoga o) {
        return znak.compareTo(o.znak);
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof ZnakStoga) {
            return znak.equals(((ZnakStoga) o).znak);
        }
        return false;
    }

    @Override
    public int hashCode() {
        return znak.hashCode();
    }
}

class UlazniZnak implements Comparable<UlazniZnak> {
    String znak;

    UlazniZnak(String z) {
        this.znak = z;
    }

    UlazniZnak() {
    }

    @Override
    public int compareTo(UlazniZnak o) {
        return znak.compareTo(o.znak);
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof UlazniZnak) {
            return znak.equals(((UlazniZnak) o).znak);
        }
        return false;
    }

    @Override
    public int hashCode() {
        return znak.hashCode();
    }
}

class Ulaz implements Comparable<Ulaz> {
    UlazniZnak ulZnak;
    ZnakStoga znakSt;

    Ulaz(UlazniZnak ul, ZnakStoga st) {
        this.ulZnak = ul;
        this.znakSt = st;
    }

    @Override
    public int compareTo(Ulaz o) {
        return this.ulZnak.compareTo(o.ulZnak) + this.znakSt.compareTo(o.znakSt);
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Ulaz) {
            return this.ulZnak.equals(((Ulaz) o).ulZnak) && this.znakSt.equals(((Ulaz) o).znakSt);
        }
        return false;
    }

    @Override
    public int hashCode() {
        return ulZnak.hashCode() + znakSt.hashCode();
    }

}

class Izlaz {
    Stanje s;
    ArrayDeque<ZnakStoga> znakovi = new ArrayDeque<>();

    Izlaz(Stanje st, ArrayDeque<ZnakStoga> list) {
        this.s = st;
        this.znakovi = list;
    }
}