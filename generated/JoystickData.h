// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file JoystickData.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool fastddsgen.
 */

#ifndef _FAST_DDS_GENERATED_JOYSTICKDATA_H_
#define _FAST_DDS_GENERATED_JOYSTICKDATA_H_

#include <array>
#include <bitset>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include <fastcdr/cdr/fixed_size_string.hpp>
#include <fastcdr/xcdr/external.hpp>
#include <fastcdr/xcdr/optional.hpp>



#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#define eProsima_user_DllExport __declspec( dllexport )
#else
#define eProsima_user_DllExport
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define eProsima_user_DllExport
#endif  // _WIN32

#if defined(_WIN32)
#if defined(EPROSIMA_USER_DLL_EXPORT)
#if defined(JOYSTICKDATA_SOURCE)
#define JOYSTICKDATA_DllAPI __declspec( dllexport )
#else
#define JOYSTICKDATA_DllAPI __declspec( dllimport )
#endif // JOYSTICKDATA_SOURCE
#else
#define JOYSTICKDATA_DllAPI
#endif  // EPROSIMA_USER_DLL_EXPORT
#else
#define JOYSTICKDATA_DllAPI
#endif // _WIN32

namespace eprosima {
namespace fastcdr {
class Cdr;
class CdrSizeCalculator;
} // namespace fastcdr
} // namespace eprosima







/*!
 * @brief This class represents the structure JoystickData defined by the user in the IDL file.
 * @ingroup JoystickData
 */
class JoystickData
{
public:

    /*!
     * @brief Default constructor.
     */
    eProsima_user_DllExport JoystickData();

    /*!
     * @brief Default destructor.
     */
    eProsima_user_DllExport ~JoystickData();

    /*!
     * @brief Copy constructor.
     * @param x Reference to the object JoystickData that will be copied.
     */
    eProsima_user_DllExport JoystickData(
            const JoystickData& x);

    /*!
     * @brief Move constructor.
     * @param x Reference to the object JoystickData that will be copied.
     */
    eProsima_user_DllExport JoystickData(
            JoystickData&& x) noexcept;

    /*!
     * @brief Copy assignment.
     * @param x Reference to the object JoystickData that will be copied.
     */
    eProsima_user_DllExport JoystickData& operator =(
            const JoystickData& x);

    /*!
     * @brief Move assignment.
     * @param x Reference to the object JoystickData that will be copied.
     */
    eProsima_user_DllExport JoystickData& operator =(
            JoystickData&& x) noexcept;

    /*!
     * @brief Comparison operator.
     * @param x JoystickData object to compare.
     */
    eProsima_user_DllExport bool operator ==(
            const JoystickData& x) const;

    /*!
     * @brief Comparison operator.
     * @param x JoystickData object to compare.
     */
    eProsima_user_DllExport bool operator !=(
            const JoystickData& x) const;

    /*!
     * @brief This function copies the value in member buttonStates
     * @param _buttonStates New value to be copied in member buttonStates
     */
    eProsima_user_DllExport void buttonStates(
            const std::vector<int32_t>& _buttonStates);

    /*!
     * @brief This function moves the value in member buttonStates
     * @param _buttonStates New value to be moved in member buttonStates
     */
    eProsima_user_DllExport void buttonStates(
            std::vector<int32_t>&& _buttonStates);

    /*!
     * @brief This function returns a constant reference to member buttonStates
     * @return Constant reference to member buttonStates
     */
    eProsima_user_DllExport const std::vector<int32_t>& buttonStates() const;

    /*!
     * @brief This function returns a reference to member buttonStates
     * @return Reference to member buttonStates
     */
    eProsima_user_DllExport std::vector<int32_t>& buttonStates();


    /*!
     * @brief This function copies the value in member axisStates
     * @param _axisStates New value to be copied in member axisStates
     */
    eProsima_user_DllExport void axisStates(
            const std::vector<double>& _axisStates);

    /*!
     * @brief This function moves the value in member axisStates
     * @param _axisStates New value to be moved in member axisStates
     */
    eProsima_user_DllExport void axisStates(
            std::vector<double>&& _axisStates);

    /*!
     * @brief This function returns a constant reference to member axisStates
     * @return Constant reference to member axisStates
     */
    eProsima_user_DllExport const std::vector<double>& axisStates() const;

    /*!
     * @brief This function returns a reference to member axisStates
     * @return Reference to member axisStates
     */
    eProsima_user_DllExport std::vector<double>& axisStates();


    /*!
     * @brief This function copies the value in member hatStates
     * @param _hatStates New value to be copied in member hatStates
     */
    eProsima_user_DllExport void hatStates(
            const std::vector<int32_t>& _hatStates);

    /*!
     * @brief This function moves the value in member hatStates
     * @param _hatStates New value to be moved in member hatStates
     */
    eProsima_user_DllExport void hatStates(
            std::vector<int32_t>&& _hatStates);

    /*!
     * @brief This function returns a constant reference to member hatStates
     * @return Constant reference to member hatStates
     */
    eProsima_user_DllExport const std::vector<int32_t>& hatStates() const;

    /*!
     * @brief This function returns a reference to member hatStates
     * @return Reference to member hatStates
     */
    eProsima_user_DllExport std::vector<int32_t>& hatStates();

private:

    std::vector<int32_t> m_buttonStates;
    std::vector<double> m_axisStates;
    std::vector<int32_t> m_hatStates;

};

#endif // _FAST_DDS_GENERATED_JOYSTICKDATA_H_



