#include "player.h"

#include <algorithm>

Player::Player(string name, vector<char> winds) {
    playerId = 0;
    playerName = name;
    avaliableWinds = winds;
};

void Player::playWith(int p) {
    bannedOpponents.push_back(p);
}

void Player::setId(int id) {
    playerId = id;
}

int Player::getId() {
    return playerId;
}

string Player::getName() {
    return playerName;
}

bool Player::canBeWind(char wind) {
    return (find(avaliableWinds.begin(), avaliableWinds.end(), wind) != avaliableWinds.end());
}

bool Player::removeWind(char wind) {
    auto index = find(avaliableWinds.begin(), avaliableWinds.end(), wind);
    if(index == avaliableWinds.end()) {
        return false;
    }
    avaliableWinds.erase(index);
    return true;
}

void Player::resetWinds() {
    avaliableWinds = {'n', 'e', 's', 'w'};
}

bool Player::isPlayerIdBanned(int id) {
    return (find(bannedOpponents.begin(), bannedOpponents.end(), id) != bannedOpponents.end());
}

void Player::addBackWind(char wind) {
    avaliableWinds.push_back(wind);
}

vector<char> Player::getWinds(){
    return avaliableWinds;
}