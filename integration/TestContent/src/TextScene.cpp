//  -------------------------------------------------------------------------
//  Copyright (C) 2015 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#include "TestScenes/TextScene.h"
#include "ramses-client-api/RamsesClient.h"
#include "ramses-client-api/Scene.h"
#include "ramses-client-api/TranslateNode.h"
#include "ramses-client-api/MeshNode.h"
#include "ramses-client-api/Appearance.h"
#include "ramses-client-api/Effect.h"
#include "ramses-client-api/EffectDescription.h"
#include "ramses-client-api/UniformInput.h"
#include "ramses-client-api/OrthographicCamera.h"
#include "PlatformAbstraction/PlatformMath.h"
#include "ramses-utils.h"

namespace ramses_internal
{
    TextScene::TextScene(ramses::RamsesClient& ramsesClient, ramses::Scene& scene, UInt32 state, const Vector3& cameraPosition)
        : TextScene_Base(ramsesClient, scene, cameraPosition)
    {
        const ramses::FontId font = m_fontRegistry.createFreetype2Font("res/ramses-test-client-Roboto-Bold.ttf");
        const ramses::FontId chineseFont = m_fontRegistry.createFreetype2Font("res/ramses-test-client-WenQuanYiMicroHei.ttf");
        const ramses::FontId lightFont = m_fontRegistry.createFreetype2Font("res/ramses-test-client-Roboto-Light.ttf");
        const ramses::FontId arabicFont = m_fontRegistry.createFreetype2Font("res/ramses-test-client-DroidKufi-Regular.ttf");

        m_font = m_fontRegistry.createFreetype2FontInstance(font, 64u);
        m_chineseFont = m_fontRegistry.createFreetype2FontInstance(chineseFont, 32u);
        m_lightFont = m_fontRegistry.createFreetype2FontInstance(lightFont, 26u);
        m_lightAutoHintFont = m_fontRegistry.createFreetype2FontInstance(lightFont, 26u, true);
        m_shapingArabicFont = m_fontRegistry.createFreetype2FontInstanceWithHarfBuzz(arabicFont, 26u);
        m_shapingArabicAutoHintFont = m_fontRegistry.createFreetype2FontInstanceWithHarfBuzz(arabicFont, 26u, true);

        /// Set appearance uniforms
        ramses::UniformInput colorInput;
        ramses::Effect* textEffect = ramses::RamsesUtils::CreateStandardTextEffect(m_client, colorInput);

        /// Create text lines
        //const std::u32string str = U"ÄÖÜß";
        std::u32string str{ 196, 214, 220, 223 };
        auto glyphMetrics = m_textCache.getPositionedGlyphs(str, m_font);
        m_textUTF = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshUTF = m_textCache.getTextLine(m_textUTF)->meshNode;

        glyphMetrics = m_textCache.getPositionedGlyphs(U"Text", m_font);
        m_textASCII = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshASCII = m_textCache.getTextLine(m_textASCII)->meshNode;

        glyphMetrics = m_textCache.getPositionedGlyphs(U"12345", m_font);
        m_textDigits = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshDigits = m_textCache.getTextLine(m_textDigits)->meshNode;

        //str = U"我們在天上的父，";
        str = { 25105, 20497, 22312, 22825, 19978, 30340, 29238, 65292 };
        glyphMetrics = m_textCache.getPositionedGlyphs(str, m_chineseFont);
        m_textChinese = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshChinese = m_textCache.getTextLine(m_textChinese)->meshNode;

        glyphMetrics = m_textCache.getPositionedGlyphs(U"NO au7ohint1ng", m_lightFont);
        m_textLight = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshLight = m_textCache.getTextLine(m_textLight)->meshNode;

        glyphMetrics = m_textCache.getPositionedGlyphs(U"au7ohint1ng ON", m_lightAutoHintFont);
        m_textLightAutoHinting = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshLightAutoHinting = m_textCache.getTextLine(m_textLightAutoHinting)->meshNode;

        //str = U"الصحة";
        str = { 0x627, 0x644, 0x635, 0x62d, 0x629 };
        glyphMetrics = m_textCache.getPositionedGlyphs(str, m_shapingArabicFont);
        m_textShaping = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshShaping = m_textCache.getTextLine(m_textShaping)->meshNode;

        //str = U"الصحة (autohint)";
        str = { 0x627, 0x644, 0x635, 0x62d, 0x629, 0x20, 0x028, 0x061, 0x075, 0x074, 0x06f, 0x068, 0x069, 0x06e, 0x074, 0x029 };
        const ramses::FontInstanceOffsets fontOffsets{ { m_shapingArabicAutoHintFont, 0u },{ m_lightAutoHintFont, 5u } };
        glyphMetrics = m_textCache.getPositionedGlyphs(str, fontOffsets);
        m_textShapingAutoHint = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshShapingAutoHint = m_textCache.getTextLine(m_textShapingAutoHint)->meshNode;

        //str = U"るョン(X6)耀世!";
        str = { 0x308b, 0x30e7, 0x30f3, 0x0028, 0x0058, 0x0036, 0x0029, 0x8000, 0x4e16, 0x0021 };
        const ramses::FontInstanceId smallFont = m_fontRegistry.createFreetype2FontInstance(font, 26u);
        const ramses::FontInstanceId smallChineseFont = m_fontRegistry.createFreetype2FontInstance(chineseFont, 26u);
        const ramses::FontInstanceOffsets fontOffsets2{ { smallChineseFont, 0u },{ smallFont, 3u },{ smallChineseFont, 7u },{ smallFont, 9u } };
        glyphMetrics = m_textCache.getPositionedGlyphs(str, fontOffsets2);
        m_textFontCascade = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshFontCascade = m_textCache.getTextLine(m_textFontCascade)->meshNode;

        // add vertical offset for chinese font
        for (auto& glyph : glyphMetrics)
        {
            if (glyph.key.fontInstanceId == smallChineseFont)
                glyph.posY -= 40;
        }
        m_textFontCascadeWithVerticalOffset = m_textCache.createTextLine(glyphMetrics, *textEffect);
        m_meshFontCascadeWithVerticalOffset = m_textCache.getTextLine(m_textFontCascadeWithVerticalOffset)->meshNode;

        for (auto textMesh : { m_meshUTF, m_meshASCII, m_meshDigits, m_meshChinese, m_meshLight, m_meshLightAutoHinting, m_meshShaping, m_meshShapingAutoHint, m_meshFontCascade, m_meshFontCascadeWithVerticalOffset })
        {
            textMesh->getAppearance()->setInputValueVector4f(colorInput, 1.0f, 0.0f, 0.0f, 1.0f);
            textMesh->getAppearance()->setBlendingOperations(ramses::EBlendOperation_Add, ramses::EBlendOperation_Add);
            textMesh->getAppearance()->setBlendingFactors(ramses::EBlendFactor_SrcAlpha, ramses::EBlendFactor_OneMinusSrcAlpha, ramses::EBlendFactor_SrcAlpha, ramses::EBlendFactor_OneMinusSrcAlpha);
        }
        for (auto textMesh : { m_meshChinese, m_meshFontCascade, m_meshFontCascadeWithVerticalOffset })
            textMesh->getAppearance()->setInputValueVector4f(colorInput, 0.0f, 0.0f, 1.0f, 1.0f);

        ramses::TranslateNode* translateUTF = m_scene.createTranslateNode();
        ramses::TranslateNode* translateASCII = m_scene.createTranslateNode();
        ramses::TranslateNode* translateDigits = m_scene.createTranslateNode();
        ramses::TranslateNode* translateChinese = m_scene.createTranslateNode();
        ramses::TranslateNode* translateLight = m_scene.createTranslateNode();
        ramses::TranslateNode* translateLightAutoHinting = m_scene.createTranslateNode();
        ramses::TranslateNode* translateFontCascade = m_scene.createTranslateNode();
        ramses::TranslateNode* translateShaping = m_scene.createTranslateNode();
        ramses::TranslateNode* translateShapingAutoHint = m_scene.createTranslateNode();

        translateUTF->setTranslation(2, 10, -1);
        translateASCII->setTranslation(2, 80, -1);
        translateDigits->setTranslation(2, 160, -1);
        translateChinese->setTranslation(2, 75, -1);
        translateLight->setTranslation(2, 120, -1);
        translateLightAutoHinting->setTranslation(2, 90, -1);
        translateFontCascade->setTranslation(2, 110, -1);
        translateShaping->setTranslation(2, 10, -1);
        translateShapingAutoHint->setTranslation(2, 80, -1);

        m_meshUTF->setParent(*translateUTF);
        m_meshASCII->setParent(*translateASCII);
        m_meshDigits->setParent(*translateDigits);
        m_meshChinese->setParent(*translateChinese);
        m_meshLight->setParent(*translateLight);
        m_meshLightAutoHinting->setParent(*translateLightAutoHinting);
        m_meshFontCascade->setParent(*translateFontCascade);
        m_meshFontCascadeWithVerticalOffset->setParent(*translateFontCascade);
        m_meshShaping->setParent(*translateShaping);
        m_meshShapingAutoHint->setParent(*translateShapingAutoHint);

        setState(state);
    }

    void TextScene::setState(UInt32 state)
    {
        switch (state)
        {
        case EState_INITIAL:
        case EState_INITIAL_128_BY_64_VIEWPORT:
            addMeshNodeToDefaultRenderGroup(*m_meshUTF, 1);
            addMeshNodeToDefaultRenderGroup(*m_meshASCII, 2);
            addMeshNodeToDefaultRenderGroup(*m_meshDigits, 3);
            addMeshNodeToDefaultRenderGroup(*m_meshChinese, 0);
            break;

        case EState_DELETED_TEXTS:
        {
            addMeshNodeToDefaultRenderGroup(*m_meshUTF, 1);
            addMeshNodeToDefaultRenderGroup(*m_meshASCII, 2);
            addMeshNodeToDefaultRenderGroup(*m_meshDigits, 3);
            addMeshNodeToDefaultRenderGroup(*m_meshChinese, 0);

            m_textCache.deleteTextLine(m_textUTF);
            m_textCache.deleteTextLine(m_textUTF);
            m_textCache.deleteTextLine(m_textDigits);
            m_textCache.deleteTextLine(m_textASCII);
            break;
        }

        case EState_FORCE_AUTO_HINTING:
        {
            addMeshNodeToDefaultRenderGroup(*m_meshLight, 0);
            addMeshNodeToDefaultRenderGroup(*m_meshLightAutoHinting, 0);
        }
        break;

        case EState_FONT_CASCADE:
        {
            addMeshNodeToDefaultRenderGroup(*m_meshFontCascade, 0);
        }
        break;

        case EState_FONT_CASCADE_WITH_VERTICAL_OFFSET:
        {
            addMeshNodeToDefaultRenderGroup(*m_meshFontCascadeWithVerticalOffset, 0);
        }
        break;

        case EState_SHAPING:
        {
            addMeshNodeToDefaultRenderGroup(*m_meshShaping, 0);
            addMeshNodeToDefaultRenderGroup(*m_meshShapingAutoHint, 0);
            break;
        }

        }

        if (EState_INITIAL_128_BY_64_VIEWPORT == state)
        {
            addMeshNodeToDefaultRenderGroup(*m_meshUTF, 1);
            addMeshNodeToDefaultRenderGroup(*m_meshASCII, 2);
            addMeshNodeToDefaultRenderGroup(*m_meshDigits, 3);
            addMeshNodeToDefaultRenderGroup(*m_meshChinese, 0);

            m_textOrthoCamera->setViewport(0, 0, 128, 64);
        }
    }
}
