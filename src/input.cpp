#include "input.h"

std::map<Input_Type, Input_Trigger> input_mapping {
    INPUT_TYPES(DEF_MAP)
};

std::queue<Input_Type> inputs;

void handleInputs() {
    while (!inputs.empty()) {
        Input_Type t = inputs.front();

        input_mapping[t](); // calls input trigger function associated with input type
        
        inputs.pop();
    }
}