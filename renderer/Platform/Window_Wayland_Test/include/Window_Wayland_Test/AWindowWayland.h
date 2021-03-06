//  -------------------------------------------------------------------------
//  Copyright (C) 2017 BMW Car IT GmbH
//  -------------------------------------------------------------------------
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at https://mozilla.org/MPL/2.0/.
//  -------------------------------------------------------------------------

#ifndef RAMSES_AWINDOWWAYLAND_H
#define RAMSES_AWINDOWWAYLAND_H

#include "WindowEventHandlerMock.h"
#include "RendererLib/DisplayConfig.h"
#include "RendererTestUtils.h"
#include "DisplayConfigImpl.h"
#include "UnixUtilities/EnvironmentVariableHelper.h"
#include "UnixUtilities/UnixDomainSocketHelper.h"
#include "Utils/StringUtils.h"

using namespace testing;


namespace ramses_internal
{
    template <typename WINDOWTYPE>
    class AWindowWayland : public testing::Test
    {
    public:
        virtual void SetUp() override
        {
            createWaylandWindow();
        }

        virtual void TearDown() override
        {
            destroyWaylandWindow();
        }

    protected:
        void createWaylandWindow()
        {
            m_window = new WINDOWTYPE(m_config.impl.getInternalDisplayConfig(), m_eventHandlerMock, 0);
        }

        void destroyWaylandWindow()
        {
            delete m_window;
            m_window = nullptr;
        }

        StrictMock<WindowEventHandlerMock> m_eventHandlerMock;
        ramses::DisplayConfig              m_config = RendererTestUtils::CreateTestDisplayConfig(0);
        WINDOWTYPE*                        m_window = nullptr;
        EnvironmentVariableHelper m_environment;
    };
}

#endif
