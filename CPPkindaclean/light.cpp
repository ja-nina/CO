Light::Light(int id, int T) {
    this->id = id;
    this->T = T;
    this->differenceInCycle = 0;

}

void Light::setUsed() {
    for (int i = 0; i < T; i++) {
        this->used.push_back('0'); //chodzi o to, że vector<bool> jest zjebany a int zajmuje więcej miejsca niz bool
    }
}


Light::~Light() {
    used.clear();
}

bool Light::isGreen(int idStreet, int time) {
    if (this->starting.count(idStreet) == 1 && this->schedule.count(idStreet) == 1) {
        if (this->starting[idStreet] == -1) {
            return false;
        }
        if ((this->starting[idStreet] <= time % this->cycle) &&
            (time % this->cycle < (this->starting[idStreet] + this->schedule[idStreet]))) {
            return true;
        }
    }
    return false;
}

bool Light::canCross(int time) {
    bool ans = this->used[time] == '0';
    return ans;
}

void Light::addToSchedule(int idStreet, int length) {
    this->schedule[idStreet] = length;
    if (length == 0) {
        this->starting[idStreet] = -1;
        return;
    }
    this->starting[idStreet] = this->cycle;
    this->usedStreets.push_back(idStreet);
    this->numberOfUsedStreets += 1;
    this->cycle += length;
}

void Light::reset() {
    for (int i = 0; i < this->T; i++) {
        this->used[i] = '0';
    }
    this->differenceInCycle = 0;
}

Light *Light::copy() {
    auto *newLight = new Light(this->id, this->T);
    for (auto &it: this->schedule) {
        newLight->schedule[it.first] = it.second;
    }
    for (auto &it: this->starting) {
        newLight->starting[it.first] = it.second;
    }
    newLight->cycle = cycle;
    newLight->numberOfUsedStreets = this->numberOfUsedStreets;
    for (auto element: this->used) {
        newLight->used.push_back(element);
    }
    for (auto element: this->usedStreets) {
        newLight->usedStreets.push_back(element);
    }
    return newLight;
}

