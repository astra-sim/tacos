/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#pragma once

#ifdef DEBUG
#define DebugLog(x) x;
#define ReleaseLog(x)
#else
#define DebugLog(x)
#define ReleaseLog(x) x;
#endif
