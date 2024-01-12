/**
 * @file GameScreen.cc
 *
 * @brief Source file for game screen class deriving from
 * ActiveScreen class. Used to handle monopoly game activities
 * and drawing.
 *
 * @author Kamil Kosnik, Kacper Radzikowski
 *
 */

#include <cmath>
#include <numbers>
#include "GameScreen.h"

GameScreen::GameScreen(std::vector<std::shared_ptr<playerSettings>> playerSettingsList_) {
	setContextWindow(ContextWindow::GetInstance());
	std::cout << "GS constructor" << std::endl;
	setScreenType(MONOPOLY_GAME);

	// TODO: exception handling
	if (!getFont().loadFromFile("textures_and_fonts/fonts/Kabel-Heavy.ttf"))
		std::cout << "Font not found!\n";

	setFont(getFont());

	monopoly_game_engine_.setFont(getFont());
	monopoly_game_engine_.createBoard();
	monopoly_game_engine_.clearPlayers();
	monopoly_game_engine_.createPlayers(playerSettingsList_);
	monopoly_game_engine_.setPlayerIndexTurn(0);
	monopoly_game_engine_.createButtonRollDice();
	monopoly_game_engine_.createTextTurnInfo();
	monopoly_game_engine_.createTextRolledValue();
	monopoly_game_engine_.createTextPlayersInfo();
	monopoly_game_engine_.createTextBiddedProperty();
	monopoly_game_engine_.createTextBidderInfo();
	monopoly_game_engine_.createTextHighestBidInfo();
	monopoly_game_engine_.createTextLeadingBidderInfo();
	monopoly_game_engine_.createCurrentOfferBidderInfo();
	monopoly_game_engine_.createButtonBuyResign();
	monopoly_game_engine_.createButtonNextProperty();
	monopoly_game_engine_.createButtonPerviousProperty();
	monopoly_game_engine_.showPropertyData(1, false);
	monopoly_game_engine_.createButtonsBuySellHouseHotel();
	monopoly_game_engine_.createButtonsBankrupt();
	monopoly_game_engine_.createButtonsNextTurn();
	monopoly_game_engine_.createButtonsJailPay();
	monopoly_game_engine_.createAuctionOfferButtons();
	monopoly_game_engine_.createAuctionBidButton();
	monopoly_game_engine_.createAuctionResignButton();
	monopoly_game_engine_.createAvailableHousesHotelText();
	monopoly_game_engine_.createButtonWithdraw();
	monopoly_game_engine_.getWithdraw().setFont(getFont());
	monopoly_game_engine_.getWithdraw().createChoosePlayerScreen();
	monopoly_game_engine_.getWithdraw().createValuePlayerScreen();
	monopoly_game_engine_.getWithdraw().createDecisionPlayerScreen();
	monopoly_game_engine_.createMortagingButton();
	monopoly_game_engine_.getWithdraw().setBoard(monopoly_game_engine_.getBoard());
	for (auto button_ptr : monopoly_game_engine_.getWithdraw().getButtons()) {
		monopoly_game_engine_.addButton(button_ptr);
	}
	monopoly_game_engine_.createChanceCards();
	monopoly_game_engine_.turnInfoTextShow();
}

ScreenEventType GameScreen::worker() {
	ScreenEventType eventType = IDLE;
	for (auto element : monopoly_game_engine_.getButtons()) {
		element->setIsClicked(false);
		element->setIsActive(false);
		if (element->getIsVisible()) {
			if (element->isMouseOver(getContextWindow()->getWindow())) {
				element->mouseIsOver();
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					element->setIsClicked(true);
					element->setIsActive(true);
				} else {
					element->setWasReleased(true);
				}
			} else {
				element->setWasReleased(true);
				element->mouseIsNotOver();
			}
		}

		buttonSetColors(element);
	}

	if (monopoly_game_engine_.monopolyGameWorker() == false)  // game endeds
	{
		return GAME_ENDED;
	}

	return eventType;
}

void GameScreen::draw() {
	std::shared_ptr<Board> game_board_ptr = monopoly_game_engine_.getBoard();

	switch (monopoly_game_engine_.getScreenType()) {
		case BOARDGAME: {
			// draw buttons and texts
			for (auto element : monopoly_game_engine_.getButtons()) {
				if (element->getIsVisible()) {
					element->draw(getContextWindow()->getWindow());
				}
			}
			for (auto element : monopoly_game_engine_.getTexts()) {
				getContextWindow()->getWindow().draw(*element);
			}

			// draw board
			sf::Sprite sprite;
			for (auto field : monopoly_game_engine_.getBoard()->getBoard()) {
				FieldType field_type = std::visit([](Field& field) { return field.getType(); }, field);
				sprite = std::visit([](Field& field) { return field.getSprite(); }, field);
				getContextWindow()->getWindow().draw(sprite);
				float rotation = std::visit([](Field& field) { return field.getRotation(); }, field);

				// draw property name text
				sf::Text nameText;
				if (field_type == STREET || field_type == UTILITY || field_type == STATION || field_type == TAX) {
					nameText = std::visit([](Field& field) { return field.getNameText(); }, field);
					nameText = monopoly_game_engine_.getPropertyNameToDraw(nameText, sprite, rotation);
					getContextWindow()->getWindow().draw(nameText);
				}

				if (field_type == STREET || field_type == UTILITY || field_type == STATION) {
					sf::RectangleShape owner_flag;
					switch (field_type) {
						case STREET: {
							StreetField& field_specified = std::get<StreetField>(field);
							owner_flag = field_specified.getOwnerFlag();
							std::shared_ptr<Player> owner_ptr = field_specified.getOwner();
							if (owner_ptr != nullptr) {
								sf::Color player_color = owner_ptr->getColor();
								if (field_specified.getIsMortgaged()) {
									player_color.a = 100;
									if (player_color.b == 255) {
										player_color.b = 180;
									} else if (player_color.r == 255 && player_color.g == 0) {
										player_color.r = 130;
									}
								}
								owner_flag.setFillColor(player_color);

								if (field_specified.getIsHotel()) {
									sf::Sprite hotel_sprite = monopoly_game_engine_.getHotelSprite(field_specified);
									sf::Texture hotel_texture = monopoly_game_engine_.getHotelTexture();
									hotel_sprite.setTexture(hotel_texture, true);
									getContextWindow()->getWindow().draw(hotel_sprite);
								} else if (field_specified.getHouseNumber() != 0) {
									for (unsigned int i = 1; i <= field_specified.getHouseNumber(); ++i) {
										sf::Sprite house_sprite =
											monopoly_game_engine_.getHouseSprite(field_specified, i);
										sf::Texture house_texture = monopoly_game_engine_.getHouseTexture();
										house_sprite.setTexture(house_texture, true);
										getContextWindow()->getWindow().draw(house_sprite);
									}
								}
							}
						} break;

						case UTILITY: {
							UtilityField& field_specified = std::get<UtilityField>(field);
							owner_flag = field_specified.getOwnerFlag();
							std::shared_ptr<Player> owner_ptr = field_specified.getOwner();
							if (owner_ptr != nullptr) {
								sf::Color player_color = owner_ptr->getColor();
								if (field_specified.getIsMortgaged()) {
									player_color.a = 100;
									if (player_color.b == 255) {
										player_color.b = 180;
									} else if (player_color.r == 255 && player_color.g == 0) {
										player_color.r = 130;
									}
								}
								owner_flag.setFillColor(player_color);
							}
						} break;

						case STATION: {
							StationField& field_specified = std::get<StationField>(field);
							owner_flag = field_specified.getOwnerFlag();
							std::shared_ptr<Player> owner_ptr = field_specified.getOwner();
							if (owner_ptr != nullptr) {
								sf::Color player_color = owner_ptr->getColor();
								if (field_specified.getIsMortgaged()) {
									player_color.a = 100;
									if (player_color.b == 255) {
										player_color.b = 180;
									} else if (player_color.r == 255 && player_color.g == 0) {
										player_color.r = 130;
									}
								}
								owner_flag.setFillColor(player_color);
							}
						} break;
						default:
							break;
					}

					getContextWindow()->getWindow().draw(owner_flag);
				}
			}

			// draw player
			for (auto player : monopoly_game_engine_.getPlayers()) {
				getContextWindow()->getWindow().draw(player->getSprite());
			}

			// draw property data for buy/sell
			if (monopoly_game_engine_.getTurnState() == BUY_ACTION) {
				getContextWindow()->getWindow().draw(monopoly_game_engine_.getPropertyDataSprite());
				for (auto text_ptr : monopoly_game_engine_.getPropertyDataTexts()) {
					getContextWindow()->getWindow().draw(*text_ptr);
				}
			}

			// draw property browser
			getContextWindow()->getWindow().draw(monopoly_game_engine_.getAllPropertyDataSprite());
			for (auto text_ptr : monopoly_game_engine_.getAllPropertyDataTexts()) {
				getContextWindow()->getWindow().draw(*text_ptr);
			}

			// Notifications wall draw
			for (auto notiTextPtr : monopoly_game_engine_.getNotificationsWall().getWall()) {
				getContextWindow()->getWindow().draw(*notiTextPtr);
			}
		} break;

		case WITHDRAW_ADD_VALUE:
			monopoly_game_engine_.getWithdraw().getSpritePropertyPlayer1().setTexture(
				*monopoly_game_engine_.getWithdraw().getTexturePropertyPlayer1(), true);
			getContextWindow()->getWindow().draw(monopoly_game_engine_.getWithdraw().getSpritePropertyPlayer1());
			for (auto text_ptr : monopoly_game_engine_.getWithdraw().getTextsPropertyPlayer1()) {
				getContextWindow()->getWindow().draw(*text_ptr);
			}
			monopoly_game_engine_.getWithdraw().getSpritePropertyPlayer1Index().setTexture(
				*monopoly_game_engine_.getWithdraw().getTexturePropertyPlayer1Index(), true);
			getContextWindow()->getWindow().draw(monopoly_game_engine_.getWithdraw().getSpritePropertyPlayer1Index());
			for (auto text_ptr : monopoly_game_engine_.getWithdraw().getTextsPropertyPlayer1Index()) {
				getContextWindow()->getWindow().draw(*text_ptr);
			}
			monopoly_game_engine_.getWithdraw().getSpritePropertyPlayer2().setTexture(
				*monopoly_game_engine_.getWithdraw().getTexturePropertyPlayer2(), true);
			getContextWindow()->getWindow().draw(monopoly_game_engine_.getWithdraw().getSpritePropertyPlayer2());
			for (auto text_ptr : monopoly_game_engine_.getWithdraw().getTextsPropertyPlayer2()) {
				getContextWindow()->getWindow().draw(*text_ptr);
			}
			monopoly_game_engine_.getWithdraw().getSpritePropertyPlayer2Index().setTexture(
				*monopoly_game_engine_.getWithdraw().getTexturePropertyPlayer2Index(), true);
			getContextWindow()->getWindow().draw(monopoly_game_engine_.getWithdraw().getSpritePropertyPlayer2Index());
			for (auto text_ptr : monopoly_game_engine_.getWithdraw().getTextsPropertyPlayer2Index()) {
				getContextWindow()->getWindow().draw(*text_ptr);
			}
		case WITHDRAW_CHOOSE_PLAYER:	// Intentional fall through
		case WITHDRAW_DECISION:
			for (auto button_ptr : monopoly_game_engine_.getWithdraw().getButtons()) {
				if (button_ptr->getIsVisible()) {
					button_ptr->draw(getContextWindow()->getWindow());
				}
			}

			for (auto text_ptr : monopoly_game_engine_.getWithdraw().getTexts()) {
				getContextWindow()->getWindow().draw(*text_ptr);
			}
			break;

		case AUCTION:
			// draw buttons and texts
			for (auto element : monopoly_game_engine_.getAuctionButtons()) {
				if (element->getIsVisible()) {
					element->draw(getContextWindow()->getWindow());
				}
			}
			for (auto element : monopoly_game_engine_.getAuctionTexts()) {
				getContextWindow()->getWindow().draw(*element);
			}

			// draw property data for buy/sell
			getContextWindow()->getWindow().draw(monopoly_game_engine_.getPropertyDataSprite());
			for (auto text_ptr : monopoly_game_engine_.getPropertyDataTexts()) {
				getContextWindow()->getWindow().draw(*text_ptr);
			}
			break;

		case RESULT:

		break;

		default:
			break;
	}
}

std::vector<std::shared_ptr<Player>> GameScreen::getPlayersResult() {
	return monopoly_game_engine_.getPlayersResult();
}