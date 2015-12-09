#include <iostream>
#include <string>
#include <map>
#include <list>
#include <algorithm>

struct Side {
    bool dead() {
        return m > 0 && c > m;
    }
    bool cannotrow() {
        return boat == false ? true : (m + c)==  0;
    }
    void print() {
        std::cout <<"(" << m << "," << c << "," << boat << ")";
    }

    int m;
    int c;
    bool boat;
};
struct World {
    World():left(), right() {
        left.m =3;
        left.c =3;
        left.boat = true;
        right.m =0;
        right.c =0;
        right.boat = false;
    } ;
    Side left;
    Side right;
    bool done() {
        return right.m + right.c == 6;
    }
    bool dead() {
        return right.dead() || left.dead();
    }
    bool cannotrow() {
        return right.cannotrow() &&  left.cannotrow();
    }
    void print() {
        left.print();
        std::cout <<" ";
        right.print() ;
        std::cout <<std::endl;
    }
};
bool operator==(const Side & a, const Side & b) {
    return a.c == b.c && a.m == b.m && a.boat == b.boat;
}
bool operator==(const World & a, const World & b) {
    return a.left == b.left && a.right == b.right;
}

World move(World w, int c, int m) {
    if (w.left.boat) {
        w.left.c -= c;
        w.left.m -= m;
        w.left.boat = false;
        w.right.boat = true;
        w.right.c+=c;
        w.right.m+= m;
    }
    else {
        w.right.c-= c;
        w.right.m -= m;
        w.right.boat = false;
        w.left.boat = true;
        w.left.c+=c;
        w.left.m+= m;
    }
    return w;
}

bool solve(World w, std::list<World> &history) {
    if (std::find(history.begin(), history.end(), w)  != history.end())
        return false;
    history.push_back(w);
    if (w.done()) {
        w.print();
        return true;
    }
    if (w.dead() || w.cannotrow())
        return false;
    if (w.left.boat) {
        if ((w.left.c > 1 && solve(move(w,2,0) , history)) ||
            (w.left.c > 0 && w.left.m > 0 && solve(move(w,1,1) , history)) ||
            (w.left.m > 1 && solve(move(w,0,2) , history)) ||
            (w.left.m > 0 && solve(move(w,0,1) , history)) ||
            (w.left.c > 0 && solve(move(w,1,0) , history))
            ) {
            w.print();
            return true;
        }
    } else {
        if ((w.right.c > 1 && solve(move(w,2,0) , history)) ||
            (w.right.c > 0 && w.right.m > 0 && solve(move(w,1,1) , history)) ||
            (w.right.m > 1 && solve(move(w,0,2) , history)) ||
            (w.right.m > 0 && solve(move(w,0,1) , history))||
            (w.right.c > 0 && solve(move(w,1,0) , history)) ) {
            w.print();
            return true;
        }
    }
    return false;
}

int main () {
    std::list<World> history;
    World w;
    std::cout << "found a solution " << solve(w, history) << std::endl;
    return 0;
}
