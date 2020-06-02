#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "timer.h"

#define INFINI 999

int min(int a, int b)
{
	return (a < b) ? a : b;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

class Node
{
private:
	std::string key;
	bool feuille;
	int value;
	std::vector<Node*> fils;
	int bMin = true;

public:
	Node(std::string key): key(key), feuille(false), value(0)
	{
		std::cout << "Creation du noeud " << key << ".\n";
	}

public:
	Node(std::string key, int value): key(key), feuille(true), value(value)
	{
		std::cout << "Creation de la feuille " << key << " de valeur " << intToString(value) << ".\n";
	}

private:
	static std::string intToString(int x)
	{
		if (x >= INFINI)
			return "Inf";

		else if (x <= -INFINI)
			return "-Inf";

		std::stringstream ss;
		ss << x;
		return ss.str();
	}

public:
	void addFils(Node& f)
	{
		fils.push_back(&f);
		f.bMin = !bMin;
		std::string s1, s2;

		if (feuille)
			s1 = intToString(value);
		else
			s1 = bMin ? "min" : "max";

		if (f.feuille)
			s2 = intToString(f.value);
		else
			s2 = f.bMin ? "min" : "max";

		std::cout << key << "(" << s1 << ") -> " << f.key << "(" << s2 << ")\n";
	}

	int alphabeta()
	{
		return alphabeta(-INFINI, INFINI);
	}

	int alphabeta(int alpha, int beta)
	{
		std::cout << "Exploration du noeud " << key << "(" << intToString(alpha) << ", " << intToString(beta) << ")";

		if (feuille)
		{
			std::cout << " valeur = " << value << std::endl;
			return value;
		}

		std::cout << std::endl;

		if (bMin)
		{
			int v = INFINI;

			for (Node* f : fils)
			{
				v = min(v, f->alphabeta(alpha, beta));
				std::cout << "Noeud " << key << " v = " << intToString(v);

				if (alpha >= v)
				{
					std::cout << " => coupure alpha" << std::endl;
					return v;
				}

				beta = min(beta, v);
				std::cout << " => beta = " << intToString(beta) << std::endl;
			}

			return v;
		}
		else
		{
			int v = -INFINI;

			for (Node* f : fils)
			{
				v = max(v, f->alphabeta(alpha, beta));
				std::cout << "Noeud " << key << " v = " << intToString(v);

				if (beta <= v)
				{
					std::cout << " => coupure beta" << std::endl;
					return v;
				}

				alpha = max(alpha, v);
				std::cout << " => alpha = " << intToString(alpha) << std::endl;
			}

			return v;
		}
	}

	void setMin(bool min)
	{
		bMin = min;
	}

	~Node()
	{
		fils.~vector();
	}
};

int main()
{
	Node A("A");
	A.setMin(false);

	Node B("B"), C("C"), D("D"), E("E"), F("F"), G("G"), H("H"), I("I"), J("J"), K("K"), L("L"),
		 M("M"), N("N"), O("O"), P("P"), Q("Q"), R("R"), S("S"), T("T"), U("U"), V("V");

	Node F1("F1", 3),  F2("F2", 9), F3("F3", 1), F4("F4", 8), F5("F5", 6), F6("F6", 20),
		 F7("F7", 0), F8("F8", 13), F9("F9", 2), F10("F10", 4), F11("F11", 1), F12("F12", 5),
		 F13("F13", 2), F14("F14", 8), F15("F15", 21), F16("F16", 14), F17("F17", 2), F18("F18", 4),
		 F19("F19", 7), F20("F20", 5), F21("F21", 8), F22("F22", 13), F23("F23", 11), F24("F24", 4);

	A.addFils(B); A.addFils(C); A.addFils(D);
	B.addFils(E); B.addFils(F);
	C.addFils(G); C.addFils(H);
	D.addFils(I); D.addFils(J);
	E.addFils(K); E.addFils(L);
	F.addFils(M); F.addFils(N);
	G.addFils(O); G.addFils(P);
	H.addFils(Q); H.addFils(R);
	I.addFils(S); I.addFils(T);
	J.addFils(U); J.addFils(V);
	K.addFils(F1); K.addFils(F2);
	L.addFils(F3); L.addFils(F4);
	M.addFils(F5); M.addFils(F6);
	N.addFils(F7); N.addFils(F8);
	O.addFils(F9); O.addFils(F10);
	P.addFils(F11); P.addFils(F12);
	Q.addFils(F13); Q.addFils(F14);
	R.addFils(F15); R.addFils(F16);
	S.addFils(F17); S.addFils(F18);
	T.addFils(F19); T.addFils(F20);
	U.addFils(F21); U.addFils(F22);
	V.addFils(F23); V.addFils(F24);

	std::cout << "\nCalcul de la valeur du noeud A...\n";
	std::cout << "On note v la valeur qui remonte des noeuds plus profonds.\n\n";

	int result = A.alphabeta();

	std::cout << "\nValeur du noeud A : " << result << "\n\n";

	std::cin.get();
	return 0;
}