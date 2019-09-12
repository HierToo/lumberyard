/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once

namespace AZ
{
    /**
     * Enum of supported platforms. Just for user convenience.
     */
    enum PlatformID
    {
        PLATFORM_WINDOWS_32 = 0,
        PLATFORM_WINDOWS_64,
#if defined(AZ_EXPAND_FOR_RESTRICTED_PLATFORM) || defined(AZ_TOOLS_EXPAND_FOR_RESTRICTED_PLATFORMS)
#define AZ_RESTRICTED_PLATFORM_EXPANSION(CodeName, CODENAME, codename, PrivateName, PRIVATENAME, privatename, PublicName, PUBLICNAME, publicname, PublicAuxName1, PublicAuxName2, PublicAuxName3)\
        PLATFORM_##PUBLICNAME,
#if defined(AZ_EXPAND_FOR_RESTRICTED_PLATFORM)
        AZ_EXPAND_FOR_RESTRICTED_PLATFORM
#else
        AZ_TOOLS_EXPAND_FOR_RESTRICTED_PLATFORMS
#endif
#undef AZ_RESTRICTED_PLATFORM_EXPANSION
#endif
        PLATFORM_LINUX_64,
        PLATFORM_ANDROID,       // ARMv7 / 32-bit
        PLATFORM_APPLE_IOS,
        PLATFORM_APPLE_OSX,
        PLATFORM_APPLE_TV,
        PLATFORM_ANDROID_64,    // ARMv8 / 64-bit
        // Add new platforms here

        PLATFORM_MAX  ///< Must be last
    };
} // namespace AZ
