/**
 * @file AiAdapter.h
 *
 * @brief TODO
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#ifndef AI_ADAPTER_H
#define AI_ADAPTER_H

#include <algorithm>

/**
 * @class AiAdapter
 * @brief Represents an adapter for AI input data.
 */
class AiAdapter {
	std::vector<double> networkInputs_;                        /**< Vector storing the AI network inputs. */
    const unsigned int TURN = 0;                               /**< Index for turn in network inputs. */
    const unsigned int POS = 4;                                /**< Index for position in network inputs. */
    const unsigned int MONEY = 8;                              /**< Index for money in network inputs. */
    const unsigned int CARD = 12;                              /**< Index for cards in network inputs. */
    const unsigned int JAIL = 16;                              /**< Index for jail state in network inputs. */
    const unsigned int OWNER = 20;                             /**< Index for property ownership in network inputs. */
    const unsigned int MORT = 48;                              /**< Index for mortgage state in network inputs. */
    const unsigned int HOUSE = 76;                             /**< Index for house state in network inputs. */
    unsigned int SELECT = 98;                                  /**< Index for selection in network inputs. */
    const unsigned int SELECT_MONEY = 126;                     /**< Index for selection money state in network inputs. */

	const std::vector<int> PROPERTIES = {-1, 0, -1, 1, -1, 2, 3, -1, 4, 5, -1, 6, 7, 8, 9, 10, 11, -1, 12, 13, -1, 14,
                                          -1, 15, 16, 17, 18, 19, 20, 21, -1, 22, 23, -1, 24, 25, -1, 26, -1, 27}; /**< Vector representing property states. */
    const std::vector<int> HOUSES = {-1, 0, -1, 1, -1, -1, 2, -1, 3, 4, -1, 5, -1, 6, 7, -1, 8, -1, 9, 10, -1, 11, -1,
                                      12, 13, -1, 14, 15, -1, 16, -1, 17, 18, -1, 19, -1, -1, 20, -1, 21}; /**< Vector representing house states. */

   public:
	 /**
     * @brief Constructor for the AiAdapter class.
     */
    AiAdapter();

    /**
     * @brief Gets the AI network inputs.
     * @return Vector of AI network inputs.
     */
    std::vector<double> getInputs();

    /**
     * @brief Converts money into a format suitable for AI input.
     * @param money The money value to convert.
     * @return The converted money value.
     */
    double convertMoney(unsigned int money);

    /**
     * @brief Converts money value into a format suitable for AI input.
     * @param value The money value to convert.
     * @return The converted money value.
     */
    double convertMoneyValue(double value);

    /**
     * @brief Converts house value into a format suitable for AI input.
     * @param value The house value to convert.
     * @return The converted house value.
     */
    double convertHouseValue(double value);

    /**
     * @brief Converts position into a format suitable for AI input.
     * @param position The position to convert.
     * @return The converted position.
     */
    double convertPosition(unsigned int position);

    /**
     * @brief Converts card value into a format suitable for AI input.
     * @param cards The card value to convert.
     * @return The converted card value.
     */
    double convertCard(unsigned int cards);

    /**
     * @brief Converts house state into a format suitable for AI input.
     * @param isHotel Flag indicating if it's a hotel.
     * @param houseNumber The house number.
     * @return The converted house state.
     */
    double convertHouse(bool isHotel, unsigned int houseNumber);

    /**
     * @brief Sets the turn for a given index.
     * @param index The index for which to set the turn.
     */
    void setTurn(unsigned int index);

    /**
     * @brief Sets the selection for a given index.
     * @param index The index for which to set the selection.
     */
    void setSelection(unsigned int index);

    /**
     * @brief Sets the selection state for a given index.
     * @param index The index for which to set the selection state.
     * @param state The state to set.
     */
    void setSelectionState(unsigned int index, int state);

    /**
     * @brief Sets the money context state.
     * @param state The state to set.
     */
    void setMoneyContext(int state);

    /**
     * @brief Clears the selection state.
     */
    void clearSelectionState();

    /**
     * @brief Sets the position for a given index.
     * @param index The index for which to set the position.
     * @param position The position to set.
     */
    void setPosition(unsigned int index, unsigned int position);

    /**
     * @brief Sets the money for a given index.
     * @param index The index for which to set the money.
     * @param money The money to set.
     */
    void setMoney(unsigned int index, unsigned int money);

    /**
     * @brief Sets the card for a given index.
     * @param index The index for which to set the card.
     * @param cards The card to set.
     */
    void setCard(unsigned int index, unsigned int cards);

    /**
     * @brief Sets the jail state for a given index.
     * @param index The index for which to set the jail state.
     * @param state The state to set.
     */
    void setJail(unsigned int index, unsigned int state);

    /**
     * @brief Sets the owner state for a given property.
     * @param property The property for which to set the owner state.
     * @param state The state to set.
     */
    void setOwner(unsigned int property, unsigned int state);

    /**
     * @brief Sets the mortgage state for a given property.
     * @param property The property for which to set the mortgage state.
     * @param state The state to set.
     */
    void setMortgage(unsigned int property, unsigned int state);

    /**
     * @brief Sets the house state for a given house.
     * @param isHotel Flag indicating if it's a hotel.
     * @param houseNumber The house number.
     * @param id The ID to set.
     */
    void setHouse(bool isHotel, unsigned int houseNumber, unsigned int id);
};

#endif