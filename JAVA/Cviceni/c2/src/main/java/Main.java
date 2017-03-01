
import static java.lang.Math.sqrt;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;
import java.util.Random;
import java.util.Scanner;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author matoous
 */
public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        sc.useLocale(Locale.US);
        System.out.println("Nekonecny vstup: ");
        InfiniteInputer(sc);
        System.out.printf("Nahodne pole a jeho tisk, parametry: x size: %d, y size: %d, lower bound: %d, upper bound: %d\n", 4, 6, 10, 90);
        int arr[][] = Random2DArray(4,6,10,90);
        Print2DArray(arr);
        System.out.println("Fill and print array: ");
        int arr2[] = new int[20];
        FillArray(arr2);
        PrintArray(arr2);
        System.out.println("Now sort this array:");
        SortArray(arr2);
        PrintArray(arr2);
        int somenum;
        System.out.println("Factorization");
        String s = Factorization(sc);
        System.out.println(s);
    }
    
    
    public static void InfiniteInputer(Scanner sc){
        int num;
        int total = 0;
        int cnt = 0;
        int b = Integer.MIN_VALUE, sb = Integer.MIN_VALUE;
        boolean konst = true, rost = true, nerost = true, kles = true, nekles = true;
        int prev = 0;
        num = sc.nextInt();
        if(num == 0){
            System.out.println("Unable to find average.");
            return;
        }
        else{
            total += num;
            cnt++;
            b = num;
        }
        prev = num;
        while((num = sc.nextInt()) != 0){
            total += num;
            cnt++;
            if(num > b){
                sb = b;
                b = num;
            }
            else if(num > sb){
                sb = num;
            }
            if(konst && !(prev == num)) konst = false;
            if(rost && !(prev < num)) rost = false;
            if(nerost && !(num <= prev)) nerost = false;
            if(kles && !(prev > num)) kles = false;
            if(nekles && !(num >= prev)) nekles = false;
            prev = num;
        }
        double avg = (double)total/(double)cnt;
        System.out.println("Average: "+ avg);
        System.out.println("Second biggest: "+sb);
        if(konst) System.out.println("Funkce je konstantni.");
        if(rost) System.out.println("Funkce je rostouci.");
        if(nerost) System.out.println("Funkce je nerostouci.");
        if(kles) System.out.println("Funkce je klesajici.");
        if(nekles) System.out.println("Funkce je neklesajici.");
    }
    
    public static void FillArray(int arr[]){
        for(int i = 0; i < arr.length; i++){
            arr[i]=i+1;
        }
    }
    
    public static int[][] Random2DArray(int x, int y, int d, int u){
        Random rnd = new Random();
        int bound = u - d;
        int[][] arr = new int[x][y];
        for(int i = 0; i < x; i++){
            for(int j = 0; j < y; j++){
                arr[i][j] = rnd.nextInt(u)+d;
            }
        }
        return arr;
    }
    
    public static void Print2DArray(int[][] arr){
        for(int i = 0; i < arr.length; i++){
            for(int u = 0; u < arr[i].length; u++){
                System.out.print(arr[i][u]+" ");
            }
            System.out.println();
        }
    }
    
    public static void PrintArray(int arr[]){
        System.out.println(Arrays.toString(arr));
    }
    
    public static void SortArray(int arr[]){
        Arrays.sort(arr);
    }
    
    public static String Factorization(Scanner sc){
        int num;
        String outs = "";
        System.out.print("Enter the num: ");
        num = sc.nextInt();
        List<Integer> factors = PrimeFactorization(num);
        outs += num + " = ";
        for(int i = 0; i < factors.size(); i++){
            if(i == 0){
                outs += factors.get(i);
            } else{
                outs += "*" + factors.get(i);
            }
        }
        outs += "\n";
        return outs;
    }
    
    public static List<Integer> PrimeFactorization(int num){
        List<Integer> factors = new ArrayList<Integer>();
        if(num == 1) factors.add(1);
        int i = 2;
        while(num != 1){ // Sieve is for noobs with intel i3
            if(num%i == 0){
                while(num%i == 0){
                    factors.add(i);
                    num /= i;
                }
            }
            i++;
        }
        return factors;
    }
}
