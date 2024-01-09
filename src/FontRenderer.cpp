#include "FontRenderer.h"
#include "SDL_ttf.h"
#include <SDL.h>
#include "GameRenderer.h"
#include "TransformUtil.h"
#include "Vector.h"
#include "Entity.h"
#include <tuple>

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
            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            
            auto screenPos = UtilSimpleECS::TransformUtil::worldToScreenSpace(position.x, position.y);
            SDL_Rect renderQuad = { screenPos.x - textWidth/2, screenPos.y - textHeight/2, textWidth, textHeight };
            SDL_RenderCopy(GameRenderer::renderer, textTexture, NULL, &renderQuad);
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
}

SimpleECS::FontRenderer::FontRenderer(std::string text, std::string pathToFont, uint16_t size)
{
    this->text = text;
    this->path = pathToFont;
    this->size = size;

    pImpl = std::make_unique<FontRendererImpl>();
}

FontRenderer::FontRenderer(std::string text, std::string pathToFont)
{
    this->text = text;
    this->path = pathToFont;

    pImpl = std::make_unique<FontRendererImpl>();
}

// Font renderer implementation
void FontRenderer::initialize()
{
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    }

	pImpl->font = TTF_OpenFont(path.c_str(), size);
    if (pImpl->font == NULL)
    {
        printf("Unable to open font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    pImpl->renderText(text, color, Vector(entity->transform.position.x, entity->transform.position.y));
}

void SimpleECS::FontRenderer::update()
{
    pImpl->renderText(text, color, Vector(entity->transform.position.x, entity->transform.position.y));
}
