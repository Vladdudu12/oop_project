#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include <ctime>
#include <conio.h>

using namespace std;


#define txtExtension ".txt";
#define csvExtension ".csv";
#define binExtension ".bin";

ifstream f("command.in");
//am facut citire din fisier pentru a testa mai multe functii rapid
//Nu mai suportam sa scriu a 20-a oara CREATE TABLE

//----------------------------------------COMENZI CU CARE AM TESTAT----------------------------------------------//
//CREATE TABLE persoane IF NOT EXISTS((nume,TEXT,25,dudu), (varsta,INTEGER,3,20), (buget,FLOAT,6,200.525))
//DISPLAY TABLE persoane
//INSERT INTO persoane VALUES(mihai,18,300.50)
//INSERT INTO persoane VALUES(ana,20,500.25)
//SELECT (nume,varsta) FROM persoane
//UPDATE persoane SET nume = marcel WHERE nume = mihai
//DISPLAY TABLE persoane

enum COMMAND_TYPES
{
	CREATE = 1, DROP = 2, DISPLAY = 3,
	INSERT = 11, DELETE = 12, SELECT = 13, UPDATE = 14, IMPORT = 15,
	UNDEFINED = -1
};

enum COLUMN_TYPES
{
	TEXT = 1, INTEGER = 2, FLOAT = 3,
	UNKNOWN = -1
};

class TEXT_COLUMN;
class INT_COLUMN;
class FLOAT_COLUMN;


class COLUMN
{
protected:
	string columnName = "Unknown";
	COLUMN_TYPES columnType = COLUMN_TYPES::UNKNOWN;
	int columnSize = 0;

public:
#pragma region Constructors
	COLUMN()
	{

	}

	COLUMN(string _columnName, COLUMN_TYPES _columnType, int _columnSize) :columnName(_columnName), columnType(_columnType), columnSize(_columnSize)
	{

	}


	COLUMN(const COLUMN& c)
	{
		this->columnName = c.columnName;
		this->columnType = c.columnType;
		this->columnSize = c.columnSize;
	}
#pragma endregion

#pragma region Methods
#pragma endregion

#pragma region Getters and Setters
	string getColumnName()
	{
		return this->columnName;
	}
	COLUMN_TYPES getColumnType()
	{
		return this->columnType;
	}
	int getColumnSize()
	{
		return this->columnSize;
	}

	void setColumnName(string columnName)
	{
		if (columnName.length() > 0)
		{
			this->columnName = columnName;
		}
		else
		{
			throw exception("\nERROR! Invalid columnName");
		}
	}

	void setColumnType(COLUMN_TYPES columnType)
	{
		//Nu am idee cum sa fac validare
		//poate cu un switch si sa intre pe default si sa arunce exceptie ca e invalid tipul?
		this->columnType = columnType;
	}

	void setColumnSize(int columnSize)
	{
		if (columnSize > 0)
		{
			this->columnSize = columnSize;
		}
		else
		{
			throw exception("\nERROR! Invalid columnSize");
		}
	}

#pragma endregion

#pragma region Operators
	COLUMN& operator= (const COLUMN& c)
	{
		if (this != &c)
		{
			this->columnName = c.columnName;
			this->columnType = c.columnType;
			this->columnSize = c.columnSize;
		}

		return *this;
	}
#pragma endregion

	~COLUMN()
	{

	}

#pragma region Friend Methods
	friend ostream& operator<<(ostream& out, const COLUMN& col)
	{
		out << "\nNume coloana: " << col.columnName;
		out << "\nTip coloana: " << col.columnType; //facut cu map ca sa apara tipul literal
		out << "\nDimensiune coloana: " << col.columnSize;

		return out;
	}
	friend ostream& operator<<(ostream& out, const TEXT_COLUMN& txtCol);
	friend ostream& operator<<(ostream& out, const INT_COLUMN& intCol);
	friend ostream& operator<<(ostream& out, const FLOAT_COLUMN& floatCol);
#pragma endregion
};

class TEXT_COLUMN : public COLUMN
{
	string defaultValue = "";
	int numberOfValues = 0;
	string* values = NULL;
public:
#pragma region Constructors
	TEXT_COLUMN()
	{

	}

	TEXT_COLUMN(string _columnName, COLUMN_TYPES _columnType, int _columnSize, string _defaultValue) : COLUMN(_columnName, _columnType, _columnSize), defaultValue(_defaultValue)
	{

	}

	TEXT_COLUMN(string _columnName, COLUMN_TYPES _columnType, int _columnSize, string _defaultValue, int _numberOfValues, string* _values) : COLUMN(_columnName, _columnType, _columnSize), defaultValue(_defaultValue)
	{
		if (_numberOfValues < 0)
		{
			this->numberOfValues = 0;
			this->values = NULL;
		}
		else
		{
			this->numberOfValues = _numberOfValues;
			this->values = new string[this->numberOfValues];
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = _values[i];
			}
		}
	}

	TEXT_COLUMN(const TEXT_COLUMN& txtCol)
	{
		this->columnName = txtCol.columnName;
		this->columnType = txtCol.columnType;
		this->columnSize = txtCol.columnSize;
		this->defaultValue = txtCol.defaultValue;
		if (txtCol.numberOfValues <= 0)
		{
			this->numberOfValues = 0;
			this->values = NULL;
		}
		else
		{
			this->numberOfValues = txtCol.numberOfValues;
			this->values = new string[this->numberOfValues];
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = txtCol.values[i];
			}
		}
	}

#pragma endregion

#pragma region Methods
	void showEntryAtIndex(int index)
	{
		std::cout << "\n-------------------\n";
		std::cout << "\nNumele Coloanei: " << this->columnName;
		std::cout << "\nTipul Coloanei: " << this->columnType;
		std::cout << "\nDimensiunea Coloanei: " << this->columnSize;
		std::cout << "\nValoarea Implicita: " << this->defaultValue;
		std::cout << "\nValoare: " << this->values[index];
	}
	void generateReportEntryAtIndex(int index, string filename)
	{
		ofstream file(filename, ios::out | ios::app);
		file << "\n-------------------\n";
		file << "\nNumele Coloanei: " << this->columnName;
		file << "\nTipul Coloanei: " << this->columnType;
		file << "\nDimensiunea Coloanei: " << this->columnSize;
		file << "\nValoarea Implicita: " << this->defaultValue;
		file << "\nValoare: " << this->values[index];
	}

	void saveTextColumn(string filename)
	{
		ofstream file(filename, ios::app);
		if (file.is_open())
		{
			file << this->columnName << "\n";
			file << this->columnType << "\n";
			file << this->columnSize << "\n";
			file << this->defaultValue << "\n";
			file << this->numberOfValues << "\n";
			for (int i = 0; i < this->numberOfValues; i++)
			{
				file << this->values[i] << "\n";
			}

			file.close();
		}
		else
		{
			throw exception("Nu exista fisierul cerut");
		}
	}
#pragma endregion

#pragma region Getters and Setters
	string getDefaultValue()
	{
		return this->defaultValue;
	}

	int getNumberOfValues()
	{
		return this->numberOfValues;
	}

	//nu mai stiu daca asa se face ca sa fie secure-ish
	string* getValues()
	{
		string* copy = this->values;
		return copy;
	}

	int getIndex(string value)
	{
		int index = -1;
		for (int i = 0; i < this->numberOfValues; i++)
		{
			if (this->values[i] == value)
			{
				index = i;
			}
		}

		if (index == -1)
		{
			throw exception("\nERROR! Index invalid");
		}

		return index;
	}

	void setValueAtIndex(int index, string value)
	{
		if (index > this->numberOfValues)
		{
			throw exception("\nERROR! Index invalid");
		}
		else
		{
			this->values[index] = value;
		}
	}

	void setDefaultValue(string defaultValue)
	{
		if (defaultValue.length() > 0)
		{
			this->defaultValue = defaultValue;
		}
		else
		{
			throw exception("\nERROR! Invalid defaultValue");
		}
	}

	void setValues(int numberOfValues, string* values)
	{
		if (numberOfValues > 0)
		{
			this->numberOfValues = numberOfValues;
			delete[] this->values;
			this->values = new string[this->numberOfValues];
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = values[i];
			}
		}
		else
		{
			throw exception("\nERROR! Invalid arguments");
		}
	}
#pragma endregion

#pragma region Operators
	TEXT_COLUMN& operator= (const TEXT_COLUMN& txtCol)
	{
		if (this != &txtCol)
		{
			this->columnName = txtCol.columnName;
			this->columnType = txtCol.columnType;
			this->columnSize = txtCol.columnSize;
			this->defaultValue = txtCol.defaultValue;
			if (txtCol.numberOfValues <= 0)
			{
				this->numberOfValues = 0;
				this->values = NULL;
			}
			else
			{
				this->numberOfValues = txtCol.numberOfValues;
				this->values = new string[this->numberOfValues];
				for (int i = 0; i < this->numberOfValues; i++)
				{
					this->values[i] = txtCol.values[i];
				}
			}
		}

		return *this;
	}

	TEXT_COLUMN& operator++() //NETESTAT
	{
		TEXT_COLUMN copy = *this;

		delete[] this->values;
		this->numberOfValues++;
		this->values = new string[this->numberOfValues];
		if (this->numberOfValues > 1)
		{
			for (int i = 0; i < copy.numberOfValues; i++)
			{
				this->values[i] = copy.values[i];
			}
		}

		this->values[this->numberOfValues - 1] = this->defaultValue;

		return *this;
	}

	TEXT_COLUMN operator++(int) //NETESTAT
	{
		TEXT_COLUMN copy = *this;

		delete[] this->values;
		this->numberOfValues++;
		this->values = new string[this->numberOfValues];
		if (this->numberOfValues > 1)
		{
			for (int i = 0; i < copy.numberOfValues; i++)
			{
				this->values[i] = copy.values[i];
			}
		}

		this->values[this->numberOfValues - 1] = this->defaultValue;

		return copy;
	}

	TEXT_COLUMN& operator--() //NETESTAT
	{
		TEXT_COLUMN copy = *this;

		delete[] this->values;
		this->numberOfValues--;
		this->values = new string[this->numberOfValues];
		if (this->numberOfValues > 1)
		{
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = copy.values[i];
			}
		}

		return *this;
	}

	TEXT_COLUMN& operator--(int) //NETESTAT
	{
		TEXT_COLUMN copy = *this;

		delete[] this->values;
		this->numberOfValues--;
		this->values = new string[this->numberOfValues];
		if (this->numberOfValues > 1)
		{
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = copy.values[i];
			}
		}

		return copy;
	}

	TEXT_COLUMN operator+= (string value)
	{
		TEXT_COLUMN copy = *this;

		delete[] this->values;
		this->numberOfValues++;
		this->values = new string[this->numberOfValues];
		if (this->numberOfValues > 1)
		{
			for (int i = 0; i < copy.numberOfValues; i++)
			{
				this->values[i] = copy.values[i];
			}
		}

		this->values[this->numberOfValues - 1] = value;

		return *this;
	}

	TEXT_COLUMN operator+ (string value)
	{
		TEXT_COLUMN res = *this;
		res += value;
		return res;
	}

	TEXT_COLUMN operator-= (int index)
	{

		if (index > this->numberOfValues)
		{
			throw exception("\nERROR! Invalid index");
		}
		else
		{
			TEXT_COLUMN copy = *this;
			delete[] this->values;
			this->numberOfValues--;
			if (this->numberOfValues <= 0)
			{
				this->numberOfValues = 0;
				this->values = NULL;
			}
			else
			{
				this->values = new string[this->numberOfValues];
				int j = 0;
				for (int i = 0; i < copy.numberOfValues; i++)
				{
					if (i != index)
					{
						this->values[j] = copy.values[i];
						j++;
					}
				}
			}
		}

		return *this;
	}

	TEXT_COLUMN operator- (int index)
	{
		TEXT_COLUMN res = *this;
		res -= index;
		return res;
	}

#pragma endregion

	~TEXT_COLUMN()
	{
		if (this->values != NULL)
		{
			delete[] this->values;
		}
	}

#pragma region Friend Methods
	friend ostream& operator<<(ostream& out, const TEXT_COLUMN& txtCol)
	{
		out << "\nNumele Coloanei: " << txtCol.columnName;
		out << "\nTipul Coloanei: " << txtCol.columnType;
		out << "\nDimensiunea Coloanei: " << txtCol.columnSize;
		out << "\nValoarea Implicita: " << txtCol.defaultValue;
		out << "\nValori: ";
		for (int i = 0; i < txtCol.numberOfValues; i++)
		{
			out << "\nvalues[" << i << "]: " << txtCol.values[i];
		}
		return out;
	}
#pragma endregion
};

class INT_COLUMN : public COLUMN
{
	int defaultValue = 0;
	int numberOfValues = 0;
	int* values = NULL;

public:
#pragma region Constructors

	INT_COLUMN()
	{

	}

	INT_COLUMN(string _columnName, COLUMN_TYPES _columnType, int _columnSize, int _defaultValue) : COLUMN(_columnName, _columnType, _columnSize), defaultValue(_defaultValue)
	{

	}

	INT_COLUMN(string _columnName, COLUMN_TYPES _columnType, int _columnSize, int _defaultValue, int _numberOfValues, int* _values) : COLUMN(_columnName, _columnType, _columnSize), defaultValue(_defaultValue)
	{
		if (_numberOfValues <= 0)
		{
			this->numberOfValues = 0;
			this->values = NULL;
		}
		else
		{
			this->numberOfValues = _numberOfValues;
			this->values = new int[this->numberOfValues];
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = _values[i];
			}
		}
	}

	INT_COLUMN(const INT_COLUMN& intCol)
	{
		this->columnName = intCol.columnName;
		this->columnType = intCol.columnType;
		this->columnSize = intCol.columnSize;
		this->defaultValue = intCol.defaultValue;
		if (intCol.numberOfValues <= 0)
		{
			this->numberOfValues = 0;
			this->values = NULL;
		}
		else
		{
			this->numberOfValues = intCol.numberOfValues;
			this->values = new int[this->numberOfValues];
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = intCol.values[i];
			}
		}
	}

#pragma endregion

#pragma region Methods
	void showEntryAtIndex(int index)
	{
		std::cout << "\n-------------------\n";
		std::cout << "\nNumele Coloanei: " << this->columnName;
		std::cout << "\nTipul Coloanei: " << this->columnType;
		std::cout << "\nDimensiunea Coloanei: " << this->columnSize;
		std::cout << "\nValoarea Implicita: " << this->defaultValue;
		std::cout << "\nValoare: " << this->values[index];
	}

	void generateReportEntryAtIndex(int index, string filename)
	{
		ofstream file(filename, ios::out | ios::app);
		file << "\n-------------------\n";
		file << "\nNumele Coloanei: " << this->columnName;
		file << "\nTipul Coloanei: " << this->columnType;
		file << "\nDimensiunea Coloanei: " << this->columnSize;
		file << "\nValoarea Implicita: " << this->defaultValue;
		file << "\nValoare: " << this->values[index];
	}


	void saveIntColumn(string filename)
	{
		ofstream file(filename, ios::app);
		if (file.is_open())
		{
			file << this->columnName << "\n";
			file << this->columnType << "\n";
			file << this->columnSize << "\n";
			file << this->defaultValue << "\n";
			file << this->numberOfValues << "\n";
			for (int i = 0; i < this->numberOfValues; i++)
			{
				file << this->values[i] << "\n";
			}

			file.close();
		}
		else
		{
			throw exception("Nu exista fisierul cerut");
		}
	}

#pragma endregion

#pragma region Getters and Setters
	int getDefaultValue()
	{
		return this->defaultValue;
	}

	int getNumberOfValues()
	{
		return this->numberOfValues;
	}

	//nu mai stiu daca asa e secure
	int* getValues()
	{
		int* copy = this->values;
		return copy;
	}

	int getIndex(int value)
	{
		int index = -1;
		for (int i = 0; i < this->numberOfValues; i++)
		{
			if (this->values[i] == value)
			{
				index = i;
			}
		}

		if (index == -1)
		{
			throw exception("\nERROR! Index invalid");
		}

		return index;
	}

	void setValueAtIndex(int index, int value)
	{
		if (index > this->numberOfValues)
		{
			throw exception("\nERROR! Index invalid");
		}
		else
		{
			this->values[index] = value;
		}
	}

	void setDefaultValue(int defaultValue)
	{
		if (defaultValue >= 0)
		{
			this->defaultValue = defaultValue;
		}
		else
		{
			throw exception("\nERROR! Invalid defaultValue");
		}
	}

	void setValues(int numberOfValues, int* values)
	{
		if (numberOfValues > 0)
		{
			this->numberOfValues = numberOfValues;
			delete[] this->values;
			this->values = new int[this->numberOfValues];
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = values[i];
			}
		}
		else
		{
			throw exception("\nERROR! Invalid arguments");
		}
	}

#pragma endregion

#pragma region Operators
	INT_COLUMN& operator= (const INT_COLUMN& intCol)
	{
		if (this != &intCol)
		{
			this->columnName = intCol.columnName;
			this->columnType = intCol.columnType;
			this->columnSize = intCol.columnSize;
			this->defaultValue = intCol.defaultValue;
			if (intCol.numberOfValues <= 0)
			{
				this->numberOfValues = 0;
				this->values = NULL;
			}
			else
			{
				this->numberOfValues = intCol.numberOfValues;
				this->values = new int[this->numberOfValues];
				for (int i = 0; i < this->numberOfValues; i++)
				{
					this->values[i] = intCol.values[i];
				}
			}
		}

		return *this;
	}

	INT_COLUMN operator+= (int value)
	{
		INT_COLUMN copy = *this;

		delete[] this->values;
		this->numberOfValues++;
		this->values = new int[this->numberOfValues];
		if (this->numberOfValues > 1)
		{
			for (int i = 0; i < copy.numberOfValues; i++)
			{
				this->values[i] = copy.values[i];
			}
		}

		this->values[this->numberOfValues - 1] = value;

		return *this;
	}

	INT_COLUMN operator+ (int value)
	{
		INT_COLUMN res = *this;
		res += value;
		return res;
	}

	INT_COLUMN operator-= (int index)
	{
		if (index > this->numberOfValues)
		{
			throw exception("\nERROR! Invalid index");
		}
		else
		{
			INT_COLUMN copy = *this;
			delete[] this->values;
			this->numberOfValues--;
			if (this->numberOfValues <= 0)
			{
				this->numberOfValues = 0;
				this->values = NULL;
			}
			else
			{
				this->values = new int[this->numberOfValues];
				int j = 0;
				for (int i = 0; i < copy.numberOfValues; i++)
				{
					if (i != index)
					{
						this->values[j] = copy.values[i];
						j++;
					}
				}
			}
		}

		return *this;
	}

	INT_COLUMN operator- (int index)
	{
		INT_COLUMN res = *this;
		res -= index;
		return res;
	}


#pragma endregion

	~INT_COLUMN()
	{
		if (this->values != NULL)
		{
			delete[] this->values;
		}
	}

#pragma region Friend Methods
	friend ostream& operator<<(ostream& out, const INT_COLUMN& intCol)
	{
		out << "\nNumele Coloanei: " << intCol.columnName;
		out << "\nTipul Coloanei: " << intCol.columnType;
		out << "\nDimensiunea Coloanei: " << intCol.columnSize;
		out << "\nValoarea Implicita: " << intCol.defaultValue;
		out << "\nValori: ";
		for (int i = 0; i < intCol.numberOfValues; i++)
		{
			out << "\nvalues[" << i << "]: " << intCol.values[i];
		}
		return out;
	}
#pragma endregion
};

class FLOAT_COLUMN : public COLUMN
{
	float defaultValue = 0.0;
	int numberOfValues = 0;
	float* values = NULL;

public:
#pragma region Constructors
	FLOAT_COLUMN()
	{

	}

	FLOAT_COLUMN(string _columnName, COLUMN_TYPES _columnType, int _columnSize, float _defaultValue) : COLUMN(_columnName, _columnType, _columnSize), defaultValue(_defaultValue)
	{

	}

	FLOAT_COLUMN(string _columnName, COLUMN_TYPES _columnType, int _columnSize, float _defaultValue, int _numberOfValues, float* _values) : COLUMN(_columnName, _columnType, _columnSize), defaultValue(_defaultValue)
	{
		if (_numberOfValues <= 0)
		{
			this->numberOfValues = 0;
			this->values = NULL;
		}
		else
		{
			this->numberOfValues = _numberOfValues;
			this->values = new float[this->numberOfValues];
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = _values[i];
			}
		}
	}

	FLOAT_COLUMN(const FLOAT_COLUMN& floatCol)
	{
		this->columnName = floatCol.columnName;
		this->columnType = floatCol.columnType;
		this->columnSize = floatCol.columnSize;
		this->defaultValue = floatCol.defaultValue;
		if (floatCol.numberOfValues <= 0)
		{
			this->numberOfValues = 0;
			this->values = NULL;
		}
		else
		{
			this->numberOfValues = floatCol.numberOfValues;
			this->values = new float[this->numberOfValues];
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = floatCol.values[i];
			}
		}
	}

#pragma endregion

#pragma region Methods
	void showEntryAtIndex(int index)
	{
		std::cout << "\n-------------------\n";
		std::cout << "\nNumele Coloanei: " << this->columnName;
		std::cout << "\nTipul Coloanei: " << this->columnType;
		std::cout << "\nDimensiunea Coloanei: " << this->columnSize;
		std::cout << "\nValoarea Implicita: " << this->defaultValue;
		std::cout << "\nValoare: " << this->values[index];
	}

	void generateReportEntryAtIndex(int index, string filename)
	{
		ofstream file(filename, ios::out | ios::app);
		file << "\n-------------------\n";
		file << "\nNumele Coloanei: " << this->columnName;
		file << "\nTipul Coloanei: " << this->columnType;
		file << "\nDimensiunea Coloanei: " << this->columnSize;
		file << "\nValoarea Implicita: " << this->defaultValue;
		file << "\nValoare: " << this->values[index];
	}

	void saveFloatColumn(string filename)
	{
		ofstream file(filename, ios::app);
		if (file.is_open())
		{
			file << this->columnName << "\n";
			file << this->columnType << "\n";
			file << this->columnSize << "\n";
			file << this->defaultValue << "\n";
			file << this->numberOfValues << "\n";
			for (int i = 0; i < this->numberOfValues; i++)
			{
				file << this->values[i] << "\n";
			}

			file.close();
		}
		else
		{
			throw exception("Nu exista fisierul cerut");
		}
	}
#pragma endregion

#pragma region Getters and Setters

	float getDefaultValue()
	{
		return this->defaultValue;
	}

	int getNumberOfValues()
	{
		return this->numberOfValues;
	}

	//nu stiu daca asa se face secure
	float* getValues()
	{
		float* copy = this->values;
		return copy;
	}

	int getIndex(float value)
	{
		int index = -1;
		for (int i = 0; i < this->numberOfValues; i++)
		{
			if (this->values[i] == value)
			{
				index = i;
			}
		}

		if (index == -1)
		{
			throw exception("\nERROR! Index invalid");
		}

		return index;
	}

	void setValueAtIndex(int index, float value)
	{
		this->values[index] = value;
	}

	void setDefaultValue(float defaultValue)
	{
		if (defaultValue >= 0)
		{
			this->defaultValue = defaultValue;
		}
		else
		{
			throw exception("\nERROR! Invalid defaultValue");
		}
	}

	void setValues(int numberOfValues, float* values)
	{
		if (numberOfValues > 0)
		{
			this->numberOfValues = numberOfValues;
			delete[] this->values;
			this->values = new float[this->numberOfValues];
			for (int i = 0; i < this->numberOfValues; i++)
			{
				this->values[i] = values[i];
			}
		}
		else
		{
			throw exception("\nERROR! Invalid arguments");
		}
	}

#pragma endregion

#pragma region Operators
	FLOAT_COLUMN& operator= (const FLOAT_COLUMN& floatCol)
	{
		if (this != &floatCol)
		{
			this->columnName = floatCol.columnName;
			this->columnType = floatCol.columnType;
			this->columnSize = floatCol.columnSize;
			this->defaultValue = floatCol.defaultValue;
			if (floatCol.numberOfValues <= 0)
			{
				this->numberOfValues = 0;
				this->values = NULL;
			}
			else
			{
				this->numberOfValues = floatCol.numberOfValues;
				this->values = new float[this->numberOfValues];
				for (int i = 0; i < this->numberOfValues; i++)
				{
					this->values[i] = floatCol.values[i];
				}
			}
		}

		return *this;
	}

	FLOAT_COLUMN operator+= (float value)
	{
		FLOAT_COLUMN copy = *this;

		delete[] this->values;
		this->numberOfValues++;
		this->values = new float[this->numberOfValues];
		if (this->numberOfValues > 1)
		{
			for (int i = 0; i < copy.numberOfValues; i++)
			{
				this->values[i] = copy.values[i];
			}
		}

		this->values[this->numberOfValues - 1] = value;

		return *this;
	}

	FLOAT_COLUMN operator+ (float value)
	{
		FLOAT_COLUMN res = *this;
		res += value;
		return res;
	}

	FLOAT_COLUMN operator-= (int index)
	{
		if (index > this->numberOfValues)
		{
			throw exception("\nERROR! Invalid index");
		}
		else
		{
			FLOAT_COLUMN copy = *this;
			delete[] this->values;
			this->numberOfValues--;
			if (this->numberOfValues <= 0)
			{
				this->numberOfValues = 0;
				this->values = NULL;
			}
			else
			{
				this->values = new float[this->numberOfValues];
				int j = 0;
				for (int i = 0; i < copy.numberOfValues; i++)
				{
					if (i != index)
					{
						this->values[j] = copy.values[i];
						j++;
					}
				}
			}
		}

		return *this;
	}

	FLOAT_COLUMN operator- (int index)
	{
		FLOAT_COLUMN res = *this;
		res -= index;
		return res;
	}

#pragma endregion

	~FLOAT_COLUMN()
	{
		if (this->values != NULL)
		{
			delete[] this->values;
		}
	}

#pragma region Friend Methods
	friend ostream& operator<<(ostream& out, const FLOAT_COLUMN& floatCol)
	{
		out << "\nNumele Coloanei: " << floatCol.columnName;
		out << "\nTipul Coloanei: " << floatCol.columnType;
		out << "\nDimensiunea Coloanei: " << floatCol.columnSize;
		out << "\nValoarea Implicita: " << floatCol.defaultValue;
		out << "\nValori: ";
		for (int i = 0; i < floatCol.numberOfValues; i++)
		{
			out << "\nvalues[" << i << "]: " << floatCol.values[i];
		}
		return out;
	}
#pragma endregion
};

//de facut cu template pentru tipurile de coloane pentru a fi mai usor de inteles
class TABLE {
	string tableName = "Unknown";
	int numberOfTextColumns = 0;
	int numberOfIntColumns = 0;
	int numberOfFloatColumns = 0;
	TEXT_COLUMN* textColumns = NULL;
	INT_COLUMN* intColumns = NULL;
	FLOAT_COLUMN* floatColumns = NULL;
	//COLUMN* columns = NULL; //trebuie specificate exact cum sunt coloanele, altfel nu se poate forma cum trebuie assignment, vom avea mai putina memorie alocata decat ne trebuie
public:
#pragma region Constructors
	TABLE()
	{

	}

	TABLE(string tableName)
	{
		this->tableName = tableName;
	}

	TABLE(string tableName, int numberOfTextColumns, int numberOfIntColumns, int numberOfFloatColumns, TEXT_COLUMN* textColumns, INT_COLUMN* intColumns, FLOAT_COLUMN* floatColumns)
	{
		this->tableName = tableName;

		//TEXT
		this->numberOfTextColumns = numberOfTextColumns;
		this->textColumns = new TEXT_COLUMN[this->numberOfTextColumns];
		for (int i = 0; i < this->numberOfTextColumns; i++)
		{
			this->textColumns[i] = textColumns[i];
		}

		//INT
		this->numberOfIntColumns = numberOfIntColumns;
		this->intColumns = new INT_COLUMN[this->numberOfIntColumns];
		for (int i = 0; i < this->numberOfIntColumns; i++)
		{
			this->intColumns[i] = intColumns[i];
		}

		//FLOAT
		this->numberOfFloatColumns = numberOfFloatColumns;
		this->floatColumns = new FLOAT_COLUMN[this->numberOfFloatColumns];
		for (int i = 0; i < this->numberOfFloatColumns; i++)
		{
			this->floatColumns[i] = floatColumns[i];
		}
	}

	TABLE(const TABLE& t)
	{
		this->tableName = t.tableName;

		//TEXT
		if (t.numberOfTextColumns <= 0)
		{
			this->numberOfTextColumns = 0;
			this->textColumns = NULL;
		}
		else
		{
			delete[] this->textColumns;
			this->numberOfTextColumns = t.numberOfTextColumns;
			this->textColumns = new TEXT_COLUMN[this->numberOfTextColumns];
			for (int i = 0; i < this->numberOfTextColumns; i++)
			{
				this->textColumns[i] = t.textColumns[i];
			}
		}

		//INT
		if (t.numberOfIntColumns <= 0)
		{
			this->numberOfIntColumns = 0;
			this->intColumns = NULL;
		}
		else
		{
			delete[] this->intColumns;
			this->numberOfIntColumns = t.numberOfIntColumns;
			this->intColumns = new INT_COLUMN[this->numberOfIntColumns];
			for (int i = 0; i < this->numberOfIntColumns; i++)
			{
				this->intColumns[i] = t.intColumns[i];
			}
		}

		//FLOAT
		if (t.numberOfFloatColumns <= 0)
		{
			this->numberOfFloatColumns = 0;
			this->floatColumns = NULL;
		}
		else
		{
			delete[] this->floatColumns;
			this->numberOfFloatColumns = t.numberOfFloatColumns;
			this->floatColumns = new FLOAT_COLUMN[this->numberOfFloatColumns];
			for (int i = 0; i < this->numberOfFloatColumns; i++)
			{
				this->floatColumns[i] = t.floatColumns[i];
			}
		}
	}

#pragma endregion

#pragma region Methods
	void removeEntry(int entryIndex)
	{
		for (int i = 0; i < this->numberOfTextColumns; i++)
		{
			this->textColumns[i] -= entryIndex;
		}
		for (int i = 0; i < this->numberOfIntColumns; i++)
		{
			this->intColumns[i] -= entryIndex;
		}
		for (int i = 0; i < this->numberOfFloatColumns; i++)
		{
			this->floatColumns[i] -= entryIndex;
		}
	}

	void showEntry(int entryIndex)
	{
		std::cout << "\n-----------------------------";
		this->textColumns->showEntryAtIndex(entryIndex);
		this->intColumns->showEntryAtIndex(entryIndex);
		this->floatColumns->showEntryAtIndex(entryIndex);
	}

	void generateEntryReport(int entryIndex, string filename)
	{
		ofstream file(filename, ios::out);
		file << "\n-----------------------------";
		file.close();
		this->textColumns->generateReportEntryAtIndex(entryIndex, filename);
		this->intColumns->generateReportEntryAtIndex(entryIndex, filename);
		this->floatColumns->generateReportEntryAtIndex(entryIndex, filename);
	}

	void showColumns(vector<string> columnNames)
	{
		for (int i = 0; i < columnNames.size(); i++)
		{
			for (int j = 0; j < this->numberOfTextColumns; j++)
			{
				if (this->textColumns[j].getColumnName() == columnNames[i])
				{
					std::cout << textColumns[j];
				}
			}

			for (int j = 0; j < this->numberOfIntColumns; j++)
			{
				if (this->intColumns[j].getColumnName() == columnNames[i])
				{
					std::cout << intColumns[j];
				}
			}

			for (int j = 0; j < this->numberOfFloatColumns; j++)
			{
				if (this->floatColumns[j].getColumnName() == columnNames[i])
				{
					std::cout << floatColumns[j];
				}
			}
		}
	}

	void showColumnsAtIndex(vector<string> columnNames, int entryIndex, string filename)
	{
		ofstream file(filename, ios::out);
		std::cout << "\n----------------------------------";
		file.close();
		for (int i = 0; i < columnNames.size(); i++)
		{
			for (int j = 0; j < this->numberOfTextColumns; j++)
			{
				if (this->textColumns[j].getColumnName() == columnNames[i])
				{
					textColumns[j].showEntryAtIndex(entryIndex);
					textColumns[j].generateReportEntryAtIndex(entryIndex, filename);
				}
			}

			for (int j = 0; j < this->numberOfIntColumns; j++)
			{
				if (this->intColumns[j].getColumnName() == columnNames[i])
				{
					intColumns[j].showEntryAtIndex(entryIndex);
					intColumns[j].generateReportEntryAtIndex(entryIndex, filename);
				}
			}

			for (int j = 0; j < this->numberOfFloatColumns; j++)
			{
				if (this->floatColumns[j].getColumnName() == columnNames[i])
				{
					floatColumns[j].showEntryAtIndex(entryIndex);
					floatColumns[j].generateReportEntryAtIndex(entryIndex, filename);
				}
			}
		}
	}

	bool checkColumns(vector<string> columnNames)
	{
		bool found;
		for (int i = 0; i < columnNames.size(); i++)
		{
			found = false;
			for (int j = 0; j < this->numberOfTextColumns; j++)
			{
				if (this->textColumns[j].getColumnName() == columnNames[i])
				{
					found = true;
				}
			}
			for (int j = 0; j < this->numberOfIntColumns; j++)
			{
				if (this->intColumns[j].getColumnName() == columnNames[i])
				{
					found = true;
				}
			}
			for (int j = 0; j < this->numberOfFloatColumns; j++)
			{
				if (this->floatColumns[j].getColumnName() == columnNames[i])
				{
					found = true;
				}
			}

			if (found == false)
			{
				return false;
			}
		}

		return true;
	}

	COLUMN_TYPES findColumn(string columnName, int& index)
	{
		for (int i = 0; i < this->numberOfTextColumns; i++)
		{
			if (this->textColumns[i].getColumnName() == columnName)
			{
				index = i;
				return COLUMN_TYPES::TEXT;
			}
		}
		for (int i = 0; i < this->numberOfIntColumns; i++)
		{
			if (this->intColumns[i].getColumnName() == columnName)
			{
				index = i;
				return COLUMN_TYPES::INTEGER;
			}
		}
		for (int i = 0; i < this->numberOfFloatColumns; i++)
		{
			if (this->floatColumns[i].getColumnName() == columnName)
			{
				index = i;
				return COLUMN_TYPES::FLOAT;
			}
		}

		index = -1;
		return COLUMN_TYPES::UNKNOWN;
	}


	void saveTable()
	{
		//config primeste numele tabelei, 
		//numberOfTextColumns, TextColumns
		//numberOfIntColumns, IntColumns, 
		//numberOfFloatColumns, FloatColumns,

		ofstream configFile;

		string filename = this->tableName + txtExtension;
		configFile.open(filename, ios::out);

		configFile << this->tableName << "\n";
		configFile << this->numberOfTextColumns << "\n";
		configFile.close();
		for (int i = 0; i < this->numberOfTextColumns; i++)
		{
			try
			{
				this->textColumns[i].saveTextColumn(filename);
			}
			catch (exception exc)
			{
				cout << exc.what();
			}
		}

		configFile.open(filename, ios::app);
		if (configFile.is_open())
		{
			configFile << this->numberOfIntColumns << "\n";
			configFile.close();
			for (int i = 0; i < this->numberOfIntColumns; i++)
			{
				try
				{
					this->intColumns[i].saveIntColumn(filename);
				}
				catch (exception exc)
				{
					cout << exc.what();
				}
			}
		}
		else
		{
			throw exception("\nNu exista fisierul!");
		}

		configFile.open(filename, ios::app);
		if (configFile.is_open())
		{
			configFile << this->numberOfFloatColumns << "\n";
			configFile.close();
			for (int i = 0; i < this->numberOfFloatColumns; i++)
			{
				try
				{
					this->floatColumns[i].saveFloatColumn(filename);
				}
				catch (exception exc)
				{
					cout << exc.what();
				}
			}
		}
		else
		{
			throw exception("\nNu exista fisierul!");
		}
	}

	void loadTable(string filename)
	{
		ifstream configFile(filename, ios::in);
		if (configFile.is_open())
		{
			string columnName;
			int columnType;
			int columnSize;
			int numberOfValues;

			configFile >> this->tableName;
			configFile >> this->numberOfTextColumns;
			this->textColumns = new TEXT_COLUMN[this->numberOfTextColumns];
			for (int i = 0; i < this->numberOfTextColumns; i++)
			{
				string defaultValue;
				string* textValues;

				configFile >> columnName;
				configFile >> columnType;
				configFile >> columnSize;
				configFile >> defaultValue;
				configFile >> numberOfValues;

				textValues = new string[numberOfValues];
				for (int j = 0; j < numberOfValues; j++)
				{
					configFile >> textValues[j];
				}
				TEXT_COLUMN txtCol(columnName, COLUMN_TYPES::TEXT, columnSize, defaultValue, numberOfValues, textValues);
				textColumns[i] = txtCol;

				delete[] textValues;
			}

			configFile >> this->numberOfIntColumns;
			this->intColumns = new INT_COLUMN[this->numberOfIntColumns];
			for (int i = 0; i < this->numberOfIntColumns; i++)
			{
				int defaultValue;
				int* intValues;

				configFile >> columnName;
				configFile >> columnType;
				configFile >> columnSize;
				configFile >> defaultValue;
				configFile >> numberOfValues;

				intValues = new int[numberOfValues];
				for (int j = 0; j < numberOfValues; j++)
				{
					configFile >> intValues[j];
				}
				INT_COLUMN intCol(columnName, COLUMN_TYPES::INTEGER, columnSize, defaultValue, numberOfValues, intValues);
				intColumns[i] = intCol;

				delete[] intValues;
			}

			configFile >> this->numberOfFloatColumns;
			this->floatColumns = new FLOAT_COLUMN[this->numberOfFloatColumns];
			for (int i = 0; i < this->numberOfFloatColumns; i++)
			{
				float defaultValue;
				float* floatValues;

				configFile >> columnName;
				configFile >> columnType;
				configFile >> columnSize;
				configFile >> defaultValue;
				configFile >> numberOfValues;

				floatValues = new float[numberOfValues];
				for (int j = 0; j < numberOfValues; j++)
				{
					configFile >> floatValues[j];
				}
				FLOAT_COLUMN floatCol(columnName, COLUMN_TYPES::INTEGER, columnSize, defaultValue, numberOfValues, floatValues);
				floatColumns[i] = floatCol;

				delete[] floatValues;
			}
			//de intrebat daca exista posibilitatea citirii aceluiasi fisier pe bucati
			configFile.close();
		}
		else
		{
			cout << "\nNu exista fisierul config";
		}
	}
#pragma endregion

#pragma region Getters and Setters
	int getNumberOfColumns()
	{
		return numberOfTextColumns + numberOfIntColumns + numberOfFloatColumns;
	}

	string getTableName()
	{
		return this->tableName;
	}

	int getNumberOfTextColumns()
	{
		return this->numberOfTextColumns;
	}

	int getNumberOfIntColumns()
	{
		return this->numberOfIntColumns;
	}

	int getNumberOfFloatColumns()
	{
		return this->numberOfFloatColumns;
	}

	TEXT_COLUMN getTextColumn(int index)
	{
		if (index > this->numberOfTextColumns)
		{
			throw exception("\nERROR! Index invalid");
		}

		return this->textColumns[index];
	}

	INT_COLUMN getIntColumn(int index)
	{
		if (index > this->numberOfIntColumns)
		{
			throw exception("\nERROR! Index invalid");
		}

		return this->intColumns[index];
	}

	FLOAT_COLUMN getFloatColumn(int index)
	{
		if (index > this->numberOfFloatColumns)
		{
			throw exception("\nERROR! Index invalid");
		}

		return this->floatColumns[index];
	}

	void setTableName(string tableName)
	{
		if (tableName.length() > 0)
		{
			this->tableName = tableName;
		}
		else
		{
			throw exception("\nERROR! Invalid tableName");
		}
	}

	void setTextColumn(int index, TEXT_COLUMN txtCol)
	{
		if (index > this->numberOfTextColumns)
		{
			throw exception("\nERROR! Index invalid");
		}

		this->textColumns[index] = txtCol;
	}

	void setIntColumn(int index, INT_COLUMN intCol)
	{
		if (index > this->numberOfIntColumns)
		{
			throw exception("\nERROR! Index invalid");
		}

		this->intColumns[index] = intCol;
	}

	void setFloatColumn(int index, FLOAT_COLUMN floatCol)
	{
		if (index > this->numberOfFloatColumns)
		{
			throw exception("\nERROR! Index invalid");
		}

		this->floatColumns[index] = floatCol;
	}

#pragma endregion

#pragma region Operators
	TABLE& operator= (const TABLE t)
	{
		if (this != &t)
		{
			this->tableName = t.tableName;

			//TEXT
			if (t.numberOfTextColumns <= 0)
			{
				this->numberOfTextColumns = 0;
				this->textColumns = NULL;
			}
			else
			{
				delete[] this->textColumns;
				this->numberOfTextColumns = t.numberOfTextColumns;
				this->textColumns = new TEXT_COLUMN[this->numberOfTextColumns];
				for (int i = 0; i < this->numberOfTextColumns; i++)
				{
					this->textColumns[i] = t.textColumns[i];
				}
			}

			//INT
			if (t.numberOfIntColumns <= 0)
			{
				this->numberOfIntColumns = 0;
				this->intColumns = NULL;
			}
			else
			{
				delete[] this->intColumns;
				this->numberOfIntColumns = t.numberOfIntColumns;
				this->intColumns = new INT_COLUMN[this->numberOfIntColumns];
				for (int i = 0; i < this->numberOfIntColumns; i++)
				{
					this->intColumns[i] = t.intColumns[i];
				}
			}

			//FLOAT
			if (t.numberOfFloatColumns <= 0)
			{
				this->numberOfFloatColumns = 0;
				this->floatColumns = NULL;
			}
			else
			{
				delete[] this->floatColumns;
				this->numberOfFloatColumns = t.numberOfFloatColumns;
				this->floatColumns = new FLOAT_COLUMN[this->numberOfFloatColumns];
				for (int i = 0; i < this->numberOfFloatColumns; i++)
				{
					this->floatColumns[i] = t.floatColumns[i];
				}
			}
		}

		return *this;
	}

	TABLE operator+= (TEXT_COLUMN column)
	{
		TABLE copy = *this;

		delete[] this->textColumns;
		this->numberOfTextColumns++;
		this->textColumns = new TEXT_COLUMN[this->numberOfTextColumns];
		if (this->numberOfTextColumns > 1)
		{
			for (int i = 0; i < this->numberOfTextColumns; i++)
			{
				this->textColumns[i] = copy.textColumns[i];
			}
		}

		this->textColumns[this->numberOfTextColumns - 1] = column;

		return *this;
	}

	TABLE operator+= (INT_COLUMN column)
	{
		TABLE copy = *this;

		delete[] this->intColumns;
		this->numberOfIntColumns++;
		this->intColumns = new INT_COLUMN[this->numberOfIntColumns];
		if (this->numberOfIntColumns > 1)
		{
			for (int i = 0; i < this->numberOfIntColumns; i++)
			{
				this->intColumns[i] = copy.intColumns[i];
			}
		}

		this->intColumns[this->numberOfIntColumns - 1] = column;

		return *this;
	}

	TABLE operator+= (FLOAT_COLUMN column)
	{
		TABLE copy = *this;

		delete[] this->floatColumns;
		this->numberOfFloatColumns++;
		this->floatColumns = new FLOAT_COLUMN[this->numberOfFloatColumns];
		if (this->numberOfFloatColumns > 1)
		{
			for (int i = 0; i < this->numberOfFloatColumns; i++)
			{
				this->floatColumns[i] = copy.floatColumns[i];
			}
		}

		this->floatColumns[this->numberOfFloatColumns - 1] = column;

		return *this;
	}

	TABLE operator+ (TEXT_COLUMN column)
	{
		TABLE res = *this;
		res += column;
		return res;
	}

	TABLE operator+ (INT_COLUMN column)
	{
		TABLE res = *this;
		res += column;
		return res;
	}

	TABLE operator+ (FLOAT_COLUMN column)
	{
		TABLE res = *this;
		res += column;
		return res;
	}

#pragma endregion

	~TABLE()
	{
		delete[] this->textColumns;
		delete[] this->intColumns;
		delete[] this->floatColumns;
	}

#pragma region Friend Methods
	friend ostream& operator<< (ostream& out, const TABLE& t)
	{
		out << "\n\n----------------------";
		out << "\nNume Tabela: " << t.tableName;
		out << "\n----------------------\n";
		out << "\nLista Coloane text: \n";
		for (int i = 0; i < t.numberOfTextColumns; i++)
		{
			out << "\nColoana Text " << i << ":";
			out << t.textColumns[i];
		}
		out << "\n----------------------\n";
		out << "\nLista Coloane int: \n";
		for (int i = 0; i < t.numberOfIntColumns; i++)
		{
			out << "\nColoana int " << i << ":";
			out << t.intColumns[i];
		}
		out << "\n----------------------\n";
		out << "\nLista Coloane float: \n";
		for (int i = 0; i < t.numberOfFloatColumns; i++)
		{
			out << "\nColoana float " << i << ":";
			out << t.floatColumns[i];
		}

		return out;
	}

#pragma endregion
};

class DATABASE {
	const int id;
	string databaseName = "ProjectDB";
	static int numberOfTables;
	TABLE* tables = NULL;
public:
#pragma region Constructors
	DATABASE(int id) :id(id)
	{

	}

	DATABASE(int id, string databaseName) :id(id)
	{
		this->databaseName = databaseName;
	}

	DATABASE(int id, string databaseName, TABLE* tables) : id(id)
	{
		this->databaseName = databaseName;
		this->tables = new TABLE[DATABASE::numberOfTables];
		for (int i = 0; i < DATABASE::numberOfTables; i++)
		{
			this->tables[i] = tables[i];
		}
	}

	DATABASE(const DATABASE& db) :id(db.id)
	{
		this->databaseName = db.databaseName;
		if (db.numberOfTables < 0)
		{
			DATABASE::numberOfTables = 0;
			this->tables = NULL;
		}
		else
		{
			delete[] this->tables;
			DATABASE::numberOfTables = db.numberOfTables;
			this->tables = new TABLE[DATABASE::numberOfTables];
			for (int i = 0; i < DATABASE::numberOfTables; i++)
			{
				this->tables[i] = db.tables[i];
			}
		}
	}
#pragma endregion

#pragma region Methods
#pragma endregion

#pragma region Getters and Setters

	string getDatabaseName()
	{
		return this->databaseName;
	}

	static int getNumberOfTables()
	{
		return DATABASE::numberOfTables;
	}

	//security
	TABLE* getTables()
	{
		TABLE* copy = tables;
		return copy;
	}


	void setDatabaseName(string databaseName)
	{
		if (databaseName.length() > 0)
		{
			this->databaseName = databaseName;
		}
		else
		{
			throw exception("\nERROR! Invalid databaseName");
		}
	}

	static void setNumberOfTables(int numberOfTables)
	{
		if (numberOfTables >= 0)
		{
			DATABASE::numberOfTables = numberOfTables;
		}
		else
		{
			throw exception("\nERROR! Invalid numberOfTables");
		}
	}

	void setTable(string tableName, TABLE t)
	{
		int index = -1;
		for (int i = 0; i < this->numberOfTables; i++)
		{
			if (this->tables[i].getTableName() == tableName)
			{
				index = i;
			}
		}

		if (index == -1)
		{
			throw exception("\nERROR! Tabela cautata nu exista");
		}
		else
		{
			this->tables[index] = t;
		}
	}

	void saveDb()
	{
		ofstream configFile;
		configFile.open("db.txt", ios::out);
		configFile << this->databaseName << "\n";
		configFile << DATABASE::numberOfTables << "\n";
		for (int i = 0; i < DATABASE::numberOfTables; i++)
		{
			configFile << this->tables[i].getTableName() << "\n";
			this->tables[i].saveTable();
		}

		configFile.close();
		//config file pentru db care contine
		//nume, numarTabele si primeste si numele tabelelor
	}

	void loadDb()
	{
		ifstream configFile;
		configFile.open("db.txt", ios::in);
		if (configFile.is_open())
		{
			string tableName;
			configFile >> this->databaseName;
			configFile >> DATABASE::numberOfTables;
			this->tables = new TABLE[DATABASE::numberOfTables];
			for (int i = 0; i < DATABASE::numberOfTables; i++)
			{
				configFile >> tableName;
				string tableFilename = tableName + txtExtension;
				TABLE t;
				t.loadTable(tableFilename);
				tables[i] = t;
				//load each table into db
			}
		}
		else
		{
			//nu am bagat cu exceptie deoarece programul ar trebui sa ruleze
			//chiar si daca nu exista un fisier in prealabil (cazul: new database)
			cout << ("Nu exista fisierul config!");
		}
	}
#pragma endregion

#pragma region Operators
	DATABASE& operator= (const DATABASE& db)
	{
		if (this != &db)
		{
			this->databaseName = db.databaseName;
			if (db.numberOfTables < 0)
			{
				DATABASE::numberOfTables = 0;
				this->tables = NULL;
			}
			else
			{
				delete[] this->tables;
				DATABASE::numberOfTables = db.numberOfTables;
				this->tables = new TABLE[DATABASE::numberOfTables];
				for (int i = 0; i < DATABASE::numberOfTables; i++)
				{
					this->tables[i] = db.tables[i];
				}
			}
		}

		return *this;
	}

	DATABASE operator+= (TABLE table)
	{
		DATABASE copy = *this;
		delete[] this->tables;
		DATABASE::numberOfTables++;
		this->tables = new(nothrow) TABLE[DATABASE::numberOfTables];
		if (DATABASE::numberOfTables > 1)
		{
			for (int i = 0; i < DATABASE::numberOfTables - 1; i++)
			{
				this->tables[i] = copy.tables[i];
			}
		}

		this->tables[DATABASE::numberOfTables - 1] = table;

		return *this;
	}

	DATABASE operator+ (TABLE table)
	{
		DATABASE res = *this;
		res += table;
		return res;
	}

	DATABASE operator-= (string tableName)
	{
		bool found = false;
		for (int i = 0; i < this->numberOfTables; i++)
		{
			if (this->tables[i].getTableName() == tableName)
			{
				found = true;
			}
		}

		if (found)
		{
			DATABASE copy = *this;
			delete[] this->tables;
			this->numberOfTables--;
			this->tables = new TABLE[this->numberOfTables];
			for (int i = 0; i < copy.numberOfTables; i++)
			{
				if (copy.tables[i].getTableName() != tableName)
				{
					this->tables[i] = copy.tables[i];
				}
			}
		}
		else
		{
			throw exception("\nERROR! Tabela ceruta nu exista");
		}

		return *this;
	}

	DATABASE operator- (string tableName)
	{
		DATABASE res = *this;
		res -= tableName;
		return res;
	}

	TABLE& operator[] (string tableName)
	{
		for (int i = 0; i < this->numberOfTables; i++)
		{
			if (this->tables[i].getTableName() == tableName)
			{
				return this->tables[i];
			}
		}

		throw exception("\nEROARE! Nu exista tabela cautata");
	}

#pragma endregion

	~DATABASE()
	{
		delete[] this->tables;
	}

#pragma region Friend Methods
	friend ostream& operator<< (ostream& out, const DATABASE db)
	{
		out << "\n\n----------------------";
		out << "\nNume DB: " << db.databaseName;
		out << "\nLista Tabele: \n";
		for (int i = 0; i < db.numberOfTables; i++)
		{
			out << "\nTabela " << i << ":";
			out << db.tables[i];
		}

		return out;
	}
#pragma endregion
};

class CommandChecker
{
	string command;

public:
#pragma region Constructors
	CommandChecker()
	{

	}

	CommandChecker(string command)
	{
		this->command = command;
	}
#pragma endregion

#pragma region Methods
	COMMAND_TYPES checkCommand()
	{
		int position;
		position = this->command.find("CREATE TABLE");
		if (position != -1)
		{
			return COMMAND_TYPES::CREATE;
		}
		position = this->command.find("DROP TABLE");
		if (position != -1)
		{
			return COMMAND_TYPES::DROP;
		}
		position = this->command.find("DISPLAY TABLE");
		if (position != -1)
		{
			return COMMAND_TYPES::DISPLAY;
		}
		position = this->command.find("INSERT INTO");
		if (position != -1)
		{
			return COMMAND_TYPES::INSERT;
		}
		position = this->command.find("DELETE FROM");
		if (position != -1)
		{
			return COMMAND_TYPES::DELETE;
		}
		position = this->command.find("SELECT");
		if (position != -1)
		{
			return COMMAND_TYPES::SELECT;
		}
		position = this->command.find("UPDATE");
		if (position != -1)
		{
			return COMMAND_TYPES::UPDATE;
		}
		position = this->command.find("IMPORT");
		if (position != -1)
		{
			return COMMAND_TYPES::IMPORT;
		}

		return COMMAND_TYPES::UNDEFINED;
	}

#pragma endregion

#pragma region Getters and Setters
	string getCommand()
	{
		return this->command;
	}

	void setCommand(string command)
	{
		if (command.length() > 0)
		{
			this->command = command;
		}
	}
#pragma endregion

#pragma region Operators
#pragma endregion


	~CommandChecker()
	{

	}

#pragma region friend Methods
#pragma endregion
};

vector<string> stringTokenizer(string line, char delimiter)
{
	vector<string> tokens;
	stringstream ss(line);
	string intermediate;

	while (getline(ss, intermediate, delimiter))
	{
		tokens.push_back(intermediate);
	}

	//for (int i = 0; i < tokens.size(); i++)
	//{
	//	std::cout << "Token[" << i << "]: " << tokens[i] << "\n";
	//}

	return tokens;
}

COLUMN_TYPES checkColumnType(string columnType)
{
	int position;
	position = columnType.find("TEXT");
	if (position != -1)
	{
		return COLUMN_TYPES::TEXT;
	}
	position = columnType.find("INTEGER");
	if (position != -1)
	{
		return COLUMN_TYPES::INTEGER;
	}
	position = columnType.find("FLOAT");
	if (position != -1)
	{
		return COLUMN_TYPES::FLOAT;
	}

	return COLUMN_TYPES::UNKNOWN;
}

void InterpretCommand(string command, DATABASE& db)
{
	CommandChecker checker(command);
	vector<string> commandWords;
	vector<string> columnData;
	vector<string> columnAttributes;

	switch (checker.checkCommand())
	{
		//DATABASE_COMMANDS
	case 1: //CREATE TABLE
	{
		string tableName;
		string columnName;
		COLUMN_TYPES columnType;
		int columnSize;

		int conditionPosition;
		bool conditionExists;

		//TODO: IMPLEMENTAT VERIFICAREA DE FISIERE EXISTENTE DEJA
		conditionPosition = command.find("IF NOT EXISTS");
		if (conditionPosition != -1)
		{
			conditionExists = true;
		}
		else
		{
			conditionExists = false;
		}

		commandWords = stringTokenizer(command, ' ');
		tableName = commandWords[2];
		TABLE t(tableName);
		columnData = stringTokenizer(command, '('); //imi returneaza cate coloane sunt si dupa pot delimita la , fiecare entry
		//datele despre coloane incep de la index 2 pana la columnData.size()
		for (int i = 2; i < columnData.size(); i++)
		{
			//stringTokenizer(columnData[i], ')'); o sa ne returneze pe [0] un string cu toate atributele unei coloane
			columnAttributes = stringTokenizer(stringTokenizer(columnData[i], ')')[0], ',');
			columnName = columnAttributes[0];
			columnType = checkColumnType(columnAttributes[1]);
			columnSize = stoi(columnAttributes[2]);
			switch (columnType)
			{
			case 1: //TEXT
			{
				string defaultValue = columnAttributes[3];
				TEXT_COLUMN txtCol(columnName, columnType, columnSize, defaultValue);
				//std::cout << "\n\nTEXT COLUMN:" << txtCol;
				t += txtCol;
				break;
			}
			case 2: //INTEGER
			{
				int defaultValue = stoi(columnAttributes[3]);
				INT_COLUMN intCol(columnName, columnType, columnSize, defaultValue);
				//std::cout << "\n\nINT COLUMN:" << intCol;
				t += intCol;
				break;
			}
			case 3: //FLOAT
			{
				float defaultValue = stof(columnAttributes[3]);
				FLOAT_COLUMN floatCol(columnName, columnType, columnSize, defaultValue);
				//std::cout << "\n\nFLOAT COLUMN:" << floatCol;
				t += floatCol;
				break;
			}
			}
		}
		db += t;
		std::cout << "\n\nTABELA CU NUMELE " << tableName << " A FOST CREATA CU SUCCES!\n";

		break;
	}
	case 2: //DROP TABLE
	{
		//cautat tabela cu numele cerut, daca exista, stergem toata tabela, altfel, nu facem nimic
		string tableName;
		commandWords = stringTokenizer(command, ' ');
		tableName = commandWords[2];
		try
		{
			db -= tableName;
			std::cout << "\n\nTABELA CU NUMELE " << tableName << " A FOST STEARSA CU SUCCES!\n";
		}
		catch (exception exc)
		{
			std::cout << exc.what();
		}

		break;
	}
	case 3: //DISPLAY TABLE
	{
		//cautat tabela cu numele cerut, daca exista, afiseaza, altfel nu facem nimic
		string tableName;
		commandWords = stringTokenizer(command, ' ');
		if (commandWords.size() > 2)
		{
			tableName = commandWords[2];
		}
		else
		{
			throw exception("!!!Sintaxa Gresita");
		}

		try
		{
			string filename = tableName + to_string(time(nullptr)) + txtExtension;
			ofstream reportFile(filename, ios::out);
			reportFile << db[tableName];

			std::cout << db[tableName];
			std::cout << "\n\nTABELA CU NUMELE " << tableName << " A FOST GASITA CU SUCCES!\n";
		}
		catch (exception exc)
		{
			std::cout << exc.what();
		}

		break;
	}
	//TABLE_COMMANDS
	case 11: //INSERT INTO
	{
		///TODO: ADAUGAT CAMPURI ARRAY numite VALUES LA COLOANELE DE TOATE TIPURILE
		//verificat daca exista VALUES
		//impartit dupa , valorile si verificat daca sunt la fel la numar ca si coloanele din tabel
		//luat la rand fiecare valoare si incercat conversie la int, float, iar daca nu merge niciuna, e string si dam assign la primul occurence cu un counter care creste
		vector<string> columnValues;
		TABLE t;
		string tableName;
		string filename;
		commandWords = stringTokenizer(command, ' ');
		tableName = commandWords[2];

		try
		{
			t = db[tableName];
			std::cout << "\n\nTABELA CU NUMELE " << tableName << " A FOST GASITA CU SUCCES!\n";
		}
		catch (exception exc)
		{
			std::cout << exc.what();
		}

		int valuesPosition;

		valuesPosition = command.find("VALUES");
		if (valuesPosition != -1)
		{
			//prima data scapam de paranteze si dupa impartim in cuvinte dupa ,
			columnValues = stringTokenizer(stringTokenizer(stringTokenizer(commandWords[3], '(')[1], ')')[0], ',');
			for (int i = 0; i < columnValues.size(); i++)
			{
				std::cout << "\n" << columnValues[i];
			}

			int textColumnCounter = 0;
			int intColumnCounter = 0;
			int floatColumnCounter = 0;

			if (t.getNumberOfColumns() != columnValues.size())
			{
				throw exception("Numarul valorilor nu corespunde cu numarul coloanelor din tabela");
			}
			else
			{
				///TODO: de gandit cum sa fie tabelele puse in ordine 
				///- asta nu e implementata inca si probabil trebuie regandit in phase 2 pentru ca acum are foarte multe probleme ce nu pot fi rezolvate fara restructurare
				for (int i = 0; i < columnValues.size(); i++)
				{
					//adaugat in fisier binar valorile
					filename = tableName + binExtension;
					ofstream dataFile(filename, ios::out | ios::app | ios::binary);
					if (dataFile.is_open())
					{
						dataFile.write((char*)&columnValues[i], sizeof(string));
						dataFile.close();
						if (!dataFile.good())
						{
							cout << "\nError occured at writing time";
						}
					}
					else
					{
						cout << "\nNu exista fisierul binar";
					}
					int dotPosition = columnValues[i].find('.');
					if (dotPosition != -1)
					{
						try
						{
							stof(columnValues[i]);
							FLOAT_COLUMN floatCol = t.getFloatColumn(floatColumnCounter);
							floatCol += stof(columnValues[i]);
							t.setFloatColumn(floatColumnCounter, floatCol);
							floatColumnCounter++;
							db.setTable(tableName, t);
						}
						catch (exception exc)
						{
							//std::cout<<exc.what();
							//inseamna ca e text => adaugam in coloana text
							TEXT_COLUMN txtCol = t.getTextColumn(textColumnCounter);
							txtCol += columnValues[i];
							t.setTextColumn(textColumnCounter, txtCol);
							textColumnCounter++;
							db.setTable(tableName, t);
						}
					}
					else
					{
						try
						{
							stoi(columnValues[i]);
							INT_COLUMN intCol = t.getIntColumn(intColumnCounter);
							intCol += stoi(columnValues[i]);
							t.setIntColumn(intColumnCounter, intCol);
							intColumnCounter++;
							db.setTable(tableName, t);
						}
						catch (exception exc)
						{
							//std::cout<<exc.what();
							//inseamna ca e text => adaugam in coloana text
							TEXT_COLUMN txtCol = t.getTextColumn(textColumnCounter);
							txtCol += columnValues[i];
							t.setTextColumn(textColumnCounter, txtCol);
							textColumnCounter++;
							db.setTable(tableName, t);
						}
					}
				}
			}

		}
		else
		{
			throw exception("Sintaxa gresita!");
		}

		break;
	}
	case 12: //DELETE FROM
	{
		//verificare table[numeColoana] => imi returneaza coloana => coloana[value] imi returneaza indexul inregistrarii, daca nu exista -1, sterg inregistrarea la index
		TABLE t;
		string tableName;
		commandWords = stringTokenizer(command, ' ');

		for (int i = 0; i < commandWords.size(); i++)
		{
			std::cout << "\n" << commandWords[i];
		}

		tableName = commandWords[2];
		try
		{
			t = db[tableName];
			std::cout << "\n\nTABELA CU NUMELE " << tableName << " A FOST GASITA CU SUCCES!\n";
		}
		catch (exception exc)
		{
			std::cout << exc.what();
		}

		int wherePosition;

		wherePosition = command.find("WHERE");
		if (wherePosition != -1)
		{
			string columnName = commandWords[4];
			string value = commandWords[6];
			COLUMN_TYPES type;
			int columnIndex;

			//trebuie o functie sa caute dupa columnName, sa imi returneze tipulColoanei si sa imi intoarca ca parametru indexul
			type = t.findColumn(columnName, columnIndex);
			switch (type)
			{
			case 1: //TEXT
			{
				try
				{
					TEXT_COLUMN txtCol = t.getTextColumn(columnIndex);
					int entryIndex = txtCol.getIndex(value);
					//trebuie sa iau toate coloanele cu index
					//si dupa sa sterg values de la entryIndex din toate coloanele
					t.removeEntry(entryIndex);
					db.setTable(tableName, t);
					std::cout << "\nA fost stearsa inregistrarea cautata!";
				}
				catch (exception exc)
				{
					std::cout << exc.what();
				}
				break;
			}
			case 2: //INT
			{
				try
				{
					INT_COLUMN intCol = t.getIntColumn(columnIndex);
					int entryIndex = intCol.getIndex(stoi(value));
					t.removeEntry(entryIndex);
					db.setTable(tableName, t);
					std::cout << "\nA fost stearsa inregistrarea cautata!";
				}
				catch (exception exc)
				{
					std::cout << exc.what();
				}
				break;
			}
			case 3: //FLOAT
			{
				try
				{
					FLOAT_COLUMN floatCol = t.getFloatColumn(columnIndex);
					int entryIndex = floatCol.getIndex(stof(value));
					t.removeEntry(entryIndex);
					db.setTable(tableName, t);
					std::cout << "\nA fost stearsa inregistrarea cautata!";
				}
				catch (exception exc)
				{
					std::cout << exc.what();
				}
				break;
			}

			case -1: //UNKNOWN
				throw exception("ERROR! Nu exista coloana cautata");
				break;
			default:
				throw exception("ERROR! Nu exista coloana cautata");
				break;
			}
		}
		else
		{
			throw exception("ERROR! Sintaxa gresita");
		}
		db.saveDb();
		db.loadDb();
		break;
	}
	case 13: //SELECT
	{
		//AICI MAI E DE GANDIT PUTIN
		//verific daca avem coloane, daca avem, identificam coloanele si le impartim intr-un vector<string> => cautam tabel[coloana[i]] => ne returneaza coloana? vad maine
		//daca avem cu ALL, display la tabela
		//daca avem si WHERE facem ca la delete verificarea doar ca nu stergem, ci doar verificam

		TABLE t;
		string tableName;
		commandWords = stringTokenizer(command, ' ');

		for (int i = 0; i < commandWords.size(); i++)
		{
			std::cout << "\n[" << i << "]: " << commandWords[i];
		}

		tableName = commandWords[3];
		string filename = tableName + to_string(time(nullptr)) + txtExtension;

		try
		{
			t = db[tableName];
			std::cout << "\n\nTABELA CU NUMELE " << tableName << " A FOST GASITA CU SUCCES!\n";
		}
		catch (exception exc)
		{
			std::cout << exc.what();
		}

		if (commandWords[1] == "ALL")
		{
			if (commandWords[2] == "FROM")
			{
				//daca nu avem conditie where, doar std::cout la tabela
				//luam toate coloanele si le afisam
				//daca avem conditie verificam conditia si luam entryIndex

				int wherePosition;

				wherePosition = command.find("WHERE");
				if (wherePosition != -1)
				{
					//avem where
					//index columnName = 5
					//index value = 7
					string columnName = commandWords[5];
					string value = commandWords[7];
					COLUMN_TYPES type;
					int columnIndex;

					type = t.findColumn(columnName, columnIndex);

					switch (type)
					{
					case 1: //TEXT
					{
						try
						{
							TEXT_COLUMN txtCol = t.getTextColumn(columnIndex);
							int entryIndex = txtCol.getIndex(value);
							t.showEntry(entryIndex);
							t.generateEntryReport(entryIndex, filename);
							std::cout << "\nA fost afisata inregistrarea cautata!";
						}
						catch (exception exc)
						{
							std::cout << exc.what();
						}
						break;
					}
					case 2: //INT
					{
						try
						{
							INT_COLUMN intCol = t.getIntColumn(columnIndex);
							int entryIndex = intCol.getIndex(stoi(value));
							t.showEntry(entryIndex);
							t.generateEntryReport(entryIndex, filename);
							std::cout << "\nA fost afisata inregistrarea cautata!";
						}
						catch (exception exc)
						{
							std::cout << exc.what();
						}
						break;
					}
					case 3: //FLOAT
					{
						try
						{
							FLOAT_COLUMN floatCol = t.getFloatColumn(columnIndex);
							int entryIndex = floatCol.getIndex(stof(value));
							t.showEntry(entryIndex);
							t.generateEntryReport(entryIndex, filename);
							std::cout << "\nA fost afisata inregistrarea cautata!";
						}
						catch (exception exc)
						{
							std::cout << exc.what();
						}
						break;
					}

					case -1: //UNKNOWN
						throw exception("ERROR! Nu exista coloana cautata");
						break;
					default:
						throw exception("ERROR! Nu exista coloana cautata");
						break;
					}
				}
				else
				{
					//nu avem where
					ofstream file(filename, ios::out);
					file << t;
					std::cout << t;
				}
			}
			else
			{
				throw exception("ERROR! Sintaxa invalida");
			}
		}
		else
		{
			//verificam coloanele pe rand daca exista in tabela
			//verificam daca avem conditia si luam entryIndex

			//impartim coloanele
			columnData = stringTokenizer(command, '(');

			cout << "\n!!!!!!!!!!!!! AJUNGE AICI\n";
			columnAttributes = stringTokenizer(stringTokenizer(columnData[1], ')')[0], ',');
			for (int i = 0; i < columnAttributes.size(); i++)
			{
				std::cout << columnAttributes[i] << endl;
			}

			if (t.checkColumns(columnAttributes))
			{
				//exista toate coloanele

				//si dupa afisam doar coloanele cerute

				//verificam where
				int wherePosition;

				wherePosition = command.find("WHERE");
				if (wherePosition != -1)
				{
					//avem where
					//index columnName = 5
					//index value = 7
					string columnName = commandWords[5];
					string value = commandWords[7];
					COLUMN_TYPES type;
					int columnIndex;

					type = t.findColumn(columnName, columnIndex);

					switch (type)
					{
					case 1: //TEXT
					{
						try
						{
							TEXT_COLUMN txtCol = t.getTextColumn(columnIndex);
							int entryIndex = txtCol.getIndex(value);
							t.showColumnsAtIndex(columnAttributes, entryIndex, filename);

							std::cout << "\nA fost afisata inregistrarea cautata!";
						}
						catch (exception exc)
						{
							std::cout << exc.what();
						}
						break;
					}
					case 2: //INT
					{
						try
						{
							INT_COLUMN intCol = t.getIntColumn(columnIndex);
							int entryIndex = intCol.getIndex(stoi(value));
							t.showColumnsAtIndex(columnAttributes, entryIndex, filename);
							std::cout << "\nA fost afisata inregistrarea cautata!";
						}
						catch (exception exc)
						{
							std::cout << exc.what();
						}
						break;
					}
					case 3: //FLOAT
					{
						try
						{
							FLOAT_COLUMN floatCol = t.getFloatColumn(columnIndex);
							int entryIndex = floatCol.getIndex(stof(value));
							t.showColumnsAtIndex(columnAttributes, entryIndex, filename);
							std::cout << "\nA fost afisata inregistrarea cautata!";
						}
						catch (exception exc)
						{
							std::cout << exc.what();
						}
						break;
					}

					case -1: //UNKNOWN
						throw exception("ERROR! Nu exista coloana cautata");
						break;
					default:
						throw exception("ERROR! Nu exista coloana cautata");
						break;
					}
				}
				else
				{
					t.showColumns(columnAttributes);
				}
			}
			else
			{
				throw exception("ERROR! Nu exista toate coloanele specificate");
			}
			///TODO: formatare la afisare ca sa se inteleaga coloanele
		}

		break;
	}
	case 14: //UPDATE --DE AICI IN JOS E ORIBIL DAR AM VRUT SA FUNCTIONEZE MACAR
	{
		//tableName index 1
		//SET index 2
		//coloana de modificat index 3
		//valoare de modificat index 5
		//WHERE index 6
		//coloana de cautat index 7
		//valoare de cautat index 9

		TABLE t;
		string tableName;
		commandWords = stringTokenizer(command, ' ');

		//for (int i = 0; i < commandWords.size(); i++)
		//{
		//	std::cout << "\n[" << i << "]: " << commandWords[i];
		//}

		tableName = commandWords[1];
		try
		{
			t = db[tableName];
			std::cout << "\n\nTABELA CU NUMELE " << tableName << " A FOST GASITA CU SUCCES!\n";
		}
		catch (exception exc)
		{
			std::cout << exc.what();
		}

		//verific daca exista set
		//verific daca exista where
		//cautam in tabela[numeColoana] => coloana[valoare] => modificam valoarea ceruta
		if (commandWords[2] == "SET" && commandWords[6] == "WHERE")
		{
			//avem sintaxa corecta

			//verificat daca exista coloana de setat
			//daca exista coloana
				//verificam where conditie
					//daca a gasit
						//set columnValue la index gasit to new Value
					//else
						//exceptie
				//else
					//exceptie
			//else
				//exceptie


			//-----------DE AICI----------//

			string columnNameToSet = commandWords[3];
			string valueToSet = commandWords[5];
			int columnToSetIndex;
			COLUMN_TYPES typeSet;


			typeSet = t.findColumn(columnNameToSet, columnToSetIndex);
			if (columnToSetIndex != -1)
			{
				int wherePosition;

				wherePosition = command.find("WHERE");
				if (wherePosition != -1)
				{
					//avem where
					//index columnName = 5
					//index value = 7
					string columnName = commandWords[7];
					string value = commandWords[9];
					COLUMN_TYPES type;
					int columnIndex;

					type = t.findColumn(columnName, columnIndex);

					switch (type)
					{
					case 1: //TEXT
					{
						try
						{
							TEXT_COLUMN txtCol = t.getTextColumn(columnIndex);
							int entryIndex = txtCol.getIndex(value);

							//ce tip de coloana avem de schimbat
							//getColumn(indexSet) -> cunosc indexul entry-ului
							//setValueAtEntryIndex(entryIndex)
							switch (typeSet)
							{
							case 1: //TEXT
							{
								try
								{
									TEXT_COLUMN txtColToSet = t.getTextColumn(columnToSetIndex);
									txtColToSet.setValueAtIndex(entryIndex, valueToSet);
									t.setTextColumn(columnToSetIndex, txtColToSet);
									db.setTable(tableName, t);
								}
								catch (exception exc)
								{
									std::cout << exc.what();
								}

								break;
							}
							case 2: //INT
							{
								try
								{
									INT_COLUMN intColToSet = t.getIntColumn(columnToSetIndex);
									intColToSet.setValueAtIndex(entryIndex, stoi(valueToSet));
									t.setIntColumn(columnToSetIndex, intColToSet);
									db.setTable(tableName, t);
								}
								catch (exception exc)
								{
									std::cout << exc.what();
								}

								break;
							}
							case 3: //FLOAT
							{
								try
								{
									FLOAT_COLUMN floatColToSet = t.getFloatColumn(columnToSetIndex);
									floatColToSet.setValueAtIndex(entryIndex, stof(valueToSet));
									t.setFloatColumn(columnToSetIndex, floatColToSet);
									db.setTable(tableName, t);
								}
								catch (exception exc)
								{
									std::cout << exc.what();
								}

								break;
							}
							break;
							case -1:
								throw exception("ERROR! Nu a fost gasita coloana");
								break;
							default:
								throw exception("ERROR! Nu a fost gasita coloana");
								break;
							}

							std::cout << "\nA fost afisata inregistrarea cautata!";
						}
						catch (exception exc)
						{
							std::cout << exc.what();
						}
						break;
					}
					case 2: //INT
					{
						try
						{
							INT_COLUMN intCol = t.getIntColumn(columnIndex);
							int entryIndex = intCol.getIndex(stoi(value));
							//setColumnValue at index

							//t.showEntry(entryIndex);

							switch (typeSet)
							{
							case 1: //TEXT
							{
								try
								{
									TEXT_COLUMN txtColToSet = t.getTextColumn(columnToSetIndex);
									txtColToSet.setValueAtIndex(entryIndex, valueToSet);
									t.setTextColumn(columnToSetIndex, txtColToSet);
									db.setTable(tableName, t);
								}
								catch (exception exc)
								{
									std::cout << exc.what();
								}

								break;
							}
							case 2: //INT
							{
								try
								{
									INT_COLUMN intColToSet = t.getIntColumn(columnToSetIndex);
									intColToSet.setValueAtIndex(entryIndex, stoi(valueToSet));
									t.setIntColumn(columnToSetIndex, intColToSet);
									db.setTable(tableName, t);
								}
								catch (exception exc)
								{
									std::cout << exc.what();
								}

								break;
							}
							case 3: //FLOAT
							{
								try
								{
									FLOAT_COLUMN floatColToSet = t.getFloatColumn(columnToSetIndex);
									floatColToSet.setValueAtIndex(entryIndex, stof(valueToSet));
									t.setFloatColumn(columnToSetIndex, floatColToSet);
									db.setTable(tableName, t);
								}
								catch (exception exc)
								{
									std::cout << exc.what();
								}

								break;
							}
							break;
							case -1:
								throw exception("ERROR! Nu a fost gasita coloana");
								break;
							default:
								throw exception("ERROR! Nu a fost gasita coloana");
								break;
							}
							std::cout << "\nA fost afisata inregistrarea cautata!";
						}
						catch (exception exc)
						{
							std::cout << exc.what();
						}
						break;
					}
					case 3: //FLOAT
					{
						try
						{
							FLOAT_COLUMN floatCol = t.getFloatColumn(columnIndex);
							int entryIndex = floatCol.getIndex(stof(value));
							//setColumnValue at index

							//t.showEntry(entryIndex);

							switch (typeSet)
							{
							case 1: //TEXT
							{
								try
								{
									TEXT_COLUMN txtColToSet = t.getTextColumn(columnToSetIndex);
									txtColToSet.setValueAtIndex(entryIndex, valueToSet);
									t.setTextColumn(columnToSetIndex, txtColToSet);
									db.setTable(tableName, t);
								}
								catch (exception exc)
								{
									std::cout << exc.what();
								}

								break;
							}
							case 2: //INT
							{
								try
								{
									INT_COLUMN intColToSet = t.getIntColumn(columnToSetIndex);
									intColToSet.setValueAtIndex(entryIndex, stoi(valueToSet));
									t.setIntColumn(columnToSetIndex, intColToSet);
									db.setTable(tableName, t);
								}
								catch (exception exc)
								{
									std::cout << exc.what();
								}

								break;
							}
							case 3: //FLOAT
							{
								try
								{
									FLOAT_COLUMN floatColToSet = t.getFloatColumn(columnToSetIndex);
									floatColToSet.setValueAtIndex(entryIndex, stof(valueToSet));
									t.setFloatColumn(columnToSetIndex, floatColToSet);
									db.setTable(tableName, t);
								}
								catch (exception exc)
								{
									std::cout << exc.what();
								}

								break;
							}
							break;
							case -1:
								throw exception("ERROR! Nu a fost gasita coloana");
								break;
							default:
								throw exception("ERROR! Nu a fost gasita coloana");
								break;
							}
							std::cout << "\nA fost afisata inregistrarea cautata!";
						}
						catch (exception exc)
						{
							std::cout << exc.what();
						}
						break;
					}

					case -1: //UNKNOWN
						throw exception("ERROR! Nu exista coloana cautata");
						break;
					default:
						throw exception("ERROR! Nu exista coloana cautata");
						break;
					}
				}
				else
				{
					//nu avem where
					throw exception("ERROR! Nu exista coloana cautata");
				}
			}
			else
			{
				throw exception("ERROR! Nu exista coloana ceruta");
			}
		}
		else
		{
			throw exception("ERROR! Sintaxa incorecta");
		}

		break;
	}
	case 15: //IMPORT
	{
		vector<string> columnValues;
		TABLE t;
		string tableName;
		string filename;
		commandWords = stringTokenizer(command, ' ');
		for (int i = 0; i < commandWords.size(); i++)
		{
			cout << commandWords[i] << endl;
		}
		if (commandWords.size() == 3)
		{
			tableName = commandWords[1];
			try
			{
				t = db[tableName];
				std::cout << "\n\nTABELA CU NUMELE " << tableName << " A FOST GASITA CU SUCCES!\n";
			}
			catch (exception exc)
			{
				std::cout << exc.what();
			}

			filename = commandWords[2];
			if (filename.substr(filename.find_last_of(".") + 1) == "csv")
			{
				//are extensia potrivita
				ifstream csvFile(filename, ios::in);
				if (csvFile.is_open())
				{
					string line;
					while (getline(csvFile, line))
					{
						columnValues = stringTokenizer(line, ',');
						if (columnValues.size() > 1)
						{
							//insert values
							for (int i = 0; i < columnValues.size(); i++)
							{
								std::cout << "\n" << columnValues[i];
							}

							int textColumnCounter = 0;
							int intColumnCounter = 0;
							int floatColumnCounter = 0;

							if (t.getNumberOfColumns() != columnValues.size())
							{
								throw exception("Numarul valorilor nu corespunde cu numarul coloanelor din tabela");
							}
							else
							{
								///TODO: de gandit cum sa fie tabelele puse in ordine 
								///- asta nu e implementata inca si probabil trebuie regandit in phase 2 pentru ca acum are foarte multe probleme ce nu pot fi rezolvate fara restructurare
								for (int i = 0; i < columnValues.size(); i++)
								{
									//adaugat in fisier binar valorile
									filename = tableName + binExtension;
									ofstream dataFile(filename, ios::out | ios::app | ios::binary);
									if (dataFile.is_open())
									{
										dataFile.write((char*)&columnValues[i], sizeof(string));
										dataFile.close();
										if (!dataFile.good())
										{
											cout << "\nError occured at writing time";
										}
									}
									else
									{
										cout << "\nNu exista fisierul binar";
									}
									int dotPosition = columnValues[i].find('.');
									if (dotPosition != -1)
									{
										try
										{
											stof(columnValues[i]);
											FLOAT_COLUMN floatCol = t.getFloatColumn(floatColumnCounter);
											floatCol += stof(columnValues[i]);
											t.setFloatColumn(floatColumnCounter, floatCol);
											floatColumnCounter++;
											db.setTable(tableName, t);
										}
										catch (exception exc)
										{
											//std::cout<<exc.what();
											//inseamna ca e text => adaugam in coloana text
											TEXT_COLUMN txtCol = t.getTextColumn(textColumnCounter);
											txtCol += columnValues[i];
											t.setTextColumn(textColumnCounter, txtCol);
											textColumnCounter++;
											db.setTable(tableName, t);
										}
									}
									else
									{
										try
										{
											stoi(columnValues[i]);
											INT_COLUMN intCol = t.getIntColumn(intColumnCounter);
											intCol += stoi(columnValues[i]);
											t.setIntColumn(intColumnCounter, intCol);
											intColumnCounter++;
											db.setTable(tableName, t);
										}
										catch (exception exc)
										{
											//std::cout<<exc.what();
											//inseamna ca e text => adaugam in coloana text
											TEXT_COLUMN txtCol = t.getTextColumn(textColumnCounter);
											txtCol += columnValues[i];
											t.setTextColumn(textColumnCounter, txtCol);
											textColumnCounter++;
											db.setTable(tableName, t);
										}
									}
								}
							}

						}
						else
						{
							throw exception("Exista alt delimitator decat , in fisierul csv");
						}
					}
				}
				else
				{
					throw exception("Nu exista fisierul csv");
				}
			}
			else
			{
				throw exception("fisierul are extensia gresita");
			}

		}
		else
		{
			throw exception("Sintaxa este gresita!");
		}
		//hard save and reload ca sa apara modificarile din baza de date
		db.saveDb();
		db.loadDb();
		break;
	}

	case -1:
		std::cout << "Sintaxa gresita!";
		break;
	default:
		std::cout << "Sintaxa gresita!";
		break;
	}
}

int DATABASE::numberOfTables = 0;

class CommandBuilder
{
protected:
	string tableName;
	string command;
public:
	CommandBuilder() {}
	CommandBuilder(string tableName) : tableName(tableName) {}

	virtual string buildCommand() = 0;

	void setTableName(string tableName)
	{
		this->tableName = tableName;
	}

	string getTableName()
	{
		return this->tableName;
	}
};

//CREATE TABLE table_name [IF NOT EXISTS] ((nume_coloana_1,tip,dimensiune,valoare_implicita),(...))
class CreateBuilder : public CommandBuilder
{
private:
	bool conditionExists = false;
	int numberOfColumns;
	vector<string> columnNames;
	vector<string> columnTypes;
	vector<string> columnSizes;
	vector<string> defaultValues;
public:
	CreateBuilder(string tableName, bool conditionExists, int numberOfColumns, vector<string> columnNames, vector<string> columnTypes, vector<string> columnSizes, vector<string> defaultValues) : CommandBuilder(tableName)
	{
		this->conditionExists = conditionExists;
		this->numberOfColumns = numberOfColumns;
		if (this->numberOfColumns > 0)
		{
			for (int i = 0; i < this->numberOfColumns; i++)
			{
				this->columnNames.push_back(columnNames[i]);
				this->columnTypes.push_back(columnTypes[i]);
				this->columnSizes.push_back(columnSizes[i]);
				this->defaultValues.push_back(defaultValues[i]);
			}
		}
	}

	string buildCommand()
	{
		this->command = "CREATE TABLE " + this->tableName;
		if (this->conditionExists == true)
		{
			this->command += " IF NOT EXISTS";
		}
		this->command += "(";
		for (int i = 0; i < this->numberOfColumns; i++)
		{
			this->command += "(" + this->columnNames[i] + "," + this->columnTypes[i] + "," + this->columnSizes[i] + "," + this->defaultValues[i] + ")";
			if (i < this->numberOfColumns - 1)
			{
				this->command += ", ";
			}
		}
		this->command+= ")";

		return this->command;
	}
};

//DROP TABLE table_name
class DropBuilder : public CommandBuilder
{
public:
	DropBuilder() {}
	DropBuilder(string tableName) : CommandBuilder(tableName) {}

	string buildCommand()
	{
		this->command = "DROP TABLE " + this->tableName;
		return this->command;
	}
};

//DISPLAY TABLE table_name
class DisplayBuilder : public CommandBuilder
{
public:
	DisplayBuilder() {}
	DisplayBuilder(string tableName) : CommandBuilder(tableName) {}

	string buildCommand()
	{
		this->command = "DISPLAY TABLE " + this->tableName;
		return this->command;
	}
};

//INSERT INTO table_name VALUES(...);
class InsertBuilder : public CommandBuilder
{
private:
	vector<string>values;
public:
	InsertBuilder() {}
	InsertBuilder(string tableName, vector<string> values) : CommandBuilder(tableName)
	{
		for (int i = 0; i < values.size(); i++)
		{
			this->values[i] = values[i];
		}
	}

	string buildCommand()
	{
		this->command = "INSERT INTO " + this->tableName + " VALUES(";
		for (int i = 0; i < this->values.size(); i++)
		{
			this->command += this->values[i];
			if (i < this->values.size() - 1)
			{
				this->command += ",";
			}
		}
		this->command += ")";
		return this->command;
	}
};

//DELETE FROM table_name WHERE column_name=value
class DeleteBuilder : public CommandBuilder
{
private:
	string columnName;
	string value;
public:
	DeleteBuilder() {}
	DeleteBuilder(string tableName, string columnName, string value) : CommandBuilder(tableName)
	{
		this->columnName = columnName;
		this->value = value;
	}

	string buildCommand()
	{
		this->command = "DELETE FROM " + this->tableName + " WHERE " + this->columnName + "=" + this->value;
		return this->command;
	}
};

//SELECT ALL/(coloana) FROM table_name [WHERE column_name=value]
class SelectBuilder: protected CommandBuilder
{
protected:
	string finder;
	bool conditionExists = false;
public:
	SelectBuilder() {}
	SelectBuilder(string tableName, bool conditionExists) : CommandBuilder(tableName)
	{
		this->conditionExists = conditionExists;
	}
};

//SELECT ALL FROM table_name [WHERE column_name=value]
class SelectAllBuilder : public SelectBuilder
{
	string columnName;
	string value;
public:
	SelectAllBuilder()
	{
		this->finder = "ALL";
	}

	SelectAllBuilder(string tableName, bool conditionExists) :SelectBuilder(tableName, conditionExists)
	{
		this->finder = "ALL";
	}


	SelectAllBuilder(string tableName, bool conditionExists, string columnName, string value) :SelectBuilder(tableName, conditionExists)
	{
		this->finder = "ALL";
		this->columnName = columnName;
		this->value = value;
	}

	string buildCommand()
	{
		this->command += "SELECT " + this->finder + " FROM " + this->tableName;
		if (this->conditionExists == true)
		{
			this->command += " WHERE " + this->columnName + "=" + this->value;
		}
		return this->command;
	}
};

//SELECT (column,..) FROM table_name [WHERE column_name=value]
class SelectColumnsBuilder : public SelectBuilder
{
	int numberOfColumns;
	vector<string> columns;
	string checkColumnName;
	string checkValue;
public:
	SelectColumnsBuilder(string tableName, bool conditionExists, int numberOfColumns, vector<string> columns) :SelectBuilder(tableName, conditionExists)
	{
		this->numberOfColumns = numberOfColumns;
		if (this->numberOfColumns > 0)
		{
			for (int i = 0; i < this->numberOfColumns; i++)
			{
				this->columns.push_back(columns[i]);
			}
		}
	}

	SelectColumnsBuilder(string tableName, bool conditionExists, int numberOfColumns, vector<string> columns, string checkColumnName, string checkValue) :SelectBuilder(tableName, conditionExists)
	{
		this->numberOfColumns = numberOfColumns;
		if (this->numberOfColumns > 0)
		{
			for (int i = 0; i < this->numberOfColumns; i++)
			{
				this->columns.push_back(columns[i]);
			}
		}
		this->checkColumnName = checkColumnName;
		this->checkValue = checkValue;
	}

	string buildCommand()
	{
		for (int i = 0; i < this->numberOfColumns; i++)
		{
			this->finder += this->columns[i] + ",";

			if(i < this->numberOfColumns - 1)
			{
				this->finder += ",";
			}
		}
		this->command = "SELECT (" + this->finder + ")" + " FROM " + this->tableName;
		if (this->conditionExists == true)
		{
			this->command += " WHERE " + this->checkColumnName + "=" + this->checkValue;
		}
		return this->command;
	}
};

//UPDATE table_name SET column_name=value WHERE column_name=value
class UpdateBuilder : public CommandBuilder
{
private:
	string setColumnName;
	string setValue;
	string checkColumnName;
	string checkValue;

public:
	UpdateBuilder() {}
	UpdateBuilder(string tableName, string setColumnName, string setValue, string checkColumnName, string checkValue) : CommandBuilder(tableName)
	{
		this->setColumnName = setColumnName;
		this->setValue = setValue;
		this->checkColumnName = checkColumnName;
		this->checkValue = checkValue;
	}

	string buildCommand()
	{
		this->command = "UPDATE " + this->tableName + " SET " + this->setColumnName + "=" + this->setValue + " WHERE " + this->checkColumnName + "=" + this->checkValue;
		return this->command;
	}
};

//IMPORT table_name filename
class ImportBuilder : public CommandBuilder
{
private: 
	string filename;
public:
	ImportBuilder() {}
	ImportBuilder(string tableName, string filename) : CommandBuilder(tableName)
	{
		this->filename = filename;
	}


	string buildCommand()
	{
		this->command = "IMPORT " + this->tableName + " " + this->filename;
		return this->command;
	}
};


class IMenu 
{
public:
	virtual void showMenu() = 0;
	virtual void closeMenu(string& command) = 0;
};

void displayMenu()
{
	//CREATE = 1, DROP = 2, DISPLAY = 3,
	//INSERT = 11, DELETE = 12, SELECT = 13, UPDATE = 14, IMPORT = 15,
	system("cls");
	cout << "##############################################\n";
	cout << "\t\tMeniu Utilizare \t \n";
	cout << " 1.CREATE TABLE\n";
	cout << " 2.DROP TABLE\n";
	cout << " 3.DISPLAY TABLE\n";
	cout << " 11.INSERT INTO TABLE\n";
	cout << " 12.DELETE VALUES FROM TABLE\n";
	cout << " 13.SELECT FROM TABLE\n";
	cout << " 14.UPDATE VALUE FROM TABLE\n";
	cout << " 15.IMPORT FROM CSV FILE\n";
	cout << " 20.CUSTOM COMMAND\n";
	cout << " 0.Exit\n";
}

void createTable(DATABASE db)
{
	system("cls");

	bool conditie = false;
	string alegere;
	string tableName;
	vector<string> columnNames;
	vector<string> columnTypes;
	vector<string> columnSizes;
	vector<string> defaultValues;
	string columnName;
	string columnType;
	string columnSize;
	string defaultValue;

	cout << "Nume Tabela: "; cin >> tableName;
	cout << "Conditie IF NOT EXISTS ? (y sau n)\n";
	cout << "Alegere: ";
	cin >> alegere;
	if (alegere == "y" || alegere == "Y")
	{
		conditie = true;
	}
	else if (alegere == "n" || alegere == "N")
	{
		conditie = false;
	}

	cout << "Nume Coloana: "; cin >> columnName;
	cout << "Tip Coloana: "; cin >> columnType;
	cout << "Marime Coloana: "; cin >> columnSize;
	cout << "Valoare Default: "; cin >> defaultValue;
	columnNames.push_back(columnName);
	columnTypes.push_back(columnType);
	columnSizes.push_back(columnSize);
	defaultValues.push_back(defaultValue);
	cout << "\nAdaugati alta coloana in tabela? (y/n)\n";
	cout << "Alegere: ";
	cin >> alegere;
	while (alegere == "y" || alegere == "Y")
	{
		cout << "Nume Coloana: "; cin >> columnName;
		cout << "Tip Coloana: "; cin >> columnType;
		cout << "Marime Coloana: "; cin >> columnSize;
		cout << "Valoare Default: "; cin >> defaultValue;
		columnNames.push_back(columnName);
		columnTypes.push_back(columnType);
		columnSizes.push_back(columnSize);
		defaultValues.push_back(defaultValue);
		cout << "Adaugati alta coloana in tabela? (y/n)\n";
		cout << "Alegere: ";
		cin >> alegere;
	}


	try
	{
		CreateBuilder create(tableName, conditie, columnNames.size(), columnNames, columnTypes, columnSizes, defaultValues);
		cout << create.buildCommand();

		InterpretCommand(create.buildCommand(), db);
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}
	catch (exception exc)
	{
		cout << exc.what();
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}
}

void dropTable(DATABASE db)
{
	system("cls");

	string tableName;
	string alegere;

	cout << "Nume Tabela: "; cin >> tableName;
	cout << "Sunteti sigur? (y sau n)\n";
	cout << "Alegere: ";
	cin >> alegere;
	if (alegere == "y" || alegere == "Y")
	{

		try
		{
			DropBuilder drop(tableName);

			cout << drop.buildCommand();
			InterpretCommand(drop.buildCommand(), db);
			cout << "\nApasati orice tasta pentru a continua...";
			_getch();
			displayMenu();
		}
		catch (exception exc)
		{
			cout << exc.what();
			cout << "\nApasati orice tasta pentru a continua...";
			_getch();
			displayMenu();
		}
	}
	else if (alegere == "n" || alegere == "N")
	{
		displayMenu();
	}
}

void displayTable(DATABASE db)
{
	system("cls");

	string tableName;
	string alegere;

	cout << "Nume Tabela: "; cin >> tableName;
	cout << "Sunteti sigur? (y sau n)\n";
	cout << "Alegere: ";
	cin >> alegere;
	if (alegere == "y" || alegere == "Y")
	{

		try
		{
			DisplayBuilder display(tableName);

			cout << display.buildCommand();

			InterpretCommand(display.buildCommand(), db);
			cout << "\nApasati orice tasta pentru a continua...";
			_getch();
			displayMenu();
		}
		catch (exception exc)
		{
			cout << exc.what();
			cout << "\nApasati orice tasta pentru a continua...";
			_getch();
			displayMenu();
		}
	}
	else if (alegere == "n" || alegere == "N")
	{
		displayMenu();
	}
}

void insertData(DATABASE db)
{
	system("cls");

	string tableName;
	vector<string>values;
	string value;
	string alegere;

	cout << "Nume Tabela: "; cin >> tableName;
	cout << "Inserati Valori: "; cin >> value;
	values.push_back(value);
	cout << "Mai adaugati alta valoare? (y/n)";
	cout << "Alegere: ";
	cin >> alegere;
	while (alegere == "y" || alegere == "Y")
	{
		cout << "Inserati Valori: "; cin >> value;
		values.push_back(value);
		cout << "Mai adaugati alta valoare? (y/n)";
		cout << "\nAlegere: ";
		cin >> alegere;
	}

	InsertBuilder insert(tableName, values);
	try
	{
		InsertBuilder insert(tableName, values);

		cout << insert.buildCommand();

		InterpretCommand(insert.buildCommand(), db);
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}
	catch (exception exc)
	{
		cout << exc.what();
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}
}

void deleteData(DATABASE db)
{
	system("cls");

	string tableName;
	string columnName;
	string value;
	string alegere;

	cout << "Nume Tabela: "; cin >> tableName;
	cout << "Ce coloana sa verificam?"; cin >> columnName;
	cout << "Cu ce valoare sa verificam coloana aleasa?"; cin >> value;
	
	DeleteBuilder del(tableName, columnName, value);

	try
	{
		InterpretCommand(del.buildCommand(), db);
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}
	catch (exception exc)
	{
		cout << exc.what();
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}

}

void selectData(DATABASE db)
{
	system("cls");

	string columnName;
	string value;
	string alegere;
	bool conditie = false;

	string tableName;
	cout << "Nume Tabela: "; cin >> tableName;
	cout << "Selectam TOATE coloanele? (y/n): "; cin >> alegere;
	if (alegere == "y" || alegere == "Y")
	{
		cout << "Verificam si o conditie? (y/n): "; cin >> alegere;
		if (alegere == "y" || alegere == "Y")
		{
			conditie = true;
			cout << "Ce coloana sa verificam?"; cin >> columnName;
			cout << "Cu ce valoare sa verificam coloana aleasa?"; cin >> value;
			try
			{
				SelectAllBuilder selectAll(tableName, conditie, columnName, value);

				cout << selectAll.buildCommand();

				InterpretCommand(selectAll.buildCommand(), db);
				cout << "\nApasati orice tasta pentru a continua...";
				_getch();
				displayMenu();
			}
			catch (exception exc)
			{
				cout << exc.what();
				cout << "\nApasati orice tasta pentru a continua...";
				_getch();
				displayMenu();
			}
		}
		else if (alegere == "n" || alegere == "N")
		{
			conditie = false;
			try
			{
				SelectAllBuilder selectAll(tableName, conditie);

				cout << selectAll.buildCommand();

				InterpretCommand(selectAll.buildCommand(), db);
				cout << "\nApasati orice tasta pentru a continua...";
				_getch();
				displayMenu();
			}
			catch (exception exc)
			{
				cout << exc.what();
				cout << "\nApasati orice tasta pentru a continua...";
				_getch();
				displayMenu();
			}
		}
	}
	else if (alegere == "n" || alegere == "N")
	{
		vector<string> columns;
		string column;
		cout << "Ce coloana sa afisam? "; cin >> column;
		columns.push_back(column);
		cout << "Mai afisam alta coloana? (y/n)"; cin >> alegere;
		while (alegere != "n" || alegere == "N")
		{
			cout << "Ce coloana sa afisam? "; cin >> column;
			columns.push_back(column);
			cout << "Mai afisam alta coloana? (y/n)"; cin >> alegere;
		}

		cout << "Verificam si o conditie? (y/n): "; cin >> alegere;
		if (alegere == "y" || alegere == "Y")
		{
			conditie = true;
			cout << "Ce coloana sa verificam?"; cin >> columnName;
			cout << "Cu ce valoare sa verificam coloana aleasa?"; cin >> value;
			try
			{
				SelectColumnsBuilder selectColumns(tableName, conditie, columns.size(), columns, columnName, value);

				cout << selectColumns.buildCommand();

				InterpretCommand(selectColumns.buildCommand(), db);
				cout << "\nApasati orice tasta pentru a continua...";
				_getch();
				displayMenu();
			}
			catch (exception exc)
			{
				cout << exc.what();
				cout << "\nApasati orice tasta pentru a continua...";
				_getch();
				displayMenu();
			}
		}
		else if (alegere == "n" || alegere == "N")
		{
			conditie = false;
			try
			{
				SelectColumnsBuilder selectColumns(tableName, conditie, columns.size(), columns);

				cout << selectColumns.buildCommand();

				InterpretCommand(selectColumns.buildCommand(), db);
				cout << "\nApasati orice tasta pentru a continua...";
				_getch();
				displayMenu();
			}
			catch (exception exc)
			{
				cout << exc.what();
				cout << "\nApasati orice tasta pentru a continua...";
				_getch();
				displayMenu();
			}
		}
	}
}

void updateData(DATABASE db)
{
	system("cls");

	string tableName;
	string alegere;

	string setColumnName;
	string setValue;
	string checkColumnName;
	string checkValue;

	cout << "Nume Tabela: "; cin >> tableName;
	cout << "Din ce coloana modificam?:"; cin >> setColumnName;
	cout << "Valoarea pe care o vom modifica:"; cin >> setValue;
	cout << "Numele coloanei asupra careia ii aplicam o conditie:"; cin >> checkColumnName;
	cout << "Valoarea pe care o verificam:"; cin >> checkValue;

	try
	{
		UpdateBuilder update(tableName, setColumnName, setValue, checkColumnName, checkValue);

		cout << update.buildCommand();

		InterpretCommand(update.buildCommand(), db);
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}
	catch (exception exc)
	{
		cout << exc.what();
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}
}

void importData(DATABASE db)
{
	system("cls");

	string tableName;
	string filename;

	cout << "Nume Tabela: "; cin >> tableName;
	cout << "Numele fisierului.csv din care sa inseram date"; cin >> filename;
	try
	{
		ImportBuilder import(tableName, filename);

		cout << import.buildCommand();

		InterpretCommand(import.buildCommand(), db);
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}
	catch (exception exc)
	{
		cout << exc.what();
		cout << "\nApasati orice tasta pentru a continua...";
		_getch();
		displayMenu();
	}
}

class Menu : IMenu
{
private:
	DATABASE db;
public:
	Menu(DATABASE _db) : db(_db) { }
	
	void showMenu() 
	{
		int option;
		string command;
		system("cls");
		displayMenu();
		cout << "\nAlege optiunea: "; cin >> option;
		while (option != 0)
		{

			//CREATE = 1, DROP = 2, DISPLAY = 3,
			//INSERT = 11, DELETE = 12, SELECT = 13, UPDATE = 14, IMPORT = 15,
			switch (option)
			{
			case 1:
				createTable(this->db);
				cout << "\nAlege optiunea: "; cin >> option;
				break;
			case 2:
				dropTable(this->db);
				cout << "\nAlege optiunea: "; cin >> option;
				break;
			case 3:
				displayTable(this->db);
				cout << "\nAlege optiunea: "; cin >> option;
				break;
			case 11:
				insertData(this->db);
				cout << "\nAlege optiunea: "; cin >> option;
				break;
			case 12:
				deleteData(this->db);
				cout << "\nAlege optiunea: "; cin >> option;
				break;
			case 13:
				selectData(this->db);
				cout << "\nAlege optiunea: "; cin >> option;
				break;
			case 14:
				updateData(this->db);
				cout << "\nAlege optiunea: "; cin >> option;
				break;
			case 15:
				importData(this->db);
				cout << "\nAlege optiunea: "; cin >> option;
				break;
			case 20:
				closeMenu(command);
				while (command != "BACK")
				{
					try
					{
						cout << "\n" << command << "\n";
						InterpretCommand(command, db);
					}
					catch (exception err)
					{
						cout << err.what();
					}
					cout << "\nApasa orice tasta pentru a continua...";
					_getch();
					closeMenu(command);
				}

				cout << "\nApasati orice tasta pentru a continua...";
				_getch();
				displayMenu();
				cout << "\nAlege optiunea: "; cin >> option;

				break;
			case 0:
				cout << "\nVa multumesc pentru utilizare!";
				break;
			default:
				cout << "\nOptiune invalida! Mai incearca o data: "; cin >> option;
				break;
			}
		}
	}

	void closeMenu(string& command)
	{
		system("cls");
		cout << "Pentru a porni meniul din nou, scrie BACK";
		cout << "\nIntrodu comanda: "; getline(std::cin,command);

	}
};

int main(int argc, char* argv[])
{
	//TEXT_COLUMN txtC("Nume", COLUMN_TYPES::TEXT, 25, "NULL");
	//INT_COLUMN intC("Nume", COLUMN_TYPES::INTEGER, 25, 200);
	//FLOAT_COLUMN floatC("Nume", COLUMN_TYPES::FLOAT, 25, 200.525);
	//std::cout << txtC << endl << intC << endl << floatC << endl;

	DATABASE db(1, "ProjectDB");
	db.loadDb();
	Menu menu(db);

	//std::cout << "argc: " << argc << endl;
	//for (int i = 0; i < argc; i++)
	//{
	//	cout << "argv[" << i << "]:" << argv[i] << endl;
	//}
	if (argc == 1)
	{
		menu.showMenu();
		//string command;
		//cout << "Comanda: ";
		//while (getline(std::cin, command))
		//{
		//	try
		//	{
		//		cout << "\n" << command << "\n";
		//		InterpretCommand(command, db);
		//	}
		//	catch (exception err)
		//	{
		//		cout << err.what();
		//	}
		//	cout << "\nApasa orice tasta pentru a continua...";
		//	_getch();
		//	system("cls");
		//	cout << "Comanda: ";
		//}
	}
	else if (argc > 1 && argc < 7)
	{
		//iterat prin fiecare argv
		//argv[0] = .exe file
		for (int i = 1; i < argc; i++)
		{
			string fisierCurent = argv[i];
			if (fisierCurent.substr(fisierCurent.find_last_of(".") + 1) == "txt")
			{
				//cout << "are extensia corecta";
				ifstream inputFile(fisierCurent, ios::in);
				if (inputFile.is_open())
				{
					//facem ceva
					string command;
					while (getline(inputFile, command))
					{
						try
						{
							cout << "\n" << command << "\n";
							InterpretCommand(command, db);
						}
						catch (exception err)
						{
							cout << err.what();
						}
					}
					inputFile.close();
				}
				else
				{
					cout << "\n!!!Nu exista fisierul cu numele " << fisierCurent << "\n";
					//merge cu exceptie
				}

			}
			else
			{
				cout << "\n!!!Nu are extensia corecta\n";
				//merge cu exceptie
			}
			//check if argv are .txt si dupa incepem nebunia cu fisiere

		}
	}
	else
	{
		cout << "\n!!!Aveti voie MAXIM 5 fisiere\n";
	}


	db.saveDb();
	//std::cout << "\n\n#########################################\n\n";
	//std::cout << db;
	return 0;


	// meniu cu optiuni predefinite pentru partile importante ale comenzii,
	// optiune insert manual command
	// optiune exit
	
	//TODO: facut meniu cu citire din consola pentru param unei functii
	//cu completare a sintaxei automata intr-un stringstream (cred? de vazut)
}

//TODO: getteri si setteri pentru clase - validari la setteri -DONE
//TODO: adaugat un camp constant (id la database?) - DONE
//TODO: operatori[] - DONE
//TODO: operatori+ - * sau / - DONE
//TODO: operatori++ -- cu ambele forme - merge la columns la ++ adauga element cu default value / la -- scoate ultimul element - DONE
//TODO: operator cast - de implementat
//TODO: operator ! - posibil un delete 
//TODO: operator < > =< >= -pot compara inregistrari in tabela
//TODO: operator == -pot compara inregistrari in tabela
//TODO: operatori << si >> -dupa ce o sa fac meniu implementez citire pentru ca e ciudat de integrat acum	