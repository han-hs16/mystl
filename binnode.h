#pragma once
#include"stack.h"
#include"queue.h"

namespace mystl {

#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p) ? (p)->height:-1) // �ڵ�߶�
	typedef enum { RB_RED, RB_BLACK } RBColor; // �ڵ���ɫ

	// ״̬�����ʵ��ж�
#define IsRoot(x)		(!((x).parent))
#define IsLChild(x)		(!IsRoot(x) && (&(x) == (x).parent->lChild))
#define IsRChild(x)		(!IsRoot(x) && (&(x) == (x).parent->rChild))
#define HasParent(x)	(!IsRoot(x))
#define HasLChild(x)	((x).lChild)
#define HasRChild(x)	((x).rChild)
#define HasChild(x)		(HasLChild(x) || HasRChild(x))
#define HasBothChild(x)	(HasLChild(x) && HasRChild(x))
#define IsLeaf(x)		(!HasChild(x))

// ��˽ڵ���صĽڵ��Լ�ָ��
#define sibling(p) (			\
	IsLChild(*(p)) ?			\
		(p)->parent->rChild :	\
		(p)->parent->lChild		\
) //�ֵ�

#define uncle(p) (					\
	IsLChild(*((p)->parent)) ?		\
		(p)->parent->parent->rChild \
		(p)->parent->parent->lChild \
) // ����= ���׵��ֵ�

#define FromParentTo(x) (	\
	IsRoot(x) ? _root : (	\
	IsLChild(x) ? (x).parent->lChild : (x).parent->rChild \
	) \
) //���Ը��׵�ָ��

	// �������ڵ�ģ����
	template<typename T> struct BinNode { 
		T data;
		BinNodePosi(T) parent; BinNodePosi(T) lChild; BinNodePosi(T) rChild;
		int height;
		int npl; // null path length 
		RBColor color;

		// ���캯��
		BinNode() : parent(nullptr), lChild(nullptr), rChild(nullptr), height(0), npl(1), color(RB_RED) {};
		BinNode(T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED)
			:data(e), parent(p), lChild(lc), rChild(rc), height(h), npl(l), color(c) {}

		// ����
		int size(); //��ǰ�ڵ�ĺ�������������Ĺ�ģ
		BinNodePosi(T) insertAsLC(T const&); // ���뵽����
		BinNodePosi(T) insertAsRC(T const&);
		BinNodePosi(T) succ(); //��ǰ�ڵ�����������ֱ�Ӻ��

		
		template<typename VST> void travLevel(VST&); //������α���
		template<typename VST> void travPre(VST&); //�����������
		template<typename VST> void travIn(VST&); //�����������
		template<typename VST> void travPost(VST&); //�����������
		
	// �Ƚ����� �б���
		bool operator<(BinNode const& bn) { return data < bn.data; }
		bool operator==(BinNode const& bn) { return data == bn.data; }

	private:
		template<typename VST> void travPre_R(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_R(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travPost_R(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travPre_I1(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travPre_I2(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_I1(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_I2(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_I3(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travIn_I4(BinNodePosi(T) x, VST& visit);
		template<typename VST> void travPost_I(BinNodePosi(T) x, VST& visit);

	};


	/*
					IMPLEMENT
	*/
	
	template<typename T>
	int BinNode<T>::size() {
		if (!this) return 0;
		int count = 0;
		queue<BinNodePosi(T)> Q;
		Q.enqueue(this);
		while (!Q.empty()) {
			BinNodePosi(T) x = Q.dequeue();	count++;
			if (HasLChild(*x)) Q.enqueue(x->lChild);
			if (HasRChild(*x)) Q.enqueue(x->rChild);
		}
		return count;
	}




	// ���뺢�ӽڵ�
	template<typename T>
	BinNodePosi(T) BinNode<T>::insertAsLC(T const& e) { return lChild = new BinNode(e, this); }

	template<typename T>
	BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) { return rChild = new BinNode(e, this); }

	// ���������ֱ�Ӻ��: �����Һ��ӣ���Ϊ����������������ӣ�����(û���Һ���)������ݵ����ڵ�(����˸��ڵ���Ȼ���Һ��ӣ�˵���Ѿ����ʹ����������ݣ�ֱ��Ϊ����)
	template<typename T>
	BinNodePosi(T) BinNode<T>::succ() {
		BinNodePosi(T) s = this;
		if (rChild) {
			s = rChild;
			while (HasLChild(*s)) s = s->lChild;
		}
		else {
			while (IsRChild(*s)) s = s->parent;
			s = s->parent; // ��������Ϸ��ƶ�һ�����ִ�ֱ�Ӻ��
		}
		return s;
	}


//����

//����    
	template<typename T>
	template<typename VST> 
	void BinNode<T>::travPre(VST&visit) {
		switch (rand() % 3) {
		case 1: travPre_I1(this, visit); break;
		case 2: travPre_I2(this, visit); break;
		default: travPre_R(this, visit); break;
		}		
	}


		// recursion
	template<typename T>
	template<typename VST>
	void BinNode<T>::travPre_R(BinNodePosi(T) x, VST& visit) {
		if (!x) return;
		visit(x->data);
		travPre_R(x->lChild, visit);
		travPre_R(x->rChild, visit);
	}

		// version 1
	template<typename T>
	template<typename VST>
	void  BinNode<T>::travPre_I1(BinNodePosi(T) x, VST& visit) {
		stack <BinNodePosi(T)> S;
		if (x) S.push(x);
		while (!S.empty()) {
			x = S.pop(); visit(x->data);
			if (HasRChild(*x)) S.push(x->rChild); //��ѹ���Ҳ࣬������
			if (HasLChild(*x)) S.push(x->lChild);
		}
	}

		// version 2
	template<typename T, typename VST>
	static void visitAlongLeftBranch(BinNodePosi(T) x, VST& visit, stack <BinNodePosi(T)>& S) {
		while (x) {
			visit(x->data);
			S.push(x->rChild);
			x = x->lChild;
		}
	}

	template<typename T>
	template<typename VST>
	void BinNode<T>::travPre_I2(BinNodePosi(T) x, VST& visit) {
		stack<BinNodePosi(T)> S;
		while (true) {
			visitAlongLeftBranch(x, visit, S);
			if (S.empty()) break;
			x = S.pop();
		}
	}



// ����
	template<typename T> 
	template <typename VST>
	void BinNode<T> ::travIn(VST& visit) {
		switch (rand() % 5) {
		case 1: travIn_I1(this, visit); break;
		case 2: travIn_I2(this, visit); break;
		case 3: travIn_I3(this, visit); break;
		case 4: travIn_I4(this, visit); break;
		default: travIn_R(this, visit); break;
		}
	}

	//  recursion 
	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_R(BinNodePosi(T) x, VST& visit) {
		if (!x) return;
		travIn_R(x->lChild, visit);
		visit(x->data);
		travIn_R(x->rChild, visit);
	}

	// version1
	template<typename T>
	static void goAlongLeftBranch(BinNodePosi(T) x, stack<BinNodePosi(T)>& S) {
		while (x) {
			S.push(x); x = x->lChild;
		}
	}

	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_I1(BinNodePosi(T) x, VST& visit) {
		stack<BinNodePosi(T)> S;
		while (true) {
			goAlongLeftBranch(x, S);
			if (S.empty()) break;
			x = S.pop();
			visit(x->data);
			x = x->rChild;
		}
	}


	// version 2
	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_I2(BinNodePosi(T) x, VST& visit) {
		stack<BinNodePosi(T)> S;
		while (true) {
			if (x) {
				S.push(x);
				x = x->lChild;
			}
			else if (!S.empty()) {
				x = S.pop();
				visit(x->data);
				x = x->rChild;
			}
			else
				break;
		}
	}

	// version 3 �޸���ջ
	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_I3(BinNodePosi(T) x, VST& visit) {
		bool backtrack = false; //ǰһ���Ƿ�����������ݣ�ʡȥջ
		while (true)
			if (!backtrack && HasLChild(*x)) //�����������Ҳ��Ǹոջ��ݣ���
				x = x->lChild;
			else { //��������������ոջ��ݣ��൱������������
				visit(x->data);
				if (HasRChild(*x)) {
					x = x->rChild; //������������������
					backtrack = false; // �رջ��ݱ�־
				}
				else {// ��������Ϊ�գ���
					if (!(x = x->succ())) break; //����(���ִ�ĩ�ڵ�ʱ���˳�����)
					backtrack = true; //�����û��ݱ�־
				}
			}
	}

	//version 4, �޸���ջ���ޱ�־λ
	template<typename T>
	template<typename VST>
	void BinNode<T>::travIn_I4(BinNodePosi(T) x, VST& visit) {
		while (true) {
			if (HasLChild(*x))
				x = x->lChild;
			else {
				visit(x->data);
				while (!HasRChild(*x))
					if (!(x = x->succ())) return;
					else visit(x->data);
				x = x->rChild;
			}
		}
	}




// ����

	template<typename T>
	template<typename VST> 
	void BinNode<T>::travPost(VST& visit) {
		switch (rand() % 2) {
		case 1: travPost_I(this, visit);
		default: travPost_R(this, visit);
		}
	}


	// recursion
	template<typename T>
	template<typename VST>
	void BinNode<T>::travPost_R(BinNodePosi(T) x, VST& visit) {
		if (!x) return;
		travPost_R(x->lChild, visit);
		travPost_R(x->rChild, visit);
		visit(x->data);
	}

	// �������������, ����������ʵ��û��ڵ�Ļ���������
	template<typename T> // ����Sջ���ڵ�Ϊ���������У��ҵ�������ɼ�Ҷ�ڵ�
	static void gotoHLVFL(stack <BinNodePosi(T)>& S) {  // ��;�����ڵ�������ջ
		while (BinNodePosi(T) x = S.top()) {
			if (HasLChild(*x)) {//����������
				if (HasRChild(*x)) S.push(x->rChild); // �����Һ��ӣ�������ջ
				S.push(x->lChild); //Ȼ���ת������
			}
			else
				S.push(x->rChild);
		}
		S.pop();
	}

	
	template<typename T>
	template<typename VST>
	void BinNode<T>::travPost_I(BinNodePosi(T) x, VST& visit) {
		stack<BinNodePosi(T)> S; //����ջ
		if (x) S.push(x);
		while (!S.empty()) {
			if (S.top() != x->parent)//��ջ���ǵ�ǰ�ڵ�֮��(���Ϊ������)�� ��ʱ��Ҫ
				gotoHLVFL(S); // ����������Ϊ���������У��ҵ�HLVFL
			x = S.pop(); visit(x->data); //����ջ��(��ǰһ���ڵ�ĺ��)�� ������֮
		}
	}


// ��α��� level
	template<typename T>
	template<typename VST>
	void BinNode<T>::travLevel(VST& visit) {//��α����㷨
		queue<BinNodePosi(T)> Q;
		Q.enqueue(this);
		while (!Q.empty()) {
			BinNodePosi(T) x = Q.dequeue(); visit(x->data);
			if (HasLChild(*x)) Q.enqueue(x->lChild);
			if (HasRChild(*x)) Q.enqueue(x->rChild);
		}
	}
};