#include <iostream>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <stdlib.h>
#include <getopt.h>

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
            std::cout <<"(" << missionaries_ << "," << gorillas_ << "," <<  monkeys_ << ","<< boat_ << ")";
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

    int apes=1, missionaries=3, monkeys=2;
    while (1)  {
        static struct option long_options[] = {
            {"apes",     required_argument, 0,  0 },
            {"missionaries",  required_argument, 0,  0 },
            {"monkeys", required_argument,       0,  0 },
            {0,         0,                 0,  0 }
        };
        int option_index = 0;
        char c = getopt_long(argc, argv, "a:m:M:", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 0:
                switch(option_index ) {
                    case 0: apes = atoi(optarg);break;
                    case 1: missionaries = atoi(optarg);break;
                    case 2: monkeys= atoi(optarg);break;
                }
                break;
            case 'a': apes = atoi(optarg);break;
            case 'm': missionaries = atoi(optarg);break;
            case 'M': monkeys= atoi(optarg);break;
        }
    }
    std::list<World> history;
    World w(apes,monkeys, missionaries);
    std::cout << "found a solution " << w. solve(history) << std::endl;
    return 0;
}
