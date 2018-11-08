#ifndef DATABASEQUERY_TESTS_H
#define DATABASEQUERY_TESTS_H

#include "../_generator/generator.h"
#include "../query.h"

typedef std::vector<test_row_t> table_t;
typedef std::vector<predicate_t<test_row_t>> predicates_t;

template <bool result>
class TestAll {
public:
	std::pair<table_t, predicates_t> test_data;
	bool computed_result;

	TestAll() : test_data(generate_all(result)) {}
	void run_test() {
		computed_result = is_satisfied_for_all(test_data.second, test_data.first);
	}
	bool verify() {
		return computed_result == result;
	}
};

template <bool result>
class TestAny {
public:
	std::pair<table_t, predicates_t> test_data;
	bool computed_result;

	TestAny() : test_data(generate_any(result)) {}
	void run_test() {
		computed_result = is_satisfied_for_any(test_data.second, test_data.first);
	}
	bool verify() {
		return computed_result == result;
	}
};

#endif