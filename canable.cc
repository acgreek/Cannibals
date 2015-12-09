#include <iostream>
#include <string>
#include <map>
#include <list>
#include <algorithm>

class Side {
    public:
        Side(int pm, int pc, bool pboat): m(pm), c(pc), boat(pboat){ }
        bool dead() const{
            return m > 0 && c > m;
        }
        bool cannotrow()const {
            return boat == false ? true : (m + c)==  0;
        }
        void print() const{
            std::cout <<"(" << m << "," << c << "," << boat << ")";
        }
        void moveTo(Side & other, int pc, int pm) {
            c -= pc;
            m -= pm;
            boat = false;
            other.boat = true;
            other.c+=pc;
            other.m+= pm;
        }
        int canables() const { return c;}
        int missionaries() const { return m;}
        int hasBoat()const  { return boat;}
        bool operator==(const Side & b)const {
             return canables() == b.canables() && missionaries() == b.missionaries() && hasBoat() == b.hasBoat();
        }
    private:
        int m;
        int c;
        bool boat;
};

class World {
    public:
        World():left(3,3,true), right(0,0,false) { } ;
        bool done()const  {
            return right.missionaries() + right.canables() == 6;
        }
        bool dead() const{
            return right.dead() || left.dead();
        }
        bool cannotrow()const {
            return right.cannotrow() &&  left.cannotrow();
        }
        void print() const{
            left.print();
            std::cout <<" ";
            right.print() ;
            std::cout <<std::endl;
        }
        World move(int c, int m) const {
            World nw = *this;
            if (nw.left.hasBoat()) {
                nw.left.moveTo(nw.right, c,m);
            }
            else {
                nw.right.moveTo(nw.left,c,m);
            }
            return nw;
        }
        bool operator==(const World & b) {
            return left == b.left && right == b.right;
        }
        bool solve(std::list<World> &history) const {
            if (std::find(history.begin(), history.end(), *this)  != history.end())
                return false;
            history.push_back(*this);
            if (done()) {
                print();
                return true;
            }
            if (dead() || cannotrow())
                return false;
            if (solveSide(left.hasBoat() ? left : right,history)) {
                print();
                return true;
            }
            return false;
        }
    private:
        bool solveSide(const Side & side,std::list<World> &history) const {
            return ((side.canables() > 1 && move(2,0) .solve( history)) ||
                    (side.canables() > 0 && side.missionaries() > 0 && move(1,1).solve( history)) ||
                    (side.missionaries() > 1 && move(0,2).solve( history)) ||
                    (side.missionaries() > 0 && move(0,1).solve(history))||
                    (side.canables() > 0 && move(1,0).solve( history)) );

        }
        Side left;
        Side right;
};

int main () {
    std::list<World> history;
    World w;
    std::cout << "found a solution " << w. solve(history) << std::endl;
    return 0;
}
