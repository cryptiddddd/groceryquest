#include "player.h"

#include <algorithm>
#include <sstream>
#include <string>
#include <map>

using namespace std;


bool Player::addFlag(string newFlag) {
    bool oldStatus = flags[newFlag];
    flags[newFlag] = true;

    return oldStatus;
}

void Player::addItem(string newItem) {
    inventory[newItem] = inventory[newItem] + 1;
}

bool Player::checkFlag(string flag) {
    return flags[flag];
}

bool Player::checkObjectives() {
    if (objectives.empty()) return false;

    for (auto const& objectiveData : objectives) {
        if (!flags[objectiveData.first]) return false;
    }

    return true;
}

bool Player::hasItem(string item) {
    return inventory[item] != 0;
}

string Player::readObjectives() {
    if (objectives.empty()) return "You have no quest yet!";

    stringstream message;

    for (auto const& [flag, text] : objectives) {
        string checkBox = (flags[flag]) ? "[x] " : "[ ] ";
        message << "\t" << checkBox << text << "\n";
    }

    return message.str();
}

bool Player::removeItem(string item) {
    bool hadItem = inventory[item];

    if (hadItem) {
        --inventory[item];
    }

    return hadItem;
}

void Player::setObjective(string flagName, string objectiveText) {
    objectives[flagName] = objectiveText;
}

string Player::showInventory() {
    if (inventory.empty()) {
        return "You have nothing.";
    }

    stringstream items;

    for (const auto& [item, count] : inventory) {
        if (!count) continue;

        items << "\t- " << item;
        if (count > 1) {
            items << "(x" << count << ")";
        }
        items << "\n";
    }

    return items.str();
}
