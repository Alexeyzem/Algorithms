#include "iostream"
#include "array"
#include "queue"
#include "cassert"
#include "string"
#include "unordered_map"
#include "set"
//                          Восьминашки
//Перемещение костяшки вверх меняет число инверсий на четное значение
//                            Пятнашки
// условие на достижимость: четность перестановки без 0 + номер строки 0?(уточнить)
// лучше переходить почти сразу на эвристический поиск
// для очереди с приоритетом лучше std::set
//сначала ключ удалить, потом добваить в сет.

const int SideSize = 4;
const int FieldSize = SideSize*SideSize;
const std::array<char, FieldSize> FinishField({1 , 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0});




class GameState{
public:
    GameState( const std::array<char, FieldSize>& _field);
    bool CanMoveLeft() const;
    bool CanMoveRight() const;
    bool CanMoveUp() const;
    bool CanMoveDown() const;
    bool Check() const;
    GameState MoveLeft() const;
    GameState MoveRight() const;
    GameState MoveUp() const;
    GameState MoveDown() const;
    char GetFieldValue(size_t pos) const {return field[pos];};
    bool IsFinish() const;

    bool operator == (const GameState& state) const{
        return field == state.field;
    }
    bool operator != (const GameState& state)const{
        return !(*this == state);
    }
    size_t Evristik() const;
private:
    std::array<char, FieldSize> field;
    size_t zeroPos;
};
struct StateHasher{
    size_t operator() (const GameState& state) const{
        size_t hash = 0;
        size_t p = 137;
        for (int i = 0; i < FieldSize; ++i){
            hash += state.GetFieldValue(i)*p;
        }
        return hash;
    }
};


GameState::GameState( const std::array<char, FieldSize>& _field):field(_field), zeroPos(-1){
    for (int i = 0; i < field.size(); ++i){
        if (field[i] == 0){
            zeroPos = i;
        }
    }
    assert(zeroPos >= 0);
};

bool GameState::CanMoveLeft() const{
    return zeroPos % SideSize < SideSize - 1;
}
bool GameState::CanMoveRight() const{
    return zeroPos % SideSize > 0;
}
bool GameState::CanMoveUp() const{
    return zeroPos < FieldSize - SideSize;
}
bool GameState::CanMoveDown() const{
    return zeroPos >= SideSize;
}

GameState GameState::MoveLeft() const{
    assert(CanMoveLeft());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos+1]);
    ++newState.zeroPos;
    return newState;
}
GameState GameState::MoveRight() const{
    assert(CanMoveRight());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos-1]);
    --newState.zeroPos;
    return newState;
}
GameState GameState::MoveUp() const{
    assert(CanMoveUp());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos+SideSize]);
    newState.zeroPos += SideSize;
    return newState;
}
GameState GameState::MoveDown() const{
    assert(CanMoveDown());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos-SideSize]);
    newState.zeroPos -= SideSize;
    return newState;
}

bool GameState::IsFinish() const{
    return field == FinishField;
}

size_t GameState::Evristik() const {
    size_t result = 0;
    for(int i = 0; i < field.size(); ++i){
        if (field[i] != i-1 && field[i] != 0){
           result = result + abs((i+1) % FieldSize - field[i]%FieldSize) + abs((i+1)/FieldSize - field[i]/FieldSize);
        }
    }
    return result;
}

bool GameState::Check() const {
    int chet = 0;
    for (int i = 1; i < FieldSize; ++i){
        if (field[i]!=0){
            for (int j = 0; j < i; j++){
                if (field[j]!=0 && field[i] < field[j]){
                    chet++;
                }
            }
        }
    }
    chet += zeroPos/SideSize - 1;
    return chet%2==0;
}


struct GameSet{
public:
    GameState gameState;
    size_t priopity;
};

struct Compare{
    bool operator()(const GameSet l, const GameSet r) const{
        return l.priopity <= r.priopity;
    }
};
std::string Get16thSolution(const GameState& state) {
    if(!state.Check()){
        return "";
    }
    std::set<GameSet, Compare> s;
    s.insert(GameSet{state, 1});
    std::unordered_map<GameState, size_t, StateHasher> distance;
    std::unordered_map<GameState, char, StateHasher> visited;
    visited[state] = 'S';
    distance[state] = state.Evristik();
    while (!s.empty()){
        auto it = s.begin();
        GameSet current = *it;
        s.erase(it);
        if (current.gameState.IsFinish()){
            break;
        }
        if(current.gameState.CanMoveLeft()){
            GameState newState = current.gameState.MoveLeft();
            if (distance.find(newState) == distance.end()){
                distance[newState] = newState.Evristik();
                s.insert(GameSet{newState, newState.Evristik()});
                visited[newState] = 'L';
            }
        }
        if(current.gameState.CanMoveUp()){
            GameState newState = current.gameState.MoveUp();
            if (distance.find(newState) == distance.end()){
                distance[newState] = newState.Evristik();
                s.insert(GameSet{newState, newState.Evristik()});
                visited[newState] = 'U';
            }
        }
        if(current.gameState.CanMoveRight()){
            GameState newState = current.gameState.MoveRight();
            if (distance.find(newState) == distance.end()){
                distance[newState] = newState.Evristik();
                s.insert(GameSet{newState, newState.Evristik()});
                visited[newState] = 'R';
            }
        }
        if(current.gameState.CanMoveDown()){
            GameState newState = current.gameState.MoveDown();
            if (distance.find(newState) == distance.end()){
                distance[newState] = newState.Evristik();
                s.insert(GameSet{newState, newState.Evristik()});
                visited[newState] = 'D';
            }
        }
    }
    std::string result;
    GameState current(FinishField);
    char move = visited[current];
    while (move != 'S'){
        result += move;
        switch (move) {
            case 'L':
                current = current.MoveRight();
                break;
            case 'R':
                current = current.MoveLeft();
                break;
            case 'U':
                current = current.MoveDown();
                break;
            case 'D':
                current = current.MoveUp();
                break;
            default:
                assert(false);
                break;
        }
        move = visited[current];
    }
    return result;
}

std::ostream& operator << (std::ostream& out, const GameState& state){
    for (size_t y = 0; y < SideSize; ++y){
        for (size_t x = 0; x < SideSize; ++x){
            out << static_cast<int>(state.GetFieldValue(y*SideSize+x) )<< " ";
        }
        out << std::endl;
    }
    out << std::endl;
    return out;
}

int main(){
    std::array<char, FieldSize> arr{};
    for(int i = 0; i < FieldSize; ++i){
        int tmp;
        std:: cin >> tmp;
        arr[i] = tmp;
    }
    GameState state (arr);
    std::string result = Get16thSolution(state);
//    for (int i = result.size()-1; i >= 0; --i){
//        char move = result[i];
//        switch (move) {
//            case 'L':
//                state = state.MoveLeft();
//                break;
//            case 'R':
//                state = state.MoveRight();
//                break;
//            case 'U':
//                state = state.MoveUp();
//                break;
//            case 'D':
//                state = state.MoveDown();
//                break;
//        }
//        std::cout << state;
//    }
if (result.empty()){
    std::cout << -1;
    return 0;
}
std::cout << result.size() << std::endl;
for (int i = result.size()-1 ; i >=0; i--){
    std::cout << result[i];
}
    return 0;
}