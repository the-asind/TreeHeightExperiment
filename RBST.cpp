//
// Created by kashaev.2021 on 16.05.2024.
//
using namespace std;
#include <utility>
#include <iostream>


template<class M>
struct RBST {
    using T = typename M::T;
    using E = typename M::E;

    struct node {
        node *l, *r, *p;
        T val;
        E lazy;
        bool rev;
        int sz;

        node(T v, E p) : l(nullptr),r(nullptr),val(v),lazy(p),rev(false),sz(1) {}
    };

    int size(node* a) { return !a ? 0 : a->sz; }
    T val(node* a) { return !a ? M::dt() : (eval(a), a->val); }
    node* fix(node* a) {
        a->sz = size(a->l) + 1 + size(a->r);
        a->val = M::f(val(a->l), a->val, val(a->r));
        return a;
    }
    void eval(node* a) {
        if(a->lazy != M::de()) {
            a->val = M::g(a->val, a->lazy, size(a));
            if(a->l) a->l->lazy = M::h(a->l->lazy, a->lazy);
            if(a->r) a->r->lazy = M::h(a->r->lazy, a->lazy);
            a->lazy = M::de();
        }
        if(a->rev) {
            swap(a->l, a->r);
            if(a->l) a->l->rev ^= 1;
            if(a->r) a->r->rev ^= 1;
            a->rev = false;
        }
    }

    inline int xor128() {
        static int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
        int t;
        t = x ^ (x << 11); x = y; y = z; z = w;
        return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
    }

    node* merge(node *a, node *b) {
        if(!a) return b;
        if(!b) return a;
        eval(a); eval(b);
        if(xor128() % (size(a) + size(b)) < size(a)) {
            a->r = merge(a->r, b);
            if(a->r) a->r->p = a;
            return fix(a);
        } else {
            b->l = merge(a, b->l);
            if(b->l) b->l->p = b;
            return fix(b);
        }
    }
    // [0,k) [k,n)
    std::pair<node*, node*> split(node *a, int k) {
        if(!a) return pair<node*, node*>(nullptr, nullptr);
        eval(a);
        node *sl, *sr;
        if(k <= size(a->l)) {
            tie(sl, sr) = split(a->l, k);
            if(a->l) a->l->p = nullptr;
            a->l = sr;
            if(a->l) a->l->p = a;
            return pair<node*, node*>(sl, fix(a));
        }
        tie(sl, sr) = split(a->r, k - size(a->l) - 1);
        if(a->r) a->r->p = nullptr;
        a->r = sl;
        if(a->r) a->r->p = a;
        return pair<node*, node*>(fix(a), sr);
    }
    // Insert/delete elements
    void insert(node*& a, int k, const T& x) {
        node *sl, *sr;
        tie(sl, sr) = split(a, k);
        a = merge(sl, merge(new node(x, M::de()), sr));
    }
    T erase(node*& a, int k) {
        node *sl, *sr, *tl, *tr;
        tie(sl, sr) = split(a, k + 1);
        tie(tl, tr) = split(sl, k);
        a = merge(tl, sr);
        return val(tr);
    }
    // check checkpoint update
    void update(node*& a, int k, const E& x) {
        node *sl, *sr, *tl, *tr;
        tie(sl, sr) = split(a, k + 1);
        tie(tl, tr) = split(sl, k);
        if(tr) tr->val = M::g(tr->val, x, size(tr));
        a = merge(merge(tl, tr), sr);
    }
    // interval update
    void update(node*& a, int l, int r, const E& m) {
        node *sl, *sr, *tl, *tr;
        tie(sl, sr) = split(a, r);
        tie(tl, tr) = split(sl, l);
        if(tr) tr->lazy = M::h(tr->lazy, m);
        a = merge(merge(tl, tr), sr);
    }
    // point of contact (math.)
    T get(node*& a, int k) {
        node *sl, *sr, *tl, *tr;
        tie(sl, sr) = split(a, k + 1);
        tie(tl, tr) = split(sl, k);
        T res = !tr ? M::dt() : tr->val;
        a = merge(merge(tl, tr), sr);
        return res;
    }
    // section query
    T query(node*& a, int l, int r) {
        node *sl, *sr, *tl, *tr;
        tie(sl, sr) = split(a, r);
        tie(tl, tr) = split(sl, l);
        T res = !tr ? M::dt() : tr->val;
        a = merge(merge(tl, tr), sr);
        return res;
    }
    // Inversion of interval [l,r)
    void reverse(node*& a, int l, int r) {
        node *sl, *sr, *tl, *tr;
        tie(sl, sr) = split(a, r);
        tie(tl, tr) = split(sl, l);
        if(tr) tr->rev ^= 1;
        a = merge(merge(tl, tr), sr);
    }
    // Find the root of the tree to which vertex a belongs
    node* getroot(node *a) {
        if(!a->p) return a;
        return getroot(a->p);
    }
    // Minimum position over x
    int lower_bound(node *t, const T &x) {
        if(!t) return 0;
        if(x <= val(t)) return lower_bound(t->l, x);
        return lower_bound(t->r, x) + size(t->l) + 1;
    }
    // Minimum position greater than x
    int upper_bound(node *t, const T &x) {
        if(!t) return 0;
        if(x < val(t)) return upper_bound(t->l, x);
        return upper_bound(t->r, x) + RBST<T>::size(t->l) + 1;
    }
    // Number of x
    int count(node *t, const T &x) {
        return upper_bound(t, x) - lower_bound(t, x);
    }
    // Find the kth element
    T kth_element(node *t, int k) {
        if(k < RBST<T>::size(t->l)) return kth_element(t->l, k);
        if(k == RBST<T>::size(t->l)) return val(t);
        return kth_element(t->r, k - RBST<T>::size(t->l) - 1);
    }
    // Add element x
    void insert_key(node *&t, const T &x) {
        RBST<T>::insert(t, lower_bound(t, x), x);
    }
    // Erase element x
    void erase_key(node *&t, const T &x) {
        if(!count(t, x)) return;
        RBST<T>::erase(t, lower_bound(t, x));
    }
    // For debugging
    void debug(node* t) {
        if(t == nullptr) return;
        cout << "{";
        debug(t->l);
        cout << " " << t->val << " ";
        debug(t->r);
        cout << "}";
    }
};
