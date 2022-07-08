package lab1;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.PriorityQueue;
import java.util.Scanner;

import java.util.Iterator;

public class SimEnka {
    public static void main(String[] args) {

        String[] ulazniNizovi;
        Stanje[] listaStanja;
        String[] simboliAbecede;
        String[] prihvatStanja;
        Stanje pocStanje = new Stanje();

        Scanner sc = new Scanner(System.in);

        String line = sc.nextLine();
        ulazniNizovi = line.split("\\|");

        // for (int i = 0; i < ulazniNizovi.length; i++) {
        // System.out.println(ulazniNizovi[i]);
        // }

        line = sc.nextLine();
        String[] stanjaString = line.split(",");

        listaStanja = new Stanje[stanjaString.length];
        for (int i = 0; i < stanjaString.length; i++) {
            listaStanja[i] = new Stanje();
            listaStanja[i].stanje = stanjaString[i];
            listaStanja[i].mapaPrijelaza = new HashMap<>();
            // System.out.println(listaStanja[i].stanje);

        }
        line = sc.nextLine();
        simboliAbecede = line.split(",");

        // for (int i = 0; i < simboliAbecede.length; i++) {
        // System.out.println(simboliAbecede[i]);
        // }

        line = sc.nextLine();
        prihvatStanja = line.split(",");

        line = sc.nextLine();
        for (int i = 0; i < listaStanja.length; i++) {
            if (listaStanja[i].stanje.equals(line)) {
                pocStanje = listaStanja[i];
            }
        }

        while (sc.hasNextLine()) {
            line = sc.nextLine();
            String[] prijelaz = line.split("->");
            String[] stanjeISimbol = prijelaz[0].split(",");
            String[] prijelaznaStanjaString = prijelaz[1].split(",");

            for (int i = 0; i < listaStanja.length; i++) {
                if (listaStanja[i].stanje.equals(stanjeISimbol[0]) && !prijelaz[1].equals("#")) {
                    // System.out.println(
                    // "Stanje iz liste stanja i iz stringa: " + listaStanja[i] + " " +
                    // stanjeISimbol[0]);
                    ArrayList<Stanje> prijelaznaStanja = new ArrayList<>();
                    for (int j = 0; j < prijelaznaStanjaString.length; j++) {

                        for (int k = 0; k < listaStanja.length; k++) {
                            if (listaStanja[k].stanje.equals(prijelaznaStanjaString[j])) {
                                prijelaznaStanja.add(listaStanja[k]);
                                // System.out.println("Addano je stanje: " + listaStanja[k]);
                                break;
                            }
                        }

                    }
                    listaStanja[i].mapaPrijelaza.put(stanjeISimbol[1], prijelaznaStanja);
                    break;
                }

            }
        }

        // for (int i = 0; i < listaStanja.length; i++) {

        // System.out.println(listaStanja[i].stanje + " stanje?");
        // HashMap<String, ArrayList<Stanje>> mapa = listaStanja[i].mapaPrijelaza;
        // for (String str : mapa.keySet()) {
        // System.out.print(str + " -> ");
        // Iterator<Stanje> it = mapa.get(str).iterator();
        // while (it.hasNext()) {
        // System.out.println(it.next().stanje + "__");
        // }

        // }

        // System.out.println("----------------");
        // }

        for (int i = 0; i < ulazniNizovi.length; i++) {
            metoda(listaStanja, pocStanje, ulazniNizovi[i]);
        }
        sc.close();

    }

    static void metoda(Stanje listaStanja[], Stanje poc, String ulaz) {
        PriorityQueue<Stanje> queue = new PriorityQueue<>();
        PriorityQueue<Stanje> pocQueue = new PriorityQueue<>();

        pocQueue.add(poc);
        ArrayList<ArrayList<Stanje>> velikaLista = new ArrayList<>();
        velikaLista.add(poc.mapaPrijelaza.get("$"));

        while (!velikaLista.isEmpty()) {
            Iterator<ArrayList<Stanje>> it = velikaLista.iterator();
            ArrayList<ArrayList<Stanje>> velikaLista2 = new ArrayList<>();
            while (it.hasNext()) { // za svako stanje koje ima $-prijelaz u ovom passu
                ArrayList<Stanje> pocListUnutra = it.next();
                if (pocListUnutra != null) {
                    Iterator<Stanje> it2 = pocListUnutra.iterator();
                    while (it2.hasNext()) {
                        Stanje s = it2.next();
                        if (!pocQueue.contains(s)) {
                            pocQueue.add(s);
                            if (s.mapaPrijelaza.get("$") != null) {
                                velikaLista2.add(s.mapaPrijelaza.get("$"));
                            }

                        }

                    }
                }
            }

            velikaLista = velikaLista2;
        }

        System.out.print(pocQueue.peek().stanje);
        queue.add(pocQueue.poll());
        while (pocQueue.peek() != null) {
            System.out.print("," + pocQueue.peek().stanje);
            queue.add(pocQueue.poll());
        }

        String niz[] = ulaz.split(",");

        for (int i = 0; i < niz.length; i++) { // za svaki simbol u nizu
            PriorityQueue<Stanje> pomQueue = new PriorityQueue<>();
            System.out.print("|");

            if (!queue.isEmpty()) { // u queue su sva stanja u kojima je automat bio u proslom prolazu
                Iterator<Stanje> it = queue.iterator();

                while (it.hasNext()) { // za svako stanje
                    Stanje trenutno;
                    trenutno = it.next();

                    if (trenutno.mapaPrijelaza.get(niz[i]) != null) { // ako postoji prijelaz za simbol
                        Iterator<Stanje> it2 = trenutno.mapaPrijelaza.get(niz[i]).iterator();
                        while (it2.hasNext()) { // za svako stanje u koje se prelazi simbolom
                            Stanje pomStanje = it2.next();
                            if (!pomQueue.contains(pomStanje)) { // dodaj u pomocni queue ako vec ne postoji
                                pomQueue.add(pomStanje);
                            }
                            ArrayList<ArrayList<Stanje>> velikaListaUnutra = new ArrayList<>(); // lista za $-prijelaze
                                                                                                // svih stanja koji to
                                                                                                // imaju
                            velikaListaUnutra.add(pomStanje.mapaPrijelaza.get("$"));

                            while (!velikaListaUnutra.isEmpty()) { // dok postoji stanje koje ima $-prijelaz
                                Iterator<ArrayList<Stanje>> it3 = velikaListaUnutra.iterator();
                                ArrayList<ArrayList<Stanje>> velikaLista2Unutra = new ArrayList<>();
                                while (it3.hasNext()) { // za svako stanje koje ima $-prijelaz u ovom passu
                                    ArrayList<Stanje> pocListUnutra = it3.next();
                                    if (pocListUnutra != null) {
                                        Iterator<Stanje> it4 = pocListUnutra.iterator();
                                        while (it4.hasNext()) {
                                            Stanje s = it4.next();
                                            if (!pomQueue.contains(s)) {
                                                pomQueue.add(s);
                                                if (s.mapaPrijelaza.get("$") != null) {
                                                    velikaLista2Unutra.add(s.mapaPrijelaza.get("$"));
                                                }

                                            }

                                        }
                                    }
                                }
                                velikaListaUnutra.clear();
                                velikaListaUnutra = velikaLista2Unutra;
                            }

                        }
                    }

                }

            }
            queue.clear();
            if (pomQueue.isEmpty()) {
                System.out.print("#");
            } else {
                System.out.print(pomQueue.peek().stanje);
                queue.add(pomQueue.poll());
                while (pomQueue.peek() != null) {
                    System.out.print("," + pomQueue.peek().stanje);
                    queue.add(pomQueue.poll());
                }
            }

        }
        System.out.println("");
    }
}

class Stanje implements Comparable<Stanje> {
    String stanje;
    HashMap<String, ArrayList<Stanje>> mapaPrijelaza;

    @Override
    public int compareTo(Stanje o) {
        return stanje.compareTo(o.stanje);
    }

}
