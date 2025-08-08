// #include <nautilus/Engine.hpp>
// #include <nautilus/core.hpp>
//
// using namespace nautilus;
//
// val<int32_t> conditionalSum(val<int32_t> size, val<bool*> mask, val<int32_t*> array) {
// 	val<int32_t> sum = 0;
// 	for (val<int32_t> i = 0; i < size; i++) {
// 		// check mask
// 		if (mask[i]) {
// 			// load value from array at position i
// 			val<int32_t> value = array[i];
// 			// add value to sum
// 			sum += value;
// 		}
// 	}
// 	return sum;
// }
//
// int main(int, char*[]) {
// 	engine::Options options;
// 	options.setOption("engine.backend", "cpp");
// 	// options.setOption("dump.all", true);
//
// 	options.setOption("engine.Compilation", true);
// 	auto engine = engine::NautilusEngine(options);
// 	auto function = engine.registerFunction(conditionalSum);
//
// 	auto mask = new bool[4]{true, true, false, true};
// 	auto array = new int32_t[4] {1, 2, 3, 4};
//
// 	auto result = function(4, mask, array);
// 	std::cout << "Result: " << result << std::endl;
// 	return 0;
// }

#include <nautilus/Engine.hpp>
#include <nautilus/core.hpp>
#include <iostream>
using namespace nautilus;

void testExecute(val<uint8_t*> var1, val<uint8_t*> var2, val<uint8_t*> var3) {
    // Just access the first byte of each parameter to verify they're valid
    val<uint8_t> byte1 = var1[0];
    val<uint8_t> byte2 = var2[0];
    val<uint8_t> byte3 = var3[0];
    // Function completes successfully if we reach here
}

int main(int, char*[]) {
    engine::Options options;
    options.setOption("engine.backend", "cpp");
    options.setOption("engine.Compilation", true);
    auto engine = engine::NautilusEngine(options);

    auto function = engine.registerFunction(testExecute);

    // Create test data
    uint8_t data1[20] = {1,2,3,4,5};
    uint8_t data2[20] = {6,7,8,9,10};
    uint8_t data3[20] = {11,12,13,14,15};

    function(data1, data2, data3);
    std::cout << "Test executed successfully!" << std::endl;
    return 0;
}

