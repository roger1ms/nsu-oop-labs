#ifndef STRATEGY_H
#define STRATEGY_H

#include <string>
#include <vector>

class Strategy {
protected:
    std::vector<char> myMoves;
    std::vector<char> opponent1Moves;
    std::vector<char> opponent2Moves;
    std::vector<int> totalScores;
    int lastScore;
    
public:
    Strategy() : totalScores{0,0,0}, lastScore(0){}

    virtual char makeMove() = 0;
    virtual std::string getName() const = 0;
    
    void addToHistory(char myMove, char opp1Move, char opp2Move) {
        myMoves.push_back(myMove);
        opponent1Moves.push_back(opp1Move);
        opponent2Moves.push_back(opp2Move);
        
        int myScore = 0, score1 = 0, score2 = 0;
        
        if (myMove == 'C' && opp1Move == 'C' && opp2Move == 'C') {
            myScore = score1 = score2 = 7;
        }
        else if (myMove == 'C' && opp1Move == 'C' && opp2Move == 'D') {
            myScore = 3; score1 = 3; score2 = 9;
        }
        else if (myMove == 'C' && opp1Move == 'D' && opp2Move == 'C') {
            myScore = 3; score1 = 9; score2 = 3;
        }
        else if (myMove == 'D' && opp1Move == 'C' && opp2Move == 'C') {
            myScore = 9; score1 = 3; score2 = 3;
        }
        else if (myMove == 'C' && opp1Move == 'D' && opp2Move == 'D') {
            myScore = 0; score1 = 5; score2 = 5;
        }
        else if (myMove == 'D' && opp1Move == 'C' && opp2Move == 'D') {
            myScore = 5; score1 = 0; score2 = 5;
        }
        else if (myMove == 'D' && opp1Move == 'D' && opp2Move == 'C') {
            myScore = 5; score1 = 5; score2 = 0;
        }
        else if (myMove == 'D' && opp1Move == 'D' && opp2Move == 'D') {
            myScore = score1 = score2 = 1;
        }
        lastScore = myScore;
        totalScores[0] += myScore;
        totalScores[1] += score1;
        totalScores[2] += score2;
    }
    
    virtual void copyHistoryFrom(const Strategy& other) {
        myMoves = other.myMoves;
        opponent1Moves = other.opponent1Moves;
        opponent2Moves = other.opponent2Moves;
        totalScores = other.totalScores;
        lastScore = other.lastScore;
    }
    
    void clearHistory() {
        myMoves.clear();
        opponent1Moves.clear();
        opponent2Moves.clear();
        totalScores = {0, 0, 0}; 
    }
    
    std::vector<int> getScores() const {
        return totalScores;
    }
};

#endif