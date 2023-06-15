#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <map>
#include <string>


class Player {
    private:
        // Flag name; set or unset.
        std::map<std::string, bool> flags;
        // Item name; item count.
        std::map<std::string, int> inventory;
        // Corresponding flag name; descriptiove text.
        std::map<std::string, std::string> objectives;

    public:
        std::string name;

        /**
         * Sets a player flag to true.
         * @param newFlag Name of the new flag.
         * @return True if the flag was present before setting. False if this is the first time the flag has been marked.
        */
        bool addFlag(std::string newFlag);

        /**
         * Adds an item to player inventory.
         * @param newItem Item to add to player inventory.
        */
        void addItem(std::string newItem);

        /**
         * Checks a flag's status.
         * @param flag Flag's name.
         * @return True if flag has been set, false if absent.
        */
        bool checkFlag(std::string flag);

        /**
         * Checks if the player has completed all objectives.
         * @return True if all objectives have been completed.
        */
        bool checkObjectives();

        /**
         * Checks if the player has an item.
         * @param item Item name.
         * @return True if player has item, false other.
        */
        bool hasItem(std::string item);

        /**
         * Returns a string listing all objectives and completion status.
         * @return A formatted string ready to print. Looks like a checklist with boxes filled in.
        */
        std::string readObjectives();

        /**
         * Removes an item from the player's inventory.
         * @param item The item to remove from inventory.
         * @return Returns false if the item was not present in inventory. True if it was, and is now removed.
        */
        bool removeItem(std::string item);

        /**
         * Sets a new objective, or updates old objective.
         * @param flagName Name of the flag that indicates the objective has been completed.
         * @param objectiveText Text to show the player to describe the objective.
        */
        void setObjective(std::string flagName, std::string objectiveText);

        /**
         * Lists all items in the player's inventory.
         * @return A string naming all items in a player's inventory.
        */
        std::string showInventory();
};


#endif
