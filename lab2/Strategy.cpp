#include "Strategy.h"
#include <cstdlib>
#include <ctime>

class AllCStrategy : public Strategy {
public:
    std::string getName() const override {
        return "allc";
    } 
    char makeMove() override {
        return 'C';
    }
};

class AllDStrategy : public Strategy {
public:
    std::string getName() const override {
        return "alld";
    } 
    char makeMove() override {
        return 'D';
    }
};

class RandomStrategy : public Strategy {
public:
    RandomStrategy() {
        srand(time(0)); 
    }
    
    char makeMove() override {
        return (rand() % 2 == 0) ? 'C' : 'D';
    }
    
    std::string getName() const override {
        return "random";
    }
};

class RevengeStrategy : public Strategy {
public: 
    std::string getName() const override {
        return "revenge";
    }

    char makeMove() override {
        if (opponent1Moves.empty() && opponent2Moves.empty()) {
            return 'C';
        }
        else {
            char move1 = opponent1Moves.back(), move2 = opponent2Moves.back();
            return (move1 == 'C' && move2 == 'C') ? 'C' : 'D';
        }
    }

};

class ProvocationStrategy : public Strategy {
public: 
    std::string getName() const override {
        return "provocate";
    }

    char makeMove() override {
        if (opponent1Moves.empty() && opponent2Moves.empty()) {
            return 'D';
        }
        else {
            char move1 = opponent1Moves.back(), move2 = opponent2Moves.back();
            return (move1 == 'D' || move2 == 'D') ? 'C' : 'D';
        }
    }

};

class CycleAgressionStrategy : public Strategy {
private:
    int movesInMode = 0;
    int maxMovesInMode;
public: 
    std::string getName() const override {
        return "cycled";
    }

    char makeMove() override {
        if (movesInMode >= maxMovesInMode) {
            maxMovesInMode = rand() % 6 + 1;  // от 1 до 3 ходов
            movesInMode = 0;
            return 'C';
        }
        movesInMode++;
        return 'D';
    }

};

class RememberFirstEnemyStrategy : public Strategy {
private:
    int EnemyNum;
public:
    RememberFirstEnemyStrategy() : EnemyNum(0) {}

    std::string getName() const override {
        return "youmyenemy";
    }

    char makeMove() override {
        if (opponent1Moves.empty() && opponent2Moves.empty()) {
            return 'C';
        }
        else {
            char move1 = opponent1Moves.back(), move2 = opponent2Moves.back();
            if (EnemyNum != 0) {
                return 'D';
            }
            if (move1 == 'D') {
                EnemyNum = 1;
                return 'D';
            } else if (move2 == 'D') {
                EnemyNum = 2;
                return 'D';
            }
            
            return 'C';
        }
    }
};

class AnalyzerStrategy : public Strategy {
private:
    int FirstCntD, SecondCntD;
public:
    AnalyzerStrategy() : FirstCntD(0), SecondCntD(0) {}

    std::string getName() const override {
        return "analyzer";
    }

    char makeMove() override {
        if (opponent1Moves.empty() && opponent2Moves.empty()) {
            return 'C';
        }
        else {
            char move1 = opponent1Moves.back(), move2 = opponent2Moves.back();
            if (move1 == 'D') {
                FirstCntD++;
            }
            if (move2 == 'D') {
                SecondCntD++;
            }
            double percent1 = (double)FirstCntD / opponent1Moves.size();
            double percent2 = (double)SecondCntD / opponent2Moves.size();
            return (percent1 > 0.5 || percent2 > 0.5) ? 'D' : 'C';
        }
    }
};

class CopyBestPlayerStrategy : public Strategy {
public:
    std::string getName() const override {
        return "copybest";
    }

    char makeMove() override {
        if (opponent1Moves.empty() && opponent2Moves.empty()) {
            return 'C';
        }
        else {
            char mymove = myMoves.back(), move1 = opponent1Moves.back(), move2 = opponent2Moves.back();
            int maxScore = totalScores[0];
            int winnerIndex = 0;
            for (int i = 1; i < totalScores.size(); i++) {
                if (totalScores[i] > maxScore) {
                    maxScore = totalScores[i];
                    winnerIndex = i;
                }
            }
            if (winnerIndex == 0) return mymove;
            if (winnerIndex == 1) return move1;
            if (winnerIndex == 2) return move2;

            return 'C';
        }
    }
};

class MixOfStrategies : public Strategy {
private:
    std::vector<Strategy*> Mix;
    
public:
    MixOfStrategies() {
        Mix.push_back(new AllCStrategy());
        Mix.push_back(new AllDStrategy());
        Mix.push_back(new RandomStrategy());
        Mix.push_back(new RevengeStrategy());
        Mix.push_back(new RememberFirstEnemyStrategy());
        Mix.push_back(new AnalyzerStrategy());
        Mix.push_back(new CopyBestPlayerStrategy());
    }

    ~MixOfStrategies() {
        for (Strategy* strategy : Mix) {
            delete strategy;
        }
    }
    
    std::vector<int> getRandomKeys(int count, int MixSize) {
        std::vector<int> sp;
        while (sp.size() < count) {
            int randomIndex = rand() % MixSize;
            bool alreadyExists = false;
            for (int existing : sp) {
                if (existing == randomIndex) {
                    alreadyExists = true;
                    break;
                }
            }
            if (!alreadyExists) {
                sp.push_back(randomIndex);
            }
        }

        return sp;
    }

    std::string getName() const override {
        return "strategymix";
    }

    char makeMove() override {
        for (Strategy* member : Mix) {
            member->copyHistoryFrom(*this);
        }
        std::vector<int> randomKeys = getRandomKeys(5,Mix.size());
        int voteC = 0, voteD = 0;
        for (int i : randomKeys) {
            Strategy* s = Mix[i];
            char vote = s->makeMove();
            if (vote == 'C') voteC++; else voteD++;
        }
        return (voteC > voteD) ? 'C' : 'D';
    }
};

class FindBestStrategy : public Strategy {
private:
    struct Observation {
        int totalScore = 0;
        int count = 0;
        double average() const {
            return (count == 0) ? 4 : (double)totalScore / count;
        }
    };
    Observation stat[2][2][2];
    int steps;
    
public:
    FindBestStrategy() : steps(0) {}
    
    std::string getName() const override {
        return "findbest";
    }

    char makeMove() override {
        steps++;
        
        if (!opponent1Moves.empty()) {
            char myLastMove = myMoves.back();
            char opp1Last = opponent1Moves.back();
            char opp2Last = opponent2Moves.back();
            int a = (opp1Last == 'C') ? 0 : 1;
            int b = (opp2Last == 'C') ? 0 : 1; 
            int c = (myLastMove == 'C') ? 0 : 1;
            
            stat[a][b][c].count++;
            stat[a][b][c].totalScore += lastScore;
        }
        
        if (steps <= 20) {
            return (rand() % 2 == 0) ? 'C' : 'D';
        }
        
        double bestScore = -1;
        char bestMove = 'C';
        
        for (int a = 0; a < 2; a++) {
            for (int b = 0; b < 2; b++) {
                for (int c = 0; c < 2; c++) {
                    if (stat[a][b][c].count > 0) {
                        double avg = stat[a][b][c].average();
                        if (avg > bestScore) {
                            bestScore = avg;
                            bestMove = (c == 0) ? 'C' : 'D';
                        }
                    }
                }
            }
        }
        
        return (bestScore == -1) ? 'C' : bestMove;
    }
};