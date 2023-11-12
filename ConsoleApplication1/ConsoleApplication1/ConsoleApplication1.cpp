#include <algorithm>
#include <iostream>
#include <time.h>

#define N 10

using namespace std;

template <class T>
class Vertex
{
protected:
	static int W;
	T Data;
	Vertex<T>* Left;
	Vertex<T>* Right;
	Vertex<T>* Head;

public:


	Vertex() :Left(NULL), Right(NULL), Head(NULL)
	{

	}

	~Vertex()
	{
		Data = 0;
		Left = NULL;
		Right = NULL;
	}

	int getW()
	{
		return this->W;
	}

	void setW(int new_weight)
	{
		this->W = new_weight;
	}

	void setLeft(Vertex<T>* new_Left)
	{
		this->Left = new_Left;
	}

	void setRight(Vertex<T>* new_Right)
	{
		this->Right = new_Right;
	}

	void setHead(Vertex<T>* new_Head)
	{
		this->Head = new_Head;
	}

	Vertex<T>* getLeft()
	{
		return this->Left;
	}

	Vertex<T>* getRight()
	{
		return this->Right;
	}

	Vertex<T>* getHead()
	{
		return this->Head;
	}

	T getData()
	{
		return Data;
	}

	void setData(T new_data)
	{
		this->Data = new_data;
	}
};

template <class T>
int Vertex<T>::W = 0;

template <class T>
class BinaryTree : public Vertex<T>
{
protected:
	Vertex<T>* Root;
	T A[N];
public:

	BinaryTree() :Root(NULL)
	{

	}

	void setRoot(Vertex<T>* new_Root)
	{
		this->Root = new_Root;
	}

	int random()
	{
		return rand() % 256;
	}

	void fillArr()
	{
		for (int i = 0; i < N; i++)
		{
			A[i] = random();
		}
	}

	int size(Vertex<T>* p)
	{
		if (p == NULL)
		{
			return 0;
		}
		else
		{
			return 1 + size(p->getLeft()) + size(p->getRight());
		}
	}

	int height(Vertex<T>* p)
	{
		if (p == NULL)
		{
			return 0;
		}
		else
		{
			return 1 + max(height(p->getLeft()), height(p->getRight()));
		}
	}

	float averageHeight()
	{
		return SDP(Root, 1) / (float)size(Root);
	}

	virtual int SDP(Vertex<T>* p, int level) = 0;

	int sum(Vertex<T>* p)
	{
		if (p == NULL)
		{
			return 0;
		}
		else
		{
			return p->getData() + sum(p->getLeft()) + sum(p->getRight());
		}
	}

	void printLR(Vertex<T>* p)
	{
		if (p != NULL)
		{
			printLR(p->getLeft());
			cout << p->getData() << " ";
			printLR(p->getRight());
		}
	}

	Vertex<T>* getRoot()
	{
		return this->Root;
	}

};

template <class T>
class DOPTree : public BinaryTree<T>
{
private:
	int AW[N + 1][N + 1];
	int AP[N + 1][N + 1];
	int AR[N + 1][N + 1];
	int W[N];
	T V[N];
public:

	void randVW()
	{
		for (int i = 0; i < N; i++)
		{
			V[i] = i + 1;
			W[i] = 1 + rand() % 10;
		};
	}

	void calculateW()
	{
		for (int i = 0; i <= N; i++)
			for (int j = i + 1; j <= N; j++)
				AW[i][j] = AW[i][j - 1] + W[j - 1];
	}

	void calculatePR()
	{
		int i, j, m, min, k, h;
		for (i = 0; i < N; i++)
		{
			j = i + 1;
			AP[i][j] = AW[i][j];
			AR[i][j] = j;
		}
		for (h = 2; h <= N; h++)
		{
			for (i = 0; i <= N - h; i++)
			{
				j = i + h;
				m = AR[i][j - 1];
				min = AP[i][m - 1] + AP[m][j];
				for (k = m + 1; k <= AR[i + 1][j]; k++)
				{
					int x = AP[i][k - 1] + AP[k][j];
					if (x < min)
					{
						m = k;
						min = x;
					}
				}
				AP[i][j] = min + AW[i][j];
				AR[i][j] = m;
			}
		}
	}

	void add(T D, int W, Vertex<T>*& p)
	{
		if (p == NULL)
		{
			p = new Vertex<T>;
			if (this->getRoot() == NULL) {
				this->setRoot(p);
				this->setHead(NULL);
			}
			p->setData(D);
			p->setW(W);
		}
		else if (D < p->getData())
		{
			Vertex<T>* pp = p->getLeft();
			add(D, W, pp);
			p->setLeft(pp);
		}

		else if (D > p->getData())
		{
			Vertex<T>* pq = p->getRight();
			add(D, W, pq);
			p->setRight(pq);
		}

	}

	void createTree(int L, int R, Vertex<T>*& root)
	{
		if (L < R)
		{
			int k = AR[L][R];
			add(V[k - 1], W[k - 1], root);
			createTree(L, k - 1, root);
			createTree(k, R, root);
		}
	}

	int SDP(Vertex<T>* p, int level)
	{
		if (p == NULL)
		{
			return 0;
		}
		else
		{
			return level * (p->getW()) + SDP(p->getLeft(), level + 1) +
				SDP(p->getRight(), level + 1);
		}
	}

	void printAWmatrix()
	{
		cout << "AW   ";
		for (int i = 0; i < N + 1; i++) printf("%3d ", i);
		cout << endl;
		for (int i = 0; i < N + 1; i++) cout << "____";
		cout << "____" << endl;
		for (int i = 0; i < N + 1; i++)
		{
			printf("%3d |", i);
			for (int j = 0; j < N + 1; j++) printf("%3d|", AW[i][j]);
			cout << endl;
			for (int j = 0; j < N + 1; j++) cout << "____";
			cout << "____|" << endl;
		}
		cout << endl;
	}

	void printAPmatrix()
	{
		cout << "AP   ";
		for (int i = 0; i < N + 1; i++) printf("%3d ", i);
		cout << endl;
		for (int i = 0; i < N + 1; i++) cout << "____";
		cout << "____" << endl;
		for (int i = 0; i < N + 1; i++)
		{
			printf("%3d |", i);
			for (int j = 0; j < N + 1; j++) printf("%3d|", AP[i][j]);
			cout << endl;
			for (int j = 0; j < N + 1; j++) cout << "____";
			cout << "____|" << endl;
		}
		cout << endl;
	}

	void printARmatrix()
	{
		cout << "AR   ";
		for (int i = 0; i < N + 1; i++) printf("%3d ", i);
		cout << endl;
		for (int i = 0; i < N + 1; i++) cout << "____";
		cout << "____" << endl;
		for (int i = 0; i < N + 1; i++)
		{
			printf("%3d |", i);
			for (int j = 0; j < N + 1; j++) printf("%3d|", AR[i][j]);
			cout << endl;
			for (int j = 0; j < N + 1; j++) cout << "____";
			cout << "____|" << endl;
		}
		cout << endl;
	}

	int sumW(Vertex<T>* p)
	{
		if (p == NULL)
		{
			return 0;
		}
		else
		{
			return p->getW() + sumW(p->getLeft()) + sumW(p->getRight());
		}
	}

	float WaverageHeight()
	{
		return (float)SDP(this->getRoot(), 1) / sumW(this->getRoot());
	}
};

int main()
{
	srand(time(NULL));

	DOPTree<int> tree;

	tree.randVW();
	tree.calculateW();
	tree.calculatePR();
	Vertex<int>* p = tree.getRoot();
	tree.createTree(0, N, p);

	cout << "DOP left to right:\n";
	tree.printLR(p);
	cout << endl << endl;
	tree.printAWmatrix();
	cout << endl << endl;
	tree.printAPmatrix();
	cout << endl << endl;
	tree.printARmatrix();

	printf("\n\nn=100 | size |    sum    | height | Wavgh\n");
	printf(
		"DOP   | %4d | %9d | %6d | %3.3f\n",
		tree.size(tree.getRoot()),
		tree.sum(tree.getRoot()),
		tree.height(tree.getRoot()),
		tree.WaverageHeight());

	return 0;
}