#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int STARTING_HEALTH = 100;
const int MAX_MONEY = 10000;
const int STARTING_MONEY = 1000;
const int WIN_PRIZE = 2700;
const int LOSE_PRIZE = 2400;

const string START_COMMAND = "start";
const string FIGHT_OUTPUT = "fight!\n";
const string ADD_PLAYER_COMMAND = "add-user";
const string DONE_OUTPUT = "ok\n";
const string GET_MONEY_COMMAND = "get-money";
const string GET_HEALTH_COMMAND = "get-health";
const string GO_AFK_COMMAND = "go-afk";
const string GO_ATK_COMMAND = "go-atk";
const string BUY_COMMAND = "buy";
const string INVALID_USER_OUTPUT = "user not available\n";
const string INVALID_WEAPON_OUTPUT = "weapon not available\n";
const string ALREADY_HAVE_WEAPON_OUTPUT = "you already have this weapon\n";
const string NOT_ENOUGH_MONEY_OUTPUT = "insufficient money\n";
const string WEAPON_BOUGHT_OUTPUT = "weapon bought successfully\n";
const string SHOOT_COMMAND = "shoot";
const string ATTACKER_DEAD_OUTPUT = "attacker is dead\n";
const string ATTACKE_DEAD_OUTPUT = "attacked is dead\n";
const string ATTACKER_DOES_NOT_HAVE_GUN_PUTPUT = "attacker doesn't have this gun\n";
const string FRIENDLY_FIRE_OUTPUT = "you can't shoot this player\n";
const string SUCCESFULL_SHOT = "nice shot\n";
const string SCOREBOARD_COMMAND = "score-board";
const string SCOREBOARD_SEPERATOR = " ";
const string ROUND_COMMAND = "round";
const string CAN_NOT_BUY_OUTPUT = "you can't buy weapons anymore\n";
const string GAME_NOT_STARTED_OUTPUT = "The game hasn't started yet\n";
const string COUNTER_TERRORIST_WIN = "counter-terrorist won\n";
const string TERRORIST_WIN = "terrorist won\n";
const string DEFAULT_WEAPON_NAME = "knife";
const string TERRORIST_INPUT = "terrorist";

const string TERRORIST_TEAM = "terrorist players:\n";
const string COUNTER_TERRORIST_TEAM = "counter-terrorist players:\n";
const string AFK = "afk";
const string ATK = "atk";

const int NOT_STARTED = 0;
const int STARTED = 1;

class Weapon
{
private:
    string name;
    int price;
    int damage;
    int prize;

public:
    Weapon(string _name, int _price, int _damage, int _prize);
    string get_name() const { return name; }
    int get_price() { return price; }
    int get_damage() { return damage; }
    int get_prize() { return prize; }
};

const Weapon HEAVY = Weapon("heavy", 3000, 45, 100);
const Weapon PISTOL = Weapon("pistol", 400, 20, 200);
const Weapon KNIFE = Weapon("knife", 0, 35, 500);
const vector<Weapon> VALID_WEAPONS = {HEAVY, PISTOL, KNIFE};

class Player
{
private:
    string name;
    string team;
    int health;
    int money;
    vector<Weapon> weapons;
    string state;
    int kills;
    int deaths;

public:
    Player(string input_name, string input_team);
    string get_name() const { return name; }
    int get_money() { return money; }
    int get_health() { return health; }
    int get_kills() { return kills; };
    int get_deaths() { return deaths; };
    void set_health(int new_health) { health = new_health; }
    void set_state(string new_state) { state = new_state; }
    string get_state() { return state; }
    string get_team() { return team; }
    void buy(string weapon_name, int game_state);
    bool is_weapon_valid(string weapon_name);
    bool does_have_weapon(Weapon weapon);
    bool has_enough_money(Weapon weapon);
    void add_weapon(Weapon weapon);
    bool is_alive();
    void inc_deaths() { deaths++; };
    void inc_kills() { kills++; };
    void lower_health(int damage);
    void add_money(int prize);
    void print_scores();
    void dec_money(int price);
    Weapon find_weapon(string weapon_name);
    bool operator<(Player p);
};

class CSGO
{
private:
    vector<Player> players;

public:
    void play_game();
    void round_command(string command, int &game_state);
    bool is_user_valid(string username);
    bool does_user_exist(string username);
    void shoot(string attacker_name, string attacked_name, string weapon_name, int game_state);
    bool are_teammates(Player player1, Player player2);
    void shot_successful(Player *attacker, Player *attacked, Weapon weapon);
    void scoreboard(string team);
    vector<Player> find_players_of_team(string team);
    void general_commands(string command, string username);
    void add_player();
    void end_game();
    int find_team_members(string team);
    void add_money_team(string team, int prize);
    Player *find_player_reference(string username);
    void play_round(int &game_state);
    void check_buy(string command, string username, int game_state);
    void check_shoot(string command, string username, int game_state);
};

bool Player::operator<(Player p)
{
    if (kills != p.kills)
        return kills > p.kills;
    else if (deaths != p.deaths)
        return deaths < p.deaths;
    else
        return name < p.name;
}

void Player::print_scores()
{
    cout << name << SCOREBOARD_SEPERATOR << kills << SCOREBOARD_SEPERATOR << deaths << endl;
}

void Player::dec_money(int price)
{
    money -= price;
}
Weapon Player::find_weapon(string weapon_name)
{
    auto iter = find_if(VALID_WEAPONS.begin(), VALID_WEAPONS.end(), [&weapon_name](const Weapon &w)
                        { return w.get_name() == weapon_name; });
    return VALID_WEAPONS[distance(VALID_WEAPONS.begin(), iter)];
}

Player *CSGO::find_player_reference(string username)
{
    auto iter = find_if(players.begin(), players.end(), [&username](const Player &p)
                        { return p.get_name() == username; });
    return &players[distance(players.begin(), iter)];
}
void Player::lower_health(int damage)
{
    health -= damage;
    if (health < 0)
    {
        health = 0;
        weapons.clear();
        Weapon default_weapon = find_weapon(DEFAULT_WEAPON_NAME);
        add_weapon(default_weapon);
    }
}

void Player::add_money(int prize)
{
    money += prize;
    if (money > MAX_MONEY)
        money = MAX_MONEY;
}

bool Player::is_alive()
{
    return health > 0;
}

bool CSGO::are_teammates(Player player1, Player player2)
{
    return player1.get_team() == player2.get_team();
}

void CSGO::shot_successful(Player *attacker, Player *attacked, Weapon weapon)
{
    attacked->lower_health(weapon.get_damage());
    if (!attacked->is_alive())
    {
        attacked->inc_deaths();
        attacker->inc_kills();
        attacker->add_money(weapon.get_prize());
    }
}
void CSGO::shoot(string attacker_name, string attacked_name, string weapon_name, int game_state)
{
    Player *attacker = find_player_reference(attacker_name);
    Player *attacked = find_player_reference(attacked_name);
    Weapon weapon = attacker->find_weapon(weapon_name);
    if (game_state == NOT_STARTED)
        cout << GAME_NOT_STARTED_OUTPUT;
    else if (!attacker->is_alive())
        cout << ATTACKER_DEAD_OUTPUT;
    else if (!attacked->is_alive())
        cout << ATTACKE_DEAD_OUTPUT;
    else if (!attacker->does_have_weapon(weapon))
        cout << ATTACKER_DOES_NOT_HAVE_GUN_PUTPUT;
    else if (are_teammates(*attacker, *attacked))
        cout << FRIENDLY_FIRE_OUTPUT;
    else
    {
        shot_successful(attacker, attacked, weapon);
        cout << SUCCESFULL_SHOT;
    }
}

void Player::add_weapon(Weapon weapon)
{
    weapons.push_back(weapon);
}

Weapon::Weapon(string input_name, int _price, int _damage, int _prize)
{
    name = input_name;
    price = _price;
    damage = _damage;
    prize = _prize;
}
bool Player::is_weapon_valid(string weapon_name)
{
    for (int i = 0; i < VALID_WEAPONS.size(); i++)
    {
        if (weapon_name == VALID_WEAPONS[i].get_name())
            return true;
    }
    return false;
}

bool Player::has_enough_money(Weapon weapon)
{
    return money >= weapon.get_price();
}

bool Player::does_have_weapon(Weapon weapon)
{
    for (int i = 0; i < weapons.size(); i++)
    {
        if (weapon.get_name() == weapons[i].get_name())
            return true;
    }
    return false;
}

void Player::buy(string weapon_name, int game_state)
{
    if (!is_weapon_valid(weapon_name))
        cout << INVALID_WEAPON_OUTPUT;
    else
    {
        Weapon new_weapon = find_weapon(weapon_name);
        if (game_state == STARTED)
            cout << CAN_NOT_BUY_OUTPUT;
        else if (does_have_weapon(new_weapon))
            cout << ALREADY_HAVE_WEAPON_OUTPUT;
        else if (!has_enough_money(new_weapon))
            cout << NOT_ENOUGH_MONEY_OUTPUT;
        else
        {
            add_weapon(new_weapon);
            dec_money(new_weapon.get_price());
            cout << WEAPON_BOUGHT_OUTPUT;
        }
    }
}

Player::Player(string input_name, string input_team)
{
    name = input_name;
    health = STARTING_HEALTH;
    money = STARTING_MONEY;
    state = ATK;
    kills = 0;
    deaths = 0;
    if (input_team == TERRORIST_INPUT)
        team = TERRORIST_TEAM;
    else
        team = COUNTER_TERRORIST_TEAM;
}

bool CSGO::does_user_exist(string username)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (username == players[i].get_name())
            return true;
    }
    return false;
}

bool CSGO::is_user_valid(string username)
{
    if (!does_user_exist(username))
        return false;
    else if (find_player_reference(username)->get_state() == AFK)
        return false;
    else
        return true;
}

vector<Player> CSGO::find_players_of_team(string team)
{
    vector<Player> team_players;
    for (int i = 0; i < players.size(); i++)
    {
        if (team == players[i].get_team())
            team_players.push_back(players[i]);
    }
    return team_players;
}

void CSGO::scoreboard(string team)
{
    cout << team;
    vector<Player> team_players = find_players_of_team(team);
    sort(team_players.begin(), team_players.end());
    for (int i = 0; i < team_players.size(); i++)
        team_players[i].print_scores();
}

void CSGO::add_player()
{
    string name, team;
    cin >> name >> team;
    Player new_player(name, team);
    Weapon default_weapon = new_player.find_weapon(DEFAULT_WEAPON_NAME);
    new_player.add_weapon(default_weapon);
    players.push_back(new_player);
}

int CSGO::find_team_members(string team)
{
    int result = 0;
    for (int i = 0; i < players.size(); i++)
    {
        if (team == players[i].get_team() && players[i].get_health() > 0 && players[i].get_state() == ATK)
            result++;
    }
    return result;
}

void CSGO::add_money_team(string team, int prize)
{
    for (int i = 0; i < players.size(); i++)
    {
        if (team == players[i].get_team())
            players[i].add_money(prize);
    }
}

void CSGO::end_game()
{
    if (find_team_members(COUNTER_TERRORIST_TEAM) == 0)
    {
        add_money_team(TERRORIST_TEAM, WIN_PRIZE);
        add_money_team(COUNTER_TERRORIST_TEAM, LOSE_PRIZE);
        cout << TERRORIST_WIN;
    }
    else
    {
        add_money_team(COUNTER_TERRORIST_TEAM, WIN_PRIZE);
        add_money_team(TERRORIST_TEAM, LOSE_PRIZE);
        cout << COUNTER_TERRORIST_WIN;
    }
}

void CSGO::general_commands(string command, string username)
{
    Player *player = find_player_reference(username);
    if (command == GET_MONEY_COMMAND)
        cout << player->get_money() << endl;
    else if (command == GET_HEALTH_COMMAND)
        cout << player->get_health() << endl;
    else if (command == GO_AFK_COMMAND)
    {
        player->set_state(AFK);
        cout << DONE_OUTPUT;
    }
    else if (command == GO_ATK_COMMAND)
    {
        player->set_state(ATK);
        cout << DONE_OUTPUT;
    }
}

void CSGO::check_buy(string command, string username, int game_state)
{
    if (command == BUY_COMMAND)
    {
        string weapon_name;
        cin >> weapon_name;
        if (is_user_valid(username))
        {
            Player *player = find_player_reference(username);
            player->buy(weapon_name, game_state);
        }
        else
            cout << INVALID_USER_OUTPUT;
    }
}

void CSGO::check_shoot(string command, string username, int game_state)
{
    if (command == SHOOT_COMMAND)
    {
        string attacked_name, weapon_name;
        cin >> attacked_name >> weapon_name;
        if (is_user_valid(username) && is_user_valid(attacked_name))
            shoot(username, attacked_name, weapon_name, game_state);
        else
            cout << INVALID_USER_OUTPUT;
    }
}

void CSGO::round_command(string command, int &game_state)
{
    if (command == START_COMMAND)
    {
        game_state = STARTED;
        cout << FIGHT_OUTPUT;
    }
    else if (command == SCOREBOARD_COMMAND)
    {
        scoreboard(COUNTER_TERRORIST_TEAM);
        scoreboard(TERRORIST_TEAM);
    }
    else
    {
        string username;
        cin >> username;
        check_buy(command, username, game_state);
        check_shoot(command, username, game_state);
        general_commands(command, username);
    }
}

void CSGO::play_round(int &game_state)
{
    int command_count;
    cin >> command_count;
    for (int i = 0; i < command_count; i++)
    {
        string command;
        cin >> command;
        round_command(command, game_state);
    }
    end_game();
    for (int i = 0; i < players.size(); i++)
    {
        players[i].set_health(100);
    }
    game_state = NOT_STARTED;
}

void CSGO::play_game()
{
    int game_state = NOT_STARTED;
    string command_between_rounds;
    while (cin >> command_between_rounds)
    {
        if (command_between_rounds == ROUND_COMMAND)
            play_round(game_state);
        if (command_between_rounds == ADD_PLAYER_COMMAND)
        {
            add_player();
            cout << DONE_OUTPUT;
        }
        else if (command_between_rounds == SCOREBOARD_COMMAND)
        {
            scoreboard(COUNTER_TERRORIST_TEAM);
            scoreboard(TERRORIST_TEAM);
        }
    }
}

int main()
{
    int round_count;
    cin >> round_count;
    CSGO csgo;
    csgo.play_game();
    return 0;
}
