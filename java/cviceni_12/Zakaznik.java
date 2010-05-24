/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cviceni_12;

/**
 *
 * @author hapallen
 */
public class Zakaznik {

    private String jmeno;
    private String prijmeni;
    private int vek;

    public Zakaznik(String jmeno, String prijmeni, int vek) {
        this.jmeno = jmeno;
        this.prijmeni = prijmeni;
        this.vek = vek;
    }

    public String getJmeno() {
        return jmeno;
    }

    public void setJmeno(String jmeno) {
        this.jmeno = jmeno;
    }

    public String getPrijmeni() {
        return prijmeni;
    }

    public void setPrijmeni(String prijmeni) {
        this.prijmeni = prijmeni;
    }

    public int getVek() {
        return vek;
    }

    public void setVek(int vek) throws Exception {
        if (vek >= 0) {
            this.vek = vek;
        } else {
            throw new Exception("Nespraavny vek");
        }

    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Zakaznik) {
            Zakaznik z = (Zakaznik) obj;
            if (this.prijmeni.equalsIgnoreCase(z.getPrijmeni())
                    && this.jmeno.equalsIgnoreCase(z.getJmeno())
                    && this.vek == z.getVek()) {
                return true;
            }
        }
        return false;
    }

    @Override
    public String toString() {
        return String.format("Zakaznik: prijmeni: %s,jmeno: %s, vek: %d;", this.getPrijmeni(), this.getJmeno(), this.getVek());
    }
}
