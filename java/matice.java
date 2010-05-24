import java.util.Random;
import java.text.NumberFormat;
import java.lang.Math;
import java.io.*;

/** Class representing matrix with methods for matrix algebra
 * Copylefted by: Harvie 2oo9 ( http://blog.harvie.cz/ )
 * @author      Thomas Harvie Mudrunka (mudruto1)
 * @version     1.0
 */

class Matrix implements Serializable {
	public float[][] matrix;
	public final int x, y;
	private Random rnd = new Random();

	/** Construct new zero matrix described by (rows,cols) */
	Matrix(int i, int j) {
		x = i;
		y = j;
		matrix = new float[x][y];
		for(i = 0;i < x;i++) for(j = 0;j < y;j++) matrix[i][j] = 0;
	}

	/** Construct new matrix from (2d_array) */
	Matrix(float[][] m) {
		x = m.length;
		y = m[0].length;
		matrix = m;
	}

	/** Return matrix as multiline String ready to output */
	public String toString() {
		String out = new String("");
		for(int i = 0;i < x;i++) {
			out += "|\t";
			for(int j = 0;j < y;j++) out += (NumberFormat.getInstance().format(matrix[i][j])+"\t");
			out += "|\n";
		}
		return out;
	}

	/** Print matrix to console */
	public void print() {
		System.out.println(this.toString());
	}

	/** Randomize matrix with numbers x, where: 0 <= x < max */
	public void randomize(int max) {
		for(int i = 0;i < x;i++) for(int j = 0;j < y;j++) matrix[i][j] = rnd.nextInt(max);
	}

	/** Compare size of this and another matrix */
	public boolean compatible(Matrix m) {
		if(m.x == this.x && m.y == this.y) return true;
		System.err.println("Cannot add/subtract/multiply two matrices with different sizes!");
		return false;
	}

	/** Add another matrix to this and return result */
	public Matrix add(Matrix m) {
		if(!compatible(m)) return null;
		Matrix o = new Matrix(x,y);
		for(int i = 0;i < o.x;i++) for(int j = 0;j < o.y;j++) o.matrix[i][j] += this.matrix[i][j];
		for(int i = 0;i < o.x;i++) for(int j = 0;j < o.y;j++) o.matrix[i][j] += m.matrix[i][j];
		return o;
	}

	/** Subtract another matrix from this and return result */
	public Matrix subtract(Matrix m) {
		if(!compatible(m)) return null;
		Matrix o = new Matrix(x,y);
		for(int i = 0;i < o.x;i++) for(int j = 0;j < o.y;j++) o.matrix[i][j] += this.matrix[i][j];
		for(int i = 0;i < o.x;i++) for(int j = 0;j < o.y;j++) o.matrix[i][j] -= m.matrix[i][j];
		return o;
	}

	/** Scalar-multiply this matrix by another one and return result */
	public Matrix multiply(Matrix m) {
		if(!compatible(m)) return null;
		Matrix o = new Matrix(x,y);
		for(int i = 0;i < o.x;i++) for(int j = 0;j < o.y;j++) o.matrix[i][j] += this.matrix[i][j];
		for(int i = 0;i < o.x;i++) for(int j = 0;j < o.y;j++) o.matrix[i][j] *= m.matrix[i][j];
		return o;
	}

	/** Matrix-multiply this matrix by another one and return result */
	public Matrix mmultiply(Matrix m) {
		if(this.y != m.x) {
			System.err.println("Cannot multiply those two matrices!");
			return null;
		}
		Matrix o = new Matrix(this.x,m.y);
		for(int i = 0;i < o.x;i++) for(int j = 0;j < o.y;j++) {
			for(int z = 0;z < this.y;z++)	o.matrix[i][j] += this.matrix[i][z] * m.matrix[z][j];
		}
		return o;
	}

	/** Return matrix representing this matrix with swapped rows a and b */
	public Matrix swap_rows(int a, int b) {
		Matrix o = new Matrix(x,y);
		int i, j;
		for(i = 0;i < o.x;i++) for(j = 0;j < o.y;j++) o.matrix[i][j] += this.matrix[i][j];
		float tmp[] = o.matrix[a];
		o.matrix[a] = o.matrix[b];
		o.matrix[b] = tmp;
		return o;
	}

	/** Return determinant of this matrix */
	public double determinant() {
		System.err.println("TODO: Determinant!");
		return 0;
	}

	/*public float SIM_MIN(float a, float b) {
  	return (a < b ? a : b);
	}

	public double fabs(double a) {
  	return Math.abs(a);
	}*/

	/** Return matrix representing upper triangle format of this matrix */
	public Matrix echelon() {
		System.err.println("Reducing to echelon row form is not working properly!");
		//return null;
		Matrix o = new Matrix(x,y);
		int i, j;
		for(i = 0;i < o.x;i++) for(j = 0;j < o.y;j++) o.matrix[i][j] += this.matrix[i][j];

		for(int row = x; row >= 0; row--) {
			//reduceRow(row);
      	double multiplier;
      	for(j=row+1; j < y; j++) {
         	if(o.matrix[row][j] != 0) {
            	multiplier = -o.matrix[row][j];
            	//addRow(j, row, multiplier);
							//(int fromRow, int toRow, double mult)
								for(i=0; i<y; i++) {
      						o.matrix[row][i] += o.matrix[j][i]*multiplier;
      					}
         	}
      	}
    }
		/*int lead = 0;

		for(int r = 0; r < x; r++) {
			if(x <= lead)	{
				return o;
			}
			i = r;
			while(o.matrix[i][lead] == 0) {
				i++;
				if(x == i) {
					i = r;
					lead++;
					if(y == lead)	{
						return o;
					}
				}
			}
			o = o.swap_rows(i, r);
			for(j = 0;j < y; j++) o.matrix[r][j] /= o.matrix[r][lead];
			for(int row = 0; row < x; row++)
			{
				if(row != r)
				{
					for(int l = 0; l < y; l++)
						 o.matrix[row][l] -= o.matrix[i][lead] * o.matrix[r][l];
				}
			}
			lead++;
		}
		*/

		return o;
	}

	/** Serialize this object to file specified by its name (and path) */
	public boolean save(String file) {
		try {
    	ObjectOutputStream os = new ObjectOutputStream(new FileOutputStream(file));
    	os.writeObject(this);
    	os.close();
		} catch (Exception e) {
      e.printStackTrace();
			return false;
		}
		return true;
	}

	/** Deserialize and return Matrix object from file specified by its name (and path) */
	public static Matrix load(String file) {
		Matrix m = null;
		try {
    	ObjectInputStream is = new ObjectInputStream(new FileInputStream(file));
    	m = (Matrix) is.readObject();
    	is.close();
		} catch (Exception e) {
      e.printStackTrace();
		}
		return m;
	}


}

/** Class demonstrating usage of Matrix class */
public class matice {
	public static void main(String[] argv) {
		System.out.println("You has matrix! Follow the black habit ;o)\n");

		String file = "f.matrix";

		System.out.println("Created matrix F and saved to file "+file+" =");
		Matrix f = new Matrix(3,3); f.randomize(2); f.print();
		f.save(file);

		System.out.println("Loaded matrix G from file "+file+" =");
		Matrix g = Matrix.load(file); g.print();

		System.exit(0);

		System.out.println("Created matrix A =");
		Matrix a = new Matrix(3,3); a.randomize(2); a.print();

		System.out.println("Created matrix B =");
		Matrix b = new Matrix(new float[][] {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9}
    });
		b.print();

		/*System.out.println("Row Echelon form of B =");
		b.echelon().print();
		*/

		System.out.println("A + B =");
		a.add(b).print();

		System.out.println("A - B =");
		a.subtract(b).print();

		System.out.println("A * B =");
		a.multiply(b).print();

		System.out.println("Swap rows 0 and 1 of matrix B =");
		b.swap_rows(0,1).print();

		System.out.println("Created matrix C =");
		Matrix c = new Matrix(3,4); c.randomize(20); c.print();

		System.out.println("Created matrix D =");
		Matrix d = new Matrix(4,2); d.randomize(10); d.print();

		System.out.println("C . D =");
		c.mmultiply(d).print();
	}
}

/* Echelon debug:

B: matrix(
 [1,2,3],
 [4,5,6],
 [7,8,9]
);

echelon(B) = matrix(
[1,2,3],
[0,1,2],
[0,0,0]
)

**********************************/


/* Example:

0 ;) harvie@harvie-ntb prg $ javac matice.java; java matice
You has matrix! Follow the black habit ;o)

Created matrix A =
|	0	0	0	|
|	0	0	1	|
|	1	1	0	|

Created matrix B =
|	8	9	8	|
|	7	9	6	|
|	7	4	8	|

A + B =
|	8	9	8	|
|	7	9	7	|
|	8	5	8	|

A - B =
|	-8	-9	-8	|
|	-7	-9	-5	|
|	-6	-3	-8	|

A * B =
|	0	0	0	|
|	0	0	6	|
|	7	4	0	|

Created matrix C =
|	19	12	2	7	|
|	1	3	8	12	|
|	7	5	13	4	|

Created matrix D =
|	8	2	|
|	7	8	|
|	8	4	|
|	5	7	|

C . D =
|	287	191	|
|	153	142	|
|	215	134	|

*/
