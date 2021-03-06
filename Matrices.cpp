#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

using namespace std;

class Fraction
{
public:
	long long int numerator;
	long long int denominator;
	double decimalVal;

	Fraction()
	{
		this->numerator = 0;
		this->denominator = 1;
		this->decimalVal = 0;
	}

	Fraction(long long int numerator)
	{
		this->numerator = numerator;
		this->denominator = 1;
		this->decimalVal = (double)numerator;
	}

	Fraction(double numerator)
	{
		long long int intPart = (long long int)numerator;
		double dummy;
		Fraction f(intPart);
		numerator -= intPart;
		long long int denom = 1;
		int i = 0;
		while (abs(modf(numerator, &dummy)) > 0.0001 && abs(modf(numerator, &dummy)) < 0.9999 && i < 8)
		{
			numerator *= 10;
			denom *= 10;
			i++;
		}
		if (abs(modf(numerator, &dummy)) > 0.9999)
		{
			numerator = (long long int)numerator + 1;
		}
		Fraction f2(numerator, denom);
		f2.simplify();
		Fraction me = f + f2;
		me.simplify();
		this->numerator = me.numerator;
		this->denominator = me.denominator;
		this->decimalVal = (double)this->numerator / this->denominator;
	}

	Fraction(long long int numerator, long long int denominator)
	{
		this->numerator = numerator;
		this->denominator = denominator;
		this->decimalVal = (double)this->numerator / (double)this->denominator;
	}

	Fraction(const Fraction& other)
	{
		this->numerator = other.numerator;
		this->denominator = other.denominator;
		this->decimalVal = other.decimalVal;
	}

	static long long int gcd(long long int a, long long int b)
	{
		if (b == 0)
		{
			return a;
		}
		else
		{
			return Fraction::gcd(b, a % b);
		}
	}

	static long long int gcm(long long int a, long long int b)
	{
		if (a > b && a % b == 0)
			return a;
		if (b > a && b % a == 0)
			return b;

		long long int biggest = a, smallest = b;
		if (b > biggest)
		{
			biggest = b;
			smallest = a;
		}

		for (long long int i = 2 * biggest; true; i += biggest)
		{
			if (i % smallest == 0)
			{
				return i;
			}
		}
	}

	void simplify()
	{
		long long int gcdTB = Fraction::gcd(this->numerator, this->denominator);
		if (gcdTB != 1)
		{
			this->numerator /= gcdTB;
			this->denominator /= gcdTB;
		}
	}

	string getFraction()
	{
		string strFraction;
		if (this->denominator != 1 && this->numerator != 0 && this->numerator != this->denominator)
		{
			strFraction = to_string(this->numerator) + "/" + to_string(this->denominator);
		}
		else
		{
			if (this->numerator == this->denominator)
			{
				strFraction = "1";
			}
			else
			{
				strFraction = to_string(this->numerator);
			}
		}
		return strFraction;
	}

	Fraction operator+(const Fraction& other)
	{
		if (this->denominator != other.denominator)
		{
			long long int newDenom = Fraction::gcm(this->denominator, other.denominator);
			Fraction newMe(this->numerator * (newDenom / this->denominator), newDenom);
			Fraction newThem(other.numerator * (newDenom / other.denominator), newDenom);
			return newMe + newThem;
		}
		else
		{
			Fraction result(this->numerator + other.numerator, this->denominator);
			result.simplify();
			return result;
		}
	}

	Fraction operator*(const Fraction& other)
	{
		Fraction result(this->numerator * other.numerator, this->denominator * other.denominator);
		result.simplify();
		return result;
	}

	bool operator==(const Fraction& other)
	{
		return this->decimalVal == other.decimalVal;
	}

	bool operator==(const double otherVal)
	{
		return this->decimalVal == otherVal;
	}

	bool operator!=(const Fraction& other)
	{
		return this->decimalVal != other.decimalVal;
	}

	bool operator!=(const double otherVal)
	{
		return this->decimalVal != otherVal;
	}
};

class Matrix
{
protected:
	Fraction** matrix;
	int elements;
	int m;
	int n;
	bool isSquare;

public:
	Matrix()
	{
		//cout << "Matrix default ctor\n";
		this->matrix = NULL;
		this->elements = 0;
		this->m = 0;
		this->n = 0;
		this->isSquare = false;
	}

	Matrix(const Matrix& other)
	{
		//cout << "Matrix copy ctor\n";
		this->m = other.m;
		this->n = other.n;

		this->matrix = new Fraction*[this->m];
		for (int i = 0; i < this->m; i++)
		{
			this->matrix[i] = new Fraction[this->n];
		}

		for (int i = 0; i < this->m; i++)
		{
			for (int j = 0; j < this->n; j++)
			{
				this->matrix[i][j] = other.matrix[i][j];
			}
		}

		this->elements = other.elements;
		this->isSquare = other.isSquare;
	}

	Matrix(int m, int n)
	{
		//cout << "Matrix two long long int ctor\n";
		matrix = new Fraction*[m];
		for (int i = 0; i < m; i++)
		{
			matrix[i] = new Fraction[n];
		}
		for (int i = 0; i < m; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (j != i)
				{
					Fraction f((long long int)0);
					matrix[i][j] = f;
				}
				else
				{
					Fraction f((long long int)1);
					matrix[i][j] = f;
				}
			}
		}
		this->m = m;
		this->n = n;
		this->elements = m * n;
		if (this->m == this->n)
		{
			this->isSquare = true;
		}
		else
		{
			this->isSquare = false;
		}
	}

	virtual ~Matrix()
	{
		//cout << "Matrix dtor\n";
		for (int i = 0; i < this->m; i++)
		{
			delete[] this->matrix[i];
		}
		delete[] this->matrix;
	}

	void fill(double num)
	{
		Fraction f(num);
		for (int i = 0; i < this->m; i++)
		{
			for (int j = 0; j < this->n; j++)
			{
				this->matrix[i][j] = f;
			}
		}
	}

	int getNumRows()
	{
		return this->m;
	}

	int getNumCols()
	{
		return this->n;
	}

	Fraction** getMatrix()
	{
		return this->matrix;
	}

	void scale(double scalar)
	{
		Fraction f(scalar);
		for (int i = 0; i < this->m; i++)
		{
			for (int j = 0; j < this->n; j++)
			{
				this->matrix[i][j] = this->matrix[i][j] * f;
			}
		}
	}

	void print()
	{
		int padding = 0;
		int biggest = 0;
		for (int i = 0; i < this->m; i++)
		{
			for (int j = 0; j < this->n; j++)
			{
				if (this->matrix[i][j].getFraction().length() > biggest)
				{
					biggest = this->matrix[i][j].getFraction().length();
				}
			}
		}
		padding = biggest;

		for (int i = 0; i < this->m; i++)
		{
			if (i == 0 || i == this->m - 1)
				cout << "[";
			else
				cout << "|";
			for (int j = 0; j < this->n; j++)
			{
				cout << setw(padding + 1) << this->matrix[i][j].getFraction();
				if (j != this->n - 1)
				{
					cout << " ";
				}
			}
			if (i == this->m - 1 || i == 0)
				cout << "]\n";
			else
				cout << "|\n";
		}
	}

	bool set(int i, int j, double value)
	{
		Fraction f(value);
		if (i >= this->m || j >= this->n)
		{
			cout << "Matrix[" << i << "][" << j << "] does not exist!!!\n";
			return false;
		}
		else
		{
			this->matrix[i][j] = f;
			return true;
		}
	}

	//Switches two rows
	static void rowOp1(Matrix& m, int r1, int r2)
	{
		cout << "Switch rows " << r1 + 1 << " and " << r2 + 1 << endl;
		if (r1 < m.getNumRows() && r2 < m.getNumRows())
		{
			if (r1 == r2)
			{
				return;
			}
			Fraction* temp = m.matrix[r1];
			m.matrix[r1] = m.matrix[r2];
			m.matrix[r2] = temp;
		}
		else
		{
			cout << "can't switch rows, invalid rownums\n";
		}
	}

	//Multiplies a row by a scalar
	template<typename Number>
	static void rowOp2(Matrix& m, int row, Number scalar)
	{
		cout << "Multiply row " << row + 1 << " by " << scalar.getFraction() << endl;
		if (row < m.getNumRows())
		{
			Fraction f(scalar);
			for (int i = 0; i < m.getNumCols(); i++)
			{
				m.matrix[row][i] = m.matrix[row][i] * f;
			}
		}
		else
		{
			cout << "can't multiply row, invalid rownum\n";
		}
	}

	//Adds a scalar multiple of one row to another
	template<typename Number>
	static void rowOp3(Matrix& m, int targetRow, Number scalar, int otherRow)
	{
		cout << "Add " << scalar.getFraction() << " times row " << otherRow + 1 << " to row " << targetRow + 1 << endl;
		if (targetRow < m.getNumRows() && otherRow < m.getNumRows())
		{
			Fraction f(scalar);
			for (int i = 0; i < m.getNumCols(); i++)
			{
				m.matrix[targetRow][i] = m.matrix[targetRow][i] + (m.matrix[otherRow][i] * f);
			}
		}
		else
		{
			cout << "can't add multiple of row to other row, invalid rownums\n";
		}
	}

	void beautify()
	{
		int* leftZeros = new int[this->m];
		for (int i = 0; i < this->m; i++)
		{
			leftZeros[i] = 0;
		}
		for (int i = 0; i < this->m; i++)
		{
			for (int j = 0; j < this->n; j++)
			{
				if (this->matrix[i][j] == 0)
				{
					leftZeros[i]++;
				}
				else
				{
					break;
				}
			}
		}
		for (int i = 0; i < this->m; i++)
		{
			int switchRow = i;
			for (int j = i; j < this->m; j++)
			{
				if (leftZeros[j] < leftZeros[switchRow])
				{
					switchRow = j;
				}
			}
			if (i != switchRow)
			{
				Matrix::rowOp1(*this, i, switchRow);
			}
			int temp = leftZeros[i];
			leftZeros[i] = leftZeros[switchRow];
			leftZeros[switchRow] = temp;
		}
		delete[] leftZeros;
	}

	static void reduceUpperColumn(Matrix& m, int col, int row)
	{
		int initRow = row;
		row--;
		for (row; row >= 0; row--)
		{
			if (m.matrix[row][col] != 0)
			{
				Fraction f(-1.0);
				Matrix::rowOp3<Fraction>(m, row, m.matrix[row][col] * f, initRow);
			}
		}
	}

	static void reduceLowerColumn(Matrix& m, int col, int row)
	{
		int initRow = row;
		row++;
		for (row; row < m.getNumRows(); row++)
		{
			if (m.matrix[row][col] != 0)
			{
				Fraction f(-1.0);
				Matrix::rowOp3<Fraction>(m, row, m.matrix[row][col] * f, initRow);
			}
		}
	}

	Matrix toRREF()
	{
		Matrix rref(*this);
		cout << "\nSTEPS:\n-------\n";

		for (int i = 0; i < this->m; i++)
		{
			for (int j = i; j < this->n; j++)
			{
				if (rref.matrix[i][j] != 0)
				{
					Fraction f(rref.matrix[i][j].denominator, rref.matrix[i][j].numerator);
					if (rref.matrix[i][j] != 1)
					{
						Matrix::rowOp2<Fraction>(rref, i, f);
					}
					Matrix::reduceLowerColumn(rref, j, i);
					break;
				}
				else
				{
					int tempRowIndex = i + 1;
					bool switchedRow = false;
					while (tempRowIndex < this->n && !switchedRow)
					{
						if (rref.matrix[tempRowIndex][j] != 0)
						{
							Matrix::rowOp1(rref, i, tempRowIndex);
							switchedRow = true;
						}
						tempRowIndex++;
					}
					j--;
				}
			}
		}
		rref.beautify();
		for (int i = 0; i < this->m; i++)
		{
			for (int j = i; j < this->n; j++)
			{
				if (rref.matrix[i][j] != 0)
				{
					Matrix::reduceUpperColumn(rref, j, i);
					break;
				}
			}
		}
		cout << "-------\n";
		return rref;
	}

	Matrix operator+(const Matrix &other) const
	{
		if (this->m == other.m && this->n == other.n)
		{
			Matrix result(this->m, this->n);
			for (int i = 0; i < this->m; i++)
			{
				for (int j = 0; j < this->n; j++)
				{
					result.set(i, j, (this->matrix[i][j] + other.matrix[i][j]).decimalVal);
				}
			}
			return result;
		}
		else
		{
			cout << "Bad matrix addition!!!\n";
			exit(0);
		}
	}

	Matrix operator*(const Matrix &other)
	{
		if (this->n == other.m)
		{
			Matrix result(this->m, other.n);
			for (int i = 0; i < this->m; i++)
			{
				for (int j = 0; j < other.n; j++)
				{
					double sum = 0;
					for (int k = 0; k < this->n; k++)
					{
						sum += (this->matrix[i][k] * other.matrix[k][j]).decimalVal;
					}
					result.set(i, j, sum);
				}
			}
			return result;
		}
		else
		{
			cout << "Bad matrix multiplication!!!\n";
			exit(0);
		}
	}
};

class Vector : public Matrix
{
private:

public:
	Vector(double arr[], int size) : Matrix(size, 1)
	{
		for (int i = 0; i < size; i++)
		{
			Fraction f(arr[i]);
			this->matrix[i][0] = arr[i];
		}
	}

	Vector(int size) : Matrix(size, 1)
	{

	}

	~Vector()
	{

	}

	long long int operator*(const Vector& other)
	{
		if (other.elements != this->elements)
		{
			cout << "Bad vector dot product!!!\n";
			exit(0);
		}
		else
		{
			long long int result = 0;
			for (int i = 0; i < this->elements; i++)
			{
				result += (this->matrix[i][0] * other.matrix[i][0]).decimalVal;
			}
			return result;
		}
	}
};

int main()
{
	int m, n;
	Matrix matrices[2];

	//for (int i = 0; i < 2; i++)
	//{
	cout << "What are the dimensions of your matrix (MxN)?\n";
	cout << "M: ";
	cin >> m;
	cout << "N: ";
	cin >> n;

	Matrix *matrix = new Matrix(m, n);
	//matrices[i] = *matrix;
	matrices[0] = *matrix;

	double entry;
	for (int i = 0; i < matrix->getNumRows(); i++)
	{
		cout << "Enter the values for row " << i + 1 << " separated by a space: ";
		for (int j = 0; j < matrix->getNumCols(); j++)
		{
			cin >> entry;
			matrix->set(i, j, entry);
		}
	}
	//}

	cout << endl;
	matrices[0].print();
	Matrix ref = matrices[0].toRREF();
	cout << "\n==RREF==>\n\n";
	ref.print();

	return 0;
}
