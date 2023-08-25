#include "physics.hpp"

StaticObjects::ObjectType::iterator StaticObjects::begin() {
    return objects.begin();
}

StaticObjects::ObjectType::iterator StaticObjects::end() {
    return objects.end();
}

uint32_t StaticObjects::Add(Object obj) {
    uint32_t tmp_cnt = counter;
    StoredObject tmp{ obj, counter };
    objects.push_back(tmp);
    counter += 1;
    return tmp_cnt;
}

void StaticObjects::Remove(uint32_t id) {
    objects.erase(objects.begin() + id);
}

Object StaticObjects::LineToObject(Line2D line) {
}