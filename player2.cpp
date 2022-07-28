#include "player.hpp"

static const int SIZE = 8;

char to_char(Player::piece p){
    switch (p) {
        case Player::piece::x:
            return 'x';
        case Player::piece::X:
            return 'X';
        case Player::piece::o:
            return 'o';
        case Player::piece::O:
            return 'O';
        case Player::piece::e:
            return ' ';
    }
    return ' ';
}

Player::piece to_piece(char c){
    switch (c) {
        case 'x':
            return Player::piece::x;
        case 'X':
            return Player::piece::X;
        case 'o':
            return Player::piece::o;
        case 'O':
            return Player::piece::O;
        case ' ':
            return Player::piece::e;
    }
    return Player::piece::e;
}

int get_pieces_left (Player::piece** board, Player::piece p[2]){
    int res=0;
    for (int i=0; i < SIZE; i++)
        for (int j=0; j < SIZE; j++)
            if(board[i][j]==p[0] || board[i][j]==p[1] )res++;
    return res;
}

struct Move {
    std::pair<int, int> start;
    std::pair<int, int> finish;
    std::pair<int, int> eat;
    Player::piece piece_moving;

    Move() {
        start = std::make_pair(0,0);
        finish = std::make_pair(0,0);
        eat = std::make_pair(0,0);
        piece_moving = Player::piece::e;
    }

    void print() {
        if(eat == std::make_pair(0,0))
            std::cout<<to_char(piece_moving)<<": "<<"("<<start.first<<", "<<start.second<<") --> ("<<finish.first<<", "<<finish.second<<")"<<std::endl;
        else
            std::cout<<to_char(piece_moving)<<": "<<"("<<start.first<<", "<<start.second<<") --> ("<<finish.first<<", "<<finish.second<<") -- eating in: ("<<eat.first<<", "<<eat.second<<")"<<std::endl;
    }

    void create_move(int start_x, int start_y, int finish_x, int finish_y, int eaten_x, int eaten_y, Player::piece p) {
        start = std::make_pair(start_x, start_y);
        finish = std::make_pair(finish_x, finish_y);
        eat = std::make_pair(eaten_x, eaten_y);
        piece_moving = p;
    }

    bool operator==(Move m) {
        bool res = false;

        if((this->eat == m.eat) && (this->finish == m.finish) && (this->start == m.start) && (this->piece_moving == m.piece_moving))
            res = true;

        return res;
    }
};

Player::piece get_dame(Player::piece p) {
    if(p == Player::piece::x)
        return Player::piece::X;
    else
        return Player::piece::O;
}

Player::piece next_player(Player::piece p) {
    if(p == Player::piece::x)
        return Player::piece::o;
    else
        return Player::piece::x;
}

Move* get_all_moves(Player::piece** const b, Player::piece p, int direction) {

    Move* array = new Move[40];
    int before_x = 0, before_y = 0, after_x = 0, after_y = 0, index = 0;

    Player::piece p_dame = get_dame(p), enemy_dame = (get_dame(next_player(p))), enemy_pawn = next_player(p);

    for(int j = 0; j < SIZE; j++) {
        for(int k = 0; k < SIZE; k++) {

            //mi salvo le vecchie coordinate
            before_x = j;
            before_y = k;

            //se trovo un pezzo normale
            if(b[before_x][before_y] == p) {

                //provo a saltare a sinistra
                //mi salvo le nuove coordinate
                after_x = before_x + direction;
                after_y = before_y - direction;

                //guardo se le coordinate vanno bene
                if((after_x >= 0) && (after_x < SIZE) && (after_y >= 0) && (after_y < SIZE)) {
                    //guardo se è libero
                    if(b[after_x][after_y] == Player::piece::e) {
                        array[index].create_move(before_x, before_y, after_x, after_y, 0, 0, p);

                        index++;
                    }//guardo se posso mangiare
                    else if(b[after_x][after_y] == enemy_pawn) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x + direction) >= 0) && ((after_x + direction) < SIZE) && ((after_y - direction) >= 0) && ((after_y - direction) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x + direction][after_y - direction] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x + direction), (after_y - direction), after_x, after_y, p);

                                index++;
                            }
                        }
                    }
                }

                //provo a saltare a destra
                //mi salvo le nuove coordinate
                after_x = before_x + direction;
                after_y = before_y + direction;

                //guardo se le coordinate vanno bene
                if((after_x >= 0) && (after_x < SIZE) && (after_y >= 0) && (after_y < SIZE)) {
                    //guardo se è libero
                    if(b[after_x][after_y] == Player::piece::e) {
                        array[index].create_move(before_x, before_y, after_x, after_y, 0, 0, p);

                        index++;
                    }//guardo se posso mangiare
                    else if(b[after_x][after_y] == enemy_pawn) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x + direction) >= 0) && ((after_x + direction) < SIZE) && ((after_y + direction) >= 0) && ((after_y + direction) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x + direction][after_y + direction] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x + direction), (after_y + direction), after_x, after_y, p);

                                index++;
                            }
                        }
                    }
                }
            }//se trovo una dama
            else if(b[before_x][before_y] == p_dame) {

                //provo a saltare in alto a sinistra
                //mi salvo le nuove coordinate
                after_x = before_x + 1;
                after_y = before_y - 1;

                //guardo se le coordinate vanno bene
                if((after_x >= 0) && (after_x < SIZE) && (after_y >= 0) && (after_y < SIZE)) {
                    //guardo se è libero
                    if(b[after_x][after_y] == Player::piece::e) {
                        array[index].create_move(before_x, before_y, after_x, after_y, 0, 0, p_dame);

                        index++;
                    }//guardo se posso mangiare
                    else if((b[after_x][after_y] == enemy_pawn) || (b[after_x][after_y] == enemy_dame)) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x + 1) >= 0) && ((after_x + 1) < SIZE) && ((after_y - 1) >= 0) && ((after_y - 1) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x + 1][after_y - 1] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x + 1), (after_y - 1), after_x, after_y, p_dame);

                                index++;
                            }
                        }
                    }
                }

                //provo a saltare in alto a destra
                //mi salvo le nuove coordinate
                after_x = before_x + 1;
                after_y = before_y + 1;

                //guardo se le coordinate vanno bene
                if((after_x >= 0) && (after_x < SIZE) && (after_y >= 0) && (after_y < SIZE)) {
                    //guardo se è libero
                    if(b[after_x][after_y] == Player::piece::e) {
                        array[index].create_move(before_x, before_y, after_x, after_y, 0, 0, p_dame);

                        index++;
                    }//guardo se posso mangiare
                    else if((b[after_x][after_y] == enemy_pawn) || (b[after_x][after_y] == enemy_dame)) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x + 1) >= 0) && ((after_x + 1) < SIZE) && ((after_y + 1) >= 0) && ((after_y + 1) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x + 1][after_y + 1] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x + 1), (after_y + 1), after_x, after_y, p_dame);

                                index++;
                            }
                        }
                    }
                }

                //provo a saltare in basso a sinistra
                //mi salvo le nuove coordinate
                after_x = before_x - 1;
                after_y = before_y - 1;

                //guardo se le coordinate vanno bene
                if((after_x >= 0) && (after_x < SIZE) && (after_y >= 0) && (after_y < SIZE)) {
                    //guardo se è libero
                    if(b[after_x][after_y] == Player::piece::e) {
                        array[index].create_move(before_x, before_y, after_x, after_y, 0, 0, p_dame);

                        index++;
                    }//guardo se posso mangiare
                    else if((b[after_x][after_y] == enemy_pawn) || (b[after_x][after_y] == enemy_dame)) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x - 1) >= 0) && ((after_x - 1) < SIZE) && ((after_y - 1) >= 0) && ((after_y - 1) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x - 1][after_y - 1] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x - 1), (after_y - 1), after_x, after_y, p_dame);

                                index++;
                            }
                        }
                    }
                }

                //provo a saltare in basso a destra
                //mi salvo le nuove coordinate
                after_x = before_x - 1;
                after_y = before_y + 1;

                //guardo se le coordinate vanno bene
                if((after_x >= 0) && (after_x < SIZE) && (after_y >= 0) && (after_y < SIZE)) {
                    //guardo se è libero
                    if(b[after_x][after_y] == Player::piece::e) {
                        array[index].create_move(before_x, before_y, after_x, after_y, 0, 0, p_dame);

                        index++;
                    }//guardo se posso mangiare
                    else if((b[after_x][after_y] == enemy_pawn) || (b[after_x][after_y] == enemy_dame)) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x - 1) >= 0) && ((after_x - 1) < SIZE) && ((after_y + 1) >= 0) && ((after_y + 1) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x - 1][after_y + 1] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x - 1), (after_y + 1), after_x, after_y, p_dame);

                                index++;
                            }
                        }
                    }
                }
            }
        }
    }

    return array;
}

Move get_random_move(Player::piece** board, Player::piece player) {

    int direction=0;
    if(player == Player::piece::x) direction = 1;
    else direction = -1;

    Move* moves = get_all_moves(board, player, direction);
    Move final_move;

    int size = 0, random_index = 0;

    for(int i = 0; ((i < 40) && (moves[i].piece_moving != Player::piece::e)); i++)
        size++;

    if(size != 0) {
        std::srand(time(NULL));
        random_index = (std::rand() % size);
        final_move = moves[random_index];
    }

    delete[] moves;

    return final_move;
}

bool are_board_equals(Player::piece** m1, Player::piece** m2) {
    bool res = true;
    for(int i = 0; ((i < SIZE) && (res)); i++) {
        for(int j = 0; ((j < SIZE) && (res)); j++) {
            if(m1[i][j] != m2[i][j])
                res = false;
        }
    }
    return res;
}

Player::piece** create_board(Player::piece** const b, Move m) {
    Player::piece** res = b;

    //dove parte diventa vuoto
    res[m.start.first][m.start.second] = Player::piece::e;

    //controllo se il pezzo è x
    if(m.piece_moving == Player::piece::x) {
        //controllo se diventa una dama o no
        if(m.finish.first == (SIZE - 1))
            res[m.finish.first][m.finish.second] = Player::piece::X;
        else
            res[m.finish.first][m.finish.second] = Player::piece::x;
    }//controllo se il pezzo è o
    else if(m.piece_moving == Player::piece::o) {
        //controllo se diventa una dama o no
        if(m.finish.first == 0)
            res[m.finish.first][m.finish.second] = Player::piece::O;
        else
            res[m.finish.first][m.finish.second] = Player::piece::o;
    }//controllo se il pezzo è altro (dama)
    else
        res[m.finish.first][m.finish.second] = m.piece_moving;

    //se ha mangiato il pezzo mangiato diventa vuoto
    //senno rimetto vuota la tessera (0,0)
    res[m.eat.first][m.eat.second] = Player::piece::e;

    return res;
}

void delete_board(Player::piece** (&board)){
    for(int i = 0; i < SIZE; i++) delete[] board[i];
    delete[] board;
}

bool file_exists(const std::string& filename){
    std::ifstream f(filename.c_str());
    return f.good();
}

Player::piece** initialize_board(){
    Player::piece** matrix = nullptr;
    matrix = new Player::piece*[SIZE];
    for(int i = 0; i < SIZE; i++)
        matrix[i] = new Player::piece[SIZE];

    return matrix;
}

struct Player::Impl{
    Impl* next;
    Player::piece** board; // the Dama board
    int index; // the index of the board
    int player_nr; // the player number
};

Player::Player(int player_nr) {

    std::cout << "constructor called" << std::endl;

    //checks if player number is valid otherwise throws an exception
    if (player_nr != 1 && player_nr != 2)
        throw player_exception{player_exception::index_out_of_bounds, "The player can only be 0 or 1"};

    pimpl = new Impl{nullptr, nullptr, 0, player_nr}; // initializes the memory

    std::cout << "constructor ended" << std::endl;

} // constructor

Player::~Player(){
    std::cout << "destructor called" << std::endl;
    // loops the list
    while(pimpl != nullptr){
        Impl* temp = pimpl; // saves the list address
        pimpl = pimpl->next; // goes to the next node
        if(temp->board != nullptr) {
            delete_board(temp->board);
        }
        delete temp; // deletes the memory
    }
    delete pimpl; // deletes the last memory
    std::cout << "destructor ended" << std::endl;
} // destructor

Player::Player(const Player& copy){
    std::cout << "copy constructor called" << std::endl;
    this->pimpl = new Impl{
            nullptr,
            initialize_board(),
            copy.pimpl->index,
            copy.pimpl->player_nr
    };

    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            this->pimpl->board[i][j] = copy.pimpl->board[i][j];
        }
    }

    Impl* copyTemp = copy.pimpl;
    Impl* thisTemp = this->pimpl;

    while(copyTemp->next) {
        thisTemp->next = new Impl{
                nullptr,
                initialize_board(),
                thisTemp->index + 1,
                copyTemp->player_nr
        };

        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                thisTemp->next->board[i][j] = copyTemp->next->board[i][j];
            }
        }
        thisTemp = thisTemp->next;
        copyTemp = copyTemp->next;
    }

    std::cout << "copy constructor ended" << std::endl;
}

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

        for(int i = 0; i < SIZE; i++) {
            for(int j = 0; j < SIZE; j++) {
                this->pimpl->board[i][j] = copy.pimpl->board[i][j];
            }
        }

        Impl* copyTemp = copy.pimpl;
        Impl* thisTemp = this->pimpl;

        while(copyTemp->next) {
            thisTemp->next = new Impl{
                    nullptr,
                    initialize_board(),
                    thisTemp->index + 1,
                    copyTemp->player_nr
            };

            for(int i = 0; i < SIZE; i++) {
                for(int j = 0; j < SIZE; j++) {
                    thisTemp->next->board[i][j] = copyTemp->next->board[i][j];
                }
            }
            thisTemp = thisTemp->next;
            copyTemp = copyTemp->next;
        }


    }
    std::cout << "operator= ended" << std::endl;
    return *this;
}

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

void Player::load_board(const std::string& filename){
    std::cout << "load board called" << std::endl;
    if(this->pimpl->next == nullptr)
        this->pimpl->board = initialize_board();
    Impl* temp = this->pimpl;
    int lastIndex = this->pimpl->index;
    // goes to the end of the player list
    while(temp->next) {
        lastIndex++;
        temp = temp->next;
    }
    if(!file_exists(filename))
        throw player_exception{player_exception::missing_file, "file not found"};
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
            throw player_exception{player_exception::invalid_board, "invalid board"};
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
            throw player_exception{player_exception::invalid_board, "invalid board"};
        }
        if(count_x>12 || count_o>12){
            delete_board(board);
            throw player_exception{player_exception::invalid_board, "invalid board"};
        }

    }
    file.close();
    //cout<<readCharacters;

    temp = this->pimpl;
    int last_index = this->pimpl->index;
    // goes to the end of the player list
    while(temp->next) {
        last_index++;
        temp = temp->next;
    }

    // adding the board to the last player memory
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

    //deleteBoard(temp->board);
    delete_board(board);
    //delete temp1;
    std::cout << "load board ended" << std::endl;
}

void Player::store_board(const std::string& filename, int history_offset) const{
    std::cout << "store_board called" << std::endl;
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }

    if (history_offset>last_index || history_offset < 0 || last_index+1==0 )
        throw player_exception{player_exception::index_out_of_bounds, "history_offset not valid"};

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

void Player::init_board(const std::string& filename) const{
    // initial board
    std::cout << "init_board called" << std::endl;

    if(this->pimpl->next == nullptr)
        this->pimpl->board = initialize_board();


    // allocates the memory
    Player::piece **initial_board = initialize_board();

    // fill starting board with the default field
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

    // goes to the end of the player memory
    Impl* temp = this->pimpl;
    int lastIndex = this->pimpl->index;
    // goes to the end of the player list
    while(temp->next) {
        lastIndex++;
        temp = temp->next;
    }

    temp->next = new Impl{
            nullptr,
            initialize_board(),
            lastIndex+1,
            this->pimpl->player_nr
    };
    temp = temp->next;

    // filling the board
    for(int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            temp->board[i][j] = initial_board[i][j];
        }
    }

    std::fstream file;
    file.open(filename, std::fstream::out);

    for(int i = SIZE - 1; i >= 0; i--) {
        for(int j = 0; j < SIZE; j++) {
            file << to_char(temp->board[i][j]);
            if(j != SIZE - 1) file << ' ';
        }
        if(i != 0)
            file << "\n";
    }

    file.close();
    // deletes the temporary variables

    delete_board(initial_board);
    //delete_board(temp1->board);
    std::cout << "init_board ended" << std::endl;
}

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
    if(temp->player_nr == 1) player = x;
    else player = o;

    best_move = get_random_move(temp_board, player);

    for (int i=0; i<SIZE ; i++)
        for (int j=0; j<SIZE; j++)
            temp_board[i][j]=temp->board[i][j];

    Player::piece** res = create_board(temp_board, best_move);


    temp = this->pimpl;
    last_index = this->pimpl->index;
    // goes to the end of the player list
    while(temp->next) {
        last_index++;
        temp = temp->next;
    }

    // adding the board to the last player memory
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


    //controllo le mosse del player 1
    moves_x = get_all_moves(before, Player::piece::x, 1);

    for(int i = 0; ((i < 40) && (moves_x[i].piece_moving != Player::piece::e)); i++) {
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                copy[i][j] = before[i][j];
        if(are_board_equals(after, create_board(copy, moves_x[i])))
            res = true;
    }

    //controllo le mosse del player 2
    moves_o = get_all_moves(before, Player::piece::o, -1);

    for(int i = 0; ((i < 40) && (moves_o[i].piece_moving != Player::piece::e) && (!res)); i++) {
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

void Player::pop(){
    std::cout << "pop called" << std::endl;
    if(!this->pimpl->board)
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

bool Player::wins(int player_nr) const {

    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }


    if(last_index+1==0 || ((player_nr != 1) && (player_nr != 2)))
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};

    bool res;
    Player::piece player[2];
    if(player_nr == 2){
        player[0] = x;
        player [1] = X;
    }
    else {
        player[0] = o;
        player[1]=O;
    }

    if(get_pieces_left(temp->board, player)==0) return true;
    else return false;
}

bool Player::wins() const {

    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }


    if(last_index+1==0)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};

    bool res;
    Player::piece player[2];
    if(temp->player_nr == 2){
        player[0] = x;
        player [1] = X;
    }
    else {
        player[0] = o;
        player[1]=O;
    }

    if(get_pieces_left(temp->board, player)==0) return true;
    else return false;
}

bool Player::loses(int player_nr) const {

    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }


    if(last_index+1==0 || ((player_nr != 1) && (player_nr != 2)))
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};

    bool res;
    Player::piece player[2];
    if(player_nr == 1){
        player[0] = x;
        player [1] = X;
    }
    else {
        player[0] = o;
        player[1]=O;
    }

    if(get_pieces_left(temp->board, player)==0) return true;
    else return false;
}

bool Player::loses() const {
    Impl* temp = this->pimpl;
    int last_index=-1;
    while (temp->next) {
        last_index = temp->index;
        temp=temp->next;
    }


    if(last_index+1==0)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};

    bool res;
    Player::piece player[2];
    if(temp->player_nr == 1){
        player[0] = x;
        player [1] = X;
    }
    else {
        player[0] = o;
        player[1]=O;
    }

    if(get_pieces_left(temp->board, player)==0) return true;
    else return false;
}

int Player::recurrence() const{
    std::cout << "recurrence called" << std::endl;
    if(!this->pimpl->board)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};
    int count=0;
    Impl* temp = this->pimpl;
    while (temp->next) {
        temp=temp->next;
    }
    Player::piece** board = temp->board;

    temp = this->pimpl;

    while (temp->next){
        if(are_board_equals(board, temp->board)) count++;
        temp=temp->next;
    }


    std::cout << "recurrence ended" << std::endl;

    return count+1;
}

