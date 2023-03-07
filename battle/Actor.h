#pragma once

#include "globals.h"

#include "FontManager.h"
#include "TextureManager.h"
#include "ActionManager.h"

constexpr int DIRECTIONS = 8;
constexpr int INIT_DIRECTION = 6;

constexpr float MAX_RUN_SPEED = 2.0f;
constexpr float MAX_WALK_SPEED = 1.0f;
constexpr float MAX_STOP_SPEED = 0.5f;

constexpr int FRAME_CYCLE = 100;
constexpr int MIN_ACTION_CYCLE = 1000;
constexpr int MAX_ACTION_CYCLE = 5000;

constexpr int MIN_BATTLE_CYCLE = 500;
constexpr int MAX_BATTLE_CYCLE = 1000;

constexpr int MAX_HP = 999;
constexpr int MIN_HIT = 1;
constexpr int MAX_HIT = 10;

const string HP_FONT = "Lato";
constexpr int HP_FONT_SIZE = 16;
const sf::Color HP_COLOR = sf::Color::Red;

constexpr float SQR = 0.7071f;

const array<sf::Vector2f, DIRECTIONS> VECTORS = {
    sf::Vector2f(0.0,   1.0),   // 0
    sf::Vector2f(SQR,   SQR),   // 1
    sf::Vector2f(1.0,   0.0),   // 2
    sf::Vector2f(SQR,  -SQR),   // 3
    sf::Vector2f(0.0,  -1.0),   // 4
    sf::Vector2f(-SQR, -SQR),   // 5
    sf::Vector2f(-1.0,  0.0),   // 6
    sf::Vector2f(-SQR,  SQR)    // 7
};

enum STATUS {
    NORMAL = 0,
    BATTLE = 1,
    DIED = 2
};

constexpr array<ACTION, 2> RUN_ACTIONS = { ACTION::RUN, ACTION::ADVANCE };
constexpr array<ACTION, 2> WALK_ACTIONS = { ACTION::WALK, ACTION::WALK };
constexpr array<ACTION, 2> STAND_ACTIONS = { ACTION::STAND, ACTION::REST };
constexpr ACTION ATTACK_ACTION = ACTION::ATTACK;
constexpr ACTION HIT_ACTION = ACTION::HIT;
constexpr ACTION DEFENCE_ACTION = ACTION::DEFENCE;
constexpr ACTION DIE_ACTION = ACTION::DIE;

class Actor
{
public:

    void init(int id);
    void init(string name);
    void random() noexcept;
    void play(sf::Time elapsed);
    void step();
    void turn() noexcept;
    void battle();
    void hit();
    void attack();
    void die();

    bool isMoving() noexcept;
    bool isAliving() noexcept;
    bool isFighting() noexcept;

    const int& id = m_id;
    const int& type = m_type;
    const string& name = m_name;
    const int& hp = m_hp;

    const STATUS& status = m_status;

    const sf::Vector2f& position = m_position;
    const sf::Vector2f& prev_position = m_prev_position;

    const unique_ptr<sf::Sprite>& sprite = m_sprite;
    const unique_ptr<sf::Text>& text = m_text;

    friend class ActorManager;

private:

    void init();

    void initSprite();
    void initText();

    int getActionStartFrame();
    int getActionFrameCount();

    sf::Vector2f genPosition();
    int genActionCycle() noexcept;
    int genBattleCycle() noexcept;
    int genDirection() noexcept;
    int genAction() noexcept;
    float genSpeed() noexcept;

    static constexpr int getScreenDirection(int direction) noexcept;
    static constexpr int getTextureDirection(int direction) noexcept;

    int m_id{};
    int m_type{};
    string m_name{};
    int m_hp{};

    int m_frame_timer{};
    int m_frame_step{};

    int m_action_cycle{};
    int m_action_timer{};

    int m_battle_cycle{};
    int m_battle_timer{};

    int m_action_id{};
    int m_direction{};

    float m_speed{};

    bool m_change{};
    
    STATUS m_status{};

    sf::Vector2i m_frame;
    sf::IntRect m_area;

    sf::Vector2f m_position;
    sf::Vector2f m_prev_position;

    unique_ptr<sf::Sprite> m_sprite;
    unique_ptr<sf::Text> m_text;

    ActionSet* mp_action_set = nullptr;
    sf::Texture* mp_texture = nullptr;

    set<int> m_enemy_ids;
    queue<int> m_hits;

    static inline sf::IntRect region = sf::IntRect(0, 0, INIT_WIDTH, INIT_HEIGHT);
};