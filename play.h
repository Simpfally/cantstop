#ifndef PLAY_H
#define PLAY_H

#include <vector>
#include <utility>
#include "cantstop.h"
#include "dicey.h"

joli few_games(std::vector<std::unique_ptr<cs_ai::AI>>& players, int n);

#endif
