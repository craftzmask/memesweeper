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
		void Draw(const Vei2& gridPos, bool isFucked, Graphics& gfx) const;
		void Reveal();
		bool HasRevealed() const;
		void ToggleFlag();
		bool HasFlagged() const;
		void SpawnMeme();
		bool HasMeme() const;
		void SetNeighborMemes(int nMemes);
	private:
		void DrawTileNumber(const Vei2& gridPos, int n, Graphics& gfx) const;
	private:
		State state = State::Hidden;
		bool hasMeme = false;
		int nNeighborMemes = -1;
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
	int GetNeightborMemes(const Vei2& gridPos) const;
	bool isFucked = false;

private:
	static constexpr int width = 20;
	static constexpr int height = 20;
	Tile field[width * height];
};