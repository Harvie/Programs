/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package cviceni_12;

/**
 *
 * @author hapallen
 */
public class Prvek {

    private Zakaznik zakaznik;

    private Prvek predchozi;

    private Prvek dalsi;

    public Prvek(Zakaznik zakaznik) {
        this.zakaznik = zakaznik;
    }

    public Prvek getDalsi() {
        return dalsi;
    }

    public void setDalsi(Prvek dalsi) {
        this.dalsi = dalsi;
    }

    public Prvek getPredchozi() {
        return predchozi;
    }

    public void setPredchozi(Prvek predchozi) {
        this.predchozi = predchozi;
    }

    public Zakaznik getZakaznik() {
        return zakaznik;
    }

    public void setZakaznik(Zakaznik zakaznik) {
        this.zakaznik = zakaznik;
    }



}
