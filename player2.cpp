#include "player.hpp"

static const int SIZE = 8;
/**
 * @brief convert to char a piece
 * @param p the piece to convert
 */
char to_char(Player::piece p){
    switch (p) {
        case Player::piece::x:
            return 'x';
        case Player::piece::o:
            return 'o';
        case Player::piece::X:
            return 'X';
        case Player::piece::O:
            return 'O';
        case Player::piece::e:
            return ' ';
    }
    return ' ';
}
/**
 * @brief convert to piece a char
 * @param c the char to convert
 */
Player::piece to_piece(char c){
    switch (c) {
        case 'x':
            return Player::piece::x;
        case 'o':
            return Player::piece::o;
        case 'X':
            return Player::piece::X;
        case 'O':
            return Player::piece::O;
        case ' ':
            return Player::piece::e;
    }
    return Player::piece::e;
}
/**
 * @brief get the number of piece of a player
 * @param board the game board
 * @param player the player to check [0] = (x V o) [1] = (X V O) 
 */
int get_left (Player::piece** board, Player::piece p[2]){
    int count=0;
    for (int i=0; i < 8; i++)
        for (int j=0; j < 8; j++)
            if(board[i][j]==p[1] || board[i][j]==p[0] ) count++;
    return count;
}
/**
 * @brief struct to store a move
 */
struct Move {
    int start[2];
    int eat [2];
    Player::piece moving;
    int finish[2];
    Move() {
        start [0] = 0;
        start [1] = 0;
        eat [0] = 0;
        eat [1] = 0;
        finish [0] = 0;
        finish [1] = 0;
        moving = Player::piece::e;
    }
    void create_move(int start_x, int start_y, int finish_x, int finish_y, int eaten_x, int eaten_y, Player::piece p) {
        start [0] = start_x;
        start [1] = start_y;
        finish [0] = finish_x;
        finish [1] = finish_y;
        moving = p;
        eat [0] = eaten_x;
        eat [1] = eaten_y;
    }
};
/**
 * @brief return the corresponding dama of a piece 
 */
Player::piece get_dame(Player::piece p) {
    return (p == Player::piece::x) ?  Player::piece::X :  Player::piece::O;
}
/**
 * @brief return the player of the next round
 */
Player::piece next_player(Player::piece p) {
    return (p == Player::piece::x) ?  Player::piece::o :  Player::piece::x;
}
/**
 * @brief return all the possible moves of a player
 * @param direction the move direction 1 or -1
 * @param board the board game
 * @param p the player piece
 */
Move* get_all_moves(int direction, Player::piece** const b, Player::piece p) {
    int now_x = 0, now_y = 0;
    int future_x = 0, future_y = 0;
    Move* possible_moves = new Move[40];
    Player::piece p_dame = get_dame(p);
    Player::piece enemy_dame = (get_dame(next_player(p)));
    Player::piece enemy_pawn = next_player(p);
    Player::piece aux_pawn = p;
    int index = 0;

    for(int j = 0; j < 8; j++) {

        for(int k = 0; k < 8; k++) {
            now_x = j;
            now_y = k;
            aux_pawn = b[now_x][now_y];

            if( p == Player::piece::x){
                switch (aux_pawn){
                    case Player::piece::x:
                        future_x = now_x + direction;
                        future_y = now_y - direction;
                        if((future_x >= 0) && (future_y >= 0) && (future_y < SIZE) && (future_x < SIZE)) {
                            aux_pawn = b[future_x][future_y];
                            if(aux_pawn == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p);
                                index++;
                            }else if(aux_pawn == enemy_pawn) {
                                if(((future_x + direction) >= 0) && ((future_y - direction) >= 0) && ((future_x + direction) < SIZE)  && ((future_y - direction) < SIZE)) {
                                    aux_pawn= b[future_x + direction][future_y - direction];
                                    if((aux_pawn == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x + direction), (future_y - direction), future_x, future_y, p);
                                        index++;
                                    }
                                }
                            }
                        }
                        future_y = now_y + direction;
                        future_x = now_x + direction;
                        if((future_x >= 0) && (future_y >= 0) && (future_x < SIZE) &&  (future_y < SIZE)) {
                            aux_pawn = b[future_x][future_y];
                            if(aux_pawn == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p);
                                index++;
                            }else if(aux_pawn == enemy_pawn) {
                                if(((future_x + direction) >= 0) && ((future_y + direction) >= 0) && ((future_x + direction) < SIZE) &&  ((future_y + direction) < SIZE)) {
                                    aux_pawn = b[future_x + direction][future_y + direction];
                                    if((aux_pawn == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x + direction), (future_y + direction), future_x, future_y, p);
                                        index++;
                                    }
                                }
                            }
                        }
                        break;
                    case Player::piece::X:
                        future_y = now_y - 1;
                        future_x = now_x + 1;
                        if((future_x >= 0) && (future_y >= 0) && (future_x < SIZE) &&  (future_y < SIZE)) {
                            aux_pawn = b[future_x][future_y];
                            if(aux_pawn == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p_dame);
                                index++;
                            }else if((aux_pawn == enemy_pawn) || (aux_pawn == enemy_dame)) {
                                if(((future_x + 1) >= 0) && ((future_x + 1) < SIZE) && ((future_y - 1) >= 0) && ((future_y - 1) < SIZE)) {
                                    if((b[future_x + 1][future_y - 1] == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x + 1), (future_y - 1), future_x, future_y, p_dame);
                                        index++;
                                    }
                                }
                            }
                        }
                        future_x = now_x + 1;
                        future_y = now_y + 1;
                        if((future_x >= 0) && (future_x < SIZE) && (future_y >= 0) && (future_y < SIZE)) {
                            if(b[future_x][future_y] == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p_dame);
                                index++;
                            }else if((b[future_x][future_y] == enemy_pawn) || (b[future_x][future_y] == enemy_dame)) {
                                if(((future_x + 1) >= 0) && ((future_x + 1) < SIZE) && ((future_y + 1) >= 0) && ((future_y + 1) < SIZE)) {
                                    if((b[future_x + 1][future_y + 1] == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x + 1), (future_y + 1), future_x, future_y, p_dame);
                                        index++;
                                    }
                                }
                            }
                        }
                        future_x = now_x - 1;
                        future_y = now_y - 1;
                        if((future_x >= 0) && (future_x < 8) && (future_y >= 0) && (future_y < 8)) {
                            if(b[future_x][future_y] == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p_dame);
                                index++;
                            }else if((b[future_x][future_y] == enemy_pawn) || (b[future_x][future_y] == enemy_dame)) {
                                if(((future_x - 1) >= 0) && ((future_x - 1) < SIZE) && ((future_y - 1) >= 0) && ((future_y - 1) < SIZE)) {
                                    if((b[future_x - 1][future_y - 1] == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x - 1), (future_y - 1), future_x, future_y, p_dame);
                                        index++;
                                    }
                                }
                            }
                        }
                        future_x = now_x - 1;
                        future_y = now_y + 1;
                        if((future_x >= 0) && (future_x < SIZE) && (future_y >= 0) && (future_y < SIZE)) {
                            if(b[future_x][future_y] == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p_dame);
                                index++;
                            }else if((b[future_x][future_y] == enemy_pawn) || (b[future_x][future_y] == enemy_dame)) {
                                if(((future_x - 1) >= 0) && ((future_x - 1) < SIZE) && ((future_y + 1) >= 0) && ((future_y + 1) < SIZE)) {
                                    if((b[future_x - 1][future_y + 1] == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x - 1), (future_y + 1), future_x, future_y, p_dame);
                                        index++;
                                    }
                                }
                            }
                        }
                        break;
                }
            }else if (p == Player::piece::o){
                switch (aux_pawn){
                    case Player::piece::o:
                        future_x = now_x + direction;
                        future_y = now_y - direction;
                        if((future_x >= 0) && (future_y >= 0) && (future_y < SIZE) && (future_x < SIZE)) {
                            aux_pawn = b[future_x][future_y];
                            if(aux_pawn == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p);
                                index++;
                            }else if(aux_pawn == enemy_pawn) {
                                if(((future_x + direction) >= 0) && ((future_y - direction) >= 0) && ((future_x + direction) < SIZE)  && ((future_y - direction) < SIZE)) {
                                    aux_pawn= b[future_x + direction][future_y - direction];
                                    if((aux_pawn == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x + direction), (future_y - direction), future_x, future_y, p);
                                        index++;
                                    }
                                }
                            }
                        }
                        future_y = now_y + direction;
                        future_x = now_x + direction;
                        if((future_x >= 0) && (future_y >= 0) && (future_x < SIZE) &&  (future_y < SIZE)) {
                            aux_pawn = b[future_x][future_y];
                            if(aux_pawn == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p);
                                index++;
                            }else if(aux_pawn == enemy_pawn) {
                                if(((future_x + direction) >= 0) && ((future_y + direction) >= 0) && ((future_x + direction) < SIZE) &&  ((future_y + direction) < SIZE)) {
                                    aux_pawn = b[future_x + direction][future_y + direction];
                                    if((aux_pawn == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x + direction), (future_y + direction), future_x, future_y, p);
                                        index++;
                                    }
                                }
                            }
                        }
                        break;
                    case Player::piece::O:
                        future_y = now_y - 1;
                        future_x = now_x + 1;
                        if((future_x >= 0) && (future_y >= 0) && (future_x < SIZE) &&  (future_y < SIZE)) {
                            aux_pawn = b[future_x][future_y];
                            if(aux_pawn == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p_dame);
                                index++;
                            }else if((aux_pawn == enemy_pawn) || (aux_pawn == enemy_dame)) {
                                if(((future_x + 1) >= 0) && ((future_x + 1) < SIZE) && ((future_y - 1) >= 0) && ((future_y - 1) < SIZE)) {
                                    if((b[future_x + 1][future_y - 1] == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x + 1), (future_y - 1), future_x, future_y, p_dame);
                                        index++;
                                    }
                                }
                            }
                        }
                        future_x = now_x + 1;
                        future_y = now_y + 1;
                        if((future_x >= 0) && (future_x < SIZE) && (future_y >= 0) && (future_y < SIZE)) {
                            if(b[future_x][future_y] == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p_dame);
                                index++;
                            }else if((b[future_x][future_y] == enemy_pawn) || (b[future_x][future_y] == enemy_dame)) {
                                if(((future_x + 1) >= 0) && ((future_x + 1) < SIZE) && ((future_y + 1) >= 0) && ((future_y + 1) < SIZE)) {
                                    if((b[future_x + 1][future_y + 1] == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x + 1), (future_y + 1), future_x, future_y, p_dame);
                                        index++;
                                    }
                                }
                            }
                        }
                        future_x = now_x - 1;
                        future_y = now_y - 1;
                        if((future_x >= 0) && (future_x < 8) && (future_y >= 0) && (future_y < 8)) {
                            if(b[future_x][future_y] == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p_dame);
                                index++;
                            }else if((b[future_x][future_y] == enemy_pawn) || (b[future_x][future_y] == enemy_dame)) {
                                if(((future_x - 1) >= 0) && ((future_x - 1) < SIZE) && ((future_y - 1) >= 0) && ((future_y - 1) < SIZE)) {
                                    if((b[future_x - 1][future_y - 1] == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x - 1), (future_y - 1), future_x, future_y, p_dame);
                                        index++;
                                    }
                                }
                            }
                        }
                        future_x = now_x - 1;
                        future_y = now_y + 1;
                        if((future_x >= 0) && (future_x < SIZE) && (future_y >= 0) && (future_y < SIZE)) {
                            if(b[future_x][future_y] == Player::piece::e) {
                                possible_moves[index].create_move(now_x, now_y, future_x, future_y, 0, 0, p_dame);
                                index++;
                            }else if((b[future_x][future_y] == enemy_pawn) || (b[future_x][future_y] == enemy_dame)) {
                                if(((future_x - 1) >= 0) && ((future_x - 1) < SIZE) && ((future_y + 1) >= 0) && ((future_y + 1) < SIZE)) {
                                    if((b[future_x - 1][future_y + 1] == Player::piece::e)) {
                                        possible_moves[index].create_move(now_x, now_y, (future_x - 1), (future_y + 1), future_x, future_y, p_dame);
                                        index++;
                                    }
                                }
                            }
                        }
                        break;
                }
            }

        }
    }
    return possible_moves;
}
/**
 * @brief get a random move between all the possible moves
 * @param board the board game
 * @param player the player piece
 */
Move get_random_move(Player::piece** board, Player::piece player) {
    Move final_move;
    int direction=0;
    (player == Player::piece::x)? direction = 1 : direction = -1;
    Move* moves = get_all_moves(direction, board, player);
    int size = 0, random_index = 0;
    for(int i = 0; ((i < 40) && (moves[i].moving != Player::piece::e)); i++) size++;
    if(size != 0) {
        srand(time(NULL)*time(NULL)/rand());
        random_index = (std::rand() % size);
        cout<<random_index<<endl;
        final_move = moves[random_index];
    }
    delete[] moves;
    return final_move;
}
/**
 * @brief compare two boards
 * @param m1 the first board
 * @param m2 the second board
 */
bool are_board_equals(Player::piece** m1, Player::piece** m2) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            if(m1[i][j] != m2[i][j])
                return false;
        }
    }
    return true;
}
/**
 * @brief create a board given the move
 * @param b the board
 * @param m the move
 */
Player::piece** create_board(Player::piece** const b, Move m) {
    Player::piece** b_nxt = b;
    b_nxt[m.start[0]][m.start[1]] = Player::piece::e;
    if(m.moving == Player::piece::x) {
        (m.finish[0] == (SIZE - 1)) ? b_nxt[m.finish[0]][m.finish[1]] = Player::piece::X : b_nxt[m.finish[0]][m.finish[1]] = Player::piece::x;
    }else if(m.moving == Player::piece::o) {
        (m.finish[0] == 0) ? b_nxt[m.finish[0]][m.finish[1]] = Player::piece::O : b_nxt[m.finish[0]][m.finish[1]] = Player::piece::o;
    }else b_nxt[m.finish[0]][m.finish[1]] = m.moving;
    b_nxt[m.eat[0]][m.eat[1]] = Player::piece::e;
    return b_nxt;
}
/**
 * @brief delete memory allocated for the board
 * @param board the board to delete
 */
void delete_board(Player::piece** (&board)){
    for(int i = 0; i < 8; i++) delete[] board[i];
    delete[] board;
}
/**
 * @brief check if the filename is valid
 * @param filename the filename to check
 */
bool file_exists(const std::string& filename){
    std::ifstream f(filename.c_str());
    return f.good();
}
/**
 * @brief initialize the board
 */
Player::piece** initialize_board(){
    Player::piece** board = nullptr;
    board = new Player::piece*[8];
    for(int i = 0; i < SIZE; i++)
        board[i] = new Player::piece[8];
    return board;
}
/**
 * @brief struct for the history of the player
 */
struct Player::Impl{
    Impl* next;
    Player::piece** board;
    int index;
    int player_nr;
};
/**
 * @brief constructor of the player
 */
Player::Player(int player_nr) {
    if (player_nr != 1 && player_nr != 2)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: The player can only be 0 or 1"};
    pimpl = new Impl{nullptr, nullptr, 0, player_nr};
}
/**
 * @brief Destructor of the player
 */
Player::~Player(){
    std::cout << "destructor called" << std::endl;
    while(pimpl != nullptr){
        Impl* temp = pimpl;
        pimpl = pimpl->next;
        if(temp->board != nullptr) delete_board(temp->board);
        delete temp;
    }
    delete pimpl;
    std::cout << "destructor ended" << std::endl;
}
/**
 * @brief copy constructor of the player
 */
Player::Player(const Player& copy) {
    std::cout << "copy constructor called" << std::endl;
    this->pimpl = new Impl{
            nullptr,
            initialize_board(),
            copy.pimpl->index,
            copy.pimpl->player_nr
    };
    if (copy.pimpl->board != nullptr) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                this->pimpl->board[i][j] = copy.pimpl->board[i][j];
            }
        }
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            this->pimpl->board[i][j] = copy.pimpl->board[i][j];
    Impl *copyTemp = copy.pimpl;
    Impl *thisTemp = this->pimpl;
    while (copyTemp->next) {
        thisTemp->next = new Impl{
                nullptr,
                initialize_board(),
                thisTemp->index + 1,
                copyTemp->player_nr
        };

        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                thisTemp->next->board[i][j] = copyTemp->next->board[i][j];
        thisTemp = thisTemp->next;
        copyTemp = copyTemp->next;
    }
}else this->pimpl->board = nullptr;
    std::cout << "copy constructor ended" << std::endl;
}
/**
 * @brief copy assignment operator of the player
 */
Player& Player::operator=(const Player& copy) {
    std::cout << "operator= called" << std::endl;
    if(this->pimpl != copy.pimpl) {
        delete pimpl;
        this->pimpl = new Impl{
                nullptr,
                initialize_board(),
                copy.pimpl->index,
                copy.pimpl->player_nr
        };
        for(int i = 0; i < 8; i++)
            for(int j = 0; j < 8; j++)
                this->pimpl->board[i][j] = copy.pimpl->board[i][j];
        Impl* copyTemp = copy.pimpl;
        Impl* thisTemp = this->pimpl;
        while(copyTemp->next) {
            thisTemp->next = new Impl{
                    nullptr,
                    initialize_board(),
                    thisTemp->index + 1,
                    copyTemp->player_nr
            };
            for(int i = 0; i < SIZE; i++)
                for(int j = 0; j < SIZE; j++)
                    thisTemp->next->board[i][j] = copyTemp->next->board[i][j];
            thisTemp = thisTemp->next;
            copyTemp = copyTemp->next;
        }
    }
    std::cout << "operator= ended" << std::endl;
    return *this;
}
/**
 * @brief operator () to return a specific piece given it's coordinates
 */
Player::piece Player::operator()(int r, int c, int history_offset) const {
    std::cout << "operator() called" << std::endl;
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }
    if((r < 0) || (r >= SIZE) || (c < 0) || (c >= SIZE) || (history_offset < 0) || (history_offset > (last_index) ) || last_index+1==0 )
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid data (r/c/history_offset)"};
    temp = this->pimpl;
    int index_required = last_index-history_offset;
    while(temp->index!=index_required+1){
        temp = temp->next;
    }

    std::cout << "operator() ended" << std::endl;
    return (temp->board[r][c]);
}
/**
 * @brief load the board from a file
 * @param filename the filename to load from
 */
void Player::load_board(const std::string& filename){
    std::cout << "load board called" << std::endl;
    if(this->pimpl->next == nullptr)
        this->pimpl->board = initialize_board();
    Impl* temp = this->pimpl;
    int lastIndex = this->pimpl->index;
    while(temp->next) {
        lastIndex++;
        temp = temp->next;
    }
    if(!file_exists(filename))
        throw player_exception{player_exception::missing_file, "ERROR: file not found"};
    std::fstream file(filename, std::fstream::in);
    Player::piece** board = initialize_board();
    char c;
    int count_x=0;
    int count_o=0;
    int readCharacters = 0, i = SIZE - 1, j = 0;
    while(file.get(c)) {
        if (i<0){
            delete_board(board);
            throw player_exception{player_exception::invalid_board, "ERROR: invalid board"};
        }
        switch (c) {
            case 'x':
                count_x++;
                break;
            case 'X':
                count_x++;
                break;
            case 'o':
                count_o++;
                break;
            case 'O':
                count_o++;
                break;
        }
        if ((i+j)%2 == 0 && c != ' ' && c != '\n' ){
            delete_board(board);
            throw player_exception{player_exception::invalid_board, "ERROR: invalid board"};
        }
        if(c != '\n'){
            board[i][j] = to_piece(c);
            std::cout << to_char(board[i][j]);
            j++;
            readCharacters++;
            if(j == SIZE){
                j = 0;
                i--;
                std::cout << std::endl;
            }
        }
        if (file.get(c) && c != ' ' && c != '\n' ){
            delete_board(board);
            throw player_exception{player_exception::invalid_board, "ERROR: invalid board"};
        }
        if(readCharacters > (SIZE * SIZE)){
            delete_board(board);
            throw player_exception{player_exception::invalid_board, "ERROR: invalid board"};
        }
        if(count_x>12 || count_o>12){
            delete_board(board);
            throw player_exception{player_exception::invalid_board, "ERROR: invalid board"};
        }
    }
    file.close();
    temp = this->pimpl;
    int last_index = this->pimpl->index;
    while(temp->next) {
        last_index++;
        temp = temp->next;
    }
    temp->next = new Impl{
            nullptr,
            initialize_board(),
            lastIndex+1,
            this->pimpl->player_nr
    };
    temp = temp->next;
    for(i = 0; i < SIZE; i++){
        for(j = 0; j < SIZE; j++) {
            temp->board[i][j] = board[i][j];
        }
    }
    delete_board(board);
    std::cout << "load board ended" << std::endl;
}
/**
 * @brief store the board to a file given the offset
 * @param filename the filename to store to
 * @param history_offset the 'index' of the history
 */
void Player::store_board(const std::string& filename, int history_offset) const{
    std::cout << "store_board called" << std::endl;
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }
    if (history_offset>last_index || history_offset < 0 || last_index+1==0 )
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: history_offset not valid"};
    temp = this->pimpl;
    int index_required = last_index-history_offset;
    while(temp->index!=index_required+1){
        temp = temp->next;
    }
    std::fstream file;
    file.open(filename, std::fstream::out);
    for(int i = (SIZE - 1); i >= 0; i--) {
        for(int j = 0; j < SIZE; j++) {
            file << to_char(temp->board[i][j]);
            if(j != (SIZE - 1))
                file << ' ';
        }
        if(i != 0)
            file << "\n";
    }

    file.close();
    std::cout << "store_board ended" << std::endl;
}
/**
 * @brief initialize and store an new board
 * @param filename the filename to store to
 */
void Player::init_board(const std::string& filename) const{
    std::cout << "init_board called" << std::endl;
    Player::piece **initial_board = initialize_board();
    for(int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i >= 0 && i <= 2)
                ((i + j) % 2) == 0 ? initial_board[i][j] = piece::e : initial_board[i][j] = piece::x;
            else if (i >= 5 && i <= 7)
                ((i + j) % 2) == 0 ? initial_board[i][j] = piece::e : initial_board[i][j] = piece::o;
            else
                initial_board[i][j] = piece::e;
        }
    }
    std::fstream file;
    file.open(filename, std::fstream::out);
    for(int i = SIZE - 1; i >= 0; i--) {
        for(int j = 0; j < SIZE; j++) {
            file << to_char(initial_board[i][j]);
            if(j != SIZE - 1) file << ' ';
        }
        if(i != 0)
            file << "\n";
    }
    file.close();
    delete_board(initial_board);
    std::cout << "init_board ended" << std::endl;
}
/**
 * @brief make a move on the board
 */
void Player::move(){
    std::cout << "move called" << std::endl;
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }
    if(last_index+1==0)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};
    Player::piece ** temp_board = initialize_board();

    for (int i=0; i<SIZE ; i++)
        for (int j=0; j<SIZE; j++)
            temp_board[i][j]=temp->board[i][j];
    Move best_move;
    Player::piece player;
    (temp->player_nr == 1) ? player = x : player = o;
    best_move = get_random_move(temp_board, player);
    for (int i=0; i<SIZE ; i++)
        for (int j=0; j<SIZE; j++)
            temp_board[i][j]=temp->board[i][j];
    Player::piece** res = create_board(temp_board, best_move);
    temp = this->pimpl;
    last_index = this->pimpl->index;
    while(temp->next) {
        last_index++;
        temp = temp->next;
    }
    temp->next = new Impl{
            nullptr,
            initialize_board(),
            last_index+1,
            this->pimpl->player_nr
    };
    temp = temp->next;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++) {
            temp->board[i][j] = res[i][j];
        }
    }
    delete_board(temp_board);
    std::cout << "move ended" << std::endl;
}
/**
 * @brief check if the move is valid
 */
bool Player::valid_move() const{
    std::cout << "valid_move called" << std::endl;
    bool res = false;
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }
    if(last_index+1<2)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};
    Player::piece** before = initialize_board();
    Player::piece** after = initialize_board();
    Player::piece** copy = initialize_board();
    Move* moves_x;
    Move* moves_o;

    temp = this->pimpl;
    Impl* temp0 = this -> pimpl;

    while (temp->next->next){
        temp=temp->next;
    }
    temp0 = temp;
    temp = temp->next;

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            before[i][j] = temp0->board[i][j];

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            after[i][j] = temp->board[i][j];

    if(are_board_equals(after, before)) {
        delete_board(before);
        delete_board(after);
        delete_board(copy);
        std::cout << "valid_move ended" << std::endl;
        return false;
    }
    moves_x = get_all_moves(1, before, Player::piece::x);
    for(int i = 0; ((i < 40) && (moves_x[i].moving != Player::piece::e)); i++) {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                copy[i][j] = before[i][j];
        if(are_board_equals(after, create_board(copy, moves_x[i])))
            res = true;
    }
    moves_o = get_all_moves(-1, before, Player::piece::o);
    for(int i = 0; ((i < 40) && (moves_o[i].moving != Player::piece::e) && (!res)); i++) {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                copy[i][j] = before[i][j];
        if(are_board_equals(after, create_board(copy, moves_o[i])))
            res = true;
    }
    delete_board(before);
    delete_board(after);
    delete_board(copy);
    delete[] moves_x;
    delete[] moves_o;
    std::cout << "valid_move ended" << std::endl;
    return res;
}
/**
 * @brief delete the last board in the history
 */
void Player::pop(){
    std::cout << "pop called" << std::endl;
    if(!this->pimpl->board )
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};
    if (!this->pimpl->next && this->pimpl->index == 0)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};
    Impl* temp0 = this->pimpl;
    if (temp0->next == nullptr) {
        delete_board(temp0->board);
        delete temp0;
        std::cout << "pop ended" << std::endl;
        pimpl->next= nullptr;
        pimpl->board=nullptr;
        return;
    }
    while(temp0->next->next)temp0 = temp0->next;
    Impl* temp1 = temp0;
    temp0=temp0->next;
    temp1->next= nullptr;
    delete_board(temp0->board);
    delete temp0;
    std::cout << "pop ended" << std::endl;
}
/**
 * @brief check if player_nr is the winner
 * @param player_nr the player number 1(x) or 2(o)
 */
bool Player::wins(int player_nr) const {
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }
    if(last_index+1==0 || ((player_nr != 1) && (player_nr != 2)))
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};
    Player::piece player[2];
    if(player_nr == 2){
        player[0] = x;
        player [1] = X;
    }
    else {
        player[0] = o;
        player[1]=O;
    }
    return (get_left(temp->board, player)==0) ?  true : false;
}
/**
 * @brief check if the game is wan by player this
 */
bool Player::wins() const {
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }
    if(last_index+1==0)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};
    Player::piece player[2];
    if(temp->player_nr == 2){
        player[0] = x;
        player [1] = X;
    }
    else {
        player[0] = o;
        player[1]=O;
    }
    return (get_left(temp->board, player)==0) ?  true : false;
}
/**
 * @brief check if player_nr is the loser
 * @param player_nr the player number 1(x) or 2(o)
 */
bool Player::loses(int player_nr) const {
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }
    if(last_index+1==0 || ((player_nr != 1) && (player_nr != 2)))
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};
    Player::piece player[2];
    if(player_nr == 1){
        player[0] = x;
        player [1] = X;
    }
    else {
        player[0] = o;
        player[1]=O;
    }
    return (get_left(temp->board, player)==0) ? true : false;
}
/**
 * @brief check if the game is losed by player this
 */
bool Player::loses() const {
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }
    if(last_index+1==0)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};
    Player::piece player[2];
    if(temp->player_nr == 1){
        player[0] = x;
        player [1] = X;
    }
    else {
        player[0] = o;
        player[1]=O;
    }
    return (get_left(temp->board, player)==0) ? true: false;
}
/**
 * @brief give the number of recurrence of the last board in the history
 */
int Player::recurrence() const{
    std::cout << "recurrence called" << std::endl;
    if(!this->pimpl->board)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};
    int count=0;
    Impl* temp = this->pimpl;
    while (temp->next)
        temp=temp->next;
    Player::piece** board = temp->board;
    temp = this->pimpl;
    while (temp->next){
        if(are_board_equals(board, temp->board)) count++;
        temp=temp->next;
    }
    std::cout << "recurrence ended" << std::endl;
    return count+1;
}

