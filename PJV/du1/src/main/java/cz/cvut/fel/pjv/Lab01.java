package cz.cvut.fel.pjv;

import java.math.BigDecimal;
import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Locale;
import java.util.Scanner;

public class Lab01 {

    public void start(String[] args) {
        homework();
    }

    private void homework() {
        Scanner sc = new Scanner(System.in);
        sc.useLocale(Locale.US);
        int o, p;
        BigDecimal a, b, c;
        DecimalFormat df = new DecimalFormat();
        df.setGroupingUsed(false);
        DecimalFormatSymbols ds = new DecimalFormatSymbols();
        ds.setDecimalSeparator('.');
        df.setDecimalFormatSymbols(ds);
        System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):");
        o = sc.nextInt();
        switch (o) {
            case 1:
                System.out.println("Zadej scitanec: ");
                a = sc.nextBigDecimal();
                System.out.println("Zadej scitanec: ");
                b = sc.nextBigDecimal();
                System.out.println("Zadej pocet desetinnych mist: ");
                p = sc.nextInt();
                if (p < 0) {
                    System.out.println("Chyba - musi byt zadane kladne cislo!");
                } else {
                    df.setMaximumFractionDigits(p);
                    df.setMinimumFractionDigits(p);
                    System.out.printf("%s + %s = %s\n", df.format(a), df.format(b), df.format(a.add(b)));
                }
                break;
            case 2:
                System.out.println("Zadej mensenec: ");
                a = sc.nextBigDecimal();
                System.out.println("Zadej mensitel: ");
                b = sc.nextBigDecimal();
                System.out.println("Zadej pocet desetinnych mist: ");
                p = sc.nextInt();
                if (p < 0) {
                    System.out.println("Chyba - musi byt zadane kladne cislo!");
                } else {
                    df.setMaximumFractionDigits(p);
                    df.setMinimumFractionDigits(p);
                    System.out.printf("%s - %s = %s\n", df.format(a), df.format(b), df.format(a.subtract(b)));
                }
                break;
            case 3:
                System.out.println("Zadej cinitel: ");
                a = sc.nextBigDecimal();
                System.out.println("Zadej cinitel: ");
                b = sc.nextBigDecimal();
                System.out.println("Zadej pocet desetinnych mist: ");
                p = sc.nextInt();
                if (p < 0) {
                    System.out.println("Chyba - musi byt zadane kladne cislo!");
                } else {
                    df.setMaximumFractionDigits(p);
                    df.setMinimumFractionDigits(p);
                    System.out.printf("%s * %s = %s\n", df.format(a), df.format(b), df.format(a.multiply(b)));
                }
                break;
            case 4:
                System.out.println("Zadej delenec: ");
                a = sc.nextBigDecimal();
                System.out.println("Zadej delitel: ");
                b = sc.nextBigDecimal();
                if (b.compareTo(BigDecimal.ZERO) == 0) {
                    System.out.println("Pokus o deleni nulou!");
                } else {
                    System.out.println("Zadej pocet desetinnych mist: ");
                    p = sc.nextInt();
                    if (p < 0) {
                        System.out.println("Chyba - musi byt zadane kladne cislo!");
                    } else {
                        df.setMaximumFractionDigits(p);
                        df.setMinimumFractionDigits(p);
                        System.out.printf("%s / %s = %s\n", df.format(a), df.format(b), df.format(a.divide(b,1000, RoundingMode.CEILING)));
                    }
                }
                break;
            default:
                System.out.println("Chybna volba!");
                break;
        }
    }
}
