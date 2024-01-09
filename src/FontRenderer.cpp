#include "FontRenderer.h"
#include "SDL_ttf.h"
#include <SDL.h>
#include "GameRenderer.h"
#include "TransformUtil.h"
#include "Vector.h"

using namespace SimpleECS;
using namespace UtilSimpleECS;

// Impl implementation
class FontRenderer::FontRendererImpl {
public:
    FontRendererImpl();
    ~FontRendererImpl();

	void renderText(std::string text, Color color, Vector position);
	TTF_Font* font = nullptr;
    SDL_Texture* textTexture = nullptr;
};

FontRenderer::FontRendererImpl::FontRendererImpl()
{

}

FontRenderer::FontRendererImpl::~FontRendererImpl()
{
    SDL_DestroyTexture(textTexture);
}

void FontRenderer::FontRendererImpl::renderText(std::string text, Color color, Vector position)
{
	// Create temporary surface
	SDL_Color renderColor = { color.r, color.g, color.b, color.a };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), renderColor);
    if (textSurface == NULL)
    {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //Create texture from surface pixels
        textTexture = SDL_CreateTextureFromSurface(GameRenderer::renderer, textSurface);
        if (textTexture == NULL)
        {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions
            int mWidth = textSurface->w;
            int mHeight = textSurface->h;
            
            auto screenPos = UtilSimpleECS::TransformUtil::worldToScreenSpace(position.x, position.y);
            SDL_Rect renderQuad = { screenPos.first, screenPos.second, textSurface->w, textSurface->h };
            SDL_RenderCopy(GameRenderer::renderer, textTexture, NULL, &renderQuad);
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
}

FontRenderer::FontRenderer(std::string text, std::string pathToFont)
{
    this->text = text;
    this->path = pathToFont;
}

// Font renderer implementation
void FontRenderer::initialize()
{
	pImpl->font = TTF_OpenFont(path.c_str(), 28);
}

void SimpleECS::FontRenderer::update()
{

}
