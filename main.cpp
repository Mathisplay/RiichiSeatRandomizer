#include <iostream>
#include <vector>

#include "player.h"
#include "playerList.h"

using namespace std;

PlayerList players;

int main() {
    string player;
    while(cin >> player) {
        vector<char> winds = {'n', 'e', 's', 'w'};
        players.addPlayer(Player(player, winds));
    }
    auto play = players.getPlayers();
    // for(auto item : *play) {
    //     cout << item.getName() << endl;
    // }
    auto windedPlayers = players.assignWinds(8);
    players.assignTables(windedPlayers);
    return 0;
}