/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cviceni_12;

/**
 *
 * @author hapallen
 */
public class Fronta {

    /**
     * 
     */
    private Prvek hlava;
    private Prvek konec;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Fronta f = new Fronta();
        Zakaznik z1 = new Zakaznik("Ales", "A", 2);
        f.pridejPrvekNaKonec(new Prvek(z1));
        System.out.println(f);
        Zakaznik z2 = new Zakaznik("Beda", "B", 3);
        Zakaznik z3 = new Zakaznik("Cenek", "C", 4);
        f.pridejPrvekNaKonec(new Prvek(z2));
         System.out.println(f);
        f.pridejPrvekNaKonec(new Prvek(z3));
         System.out.println(f);
        Prvek p = f.odeberPrvekZeZacatku();
        System.out.println(p.getZakaznik());
        System.out.println(f);
    }

    /**
     * prida na konec fronty zakaznika
     * @param p prvek, ktery pridavam
     */
    public void pridejPrvekNaKonec(Prvek p) {
        if (hlava == null) {
            hlava = p;
        } else {
            p.setPredchozi(konec);
            konec.setDalsi(p);
        }
        konec = p;
    }

    /**
     * odebere ze zacatku fronty zakaznika
     * @return
     */
    public Prvek odeberPrvekZeZacatku() {
        Prvek p = null;
        p = hlava;
        if (hlava != null) {
            hlava.getDalsi().setPredchozi(null);
            hlava = hlava.getDalsi();
            p.setDalsi(null);
        }
        return p;
    }

    /**
     * vypise seznam zakazniku ve fronte
     * @return
     */
    public String toString(){
        String s = "";
        Prvek aktualni = hlava;
        int  i=0;
        while(aktualni != null){
            i++;
            Zakaznik z = aktualni.getZakaznik();
            s += String.format("Zakaznik c.%d: %s",i ,z.toString());
            aktualni = aktualni.getDalsi();
        }
        return s;
    }
    /**
     * najde prvniho zakaznika, ktery ma zadane prijmeni
     * @param prijmeni vyhledavane prijmeni
     * @return nalezeny zakaznik
     */
    public Zakaznik najdiPodlePrijmeni(String prijmeni){
            Zakaznik z = null;
            Prvek aktualni = hlava;

        while(aktualni != null){
            Zakaznik zTmp = aktualni.getZakaznik();
            if(zTmp.getPrijmeni().equalsIgnoreCase(prijmeni)){
                z = zTmp;
                break;
            }
            aktualni = aktualni.getDalsi();
        }
        return z;
    }


}
