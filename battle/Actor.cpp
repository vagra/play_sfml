#include "Actor.h"
#include "ActorManager.h"


void Actor::init(int type) {
	m_type = type;
	m_name = ActionManager::getActionSetName(type);

	init();
}

void Actor::init(string name) {
	m_name = name;
	m_type = ActionManager::getActionSetIndex(name);

	init();
}

void Actor::init() {

	m_id = ActorManager::genID();

	mp_action_set = ActionManager::getActionSet(m_name);
	mp_texture = TextureManager::getTexture(m_name);
	m_area = sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT);

	m_position = genPosition();
	m_prev_position = m_position;

	m_hp = MAX_HP;
	m_battle_cycle = genBattleCycle();
	m_battle_timer = 0;

	random();

	initSprite();
	initText();
	
	step();
}

void Actor::initSprite() {
	m_sprite.reset();

	m_sprite = make_unique<sf::Sprite>();

	m_sprite->setTexture(*mp_texture);
	m_sprite->setScale(SCALE, SCALE);
	m_sprite->setOrigin(ORIGIN_X, ORIGIN_Y);
	m_sprite->setPosition(m_position);
}

void Actor::initText() {
	m_text.reset();

	m_text = make_unique<sf::Text>();

	m_text->setFont(*FontManager::getFont(HP_FONT));
	m_text->setCharacterSize(HP_FONT_SIZE);
	m_text->setFillColor(HP_COLOR);
	m_text->setString(to_string(m_hp));
	m_text->setOrigin(0, ORIGIN_Y);
	m_text->setPosition(m_position);
}

void Actor::random() noexcept {
	m_frame_timer = 0;
	m_frame_step = 0;
	m_action_timer = 0;

	m_action_cycle = genActionCycle();
	m_speed = genSpeed();
	m_direction = genDirection();
	m_action_id = genAction();
}


void Actor::play(sf::Time elapsed) {

	const int ms = elapsed.asMilliseconds();

	m_frame_timer += ms;
	m_action_timer += ms;
	m_battle_timer += ms;

	if (m_battle_timer >= m_battle_cycle) {
		m_battle_timer = 0;
		m_battle = false;
	}

	if (m_battle) {
		m_text->setString(to_string(m_hp));
	}

	if (m_action_timer >= m_action_cycle) {
		m_action_timer = 0;
		random();
	}

	if (m_frame_timer >= FRAME_CYCLE || m_change) {
		m_frame_timer = m_frame_timer % FRAME_CYCLE;
		m_change = false;
		step();
	}

	if (m_move) {
		const sf::Vector2f offset = VECTORS.at(m_direction) * m_speed;

		m_sprite->move(offset);
		m_text->move(offset);
	}

	m_prev_position = m_position;
	m_position = m_sprite->getPosition();
}

void Actor::step() {
	const int direction = getScreenDirection(m_direction);

	m_frame.x = getActionStartFrame() + m_frame_step;
	m_frame.y = direction;

	m_area.left = m_frame.x * FRAME_WIDTH;
	m_area.top = m_frame.y * FRAME_HEIGHT;

	m_sprite->setTextureRect(m_area);

	m_frame_step = (m_frame_step + 1) % getActionFrameCount();
}

void Actor::turn() noexcept {
	const int range = rand() % (DIRECTIONS - 3) + 2;
	m_direction = (m_direction + range) % DIRECTIONS;
}

sf::Vector2f Actor::genPosition() {
	const float x = narrow_cast<float>(rand() % INIT_WIDTH);
	const float y = narrow_cast<float>(rand() % INIT_HEIGHT);

	return sf::Vector2f(x, y);
}

int Actor::genActionCycle() noexcept {
	return rand() % (MAX_ACTION_CYCLE - MIN_ACTION_CYCLE) + MIN_ACTION_CYCLE;
}

int Actor::genBattleCycle() noexcept {
	return rand() % (MAX_BATTLE_CYCLE - MIN_BATTLE_CYCLE) + MIN_BATTLE_CYCLE;
}

int Actor::genDirection() noexcept {
	const int left = region.left;
	const int top = region.top;
	const int right = region.width + region.left;
	const int bottom = region.height + region.top;

	if (m_position.x < left && m_position.y < top) return 1;
	if (m_position.x < left && m_position.y > bottom) return 3;
	if (m_position.x > right && m_position.y > bottom) return 5;
	if (m_position.x > right &&	m_position.y < top) return 7;

	if (m_position.y < top) return 0;
	if (m_position.x < left) return 2;
	if (m_position.y > bottom) return 4;
	if (m_position.x > right) return 6;

	return rand() % DIRECTIONS;
}

int Actor::genAction() noexcept {

	int action_id = m_action_id;

	if (m_speed > MAX_WALK_SPEED) {
		action_id = RUN_ACTIONS.at(rand() % RUN_COUNT);
	}
	else if (m_speed > MAX_STOP_SPEED) {
		action_id = WALK_ACTIONS.at(rand() % WALK_COUNT);
	}
	else {
		action_id = STOP_ACTIONS.at(rand() % STOP_COUNT);
	}

	if (m_action_id == action_id) {
		m_frame_step = 0;
		m_change = true;
	}

	return action_id;
}

float Actor::genSpeed() noexcept {
	float speed = rand() % narrow_cast<int>(MAX_RUN_SPEED * 100) / 100.0f;

	if (speed < MAX_STOP_SPEED) {
		speed = 0;
		m_move = false;
	}
	else {
		m_move = true;
	}

	return speed;
}

int Actor::getActionStartFrame() {
	return mp_action_set->getAction(m_action_id)->start;
}

int Actor::getActionFrameCount() {
	return mp_action_set->getAction(m_action_id)->frames;
}

constexpr int Actor::getScreenDirection(int direction) noexcept {
	return (DIRECTIONS + INIT_DIRECTION - direction) % DIRECTIONS;
}

constexpr int Actor::getTextureDirection(int direction) noexcept {
	return (DIRECTIONS + INIT_DIRECTION - direction) % DIRECTIONS;
}