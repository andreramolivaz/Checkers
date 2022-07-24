#include "player.hpp"

static const int SIZE = 8;
static const int MAX_MOVES = 40;

//--------------------------------------------------------

char toChar(Player::piece p);

void get_piece_left(Player::piece** b, int (&res)[4]);

//--------------------------------------------------------

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
            std::cout<<toChar(piece_moving)<<": "<<"("<<start.first<<", "<<start.second<<") --> ("<<finish.first<<", "<<finish.second<<")"<<std::endl;
        else
            std::cout<<toChar(piece_moving)<<": "<<"("<<start.first<<", "<<start.second<<") --> ("<<finish.first<<", "<<finish.second<<") -- eating in: ("<<eat.first<<", "<<eat.second<<")"<<std::endl;
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

//--------------------------------------------------------

bool compare_matrices(Player::piece** m1, Player::piece** m2) {

    bool res = true;

    for(int i = 0; ((i < SIZE) && (res)); i++) {
        for(int j = 0; ((j < SIZE) && (res)); j++) {
            if(m1[i][j] != m2[i][j])
                res = false;
        }
    }

    return res;
}

int get_player_number(Player::piece p) {
    if(p == Player::piece::x)
        return 1;
    else
        return 2;
}

bool file_exists (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

Player::piece get_dame(Player::piece p) {
    if(p == Player::piece::x)
        return Player::piece::X;
    else
        return Player::piece::O;
}

char toChar(Player::piece p) {

    char res = ' ';

    switch(p) {
        case Player::piece::x:
            res = 'x';
            break;
        case Player::piece::X:
            res = 'X';
            break;
        case Player::piece::o:
            res = 'o';
            break;
        case Player::piece::O:
            res = 'O';
            break;
        case Player::piece::e:
            res = ' ';
            break;
    }

    return res;
}

void print_board(Player::piece** b) {
    for(int i = (SIZE - 1); i >= 0; i--) {
        for(int j = 0; j < SIZE; j++) {
            std::cout<<toChar(b[i][j]);
        }
        std::cout<<std::endl;
    }
}

Player::piece toPiece(char c) {

    Player::piece res = Player::piece::e;

    switch(c) {
        case 'x':
            res = Player::piece::x;
            break;
        case 'X':
            res = Player::piece::X;
            break;
        case 'o':
            res = Player::piece::o;
            break;
        case 'O':
            res = Player::piece::O;
            break;
        case ' ':
            res = Player::piece::e;
            break;
    }

    return res;
}

Player::piece** create_matrix() {
    Player::piece** array2D = 0;
    array2D = new Player::piece*[SIZE];

    for (int h = 0; h < SIZE; h++) {
        array2D[h] = new Player::piece[SIZE];
    }

    return array2D;
}

void delete_matrix(Player::piece** (&b)) {
    for(int i = 0; i < SIZE; i++) {
        delete [] b[i];
    }
    delete [] b;
}

Player::piece next_player(Player::piece p) {
    if(p == Player::piece::x)
        return Player::piece::o;
    else
        return Player::piece::x;
}

int get_direction(Player::piece p) {
    if(p == Player::piece::x)
        return 1;
    else
        return -1;
}

void get_piece_left(Player::piece** b, int (&res)[4]) {

    //0 -> tutti pezzi di p1 (x e X)
    //1 -> X
    //2 -> tutti pezzi di p2 (o e O)
    //3 -> O

    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(b[i][j] == Player::piece::x)
                res[0]++;
            else if(b[i][j] == Player::piece::X) {
                res[0]++;
                res[1]++;
            }
            else if(b[i][j] == Player::piece::o)
                res[2]++;
            else if(b[i][j] == Player::piece::O) {
                res[2]++;
                res[3]++;
            }
        }
    }
}

bool invalid_board(Player::piece** m) {

    bool res = false;
    int pieces[4] = {0};
    get_piece_left(m, pieces);

    if((!res) && ((pieces[0] > 12) || (pieces[2] > 12)))
        res = true;

    for(int i = 0; ((i < SIZE) && (!res)); i++) {
        for(int j = 0; ((j < SIZE) && (!res)); j++) {
            if((((i + j) % 2) == 0) && (m[i][j] != Player::piece::e))
                res = true;
        }
    }

    return res;
}

Move* get_all_moves(Player::piece** const b, Player::piece p, int direction) {

    Move* array = new Move[MAX_MOVES];
    int before_x = 0, before_y = 0, after_x = 0, after_y = 0, index = 0;
    bool found = false;
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
                        found = true;
                        index++;
                    }//guardo se posso mangiare
                    else if(b[after_x][after_y] == enemy_pawn) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x + direction) >= 0) && ((after_x + direction) < SIZE) && ((after_y - direction) >= 0) && ((after_y - direction) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x + direction][after_y - direction] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x + direction), (after_y - direction), after_x, after_y, p);
                                found = true;
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
                        found = true;
                        index++;
                    }//guardo se posso mangiare
                    else if(b[after_x][after_y] == enemy_pawn) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x + direction) >= 0) && ((after_x + direction) < SIZE) && ((after_y + direction) >= 0) && ((after_y + direction) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x + direction][after_y + direction] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x + direction), (after_y + direction), after_x, after_y, p);
                                found = true;
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
                        found = true;
                        index++;
                    }//guardo se posso mangiare
                    else if((b[after_x][after_y] == enemy_pawn) || (b[after_x][after_y] == enemy_dame)) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x + 1) >= 0) && ((after_x + 1) < SIZE) && ((after_y - 1) >= 0) && ((after_y - 1) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x + 1][after_y - 1] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x + 1), (after_y - 1), after_x, after_y, p_dame);
                                found = true;
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
                        found = true;
                        index++;
                    }//guardo se posso mangiare
                    else if((b[after_x][after_y] == enemy_pawn) || (b[after_x][after_y] == enemy_dame)) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x + 1) >= 0) && ((after_x + 1) < SIZE) && ((after_y + 1) >= 0) && ((after_y + 1) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x + 1][after_y + 1] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x + 1), (after_y + 1), after_x, after_y, p_dame);
                                found = true;
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
                        found = true;
                        index++;
                    }//guardo se posso mangiare
                    else if((b[after_x][after_y] == enemy_pawn) || (b[after_x][after_y] == enemy_dame)) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x - 1) >= 0) && ((after_x - 1) < SIZE) && ((after_y - 1) >= 0) && ((after_y - 1) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x - 1][after_y - 1] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x - 1), (after_y - 1), after_x, after_y, p_dame);
                                found = true;
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
                        found = true;
                        index++;
                    }//guardo se posso mangiare
                    else if((b[after_x][after_y] == enemy_pawn) || (b[after_x][after_y] == enemy_dame)) {
                        //guardo se le nuove coordinate vanno bene
                        if(((after_x - 1) >= 0) && ((after_x - 1) < SIZE) && ((after_y + 1) >= 0) && ((after_y + 1) < SIZE)) {
                            //guardo se è libero dopo il salto
                            if((b[after_x - 1][after_y + 1] == Player::piece::e)) {
                                array[index].create_move(before_x, before_y, (after_x - 1), (after_y + 1), after_x, after_y, p_dame);
                                found = true;
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

bool is_valid_move(Player::piece** const b, Player::piece p, Move m) {

    Move* moves = get_all_moves(b, p, get_direction(p));
    bool res = false;

    for(int i = 0; ((i < MAX_MOVES) && (!res)); i++) {
        if((moves[i].start != std::make_pair(0,0)) && (m == moves[i]))
            res = true;
    }

    delete[] moves;

    return res;
}

int evaluate(Player::piece** b, Player::piece player_pawn) {

    int player_pieces_left, enemy_pieces_left, player_dames_left, enemy_dames_left;
    int pieces_left[4] = {0};
    get_piece_left(b, pieces_left);

    if(player_pawn == Player::piece::x) {
        player_pieces_left = pieces_left[0];
        enemy_pieces_left = pieces_left[2];
        player_dames_left = pieces_left[1];
        enemy_dames_left = pieces_left[3];
    }
    else {
        player_pieces_left = pieces_left[2];
        enemy_pieces_left = pieces_left[0];
        player_dames_left = pieces_left[3];
        enemy_dames_left = pieces_left[1];
    }

    //print_board(b);

    //std::cout<<"------> player_pawns_left: "<<player_pawns_left<<" - enemy_pawns_left: "<<enemy_pawns_left<<" - player_dames_left: "<<player_dames_left<<" - enemy_dames_left: "<<enemy_dames_left<<std::endl;

    return (2*(player_pieces_left - enemy_pieces_left) + (player_dames_left - enemy_dames_left));
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

void copy_board(Player::piece** (&dest), Player::piece** source) {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

Move get_random_move(Player::piece** board, Player::piece player) {

    Move* moves = get_all_moves(board, player, get_direction(player));
    Move final_move;

    int size = 0, random_index = 0;

    for(int i = 0; ((i < MAX_MOVES) && (moves[i].piece_moving != Player::piece::e)); i++)
        size++;

    if(size != 0) {
        std::srand(time(NULL));
        random_index = (std::rand() % size);
        final_move = moves[random_index];
    }

    delete[] moves;

    return final_move;
}

int minimax(Player::piece** position, int depth, Player::piece max_player, Player::piece next, Move& move_return, int alpha, int beta) {
    if(depth == 0)
        return evaluate(position, next_player(next));

    if(max_player == next) {
        int max_evaluation = -1000;
        Player::piece** p = create_matrix();
        copy_board(p, position);
        Move* moves = get_all_moves(p, max_player, get_direction(max_player));
        Move best;
        for(int i = 0; ((i < MAX_MOVES) && (moves[i].piece_moving != Player::piece::e)); i++) {
            copy_board(p, position);

            int evaluation = minimax(create_board(p, moves[i]), (depth - 1), max_player, next_player(max_player), best, alpha, beta);
            if(evaluation > max_evaluation) {
                max_evaluation = evaluation;
                best = moves[i];

                alpha = std::max(alpha, max_evaluation);
                if (beta <= alpha)
                    break;
            }
        }

        move_return = best;

        delete_matrix(p);
        delete[] moves;

        return max_evaluation;
    }
    else {
        int min_evaluation = 1000;
        Player::piece** p = create_matrix();
        copy_board(p, position);
        Move* moves = get_all_moves(p, next, get_direction(next));
        Move best;
        for(int i = 0; ((i < MAX_MOVES) && (moves[i].piece_moving != Player::piece::e)); i++) {
            copy_board(p, position);

            int evaluation = minimax(create_board(p, moves[i]), (depth - 1), max_player, max_player, best, alpha, beta);
            if(evaluation < min_evaluation) {
                min_evaluation = evaluation;

                beta = std::min(beta, min_evaluation);
                if (beta <= alpha)
                    break;
            }
        }

        delete_matrix(p);
        delete[] moves;

        return min_evaluation;
    }
}

Move get_best_move(Player::piece** board, Player::piece max) {
    Move best;
    minimax(board, 5, max, max, best, -1000, 1000);
    return best;
}

//--------------------------------------------------------

class List {
    public:
        List() {
            head = nullptr;
            size = 0;
        }

        List(const List& s) {
            head = copy(s.head);
            size = s.size;
        }

        ~List() {
            destroy(head);
        }

        void append(Player::piece** b) {
            if(head == nullptr)
                prepend(b);
            else {
                Cell* pc = head;
                while (pc->next != nullptr)
                    pc = pc->next;
                pc->next = new Cell;
                pc->next->board = create_matrix();
                copy_board(pc->next->board, b);
                pc->next->next = nullptr;
                size++;
            }
        }

        void prepend(Player::piece** b) {
            Cell* pc = new Cell;
            pc->board = create_matrix();
            copy_board(pc->board, b);
            pc->next = head;
            head = pc;
            size++;
        }

        bool empty() const {
            return (size == 0);
        }

        Player::piece** last() {
            if(!head)
                return nullptr;
            else {
                Cell* pc = head;
                while (pc->next != nullptr)
                    pc = pc->next;
                
                return pc->board;
            }
        }

        Player::piece** first() {
            return head->board;
        }

        void pop() {
            Cell* pc = head;
            delete_matrix(pc->board);
            head = pc->next;
            delete pc;
            size--;
        }

        Player::piece** at(int index) {
            Cell* pc = head;
            while ((pc->next != nullptr) && (index > 0)) {
                pc = pc->next;
                index--;
            }

            if (pc)
                return pc->board;
            else
                return nullptr;
        }

        void print() {
            if(head != nullptr)  {
                Cell* pc = head;
                while (pc != nullptr) {
                    print_board(pc->board);
                    std::cout<<"--------------------------"<<std::endl;
                    pc = pc->next;
                }
            }
            else
                std::cout<<"EMPTY HISTORY"<<std::endl;
        }

        int get_size() {
            return size;
        }

        List& operator=(const List& x) {
            if (this != &x) {
                destroy(head);
                head = copy(x.head);
                size = x.size;
            }
            return *this;
        }

    private:
        struct Cell {
            Player::piece** board;
            Cell* next;
        };

        Cell* head;
        int size;

        void destroy(Cell* pc) {
            if (pc) {
                destroy(pc->next);
                delete_matrix(pc->board);
                delete pc;
            }
        }

        Cell* copy(Cell* source) const {
            if (source == nullptr)
                return nullptr;
            else {
                Cell* dest = new Cell;
                dest->board = create_matrix();
                copy_board(dest->board, source->board);
                dest->next = copy(source->next);
                return dest;
            }
        }
};

//--------------------------------------------------------

struct Player::Impl {
    List history;
    piece pawn;
    piece dame;
};

Player::Player(int player_nr) {

    if((player_nr != 1) && (player_nr != 2))
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number"};

    pimpl = new Impl;
    
    if(player_nr == 1) {
        pimpl->pawn = x;
        pimpl->dame = X;
    }
    else {
        pimpl->pawn = o;
        pimpl->dame = O;
    }
}

Player::~Player() {
    delete pimpl;
}

Player::Player(const Player& p) {
    pimpl = new Impl;
    pimpl->history = p.pimpl->history;
    pimpl->dame = p.pimpl->dame;
    pimpl->pawn = p.pimpl->pawn;
}

Player& Player::operator=(const Player& p) {
    if(this != &p) {
        delete pimpl;
        pimpl = new Impl;
        pimpl->history = p.pimpl->history;
        pimpl->dame = p.pimpl->dame;
        pimpl->pawn = p.pimpl->pawn;
    }
    return *this;
}

Player::piece Player::operator()(int r, int c, int history_offset) const {

    if((r < 0) || (r >= SIZE) || (c < 0) || (c >= SIZE) || (history_offset < 0) || (history_offset > (pimpl->history.get_size() - 1)))
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid coordinates or offset"};

    return (pimpl->history.at(history_offset)[r][c]);
}

void Player::load_board(const std::string& filename) {
    
    if(!file_exists(filename) || (filename.substr(filename.length() - 4) != ".txt"))
        throw player_exception{player_exception::missing_file, "ERROR: missing file or wrong file extension"};
    
    std::fstream file(filename, std::fstream::in);
    char c;
    int i = (SIZE - 1), j = 0, index = 0;
    piece** m = create_matrix();
    
    while(file.get(c)) {
        if(c != '\n') {
            if((index % 2) == 0) {
                m[i][j] = toPiece(c);

                j++;
                if(j == SIZE) {
                    j = 0;
                    i--;
                }
            }
            else if(((index % 2) != 0) && (c != ' ')) {
                delete_matrix(m);
                throw player_exception{player_exception::invalid_board, "ERROR: invalid board"};
            }

            index++;
        }
        else
            index = 0;
    }

    file.close();

    if(invalid_board(m)) {
        delete_matrix(m);
        throw player_exception{player_exception::invalid_board, "ERROR: invalid board"};
    }
    
    pimpl->history.prepend(m);
    delete_matrix(m);
}

void Player::store_board(const std::string& filename, int history_offset) const {

    if((history_offset < 0) || (history_offset > (pimpl->history.get_size() - 1)))
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid offset"};

    std::fstream file;
    file.open(filename, std::fstream::out);

    for(int i = (SIZE - 1); i >= 0; i--) {
        for(int j = 0; j < SIZE; j++) {
            file << toChar(pimpl->history.at(history_offset)[i][j]);
            if(j != (SIZE - 1))
                file << ' ';
        }
        if(i != 0)
            file << "\n";
    }

    file.close();
}

void Player::init_board(const std::string& filename) const {

    piece** m = create_matrix();
    std::fstream file;

    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {

            if((i >= 0) && (i <= 2)) {
                if(((i + j) % 2) != 0)
                    m[i][j] = piece::x;
                else
                    m[i][j] = piece::e;
            }
            else if((i >= 5) && (i <= 7)) {
                if(((i + j) % 2) != 0)
                    m[i][j] = piece::o;
                else
                    m[i][j] = piece::e;
            }
            else
                m[i][j] = piece::e;
        }
    }

    file.open(filename, std::fstream::out);
    
    for(int i = (SIZE - 1); i >= 0; i--) {
        for(int j = 0; j < SIZE; j++) {
            file << toChar(m[i][j]);
            if(j != (SIZE - 1))
                file << ' ';
        }
        if(i != 0)
            file << "\n";
    }

    file.close();
    delete_matrix(m);
}

void Player::move() {

    if(pimpl->history.empty())
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};

    Player::piece** copy = create_matrix();
    copy_board(copy, pimpl->history.first());

    Move best_move;

    //debug
    // if(pimpl->pawn == piece::x)
    //     best_move = get_best_move(copy, pimpl->pawn);
    // else
    //     best_move = get_random_move(copy, pimpl->pawn);

    //real code
    best_move = get_best_move(copy, pimpl->pawn);
    
    copy_board(copy, pimpl->history.first());
    pimpl->history.prepend(create_board(copy, best_move));

    delete_matrix(copy);

    /*Player::piece** copy = create_matrix();
    copy_board(copy, pimpl->history.at(0));
    Move* m = get_all_moves(copy, Player::piece::x, 1);

    for(int i = 0; i < MAX_MOVES; i++) {
        if((m[i].piece_moving != piece::e) && (is_valid_move(copy, pimpl->pawn, m[i])))
            m[i].print();
    }

    for(int i = 0; i < MAX_MOVES; i++) {
        if(m[i].piece_moving != piece::e) {
            copy_board(copy, pimpl->history.last());
            print_board(create_board(copy, m[i]));
            std::cout<<"--------------------"<<std::endl;
        }
    }*/
}

bool Player::valid_move() const {

    if(pimpl->history.get_size() < 2)
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: too few elements in history"};

    bool res = false;
    
    Player::piece** before = create_matrix();
    Player::piece** after = create_matrix();
    Player::piece** copy = create_matrix();
    Move* moves_x;
    Move* moves_o;

    copy_board(before, pimpl->history.at(1));
    copy_board(after, pimpl->history.at(0));

    if(compare_matrices(after, before)) {
        delete_matrix(before);
        delete_matrix(after);
        delete_matrix(copy);
        return res;
    }

    //controllo le mosse del player 1
    moves_x = get_all_moves(before, Player::piece::x, 1);

    for(int i = 0; ((i < MAX_MOVES) && (moves_x[i].piece_moving != Player::piece::e) && (!res)); i++) {
        copy_board(copy, before);
        if(compare_matrices(after, create_board(copy, moves_x[i])))
            res = true;
    }

    //controllo le mosse del player 2
    moves_o = get_all_moves(before, Player::piece::o, -1);

    for(int i = 0; ((i < MAX_MOVES) && (moves_o[i].piece_moving != Player::piece::e) && (!res)); i++) {
        copy_board(copy, before);
        if(compare_matrices(after, create_board(copy, moves_o[i])))
            res = true;
    }

    delete_matrix(before);
    delete_matrix(after);
    delete_matrix(copy);
    delete[] moves_x;
    delete[] moves_o;

    return res;
}

void Player::pop() {

    if(pimpl->history.empty())
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};

    pimpl->history.pop();
}

bool Player::wins(int player_nr) const {

    if((pimpl->history.empty()) || ((player_nr != 1) && (player_nr != 2)))
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};

    bool res;
    
    int pieces_left[4] = {0};
    get_piece_left(pimpl->history.at(0), pieces_left);

    if(player_nr == 1) {
        if((pieces_left[2] == 0) && (pieces_left[3] == 0))
            res = true;
        else
            res = false;
    }
    else {
        if((pieces_left[0] == 0) && (pieces_left[1] == 0))
            res = true;
        else
            res = false;
    }

    return res;
}

bool Player::wins() const {

    if(pimpl->history.empty())
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};

    return wins(get_player_number(pimpl->pawn));
}

bool Player::loses(int player_nr) const {

    if((pimpl->history.empty()) || ((player_nr != 1) && (player_nr != 2)))
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: invalid player number or empty history"};
        
    bool res;
    
    int pieces_left[4] = {0};
    get_piece_left(pimpl->history.at(0), pieces_left);

    if(player_nr == 1) {
        if((pieces_left[0] == 0) && (pieces_left[1] == 0))
            res = true;
        else
            res = false;
    }
    else {
        if((pieces_left[2] == 0) && (pieces_left[3] == 0))
            res = true;
        else
            res = false;
    }

    return res;
}

bool Player::loses() const {

    if(pimpl->history.empty())
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};

    return loses(get_player_number(pimpl->pawn));
}

int Player::recurrence() const {

    if(pimpl->history.empty())
        throw player_exception{player_exception::index_out_of_bounds, "ERROR: empty history"};

    int size = pimpl->history.get_size();
    int count = 0;

    for(int i = 0; i < size; i++) {
        if(compare_matrices(pimpl->history.at(0), pimpl->history.at(i)))
            count++;
    }

    return count;
}

//--------------------------------------------------------

