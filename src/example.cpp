/**
 * @file        example.cpp
 * @brief       A small library made to try arduino-installer on: a period that never blocks.
 * @version     1.2.0
 *
 * @author      Nima Askari (NimaLTD)
 * @email       nima.askari@gmail.com
 * @github      https://www.github.com/nimaltd
 * @linkedin    https://www.linkedin.com/in/nimaltd
 * @youtube     https://www.youtube.com/@nimaltd
 * @instagram   https://instagram.com/github.nimaltd
 *
 * @copyright   (c) 2026 Nima Askari (NimaLTD)
 *              SPDX-License-Identifier: Apache-2.0
 *              See LICENSE.md in the project root for the full license text.
 *
 * @note        C++, so example_hello() can print on Serial. The functions keep
 *              C linkage through example.h, so a C file can call them too.
 */

/*
 * ****************************************************************************************************
 * Includes
 * ****************************************************************************************************
*/

#include "example.h"
#include <Arduino.h>

/*
 * ****************************************************************************************************
 * Public function implementations
 * ****************************************************************************************************
*/

/*****************************************************************************************************/
/**
 * @brief Start a period of EXAMPLE_PERIOD_MS from now.
 *
 * @param[out] handle  The period to start.
 */
void example_init(example_t *handle)
{
    if (handle != NULL)
    {
        handle->start = millis();
        handle->count = 0U;
    }
}

/*****************************************************************************************************/
/**
 * @brief Whether the period has passed. Returns at once, and starts the next one when it has.
 *
 * @param[in,out] handle  Period from example_init().
 * @return true once every EXAMPLE_PERIOD_MS, false in between.
 */
bool example_due(example_t *handle)
{
    bool due = false;

    /* Subtracted, never added, so the answer stays right when millis() wraps
       round after 49 days. */
    if ((handle != NULL) && ((millis() - handle->start) >= (uint32_t)EXAMPLE_PERIOD_MS))
    {
        /* The next period starts where this one should have ended, not now,
           so a loop that comes back late does not make the rhythm drift. */
        handle->start += (uint32_t)EXAMPLE_PERIOD_MS;
        handle->count++;
        due = true;
    }

    return due;
}

/*****************************************************************************************************/
/**
 * @brief How many times example_due() has said yes.
 *
 * @param[in] handle  Period from example_init().
 * @return The count since example_init(), 0 for a NULL handle.
 */
uint32_t example_count(const example_t *handle)
{
    return (handle != NULL) ? handle->count : 0U;
}

/*****************************************************************************************************/
/**
 * @brief Print EXAMPLE_GREETING on Serial, and how many periods have passed.
 *
 * @param[in] handle  Period from example_init().
 */
void example_hello(const example_t *handle)
{
    Serial.print(EXAMPLE_GREETING);
    Serial.print(' ');
    Serial.println((unsigned long)example_count(handle));
}
