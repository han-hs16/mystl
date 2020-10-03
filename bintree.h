#pragma once

#include"queue.h"
#include"stack.h"
#include"binnode.h"
#include"util.h"

namespace mystl {

	template<typename T>
	class BinTree {
	protected:
		int _size;
		BinNodePosi(T) _root;
		virtual int updateHeight(BinNodePosi(T) x);
		void updateHeightAbove(BinNodePosi(T) x);
	public:
		BinTree():_size(0), _root(nullptr) {};
		~BinTree() { if (0 < _size) remove(_root); }
		int size() const { return _size; }
		bool empty() const { return !_root; }
		int remove(BinNodePosi(T) x);
		BinNodePosi(T) root() const { return _root; }
		BinNodePosi(T) insertAsRoot(T const& e);
		BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); // e��Ϊx�����Ӳ���(��Ϊxԭ��û������)
		BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e);
		BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>*& T); // T ��Ϊx������������
		BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>*& T);
		BinTree<T>* secede(BinNodePosi(T) x); //�������ӵ�ǰ����ժ����������ת��Ϊһ�Ŷ�������

		template<typename VST>
		void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } //�������
		template<typename VST>
		void travPre(VST& visit) { if (_root) _root->travPre(visit); } // �������
		template<typename VST>
		void travIn(VST& visit) { if (_root) _root ->travIn(visit); }
		template<typename VST>
		void travPost(VST& visit) { if (_root) _root->travPost(visit); } // �������

		// �Ƚ���
		bool operator<  (BinTree<T>const& t) { return _root && t._root && lt(_root, t._root); }
		bool operator== (BinTree<T>const& t) { return _root && t._root && (_root == t._root); }

	};



	template<typename T>
	int BinTree<T>::updateHeight(BinNodePosi(T) x) {
		return x->height = 1 + max(stature(x->lChild), stature(x->rChild));
	}

	template<typename T>
	void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
		while (x) {
			updateHeight(x);
			x = x->parent;
		}
	}

	template<typename T>
	BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e) {
		_size = 1;
		return _root = new BinNode<T>(e);
	}

	template<typename T>
	BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e) {
		_size++;
		x->insertAsLC(e);
		updateHeightAbove(x);
		return x->lChild;
	}

	template<typename T>
	BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e) {
		_size++;
		x->insertAsRC(e);
		updateHeightAbove(x);
		return x->rChild;
	}


	// ��������
	template<typename T>
	BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>*& S) {
		if (x->lChild = S->_root) x->lChild->parent = x; // ����
		_size += S->_size; updateHeightAbove(x); //���¹�ģ���������ȵĸ߶�
		S->_root = nullptr; S->_size = 0; release(S); S = nullptr; return x;
	}

	template<typename T>
	BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>*& S) {
		if (x->rChild = S->_root) x->rChild->parent = x;
		_size += S->_size; updateHeightAbove(x);
		S->_root = nullptr; S->_size = 0; release(S); S = nullptr; return x;
	}

	//����ɾ��
	template <typename T> //ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����ֵ
	int BinTree<T>::remove(BinNodePosi(T) x) { //assert: xΪ�������еĺϷ�λ��
		FromParentTo(*x) = nullptr; //�ж����Ը��ڵ��ָ��
		updateHeightAbove(x->parent); //�������ȸ߶�
		int n = removeAt(x); _size -= n; return n; //ɾ������x�����¹�ģ������ɾ���ڵ�����
	}

	template <typename T> //ɾ����������λ��x���Ľڵ㼰���������ر�ɾ���ڵ����ֵ
	static int removeAt(BinNodePosi(T) x) { //assert: xΪ�������еĺϷ�λ��
		if (!x) return 0; //�ݹ��������
		int n = 1 + removeAt(x->lChild) + removeAt(x->rChild); //�ݹ��ͷ���������
		release(x->data); release(x); return n; //�ͷű�ժ���ڵ㣬������ɾ���ڵ�����
	}

	// ��������
	template<typename T> // ���������������㷨�� ������x�ӵ�ǰ��ժ�����������װΪһ�Ŷ�������
	BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x) {
		FromParentTo(*x) = nullptr;
		updateHeightAbove(x->parent);
		BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = nullptr; // ������xΪ��
		S->_size = x->size(); _size -= S->_size; return S;
	}


};