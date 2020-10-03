#pragma once
#include "vector.h"

namespace mystl {

	template <typename T>
	class stack : public vector<T> {
	public: // size(), empty()��public�ӿڿ�ֱ��ʹ��
		void push(T const& e) { this->insert(this->size(), e); } // ��ջ�� ��Ч�ڽ���Ԫ����Ϊ������ĩԪ�ز���
		T pop() { return this->remove(this->size() - 1); } // ��ջ����Ч��ɾ��������ĩԪ��
		T& top() { return (*this)[this->size() - 1]; } // ȡ����ֱ�ӷ���������ĩԪ��
	};


	void convert_recursion(stack<char>& S, __int64 n, int base) {
		static char digit[] = { '0', '1', '2', '3', '4', '5','6','7','8','9','A','B','C','D','E','F' };
		while (n > 0) {
			convert_recursion(S, n / base, base);
			S.push(digit[n % base]);
		}
	}

	void convert_iterate(stack<char>& S, __int64 n, int base) {
		static char digit[] = { '0', '1', '2', '3', '4', '5','6','7','8','9','A','B','C','D','E','F' };
		int remainder = (int)(n % base);
		S.push(digit[remainder]);
		n /= base;
	}


	// ����ƥ��
	void trim(const char exp[], int& lo, int& hi) {
		while ((lo <= hi) && (exp[lo] != '(') && (exp[lo] != ')')) lo++; // first ( from left
		while ((lo <= hi) && (exp[hi] != '(') && (exp[hi] != ')')) hi--; // first ) from right
	}

	int divide(const char exp[], int lo, int hi) { // divide, visit from left to the poision where number of ( and ) is the same
		int mi = lo; int crc = 1;
		while ((0 < crc) && (++mi < hi)) {
			if (exp[mi] == ')') crc--;
			if (exp[mi] == '(') crc++;
		}
		return mi; // if mi<=hi, valid, else can not match
	}
	// recursion
	bool paren_recursion(const char exp[], int lo, int hi) {
		trim(exp, lo, hi); if (lo > hi) return true;
		if (exp[lo] != '(') return false;
		if (exp[hi] != ')') return false;
		int mi = divide(exp, lo, hi);
		if (mi > hi) return false; // number of ( ) not match until the end, so false
		return paren_recursion(exp, lo + 1, mi - 1) && paren_recursion(exp, mi + 1, hi);
	}

	// iterate
	bool paren_iterate(const char exp[], int lo, int hi) {
		stack<char> S;
		for (int i = 0; exp[i]; i++) {
			switch (exp[i]) {
			case '(': case '[': case '{': S.push(exp[i]); break;
			case ')': if ((S.empty()) || ('(' != S.pop())) return false; break;
			case ']': if ((S.empty()) || (']' != S.pop())) return false; break;
			case '}': if ((S.empty()) || ('}' != S.pop())) return false; break;
			default: break;
			}
			return S.empty();
		}
		return false;
	}


	// ��ֵ��
#define N_OPTR 9 //���������
	typedef enum { ADD, SUB, MUL, DIV, POW, FAC, L_P, R_P, EOE } Operator; //���������
	//�ӡ������ˡ������˔ڡ��׳ˡ������š������š���ʼ������ֹ��
	const char pri[N_OPTR][N_OPTR] = { //��������ȵȼ� [ջ��] [��ǰ] 6 /* |-------------- ��ǰ����� --------------| */
	/*				+	 -	   *    /    ^    !    (    )   \0   */
		/* --	+ */	'>', '>', '<', '<', '<', '<', '<', '>', '>',
		/* |	- */	'>', '>', '<', '<', '<', '<', '<', '>', '>',
		/* ջ	* */	'>', '>', '>', '>', '<', '<', '<', '>', '>',
		/* ��	/ */	'>', '>', '>', '>', '<', '<', '<', '>', '>',
		/* ��	^ */	'>', '>', '>', '>', '>', '<', '<', '>', '>',
		/* ��	! */	'>', '>', '>', '>', '>', '>', ' ', '>', '>',
		/* ��	( */	'<', '<', '<', '<', '<', '<', '<', '=', ' ',
		/* |	) */	' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		/* --	\0 */	'<', '<', '<', '<', '<', '<', '<', ' ', '='
	};

	/*
	float evaluate(char* S, char*& RPN) {
		stack<float> opnd; stack<char> optr;
		optr.push('\0');
		while (!optr.empty()) {
			if (isdigit(*S)) {
				readNumber(S, opnd); append(RPN, opnd.top()); // ��ȡ�������� ���������RPNĩβ
			}
			else {
				switch (orderBetween(optr.top(), *S)) {
				case '<': //ջ����������ȼ�����ʱ
					optr.push(*S); S++;
					break;
				case '=': // ���ȼ���ȣ�Ϊ�����Ż���β�ڱ�'\0��
					optr.pop(); S++;
					break;
				case '>':// ջ����������ȼ����ߣ��������㣬�������������ջ
					char op = optr.pop();
					append(RPN, op);
					if ('!' == op) { // ������һԪ�����
						float pOpnd = opnd.pop();
						opnd.push(calcu(op.pOpnd));
					}
					else { // ��Ԫ�����
						float pOpnd2 = opnd.pop(), pOpnd1 = opnd.pop(); //ȡ����ǰ������
						opnd.push(calcu(pOpnd1, op, pOpnd2)); // ʵʩ��Ԫ���㣬�����ջ
					}
					break;

				default: exit(-1); // �﷨����
				}//switch

			}
		}
		return opnd.pop(); // ���յĽ��
	}

	*/


	// �˻ʺ�����
	struct Queen {
		int x, y;
		Queen(int xx = 0, int yy = 0) :x(xx), y(yy) {};
		bool operator==(Queen const& q) const {
			return (x == q.x) ||
				(y == q.y) ||
				(x + y == q.x + q.y) ||
				(x - y == q.x - q.y);
		}
		bool operator != (Queen const& q) const { return !(*this == q); }
	};

	void placeQueens(int N) {
		stack<Queen> solu;
		Queen q(0, 0);
		do {
			if (N <= solu.size() || N <= q.y) { // ���Ѿ����磬 ��
				q = solu.pop(); q.y++; //����һ�У���������̽��һ��
			}
			else {
				while ((q.y < N) && (0 <= solu.find(q))) {
					q.y++; // nCheck++; //�����ҵ��ɰڷ���һ�ʺ����
				}
				if (N > q.y) { // �����ڿɰڷŵ��У���
					solu.push(q);
					//if (N <= solu.size()) nSolu++; // �����ֽ��Ѿ���Ϊȫ�ֽ⣬��ͨ��ȫ�ֱ���nSolu����
					q.x++; q.y = 0; //ת����һ�У��ӵ�0�п�ʼ����̽��һ�ʺ�
				}
			}
		} while ((0 < q.x) || (q.y < N)); // ���з�֧���Ѿ���ٻ��֦֮���㷨������
	}


	// �Թ�
	typedef enum { AVAILABLE, ROUTE, BACKTRACKED, WALL } Status; // �Թ���Ԫ״̬
	// ԭʼ���õģ� �ڵ�ǰ·���ϵģ����з��������ʧ�ܺ���ݵģ�����ʹ�õ�

	typedef enum { UNKNOWN, EAST, SOUTH, WEST, NORTH, NO_WAY } ESWN; // ��Ԫ������ڽӷ���
	// δ���� �������� ��·

	inline ESWN nextESWN(ESWN eswn) { return ESWN(eswn + 1); }// ����ת����һ���ڽӷ���

	struct Cell {
		int x, y; Status status;
		ESWN incoming, outgoing; //���룬�߳�����
	};

	#define LABY_MAX 24 //����Թ��ߴ�
	Cell laby[LABY_MAX][LABY_MAX]; //�Թ�

	// �ڸ��ѯ
	inline Cell* neighbor(Cell* cell) { // ��ѯ��ǰλ�õ����ڸ��
		switch (cell->outgoing) {
		case EAST: return cell + LABY_MAX; // ��
		case SOUTH: return cell + 1; // ��
		case WEST: return cell - LABY_MAX; // ��
		case NORTH: return cell - 1; //��
		default:exit(-1);
		}
	}

	// �ڸ�ת��
	inline Cell* advance(Cell* cell) { // �ӵ�ǰλ��ת�����ڸ��
		Cell* next;
		switch (cell->outgoing) {
		case EAST: next = cell + LABY_MAX;	next->incoming = WEST; break; //��
		case SOUTH: next = cell + 1;		next->incoming = NORTH; break;
		case WEST: next = cell - LABY_MAX;	next->incoming = EAST; break;
		case NORTH:next = cell - 1; next->incoming = SOUTH; break;
		default: exit(-1);
		}
		return next;
	}

	// 
	bool labyrinth(Cell Laby[LABY_MAX][LABY_MAX], Cell* s, Cell* t) {
		if ((AVAILABLE != s->status) || (AVAILABLE != t->status)) return false; // �˻����
		stack<Cell*> path; // ��ջ��¼ͨ·
		s->incoming = UNKNOWN; s->status = ROUTE; path.push(s); //���
		do { // ��������������̽������
			Cell* c = path.pop();
			if (c == t) return true; 
			while (NO_WAY > (c->outgoing = nextESWN(c->outgoing))) //��һ������з���
				if (AVAILABLE == neighbor(c)->status) break; //��ͼ�ҵ���δ��̽�ķ���
			if (NO_WAY <= c->outgoing) { //�����з����Ѿ����Թ�
				c->status = BACKTRACKED; c = path.pop();
			}
			else {
				path.push(c = advance(c)); c->outgoing = UNKNOWN; c->status = ROUTE;
			}
		} while (!path.empty());
		return false;

}



};
