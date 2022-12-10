//myTictactoe.cpp: playing tictactoe  

/* Version 1:
1. AI Engine
    - state representation: vector<vector<char>> 3x3
    - Print table in the next turn
    - Check the final game 
    - Find win player (X win or O win or draw)
    - Find the current player
    - If humanPlayer's turn: user choose the cordinate(tọa độ) move
    - Else 
        + Find all valid move in the turn
        + calculate the best move
    - Play the next turn 
    - Play the first turn if humanPlayer is O


2. User Interface(UI)
    - Table 3x3 
    - choose X or O
    - input cordinate start from 0 to 2
    - response if inligal move
    - response game when end
    - print current game
    - print machine's move


*/





#include <iostream>
#include <vector>

using namespace std;
const int SIZE = 3;
typedef vector<vector<char>> State;


//print current game
//input: state s before move
//output: state s after move
void printTable(State state)
{
    cout << "  -------------------" << endl;
    for (int i=0;i<SIZE;i++) {
        
        for (int j=0;j<SIZE;j++) {
            cout<<"  |  "<<state[i][j];
        }
        cout<<"  |"<<endl<<"  -------------------\n";
    }
}
State play(State s, int i, int j, char player)  //play next turn
{
    State newState(3,vector<char>(3, ' '));
    
    for ( int i=0; i<SIZE; i++) {
        for (int j=0;j<SIZE;j++) 
            newState[i][j]= s[i][j];
        cout<<endl;   
    }
    if (newState[i][j] == ' ') {
        newState[i][j]=player;
    }
    return newState;
}


//Find the next player's turn
//Input: state s 
//Output: next player
char getNextPlayer(State s)   
{
    int countX=0, countO=0;
    for (int i=0;i<SIZE;i++) {
        for (int j=0;j<SIZE;j++) {
            if (s[i][j] == 'X') countX++;
            else if (s[i][j] == 'O') countO++;
        }
    }
    if (countX==countO) return 'X';
    else if (countX>countO) return 'O';
}


//Check finnal game
//Input: state s
//Output: true/false if game end/not-end
bool isFinal(State s) 
{
    //check cols
    for (int i=0;i<SIZE;i++) {
        if (s[0][i]==s[1][i]&&s[1][i]==s[2][i]&&s[0][i]!=' ') return true;
    }
    //check rows
    for (int i=0;i<SIZE;i++) {
        if (s[i][0]==s[i][1]&&s[i][1]==s[i][2]&&s[i][1]!=' ') return true;
    }
    //check dialgonals
    if (s[0][0]==s[1][1]&&s[1][1]==s[2][2]&&s[1][1]!=' ') return true;
    if (s[2][0]==s[1][1]&&s[1][1]==s[0][2]&&s[1][1]!=' ') return true;
    // check full
    for (int i=0;i<SIZE;i++) {
        for (int j=0;i<SIZE;j++) {
            if (s[i][j]==' ') return false; 
        }
    }
    return true;
}



//tính nước đi thắng
//input: bảng s khi vừa chơi xong
//output: nước đi mang đến thắng cuộc
int getScoreFinalState(State s) 
{
    // check rows
    for (int i = 0; i < SIZE; i++)
        if (s[i][0] == s[i][1] && s[i][0] == s[i][2] && s[i][0] != ' ') 
            return s[i][0] == 'X' ? 1 : -1;
    // check cols
    for (int i = 0; i < SIZE; i++)
        if (s[0][i] == s[1][i] && s[0][i] == s[2][i] && s[0][i] != ' ')
            return s[0][i] == 'X' ? 1 : -1;
    // check diagonals        
    if (s[0][0] == s[1][1] && s[0][0] == s[2][2] && s[0][0] != ' ')
        return s[0][0] == 'X' ? 1 : -1;
    if (s[0][2] == s[1][1] && s[0][2] == s[2][0] && s[0][2] != ' ')
        return s[0][2] == 'X' ? 1 : -1;
    return 0;
}
//các nước chơi có thể đánh
//input: bảng s khi vừa kết thúc lượt người
//output: các nước chơi có thể đánh tiếp
vector<State> getNextStates(State s)  
{
    char nextPlayer = getNextPlayer(s);
    vector<State> states;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (s[i][j] == ' ') {
                State nextState = play(s, i, j, nextPlayer);
                states.push_back(nextState);
            }
        }
    }
    return states;
}

 //calculate score
 //input: bảng s
 //output: cặp giá trị (điểm số và nước đi cho điểm số đó)
 pair<int, State> getScore(State s) 
{
    if (isFinal(s)) { //Nếu có nước đi có thể kết thúc game, đánh nước đi đó
        return make_pair(getScoreFinalState(s), s);
    }
    char player = getNextPlayer(s); //biến đánh dấu nước đi hiện tại
    vector<State> states = getNextStates(s); //các nước đi hiện tại có thể chơi
    int bestScore = -10;
    State bestState;
    for (State ns: states) {
        pair<int, State> p = getScore(ns);
        int score = p.first;
        if (player == 'X') {
            if (bestScore < score || bestScore == -10) {
                bestScore = score;
                bestState = ns;
            }
        } else {
            if (bestScore > score || bestScore == -10) {
                bestScore = score;
                bestState = ns;
            }
        }
    }
    return make_pair(bestScore, bestState);
}
    
State playFirstTurn(State s) {
    int i,j;
    i=i%3;
    j=j%3;
    
    s[i][j]='X';
    return s;
}





pair<int, int> getComputerPlay(State s)   // Hàm sử dụng để máy đánh nước đi của mình
{
    pair<int, State> p = getScore(s); 
    int score = p.first;
    State newState = p.second;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (newState[i][j] != s[i][j]) return make_pair(i, j);
        }
    }
    return make_pair(-1, -1);
}

void run() {
    State s = State(3, vector<char>(3, ' '));
    char humanPlayer = ' ', currentPlayer = 'X';
    cout << "Choose your player (X or O): ";
    cin >> humanPlayer;
    cout << "You are " << humanPlayer << " player";
    
    printTable(s);
    if (humanPlayer=='O') {
        s = playFirstTurn(s);
        printTable(s);
        currentPlayer = 'O';
    } 
    while (true) { //Lệnh while chỉ kết thúc khi có người thắng
        int i = -1, j = -1; 
        if (humanPlayer == currentPlayer) {
        //    cout << "Enter your play: ";
        //    cin >> i >> j;
        //    if (s[i][j]==' ')
        //        cout << "You play (" << i << ", " << j << ')'<< endl;
        //    else {
        //        cout << "You not play (" << i << ", " << j << ')' << endl;
        //        cout << "Please play again!" << endl;
        //    }
            // TODO check legal move
            while (true) {
                cout << "Enter your play: ";
                cin >> i >> j;
                if (s[i][j]==' ') {
                    cout << "You play (" << i << ", " << j << ')'<< endl;
                    break;
                } else {
                    cout << "\nYou not play (" << i << ", " << j << ')' << endl;
                    cout << "Please play again!" << endl;
                }
            }
        } else {
            pair<int, int> move = getComputerPlay(s);
            i = move.first;
            j = move.second;
            
            cout << "I play (" << i << ", " << j << ")\n" << endl;
        }
        s = play(s, i, j, currentPlayer);
        printTable(s);

        currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
        if (isFinal(s)) {
            int score = getScoreFinalState(s);
            if (score > 0) {
                cout << "X won" << endl; 
            } else if (score < 0) {
                cout << "X lost" << endl; 
            } else {
                cout << "A draw" << endl; 
            }
            break;
        }
    }

}

int main() {
    State s = State(3, vector<char>(3, ' '));
    char humanPlayer = ' ', currentPlayer = 'X';
    cout << "Choose your player (X or O): ";
    cin >> humanPlayer;
    cout << "You are " << humanPlayer << " player\n";
    
    printTable(s);
    if (humanPlayer=='O') {

        s = playFirstTurn(s);
        printTable(s);
        currentPlayer = 'O';
    } 
    while (true) { //Lệnh while chỉ kết thúc khi có người thắng
        int i = -1, j = -1; 
        if (humanPlayer == currentPlayer) {
        //    cout << "Enter your play: ";
        //    cin >> i >> j;
        //    if (s[i][j]==' ')
        //        cout << "You play (" << i << ", " << j << ')'<< endl;
        //    else {
        //        cout << "You not play (" << i << ", " << j << ')' << endl;
        //        cout << "Please play again!" << endl;
        //    }
            // TODO check legal move
            while (true) {
                cout << "Enter your play: ";
                cin >> i >> j;
                if (s[i][j]==' ') {
                    cout << "You play (" << i << ", " << j << ')'<< endl;
                    break;
                } else {
                    cout << "\nYou not play (" << i << ", " << j << ')' << endl;
                    cout << "Please play again!" << endl;
                }
            }
        } else {
            pair<int, int> move = getComputerPlay(s);
            i = move.first;
            j = move.second;
            
            cout << "I play (" << i << ", " << j << ")\n" << endl;
        }
        s = play(s, i, j, currentPlayer);
        printTable(s);

        currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
        if (isFinal(s)) {
            int score = getScoreFinalState(s);
            if (score > 0) {
                cout << "X won" << endl; 
            } else if (score < 0) {
                cout << "X lost" << endl; 
            } else {
                cout << "A draw" << endl; 
            }
            break;
        }
    }
    
    return 0;
}

