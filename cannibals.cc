#include <iostream>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <stdlib.h>
#include "options.h"


class Side {
    public:
        Side(int m, int c, int n, bool boat): missionaries_(m), gorillas_(c),monkeys_(n), boat_(boat){ }
        bool dead() const{
            return missionaries_ > 0 && (gorillas_+monkeys_) > missionaries_;
        }
        bool cannotrow()const {
            return boat_ == false ? true : (missionaries_ + gorillas_)==  0;
        }
        void print() const{
            //std::cout <<"(missionaries=" << missionaries_ << ",aps=" << gorillas_ << ",monkeys=" <<  monkeys_ << ",has boat="<< (boat_? "true ":"false") << ")";
            std::cout <<"(";
            for(int i=0; i < missionaries_;i++)
                std::cout <<"M";
            for(int i=0; i < gorillas_;i++)
                std::cout <<"a";
            for(int i=0; i < monkeys_;i++)
                std::cout <<"m";
            std::cout << ")" << (boat_ ? "b" : "") << " ";
        }
        void moveTo(Side & other, int pc, int pm, int n) {
            gorillas_ -= pc;
            missionaries_ -= pm;
            monkeys_ -= n;
            boat_ = false;
            other.boat_ = true;
            other.gorillas_+=pc;
            other.missionaries_+= pm;
            other.monkeys_+= n;
        }
        int canables() const { return gorillas_+monkeys_;}
        int nonRowableCannibals() const { return monkeys_;}
        int rowableCannibals() const { return gorillas_;}
        int missionaries() const { return missionaries_;}
        int hasBoat()const  { return boat_;}
        bool operator==(const Side & b)const {
             return rowableCannibals() == b.rowableCannibals() && nonRowableCannibals() == b.nonRowableCannibals() && missionaries() == b.missionaries() && hasBoat() == b.hasBoat();
        }
    private:
        int missionaries_;
        int gorillas_;
        int monkeys_;
        bool boat_;
};

class World {
    public:
        World(int apes=1, int monkeys=2, int missionaries=3):left(missionaries,apes,monkeys,true), right(0,0,0,false) { } ;
        bool done()const  {
            return left.missionaries() + left.canables() ==0 ;
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
        World move(int c, int m, int n) const {
            World nw = *this;
            if (nw.left.hasBoat()) {
                nw.left.moveTo(nw.right, c,m, n);
            }
            else {
                nw.right.moveTo(nw.left,c,m, n);
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
            return ((side.rowableCannibals() > 1 && move(2,0, 0) .solve( history)) ||
                    (side.rowableCannibals() > 0 && side.missionaries() > 0 && move(1,1,0).solve( history)) ||
                    (side.nonRowableCannibals() > 0 && side.missionaries() > 0 && move(0,1,1).solve( history)) ||
                    (side.rowableCannibals() > 0 && side.nonRowableCannibals() > 0 && move(1,0,1).solve( history)) ||
                    (side.missionaries() > 1 && move(0,2,0).solve( history)) ||
                    (side.missionaries() > 0 && move(0,1,0).solve(history))||
                    (side.rowableCannibals() > 0 && move(1,0,0).solve( history)) );

        }
        Side left;
        Side right;
};

int main (int argc, char * argv[]) {
	Options opt;
	readOptions(opt, argc, argv);
    std::list<World> history;
    World w(opt.apes,opt.monkeys, opt.missionaries);
    std::cout << "found a solution " << w.solve(history) << std::endl;
    return 0;
}
