#ifndef WET2_UF_H
#define WET2_UF_H

#include <iostream>
#include "Player.h"

class UFNode {
private:
    int key;
    Player data;
    int gamesRemainder;
    permutation_t spiritRemainder;
    UFNode* next;

    void updatePath(UFNode *head, UFNode *node);
    void updateGamesPlayed(UFNode* node);

public:
    permutation_t partialSpirit;
    permutation_t teamSpirit;
    UFNode(int key, Player data) : key(key), data(data), gamesRemainder(0), spiritRemainder(permutation_t::neutral()), next(nullptr){}
    UFNode(const UFNode& invertedTree) = default;
    UFNode& operator=(const UFNode& invertedTree) = default;
    ~UFNode() = default;
    void updatePartialSpirit(UFNode *node);
    Player& getData() { return data; }
    int getKey() const {return key;}
    int getTotalGamesPlayed();
    permutation_t findPartialSpirit();
    UFNode* find();
    void setSpiritRemainder(permutation_t sp);
    void Union(UFNode* root);
    int findValue();
    void incGamesPlayed();
    void multSpirit(permutation_t sp);
};

inline void UFNode::updatePath(UFNode* head, UFNode* node)
{
    if (head == node) {
        return;
    }

    UFNode* temp;
    while (node->next) {
        temp = node->next;
        node->next = head;
        node = temp;
    }
}

inline void UFNode::updatePartialSpirit(UFNode* node)
{
    if (node->next == nullptr) {
        return;
    }

    updatePartialSpirit(node->next);

    if (node->next->next == nullptr) {
        return;
    }
    else {
        node->spiritRemainder = node->next->spiritRemainder * node->spiritRemainder;
    }
}

inline UFNode* UFNode::find()
{
    UFNode* head = this;
    while (head->next) {
        head = head->next;
    }

    updatePartialSpirit(this);
    updateGamesPlayed(this);
    updatePath(head, this);

    return head;
}
inline void UFNode::Union(UFNode *root)
{
    this->next = root;
    this->gamesRemainder -= root->gamesRemainder;
    this->spiritRemainder = this->spiritRemainder * root->spiritRemainder.inv();
}

inline void UFNode::updateGamesPlayed(UFNode* node)
{
    if (node->next == nullptr) {
        return;
    }

    updateGamesPlayed(node->next);

    if (node->next->next == nullptr) {
        return;
    }
    else {
        node->gamesRemainder += node->next->gamesRemainder;
    }
}

inline int UFNode::findValue()
{
    UFNode* parent = find();
    int res =  this->data.getGamesPlayed() + this->gamesRemainder;

    if(this != parent)
    {
        res += parent->getTotalGamesPlayed();
    }

    return res;
}

inline int UFNode::getTotalGamesPlayed() {
    return this->gamesRemainder;
}

inline void UFNode::incGamesPlayed()
{
    this->gamesRemainder++;
}

inline permutation_t UFNode::findPartialSpirit()
{
    UFNode* parent = find();
    permutation_t res = permutation_t::neutral();

    if(this != parent)
    {
        res = res * parent->spiritRemainder;
    }

    res = res * this->spiritRemainder;
    res = res * this->partialSpirit;

    return res;
}

inline void UFNode::setSpiritRemainder(permutation_t sp)
{
    this->spiritRemainder = sp;
}

inline void UFNode::multSpirit(permutation_t sp)
{
    this->spiritRemainder = sp * this->spiritRemainder;
}

#endif