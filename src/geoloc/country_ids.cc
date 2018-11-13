/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <array>

#include "../thirdparty/loguru.hh"
#include "country_ids.hh"

static std::array<const char *, (size_t) shiro::geoloc::country_id::LAST> country_lookups = {
    "BL", // 0
    "XX", // 1
    "XX", // 2
    "AD", // 3
    "XX", // 4
    "AF", // 5
    "AI", // 7
    "AL", // 8
    "AM", // 9

    "XX", // 10
    "AO", // 11
    "AQ", // 12
    "AR", // 13
    "AS", // 14
    "AT", // 15
    "AU", // 16
    "AW", // 17
    "AZ", // 18
    "XX", // 19

    "BB", // 20
    "BD", // 21
    "BE", // 22
    "BF", // 23
    "BG", // 24
    "BH", // 25
    "BI", // 26
    "BJ", // 27
    "BM", // 28
    "XX", // 29

    "BO", // 30
    "BR", // 31
    "BS", // 32
    "BT", // 33
    "BV", // 34
    "BW", // 35
    "BY", // 36
    "BZ", // 37
    "CA", // 38
    "CC", // 39

    "XX", // 40
    "CF", // 41
    "XX", // 42
    "CH", // 43
    "XX", // 44
    "CK", // 45
    "CL", // 46
    "CM", // 47
    "CN", // 48
    "CO", // 49

    "CR", // 50
    "CU", // 51
    "CV", // 52
    "CX", // 53
    "CY", // 54
    "CZ", // 55
    "DE", // 56
    "DJ", // 57
    "DK", // 58
    "DM", // 59

    "DO", // 60
    "DZ", // 61
    "EC", // 62
    "EE", // 63
    "EG", // 64
    "EH", // 65
    "ER", // 66
    "ES", // 67
    "ET", // 68
    "FI", // 69

    "FJ", // 70
    "FK", // 71
    "XX", // 72
    "FO", // 73
    "FR", // 74
    "XX", // 75
    "GA", // 76
    "GB", // 77
    "GD", // 78
    "GE", // 79

    "GF", // 80
    "GH", // 81
    "GI", // 82
    "GL", // 83
    "GM", // 84
    "GN", // 85
    "GP", // 86
    "GQ", // 87
    "GR", // 88
    "XX", // 89

    "GT", // 90
    "GU", // 91
    "GW", // 92
    "GY", // 93
    "HK", // 94
    "XX", // 95
    "HN", // 96
    "HR", // 97
    "HT", // 98
    "HU", // 99

    "ID", // 100
    "IE", // 101
    "IL", // 102
    "IN", // 103
    "IO", // 104
    "IQ", // 105
    "IS", // 107
    "IT", // 108
    "JM", // 109

    "JO", // 110
    "JP", // 111
    "KE", // 112
    "KG", // 113
    "KH", // 114
    "KI", // 115
    "KM", // 116
    "XX", // 117
    "XX", // 118
    "XX", // 119

    "KW", // 120
    "KY", // 121
    "KZ", // 122
    "XX", // 123
    "LB", // 124
    "LI", // 126
    "LK", // 127
    "LR", // 128
    "LS", // 129

    "LT", // 130
    "LU", // 131
    "LV", // 132
    "XX", // 133
    "MA", // 134
    "MC", // 135
    "XX", // 136
    "MG", // 137
    "MH", // 138
    "XX", // 139

    "ML", // 140
    "MM", // 141
    "MN", // 142
    "MP", // 144
    "MQ", // 145
    "MR", // 146
    "MS", // 147
    "MT", // 148
    "MU", // 149

    "MV", // 150
    "MW", // 151
    "MX", // 152
    "MY", // 153
    "MZ", // 154
    "NA", // 155
    "NC", // 156
    "NE", // 157
    "NF", // 158
    "NG", // 159

    "NI", // 160
    "NL", // 161
    "NO", // 162
    "NP", // 163
    "NR", // 164
    "NU", // 165
    "NZ", // 166
    "OM", // 167
    "PA", // 168
    "PE", // 169

    "PF", // 170
    "PG", // 171
    "PH", // 172
    "PK", // 173
    "PL", // 174
    "XX", // 175
    "PN", // 176
    "PR", // 177
    "PS", // 178
    "PT", // 179

    "PW", // 180
    "PY", // 181
    "QA", // 182
    "RE", // 183
    "RO", // 184
    "XX", // 185
    "RW", // 186
    "SA", // 187
    "SB", // 188
    "SC", // 189

    "SD", // 190
    "SE", // 191
    "SG", // 192
    "XX", // 193
    "SI", // 194
    "SJ", // 195
    "SK", // 196
    "SL", // 197
    "SM", // 198
    "SN", // 199

    "SO", // 200
    "SR", // 201
    "ST", // 202
    "SV", // 203
    "XX", // 204
    "SZ", // 205
    "TC", // 206
    "TD", // 207
    "TF", // 208
    "TG", // 209

    "TH", // 210
    "TJ", // 211
    "TK", // 212
    "TM", // 213
    "TN", // 214
    "TO", // 215
    "XX", // 216
    "TR", // 217
    "TT", // 218
    "TV", // 219

    "TW", // 220
    "TZ", // 221
    "UA", // 222
    "UG", // 223
    "XX", // 224
    "US", // 225
    "UY", // 226
    "UZ", // 227
    "XX", // 228
    "XX", // 229

    "VE", // 230
    "XX", // 231
    "XX", // 232
    "VN", // 233
    "VU", // 234
    "WF", // 235
    "WS", // 236
    "YE", // 237
    "YT", // 238
    "RS", // 239

    "ZA", // 240
    "ZM", // 241
    "ME", // 242
    "ZW", // 243
    "XX", // 244
    "XX", // 245
    "XX", // 246
    "AX", // 247
    "GG", // 248
    "IM", // 249

    "JE", // 250
    "XX", // 251
    "MF", // 252
};

uint8_t shiro::geoloc::get_country_id(const std::string &country_code) {
    uint8_t i = 0;
    for (const auto &code : country_lookups) {
        if (country_code.find(code, 0) != std::string::npos)
            return i;

        ++i;
    }

    LOG_F(WARNING, "Unable to find country id for country %s.", country_code.c_str());

    return (uint8_t) country_id::JP; // Default to Japan because we're weebs ¯\_(ツ)_/¯
}

std::string shiro::geoloc::get_country_name(uint8_t country) {

    if (country < (uint8_t) country_id::LAST)
        return country_lookups[country];

    LOG_F(WARNING, "Unable to find country name for country id: %hhu", country);
    return "JP"; // Default to Japan because we're weebs ¯\_(ツ)_/¯
}
