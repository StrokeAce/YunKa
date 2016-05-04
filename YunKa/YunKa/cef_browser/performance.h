// Copyright (c) 2012 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFCLIENT_PERFORMANCE_H_
#define CEF_TESTS_CEFCLIENT_PERFORMANCE_H_
#pragma once

#include "cef_browser/client_app.h"

void setHandel(HWND hend);

namespace performance {

// Render delegate creation. Called from client_app_delegates.cpp.
	void CreateRenderDelegates(ClientApp::RenderDelegateSet& delegates);

}  // namespace performance

#endif  // CEF_TESTS_CEFCLIENT_PERFORMANCE_H_
