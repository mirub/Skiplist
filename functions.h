// Copyright [2019] <Banu Miruna Elena 311CA>

#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <iostream>
#include <utility>
#include <random>
#define maxLvl 10


template <typename T>
struct Player {
    int id;
    T points;
    int prev_pos;
    int print_dif;
    int prev_print;
    Player<T> *prev;
    Player<T> *next;
    Player<T> *up;
    Player<T> *down;

    Player() {
        this->id = 0;
        this->points = 0;
        this->prev_pos = 0;
        this->print_dif = 0;
        prev = nullptr;
        next = nullptr;
        up = nullptr;
        down = nullptr;
    }

    Player(int id, T points, int prev_pos, int print_dif, int prev_print) {
	    this->id = id;
	    this->points = points;
	    this->prev_pos = prev_pos;
	    this->print_dif = print_dif;
	    this->prev_print = prev_print;
	    prev = nullptr;
	    next = nullptr;
	    up = nullptr;
	    down = nullptr;
	}
};

template <typename T>
class SkipList {
 private:
    Player<T> *head;
    Player<T> *tail;
    int levels;
    int players;

 public:
    SkipList() {
        this->head = new struct Player<T>(0, (int)INT_MIN, 0, 0, 0);
        this->tail = new struct Player<T>(0, (int)INT_MAX, 0, 0, 0);
        head->next = tail;
        tail->prev = head;
        levels = 1;
        players = 0;
    }

    explicit SkipList(int players) {
        this->players = players;
        this->head = new struct Player<T>(0, (int)INT_MIN, 0, 0, 0);
        this->tail = new struct Player<T>(0, (int)INT_MAX, 0, 0, 0);
        head->next = tail;
        tail->prev = head;
        levels = 1;
    }

    ~SkipList() {
        Player<T> *auxHead;
        auxHead = this->head;
        while (auxHead != nullptr) {
            auxHead = auxHead->down;
            while (head != nullptr) {
                Player<T> *aux = head;
                head = head->next;
                delete aux;
            }
            head = auxHead;
        }
        while (head != nullptr) {
            Player<T> *aux = head;
            head = head->next;
            delete aux;
        }
        delete auxHead;
    }

    int coinFlip() {
    	return rand()%2;
    }

    bool isEmpty() {
        if (levels == 1 && head == tail) {
            return true;
        }
        return false;
    }

    SkipList(const SkipList& s) {
    	Player<T> *p = s.head;
        Player<T> *q = s.tail;
        int k = s.levels;
        while (k > 1) {
            p = p->down;
            q = q->down;
            k--;
        }
        q = q->prev;
        while (p != q) {
            this->insertPlayerbyPoints(q->id, q->points,
            	q->prev_pos, q->print_dif, q->prev_print);
            q = q->prev;
        }
    }

    Player<T> *searchPlayerbyPoints(T val) const {
        Player<T> *p = head;
        Player<T> *t = tail;
        while (p->down != nullptr) {
            while (val >= p->next->points && p->next != t) {
                p = p->next;
            }
            p = p->down;
            t = t->down;
        }
        while (val != p->points && p->next != t) {
            p = p->next;
        }
        return p;
    }

    Player<T> *searchPlayerbyID(int val) {
        Player<T> *p = head;
        Player<T> *t = tail;
        while (p->down != nullptr) {
            while (val >= p->next->id && p->next != t) {
                p = p->next;
            }
            p = p->down;
            t = t->down;
        }
        while (val != p->id && p->next != t) {
            p = p->next;
        }
        return p;
    }

    Player<T> *searchPrevPlayerbyPoints(T val) {
        Player<T> *p = head;
        Player<T> *t = tail;
        while (p->down != nullptr) {
            while (p->next != t && val >= p->next->points) {
                p = p->next;
            }
            p = p->down;
            t = t->down;
        }
        while (val >= p->next->points && p->next != t) {
            p = p->next;
        }
        return p;
    }

    void addLevel() {
    	struct Player<T>* headUp = new struct Player<T>(0, (int)INT_MIN, 0, 0, 0);
    	struct Player<T>* tailUp = new struct Player<T>(0, (int)INT_MAX, 0, 0, 0);
    	head->up = headUp;
    	headUp->down = head;
    	head = headUp;
    	tail->up = tailUp;
    	tailUp->down = tail;
    	tail = tailUp;
        head->next = tail;
        tail->prev = head;
    	levels++;
    }

    Player<T> *insertAfterpByPoints(int id, T points, int prev_pos,
    int print_dif, int prev_print, Player<T> *p) {
        struct Player<T>* Player = new struct Player<T>;
        Player->points = points;
        Player->id = id;
        Player->prev_pos = prev_pos;
        Player->print_dif = print_dif;
        Player->prev_print = prev_print;
        Player->next = p->next;
        p->next->prev = Player;
        Player->prev = p;
        p->next = Player;
        return Player;
    }

    void insertPlayerbyPoints(int id, T points, int prev_pos, int print_dif,
    int prev_print) {
        Player<T> *p = searchPrevPlayerbyPoints(points);
        Player<T> *q = insertAfterpByPoints(id, points,
        	prev_pos, print_dif, prev_print, p);
        int i = 1;
        while (coinFlip() && i < maxLvl) {
            i++;
            if (i > levels) {
                addLevel();
            }
            while (p->up == nullptr) {
                p = p->prev;
            }
            p = p->up;
            Player<T> *aux = new struct Player<T>;
            Player<T> *m = p->next;
            aux->points = points;
            aux->id = id;
            aux->prev_pos = prev_pos;
            m->prev = aux;
            p->next = aux;
            aux->prev = p;
            aux->next = m;
            aux->down = q;
            q->up = aux;
            q = aux;
        }
    }

    void deletePlayerbyPoints(T val) {
        Player<T> *p = searchPlayerbyPoints(val);
        Player<T> *aux;
        while (p != nullptr) {
            aux = p;
            aux->prev->next = aux->next;
            aux->next->prev = aux->prev;
            p = p->up;
            delete aux;
        }
    }

     void printListbyPoints() {
        Player<T> *auxHead;
        Player<T> *auxTail;
        Player<T> *aux = this->head;
        auxHead = this->head;
        auxTail = this->tail;
        int k = levels;
        for (int i = 0; i < levels; ++i) {
            std::cout << "Nivelul " << k << " :";
            aux = aux->next;
            while (aux != auxTail) {
                std::cout << aux->points << " ";
                aux = aux->next;
            }
            auxHead = auxHead->down;
            aux = auxHead;
            auxTail = auxTail->down;
            k--;
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    SkipList<T>& operator=(const SkipList<T>& d) {
         if (!this->isEmpty()) {
            Player<T> *auxHead;
            auxHead = this->head;
            while (auxHead != nullptr) {
                auxHead = auxHead->down;
                while (head != nullptr) {
                   Player<T> *aux = head;
                    head = head->next;
                    delete aux;
                }
            head = auxHead;
            }
            while (head != nullptr) {
                Player<T> *aux = head;
                head = head->next;
                delete aux;
            }
            delete auxHead;
            this->head = new struct Player<T>(0, (int)INT_MIN, 0, 0, 0);
            this->tail = new struct Player<T>(0, (int)INT_MAX, 0, 0, 0);
            head->next = tail;
            tail->prev = head;
            levels = 1;
            players = 0;
        }
        Player<T> *p = d.head;
        Player<T> *q = d.tail;
        int k = d.levels;
        while (k > 1) {
            p = p->down;
            q = q->down;
            k--;
        }
        q = q->prev;
        while (p != q) {
            this->insertPlayerbyPoints(q->id, q->points,
            	q->prev_pos, q->print_dif, q->prev_print);
            q = q->prev;
        }
        return *this;
    }

    void PrintPlayers(std::ostream& g) {
        Player<T> *p, *q;
        p = head;
        q = tail;
        int k = levels;
        while (k > 1) {
            p = p->down;
            q = q->down;
            k--;
        }
        q = q->prev;
        while (p != q) {
            g << q->id << " " << q->points << " " << q->print_dif << '\n';
            q = q->prev;
        }
    }

    void emptyList() {
        Player<T> *auxHead;
        auxHead = this->head;
        while (auxHead != nullptr) {
            auxHead = auxHead->down;
            while (head != nullptr) {
               Player<T> *aux = head;
               head = head->next;
               delete aux;
            }
        	head = auxHead;
        }
        while (head != nullptr) {
            Player<T> *aux = head;
            head = head->next;
            delete aux;
        }
        delete auxHead;
        this->head = new struct Player<T>(0, (int)INT_MIN, 0, 0, 0);
        this->tail = new struct Player<T>(0, (int)INT_MAX, 0, 0, 0);
        head->next = tail;
        tail->prev = head;
        levels = 1;
        players = 0;
    }
};

template <typename T>
void receive_points(int *t, SkipList<T> &p, int n, int ok,
	std::ostream& g, int print) {
    Player<T> *q;
    int *id = new int[n];
    int racers = 0;
    for (int i = 0; i < n; ++i) {
        id[i] = i + 1;
        if (t[i] != 0) {
        // calculates the number of participants in the current race
            racers++;
        }
    }
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (t[i] > t[j]) {
                std::swap(t[i], t[j]);
                // swaps id accodring to the time standings
                std::swap(id[i], id[j]);
            }
            if (t[i] == t[j] && t[i] != 0 && t[j] != 0) {
                Player<T> *q = p.searchPlayerbyID(id[i]);
                Player<T> *m = p.searchPlayerbyID(id[j]);
                if (q->prev_pos > m->prev_pos && q->prev_pos != 0
                && m->prev_pos != 0) {
                    std::swap(t[i], t[j]);
                    std::swap(id[i], id[j]);
                }
                if (q->prev_pos == m->prev_pos && id[i] > id[j]
                	&& q->prev_pos == 0 && m->prev_pos == 0) {
                	// if the current players compared do not have
                	// a previous ranking
                    std::swap(t[i], t[j]);
                    std::swap(id[i], id[j]);
                }
            }
        }
    }
    int points = racers/2;
    int i = 0;
    // score is an array where the score after the current
    // race is calculated
    T *score = new T[n];
    while (i < n && t[i] == 0) {
        q = p.searchPlayerbyID(id[i]);
        score[i] = q->points;
        i++;
    }
    for (int j = i; j < n; ++j) {
        if (points == 0 && racers % 2 == 0) {
            points--;
        }
        q = p.searchPlayerbyID(id[j]);
        q->points += points;
        score[j] = q->points;
        points--;
    }
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (score[i] > score[j]) {
                std::swap(score[i], score[j]);
                std::swap(id[i], id[j]);
            }
            if (score[i] == score[j] && id[i] < id[j]) {
            	// sorts descending by id for print
                std::swap(id[i], id[j]);
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        q = p.searchPlayerbyID(id[i]);
        // ok is 1 if the current input file line
        // requires a print
        if (ok) {
            if (print != 0) {
            	// calculates the difference between the previous
            	// print position and the current one
                q->print_dif = q->prev_print - (n - i);
            }
            q->prev_print = n - i;
        }
        q->prev_pos = n - i;
    }
    if (ok) {
        SkipList<T> sorted(n);
        sorted = p;
        sorted.PrintPlayers(g);
        g << "\n";
    }
    delete [] score;
    delete [] id;
}

#endif  // _FUNCTIONS_H_
