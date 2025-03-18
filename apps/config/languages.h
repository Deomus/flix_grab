#pragma once

#include <string>
#include <algorithm>

static const char*   kLanguagesAndCodes[][2] = {
    { "None",                   "none" },
    { "Arabic",                 "ar" },
    { "Bangla",                 "bn" }, 
    { "Basque",                 "eu" },
    { "Bulgarian",              "bg" },
    { "Catalan",                "ca" },
    { "Chinese China",          "zh-hans" },    //Chinese simplified
    { "Chinese Hong Kong",      "zh-hk" },    //Chinese Hong Kong
    { "Chinese Taiwan",         "zh-hant" },    //Chinese traditional
    { "Croatian",               "hr" },
    { "Czech",                  "cs" },
    { "Danish",                 "da" },
    { "Dutch",                  "nl" },
    { "English United Kingdom", "en-gb" },
    { "English United States",  "en-us" },
    { "Estonian",               "et" },
    { "Filipino",               "fil" },
    { "Finnish",                "fi" },
    { "Flemish",                "nl-be" },
    { "French Canada",          "fr-ca" },
    { "French France",          "fr-fr" },
    { "Galician",               "gl" },
    { "German",                 "de" },
    { "Greek",                  "el" },
    { "Hebrew",                 "he" },
    { "Hindi",                  "hi" },
    { "Hungarian",              "hu" },
    { "Icelandic",              "is" },
    { "Indonesian",             "id" },
    { "Irish",                  "ga" },
    { "Italian",                "it" },
    { "Japanese",               "ja" },
    { "Kannada",                "kn" },
    { "Korean",                 "ko" },
    { "Latvian",                "lv" },
    { "Lithuanian",             "lt" },
    { "Macedonian",             "mk" },
    { "Malay",                  "ms" },
    { "Malayalam",              "ml" },
    { "Norwegian",              "no" },
    { "Norwegian Bokmal",       "nb" },
    { "Polish",                 "pl" },
    { "Portuguese Brazil",      "pt-br" },
    { "Portuguese Portugal",    "pt-pt" },
    { "Romanian",               "ro" },
    { "Russian",                "ru" },
    { "Serbian",                "sr" },
    { "Slovak",                 "sk" },
    { "Slovenian",              "sl" },
    { "Spanish Latin",          "es-419" },
    { "Spanish Spain",          "es-es" },
    { "Spanish Mexico",         "es-mx" },
    { "Swahili",                "sw" },
    { "Swedish",                "sv" },
    { "Tagalog",                "tl" },
    { "Tamil",                  "ta" },
    { "Telugu",                 "te" },
    { "Thai",                   "th" },
    { "Turkish",                "tr" },
    { "Ukrainian",              "uk" },
    { "Vietnamese",             "vi" }
};

inline std::string getLangTagByLangName(const std::string& lang_name) {
    std::string lang_tag;

    for (int i = 0; i < sizeof(kLanguagesAndCodes) / sizeof(kLanguagesAndCodes[0]); ++i) {
        if (kLanguagesAndCodes[i][0] == lang_name) {
            lang_tag = kLanguagesAndCodes[i][1];
            break;
        }
    }

    if (lang_tag.empty()) {
        for (int i = 0; i < sizeof(kLanguagesAndCodes) / sizeof(kLanguagesAndCodes[0]); ++i) {
            std::string name = kLanguagesAndCodes[i][0];
            std::string name_short = name.substr(0, name.find_first_of(" "));

            if (name_short == lang_name) {
                lang_tag = kLanguagesAndCodes[i][1];

                if (name.find("Dubbed") != std::string::npos)
                    lang_tag = lang_tag + "-full";

                break;
            }
        }
    }

    return lang_tag;
}

inline std::string getLangNameByLangTag(const std::string& lang_tag) {
    std::string lang_name;
    std::string lang_tag_in = lang_tag;
    
    std::transform(lang_tag_in.begin(), lang_tag_in.end(), lang_tag_in.begin(), [](unsigned char c) { return std::tolower(c); }); //to lower case
    
    for (int i = 0; i < sizeof(kLanguagesAndCodes) / sizeof(kLanguagesAndCodes[1]); ++i) {
        std::string tag = kLanguagesAndCodes[i][1];

        if (tag == lang_tag_in) {
            lang_name = kLanguagesAndCodes[i][0];
            break;
        }
    }

    if (lang_name.empty()) {
        for (int i = 0; i < sizeof(kLanguagesAndCodes) / sizeof(kLanguagesAndCodes[1]); ++i) {
            std::string tag = kLanguagesAndCodes[i][1];
            std::string tag_short = tag.substr(0, tag.find_first_of("-"));

            if (tag_short == lang_tag_in) {
                std::string name = kLanguagesAndCodes[i][0];
                lang_name = name.substr(0, name.find_first_of(" "));
                break;
            }
        }
    }

    if (lang_name.empty()) {
        for (int i = 0; i < sizeof(kLanguagesAndCodes) / sizeof(kLanguagesAndCodes[1]); ++i) {
            std::string tag = kLanguagesAndCodes[i][1];
            std::string tag_short = tag.substr(0, tag.find_first_of("-"));
            std::string lang_tag_in_short = lang_tag_in.substr(0, lang_tag_in.find_first_of("-"));

            if (tag_short == lang_tag_in_short) {
                std::string name = kLanguagesAndCodes[i][0];
                lang_name = name.substr(0, name.find_first_of(" "));
                break;
            }
        }

        if (!lang_name.empty()
            && lang_tag_in.find("full") != std::string::npos)
            lang_name = lang_name + " Dubbed";
    }

    return lang_name;
}

inline std::string normalizeLangTag(std::string data) {
    data.erase(remove_if(data.begin(), data.end(), isspace), data.end()); //remove spaces
    std::string tag = data;

    if (!tag.empty()) {
        std::transform(tag.begin(), tag.end(), tag.begin(), [](unsigned char c) { return std::tolower(c); }); //to lower case

        if (tag == "zh-cn"
            || tag == "cmn"
            || tag == "cmn-cn"
            || tag == "yue")
            tag = getLangTagByLangName("Chinese China");
        else if (tag == "zh-tw"
            || tag == "cmn-tw")
            tag = getLangTagByLangName("Chinese Taiwan");
        else if (tag == "es-us")
            tag = getLangTagByLangName("Spanish Latin");
        else if (tag == "fil")
            tag = getLangTagByLangName("Filipino");

        if (tag.find("-") != std::string::npos) {

            if (tag.find("full") != std::string::npos)
                tag = tag.substr(0, tag.find_first_of("-")) + "-full";

            std::string lang = getLangNameByLangTag(tag);
            std::string tag_short = tag.substr(0, tag.find_first_of("-"));

            if (lang.empty())
                lang = getLangNameByLangTag(tag_short);
            if (!lang.empty()
                && lang.find(" ") == std::string::npos)
                    tag = tag_short;
        }
    }

    return tag;
}

/* В XML tesseract языки пишутся по другому, поэтому это функция преобразует в формат tesseract*/
inline std::string tesseractLang(const std::string& lang) {
    std::string tes_lang = lang;
    //Afrikaans
    if (lang == "af")
        tes_lang = "afr";
    else if (lang == getLangTagByLangName("Arabic"))
        tes_lang = "ara";
    //Azerbaijani
    else if (lang == "az")
        tes_lang = "aze";
    //Belarusian
    else if (lang == "be")
        tes_lang = "bel";
    //Bengali
    else if (lang == getLangTagByLangName("Bangla"))
        tes_lang = "ben";
    else if (lang == getLangTagByLangName("Bulgarian"))
        tes_lang = "bul";
    else if (lang == getLangTagByLangName("Catalan")
        || lang == getLangTagByLangName("Valencian"))
        tes_lang = "cat";
    else if (lang == getLangTagByLangName("Czech"))
        tes_lang = "ces";
    else if (lang == getLangTagByLangName("Chinese China"))
        tes_lang = "chi_sim";
    else if (lang == getLangTagByLangName("Chinese Taiwan"))
        tes_lang = "chi_tra";
    else if (lang == getLangTagByLangName("Danish"))
        tes_lang = "dan";
    else if (lang == getLangTagByLangName("German"))
        tes_lang = "deu";
    else if (lang == getLangTagByLangName("Greek"))
        tes_lang = "ell";
    else if (lang == getLangTagByLangName("English United Kingdom")
        || lang == getLangTagByLangName("English United States"))
        tes_lang = "eng";
    //Esperanto 
    else if (lang == "eo")
        tes_lang = "epo";
    else if (lang == getLangTagByLangName("Estonian"))
        tes_lang = "est";
    else if (lang == getLangTagByLangName("Basque"))
        tes_lang = "eus";
    else if (lang == getLangTagByLangName("Filipino"))
        tes_lang = "";
    else if (lang == getLangTagByLangName("Finnish"))
        tes_lang = "fin";
    else if (lang == getLangTagByLangName("French France")
        || lang == getLangTagByLangName("French Canada"))
        tes_lang = "frk";
    //Frankish 
    else if (lang == "fra")
        tes_lang = "fra+frm";
    else if (lang == getLangTagByLangName("Galician"))
        tes_lang = "glg";
    else if (lang == getLangTagByLangName("Hebrew"))
        tes_lang = "heb";
    else if (lang == getLangTagByLangName("Hindi"))
        tes_lang = "hin";
    else if (lang == getLangTagByLangName("Croatian"))
        tes_lang = "hrv";
    else if (lang == getLangTagByLangName("Hungarian"))
        tes_lang = "hun";
    else if (lang == getLangTagByLangName("Icelandic"))
        tes_lang = "isl";
    else if (lang == getLangTagByLangName("Indonesian"))
        tes_lang = "ind";
    else if (lang == getLangTagByLangName("Irish"))
        tes_lang = "";
    else if (lang == getLangTagByLangName("Italian"))
        tes_lang = "ita";
    else if (lang == getLangTagByLangName("Japanese"))
        tes_lang = "jpn";
    //Kannada 
    else if (lang == "kan")
        tes_lang = "kan";
    else if (lang == getLangTagByLangName("Korean"))
        tes_lang = "kor";
    else if (lang == getLangTagByLangName("Latvian"))
        tes_lang = "lav";
    else if (lang == getLangTagByLangName("Lithuanian"))
        tes_lang = "lit";
    //Malayalam 
    else if (lang == "mal")
        tes_lang = "mal";
    else if (lang == getLangTagByLangName("Macedonian"))
        tes_lang = "mkd";
    //Maltese 
    else if (lang == "mt")
        tes_lang = "mlt";
    else if (lang == getLangTagByLangName("Malay"))
        tes_lang = "msa";
    else if (lang == getLangTagByLangName("Dutch")
        || lang == getLangTagByLangName("Flemish"))
        tes_lang = "nld";
    else if (lang == getLangTagByLangName("Norwegian") 
        || lang == getLangTagByLangName("Norwegian Bokmal"))
        tes_lang = "nor";
    else if (lang == getLangTagByLangName("Polish"))
        tes_lang = "pol";
    else if (lang == getLangTagByLangName("Portuguese Portugal")
        || lang == getLangTagByLangName("Portuguese Brazil"))
        tes_lang = "por";
    else if (lang == getLangTagByLangName("Romanian")   //Romanian; Moldavian; Moldovan
        || lang == "mo")
        tes_lang = "ron";
    else if (lang == getLangTagByLangName("Russian"))
        tes_lang = "rus";
    else if (lang == getLangTagByLangName("Slovak"))
        tes_lang = "slk";
    else if (lang == getLangTagByLangName("Slovenian"))
        tes_lang = "slv";
    else if (lang == getLangTagByLangName("Spanish Spain")     //Spanish; Castilian и Spanish; Castilian - Old
        || lang == getLangTagByLangName("Spanish Latin")
        || lang == getLangTagByLangName("Spanish Mexico"))
        tes_lang = "spa";
    //Albanian
    else if (lang == "sq")
        tes_lang = "sqi";
    else if (lang == getLangTagByLangName("Serbian"))
        tes_lang = "srp";
    else if (lang == getLangTagByLangName("Swahili"))
        tes_lang = "swa";
    else if (lang == getLangTagByLangName("Swedish"))
        tes_lang = "swe";
    else if (lang == getLangTagByLangName("Tamil"))
        tes_lang = "tam";
    //Telugu
    else if (lang == getLangTagByLangName("Telugu"))
        tes_lang = "tel";
    //Tagalog
    else if (lang == "tl")
        tes_lang = "tgl";
    else if (lang == getLangTagByLangName("Thai"))
        tes_lang = "tha";
    else if (lang == getLangTagByLangName("Turkish"))
        tes_lang = "tur";
    else if (lang == getLangTagByLangName("Ukrainian"))
        tes_lang = "ukr";
    else if (lang == getLangTagByLangName("Vietnamese"))
        tes_lang = "vie";

    return tes_lang;
}

inline std::pair<std::string, int64_t> tesseractDataUrl(const std::string& language_tag) {
    std::pair<std::string, int64_t> url;

    std::string lang_url = "https://download.freegrabapp.com/languages/";

    //Afrikaans
    if (language_tag == "af")
        url = { "", 0 };
    else if (language_tag == getLangTagByLangName("Arabic"))
        url = {lang_url + "ara.traineddata", 2380000};
    //Azerbaijani
    else if (language_tag == "az")
        url = {"", 0};
    //Belarusian
    else if (language_tag == "be")
        url = {"", 0};
    //Bengali
    else if (language_tag == "bn")
        url = {"", 0};
    else if (language_tag == getLangTagByLangName("Bulgarian"))
        url = { lang_url + "bul.traineddata", 7980000 };
    else if (language_tag == getLangTagByLangName("Catalan")
        || language_tag == getLangTagByLangName("Valencian"))
        url = { lang_url + "cat.traineddata", 6200000 };
    else if (language_tag == getLangTagByLangName("Czech"))
        url = { lang_url + "ces.traineddata", 15500000 };
    else if (language_tag == getLangTagByLangName("Chinese China"))
        url = {lang_url + "chi_sim.traineddata", 42300000 };
    else if (language_tag == getLangTagByLangName("Chinese Taiwan")
        || language_tag == getLangTagByLangName("Chinese Hong Kong"))
        url = {lang_url + "chi_tra.traineddata", 56300000 };
    else if (language_tag == getLangTagByLangName("Danish"))
        url = { lang_url + "dan.traineddata", 10100000 };
    else if (language_tag == getLangTagByLangName("German"))
        url = { lang_url + "deu.traineddata", 14700000 };
    else if (language_tag == getLangTagByLangName("Greek"))
        url = { lang_url + "ell.traineddata", 7190000 };
    else if (language_tag == getLangTagByLangName("English United Kingdom")
        || language_tag == getLangTagByLangName("English United States"))
        url = { lang_url + "eng.traineddata", 22400000 };
    //Esperanto 
    else if (language_tag == "eo")
        url = {"", 0};
    else if (language_tag == getLangTagByLangName("Estonian"))
        url = { lang_url + "est.traineddata", 14600000 };
    else if (language_tag == getLangTagByLangName("Basque"))
        url = { lang_url + "eus.traineddata", 9680000 };
    else if (language_tag == getLangTagByLangName("Filipino"))
        url = { "", 0 };
    else if (language_tag == getLangTagByLangName("Finnish"))
        url = { lang_url + "fin.traineddata", 20200000 };
    else if (language_tag == getLangTagByLangName("French France")
        || language_tag == getLangTagByLangName("French Canada"))
        url = { lang_url + "fra.traineddata", 13600000 };
    //Frankish 
    else if (language_tag == "fra")
        url = {"", 0};
    else if (language_tag == getLangTagByLangName("Galician"))
        url = { lang_url + "glg.traineddata", 7700000 };
    else if (language_tag == getLangTagByLangName("Hebrew"))
        url = {lang_url + "heb.traineddata", 5160000 };
    else if (language_tag == getLangTagByLangName("Hindi"))
        url = {lang_url + "hin.traineddata", 1570000 };
    else if (language_tag == getLangTagByLangName("Croatian"))
        url = { lang_url + "hrv.traineddata", 13200000 };
    else if (language_tag == getLangTagByLangName("Hungarian"))
        url = { lang_url + "hun.traineddata", 17200000 };
    else if (language_tag == getLangTagByLangName("Icelandic"))
        url = { lang_url + "isl.traineddata", 8620000 };
    else if (language_tag == getLangTagByLangName("Indonesian"))
        url = { lang_url + "ind.traineddata", 7900000 };
    else if (language_tag == getLangTagByLangName("Irish"))
        url = { "", 0 };
    else if (language_tag == getLangTagByLangName("Italian"))
        url = { lang_url + "ita.traineddata", 15200000 };
    else if (language_tag == getLangTagByLangName("Japanese"))
        url = {lang_url + "jpn.traineddata", 34000000 };
    //Kannada 
    else if (language_tag == "kan")
        url = {"", 0};
    else if (language_tag == getLangTagByLangName("Korean"))
        url = {lang_url + "kor.traineddata", 14600000 };
    else if (language_tag == getLangTagByLangName("Latvian"))
        url = { lang_url + "lav.traineddata", 10100000 };
    else if (language_tag == getLangTagByLangName("Lithuanian"))
        url = { lang_url + "lit.traineddata", 12000000 };
    //Malayalam 
    else if (language_tag == "mal")
        url = {"", 0};
    else if (language_tag == getLangTagByLangName("Macedonian"))
        url = { lang_url + "mkd.traineddata", 5080000 };
    //Maltese 
    else if (language_tag == "mt")
        url = {"", 0};
    else if (language_tag == getLangTagByLangName("Malay"))
        url = { lang_url + "msa.traineddata", 7860000 };
    else if (language_tag == getLangTagByLangName("Dutch")
        || language_tag == getLangTagByLangName("Flemish"))
        url = { lang_url + "nld.traineddata", 22100000 };
    else if (language_tag == getLangTagByLangName("Norwegian")
        || language_tag == getLangTagByLangName("Norwegian Bokmal"))
        url = { lang_url + "nor.traineddata", 11800000 };
    else if (language_tag == getLangTagByLangName("Polish"))
        url = { lang_url + "pol.traineddata", 18400000 };
    else if (language_tag == getLangTagByLangName("Portuguese Portugal")
        || language_tag == getLangTagByLangName("Portuguese Brazil"))
        url = { lang_url + "por.traineddata", 14600000 };
    else if (language_tag == getLangTagByLangName("Romanian")           //Romanian; Moldavian; Moldovan
        || language_tag == "mo")
        url = {lang_url + "ron.traineddata", 10500000 };
    else if (language_tag == getLangTagByLangName("Russian"))
        url = {lang_url + "rus.traineddata", 19000000 };
    else if (language_tag == getLangTagByLangName("Slovak"))
        url = { lang_url + "slk.traineddata", 13400000 };
    else if (language_tag == getLangTagByLangName("Slovenian"))
        url = { lang_url + "slv.traineddata", 9480000 };
    else if (language_tag == getLangTagByLangName("Spanish Spain")      //Spanish; Castilian и Spanish; Castilian - Old
        || language_tag == getLangTagByLangName("Spanish Latin")
        || language_tag == getLangTagByLangName("Spanish Mexico"))
        url = { lang_url + "spa.traineddata", 17400000 };
    //Albanian
    else if (language_tag == "sq")
        url = {"", 0};
    else if (language_tag == getLangTagByLangName("Serbian"))
        url = { lang_url + "srp.traineddata", 7090000 };
    else if (language_tag == getLangTagByLangName("Swahili"))
        url = { lang_url + "swa.traineddata", 5750000 };
    else if (language_tag == getLangTagByLangName("Swedish"))
        url = { lang_url + "swe.traineddata", 13000000 };
    else if (language_tag == getLangTagByLangName("Tamil"))
        url = { lang_url + "tam.traineddata", 3200000 };
    else if (language_tag == getLangTagByLangName("Telugu"))
        url = { lang_url + "tel.traineddata", 3160000 };
    //Tagalog
    else if (language_tag == "tl")
        url = {"", 0};
    else if (language_tag == getLangTagByLangName("Thai"))
        url = {lang_url + "tha.traineddata", 1020000 };
    else if (language_tag == getLangTagByLangName("Turkish"))
        url = { lang_url + "tur.traineddata", 17900000 };
    else if (language_tag == getLangTagByLangName("Ukrainian"))
        url = { lang_url + "ukr.traineddata", 11800000 };
    else if (language_tag == getLangTagByLangName("Vietnamese"))
        url = {lang_url + "vie.traineddata", 7400000 };

    return url;
}
