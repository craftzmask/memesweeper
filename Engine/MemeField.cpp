#include "MemeField.h"
#include "Vei2.h"
#include "assert.h"
#include "SpriteCodex.h"
#include <random>

MemeField::MemeField(int nMemes)
{
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> xDist(0, width - 1);
	std::uniform_int_distribution<int> yDist(0, height - 1);

	for (int i = 0; i < nMemes; i++)
	{
		int index;
		do
		{
			index = yDist(rng) * width + xDist(rng);
		} while (field[index].HasMeme());

		field[index].SpawnMeme();
	}
}

void MemeField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(0, 0, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize, SpriteCodex::baseColor);

	for (Vei2 gridPos = { 0, 0 }; gridPos.y < height; gridPos.y++)
	{
		for (gridPos.x = 0; gridPos.x < width; gridPos.x++)
		{
			TileAt(gridPos).Draw(gridPos * SpriteCodex::tileSize, isFucked, gfx);
		}
	}
}

const MemeField::Tile& MemeField::TileAt(const Vei2& gridPos) const
{
	return field[gridPos.y * width + gridPos.x];
}

MemeField::Tile& MemeField::TileAt(const Vei2& gridPos)
{
	return field[gridPos.y * width + gridPos.x];
}

void MemeField::OnRevealClick(const Vei2& screenPos)
{
	assert(Contains(screenPos));
	const Vei2 gridPos = ScreenToGrid(screenPos);
	Tile& tile = TileAt(gridPos);
	if (!tile.HasRevealed() && !tile.HasFlagged())
	{
		tile.Reveal();
		if (tile.HasMeme())
		{
			isFucked = true;
		}
	}
}

void MemeField::OnFlagClick(const Vei2& screenPos)
{
	assert(Contains(screenPos));
	const Vei2 gridPos = ScreenToGrid(screenPos);
	Tile& tile = TileAt(gridPos);
	if (!tile.HasRevealed())
	{
		tile.ToggleFlag();
	}
}

bool MemeField::Contains(const Vei2& screenPos) const
{
	return screenPos.x >= 0 && screenPos.x < width * SpriteCodex::tileSize && screenPos.y >= 0 && screenPos.y < height * SpriteCodex::tileSize;
}

Vei2 MemeField::ScreenToGrid(const Vei2& screenPos) const
{
	return screenPos / SpriteCodex::tileSize;
}

void MemeField::Tile::Draw(const Vei2& gridPos, bool isFucked, Graphics& gfx) const
{
	if (!isFucked)
	{
		switch (state)
		{
		case State::Hidden:
			SpriteCodex::DrawTileButton(gridPos, gfx);
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton(gridPos, gfx);
			SpriteCodex::DrawTileFlag(gridPos, gfx);
			break;
		case State::Revealed:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(gridPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTile0(gridPos, gfx);
			}
			break;
		}
	}
	else
	{
		switch (state)
		{
		case State::Hidden:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(gridPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileButton(gridPos, gfx);
			}
			break;
		case State::Flagged:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBomb(gridPos, gfx);
				SpriteCodex::DrawTileFlag(gridPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTileBomb(gridPos, gfx);
				SpriteCodex::DrawTileCross(gridPos, gfx);
			}
			break;
		case State::Revealed:
			if (HasMeme())
			{
				SpriteCodex::DrawTileBombRed(gridPos, gfx);
				SpriteCodex::DrawTileBomb(gridPos, gfx);
			}
			else
			{
				SpriteCodex::DrawTile0(gridPos, gfx);
			}
			break;
		}
	}

}

void MemeField::Tile::Reveal()
{
	assert(state == State::Hidden || !HasFlagged());
	state = State::Revealed;
}

bool MemeField::Tile::HasRevealed() const
{
	return state == State::Revealed;
}

void MemeField::Tile::ToggleFlag()
{
	assert(!HasRevealed());
	if (state == State::Flagged)
	{
		state = State::Hidden;
	}
	else
	{
		state = State::Flagged;
	}
}

bool MemeField::Tile::HasFlagged() const
{
	return state == State::Flagged;
}

void MemeField::Tile::SpawnMeme()
{
	assert(!HasMeme());
	hasMeme = true;
}

bool MemeField::Tile::HasMeme() const
{
	return hasMeme;
}
