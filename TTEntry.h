#pragma once

#include "board.hpp"
#include <unordered_map>


class TTEntry {

    int _value;
    int _depth;
    bool _lower;
    bool _exact;
    bool _upper;
    // Board _best;
    int _best;

public:
    TTEntry(int value, unsigned int depth, bool lower, bool exact, bool upper, int best):  // Board best):
    _value(value),
    _depth(depth),
    _lower(lower),
    _exact(exact),
    _upper(upper),
    _best(best) {}

    TTEntry() {}

    bool empty() {
        return false;
    }
    // bool operator== (const TTEntry &entry) const{
    // 	return getCurrent() == entry.getCurrent();
    // }
    bool isValid() const{

        return _depth!=0;
    }
    /*
    Board getBest() const{
        return _best;
    }
    */
    int getBest() const{
        return _best;
    }
    /*
    Board getCurrent() const{
        return Board();
    }
    bool operator== (const Board &board) const{
        return getCurrent() == board;
    }
    */
    int getValue() const{
        return _value;
    }
    unsigned int getDepth() const{
        return _depth;
    }
    bool isLower() const{
        return _lower;
    }
    bool isExact() const{
        return _exact;
    }
    bool isUpper() const{
        return _upper;
    }
    friend class TranspositionTable;
};

class TranspositionTable {



public:
    TranspositionTable() {
        _entries.reserve(10000000);
    }
    ~TranspositionTable() {}

    void clear() {
        //_entries.fill(TTEntry());
        _entries.clear();
    }
    int count(const Board& b) {
        return _entries.count(b);
        //return _entries[pos]==b?1:0;
    }

    void store(TTEntry entry, const Board& current){
        //stats.incTTstored();

        if(_entries.count(current) == 0 || _entries[current].getDepth() < entry.getDepth()){
            if(_entries.count(current) == 0){
                //stats.incTTuniq();
            }
            _entries[current] = entry;
        }
    }

    const TTEntry& get(const Board& board){
        return _entries[board];
    }

private:
    //Board::BoardHash hasher;
    std::unordered_map<Board, TTEntry, BoardHash> _entries;

};

extern TranspositionTable TT;
