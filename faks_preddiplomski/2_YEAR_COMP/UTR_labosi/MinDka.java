
import java.util.ArrayList;
import java.util.HashMap;
import java.util.PriorityQueue;
import java.util.Scanner;

public class MinDka {
    public static void main(String[] args) {
        ArrayList<Stanje> listaStanja = new ArrayList<>();
        String[] simboliAbecede;
        String[] prihvatStanja;
        Stanje pocStanje = new Stanje();

        Scanner sc = new Scanner(System.in);

        String line = sc.nextLine();
        String[] stanjaString = line.split(",");

        for (int i = 0; i < stanjaString.length; i++) {
            Stanje pom = new Stanje(stanjaString[i], new HashMap<>());
            listaStanja.add(pom);

        }

        line = sc.nextLine();
        simboliAbecede = line.split(",");

        line = sc.nextLine();
        prihvatStanja = line.split(",");

        listaStanja.forEach((s) -> {
            for (int i = 0; i < prihvatStanja.length; i++) {
                if (s.stanje.equals(prihvatStanja[i])) {
                    s.privatljivo = true;
                }
            }
        });

        line = sc.nextLine();
        int index = listaStanja.indexOf(new Stanje(line, new HashMap<String, Stanje>()));
        pocStanje = listaStanja.get(index);

        while (sc.hasNextLine()) {
            line = sc.nextLine();
            String[] prijelaz = line.split("->");
            String[] stanjeISimbol = prijelaz[0].split(",");

            Stanje trenutno = listaStanja.get(listaStanja.indexOf(new Stanje(stanjeISimbol[0], new HashMap<>())));
            Stanje iduce = listaStanja.get(listaStanja.indexOf(new Stanje(prijelaz[1], new HashMap<>())));

            trenutno.mapaPrijelaza.put(stanjeISimbol[1], iduce);
        }

        nedohvatljivaStanja(listaStanja, pocStanje, simboliAbecede);
        pocStanje = istovjetnaStanja(listaStanja, pocStanje, simboliAbecede);
        printDka(listaStanja, simboliAbecede, pocStanje);

        sc.close();

    }

    static void printDka(ArrayList<Stanje> listaStanja, String[] simboliAbecede, Stanje pocStanje) {
        for (int i = 0; i < listaStanja.size(); i++) {
            if (i + 1 < listaStanja.size())
                System.out.print(listaStanja.get(i).stanje + ",");
            else
                System.out.println(listaStanja.get(i).stanje);
        }
        for (int i = 0; i < simboliAbecede.length; i++) {
            if (i != 0)
                System.out.print("," + simboliAbecede[i]);
            else
                System.out.print(simboliAbecede[i]);
        }
        System.out.println("");
        Boolean zarez = false;
        for (int i = 0; i < listaStanja.size(); i++) {
            if (listaStanja.get(i).privatljivo) {
                if (zarez)
                    System.out.print("," + listaStanja.get(i).stanje);
                else {
                    System.out.print(listaStanja.get(i).stanje);
                    zarez = true;
                }
            }
        }
        System.out.println("");
        System.out.println(pocStanje.stanje);
        listaStanja.forEach((stanje) -> {
            HashMap<String, Stanje> mapa = stanje.mapaPrijelaza;
            ArrayList<String> sortedKeys = new ArrayList<>();
            for (String s : mapa.keySet()) {
                sortedKeys.add(s);
            }
            sortedKeys.sort(String.CASE_INSENSITIVE_ORDER);
            for (String s : sortedKeys) {
                System.out.println(stanje.stanje + "," + s + "->" + mapa.get(s).stanje);
            }
        });
    }

    static void nedohvatljivaStanja(ArrayList<Stanje> lista, Stanje poc, String[] simboli) {
        PriorityQueue<Stanje> queue = new PriorityQueue<Stanje>();
        PriorityQueue<Stanje> dohvatStanja = new PriorityQueue<Stanje>();

        queue.add(poc);

        while (queue.isEmpty() == false) {
            Stanje s = queue.poll();
            if (!dohvatStanja.contains(s)) {
                dohvatStanja.add(s);
            }
            for (int i = 0; i < simboli.length; i++) {
                Stanje pom = s.mapaPrijelaza.get(simboli[i]);
                if (!dohvatStanja.contains(pom) && !queue.contains(pom)) {
                    queue.add(pom);
                }
            }
        }

        for (int i = 0; i < lista.size(); i++) {
            if (!dohvatStanja.contains(lista.get(i))) {
                lista.remove(i);
                i--;

            }
        }

    }

    static Stanje istovjetnaStanja(ArrayList<Stanje> lista, Stanje poc, String[] simboli) {

        lista.forEach((s) -> {

            if (s.privatljivo) {
                s.grupa = "1";
            } else {
                s.grupa = "2";
            }

        });

        // System.out.println("Prosla pocetnu podjelu grupa");
        // for (int i = 0; i < lista.size(); i++) {
        // System.out.println(lista.get(i).stanje + " : " + lista.get(i).grupa);

        // }

        Boolean isti = false;

        do {

            for (int i = 0; i < lista.size(); i++) {
                lista.get(i).proslaGrupa = lista.get(i).grupa;

            }
            Boolean[] obideno = new Boolean[lista.size()];

            for (int i = 0; i < obideno.length; i++) {
                obideno[i] = false;
            }

            for (int m = 0; m < lista.size(); m++) {
                if (!obideno[m]) {
                    obideno[m] = true;
                    ArrayList<Stanje> grupa = new ArrayList<>();
                    grupa.add(lista.get(m));

                    for (int n = m + 1; n < lista.size(); n++) {
                        if (lista.get(m).grupa.equals(lista.get(n).grupa)) {
                            obideno[n] = true;
                            grupa.add(lista.get(n));
                        }
                    }
                    if (grupa.size() > 1) {

                        // System.out.println("----------------------------");
                        // for (int i = 0; i < grupa.size(); i++) {
                        // System.out.println(grupa.get(i).stanje + " : " + grupa.get(i).grupa);
                        // }
                        // System.out.println("----------------------------");

                        int currPodgrupa = 1;
                        Boolean[] obidenaGrupa = new Boolean[grupa.size()];

                        for (int i = 0; i < obidenaGrupa.length; i++) {
                            obidenaGrupa[i] = false;
                        }

                        for (int i = 0; i < grupa.size(); i++) {
                            if (!obidenaGrupa[i]) {
                                obidenaGrupa[i] = true;
                                ArrayList<Stanje> pomGrupa = new ArrayList<>();
                                pomGrupa.add(grupa.get(i));
                                for (int j = i + 1; j < grupa.size(); j++) {
                                    if (!obidenaGrupa[j]) {
                                        // System.out.println("Jesam obisla sve clanove grupe?");
                                        Boolean istaStanja = true;
                                        for (int k = 0; k < simboli.length; k++) {
                                            if (!grupa.get(i).mapaPrijelaza.get(
                                                    simboli[k]).grupa.equals(
                                                            grupa.get(j).mapaPrijelaza
                                                                    .get(simboli[k]).grupa)) {
                                                // System.out.println("Nisu isti za znak " + simboli[k]);
                                                // System.out.println(grupa.get(i).mapaPrijelaza.get(
                                                // simboli[k]).grupa + " != "
                                                // + grupa.get(j).mapaPrijelaza
                                                // .get(simboli[k]).grupa);
                                                istaStanja = false;
                                                break;
                                            }
                                        }
                                        if (istaStanja) {
                                            pomGrupa.add(grupa.get(j));
                                            obidenaGrupa[j] = true;
                                        }
                                    }
                                }
                                // System.out.println("-----------||---------------");
                                // for (int l = 0; l < pomGrupa.size(); l++) {
                                // System.out.println(pomGrupa.get(l).stanje + " : " + pomGrupa.get(l).grupa);
                                // }
                                // System.out.println("-----------||---------------");

                                if (pomGrupa.size() != grupa.size()) {

                                    for (int j = 0; j < pomGrupa.size(); j++) {
                                        pomGrupa.get(j).grupa = pomGrupa.get(j).grupa + String.valueOf(currPodgrupa);
                                    }
                                }
                            }

                            currPodgrupa++;

                        }
                    }
                }

                isti = true;
                for (int i = 0; i < lista.size(); i++) {
                    if (!lista.get(i).grupa.equals(lista.get(i).proslaGrupa))
                        isti = false;
                }
            }
        } while (!isti);

        // lista.forEach((stanje) -> {
        // System.out.println(stanje.stanje + " " + stanje.grupa);
        // });

        Boolean[] obideno = new Boolean[lista.size()];
        HashMap<Stanje, Stanje> mapaPromjene = new HashMap<>();

        for (int i = 0; i < obideno.length; i++) {
            obideno[i] = false;
        }

        for (int i = 0; i < lista.size(); i++) {
            if (!obideno[i]) {
                obideno[i] = true;
                for (int j = i + 1; j < lista.size(); j++) {
                    if (lista.get(i).grupa.equals(lista.get(j).grupa)) {
                        obideno[j] = true;
                        mapaPromjene.put(lista.get(j), lista.get(i));
                    }
                }
            }
        }

        for (int i = 0; i < lista.size(); i++) {
            HashMap<String, Stanje> pomMapa = lista.get(i).mapaPrijelaza;
            for (String znak : pomMapa.keySet()) {
                Stanje izbrisi = pomMapa.get(znak);
                if (mapaPromjene.containsKey(izbrisi)) {
                    pomMapa.replace(znak, mapaPromjene.get(izbrisi));
                }
            }
        }

        for (int i = 0; i < lista.size(); i++) {
            if (mapaPromjene.containsKey(lista.get(i))) {
                lista.remove(i);
                i--;
            }
        }

        if (mapaPromjene.containsKey(poc)) {
            return mapaPromjene.get(poc);
        }
        return poc;

    }

}

class Stanje implements Comparable<Stanje> {
    String stanje;
    HashMap<String, Stanje> mapaPrijelaza;
    Boolean privatljivo = false;
    String grupa;
    String proslaGrupa;

    @Override
    public int compareTo(Stanje o) {
        return stanje.compareTo(o.stanje);
    }

    Stanje() {

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

    Stanje(String s, HashMap<String, Stanje> mapa) {
        stanje = s;
        mapaPrijelaza = mapa;
    }

}
