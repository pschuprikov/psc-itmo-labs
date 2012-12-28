#ifndef ENTHROPY_STAT_H
#define ENTHROPY_STAT_H

#include <vector>
#include <cmath>

#include "alphabet.h"
#include "probability_counters.h"

namespace coding
{

double enthropy(probability::probability_provider const& prb);

double enthropy_by_letter(probability::probability_provider const& prb, alphabet_t const& alph);

}

#endif // ENTHROPY_STAT_H
