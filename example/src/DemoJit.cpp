#include <nautilus/Engine.hpp>
#include <nautilus/core.hpp>

using namespace nautilus;

val<int32_t> conditionalSum(val<int32_t> size, val<bool*> mask, val<int32_t*> array) {
	val<int32_t> sum = 0;
	for (val<int32_t> i = 0; i < size; i++) {
		// check mask
		if (mask[i]) {
			// load value from array at position i
			val<int32_t> value = array[i];
			// add value to sum
			sum += value;
		}
	}
	return sum;
}

int main(int, char* []) {
	engine::Options options;
	options.setOption("engine.backend", "cpp");
	// options.setOption("dump.all", true);

	options.setOption("engine.Compilation", false);
	auto engine = engine::NautilusEngine(options);
	auto function = engine.registerFunction(conditionalSum);

	auto mask = new bool[4]{true, true, false, true};
	auto array = new int32_t[4] {1, 2, 3, 4};

	auto result = function(4, mask, array);
	std::cout << "Result: " << result << std::endl;

	auto mask2 = new bool[result]{true, true, false, true, true, true, false};
	auto array2 = new int32_t[result] {1, 2, 3, 4, 5, 6, 7};

	auto res2 = function(result, mask2, array2);
	std::cout << "Dependent Result: " << res2 << std::endl;

	return 0;
}
