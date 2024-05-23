import java.util.Scanner;

public class Labos {

	public static void main(String[] args) {
		int lambda1, lambda2;
		int a0, a1, n;
		Scanner sc = new Scanner(System.in);

		System.out.print("Unesite prvi koeficijent lambda_1 rekurzivne relacije: ");
		lambda1 = sc.nextInt();
		System.out.print("Unesite drugi koeficijent lambda_2 rekurzivne relacije: ");
		lambda2 = sc.nextInt();
		System.out.print("Unesite vrijednost nultog clana niza a_0: ");
		a0 = sc.nextInt();
		System.out.print("Unesite vrijednost nultog clana niza a_1: ");
		a1 = sc.nextInt();
		System.out.print("Unesite redni broj n trazenog clana niza: ");
		n = sc.nextInt();

		System.out.println("Vrijednost n-tog clana niza pomocu formule: " + RacunFormulom(lambda1, lambda2, a0, a1, n));
		System.out.println("Vrijednost n-tog clana niza iz rekurzije: " + RacunRekurzijom(lambda1, lambda2, a0, a1, n));

		sc.close();

	}

	static int RacunFormulom(int l1, int l2, int a0, int a1, int n) {
		if (n == 0)
			return a0;
		if (n == 1)
			return a1;
		float x1, x2, phi1, phi2;
		int an;
		x1 = (float) ((l1 + Math.sqrt(l1 * l1 + 4 * l2)) * 0.5);
		x2 = (float) ((l1 - Math.sqrt(l1 * l1 + 4 * l2)) * 0.5);
		if (x1 != x2) {
			phi2 = ((a1 - a0 * x1) / (x2 - x1));
			phi1 = a0 - phi2;
			an = Math.round((float) (phi1 * Math.pow(x1, n) + phi2 * Math.pow(x2, n)));
		} else {
			if (x1 == 0)
				return 0;
			phi1 = a0;
			phi2 = ((a1 - phi1 * x1) / x1);
			an = Math.round((float) (phi1 * Math.pow(x1, n) + phi2 * n * Math.pow(x2, n)));
		}

		return an;

	}

	static int RacunRekurzijom(int l1, int l2, int a0, int a1, int n) {
		if (n == 0)
			return a0;
		if (n == 1)
			return a1;

		int an = l1 * RacunRekurzijom(l1, l2, a0, a1, n - 1) + l2 * RacunRekurzijom(l1, l2, a0, a1, n - 2);

		return an;
	}

}
