#pragma once

#include <boost/typeof/typeof.hpp>
#define typeof(t) BOOST_TYPEOF(t)

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>


#ifndef itertype
#define itertype(v) typeof((v).begin())
#endif