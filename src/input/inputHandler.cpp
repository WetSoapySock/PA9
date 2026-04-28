#include "../include/input/InputHandler.h"

Minesweeper::InputHandler::InputHandler()
{
	enabled = true;
}

void Minesweeper::InputHandler::processEvent(const sf::Event& event)
{
	if (enabled == false)
	{
		return;
	}

	auto found = callbacks.find(event.type);

	if (found != callbacks.end())
	{
		for (int i = 0; i < found->second.size(); i++)
		{
			found->second[i](event);
		}
	}
}

void Minesweeper::InputHandler::registerCallbacks(sf::Event::EventType type, Callback callback)
{
	callbacks[type].push_back(callback);
}

void Minesweeper::InputHandler::clearCallbacks()
{
	callbacks.clear();
}