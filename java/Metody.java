package seminar7;

import java.util.*;

public class Metody {
    
    public static void main(String[] args) {
        
        Scanner sca = new Scanner(System.in);
        int a = sca.nextInt();
        int b = sca.nextInt();
        int c = sca.nextInt();
        
        over(a,b,c);
        pravouhly(a,b,c);
        obvod(a,b,c);
        //obsah(a,b,c);
    }
    
    static void over(int a, int b, int c)
    {
        int j = 1;
        if((a+b) < c)
            j = 0;
        if((b+c) < a)
            j = 0;
        if((c+a) < b)
            j = 0;
        if(j == 0);
            System.out.println("Neni trojuhelnik!");
    }
    
    static void obvod(int a, int b, int c)
    {
        int o = (a + b + c);
            System.out.println("Obvod O = " + o);
    }
    
    static void obsah(int a, int b, int c)
    {
        int o = (a + b + c); // Zatim obvod
            System.out.println("Obsah S = " + o);
    }
    
    static void pravouhly(int a, int b, int c)
    {
        int sa = 0, sb = 0, sc = 0;
        if(((a * a) + (b * b)) == (c * c))
            {
            sa = a;
            sb = b;
            sc = c;
            }
        
        if(((c * c) + (b * b)) == (a * a))
            {
            sa = c;
            sb = b;
            sc = a;
            }
        
        if(((a * a) + (c * c)) == (b * b))
            {
            sa = a;
            sb = c;
            sc = b;
            }
        
        if((sa * sa + sb * sb) != (sc * sc))
            System.out.println("Neni pravouhly!");
    }
    
}
