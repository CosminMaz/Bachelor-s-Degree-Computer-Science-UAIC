package Compulsory;
public class Compulsory {
    public Compulsory() {
        System.out.println("Hello world");
        String[] languages = {"C", "C++", "C#", "Python", "Go", "Rust", "JavaScript", "PHP", "Swift", "Java"};

        int n = (int)(Math.random() * 1_000_000);
        n *= 3;
        n += 0xFF;
        n += 0x10101;
        n *= 6;
        while(n > 9){
            int temp = n;
            int sum = 0;
            while(temp != 0){
                sum += temp % 10;
                temp /= 10;
            }
            n = sum;
        }
        System.out.println("Willy-nilly, this semester I will learn " + languages[n] + ".");
    }
}
