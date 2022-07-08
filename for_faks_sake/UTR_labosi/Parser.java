import java.util.Scanner;

public class Parser {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        String line = sc.nextLine();
        StringBuilder ulaz = new StringBuilder(line);

        boolean c = S(ulaz);
        System.out.println("");

        if (c && ulaz.length() == 0) {
            System.out.println("DA");
        } else {
            System.out.println("NE");
        }

        sc.close();

    }

    public static boolean S(StringBuilder ulaz) {
        System.out.print("S");
        if (ulaz.length() > 0) {
            if (ulaz.charAt(0) == 'a') {
                ulaz = ulaz.deleteCharAt(0);
                // System.out.println("Prosao prvi a, ulaz: " + ulaz);
                if (A(ulaz)) {
                    if (!B(ulaz)) {
                        return false;
                    }
                } else {
                    return false;
                }

            } else if (ulaz.charAt(0) == 'b') {
                ulaz = ulaz.deleteCharAt(0);
                if (B(ulaz)) {
                    if (!A(ulaz)) {
                        return false;
                    }
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
        return true;
    }

    public static boolean A(StringBuilder ulaz) {
        System.out.print("A");
        if (ulaz.length() > 0) {
            if (ulaz.charAt(0) == 'b') {
                ulaz = ulaz.deleteCharAt(0);
                if (!C(ulaz)) {
                    return false;
                }
            } else if (ulaz.charAt(0) == 'a') {
                ulaz = ulaz.deleteCharAt(0);
                // System.out.println("Prosao drugi a, ulaz: " + ulaz);
                return true;
            } else {
                return false;
            }
        } else {
            return false;
        }

        return true;
    }

    public static boolean B(StringBuilder ulaz) {
        System.out.print("B");
        if (ulaz.length() > 0) {
            if (ulaz.charAt(0) == 'c' && ulaz.charAt(1) == 'c') {
                ulaz = ulaz.delete(0, 2);
                // System.out.println("trenutni ulaz: " + ulaz);
                if (!S(ulaz)) {
                    return false;
                }
                if (ulaz.charAt(0) == 'b' && ulaz.charAt(1) == 'c') {
                    ulaz = ulaz.delete(0, 2);
                    return true;
                } else {
                    return false;
                }
            }
        }

        // System.out.println("Prosao B, ulaz: " + ulaz);

        return true;
    }

    public static boolean C(StringBuilder ulaz) {
        System.out.print("C");
        if (A(ulaz)) {
            if (!A(ulaz)) {
                return false;
            }
        } else {
            return false;
        }

        return true;
    }

}
