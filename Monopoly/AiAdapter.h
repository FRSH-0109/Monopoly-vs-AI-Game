#include <vector>
#include <algorithm>

class AiAdapter {
    std::vector<float> networkInputs_;

    public:
    AiAdapter();
    std::vector<float> getInputs();
    float convertMoney(unsigned int money);
    float convertMoneyValue(float value);
    float convertHouseValue(float value);
    float convertPosition(unsigned int position);
    void setTurn(unsigned int index);
};