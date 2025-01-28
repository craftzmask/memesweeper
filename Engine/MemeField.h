#pragma once
#include "Graphics.h"

class MemeField
{
	class Tile
	{
		enum class State
		{
			Revealed,
			Hidden,
			Flagged
		};

	public:
		void Draw(const Vei2& gridPos, Graphics& gfx) const;
		void Reveal();
		bool HasRevealed() const;
		void ToggleFlag();
		bool HasFlagged() const;
		void SpawnMeme();
		bool HasMeme() const;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
	};

public:
	MemeField(int nMemes);
	void Draw(Graphics& gfx) const;
	const Tile& TileAt(const Vei2& gridPos) const;
	Tile& TileAt(const Vei2& gridPos);
	void OnRevealClick(const Vei2& screenPos);
	void OnFlagClick(const Vei2& screenPos);
	bool Contains(const Vei2& screenPos) const;
	Vei2 ScreenToGrid(const Vei2& screenPos) const;

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile field[width * height];
};