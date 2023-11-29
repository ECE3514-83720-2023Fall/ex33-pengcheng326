#include "catch.hpp"

#include "hash_dictionary.hpp"

#include <iostream> // for debugging
#include <sstream>  // for std::ostringstream
#include <string>   // for std::string

typedef unsigned int KeyType;
typedef std::string ValueType;
typedef std::size_t (*FunctionType)(const KeyType &key);
typedef HashDictionary<KeyType, ValueType, FunctionType> DictionaryType;

// a trivial hash function
std::size_t hash(const KeyType& key) 
	{ 
	return key; 
    }

TEST_CASE("Basic Tests", "[Dictionary]") {
	//To DO
	DictionaryType dict(hash);

	REQUIRE(dict.isEmpty());

	dict.add(3, "Three");
	dict.add(2, "Two");
	dict.add(6, "Six");
	dict.add(326, "three-two-six");

	REQUIRE(dict.size() == 4);

	REQUIRE(dict.contains(3));
	REQUIRE(dict.contains(2));
	REQUIRE(dict.contains(6));
	REQUIRE(dict.contains(326));


	REQUIRE(dict.get(3) == "Three");
	REQUIRE(dict.get(2) == "Two");
	REQUIRE(dict.get(6) == "Six");
	REQUIRE(dict.get(326) == "three-two-six");

	dict.remove(3);
	REQUIRE(dict.size() == 3);
	REQUIRE_FALSE(dict.contains(3));

	dict.remove(2);
	REQUIRE(dict.size() == 2);
	REQUIRE_FALSE(dict.contains(2));

	dict.remove(6);
	dict.remove(326);
	REQUIRE_FALSE(dict.contains(6));
	REQUIRE_FALSE(dict.contains(326));
	REQUIRE(dict.isEmpty());




  
}

TEST_CASE("Test large Dictionary", "[Dictionary]") {
	//To Do

	DictionaryType dict(hash);
	for (KeyType k = 0; k < 326; k++) 
	{
		std::ostringstream ss;
		ss << k;
		dict.add(k, ss.str());
	}


	REQUIRE(dict.size() == 326);
	REQUIRE(dict.contains(325));
	REQUIRE_FALSE(dict.size() == 327);
	REQUIRE_FALSE(dict.contains(327));

	for (KeyType k = 0; k < 326; k++)
	{
		std::ostringstream ss;
		ss << k;
		REQUIRE(dict.contains(k));
		REQUIRE(dict.get(k) == ss.str());
	}
  
}

