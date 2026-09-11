#pragma once

namespace Silent::Assets
{
    // =======
    // System
    // =======

    constexpr char KEY_SYS_FULLSCREEN_HINT_GENERIC[] = "Sys_FullscreenHintGeneric";
    constexpr char KEY_SYS_FULLSCREEN_HINT_MAC[]     = "Sys_FullscreenHintMac";
    constexpr char KEY_SYS_GAMEPAD_CONNECTED[]       = "Sys_GamepadConnected";
    constexpr char KEY_SYS_GAMEPAD_DISCONNECTED[]    = "Sys_GamepadDisconnected";
    constexpr char KEY_SYS_GAMEPAD_BATTERY_IS_LOW[]  = "Sys_GamepadBatteryIsLow";

    // ======
    // Intro
    // ======

    constexpr char KEY_INTRO_WARNING[]       = "Intro_Warning";
    constexpr char KEY_INTRO_FEAR_OF_BLOOD[] = "Intro_FearOfBlood";

    // ======
    // Menus
    // ======

    constexpr char KEY_MAIN_MENU_OPENING_QUOTE[] = "MainMenu_OpeningQuote";
    constexpr char KEY_MAIN_MENU_CLOSING_QUOTE[] = "MainMenu_ClosingQuote";
    constexpr char KEY_MAIN_MENU_START[]         = "MainMenu_Start";
    constexpr char KEY_MAIN_MENU_CONTINUE[]      = "MainMenu_Continue";
    constexpr char KEY_MAIN_MENU_LOAD[]          = "MainMenu_Load";
    constexpr char KEY_MAIN_MENU_OPTION[]        = "MainMenu_Option";
    constexpr char KEY_MAIN_MENU_QUIT[]          = "MainMenu_Quit";
    constexpr char KEY_MAIN_MENU_EASY[]          = "MainMenu_Easy";
    constexpr char KEY_MAIN_MENU_NORMAL[]        = "MainMenu_Normal";
    constexpr char KEY_MAIN_MENU_HARD[]          = "MainMenu_Hard";

    constexpr char KEY_OPTIONS_MENU_HEADING[]       = "OptionsMenu_Heading";
    constexpr char KEY_OPTIONS_MENU_EXTRA_OPTIONS[] = "OptionsMenu_ExtraOptions";
    constexpr char KEY_OPTIONS_MENU_GRAPHICS[]      = "OptionsMenu_Graphics";
    constexpr char KEY_OPTIONS_MENU_GAMEPLAY[]      = "OptionsMenu_Gameplay";
    constexpr char KEY_OPTIONS_MENU_INPUT[]         = "OptionsMenu_Input";
    constexpr char KEY_OPTIONS_MENU_ENHANCEMENTS[]  = "OptionsMenu_Enhancements";
    constexpr char KEY_OPTIONS_MENU_SYSTEM[]        = "OptionsMenu_System";

    constexpr char KEY_OPTIONS_MENU_EXIT[]         = "OptionsMenu_Exit";
    constexpr char KEY_OPTIONS_MENU_BRIGHT_LEVEL[] = "OptionsMenu_BrightLevel";
    constexpr char KEY_OPTIONS_MENU_CONT_CONFIG[]  = "OptionsMenu_ContConfig";
    constexpr char KEY_OPTIONS_MENU_VIBRATION[]    = "OptionsMenu_Vibration";
    constexpr char KEY_OPTIONS_MENU_AUTO_LOAD[]    = "OptionsMenu_AutoLoad";
    constexpr char KEY_OPTIONS_MENU_SOUND[]        = "OptionsMenu_Sound";
    constexpr char KEY_OPTIONS_MENU_BGM_VOL[]      = "OptionsMenu_BgmVol";
    constexpr char KEY_OPTIONS_MENU_SE_VOL[]       = "OptionsMenu_SeVol";
    constexpr char KEY_OPTIONS_MENU_LANGUAGE[]     = "OptionsMenu_Language";
    
    constexpr char KEY_OPTIONS_MENU_WEAPON_CONTROL[]   = "OptionsMenu_WeaponControl";
    constexpr char KEY_OPTIONS_MENU_BLOOD_COLOR[]      = "OptionsMenu_BloodColor";
    constexpr char KEY_OPTIONS_MENU_VIEW_CONTROL[]     = "OptionsMenu_ViewControl";
    constexpr char KEY_OPTIONS_MENU_RETREAT_TURN[]     = "OptionsMenu_RetreatTurn";
    constexpr char KEY_OPTIONS_MENU_WALK_RUN_CONTROL[] = "OptionsMenu_WalkRunControl";
    constexpr char KEY_OPTIONS_MENU_CONTROL[]          = "OptionsMenu_Control";
    constexpr char KEY_OPTIONS_MENU_AUTO_AIMING[]      = "OptionsMenu_AutoAiming";
    constexpr char KEY_OPTIONS_MENU_VIEW_MODE[]        = "OptionsMenu_ViewMode";
    constexpr char KEY_OPTIONS_MENU_BULLET_ADJUST[]    = "OptionsMenu_BulletAdjust";

    constexpr char KEY_OPTIONS_MENU_ON[]        = "OptionsMenu_On";
    constexpr char KEY_OPTIONS_MENU_OFF[]       = "OptionsMenu_Off";
    constexpr char KEY_OPTIONS_MENU_STEREO[]    = "OptionsMenu_Stereo";
    constexpr char KEY_OPTIONS_MENU_MONAURAL[]  = "OptionsMenu_Monaural";
    constexpr char KEY_OPTIONS_MENU_PRESS[]     = "OptionsMenu_Press";
    constexpr char KEY_OPTIONS_MENU_SWITCH[]    = "OptionsMenu_Switch";
    constexpr char KEY_OPTIONS_MENU_NORMAL[]    = "OptionsMenu_Normal";
    constexpr char KEY_OPTIONS_MENU_GREEN[]     = "OptionsMenu_Green";
    constexpr char KEY_OPTIONS_MENU_VIOLET[]    = "OptionsMenu_Violet";
    constexpr char KEY_OPTIONS_MENU_BLACK[]     = "OptionsMenu_Black";
    constexpr char KEY_OPTIONS_MENU_REVERSE[]   = "OptionsMenu_Reverse";
    constexpr char KEY_OPTIONS_MENU_SELF_VIEW[] = "OptionsMenu_SelfView";

    constexpr char KEY_BRIGHT_MENU_PROMPT[] = "BrightMenu_Prompt";
    constexpr char KEY_BRIGHT_MENU_LEVEL[]  = "BrightMenu_Level";

    constexpr char KEY_CONT_MENU_KEYBOARD_MOUSE[]   = "ContMenu_KeyboardMouse";
    constexpr char KEY_CONT_MENU_CONTROLLER[]       = "ContMenu_Controller";
    constexpr char KEY_CONT_MENU_GENERAL_ACTIONS[]  = "ContMenu_GeneralActions";
    constexpr char KEY_CONT_MENU_MENU_ACTIONS[]     = "ContMenu_MenuActions";
    constexpr char KEY_CONT_MENU_GAMEPLAY_ACTIONS[] = "ContMenu_GameplayActions";
    constexpr char KEY_CONT_MENU_EXIT[]             = "ContMenu_Exit";
    constexpr char KEY_CONT_MENU_TYPE_1[]           = "ContMenu_Type1";
    constexpr char KEY_CONT_MENU_TYPE_2[]           = "ContMenu_Type2";
    constexpr char KEY_CONT_MENU_TYPE_3[]           = "ContMenu_Type3";
    constexpr char KEY_CONT_MENU_ENTER[]            = "ContMenu_Enter";
    constexpr char KEY_CONT_MENU_CANCEL[]           = "ContMenu_Cancel";
    constexpr char KEY_CONT_MENU_ACTION[]           = "ContMenu_Action";
    constexpr char KEY_CONT_MENU_AIM[]              = "ContMenu_Aim";
    constexpr char KEY_CONT_MENU_LIGHT[]            = "ContMenu_Light";
    constexpr char KEY_CONT_MENU_RUN[]              = "ContMenu_Run";
    constexpr char KEY_CONT_MENU_VIEW[]             = "ContMenu_View";
    constexpr char KEY_CONT_MENU_STEP_L[]           = "ContMenu_StepL";
    constexpr char KEY_CONT_MENU_STEP_R[]           = "ContMenu_StepR";
    constexpr char KEY_CONT_MENU_PAUSE[]            = "ContMenu_Pause";
    constexpr char KEY_CONT_MENU_ITEM[]             = "ContMenu_Item";
    constexpr char KEY_CONT_MENU_MAP[]              = "ContMenu_Map";
    constexpr char KEY_CONT_MENU_OPTION[]           = "ContMenu_Option";
    constexpr char KEY_CONT_MENU_RESET[]            = "ContMenu_Reset";
    constexpr char KEY_CONT_MENU_WAITING[]          = "ContMenu_Waiting";

    constexpr char KEY_INV_MENU_EQUIPMENT[]  = "InvMenu_Equipment";
    constexpr char KEY_INV_MENU_EXIT[]       = "InvMenu_Exit";
    constexpr char KEY_INV_MENU_OPTION[]     = "InvMenu_Option";
    constexpr char KEY_INV_MENU_MAP[]        = "InvMenu_Map";
    constexpr char KEY_INV_MENU_COMMAND[]    = "InvMenu_Command";
    constexpr char KEY_INV_MENU_STATUS[]     = "InvMenu_Status";
    constexpr char KEY_INV_MENU_USE[]        = "InvMenu_Use";
    constexpr char KEY_INV_MENU_EQUIP[]      = "InvMenu_Equip";
    constexpr char KEY_INV_MENU_UNEQUIP[]    = "InvMenu_Unequip";
    constexpr char KEY_INV_MENU_RELOAD[]     = "InvMenu_Reload";
    constexpr char KEY_INV_MENU_DETAIL[]     = "InvMenu_Detail";
    constexpr char KEY_INV_MENU_ON[]         = "InvMenu_On";
    constexpr char KEY_INV_MENU_OFF[]        = "InvMenu_Off";
    constexpr char KEY_INV_MENU_LOOK[]       = "InvMenu_Look";
    constexpr char KEY_INV_MENU_USE_OK[]     = "InvMenu_UseOk";
    constexpr char KEY_INV_MENU_USE_OK_ASK[] = "InvMenu_UseOkAsk";
    constexpr char KEY_INV_MENU_USE_NG[]     = "InvMenu_UseNg";
    constexpr char KEY_INV_MENU_NUMBER[]     = "InvMenu_Number";
    constexpr char KEY_INV_MENU_NAME[]       = "InvMenu_Name";
    constexpr char KEY_INV_MENU_STOCK[]      = "InvMenu_Stock";
    constexpr char KEY_INV_MENU_FUEL[]       = "InvMenu_Fuel";

    constexpr char KEY_SAVE_LOAD_MENU_FILE[]               = "SaveLoadMenu_File";
    constexpr char KEY_SAVE_LOAD_MENU_OUT_OF_BLOCKS[]      = "SaveLoadMenu_OutOfBlocks";
    constexpr char KEY_SAVE_LOAD_MENU_NO_DATA_FILE[]       = "SaveLoadMenu_NoDataFile";
    constexpr char KEY_SAVE_LOAD_MENU_FILE_IS_DAMAGED[]    = "SaveLoadMenu_FileIsDamaged";
    constexpr char KEY_SAVE_LOAD_MENU_NEW_SAVE[]           = "SaveLoadMenu_NewSave";
    constexpr char KEY_SAVE_LOAD_MENU_CREATE_NEW_FILE[]    = "SaveLoadMenu_CreateNewFile";
    constexpr char KEY_SAVE_LOAD_MENU_NOW_SAVING[]         = "SaveLoadMenu_NowSaving";
    constexpr char KEY_SAVE_LOAD_MENU_UNABLE_TO_CREATE[]   = "SaveLoadMenu_UnableToCreate";
    constexpr char KEY_SAVE_LOAD_MENU_FINISHED_SAVING[]    = "SaveLoadMenu_FinishedSaving";
    constexpr char KEY_SAVE_LOAD_MENU_FAILED_TO_SAVE[]     = "SaveLoadMenu_FailedToSave";
    constexpr char KEY_SAVE_LOAD_MENU_DATA_IS_NOT_FOUND[]  = "SaveLoadMenu_DataIsNotFound";
    constexpr char KEY_SAVE_LOAD_MENU_DATA_IS_DAMAGED[]    = "SaveLoadMenu_DataIsDamaged";
    constexpr char KEY_SAVE_LOAD_MENU_FAILED_TO_LOAD[]     = "SaveLoadMenu_FailedToLoad";
    constexpr char KEY_SAVE_LOAD_MENU_FINISHED_LOADING[]   = "SaveLoadMenu_FinishedLoading";
    constexpr char KEY_SAVE_LOAD_MENU_NOW_LOADING[]        = "SaveLoadMenu_NowLoading";
    constexpr char KEY_SAVE_LOAD_MENU_OK_TO_OVERWRITE[]    = "SaveLoadMenu_OkToOverwrite";
    constexpr char KEY_SAVE_LOAD_MENU_DATA[]               = "SaveLoadMenu_Data";
    constexpr char KEY_SAVE_LOAD_MENU_SAVE[]               = "SaveLoadMenu_Save";
    constexpr char KEY_SAVE_LOAD_MENU_HYPER_BLASTER[]      = "SaveLoadMenu_HyperBlaster";
    constexpr char KEY_SAVE_LOAD_MENU_POWER_UP[]           = "SaveLoadMenu_PowerUp";
    constexpr char KEY_SAVE_LOAD_MENU_NEED_1_FREE_BLOCK[]  = "SaveLoadMenu_Need1FreeBlock";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_ANYWHERE[]       = "SaveLoadMenu_LocAnywhere";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_CAFE[]           = "SaveLoadMenu_LocCafe";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_BUS[]            = "SaveLoadMenu_LocBus";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_STORE[]          = "SaveLoadMenu_LocStore";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_INFIRMARY[]      = "SaveLoadMenu_LocInfirmary";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_DOGHOUSE[]       = "SaveLoadMenu_LocDoghouse";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_GORDON[]         = "SaveLoadMenu_LocGordon";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_CHURCH[]         = "SaveLoadMenu_LocChurch";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_GARAGE[]         = "SaveLoadMenu_LocGarage";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_POLICE[]         = "SaveLoadMenu_LocPolice";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_RECEPTION[]      = "SaveLoadMenu_LocReception";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_ROOM_302[]       = "SaveLoadMenu_LocRoom302";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_DIRECTORS[]      = "SaveLoadMenu_LocDirectors";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_JEWELLERY_SHOP[] = "SaveLoadMenu_LocJewelleryShop";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_POOL_HALL[]      = "SaveLoadMenu_LocPoolHall";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_ANTIQUE_SHOP[]   = "SaveLoadMenu_LocAntiqueShop";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_THEME_PARK[]     = "SaveLoadMenu_LocThemePark";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_BOAT[]           = "SaveLoadMenu_LocBoat";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_BRIDGE[]         = "SaveLoadMenu_LocBridge";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_MOTEL[]          = "SaveLoadMenu_LocMotel";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_LIGHTHOUSE[]     = "SaveLoadMenu_LocLighthouse";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_SEWER[]          = "SaveLoadMenu_LocSewer";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_NOWHERE[]        = "SaveLoadMenu_LocNowhere";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_CHILDS_ROOM[]    = "SaveLoadMenu_LocChildsRoom";
    constexpr char KEY_SAVE_LOAD_MENU_LOC_NEXT_FEAR[]      = "SaveLoadMenu_LocNextFear";

    constexpr char KEY_PAUSE_MENU_HEADING[] = "PauseMenu_Heading";

    // =================
    // Paper Map Labels
    // =================

    constexpr char KEY_PAPER_MAP_OSH_SILENT_HILL[]          = "PaperMap_OshSilentHill";
    constexpr char KEY_PAPER_MAP_OSH_TO_CENTRAL[]           = "PaperMap_OshToCentral";
    constexpr char KEY_PAPER_MAP_OSH_BACHMAN_RD[]           = "PaperMap_OshBachmanRd";
    constexpr char KEY_PAPER_MAP_OSH_BLOCH_ST[]             = "PaperMap_OshBlochSt";
    constexpr char KEY_PAPER_MAP_OSH_BRADBURY_ST[]          = "PaperMap_OshBradburySt";
    constexpr char KEY_PAPER_MAP_OSH_ELLROY_ST[]            = "PaperMap_OshEllroySt";
    constexpr char KEY_PAPER_MAP_OSH_FINNEY_ST[]            = "PaperMap_OshFinneySt";
    constexpr char KEY_PAPER_MAP_OSH_LEVIN_ST[]             = "PaperMap_OshLevinSt";
    constexpr char KEY_PAPER_MAP_OSH_MATHESON_ST[]          = "PaperMap_OshMathesonSt";
    constexpr char KEY_PAPER_MAP_OSH_MIDWICH_ST[]           = "PaperMap_OshMidwichSt";
    constexpr char KEY_PAPER_MAP_OSH_ALGERNON[]             = "PaperMap_OshAlgernon";
    constexpr char KEY_PAPER_MAP_OSH_BALKAN_CHURCH[]        = "PaperMap_OshBalkanChurch";
    constexpr char KEY_PAPER_MAP_OSH_BLUE_BELLE[]           = "PaperMap_OshBlueBelle";
    constexpr char KEY_PAPER_MAP_OSH_BOOK_STORE[]           = "PaperMap_OshBookStore";
    constexpr char KEY_PAPER_MAP_OSH_BRIDGE_CONTROL[]       = "PaperMap_OshBridgeControl";
    constexpr char KEY_PAPER_MAP_OSH_CAFE_5TO2[]            = "PaperMap_OshCafe5to2";
    constexpr char KEY_PAPER_MAP_OSH_CHAINSAWS[]            = "PaperMap_OshChainsaws";
    constexpr char KEY_PAPER_MAP_OSH_CONV_STORE_8[]         = "PaperMap_OshConvStore8";
    constexpr char KEY_PAPER_MAP_OSH_CRABS[]                = "PaperMap_OshCrabs";
    constexpr char KEY_PAPER_MAP_OSH_FISH_N_FRIES[]         = "PaperMap_OshFishNFries";
    constexpr char KEY_PAPER_MAP_OSH_FOOD_AND_LIQUOR[]      = "PaperMap_OshFoodAndLiquor";
    constexpr char KEY_PAPER_MAP_OSH_FRESH_ROASTED_COFFEE[] = "PaperMap_OshFreshRoastedCoffee";
    constexpr char KEY_PAPER_MAP_OSH_GAS_STATION[]          = "PaperMap_OshGasStation";
    constexpr char KEY_PAPER_MAP_OSH_GHOUL[]                = "PaperMap_OshGhoul";
    constexpr char KEY_PAPER_MAP_OSH_HIGH_LIFE[]            = "PaperMap_OshHighLife";
    constexpr char KEY_PAPER_MAP_OSH_HORNED_MOON_DANCE[]    = "PaperMap_OshHornedMoonDance";
    constexpr char KEY_PAPER_MAP_OSH_ICE_CREAM_BAR[]        = "PaperMap_OshIceCreamBar";
    constexpr char KEY_PAPER_MAP_OSH_ICHIRONS_DRUGS[]       = "PaperMap_OshIchironsDrugs";
    constexpr char KEY_PAPER_MAP_OSH_JIM_BEAM[]             = "PaperMap_OshJimBeam";
    constexpr char KEY_PAPER_MAP_OSH_JULIOS_AUTO_PARTS[]    = "PaperMap_OshJuliosAutoParts";
    constexpr char KEY_PAPER_MAP_OSH_LIGHTING_CO[]          = "PaperMap_OshLightingCo";
    constexpr char KEY_PAPER_MAP_OSH_MARKET[]               = "PaperMap_OshMarket";
    constexpr char KEY_PAPER_MAP_OSH_METROPOL[]             = "PaperMap_OshMetropol";
    constexpr char KEY_PAPER_MAP_OSH_PHARMACY[]             = "PaperMap_OshPharmacy";
    constexpr char KEY_PAPER_MAP_OSH_POSTON_MARKET[]        = "PaperMap_OshPostonMarket";
    constexpr char KEY_PAPER_MAP_OSH_QUEEN_BURGER[]         = "PaperMap_OshQueenBurger";
    constexpr char KEY_PAPER_MAP_OSH_ROGERS_PASTRY[]        = "PaperMap_OshRogersPastry";
    constexpr char KEY_PAPER_MAP_OSH_SCHOOL[]               = "PaperMap_OshSchool";
    constexpr char KEY_PAPER_MAP_OSH_SHOE_REPAIR[]          = "PaperMap_OshShoeRepair";
    constexpr char KEY_PAPER_MAP_OSH_SOUVENIRS[]            = "PaperMap_OshSouvenirs";
    constexpr char KEY_PAPER_MAP_OSH_SUNDRIES_SHOP[]        = "PaperMap_OshSundriesShop";
    constexpr char KEY_PAPER_MAP_OSH_TOP_SALES[]            = "PaperMap_OshTopSales";

    constexpr char KEY_PAPER_MAP_CSH_HEADING[]                 = "PaperMap_CshHeading";
    constexpr char KEY_PAPER_MAP_CSH_TO_OLD_SILENT_HILL[]      = "PaperMap_CshToOldSilentHill";
    constexpr char KEY_PAPER_MAP_CSH_CENTRAL_SILENT_HILL[]     = "PaperMap_CshCentralSilentHill";
    constexpr char KEY_PAPER_MAP_CSH_CRICHTON_ST[]             = "PaperMap_CshCrichtonSt";
    constexpr char KEY_PAPER_MAP_CSH_KOONTZ_ST[]               = "PaperMap_CshKoontzSt";
    constexpr char KEY_PAPER_MAP_CSH_SAGAN_ST[]                = "PaperMap_CshSaganSt";
    constexpr char KEY_PAPER_MAP_CSH_SIMMONS_ST[]              = "PaperMap_CshSimmonsSt";
    constexpr char KEY_PAPER_MAP_CSH_WILSON_ST[]               = "PaperMap_CshWilsonSt";
    constexpr char KEY_PAPER_MAP_CSH_ANDYS_BOOK_STORE[]        = "PaperMap_CshAndysBookStore";
    constexpr char KEY_PAPER_MAP_CSH_ANDY_SHOP[]               = "PaperMap_CshAndyShop";
    constexpr char KEY_PAPER_MAP_CSH_ANTIQUE_GREEN_LION[]      = "PaperMap_CshAntiqueGreenLion";
    constexpr char KEY_PAPER_MAP_CSH_ANTIQUES_OLD_TOWN[]       = "PaperMap_CshAntiquesOldTown";
    constexpr char KEY_PAPER_MAP_CSH_BUFFALO_CHIEF[]           = "PaperMap_CshBuffaloChief";
    constexpr char KEY_PAPER_MAP_CSH_CAFE_SUN[]                = "PaperMap_CshCafeSun";
    constexpr char KEY_PAPER_MAP_CSH_CASJS[]                   = "PaperMap_CshCasjs";
    constexpr char KEY_PAPER_MAP_CSH_CHINESE_ART_GOODS[]       = "PaperMap_CshChineseArtGoods";
    constexpr char KEY_PAPER_MAP_CSH_COLOR_PHOTO[]             = "PaperMap_CshColorPhoto";
    constexpr char KEY_PAPER_MAP_CSH_CONTRASTS_CASUAL[]        = "PaperMap_CshContrastsCasual";
    constexpr char KEY_PAPER_MAP_CSH_CSH_MOVEABLE_FEAST[]      = "PaperMap_CshCshMoveableFeast";
    constexpr char KEY_PAPER_MAP_CSH_DISCOUNT_AND_VARIETY[]    = "PaperMap_CshDiscountAndVariety";
    constexpr char KEY_PAPER_MAP_CSH_DISCO_MOONDANCE[]         = "PaperMap_CshDiscoMoondance";
    constexpr char KEY_PAPER_MAP_CSH_FAA[]                     = "PaperMap_CshFaa";
    constexpr char KEY_PAPER_MAP_CSH_FANS[]                    = "PaperMap_CshFans";
    constexpr char KEY_PAPER_MAP_CSH_FASHION_CENTER[]          = "PaperMap_CshFashionCenter";
    constexpr char KEY_PAPER_MAP_CSH_FLOWERS_ON_LEX[]          = "PaperMap_CshFlowersOnLex";
    constexpr char KEY_PAPER_MAP_CSH_GALAXY_IWAI[]             = "PaperMap_CshGalaxyIwai";
    constexpr char KEY_PAPER_MAP_CSH_GRAND_MASTER_BOOK_STORE[] = "PaperMap_CshGrandMasterBookStore";
    constexpr char KEY_PAPER_MAP_CSH_GREEN_PHARMACY[]          = "PaperMap_CshGreenPharmacy";
    constexpr char KEY_PAPER_MAP_CSH_HOBBY_AMERICA[]           = "PaperMap_CshHobbyAmerica";
    constexpr char KEY_PAPER_MAP_CSH_HOSPITAL[]                = "PaperMap_CshHospital";
    constexpr char KEY_PAPER_MAP_CSH_HOT_DOG[]                 = "PaperMap_CshHotDog";
    constexpr char KEY_PAPER_MAP_CSH_KONAMI_BURGER[]           = "PaperMap_CshKonamiBurger";
    constexpr char KEY_PAPER_MAP_CSH_LOGGIA[]                  = "PaperMap_CshLoggia";
    constexpr char KEY_PAPER_MAP_CSH_MARIG_LD[]                = "PaperMap_CshMarigLd";
    constexpr char KEY_PAPER_MAP_CSH_MARKET_A[]                = "PaperMap_CshMarketA";
    constexpr char KEY_PAPER_MAP_CSH_MUSHNICKS_FLORIST[]       = "PaperMap_CshMushnicksFlorist";
    constexpr char KEY_PAPER_MAP_CSH_NOLTEMI_AND_C[]           = "PaperMap_CshNoltemiAndC";
    constexpr char KEY_PAPER_MAP_CSH_POLICE[]                  = "PaperMap_CshPolice";
    constexpr char KEY_PAPER_MAP_CSH_POST_OFFICE[]             = "PaperMap_CshPostOffice";
    constexpr char KEY_PAPER_MAP_CSH_PUBLIC_PARKING[]          = "PaperMap_CshPublicParking";
    constexpr char KEY_PAPER_MAP_CSH_RESTAURANT_DAI_DAI[]      = "PaperMap_CshRestaurantDaiDai";
    constexpr char KEY_PAPER_MAP_CSH_ROMANTIC_JOURNEY[]        = "PaperMap_CshRomanticJourney";
    constexpr char KEY_PAPER_MAP_CSH_SALMON[]                  = "PaperMap_CshSalmon";
    constexpr char KEY_PAPER_MAP_CSH_SKY_TOYS[]                = "PaperMap_CshSkyToys";
    constexpr char KEY_PAPER_MAP_CSH_SON_CORP_386[]            = "PaperMap_CshSonCorp386";
    constexpr char KEY_PAPER_MAP_CSH_STEAM_AND_BREW_BURGER[]   = "PaperMap_CshSteamAndBrewBurger";
    constexpr char KEY_PAPER_MAP_CSH_SUE_NAIL[]                = "PaperMap_CshSueNail";
    constexpr char KEY_PAPER_MAP_CSH_TEA_ROOM[]                = "PaperMap_CshTeaRoom";
    constexpr char KEY_PAPER_MAP_CSH_THE_CAR[]                 = "PaperMap_CshTheCar";
    constexpr char KEY_PAPER_MAP_CSH_THEATER_3[]               = "PaperMap_CshTheater3";
    constexpr char KEY_PAPER_MAP_CSH_THE_MIRAGE[]              = "PaperMap_CshTheMirage";
    constexpr char KEY_PAPER_MAP_CSH_THIRTY_RECORDS[]          = "PaperMap_CshThirtyRecords";
    constexpr char KEY_PAPER_MAP_CSH_TOP_SALES[]               = "PaperMap_CshTopSales";
    constexpr char KEY_PAPER_MAP_CSH_TOWN_CENTER[]             = "PaperMap_CshTownCenter";
    constexpr char KEY_PAPER_MAP_CSH_TRAMPS_BAR[]              = "PaperMap_CshTrampsBar";
    constexpr char KEY_PAPER_MAP_CSH_VIVRE[]                   = "PaperMap_CshVivre";
    constexpr char KEY_PAPER_MAP_CSH_VOWO[]                    = "PaperMap_CshVowo";
    constexpr char KEY_PAPER_MAP_CSH_WILD_AND_COOL[]           = "PaperMap_CshWildAndCool";
    constexpr char KEY_PAPER_MAP_CSH_WIND[]                    = "PaperMap_CshWind";

    constexpr char KEY_PAPER_MAP_RESORT_HEADING[]            = "PaperMap_ResortHeading";
    constexpr char KEY_PAPER_MAP_RESORT_TO_LAKE_SIDE[]       = "PaperMap_ResortToLakeSide";
    constexpr char KEY_PAPER_MAP_RESORT_TO_OLD_SILENT_HILL[] = "PaperMap_ResortToOldSilentHill";
    constexpr char KEY_PAPER_MAP_RESORT_LAKE_SIDE[]          = "PaperMap_ResortLakeSide";
    constexpr char KEY_PAPER_MAP_RESORT_SOUTH_PARK[]         = "PaperMap_ResortSouthPark";
    constexpr char KEY_PAPER_MAP_RESORT_CRAIG_ST[]           = "PaperMap_ResortCraigSt";
    constexpr char KEY_PAPER_MAP_RESORT_BACHMAN_RD[]         = "PaperMap_ResortBachmanRd";
    constexpr char KEY_PAPER_MAP_RESORT_SANDFORD_ST[]        = "PaperMap_ResortSandfordSt";
    constexpr char KEY_PAPER_MAP_RESORT_W_SANDFORD_ST[]      = "PaperMap_ResortWSandfordSt";
    constexpr char KEY_PAPER_MAP_RESORT_WEAVER_ST[]          = "PaperMap_ResortWeaverSt";
    constexpr char KEY_PAPER_MAP_RESORT_AEC_BURGER[]         = "PaperMap_ResortAecBurger";
    constexpr char KEY_PAPER_MAP_RESORT_ANNIES_BAR[]         = "PaperMap_ResortAnniesBar";
    constexpr char KEY_PAPER_MAP_RESORT_EAST_GARAGE[]        = "PaperMap_ResortEastGarage";
    constexpr char KEY_PAPER_MAP_RESORT_HAERBEY_INN[]        = "PaperMap_ResortHaerbeyInn";
    constexpr char KEY_PAPER_MAP_RESORT_INDIAN_RUNNER[]      = "PaperMap_ResortIndianRunner";
    constexpr char KEY_PAPER_MAP_RESORT_LIGHT_HOUSE[]        = "PaperMap_ResortLightHouse";
    constexpr char KEY_PAPER_MAP_RESORT_MOTEL[]              = "PaperMap_ResortMotel";
    constexpr char KEY_PAPER_MAP_RESORT_WEST_GARAGE[]        = "PaperMap_ResortWestGarage";

    constexpr char KEY_PAPER_MAP_SCHOOL_HEADING[]         = "PaperMap_SchoolHeading";
    constexpr char KEY_PAPER_MAP_SCHOOL_FLOOR_BASEMENT[]  = "PaperMap_SchoolFloorBasement";
    constexpr char KEY_PAPER_MAP_SCHOOL_FLOOR_1[]         = "PaperMap_SchoolFloor1";
    constexpr char KEY_PAPER_MAP_SCHOOL_FLOOR_2[]         = "PaperMap_SchoolFloor2";
    constexpr char KEY_PAPER_MAP_SCHOOL_FLOOR_ROOF[]      = "PaperMap_SchoolFloorRoof";
    constexpr char KEY_PAPER_MAP_SCHOOL_DOWN[]            = "PaperMap_SchoolDown";
    constexpr char KEY_PAPER_MAP_SCHOOL_UP[]              = "PaperMap_SchoolUp";
    constexpr char KEY_PAPER_MAP_SCHOOL_BOILER_ROOM[]     = "PaperMap_SchoolBoilerRoom";
    constexpr char KEY_PAPER_MAP_SCHOOL_CHEMISTRY_LAB[]   = "PaperMap_SchoolChemistryLab";
    constexpr char KEY_PAPER_MAP_SCHOOL_CLASS_ROOM[]      = "PaperMap_SchoolClassRoom";
    constexpr char KEY_PAPER_MAP_SCHOOL_CLOCK_TOWER[]     = "PaperMap_SchoolClockTower";
    constexpr char KEY_PAPER_MAP_SCHOOL_COURTYARD[]       = "PaperMap_SchoolCourtyard";
    constexpr char KEY_PAPER_MAP_SCHOOL_ENTRANCE[]        = "PaperMap_SchoolEntrance";
    constexpr char KEY_PAPER_MAP_SCHOOL_HALL[]            = "PaperMap_SchoolHall";
    constexpr char KEY_PAPER_MAP_SCHOOL_INFIRMARY[]       = "PaperMap_SchoolInfirmary";
    constexpr char KEY_PAPER_MAP_SCHOOL_LAB_EQUIP_ROOM[]  = "PaperMap_SchoolLabEquipRoom";
    constexpr char KEY_PAPER_MAP_SCHOOL_LIBRARY[]         = "PaperMap_SchoolLibrary";
    constexpr char KEY_PAPER_MAP_SCHOOL_LIBRARY_RESERVE[] = "PaperMap_SchoolLibraryReserve";
    constexpr char KEY_PAPER_MAP_SCHOOL_LOBBY[]           = "PaperMap_SchoolLobby";
    constexpr char KEY_PAPER_MAP_SCHOOL_LOCKER_ROOM[]     = "PaperMap_SchoolLockerRoom";
    constexpr char KEY_PAPER_MAP_SCHOOL_MUSIC_ROOM[]      = "PaperMap_SchoolMusicRoom";
    constexpr char KEY_PAPER_MAP_SCHOOL_RECEPTION[]       = "PaperMap_SchoolReception";
    constexpr char KEY_PAPER_MAP_SCHOOL_ROOF[]            = "PaperMap_SchoolRoof";
    constexpr char KEY_PAPER_MAP_SCHOOL_STORAGE[]         = "PaperMap_SchoolStorage";
    constexpr char KEY_PAPER_MAP_SCHOOL_TEACHERS_ROOM[]   = "PaperMap_SchoolTeachersRoom";

    constexpr char KEY_PAPER_MAP_HOSPITAL_HEADING[]        = "PaperMap_HospitalHeading";
    constexpr char KEY_PAPER_MAP_HOSPITAL_FLOOR_BASEMENT[] = "PaperMap_HospitalFloorBasement";
    constexpr char KEY_PAPER_MAP_HOSPITAL_FLOOR_1[]        = "PaperMap_HospitalFloor1";
    constexpr char KEY_PAPER_MAP_HOSPITAL_FLOOR_2[]        = "PaperMap_HospitalFloor2";
    constexpr char KEY_PAPER_MAP_HOSPITAL_FLOOR_3[]        = "PaperMap_HospitalFloor3";
    constexpr char KEY_PAPER_MAP_HOSPITAL_DOWN[]           = "PaperMap_HospitalDown";
    constexpr char KEY_PAPER_MAP_HOSPITAL_UP[]             = "PaperMap_HospitalUp";
    constexpr char KEY_PAPER_MAP_HOSPITAL_BOILER_ROOM[]    = "PaperMap_HospitalBoilerRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_CONF_ROOM[]      = "PaperMap_HospitalConfRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_DIRS_OFFICE[]    = "PaperMap_HospitalDirsOffice";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ELEVATOR[]       = "PaperMap_HospitalElevator";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ENTREANCE[]      = "PaperMap_HospitalEntreance";
    constexpr char KEY_PAPER_MAP_HOSPITAL_EXAM_ROOM[]      = "PaperMap_HospitalExamRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_GEN_ROOM[]       = "PaperMap_HospitalGenRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_INTENSIVE_CARE[] = "PaperMap_HospitalIntensiveCare";
    constexpr char KEY_PAPER_MAP_HOSPITAL_KITCHEN[]        = "PaperMap_HospitalKitchen";
    constexpr char KEY_PAPER_MAP_HOSPITAL_LINEN_ROOM[]     = "PaperMap_HospitalLinenRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_MEDICINE_ROOM[]  = "PaperMap_HospitalMedicineRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_MORGUE[]         = "PaperMap_HospitalMorgue";
    constexpr char KEY_PAPER_MAP_HOSPITAL_NURSE_CENTER[]   = "PaperMap_HospitalNurseCenter";
    constexpr char KEY_PAPER_MAP_HOSPITAL_OFFICE[]         = "PaperMap_HospitalOffice";
    constexpr char KEY_PAPER_MAP_HOSPITAL_OP_PREP_ROOM[]   = "PaperMap_HospitalOpPrepRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_OP_ROOM[]        = "PaperMap_HospitalOpRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_RECEPTION[]      = "PaperMap_HospitalReception";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_201[]       = "PaperMap_HospitalRoom201";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_202[]       = "PaperMap_HospitalRoom202";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_203[]       = "PaperMap_HospitalRoom203";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_204[]       = "PaperMap_HospitalRoom204";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_205[]       = "PaperMap_HospitalRoom205";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_206[]       = "PaperMap_HospitalRoom206";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_301[]       = "PaperMap_HospitalRoom301";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_302[]       = "PaperMap_HospitalRoom302";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_303[]       = "PaperMap_HospitalRoom303";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_304[]       = "PaperMap_HospitalRoom304";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_305[]       = "PaperMap_HospitalRoom305";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_306[]       = "PaperMap_HospitalRoom306";
    constexpr char KEY_PAPER_MAP_HOSPITAL_ROOM_307[]       = "PaperMap_HospitalRoom307";
    constexpr char KEY_PAPER_MAP_HOSPITAL_STORAGE_ROOM[]   = "PaperMap_HospitalStorageRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_STORE_ROOM[]     = "PaperMap_HospitalStoreRoom";
    constexpr char KEY_PAPER_MAP_HOSPITAL_WAITING_ROOM[]   = "PaperMap_HospitalWaitingRoom";

    constexpr char KEY_PAPER_MAP_SEWER_LADDER[] = "PaperMap_SewerLadder";

    // ==============
    // Puzzle Labels
    // ==============

    // @todo

    // ======
    // Items
    // ======

    constexpr char KEY_ITEM_HEALTH_DRINK[]           = "Item_HealthDrink";
    constexpr char KEY_ITEM_FIRST_AID_KIT[]          = "Item_FirstAidKit";
    constexpr char KEY_ITEM_AMPOULE[]                = "Item_Ampoule";
    constexpr char KEY_ITEM_LOBBY_KEY[]              = "Item_LobbyKey";
    constexpr char KEY_ITEM_HOUSE_KEY[]              = "Item_HouseKey";
    constexpr char KEY_ITEM_KEY_OF_LION[]            = "Item_KeyOfLion";
    constexpr char KEY_ITEM_KEY_OF_WOODMAN[]         = "Item_KeyOfWoodman";
    constexpr char KEY_ITEM_KEY_OF_SCARECROW[]       = "Item_KeyOfScarecrow";
    constexpr char KEY_ITEM_LIBRARY_RESERVE_KEY[]    = "Item_LibraryReserveKey";
    constexpr char KEY_ITEM_CLASSROOM_KEY[]          = "Item_ClassroomKey";
    constexpr char KEY_ITEM_K_GORDON_KEY[]           = "Item_KGordonKey";
    constexpr char KEY_ITEM_DRAWBRIDGE_KEY[]         = "Item_DrawbridgeKey";
    constexpr char KEY_ITEM_BASEMENT_KEY[]           = "Item_BasementKey";
    constexpr char KEY_ITEM_BASEMENT_STOREROOM_KEY[] = "Item_BasementStoreroomKey";
    constexpr char KEY_ITEM_EXAMINATION_ROOM_KEY[]   = "Item_ExaminationRoomKey";
    constexpr char KEY_ITEM_ANTIQUE_SHOP_KEY[]       = "Item_AntiqueShopKey";
    constexpr char KEY_ITEM_SEWER_KEY[]              = "Item_SewerKey";
    constexpr char KEY_ITEM_KEY_OF_OPHIEL[]          = "Item_KeyOfOphiel";
    constexpr char KEY_ITEM_KEY_OF_HAGITH[]          = "Item_KeyOfHagith";
    constexpr char KEY_ITEM_KEY_OF_PHALEG[]          = "Item_KeyOfPhaleg";
    constexpr char KEY_ITEM_KEY_OF_BETHOR[]          = "Item_KeyOfBethor";
    constexpr char KEY_ITEM_KEY_OF_ARATRON[]         = "Item_KeyOfAratron";
    constexpr char KEY_ITEM_A_NOTE_TO_SCHOOL[]       = "Item_ANoteToSchool";
    constexpr char KEY_ITEM_A_NOTE_DOGHOUSE[]        = "Item_ANoteDoghouse";
    constexpr char KEY_ITEM_PICTURE_CARD[]           = "Item_PictureCard";
    constexpr char KEY_ITEM_SEWER_EXIT_KEY[]         = "Item_SewerExitKey";
    constexpr char KEY_ITEM_CHANNELING_STONE[]       = "Item_ChannelingStone";
    constexpr char KEY_ITEM_CHEMICAL[]               = "Item_Chemical";
    constexpr char KEY_ITEM_GOLD_MEDALLION[]         = "Item_GoldMedallion";
    constexpr char KEY_ITEM_SILVER_MEDALLION[]       = "Item_SilverMedallion";
    constexpr char KEY_ITEM_RUBBER_BALL[]            = "Item_RubberBall";
    constexpr char KEY_ITEM_FLAUROS[]                = "Item_Flauros";
    constexpr char KEY_ITEM_PLASTIC_BOTTLE[]         = "Item_PlasticBottle";
    constexpr char KEY_ITEM_UNKNOWN_LIQUID[]         = "Item_UnknownLiquid";
    constexpr char KEY_ITEM_PLATE_OF_TURTLE[]        = "Item_PlateOfTurtle";
    constexpr char KEY_ITEM_PLATE_OF_HATTER[]        = "Item_PlateOfHatter";
    constexpr char KEY_ITEM_PLATE_OF_CAT[]           = "Item_PlateOfCat";
    constexpr char KEY_ITEM_PLATE_OF_QUEEN[]         = "Item_PlateOfQueen";
    constexpr char KEY_ITEM_BLOOD_PACK[]             = "Item_BloodPack";
    constexpr char KEY_ITEM_DISINFECTING_ALCOHOL[]   = "Item_DisinfectingAlcohol";
    constexpr char KEY_ITEM_LIGHTER[]                = "Item_Lighter";
    constexpr char KEY_ITEM_VIDEO_TAPE[]             = "Item_Video tape";
    constexpr char KEY_ITEM_KAUFMANN_KEY[]           = "Item_KaufmannKey";
    constexpr char KEY_ITEM_RECEIPT[]                = "Item_Receipt";
    constexpr char KEY_ITEM_SAFE_KEY[]               = "Item_SafeKey";
    constexpr char KEY_ITEM_MAGNET[]                 = "Item_Magnet";
    constexpr char KEY_ITEM_MOTORCYCLE_KEY[]         = "Item_MotorcycleKey";
    constexpr char KEY_ITEM_BIRD_CAGE_KEY[]          = "Item_BirdCageKey";
    constexpr char KEY_ITEM_PLIERS[]                 = "Item_Pliers";
    constexpr char KEY_ITEM_SCREWDRIVER[]            = "Item_Screwdriver";
    constexpr char KEY_ITEM_CAMERA[]                 = "Item_Camera";
    constexpr char KEY_ITEM_RING_OF_CONTRACT[]       = "Item_RingOfContract";
    constexpr char KEY_ITEM_STONE_OF_TIME[]          = "Item_StoneOfTime";
    constexpr char KEY_ITEM_AMULET_OF_SOLOMON[]      = "Item_AmuletOfSolomon";
    constexpr char KEY_ITEM_CREST_OF_MERCURY[]       = "Item_CrestOfMercury";
    constexpr char KEY_ITEM_ANKH[]                   = "Item_Ankh";
    constexpr char KEY_ITEM_DAGGER_OF_MELCHIOR[]     = "Item_DaggerOdMelchior";
    constexpr char KEY_ITEM_DISK_OF_OUROBOROS[]      = "Item_DiskOfOuroboros";
    constexpr char KEY_ITEM_KITCHEN_KNIFE[]          = "Item_KitchenKnife";
    constexpr char KEY_ITEM_STEEL_PIPE[]             = "Item_SteelPipe";
    constexpr char KEY_ITEM_ROCK_DRILL[]             = "Item_RockDrill";
    constexpr char KEY_ITEM_HAMMER[]                 = "Item_Hammer";
    constexpr char KEY_ITEM_CHAINSAW[]               = "Item_Chainsaw";
    constexpr char KEY_ITEM_KATANA[]                 = "Item_Katana";
    constexpr char KEY_ITEM_AXE[]                    = "Item_Axe";
    constexpr char KEY_ITEM_HANDGUN[]                = "Item_Handgun";
    constexpr char KEY_ITEM_HUNTING_RIFLE[]          = "Item_HuntingRifle";
    constexpr char KEY_ITEM_SHOTGUN[]                = "Item_Shotgun";
    constexpr char KEY_ITEM_HYPER_BLASTER[]          = "Item_HyperBlaster";
    constexpr char KEY_ITEM_HANDGUN_BULLETS[]        = "Item_HandgunBullets";
    constexpr char KEY_ITEM_RIFLE_SHELLS[]           = "Item_RifleShells";
    constexpr char KEY_ITEM_SHOTGUN_SHELLS[]         = "Item_ShotgunShells";
    constexpr char KEY_ITEM_FLASHLIGHT[]             = "Item_Flashlight";
    constexpr char KEY_ITEM_POCKET_RADIO[]           = "Item_PocketRadio";
    constexpr char KEY_ITEM_GASOLINE_TANK[]          = "Item_GasolineTank";

    // ==================
    // Item Descriptions
    // ==================

    constexpr char KEY_ITEM_DESC_HEALTH_DRINK[]        = "ItemDesc_HealthDrink";
    constexpr char KEY_ITEM_DESC_FIRST_AID_KIT[]       = "ItemDesc_FirstAidKit";
    constexpr char KEY_ITEM_DESC_AMPULE[]              = "ItemDesc_Ampule";
    constexpr char KEY_ITEM_DESC_LOBBY_KEY[]           = "ItemDesc_LobbyKey";
    constexpr char KEY_ITEM_DESC_HOUSE_KEY[]           = "ItemDesc_HouseKey";
    constexpr char KEY_ITEM_DESC_KEY_OF_LION[]         = "ItemDesc_KeyOfLion";
    constexpr char KEY_ITEM_DESC_KEY_OF_WOODMAN[]      = "ItemDesc_KeyOfWoodman";
    constexpr char KEY_ITEM_DESC_KEY_OF_SCARECROW[]    = "ItemDesc_KeyOfScarecrow";
    constexpr char KEY_ITEM_DESC_LIBRARY_RESERVE_KEY[] = "ItemDesc_LibraryReserveKey";
    constexpr char KEY_ITEM_DESC_CLASSROOM_KEY[]       = "ItemDesc_ClassroomKey";
    constexpr char KEY_ITEM_DESC_K_GORDON_KEY[]        = "ItemDesc_KGordonKey";
    constexpr char KEY_ITEM_DESC_DRAWBRIDGE_KEY[]      = "ItemDesc_DrawbridgeKey";
    constexpr char KEY_ITEM_DESC_BASEMENT_KEY[]        = "ItemDesc_BasementKey";
    constexpr char KEY_ITEM_DESC_BASEMENT_STOREROOM[]  = "ItemDesc_BasementStoreroomKey";
    constexpr char KEY_ITEM_DESC_EXAMINATION_ROOM[]    = "ItemDesc_ExaminationRoomKey";
    constexpr char KEY_ITEM_DESC_ANTIQUE_SHOP_KEY[]    = "ItemDesc_AntiqueShopKey";
    constexpr char KEY_ITEM_DESC_SEWER_KEY[]           = "ItemDesc_SewerKey";
    constexpr char KEY_ITEM_DESC_KEY_OF_OPHIEL[]       = "ItemDesc_KeyOfOphiel";
    constexpr char KEY_ITEM_DESC_KEY_OF_HAGITH[]       = "ItemDesc_KeyOfHagith";
    constexpr char KEY_ITEM_DESC_KEY_OF_PHALEG[]       = "ItemDesc_KeyOfPhaleg";
    constexpr char KEY_ITEM_DESC_KEY_OF_BETHOR[]       = "ItemDesc_KeyOfBethor";
    constexpr char KEY_ITEM_DESC_KEY_OF_ARATRON[]      = "ItemDesc_KeyOfAratron";
    constexpr char KEY_ITEM_DESC_A_NOTE_TO_SCHOOL[]    = "ItemDesc_ANoteToSchool";
    constexpr char KEY_ITEM_DESC_A_NOTE_DOGHOUSE[]     = "ItemDesc_ANoteDoghouse";
    constexpr char KEY_ITEM_DESC_PICTURE_CARD[]        = "ItemDesc_PictureCard";
    constexpr char KEY_ITEM_DESC_SEWER_EXIT_KEY[]      = "ItemDesc_SewerExitKey";
    constexpr char KEY_ITEM_DESC_CHANNELING_STONE[]    = "ItemDesc_ChannelingStone";
    constexpr char KEY_ITEM_DESC_CHEMICAL[]            = "ItemDesc_Chemical";
    constexpr char KEY_ITEM_DESC_GOLD_MEDALLION[]      = "ItemDesc_GoldMedallion";
    constexpr char KEY_ITEM_DESC_SILVER_MEDALLION[]    = "ItemDesc_SilverMedallion";
    constexpr char KEY_ITEM_DESC_RUBBER_BALL[]         = "ItemDesc_RubberBall";
    constexpr char KEY_ITEM_DESC_FLAUROS[]             = "ItemDesc_Flauros";
    constexpr char KEY_ITEM_DESC_PLASTIC_BOTTLE[]      = "ItemDesc_PlasticBottle";
    constexpr char KEY_ITEM_DESC_UNKNOWN_LIQUID[]      = "ItemDesc_UnknownLiquid";
    constexpr char KEY_ITEM_DESC_PLATE_OF_TURTLE[]     = "ItemDesc_PlateOfTurtle";
    constexpr char KEY_ITEM_DESC_PLATE_OF_HATTER[]     = "ItemDesc_PlateOfHatter";
    constexpr char KEY_ITEM_DESC_PLATE_OF_CAT[]        = "ItemDesc_PlateOfCat";
    constexpr char KEY_ITEM_DESC_PLATE_OF_QUEEN[]      = "ItemDesc_PlateOfQueen";
    constexpr char KEY_ITEM_DESC_BLOOD_PACK[]          = "ItemDesc_BloodPack";
    constexpr char KEY_ITEM_DESC_DISINFECT_ALCOHOL[]   = "ItemDesc_DisinfectingAlcohol";
    constexpr char KEY_ITEM_DESC_LIGHTER[]             = "ItemDesc_Lighter";
    constexpr char KEY_ITEM_DESC_VIDEO_TAPE[]          = "ItemDesc_VideoTape";
    constexpr char KEY_ITEM_DESC_KAUFMANN_KEY[]        = "ItemDesc_KaufmannKey";
    constexpr char KEY_ITEM_DESC_RECEIPT[]             = "ItemDesc_Receipt";
    constexpr char KEY_ITEM_DESC_SAFE_KEY[]            = "ItemDesc_SafeKey";
    constexpr char KEY_ITEM_DESC_MAGNET[]              = "ItemDesc_Magnet";
    constexpr char KEY_ITEM_DESC_MOTORCYCLE_KEY[]      = "ItemDesc_MotorcycleKey";
    constexpr char KEY_ITEM_DESC_BIRD_CAGE_KEY[]       = "ItemDesc_BirdCageKey";
    constexpr char KEY_ITEM_DESC_PLIERS[]              = "ItemDesc_Pliers";
    constexpr char KEY_ITEM_DESC_SCREWDRIVER[]         = "ItemDesc_Screwdriver";
    constexpr char KEY_ITEM_DESC_CAMERA[]              = "ItemDesc_Camera";
    constexpr char KEY_ITEM_DESC_RING_OF_CONTRACT[]    = "ItemDesc_RingOfContract";
    constexpr char KEY_ITEM_DESC_STONE_OF_TIME[]       = "ItemDesc_StoneOfTime";
    constexpr char KEY_ITEM_DESC_AMULET_OF_SOLOMON[]   = "ItemDesc_AmuletOfSolomon";
    constexpr char KEY_ITEM_DESC_CREST_OF_MERCURY[]    = "ItemDesc_CrestOfMercury";
    constexpr char KEY_ITEM_DESC_ANKH[]                = "ItemDesc_Ankh";
    constexpr char KEY_ITEM_DESC_DAGGER_OF_MELCHIOR[]  = "ItemDesc_DaggerOfMelchior";
    constexpr char KEY_ITEM_DESC_DISK_OF_OUROBOROS[]   = "ItemDesc_DiskOfOuroboros";
    constexpr char KEY_ITEM_DESC_KITCHEN_KNIFE[]       = "ItemDesc_KitchenKnife";
    constexpr char KEY_ITEM_DESC_STEEL_PIPE[]          = "ItemDesc_SteelPipe";
    constexpr char KEY_ITEM_DESC_ROCK_DRILL[]          = "ItemDesc_RockDrill";
    constexpr char KEY_ITEM_DESC_HAMMER[]              = "ItemDesc_Hammer";
    constexpr char KEY_ITEM_DESC_CHAINSAW[]            = "ItemDesc_Chainsaw";
    constexpr char KEY_ITEM_DESC_KATANA[]              = "ItemDesc_Katana";
    constexpr char KEY_ITEM_DESC_AXE[]                 = "ItemDesc_Axe";
    constexpr char KEY_ITEM_DESC_HANDGUN[]             = "ItemDesc_Handgun";
    constexpr char KEY_ITEM_DESC_HUNTING_RIFLE[]       = "ItemDesc_HuntingRifle";
    constexpr char KEY_ITEM_DESC_SHOTGUN[]             = "ItemDesc_Shotgun";
    constexpr char KEY_ITEM_DESC_HYPER_BLASTER[]       = "ItemDesc_HyperBlaster";
    constexpr char KEY_ITEM_DESC_HANDGUN_BULLETS[]     = "ItemDesc_HandgunBullets";
    constexpr char KEY_ITEM_DESC_RIFLE_SHELLS[]        = "ItemDesc_RifleShells";
    constexpr char KEY_ITEM_DESC_SHOTGUN_SHELLS[]      = "ItemDesc_ShotgunShells";
    constexpr char KEY_ITEM_DESC_FLASHLIGHT[]          = "ItemDesc_Flashlight";
    constexpr char KEY_ITEM_DESC_POCKET_RADIO[]        = "ItemDesc_PocketRadio";
    constexpr char KEY_ITEM_DESC_GASOLINE_TANK[]       = "ItemDesc_GasolineTank";

    constexpr char KEY_ITEM_NOTE_CANT_USE[] = "ItemNote_CantUse";
    constexpr char KEY_ITEM_NOTE_TOO_DARK[] = "ItemNote_TooDark";

    // ==========
    // Game Over
    // ==========

    constexpr char KEY_GAME_OVER_HEADING[]      = "GameOver_Heading";
    constexpr char KEY_GAME_OVER_TIPS_HEADING[] = "GameOver_Tips";
    constexpr char KEY_GAME_OVER_TIP_1[]        = "GameOver_Tip1";
    constexpr char KEY_GAME_OVER_TIP_2[]        = "GameOver_Tip2";
    constexpr char KEY_GAME_OVER_TIP_3[]        = "GameOver_Tip3";
    constexpr char KEY_GAME_OVER_TIP_4[]        = "GameOver_Tip4";
    constexpr char KEY_GAME_OVER_TIP_5[]        = "GameOver_Tip5";
    constexpr char KEY_GAME_OVER_TIP_6[]        = "GameOver_Tip6";
    constexpr char KEY_GAME_OVER_TIP_7[]        = "GameOver_Tip7";
    constexpr char KEY_GAME_OVER_TIP_8[]        = "GameOver_Tip8";
    constexpr char KEY_GAME_OVER_TIP_9[]        = "GameOver_Tip9";
    constexpr char KEY_GAME_OVER_TIP_10[]       = "GameOver_Tip10";
    constexpr char KEY_GAME_OVER_TIP_11[]       = "GameOver_Tip11";
    constexpr char KEY_GAME_OVER_TIP_12[]       = "GameOver_Tip12";
    constexpr char KEY_GAME_OVER_TIP_13[]       = "GameOver_Tip13";
    constexpr char KEY_GAME_OVER_TIP_14[]       = "GameOver_Tip14";
    constexpr char KEY_GAME_OVER_TIP_15[]       = "GameOver_Tip15";

    // ============
    // Rank Screen
    // ============

    constexpr char KEY_RANK_GAME_RESULT[]            = "Rank_GameResult";
    constexpr char KEY_RANK_MODE[]                   = "Rank_Mode";
    constexpr char KEY_RANK_EASY[]                   = "Rank_Easy";
    constexpr char KEY_RANK_NORMAL[]                 = "Rank_Normal";
    constexpr char KEY_RANK_HARD[]                   = "Rank_Hard";
    constexpr char KEY_RANK_SAVES[]                  = "Rank_Saves";
    constexpr char KEY_RANK_CONTINUES[]              = "Rank_Continues";
    constexpr char KEY_RANK_TOTAL_TIME[]             = "Rank_TotalTime";
    constexpr char KEY_RANK_HOURS[]                  = "Rank_Hours";
    constexpr char KEY_RANK_MINUTES[]                = "Rank_Minutes";
    constexpr char KEY_RANK_SECONDS[]                = "Rank_Seconds";
    constexpr char KEY_RANK_DECIMAL_POINT[]          = "Rank_DecimalPoint";
    constexpr char KEY_RANK_KM[]                     = "Rank_Km";
    constexpr char KEY_RANK_WALKING_DISTANCE[]       = "Rank_WalkingDistance";
    constexpr char KEY_RANK_RUNINGN_DISTANCE[]       = "Rank_RuningnDistance";
    constexpr char KEY_RANK_ITEMS[]                  = "Rank_Items";
    constexpr char KEY_RANK_SLASH[]                  = "Rank_Slash";
    constexpr char KEY_RANK_PLUS[]                   = "Rank_Plus";
    constexpr char KEY_RANK_GAME_CLEAR[]             = "Rank_GameClear";
    constexpr char KEY_RANK_ENDING[]                 = "Rank_Ending";
    constexpr char KEY_RANK_GOOD_PLUS[]              = "Rank_GoodPlus";
    constexpr char KEY_RANK_GOOD[]                   = "Rank_Good";
    constexpr char KEY_RANK_BAD_PLUS[]               = "Rank_BadPlus";
    constexpr char KEY_RANK_BAD[]                    = "Rank_Bad";
    constexpr char KEY_RANK_UFO[]                    = "Rank_Ufo";
    constexpr char KEY_RANK_YOUR_RANK[]              = "Rank_YourRank";
    constexpr char KEY_RANK_DEFEATED_VIA_SHOOTINGS[] = "Rank_DefeatedViaShootings";
    constexpr char KEY_RANK_DEFEATED_VIA_MELEE[]     = "Rank_DefeatedViaMelee";
    constexpr char KEY_RANK_SHOOTING_STYLE[]         = "Rank_ShootingStyle";
    constexpr char KEY_RANK_SHORT_RANGE_SHOTS[]      = "Rank_ShortRangeShots";
    constexpr char KEY_RANK_MID_RANGE_SHOTS[]        = "Rank_MidRangeShots";
    constexpr char KEY_RANK_LONG_RANGE_SHOTS[]       = "Rank_LongRangeShots";
    constexpr char KEY_RANK_NO_AIMING_SHOTS[]        = "Rank_NoAimingShots";

    // ====================
    // Common Map Messages
    // ====================

    constexpr char KEY_COMMON_MSG_YES[]                  = "CommonMsg_Yes";
    constexpr char KEY_COMMON_MSG_NO[]                   = "CommonMsg_No";
    constexpr char KEY_COMMON_MSG_SOMEDAY[]              = "CommonMsg_Someday";
    constexpr char KEY_COMMON_MSG_NO_MAP[]               = "CommonMsg_NoMap";
    constexpr char KEY_COMMON_MSG_TOO_DARK_FOR_MAP[]     = "CommonMsg_TooDarkForMap";
    constexpr char KEY_COMMON_MSG_TAKE_FIRST_AID_KIT[]   = "CommonMsg_TakeFirstAidKit";
    constexpr char KEY_COMMON_MSG_TAKE_HEALTH_DRINK[]    = "CommonMsg_TakeHealthDrink";
    constexpr char KEY_COMMON_MSG_TAKE_AMPOULE[]         = "CommonMsg_TakeAmpoule";
    constexpr char KEY_COMMON_MSG_TAKE_HANDGUN_BULLETS[] = "CommonMsg_TakeHandgunBullets";
    constexpr char KEY_COMMON_MSG_TAKE_RIFLE_SHELLS[]    = "CommonMsg_TakeRifleShells";
    constexpr char KEY_COMMON_MSG_TAKE_SHOTGUN_SHELLS[]  = "CommonMsg_TakeShotgunShells";
    constexpr char KEY_COMMON_MSG_DOOR_JAMMED[]          = "CommonMsg_DoorJammed";
    constexpr char KEY_COMMON_MSG_DOOR_LOCKED[]          = "CommonMsg_DoorLocked";
    constexpr char KEY_COMMON_MSG_DOOR_UNLOCKED[]        = "CommonMsg_DoorUnlocked";
    constexpr char KEY_COMMON_MSG_NOW_MAKING[]           = "CommonMsg_NowMaking";

    // =========================
    // Map 0, Stage 00 Messages
    // =========================

    constexpr char KEY_M0S00_MSG_1[]  = "M0S00Msg_1";
    constexpr char KEY_M0S00_MSG_2[]  = "M0S00Msg_2";
    constexpr char KEY_M0S00_MSG_3[]  = "M0S00Msg_3";
    constexpr char KEY_M0S00_MSG_4[]  = "M0S00Msg_4";
    constexpr char KEY_M0S00_MSG_5[]  = "M0S00Msg_5";
    constexpr char KEY_M0S00_MSG_6[]  = "M0S00Msg_6";
    constexpr char KEY_M0S00_MSG_7[]  = "M0S00Msg_7";
    constexpr char KEY_M0S00_MSG_8[]  = "M0S00Msg_8";
    constexpr char KEY_M0S00_MSG_9[]  = "M0S00Msg_9";
    constexpr char KEY_M0S00_MSG_10[] = "M0S00Msg_10";
    constexpr char KEY_M0S00_MSG_11[] = "M0S00Msg_11";
    constexpr char KEY_M0S00_MSG_12[] = "M0S00Msg_12";
    constexpr char KEY_M0S00_MSG_13[] = "M0S00Msg_13";
    constexpr char KEY_M0S00_MSG_14[] = "M0S00Msg_14";
    constexpr char KEY_M0S00_MSG_15[] = "M0S00Msg_15";
    constexpr char KEY_M0S00_MSG_16[] = "M0S00Msg_16";
    constexpr char KEY_M0S00_MSG_17[] = "M0S00Msg_17";
    constexpr char KEY_M0S00_MSG_18[] = "M0S00Msg_18";
    constexpr char KEY_M0S00_MSG_19[] = "M0S00Msg_19";
    constexpr char KEY_M0S00_MSG_20[] = "M0S00Msg_20";
    constexpr char KEY_M0S00_MSG_21[] = "M0S00Msg_21";
    constexpr char KEY_M0S00_MSG_22[] = "M0S00Msg_22";
    constexpr char KEY_M0S00_MSG_23[] = "M0S00Msg_23";
    constexpr char KEY_M0S00_MSG_24[] = "M0S00Msg_24";

    // =========================
    // Map 0, Stage 01 Messages
    // =========================

    constexpr char KEY_M0S01_MSG_1[]  = "M0S01Msg_1";
    constexpr char KEY_M0S01_MSG_2[]  = "M0S01Msg_2";
    constexpr char KEY_M0S01_MSG_3[]  = "M0S01Msg_3";
    constexpr char KEY_M0S01_MSG_4[]  = "M0S01Msg_4";
    constexpr char KEY_M0S01_MSG_5[]  = "M0S01Msg_5";
    constexpr char KEY_M0S01_MSG_6[]  = "M0S01Msg_6";
    constexpr char KEY_M0S01_MSG_7[]  = "M0S01Msg_7";
    constexpr char KEY_M0S01_MSG_8[]  = "M0S01Msg_8";
    constexpr char KEY_M0S01_MSG_9[]  = "M0S01Msg_9";
    constexpr char KEY_M0S01_MSG_10[] = "M0S01Msg_10";
    constexpr char KEY_M0S01_MSG_11[] = "M0S01Msg_11";
    constexpr char KEY_M0S01_MSG_12[] = "M0S01Msg_12";
    constexpr char KEY_M0S01_MSG_13[] = "M0S01Msg_13";
    constexpr char KEY_M0S01_MSG_14[] = "M0S01Msg_14";
    constexpr char KEY_M0S01_MSG_15[] = "M0S01Msg_15";
    constexpr char KEY_M0S01_MSG_16[] = "M0S01Msg_16";
    constexpr char KEY_M0S01_MSG_17[] = "M0S01Msg_17";
    constexpr char KEY_M0S01_MSG_18[] = "M0S01Msg_18";
    constexpr char KEY_M0S01_MSG_19[] = "M0S01Msg_19";
    constexpr char KEY_M0S01_MSG_20[] = "M0S01Msg_20";
    constexpr char KEY_M0S01_MSG_21[] = "M0S01Msg_21";
    constexpr char KEY_M0S01_MSG_22[] = "M0S01Msg_22";
    constexpr char KEY_M0S01_MSG_23[] = "M0S01Msg_23";
    constexpr char KEY_M0S01_MSG_24[] = "M0S01Msg_24";
    constexpr char KEY_M0S01_MSG_25[] = "M0S01Msg_25";
    constexpr char KEY_M0S01_MSG_26[] = "M0S01Msg_26";
    constexpr char KEY_M0S01_MSG_27[] = "M0S01Msg_27";
    constexpr char KEY_M0S01_MSG_28[] = "M0S01Msg_28";
    constexpr char KEY_M0S01_MSG_29[] = "M0S01Msg_29";
    constexpr char KEY_M0S01_MSG_30[] = "M0S01Msg_30";
    constexpr char KEY_M0S01_MSG_31[] = "M0S01Msg_31";
    constexpr char KEY_M0S01_MSG_32[] = "M0S01Msg_32";
    constexpr char KEY_M0S01_MSG_33[] = "M0S01Msg_33";
    constexpr char KEY_M0S01_MSG_34[] = "M0S01Msg_34";
    constexpr char KEY_M0S01_MSG_35[] = "M0S01Msg_35";
    constexpr char KEY_M0S01_MSG_36[] = "M0S01Msg_36";
    constexpr char KEY_M0S01_MSG_37[] = "M0S01Msg_37";
    constexpr char KEY_M0S01_MSG_38[] = "M0S01Msg_38";
    constexpr char KEY_M0S01_MSG_39[] = "M0S01Msg_39";
    constexpr char KEY_M0S01_MSG_40[] = "M0S01Msg_40";
    constexpr char KEY_M0S01_MSG_41[] = "M0S01Msg_41";
    constexpr char KEY_M0S01_MSG_42[] = "M0S01Msg_42";
    constexpr char KEY_M0S01_MSG_43[] = "M0S01Msg_43";
    constexpr char KEY_M0S01_MSG_44[] = "M0S01Msg_44";
    constexpr char KEY_M0S01_MSG_45[] = "M0S01Msg_45";
    constexpr char KEY_M0S01_MSG_46[] = "M0S01Msg_46";
    constexpr char KEY_M0S01_MSG_47[] = "M0S01Msg_47";
    constexpr char KEY_M0S01_MSG_48[] = "M0S01Msg_48";
    constexpr char KEY_M0S01_MSG_49[] = "M0S01Msg_49";
    constexpr char KEY_M0S01_MSG_50[] = "M0S01Msg_50";
    constexpr char KEY_M0S01_MSG_51[] = "M0S01Msg_51";
    constexpr char KEY_M0S01_MSG_52[] = "M0S01Msg_52";
    constexpr char KEY_M0S01_MSG_53[] = "M0S01Msg_53";
    constexpr char KEY_M0S01_MSG_54[] = "M0S01Msg_54";
    constexpr char KEY_M0S01_MSG_55[] = "M0S01Msg_55";
    constexpr char KEY_M0S01_MSG_56[] = "M0S01Msg_56";
    constexpr char KEY_M0S01_MSG_57[] = "M0S01Msg_57";
    constexpr char KEY_M0S01_MSG_58[] = "M0S01Msg_58";
    constexpr char KEY_M0S01_MSG_59[] = "M0S01Msg_59";
    constexpr char KEY_M0S01_MSG_60[] = "M0S01Msg_60";
    constexpr char KEY_M0S01_MSG_61[] = "M0S01Msg_61";

    // =========================
    // Map 0, Stage 02 Messages
    // =========================

    constexpr char KEY_M0S02_MSG_1[] = "M0S02Msg_1";
    constexpr char KEY_M0S02_MSG_2[] = "M0S02Msg_2";
    constexpr char KEY_M0S02_MSG_3[] = "M0S02Msg_3";
    constexpr char KEY_M0S02_MSG_4[] = "M0S02Msg_4";
    constexpr char KEY_M0S02_MSG_5[] = "M0S02Msg_5";
    constexpr char KEY_M0S02_MSG_6[] = "M0S02Msg_6";
    constexpr char KEY_M0S02_MSG_7[] = "M0S02Msg_7";

    // =========================
    // Map 1, Stage 00 Messages
    // =========================

    constexpr char KEY_M1S00_MSG_1[]  = "M1S00Msg_1";
    constexpr char KEY_M1S00_MSG_2[]  = "M1S00Msg_2";
    constexpr char KEY_M1S00_MSG_3[]  = "M1S00Msg_3";
    constexpr char KEY_M1S00_MSG_4[]  = "M1S00Msg_4";
    constexpr char KEY_M1S00_MSG_5[]  = "M1S00Msg_5";
    constexpr char KEY_M1S00_MSG_6[]  = "M1S00Msg_6";
    constexpr char KEY_M1S00_MSG_7[]  = "M1S00Msg_7";
    constexpr char KEY_M1S00_MSG_8[]  = "M1S00Msg_8";
    constexpr char KEY_M1S00_MSG_9[]  = "M1S00Msg_9";
    constexpr char KEY_M1S00_MSG_10[] = "M1S00Msg_10";
    constexpr char KEY_M1S00_MSG_11[] = "M1S00Msg_11";
    constexpr char KEY_M1S00_MSG_12[] = "M1S00Msg_12";
    constexpr char KEY_M1S00_MSG_13[] = "M1S00Msg_13";
    constexpr char KEY_M1S00_MSG_14[] = "M1S00Msg_14";
    constexpr char KEY_M1S00_MSG_15[] = "M1S00Msg_15";
    constexpr char KEY_M1S00_MSG_16[] = "M1S00Msg_16";
    constexpr char KEY_M1S00_MSG_17[] = "M1S00Msg_17";
    constexpr char KEY_M1S00_MSG_18[] = "M1S00Msg_18";
    constexpr char KEY_M1S00_MSG_19[] = "M1S00Msg_19";
    constexpr char KEY_M1S00_MSG_20[] = "M1S00Msg_20";
    constexpr char KEY_M1S00_MSG_21[] = "M1S00Msg_21";
    constexpr char KEY_M1S00_MSG_22[] = "M1S00Msg_22";
    constexpr char KEY_M1S00_MSG_23[] = "M1S00Msg_23";
    constexpr char KEY_M1S00_MSG_24[] = "M1S00Msg_24";
    constexpr char KEY_M1S00_MSG_25[] = "M1S00Msg_25";

    // =========================
    // Map 1, Stage 01 Messages
    // =========================

    constexpr char KEY_M1S01_MSG_1[]  = "M1S01Msg_1";
    constexpr char KEY_M1S01_MSG_2[]  = "M1S01Msg_2";
    constexpr char KEY_M1S01_MSG_3[]  = "M1S01Msg_3";
    constexpr char KEY_M1S01_MSG_4[]  = "M1S01Msg_4";
    constexpr char KEY_M1S01_MSG_5[]  = "M1S01Msg_5";
    constexpr char KEY_M1S01_MSG_6[]  = "M1S01Msg_6";
    constexpr char KEY_M1S01_MSG_7[]  = "M1S01Msg_7";
    constexpr char KEY_M1S01_MSG_8[]  = "M1S01Msg_8";
    constexpr char KEY_M1S01_MSG_9[]  = "M1S01Msg_9";
    constexpr char KEY_M1S01_MSG_10[] = "M1S01Msg_10";
    constexpr char KEY_M1S01_MSG_11[] = "M1S01Msg_11";
    constexpr char KEY_M1S01_MSG_12[] = "M1S01Msg_12";
    constexpr char KEY_M1S01_MSG_13[] = "M1S01Msg_13";
    constexpr char KEY_M1S01_MSG_14[] = "M1S01Msg_14";
    constexpr char KEY_M1S01_MSG_15[] = "M1S01Msg_15";
    constexpr char KEY_M1S01_MSG_16[] = "M1S01Msg_16";
    constexpr char KEY_M1S01_MSG_17[] = "M1S01Msg_17";
    constexpr char KEY_M1S01_MSG_18[] = "M1S01Msg_18";
    constexpr char KEY_M1S01_MSG_19[] = "M1S01Msg_19";
    constexpr char KEY_M1S01_MSG_20[] = "M1S01Msg_20";
    constexpr char KEY_M1S01_MSG_21[] = "M1S01Msg_21";
    constexpr char KEY_M1S01_MSG_22[] = "M1S01Msg_22";
    constexpr char KEY_M1S01_MSG_23[] = "M1S01Msg_23";
    constexpr char KEY_M1S01_MSG_24[] = "M1S01Msg_24";
    constexpr char KEY_M1S01_MSG_25[] = "M1S01Msg_25";
    constexpr char KEY_M1S01_MSG_26[] = "M1S01Msg_26";
    constexpr char KEY_M1S01_MSG_27[] = "M1S01Msg_27";
    constexpr char KEY_M1S01_MSG_28[] = "M1S01Msg_28";
    constexpr char KEY_M1S01_MSG_29[] = "M1S01Msg_29";
    constexpr char KEY_M1S01_MSG_30[] = "M1S01Msg_30";
    constexpr char KEY_M1S01_MSG_31[] = "M1S01Msg_31";
    constexpr char KEY_M1S01_MSG_32[] = "M1S01Msg_32";
    constexpr char KEY_M1S01_MSG_33[] = "M1S01Msg_33";

    // =========================
    // Map 1, Stage 02 Messages
    // =========================

    constexpr char KEY_M1S02_MSG_1[]  = "M1S02Msg_1";
    constexpr char KEY_M1S02_MSG_2[]  = "M1S02Msg_2";
    constexpr char KEY_M1S02_MSG_3[]  = "M1S02Msg_3";
    constexpr char KEY_M1S02_MSG_4[]  = "M1S02Msg_4";
    constexpr char KEY_M1S02_MSG_5[]  = "M1S02Msg_5";
    constexpr char KEY_M1S02_MSG_6[]  = "M1S02Msg_6";
    constexpr char KEY_M1S02_MSG_7[]  = "M1S02Msg_7";
    constexpr char KEY_M1S02_MSG_8[]  = "M1S02Msg_8";
    constexpr char KEY_M1S02_MSG_9[]  = "M1S02Msg_9";
    constexpr char KEY_M1S02_MSG_10[] = "M1S02Msg_10";
    constexpr char KEY_M1S02_MSG_11[] = "M1S02Msg_11";
    constexpr char KEY_M1S02_MSG_12[] = "M1S02Msg_12";
    constexpr char KEY_M1S02_MSG_13[] = "M1S02Msg_13";
    constexpr char KEY_M1S02_MSG_14[] = "M1S02Msg_14";
    constexpr char KEY_M1S02_MSG_15[] = "M1S02Msg_15";
    constexpr char KEY_M1S02_MSG_16[] = "M1S02Msg_16";
    constexpr char KEY_M1S02_MSG_17[] = "M1S02Msg_17";
    constexpr char KEY_M1S02_MSG_18[] = "M1S02Msg_18";
    constexpr char KEY_M1S02_MSG_19[] = "M1S02Msg_19";
    constexpr char KEY_M1S02_MSG_20[] = "M1S02Msg_20";
    constexpr char KEY_M1S02_MSG_21[] = "M1S02Msg_21";
    constexpr char KEY_M1S02_MSG_22[] = "M1S02Msg_22";
    constexpr char KEY_M1S02_MSG_23[] = "M1S02Msg_23";
    constexpr char KEY_M1S02_MSG_24[] = "M1S02Msg_24";
    constexpr char KEY_M1S02_MSG_25[] = "M1S02Msg_25";
    constexpr char KEY_M1S02_MSG_26[] = "M1S02Msg_26";
    constexpr char KEY_M1S02_MSG_27[] = "M1S02Msg_27";
    constexpr char KEY_M1S02_MSG_28[] = "M1S02Msg_28";
    constexpr char KEY_M1S02_MSG_29[] = "M1S02Msg_29";
    constexpr char KEY_M1S02_MSG_30[] = "M1S02Msg_30";
    constexpr char KEY_M1S02_MSG_31[] = "M1S02Msg_31";
    constexpr char KEY_M1S02_MSG_32[] = "M1S02Msg_32";
    constexpr char KEY_M1S02_MSG_33[] = "M1S02Msg_33";
    constexpr char KEY_M1S02_MSG_34[] = "M1S02Msg_34";
    constexpr char KEY_M1S02_MSG_35[] = "M1S02Msg_35";
    constexpr char KEY_M1S02_MSG_36[] = "M1S02Msg_36";
    constexpr char KEY_M1S02_MSG_37[] = "M1S02Msg_37";
    constexpr char KEY_M1S02_MSG_38[] = "M1S02Msg_38";
    constexpr char KEY_M1S02_MSG_39[] = "M1S02Msg_39";
    constexpr char KEY_M1S02_MSG_40[] = "M1S02Msg_40";

    // =========================
    // Map 1, Stage 03 Messages
    // =========================

    constexpr char KEY_M1S03_MSG_1[]  = "M1S03Msg_1";
    constexpr char KEY_M1S03_MSG_2[]  = "M1S03Msg_2";
    constexpr char KEY_M1S03_MSG_3[]  = "M1S03Msg_3";
    constexpr char KEY_M1S03_MSG_4[]  = "M1S03Msg_4";
    constexpr char KEY_M1S03_MSG_5[]  = "M1S03Msg_5";
    constexpr char KEY_M1S03_MSG_6[]  = "M1S03Msg_6";
    constexpr char KEY_M1S03_MSG_7[]  = "M1S03Msg_7";
    constexpr char KEY_M1S03_MSG_8[]  = "M1S03Msg_8";
    constexpr char KEY_M1S03_MSG_9[]  = "M1S03Msg_9";
    constexpr char KEY_M1S03_MSG_10[] = "M1S03Msg_10";
    constexpr char KEY_M1S03_MSG_11[] = "M1S03Msg_11";
    constexpr char KEY_M1S03_MSG_12[] = "M1S03Msg_12";
    constexpr char KEY_M1S03_MSG_13[] = "M1S03Msg_13";
    constexpr char KEY_M1S03_MSG_14[] = "M1S03Msg_14";
    constexpr char KEY_M1S03_MSG_15[] = "M1S03Msg_15";
    constexpr char KEY_M1S03_MSG_16[] = "M1S03Msg_16";
    constexpr char KEY_M1S03_MSG_17[] = "M1S03Msg_17";
    constexpr char KEY_M1S03_MSG_18[] = "M1S03Msg_18";
    constexpr char KEY_M1S03_MSG_19[] = "M1S03Msg_19";
    constexpr char KEY_M1S03_MSG_20[] = "M1S03Msg_20";
    constexpr char KEY_M1S03_MSG_21[] = "M1S03Msg_21";
    constexpr char KEY_M1S03_MSG_22[] = "M1S03Msg_22";
    constexpr char KEY_M1S03_MSG_23[] = "M1S03Msg_23";
    constexpr char KEY_M1S03_MSG_24[] = "M1S03Msg_24";
    constexpr char KEY_M1S03_MSG_25[] = "M1S03Msg_25";
    constexpr char KEY_M1S03_MSG_26[] = "M1S03Msg_26";
    constexpr char KEY_M1S03_MSG_27[] = "M1S03Msg_27";
    constexpr char KEY_M1S03_MSG_28[] = "M1S03Msg_28";
    constexpr char KEY_M1S03_MSG_29[] = "M1S03Msg_29";
    constexpr char KEY_M1S03_MSG_30[] = "M1S03Msg_30";
    constexpr char KEY_M1S03_MSG_31[] = "M1S03Msg_31";
    constexpr char KEY_M1S03_MSG_32[] = "M1S03Msg_32";
    constexpr char KEY_M1S03_MSG_33[] = "M1S03Msg_33";

    // =========================
    // Map 1, Stage 04 Messages
    // =========================

    constexpr char KEY_M1S04_MSG_1[] = "M1S04Msg_1";

    // =========================
    // Map 1, Stage 06 Messages
    // =========================

    constexpr char KEY_M1S06_MSG_1[]  = "M1S06Msg_1";
    constexpr char KEY_M1S06_MSG_2[]  = "M1S06Msg_2";
    constexpr char KEY_M1S06_MSG_3[]  = "M1S06Msg_3";
    constexpr char KEY_M1S06_MSG_4[]  = "M1S06Msg_4";
    constexpr char KEY_M1S06_MSG_5[]  = "M1S06Msg_5";
    constexpr char KEY_M1S06_MSG_6[]  = "M1S06Msg_6";
    constexpr char KEY_M1S06_MSG_7[]  = "M1S06Msg_7";
    constexpr char KEY_M1S06_MSG_8[]  = "M1S06Msg_8";
    constexpr char KEY_M1S06_MSG_9[]  = "M1S06Msg_9";
    constexpr char KEY_M1S06_MSG_10[] = "M1S06Msg_10";
    constexpr char KEY_M1S06_MSG_11[] = "M1S06Msg_11";
    constexpr char KEY_M1S06_MSG_12[] = "M1S06Msg_12";
    constexpr char KEY_M1S06_MSG_13[] = "M1S06Msg_13";
    constexpr char KEY_M1S06_MSG_14[] = "M1S06Msg_14";
    constexpr char KEY_M1S06_MSG_15[] = "M1S06Msg_15";
    constexpr char KEY_M1S06_MSG_16[] = "M1S06Msg_16";

    // =========================
    // Map 2, Stage 00 Messages
    // =========================

    constexpr char KEY_M2S00_MSG_1[]  = "M2S00Msg_1";
    constexpr char KEY_M2S00_MSG_2[]  = "M2S00Msg_2";
    constexpr char KEY_M2S00_MSG_3[]  = "M2S00Msg_3";
    constexpr char KEY_M2S00_MSG_4[]  = "M2S00Msg_4";
    constexpr char KEY_M2S00_MSG_5[]  = "M2S00Msg_5";
    constexpr char KEY_M2S00_MSG_6[]  = "M2S00Msg_6";
    constexpr char KEY_M2S00_MSG_7[]  = "M2S00Msg_7";
    constexpr char KEY_M2S00_MSG_8[]  = "M2S00Msg_8";
    constexpr char KEY_M2S00_MSG_9[]  = "M2S00Msg_9";
    constexpr char KEY_M2S00_MSG_10[] = "M2S00Msg_10";
    constexpr char KEY_M2S00_MSG_11[] = "M2S00Msg_11";
    constexpr char KEY_M2S00_MSG_12[] = "M2S00Msg_12";
    constexpr char KEY_M2S00_MSG_13[] = "M2S00Msg_13";
    constexpr char KEY_M2S00_MSG_14[] = "M2S00Msg_14";
    constexpr char KEY_M2S00_MSG_15[] = "M2S00Msg_15";
    constexpr char KEY_M2S00_MSG_16[] = "M2S00Msg_16";
    constexpr char KEY_M2S00_MSG_17[] = "M2S00Msg_17";
    constexpr char KEY_M2S00_MSG_18[] = "M2S00Msg_18";
    constexpr char KEY_M2S00_MSG_19[] = "M2S00Msg_19";
    constexpr char KEY_M2S00_MSG_20[] = "M2S00Msg_20";
    constexpr char KEY_M2S00_MSG_21[] = "M2S00Msg_21";
    constexpr char KEY_M2S00_MSG_22[] = "M2S00Msg_22";
    constexpr char KEY_M2S00_MSG_23[] = "M2S00Msg_23";
    constexpr char KEY_M2S00_MSG_24[] = "M2S00Msg_24";
    constexpr char KEY_M2S00_MSG_25[] = "M2S00Msg_25";
    constexpr char KEY_M2S00_MSG_26[] = "M2S00Msg_26";
    constexpr char KEY_M2S00_MSG_27[] = "M2S00Msg_27";
    constexpr char KEY_M2S00_MSG_28[] = "M2S00Msg_28";
    constexpr char KEY_M2S00_MSG_29[] = "M2S00Msg_29";
    constexpr char KEY_M2S00_MSG_30[] = "M2S00Msg_30";
    constexpr char KEY_M2S00_MSG_31[] = "M2S00Msg_31";
    constexpr char KEY_M2S00_MSG_32[] = "M2S00Msg_32";
    constexpr char KEY_M2S00_MSG_33[] = "M2S00Msg_33";
    constexpr char KEY_M2S00_MSG_34[] = "M2S00Msg_34";
    constexpr char KEY_M2S00_MSG_35[] = "M2S00Msg_35";
    constexpr char KEY_M2S00_MSG_36[] = "M2S00Msg_36";
    constexpr char KEY_M2S00_MSG_37[] = "M2S00Msg_37";
    constexpr char KEY_M2S00_MSG_38[] = "M2S00Msg_38";
    constexpr char KEY_M2S00_MSG_39[] = "M2S00Msg_39";
    constexpr char KEY_M2S00_MSG_40[] = "M2S00Msg_40";
    constexpr char KEY_M2S00_MSG_41[] = "M2S00Msg_41";
    constexpr char KEY_M2S00_MSG_42[] = "M2S00Msg_42";
    constexpr char KEY_M2S00_MSG_43[] = "M2S00Msg_43";
    constexpr char KEY_M2S00_MSG_44[] = "M2S00Msg_44";
    constexpr char KEY_M2S00_MSG_45[] = "M2S00Msg_45";
    constexpr char KEY_M2S00_MSG_46[] = "M2S00Msg_46";
    constexpr char KEY_M2S00_MSG_47[] = "M2S00Msg_47";
    constexpr char KEY_M2S00_MSG_48[] = "M2S00Msg_48";
    constexpr char KEY_M2S00_MSG_49[] = "M2S00Msg_49";
    constexpr char KEY_M2S00_MSG_50[] = "M2S00Msg_50";
    constexpr char KEY_M2S00_MSG_51[] = "M2S00Msg_51";
    constexpr char KEY_M2S00_MSG_52[] = "M2S00Msg_52";
    constexpr char KEY_M2S00_MSG_53[] = "M2S00Msg_53";
    constexpr char KEY_M2S00_MSG_54[] = "M2S00Msg_54";
    constexpr char KEY_M2S00_MSG_55[] = "M2S00Msg_55";
    constexpr char KEY_M2S00_MSG_56[] = "M2S00Msg_56";
    constexpr char KEY_M2S00_MSG_57[] = "M2S00Msg_57";
    constexpr char KEY_M2S00_MSG_58[] = "M2S00Msg_58";
    constexpr char KEY_M2S00_MSG_59[] = "M2S00Msg_59";
    constexpr char KEY_M2S00_MSG_60[] = "M2S00Msg_60";
    constexpr char KEY_M2S00_MSG_61[] = "M2S00Msg_61";
    constexpr char KEY_M2S00_MSG_62[] = "M2S00Msg_62";
    constexpr char KEY_M2S00_MSG_63[] = "M2S00Msg_63";
    constexpr char KEY_M2S00_MSG_64[] = "M2S00Msg_64";

    // =========================
    // Map 2, Stage 01 Messages
    // =========================

    constexpr char KEY_M2S01_MSG_1[]  = "M2S01Msg_1";
    constexpr char KEY_M2S01_MSG_2[]  = "M2S01Msg_2";
    constexpr char KEY_M2S01_MSG_3[]  = "M2S01Msg_3";
    constexpr char KEY_M2S01_MSG_4[]  = "M2S01Msg_4";
    constexpr char KEY_M2S01_MSG_5[]  = "M2S01Msg_5";
    constexpr char KEY_M2S01_MSG_6[]  = "M2S01Msg_6";
    constexpr char KEY_M2S01_MSG_7[]  = "M2S01Msg_7";
    constexpr char KEY_M2S01_MSG_8[]  = "M2S01Msg_8";
    constexpr char KEY_M2S01_MSG_9[]  = "M2S01Msg_9";
    constexpr char KEY_M2S01_MSG_10[] = "M2S01Msg_10";
    constexpr char KEY_M2S01_MSG_11[] = "M2S01Msg_11";
    constexpr char KEY_M2S01_MSG_12[] = "M2S01Msg_12";
    constexpr char KEY_M2S01_MSG_13[] = "M2S01Msg_13";
    constexpr char KEY_M2S01_MSG_14[] = "M2S01Msg_14";
    constexpr char KEY_M2S01_MSG_15[] = "M2S01Msg_15";
    constexpr char KEY_M2S01_MSG_16[] = "M2S01Msg_16";
    constexpr char KEY_M2S01_MSG_17[] = "M2S01Msg_17";
    constexpr char KEY_M2S01_MSG_18[] = "M2S01Msg_18";
    constexpr char KEY_M2S01_MSG_19[] = "M2S01Msg_19";
    constexpr char KEY_M2S01_MSG_20[] = "M2S01Msg_20";
    constexpr char KEY_M2S01_MSG_21[] = "M2S01Msg_21";
    constexpr char KEY_M2S01_MSG_22[] = "M2S01Msg_22";
    constexpr char KEY_M2S01_MSG_23[] = "M2S01Msg_23";
    constexpr char KEY_M2S01_MSG_24[] = "M2S01Msg_24";
    constexpr char KEY_M2S01_MSG_25[] = "M2S01Msg_25";
    constexpr char KEY_M2S01_MSG_26[] = "M2S01Msg_26";
    constexpr char KEY_M2S01_MSG_27[] = "M2S01Msg_27";
    constexpr char KEY_M2S01_MSG_28[] = "M2S01Msg_28";
    constexpr char KEY_M2S01_MSG_29[] = "M2S01Msg_29";

    // =========================
    // Map 2, Stage 02 Messages
    // =========================

    constexpr char KEY_M2S02_MSG_1[] = "M2S02Msg_1";
    constexpr char KEY_M2S02_MSG_2[] = "M2S02Msg_2";
    constexpr char KEY_M2S02_MSG_3[] = "M2S02Msg_3";

    // =========================
    // Map 2, Stage 03 Messages
    // =========================

    constexpr char KEY_M2S03_MSG_1[] = "M2S03Msg_1";

    // =========================
    // Map 2, Stage 04 Messages
    // =========================

    constexpr char KEY_M2S04_MSG_1[]  = "M2S04Msg_1";
    constexpr char KEY_M2S04_MSG_2[]  = "M2S04Msg_2";
    constexpr char KEY_M2S04_MSG_3[]  = "M2S04Msg_3";
    constexpr char KEY_M2S04_MSG_4[]  = "M2S04Msg_4";
    constexpr char KEY_M2S04_MSG_5[]  = "M2S04Msg_5";
    constexpr char KEY_M2S04_MSG_6[]  = "M2S04Msg_6";
    constexpr char KEY_M2S04_MSG_7[]  = "M2S04Msg_7";
    constexpr char KEY_M2S04_MSG_8[]  = "M2S04Msg_8";
    constexpr char KEY_M2S04_MSG_9[]  = "M2S04Msg_9";
    constexpr char KEY_M2S04_MSG_10[] = "M2S04Msg_10";
    constexpr char KEY_M2S04_MSG_11[] = "M2S04Msg_11";
    constexpr char KEY_M2S04_MSG_12[] = "M2S04Msg_12";

    // =========================
    // Map 3, Stage 00 Messages
    // =========================

    constexpr char KEY_M3S00_MSG_1[]  = "M3S00Msg_1";
    constexpr char KEY_M3S00_MSG_2[]  = "M3S00Msg_2";
    constexpr char KEY_M3S00_MSG_3[]  = "M3S00Msg_3";
    constexpr char KEY_M3S00_MSG_4[]  = "M3S00Msg_4";
    constexpr char KEY_M3S00_MSG_5[]  = "M3S00Msg_5";
    constexpr char KEY_M3S00_MSG_6[]  = "M3S00Msg_6";
    constexpr char KEY_M3S00_MSG_7[]  = "M3S00Msg_7";
    constexpr char KEY_M3S00_MSG_8[]  = "M3S00Msg_8";
    constexpr char KEY_M3S00_MSG_9[]  = "M3S00Msg_9";
    constexpr char KEY_M3S00_MSG_10[] = "M3S00Msg_10";
    constexpr char KEY_M3S00_MSG_11[] = "M3S00Msg_11";
    constexpr char KEY_M3S00_MSG_12[] = "M3S00Msg_12";
    constexpr char KEY_M3S00_MSG_13[] = "M3S00Msg_13";
    constexpr char KEY_M3S00_MSG_14[] = "M3S00Msg_14";
    constexpr char KEY_M3S00_MSG_15[] = "M3S00Msg_15";
    constexpr char KEY_M3S00_MSG_16[] = "M3S00Msg_16";
    constexpr char KEY_M3S00_MSG_17[] = "M3S00Msg_17";
    constexpr char KEY_M3S00_MSG_18[] = "M3S00Msg_18";
    constexpr char KEY_M3S00_MSG_19[] = "M3S00Msg_19";
    constexpr char KEY_M3S00_MSG_20[] = "M3S00Msg_20";
    constexpr char KEY_M3S00_MSG_21[] = "M3S00Msg_21";
    constexpr char KEY_M3S00_MSG_22[] = "M3S00Msg_22";
    constexpr char KEY_M3S00_MSG_23[] = "M3S00Msg_23";
    constexpr char KEY_M3S00_MSG_24[] = "M3S00Msg_24";
    constexpr char KEY_M3S00_MSG_25[] = "M3S00Msg_25";
    constexpr char KEY_M3S00_MSG_26[] = "M3S00Msg_26";
    constexpr char KEY_M3S00_MSG_27[] = "M3S00Msg_27";
    constexpr char KEY_M3S00_MSG_28[] = "M3S00Msg_28";
    constexpr char KEY_M3S00_MSG_29[] = "M3S00Msg_29";
    constexpr char KEY_M3S00_MSG_30[] = "M3S00Msg_30";
    constexpr char KEY_M3S00_MSG_31[] = "M3S00Msg_31";
    constexpr char KEY_M3S00_MSG_32[] = "M3S00Msg_32";
    constexpr char KEY_M3S00_MSG_33[] = "M3S00Msg_33";
    constexpr char KEY_M3S00_MSG_34[] = "M3S00Msg_34";
    constexpr char KEY_M3S00_MSG_35[] = "M3S00Msg_35";
    constexpr char KEY_M3S00_MSG_36[] = "M3S00Msg_36";
    constexpr char KEY_M3S00_MSG_37[] = "M3S00Msg_37";

    // =========================
    // Map 3, Stage 01 Messages
    // =========================

    constexpr char KEY_M3S01_MSG_1[]  = "M3S01Msg_1";
    constexpr char KEY_M3S01_MSG_2[]  = "M3S01Msg_2";
    constexpr char KEY_M3S01_MSG_3[]  = "M3S01Msg_3";
    constexpr char KEY_M3S01_MSG_4[]  = "M3S01Msg_4";
    constexpr char KEY_M3S01_MSG_5[]  = "M3S01Msg_5";
    constexpr char KEY_M3S01_MSG_6[]  = "M3S01Msg_6";
    constexpr char KEY_M3S01_MSG_7[]  = "M3S01Msg_7";
    constexpr char KEY_M3S01_MSG_8[]  = "M3S01Msg_8";
    constexpr char KEY_M3S01_MSG_9[]  = "M3S01Msg_9";
    constexpr char KEY_M3S01_MSG_10[] = "M3S01Msg_10";
    constexpr char KEY_M3S01_MSG_11[] = "M3S01Msg_11";
    constexpr char KEY_M3S01_MSG_12[] = "M3S01Msg_12";
    constexpr char KEY_M3S01_MSG_13[] = "M3S01Msg_13";
    constexpr char KEY_M3S01_MSG_14[] = "M3S01Msg_14";
    constexpr char KEY_M3S01_MSG_15[] = "M3S01Msg_15";
    constexpr char KEY_M3S01_MSG_16[] = "M3S01Msg_16";
    constexpr char KEY_M3S01_MSG_17[] = "M3S01Msg_17";
    constexpr char KEY_M3S01_MSG_18[] = "M3S01Msg_18";
    constexpr char KEY_M3S01_MSG_19[] = "M3S01Msg_19";
    constexpr char KEY_M3S01_MSG_20[] = "M3S01Msg_20";
    constexpr char KEY_M3S01_MSG_21[] = "M3S01Msg_21";
    constexpr char KEY_M3S01_MSG_22[] = "M3S01Msg_22";
    constexpr char KEY_M3S01_MSG_23[] = "M3S01Msg_23";
    constexpr char KEY_M3S01_MSG_24[] = "M3S01Msg_24";
    constexpr char KEY_M3S01_MSG_25[] = "M3S01Msg_25";
    constexpr char KEY_M3S01_MSG_26[] = "M3S01Msg_26";

    // =========================
    // Map 3, Stage 02 Messages
    // =========================

    constexpr char KEY_M3S02_MSG_1[] = "M3S02Msg_1";
    constexpr char KEY_M3S02_MSG_2[] = "M3S02Msg_2";

    // =========================
    // Map 3, Stage 03 Messages
    // =========================

    constexpr char KEY_M3S03_MSG_1[]  = "M3S03Msg_1";
    constexpr char KEY_M3S03_MSG_2[]  = "M3S03Msg_2";
    constexpr char KEY_M3S03_MSG_3[]  = "M3S03Msg_3";
    constexpr char KEY_M3S03_MSG_4[]  = "M3S03Msg_4";
    constexpr char KEY_M3S03_MSG_5[]  = "M3S03Msg_5";
    constexpr char KEY_M3S03_MSG_6[]  = "M3S03Msg_6";
    constexpr char KEY_M3S03_MSG_7[]  = "M3S03Msg_7";
    constexpr char KEY_M3S03_MSG_8[]  = "M3S03Msg_8";
    constexpr char KEY_M3S03_MSG_9[]  = "M3S03Msg_9";
    constexpr char KEY_M3S03_MSG_10[] = "M3S03Msg_10";
    constexpr char KEY_M3S03_MSG_11[] = "M3S03Msg_11";
    constexpr char KEY_M3S03_MSG_12[] = "M3S03Msg_12";
    constexpr char KEY_M3S03_MSG_13[] = "M3S03Msg_13";
    constexpr char KEY_M3S03_MSG_14[] = "M3S03Msg_14";
    constexpr char KEY_M3S03_MSG_15[] = "M3S03Msg_15";
    constexpr char KEY_M3S03_MSG_16[] = "M3S03Msg_16";
    constexpr char KEY_M3S03_MSG_17[] = "M3S03Msg_17";
    constexpr char KEY_M3S03_MSG_18[] = "M3S03Msg_18";
    constexpr char KEY_M3S03_MSG_19[] = "M3S03Msg_19";
    constexpr char KEY_M3S03_MSG_20[] = "M3S03Msg_20";
    constexpr char KEY_M3S03_MSG_21[] = "M3S03Msg_21";
    constexpr char KEY_M3S03_MSG_22[] = "M3S03Msg_22";
    constexpr char KEY_M3S03_MSG_23[] = "M3S03Msg_23";
    constexpr char KEY_M3S03_MSG_24[] = "M3S03Msg_24";
    constexpr char KEY_M3S03_MSG_25[] = "M3S03Msg_25";
    constexpr char KEY_M3S03_MSG_26[] = "M3S03Msg_26";
    constexpr char KEY_M3S03_MSG_27[] = "M3S03Msg_27";
    constexpr char KEY_M3S03_MSG_28[] = "M3S03Msg_28";
    constexpr char KEY_M3S03_MSG_29[] = "M3S03Msg_29";
    constexpr char KEY_M3S03_MSG_30[] = "M3S03Msg_30";
    constexpr char KEY_M3S03_MSG_31[] = "M3S03Msg_31";
    constexpr char KEY_M3S03_MSG_32[] = "M3S03Msg_32";
    constexpr char KEY_M3S03_MSG_33[] = "M3S03Msg_33";
    constexpr char KEY_M3S03_MSG_34[] = "M3S03Msg_34";
    constexpr char KEY_M3S03_MSG_35[] = "M3S03Msg_35";
    constexpr char KEY_M3S03_MSG_36[] = "M3S03Msg_36";
    constexpr char KEY_M3S03_MSG_37[] = "M3S03Msg_37";
    constexpr char KEY_M3S03_MSG_38[] = "M3S03Msg_38";
    constexpr char KEY_M3S03_MSG_39[] = "M3S03Msg_39";
    constexpr char KEY_M3S03_MSG_40[] = "M3S03Msg_40";
    constexpr char KEY_M3S03_MSG_41[] = "M3S03Msg_41";
    constexpr char KEY_M3S03_MSG_42[] = "M3S03Msg_42";
    constexpr char KEY_M3S03_MSG_43[] = "M3S03Msg_43";
    constexpr char KEY_M3S03_MSG_44[] = "M3S03Msg_44";
    constexpr char KEY_M3S03_MSG_45[] = "M3S03Msg_45";
    constexpr char KEY_M3S03_MSG_46[] = "M3S03Msg_46";
    constexpr char KEY_M3S03_MSG_47[] = "M3S03Msg_47";
    constexpr char KEY_M3S03_MSG_48[] = "M3S03Msg_48";

    // =========================
    // Map 3, Stage 04 Messages
    // =========================

    constexpr char KEY_M3S04_MSG_1[]  = "M3S04Msg_1";
    constexpr char KEY_M3S04_MSG_2[]  = "M3S04Msg_2";
    constexpr char KEY_M3S04_MSG_3[]  = "M3S04Msg_3";
    constexpr char KEY_M3S04_MSG_4[]  = "M3S04Msg_4";
    constexpr char KEY_M3S04_MSG_5[]  = "M3S04Msg_5";
    constexpr char KEY_M3S04_MSG_6[]  = "M3S04Msg_6";
    constexpr char KEY_M3S04_MSG_7[]  = "M3S04Msg_7";
    constexpr char KEY_M3S04_MSG_8[]  = "M3S04Msg_8";
    constexpr char KEY_M3S04_MSG_9[]  = "M3S04Msg_9";
    constexpr char KEY_M3S04_MSG_10[] = "M3S04Msg_10";
    constexpr char KEY_M3S04_MSG_11[] = "M3S04Msg_11";
    constexpr char KEY_M3S04_MSG_12[] = "M3S04Msg_12";
    constexpr char KEY_M3S04_MSG_13[] = "M3S04Msg_13";
    constexpr char KEY_M3S04_MSG_14[] = "M3S04Msg_14";
    constexpr char KEY_M3S04_MSG_15[] = "M3S04Msg_15";
    constexpr char KEY_M3S04_MSG_16[] = "M3S04Msg_16";
    constexpr char KEY_M3S04_MSG_17[] = "M3S04Msg_17";
    constexpr char KEY_M3S04_MSG_18[] = "M3S04Msg_18";
    constexpr char KEY_M3S04_MSG_19[] = "M3S04Msg_19";
    constexpr char KEY_M3S04_MSG_20[] = "M3S04Msg_20";
    constexpr char KEY_M3S04_MSG_21[] = "M3S04Msg_21";
    constexpr char KEY_M3S04_MSG_22[] = "M3S04Msg_22";
    constexpr char KEY_M3S04_MSG_23[] = "M3S04Msg_23";
    constexpr char KEY_M3S04_MSG_24[] = "M3S04Msg_24";
    constexpr char KEY_M3S04_MSG_25[] = "M3S04Msg_25";
    constexpr char KEY_M3S04_MSG_26[] = "M3S04Msg_26";
    constexpr char KEY_M3S04_MSG_27[] = "M3S04Msg_27";
    constexpr char KEY_M3S04_MSG_28[] = "M3S04Msg_28";
    constexpr char KEY_M3S04_MSG_29[] = "M3S04Msg_29";
    constexpr char KEY_M3S04_MSG_30[] = "M3S04Msg_30";
    constexpr char KEY_M3S04_MSG_31[] = "M3S04Msg_31";
    constexpr char KEY_M3S04_MSG_32[] = "M3S04Msg_32";
    constexpr char KEY_M3S04_MSG_33[] = "M3S04Msg_33";
    constexpr char KEY_M3S04_MSG_34[] = "M3S04Msg_34";
    constexpr char KEY_M3S04_MSG_35[] = "M3S04Msg_35";
    constexpr char KEY_M3S04_MSG_36[] = "M3S04Msg_36";
    constexpr char KEY_M3S04_MSG_37[] = "M3S04Msg_37";
    constexpr char KEY_M3S04_MSG_38[] = "M3S04Msg_38";
    constexpr char KEY_M3S04_MSG_39[] = "M3S04Msg_39";
    constexpr char KEY_M3S04_MSG_40[] = "M3S04Msg_40";

    // =========================
    // Map 3, Stage 05 Messages
    // =========================

    constexpr char KEY_M3S05_MSG_1[]  = "M3S05Msg_1";
    constexpr char KEY_M3S05_MSG_2[]  = "M3S05Msg_2";
    constexpr char KEY_M3S05_MSG_3[]  = "M3S05Msg_3";
    constexpr char KEY_M3S05_MSG_4[]  = "M3S05Msg_4";
    constexpr char KEY_M3S05_MSG_5[]  = "M3S05Msg_5";
    constexpr char KEY_M3S05_MSG_6[]  = "M3S05Msg_6";
    constexpr char KEY_M3S05_MSG_7[]  = "M3S05Msg_7";
    constexpr char KEY_M3S05_MSG_8[]  = "M3S05Msg_8";
    constexpr char KEY_M3S05_MSG_9[]  = "M3S05Msg_9";
    constexpr char KEY_M3S05_MSG_10[] = "M3S05Msg_10";
    constexpr char KEY_M3S05_MSG_11[] = "M3S05Msg_11";
    constexpr char KEY_M3S05_MSG_12[] = "M3S05Msg_12";
    constexpr char KEY_M3S05_MSG_13[] = "M3S05Msg_13";
    constexpr char KEY_M3S05_MSG_14[] = "M3S05Msg_14";
    constexpr char KEY_M3S05_MSG_15[] = "M3S05Msg_15";
    constexpr char KEY_M3S05_MSG_16[] = "M3S05Msg_16";
    constexpr char KEY_M3S05_MSG_17[] = "M3S05Msg_17";
    constexpr char KEY_M3S05_MSG_18[] = "M3S05Msg_18";
    constexpr char KEY_M3S05_MSG_19[] = "M3S05Msg_19";
    constexpr char KEY_M3S05_MSG_20[] = "M3S05Msg_20";
    constexpr char KEY_M3S05_MSG_21[] = "M3S05Msg_21";
    constexpr char KEY_M3S05_MSG_22[] = "M3S05Msg_22";
    constexpr char KEY_M3S05_MSG_23[] = "M3S05Msg_23";
    constexpr char KEY_M3S05_MSG_24[] = "M3S05Msg_24";
    constexpr char KEY_M3S05_MSG_25[] = "M3S05Msg_25";

    // =========================
    // Map 3, Stage 06 Messages
    // =========================

    constexpr char KEY_M3S06_MSG_1[]  = "M3S06Msg_1";
    constexpr char KEY_M3S06_MSG_2[]  = "M3S06Msg_2";
    constexpr char KEY_M3S06_MSG_3[]  = "M3S06Msg_3";
    constexpr char KEY_M3S06_MSG_4[]  = "M3S06Msg_4";
    constexpr char KEY_M3S06_MSG_5[]  = "M3S06Msg_5";
    constexpr char KEY_M3S06_MSG_6[]  = "M3S06Msg_6";
    constexpr char KEY_M3S06_MSG_7[]  = "M3S06Msg_7";
    constexpr char KEY_M3S06_MSG_8[]  = "M3S06Msg_8";
    constexpr char KEY_M3S06_MSG_9[]  = "M3S06Msg_9";
    constexpr char KEY_M3S06_MSG_10[] = "M3S06Msg_10";
    constexpr char KEY_M3S06_MSG_11[] = "M3S06Msg_11";
    constexpr char KEY_M3S06_MSG_12[] = "M3S06Msg_12";
    constexpr char KEY_M3S06_MSG_13[] = "M3S06Msg_13";
    constexpr char KEY_M3S06_MSG_14[] = "M3S06Msg_14";
    constexpr char KEY_M3S06_MSG_15[] = "M3S06Msg_15";
    constexpr char KEY_M3S06_MSG_16[] = "M3S06Msg_16";
    constexpr char KEY_M3S06_MSG_17[] = "M3S06Msg_17";
    constexpr char KEY_M3S06_MSG_18[] = "M3S06Msg_18";
    constexpr char KEY_M3S06_MSG_19[] = "M3S06Msg_19";
    constexpr char KEY_M3S06_MSG_20[] = "M3S06Msg_20";
    constexpr char KEY_M3S06_MSG_21[] = "M3S06Msg_21";
    constexpr char KEY_M3S06_MSG_22[] = "M3S06Msg_22";
    constexpr char KEY_M3S06_MSG_23[] = "M3S06Msg_23";
    constexpr char KEY_M3S06_MSG_24[] = "M3S06Msg_24";
    constexpr char KEY_M3S06_MSG_25[] = "M3S06Msg_25";
    constexpr char KEY_M3S06_MSG_26[] = "M3S06Msg_26";
    constexpr char KEY_M3S06_MSG_27[] = "M3S06Msg_27";
    constexpr char KEY_M3S06_MSG_28[] = "M3S06Msg_28";
    constexpr char KEY_M3S06_MSG_29[] = "M3S06Msg_29";
    constexpr char KEY_M3S06_MSG_30[] = "M3S06Msg_30";
    constexpr char KEY_M3S06_MSG_31[] = "M3S06Msg_31";
    constexpr char KEY_M3S06_MSG_32[] = "M3S06Msg_32";

    // =========================
    // Map 4, Stage 01 Messages
    // =========================

    constexpr char KEY_M4S01_MSG_1[]   = "M4S01Msg_1";
    constexpr char KEY_M4S01_MSG_2[]   = "M4S01Msg_2";
    constexpr char KEY_M4S01_MSG_3[]   = "M4S01Msg_3";
    constexpr char KEY_M4S01_MSG_4[]   = "M4S01Msg_4";
    constexpr char KEY_M4S01_MSG_5[]   = "M4S01Msg_5";
    constexpr char KEY_M4S01_MSG_6[]   = "M4S01Msg_6";
    constexpr char KEY_M4S01_MSG_7[]   = "M4S01Msg_7";
    constexpr char KEY_M4S01_MSG_8[]   = "M4S01Msg_8";
    constexpr char KEY_M4S01_MSG_9[]   = "M4S01Msg_9";
    constexpr char KEY_M4S01_MSG_10[]  = "M4S01Msg_10";
    constexpr char KEY_M4S01_MSG_11[]  = "M4S01Msg_11";
    constexpr char KEY_M4S01_MSG_12[]  = "M4S01Msg_12";
    constexpr char KEY_M4S01_MSG_13[]  = "M4S01Msg_13";
    constexpr char KEY_M4S01_MSG_14[]  = "M4S01Msg_14";
    constexpr char KEY_M4S01_MSG_15[]  = "M4S01Msg_15";
    constexpr char KEY_M4S01_MSG_16[]  = "M4S01Msg_16";
    constexpr char KEY_M4S01_MSG_17[]  = "M4S01Msg_17";
    constexpr char KEY_M4S01_MSG_18[]  = "M4S01Msg_18";
    constexpr char KEY_M4S01_MSG_19[]  = "M4S01Msg_19";
    constexpr char KEY_M4S01_MSG_20[]  = "M4S01Msg_20";
    constexpr char KEY_M4S01_MSG_21[]  = "M4S01Msg_21";
    constexpr char KEY_M4S01_MSG_22[]  = "M4S01Msg_22";
    constexpr char KEY_M4S01_MSG_23[]  = "M4S01Msg_23";
    constexpr char KEY_M4S01_MSG_24[]  = "M4S01Msg_24";
    constexpr char KEY_M4S01_MSG_25[]  = "M4S01Msg_25";
    constexpr char KEY_M4S01_MSG_26[]  = "M4S01Msg_26";
    constexpr char KEY_M4S01_MSG_27[]  = "M4S01Msg_27";
    constexpr char KEY_M4S01_MSG_28[]  = "M4S01Msg_28";
    constexpr char KEY_M4S01_MSG_29[]  = "M4S01Msg_29";
    constexpr char KEY_M4S01_MSG_30[]  = "M4S01Msg_30";
    constexpr char KEY_M4S01_MSG_31[]  = "M4S01Msg_31";
    constexpr char KEY_M4S01_MSG_32[]  = "M4S01Msg_32";
    constexpr char KEY_M4S01_MSG_33[]  = "M4S01Msg_33";
    constexpr char KEY_M4S01_MSG_34[]  = "M4S01Msg_34";
    constexpr char KEY_M4S01_MSG_35[]  = "M4S01Msg_35";
    constexpr char KEY_M4S01_MSG_36[]  = "M4S01Msg_36";
    constexpr char KEY_M4S01_MSG_37[]  = "M4S01Msg_37";
    constexpr char KEY_M4S01_MSG_38[]  = "M4S01Msg_38";
    constexpr char KEY_M4S01_MSG_39[]  = "M4S01Msg_39";
    constexpr char KEY_M4S01_MSG_40[]  = "M4S01Msg_40";
    constexpr char KEY_M4S01_MSG_41[]  = "M4S01Msg_41";
    constexpr char KEY_M4S01_MSG_42[]  = "M4S01Msg_42";
    constexpr char KEY_M4S01_MSG_43[]  = "M4S01Msg_43";
    constexpr char KEY_M4S01_MSG_44[]  = "M4S01Msg_44";
    constexpr char KEY_M4S01_MSG_45[]  = "M4S01Msg_45";
    constexpr char KEY_M4S01_MSG_46[]  = "M4S01Msg_46";
    constexpr char KEY_M4S01_MSG_47[]  = "M4S01Msg_47";
    constexpr char KEY_M4S01_MSG_48[]  = "M4S01Msg_48";
    constexpr char KEY_M4S01_MSG_49[]  = "M4S01Msg_49";
    constexpr char KEY_M4S01_MSG_50[]  = "M4S01Msg_50";
    constexpr char KEY_M4S01_MSG_51[]  = "M4S01Msg_51";
    constexpr char KEY_M4S01_MSG_52[]  = "M4S01Msg_52";
    constexpr char KEY_M4S01_MSG_53[]  = "M4S01Msg_53";
    constexpr char KEY_M4S01_MSG_54[]  = "M4S01Msg_54";
    constexpr char KEY_M4S01_MSG_55[]  = "M4S01Msg_55";
    constexpr char KEY_M4S01_MSG_56[]  = "M4S01Msg_56";
    constexpr char KEY_M4S01_MSG_57[]  = "M4S01Msg_57";
    constexpr char KEY_M4S01_MSG_58[]  = "M4S01Msg_58";
    constexpr char KEY_M4S01_MSG_59[]  = "M4S01Msg_59";
    constexpr char KEY_M4S01_MSG_60[]  = "M4S01Msg_60";
    constexpr char KEY_M4S01_MSG_61[]  = "M4S01Msg_61";
    constexpr char KEY_M4S01_MSG_62[]  = "M4S01Msg_62";
    constexpr char KEY_M4S01_MSG_63[]  = "M4S01Msg_63";
    constexpr char KEY_M4S01_MSG_64[]  = "M4S01Msg_64";
    constexpr char KEY_M4S01_MSG_65[]  = "M4S01Msg_65";
    constexpr char KEY_M4S01_MSG_66[]  = "M4S01Msg_66";
    constexpr char KEY_M4S01_MSG_67[]  = "M4S01Msg_67";
    constexpr char KEY_M4S01_MSG_68[]  = "M4S01Msg_68";
    constexpr char KEY_M4S01_MSG_69[]  = "M4S01Msg_69";
    constexpr char KEY_M4S01_MSG_70[]  = "M4S01Msg_70";
    constexpr char KEY_M4S01_MSG_71[]  = "M4S01Msg_71";
    constexpr char KEY_M4S01_MSG_72[]  = "M4S01Msg_72";
    constexpr char KEY_M4S01_MSG_73[]  = "M4S01Msg_73";
    constexpr char KEY_M4S01_MSG_74[]  = "M4S01Msg_74";
    constexpr char KEY_M4S01_MSG_75[]  = "M4S01Msg_75";
    constexpr char KEY_M4S01_MSG_76[]  = "M4S01Msg_76";
    constexpr char KEY_M4S01_MSG_77[]  = "M4S01Msg_77";
    constexpr char KEY_M4S01_MSG_78[]  = "M4S01Msg_78";
    constexpr char KEY_M4S01_MSG_79[]  = "M4S01Msg_79";
    constexpr char KEY_M4S01_MSG_80[]  = "M4S01Msg_80";
    constexpr char KEY_M4S01_MSG_81[]  = "M4S01Msg_81";
    constexpr char KEY_M4S01_MSG_82[]  = "M4S01Msg_82";
    constexpr char KEY_M4S01_MSG_83[]  = "M4S01Msg_83";
    constexpr char KEY_M4S01_MSG_84[]  = "M4S01Msg_84";
    constexpr char KEY_M4S01_MSG_85[]  = "M4S01Msg_85";
    constexpr char KEY_M4S01_MSG_86[]  = "M4S01Msg_86";
    constexpr char KEY_M4S01_MSG_87[]  = "M4S01Msg_87";
    constexpr char KEY_M4S01_MSG_88[]  = "M4S01Msg_88";
    constexpr char KEY_M4S01_MSG_89[]  = "M4S01Msg_89";
    constexpr char KEY_M4S01_MSG_90[]  = "M4S01Msg_90";
    constexpr char KEY_M4S01_MSG_91[]  = "M4S01Msg_91";
    constexpr char KEY_M4S01_MSG_92[]  = "M4S01Msg_92";
    constexpr char KEY_M4S01_MSG_93[]  = "M4S01Msg_93";
    constexpr char KEY_M4S01_MSG_94[]  = "M4S01Msg_94";
    constexpr char KEY_M4S01_MSG_95[]  = "M4S01Msg_95";
    constexpr char KEY_M4S01_MSG_96[]  = "M4S01Msg_96";
    constexpr char KEY_M4S01_MSG_97[]  = "M4S01Msg_97";
    constexpr char KEY_M4S01_MSG_98[]  = "M4S01Msg_98";
    constexpr char KEY_M4S01_MSG_99[]  = "M4S01Msg_99";
    constexpr char KEY_M4S01_MSG_100[] = "M4S01Msg_100";
    constexpr char KEY_M4S01_MSG_101[] = "M4S01Msg_101";
    constexpr char KEY_M4S01_MSG_102[] = "M4S01Msg_102";
    constexpr char KEY_M4S01_MSG_103[] = "M4S01Msg_103";

    // =========================
    // Map 4, Stage 02 Messages
    // =========================

    constexpr char KEY_M4S02_MSG_1[] = "M4S02Msg_1";
    constexpr char KEY_M4S02_MSG_2[] = "M4S02Msg_2";

    // =========================
    // Map 4, Stage 03 Messages
    // =========================

    constexpr char KEY_M4S03_MSG_1[] = "M4S03Msg_1";
    constexpr char KEY_M4S03_MSG_2[] = "M4S03Msg_2";
    constexpr char KEY_M4S03_MSG_3[] = "M4S03Msg_3";
    constexpr char KEY_M4S03_MSG_4[] = "M4S03Msg_4";
    constexpr char KEY_M4S03_MSG_5[] = "M4S03Msg_5";
    constexpr char KEY_M4S03_MSG_6[] = "M4S03Msg_6";

    // =========================
    // Map 4, Stage 04 Messages
    // =========================

    constexpr char KEY_M4S04_MSG_1[]  = "M4S04Msg_1";
    constexpr char KEY_M4S04_MSG_2[]  = "M4S04Msg_2";
    constexpr char KEY_M4S04_MSG_3[]  = "M4S04Msg_3";
    constexpr char KEY_M4S04_MSG_4[]  = "M4S04Msg_4";
    constexpr char KEY_M4S04_MSG_5[]  = "M4S04Msg_5";
    constexpr char KEY_M4S04_MSG_6[]  = "M4S04Msg_6";
    constexpr char KEY_M4S04_MSG_7[]  = "M4S04Msg_7";
    constexpr char KEY_M4S04_MSG_8[]  = "M4S04Msg_8";
    constexpr char KEY_M4S04_MSG_9[]  = "M4S04Msg_9";
    constexpr char KEY_M4S04_MSG_10[] = "M4S04Msg_10";
    constexpr char KEY_M4S04_MSG_11[] = "M4S04Msg_11";
    constexpr char KEY_M4S04_MSG_12[] = "M4S04Msg_12";
    constexpr char KEY_M4S04_MSG_13[] = "M4S04Msg_13";
    constexpr char KEY_M4S04_MSG_14[] = "M4S04Msg_14";
    constexpr char KEY_M4S04_MSG_15[] = "M4S04Msg_15";
    constexpr char KEY_M4S04_MSG_16[] = "M4S04Msg_16";
    constexpr char KEY_M4S04_MSG_17[] = "M4S04Msg_17";
    constexpr char KEY_M4S04_MSG_18[] = "M4S04Msg_18";
    constexpr char KEY_M4S04_MSG_19[] = "M4S04Msg_19";
    constexpr char KEY_M4S04_MSG_20[] = "M4S04Msg_20";
    constexpr char KEY_M4S04_MSG_21[] = "M4S04Msg_21";
    constexpr char KEY_M4S04_MSG_22[] = "M4S04Msg_22";
    constexpr char KEY_M4S04_MSG_23[] = "M4S04Msg_23";
    constexpr char KEY_M4S04_MSG_24[] = "M4S04Msg_24";
    constexpr char KEY_M4S04_MSG_25[] = "M4S04Msg_25";
    constexpr char KEY_M4S04_MSG_26[] = "M4S04Msg_26";
    constexpr char KEY_M4S04_MSG_27[] = "M4S04Msg_27";
    constexpr char KEY_M4S04_MSG_28[] = "M4S04Msg_28";
    constexpr char KEY_M4S04_MSG_29[] = "M4S04Msg_29";
    constexpr char KEY_M4S04_MSG_30[] = "M4S04Msg_30";
    constexpr char KEY_M4S04_MSG_31[] = "M4S04Msg_31";
    constexpr char KEY_M4S04_MSG_32[] = "M4S04Msg_32";
    constexpr char KEY_M4S04_MSG_33[] = "M4S04Msg_33";
    constexpr char KEY_M4S04_MSG_34[] = "M4S04Msg_34";
    constexpr char KEY_M4S04_MSG_35[] = "M4S04Msg_35";
    constexpr char KEY_M4S04_MSG_36[] = "M4S04Msg_36";
    constexpr char KEY_M4S04_MSG_37[] = "M4S04Msg_37";
    constexpr char KEY_M4S04_MSG_38[] = "M4S04Msg_38";
    constexpr char KEY_M4S04_MSG_39[] = "M4S04Msg_39";
    constexpr char KEY_M4S04_MSG_40[] = "M4S04Msg_40";
    constexpr char KEY_M4S04_MSG_41[] = "M4S04Msg_41";
    constexpr char KEY_M4S04_MSG_42[] = "M4S04Msg_42";
    constexpr char KEY_M4S04_MSG_43[] = "M4S04Msg_43";
    constexpr char KEY_M4S04_MSG_44[] = "M4S04Msg_44";
    constexpr char KEY_M4S04_MSG_45[] = "M4S04Msg_45";
    constexpr char KEY_M4S04_MSG_46[] = "M4S04Msg_46";
    constexpr char KEY_M4S04_MSG_47[] = "M4S04Msg_47";
    constexpr char KEY_M4S04_MSG_48[] = "M4S04Msg_48";
    constexpr char KEY_M4S04_MSG_49[] = "M4S04Msg_49";
    constexpr char KEY_M4S04_MSG_50[] = "M4S04Msg_50";
    constexpr char KEY_M4S04_MSG_51[] = "M4S04Msg_51";
    constexpr char KEY_M4S04_MSG_52[] = "M4S04Msg_52";
    constexpr char KEY_M4S04_MSG_53[] = "M4S04Msg_53";
    constexpr char KEY_M4S04_MSG_54[] = "M4S04Msg_54";
    constexpr char KEY_M4S04_MSG_55[] = "M4S04Msg_55";
    constexpr char KEY_M4S04_MSG_56[] = "M4S04Msg_56";
    constexpr char KEY_M4S04_MSG_57[] = "M4S04Msg_57";
    constexpr char KEY_M4S04_MSG_58[] = "M4S04Msg_58";
    constexpr char KEY_M4S04_MSG_59[] = "M4S04Msg_59";
    constexpr char KEY_M4S04_MSG_60[] = "M4S04Msg_60";
    constexpr char KEY_M4S04_MSG_61[] = "M4S04Msg_61";
    constexpr char KEY_M4S04_MSG_62[] = "M4S04Msg_62";
    constexpr char KEY_M4S04_MSG_63[] = "M4S04Msg_63";
    constexpr char KEY_M4S04_MSG_64[] = "M4S04Msg_64";
    constexpr char KEY_M4S04_MSG_65[] = "M4S04Msg_65";
    constexpr char KEY_M4S04_MSG_66[] = "M4S04Msg_66";
    constexpr char KEY_M4S04_MSG_67[] = "M4S04Msg_67";

    // =========================
    // Map 4, Stage 05 Messages
    // =========================

    constexpr char KEY_M4S05_MSG_1[] = "M4S05Msg_1";
    constexpr char KEY_M4S05_MSG_2[] = "M4S05Msg_2";
    constexpr char KEY_M4S05_MSG_3[] = "M4S05Msg_3";
    constexpr char KEY_M4S05_MSG_4[] = "M4S05Msg_4";

    // =========================
    // Map 5, Stage 00 Messages
    // =========================

    constexpr char KEY_M5S00_MSG_1[] = "M5S00Msg_1";
    constexpr char KEY_M5S00_MSG_2[] = "M5S00Msg_2";
    constexpr char KEY_M5S00_MSG_3[] = "M5S00Msg_3";
    constexpr char KEY_M5S00_MSG_4[] = "M5S00Msg_4";
    constexpr char KEY_M5S00_MSG_5[] = "M5S00Msg_5";
    constexpr char KEY_M5S00_MSG_6[] = "M5S00Msg_6";

    // =========================
    // Map 5, Stage 01 Messages
    // =========================

    constexpr char KEY_M5S01_MSG_1[] = "M5S01Msg_1";
    constexpr char KEY_M5S01_MSG_2[] = "M5S01Msg_2";
    constexpr char KEY_M5S01_MSG_3[] = "M5S01Msg_3";
    constexpr char KEY_M5S01_MSG_4[] = "M5S01Msg_4";
    constexpr char KEY_M5S01_MSG_5[] = "M5S01Msg_5";

    // =========================
    // Map 5, Stage 02 Messages
    // =========================

    constexpr char KEY_M5S02_MSG_1[]  = "M5S02Msg_1";
    constexpr char KEY_M5S02_MSG_2[]  = "M5S02Msg_2";
    constexpr char KEY_M5S02_MSG_3[]  = "M5S02Msg_3";
    constexpr char KEY_M5S02_MSG_4[]  = "M5S02Msg_4";
    constexpr char KEY_M5S02_MSG_5[]  = "M5S02Msg_5";
    constexpr char KEY_M5S02_MSG_6[]  = "M5S02Msg_6";
    constexpr char KEY_M5S02_MSG_7[]  = "M5S02Msg_7";
    constexpr char KEY_M5S02_MSG_8[]  = "M5S02Msg_8";
    constexpr char KEY_M5S02_MSG_9[]  = "M5S02Msg_9";
    constexpr char KEY_M5S02_MSG_10[] = "M5S02Msg_10";
    constexpr char KEY_M5S02_MSG_11[] = "M5S02Msg_11";
    constexpr char KEY_M5S02_MSG_12[] = "M5S02Msg_12";
    constexpr char KEY_M5S02_MSG_13[] = "M5S02Msg_13";
    constexpr char KEY_M5S02_MSG_14[] = "M5S02Msg_14";
    constexpr char KEY_M5S02_MSG_15[] = "M5S02Msg_15";
    constexpr char KEY_M5S02_MSG_16[] = "M5S02Msg_16";
    constexpr char KEY_M5S02_MSG_17[] = "M5S02Msg_17";
    constexpr char KEY_M5S02_MSG_18[] = "M5S02Msg_18";
    constexpr char KEY_M5S02_MSG_19[] = "M5S02Msg_19";
    constexpr char KEY_M5S02_MSG_20[] = "M5S02Msg_20";
    constexpr char KEY_M5S02_MSG_21[] = "M5S02Msg_21";
    constexpr char KEY_M5S02_MSG_22[] = "M5S02Msg_22";
    constexpr char KEY_M5S02_MSG_23[] = "M5S02Msg_23";
    constexpr char KEY_M5S02_MSG_24[] = "M5S02Msg_24";
    constexpr char KEY_M5S02_MSG_25[] = "M5S02Msg_25";
    constexpr char KEY_M5S02_MSG_26[] = "M5S02Msg_26";
    constexpr char KEY_M5S02_MSG_27[] = "M5S02Msg_27";
    constexpr char KEY_M5S02_MSG_28[] = "M5S02Msg_28";
    constexpr char KEY_M5S02_MSG_29[] = "M5S02Msg_29";
    constexpr char KEY_M5S02_MSG_30[] = "M5S02Msg_30";
    constexpr char KEY_M5S02_MSG_31[] = "M5S02Msg_31";
    constexpr char KEY_M5S02_MSG_32[] = "M5S02Msg_32";
    constexpr char KEY_M5S02_MSG_33[] = "M5S02Msg_33";
    constexpr char KEY_M5S02_MSG_34[] = "M5S02Msg_34";
    constexpr char KEY_M5S02_MSG_35[] = "M5S02Msg_35";
    constexpr char KEY_M5S02_MSG_36[] = "M5S02Msg_36";

    // =========================
    // Map 5, Stage 03 Messages
    // =========================

    constexpr char KEY_M5S03_MSG_1[]  = "M5S03Msg_1";
    constexpr char KEY_M5S03_MSG_2[]  = "M5S03Msg_2";
    constexpr char KEY_M5S03_MSG_3[]  = "M5S03Msg_3";
    constexpr char KEY_M5S03_MSG_4[]  = "M5S03Msg_4";
    constexpr char KEY_M5S03_MSG_5[]  = "M5S03Msg_5";
    constexpr char KEY_M5S03_MSG_6[]  = "M5S03Msg_6";
    constexpr char KEY_M5S03_MSG_7[]  = "M5S03Msg_7";
    constexpr char KEY_M5S03_MSG_8[]  = "M5S03Msg_8";
    constexpr char KEY_M5S03_MSG_9[]  = "M5S03Msg_9";
    constexpr char KEY_M5S03_MSG_10[] = "M5S03Msg_10";
    constexpr char KEY_M5S03_MSG_11[] = "M5S03Msg_11";
    constexpr char KEY_M5S03_MSG_12[] = "M5S03Msg_12";
    constexpr char KEY_M5S03_MSG_13[] = "M5S03Msg_13";
    constexpr char KEY_M5S03_MSG_14[] = "M5S03Msg_14";
    constexpr char KEY_M5S03_MSG_15[] = "M5S03Msg_15";
    constexpr char KEY_M5S03_MSG_16[] = "M5S03Msg_16";
    constexpr char KEY_M5S03_MSG_17[] = "M5S03Msg_17";
    constexpr char KEY_M5S03_MSG_18[] = "M5S03Msg_18";
    constexpr char KEY_M5S03_MSG_19[] = "M5S03Msg_19";
    constexpr char KEY_M5S03_MSG_20[] = "M5S03Msg_20";
    constexpr char KEY_M5S03_MSG_21[] = "M5S03Msg_21";
    constexpr char KEY_M5S03_MSG_22[] = "M5S03Msg_22";
    constexpr char KEY_M5S03_MSG_23[] = "M5S03Msg_23";
    constexpr char KEY_M5S03_MSG_24[] = "M5S03Msg_24";
    constexpr char KEY_M5S03_MSG_25[] = "M5S03Msg_25";
    constexpr char KEY_M5S03_MSG_26[] = "M5S03Msg_26";
    constexpr char KEY_M5S03_MSG_27[] = "M5S03Msg_27";
    constexpr char KEY_M5S03_MSG_28[] = "M5S03Msg_28";
    constexpr char KEY_M5S03_MSG_29[] = "M5S03Msg_29";
    constexpr char KEY_M5S03_MSG_30[] = "M5S03Msg_30";
    constexpr char KEY_M5S03_MSG_31[] = "M5S03Msg_31";
    constexpr char KEY_M5S03_MSG_32[] = "M5S03Msg_32";
    constexpr char KEY_M5S03_MSG_33[] = "M5S03Msg_33";
    constexpr char KEY_M5S03_MSG_34[] = "M5S03Msg_34";
    constexpr char KEY_M5S03_MSG_35[] = "M5S03Msg_35";
    constexpr char KEY_M5S03_MSG_36[] = "M5S03Msg_36";
    constexpr char KEY_M5S03_MSG_37[] = "M5S03Msg_37";
    constexpr char KEY_M5S03_MSG_38[] = "M5S03Msg_38";
    constexpr char KEY_M5S03_MSG_39[] = "M5S03Msg_39";
    constexpr char KEY_M5S03_MSG_40[] = "M5S03Msg_40";
    constexpr char KEY_M5S03_MSG_41[] = "M5S03Msg_41";
    constexpr char KEY_M5S03_MSG_42[] = "M5S03Msg_42";
    constexpr char KEY_M5S03_MSG_43[] = "M5S03Msg_43";
    constexpr char KEY_M5S03_MSG_44[] = "M5S03Msg_44";
    constexpr char KEY_M5S03_MSG_45[] = "M5S03Msg_45";
    constexpr char KEY_M5S03_MSG_46[] = "M5S03Msg_46";
    constexpr char KEY_M5S03_MSG_47[] = "M5S03Msg_47";
    constexpr char KEY_M5S03_MSG_48[] = "M5S03Msg_48";
    constexpr char KEY_M5S03_MSG_49[] = "M5S03Msg_49";
    constexpr char KEY_M5S03_MSG_50[] = "M5S03Msg_50";
    constexpr char KEY_M5S03_MSG_51[] = "M5S03Msg_51";
    constexpr char KEY_M5S03_MSG_52[] = "M5S03Msg_52";
    constexpr char KEY_M5S03_MSG_53[] = "M5S03Msg_53";

    // =========================
    // Map 6, Stage 00 Messages
    // =========================

    constexpr char KEY_M6S00_MSG_1[]  = "M6S00Msg_1";
    constexpr char KEY_M6S00_MSG_2[]  = "M6S00Msg_2";
    constexpr char KEY_M6S00_MSG_3[]  = "M6S00Msg_3";
    constexpr char KEY_M6S00_MSG_4[]  = "M6S00Msg_4";
    constexpr char KEY_M6S00_MSG_5[]  = "M6S00Msg_5";
    constexpr char KEY_M6S00_MSG_6[]  = "M6S00Msg_6";
    constexpr char KEY_M6S00_MSG_7[]  = "M6S00Msg_7";
    constexpr char KEY_M6S00_MSG_8[]  = "M6S00Msg_8";
    constexpr char KEY_M6S00_MSG_9[]  = "M6S00Msg_9";
    constexpr char KEY_M6S00_MSG_10[] = "M6S00Msg_10";
    constexpr char KEY_M6S00_MSG_11[] = "M6S00Msg_11";
    constexpr char KEY_M6S00_MSG_12[] = "M6S00Msg_12";

    // =========================
    // Map 6, Stage 01 Messages
    // =========================

    constexpr char KEY_M6S01_MSG_1[]  = "M6S01Msg_1";
    constexpr char KEY_M6S01_MSG_2[]  = "M6S01Msg_2";
    constexpr char KEY_M6S01_MSG_3[]  = "M6S01Msg_3";
    constexpr char KEY_M6S01_MSG_4[]  = "M6S01Msg_4";
    constexpr char KEY_M6S01_MSG_5[]  = "M6S01Msg_5";
    constexpr char KEY_M6S01_MSG_6[]  = "M6S01Msg_6";
    constexpr char KEY_M6S01_MSG_7[]  = "M6S01Msg_7";
    constexpr char KEY_M6S01_MSG_8[]  = "M6S01Msg_8";
    constexpr char KEY_M6S01_MSG_9[]  = "M6S01Msg_9";
    constexpr char KEY_M6S01_MSG_10[] = "M6S01Msg_10";
    constexpr char KEY_M6S01_MSG_11[] = "M6S01Msg_11";
    constexpr char KEY_M6S01_MSG_12[] = "M6S01Msg_12";
    constexpr char KEY_M6S01_MSG_13[] = "M6S01Msg_13";
    constexpr char KEY_M6S01_MSG_14[] = "M6S01Msg_14";
    constexpr char KEY_M6S01_MSG_15[] = "M6S01Msg_15";
    constexpr char KEY_M6S01_MSG_16[] = "M6S01Msg_16";
    constexpr char KEY_M6S01_MSG_17[] = "M6S01Msg_17";
    constexpr char KEY_M6S01_MSG_18[] = "M6S01Msg_18";
    constexpr char KEY_M6S01_MSG_19[] = "M6S01Msg_19";
    constexpr char KEY_M6S01_MSG_20[] = "M6S01Msg_20";
    constexpr char KEY_M6S01_MSG_21[] = "M6S01Msg_21";
    constexpr char KEY_M6S01_MSG_22[] = "M6S01Msg_22";
    constexpr char KEY_M6S01_MSG_23[] = "M6S01Msg_23";
    constexpr char KEY_M6S01_MSG_24[] = "M6S01Msg_24";
    constexpr char KEY_M6S01_MSG_25[] = "M6S01Msg_25";
    constexpr char KEY_M6S01_MSG_26[] = "M6S01Msg_26";
    constexpr char KEY_M6S01_MSG_27[] = "M6S01Msg_27";
    constexpr char KEY_M6S01_MSG_28[] = "M6S01Msg_28";
    constexpr char KEY_M6S01_MSG_29[] = "M6S01Msg_29";
    constexpr char KEY_M6S01_MSG_30[] = "M6S01Msg_30";
    constexpr char KEY_M6S01_MSG_31[] = "M6S01Msg_31";
    constexpr char KEY_M6S01_MSG_32[] = "M6S01Msg_32";
    constexpr char KEY_M6S01_MSG_33[] = "M6S01Msg_33";
    constexpr char KEY_M6S01_MSG_34[] = "M6S01Msg_34";
    constexpr char KEY_M6S01_MSG_35[] = "M6S01Msg_35";
    constexpr char KEY_M6S01_MSG_36[] = "M6S01Msg_36";
    constexpr char KEY_M6S01_MSG_37[] = "M6S01Msg_37";
    constexpr char KEY_M6S01_MSG_38[] = "M6S01Msg_38";
    constexpr char KEY_M6S01_MSG_39[] = "M6S01Msg_39";
    constexpr char KEY_M6S01_MSG_40[] = "M6S01Msg_40";
    constexpr char KEY_M6S01_MSG_41[] = "M6S01Msg_41";
    constexpr char KEY_M6S01_MSG_42[] = "M6S01Msg_42";
    constexpr char KEY_M6S01_MSG_43[] = "M6S01Msg_43";
    constexpr char KEY_M6S01_MSG_44[] = "M6S01Msg_44";
    constexpr char KEY_M6S01_MSG_45[] = "M6S01Msg_45";
    constexpr char KEY_M6S01_MSG_46[] = "M6S01Msg_46";
    constexpr char KEY_M6S01_MSG_47[] = "M6S01Msg_47";
    constexpr char KEY_M6S01_MSG_48[] = "M6S01Msg_48";
    constexpr char KEY_M6S01_MSG_49[] = "M6S01Msg_49";
    constexpr char KEY_M6S01_MSG_50[] = "M6S01Msg_50";
    constexpr char KEY_M6S01_MSG_51[] = "M6S01Msg_51";
    constexpr char KEY_M6S01_MSG_52[] = "M6S01Msg_52";
    constexpr char KEY_M6S01_MSG_53[] = "M6S01Msg_53";
    constexpr char KEY_M6S01_MSG_54[] = "M6S01Msg_54";
    constexpr char KEY_M6S01_MSG_55[] = "M6S01Msg_55";
    constexpr char KEY_M6S01_MSG_56[] = "M6S01Msg_56";
    constexpr char KEY_M6S01_MSG_57[] = "M6S01Msg_57";
    constexpr char KEY_M6S01_MSG_58[] = "M6S01Msg_58";
    constexpr char KEY_M6S01_MSG_59[] = "M6S01Msg_59";
    constexpr char KEY_M6S01_MSG_60[] = "M6S01Msg_60";
    constexpr char KEY_M6S01_MSG_61[] = "M6S01Msg_61";
    constexpr char KEY_M6S01_MSG_62[] = "M6S01Msg_62";
    constexpr char KEY_M6S01_MSG_63[] = "M6S01Msg_63";
    constexpr char KEY_M6S01_MSG_64[] = "M6S01Msg_64";
    constexpr char KEY_M6S01_MSG_65[] = "M6S01Msg_65";
    constexpr char KEY_M6S01_MSG_66[] = "M6S01Msg_66";
    constexpr char KEY_M6S01_MSG_67[] = "M6S01Msg_67";
    constexpr char KEY_M6S01_MSG_68[] = "M6S01Msg_68";
    constexpr char KEY_M6S01_MSG_69[] = "M6S01Msg_69";
    constexpr char KEY_M6S01_MSG_70[] = "M6S01Msg_70";
    constexpr char KEY_M6S01_MSG_71[] = "M6S01Msg_71";
    constexpr char KEY_M6S01_MSG_72[] = "M6S01Msg_72";
    constexpr char KEY_M6S01_MSG_73[] = "M6S01Msg_73";
    constexpr char KEY_M6S01_MSG_74[] = "M6S01Msg_74";
    constexpr char KEY_M6S01_MSG_75[] = "M6S01Msg_75";
    constexpr char KEY_M6S01_MSG_76[] = "M6S01Msg_76";

    // =========================
    // Map 6, Stage 02 Messages
    // =========================

    constexpr char KEY_M6S02_MSG_1[]  = "M6S02Msg_1";
    constexpr char KEY_M6S02_MSG_2[]  = "M6S02Msg_2";
    constexpr char KEY_M6S02_MSG_3[]  = "M6S02Msg_3";
    constexpr char KEY_M6S02_MSG_4[]  = "M6S02Msg_4";
    constexpr char KEY_M6S02_MSG_5[]  = "M6S02Msg_5";
    constexpr char KEY_M6S02_MSG_6[]  = "M6S02Msg_6";
    constexpr char KEY_M6S02_MSG_7[]  = "M6S02Msg_7";
    constexpr char KEY_M6S02_MSG_8[]  = "M6S02Msg_8";
    constexpr char KEY_M6S02_MSG_9[]  = "M6S02Msg_9";
    constexpr char KEY_M6S02_MSG_10[] = "M6S02Msg_10";

    // =========================
    // Map 6, Stage 03 Messages
    // =========================

    constexpr char KEY_M6S03_MSG_1[] = "M6S03Msg_1";
    constexpr char KEY_M6S03_MSG_2[] = "M6S03Msg_2";

    // =========================
    // Map 6, Stage 04 Messages
    // =========================

    constexpr char KEY_M6S04_MSG_1[]  = "M6S04Msg_1";
    constexpr char KEY_M6S04_MSG_2[]  = "M6S04Msg_2";
    constexpr char KEY_M6S04_MSG_3[]  = "M6S04Msg_3";
    constexpr char KEY_M6S04_MSG_4[]  = "M6S04Msg_4";
    constexpr char KEY_M6S04_MSG_5[]  = "M6S04Msg_5";
    constexpr char KEY_M6S04_MSG_6[]  = "M6S04Msg_6";
    constexpr char KEY_M6S04_MSG_7[]  = "M6S04Msg_7";
    constexpr char KEY_M6S04_MSG_8[]  = "M6S04Msg_8";
    constexpr char KEY_M6S04_MSG_9[]  = "M6S04Msg_9";
    constexpr char KEY_M6S04_MSG_10[] = "M6S04Msg_10";
    constexpr char KEY_M6S04_MSG_11[] = "M6S04Msg_11";
    constexpr char KEY_M6S04_MSG_12[] = "M6S04Msg_12";
    constexpr char KEY_M6S04_MSG_13[] = "M6S04Msg_13";
    constexpr char KEY_M6S04_MSG_14[] = "M6S04Msg_14";
    constexpr char KEY_M6S04_MSG_15[] = "M6S04Msg_15";
    constexpr char KEY_M6S04_MSG_16[] = "M6S04Msg_16";
    constexpr char KEY_M6S04_MSG_17[] = "M6S04Msg_17";
    constexpr char KEY_M6S04_MSG_18[] = "M6S04Msg_18";
    constexpr char KEY_M6S04_MSG_19[] = "M6S04Msg_19";
    constexpr char KEY_M6S04_MSG_20[] = "M6S04Msg_20";
    constexpr char KEY_M6S04_MSG_21[] = "M6S04Msg_21";
    constexpr char KEY_M6S04_MSG_22[] = "M6S04Msg_22";
    constexpr char KEY_M6S04_MSG_23[] = "M6S04Msg_23";
    constexpr char KEY_M6S04_MSG_24[] = "M6S04Msg_24";
    constexpr char KEY_M6S04_MSG_25[] = "M6S04Msg_25";
    constexpr char KEY_M6S04_MSG_26[] = "M6S04Msg_26";
    constexpr char KEY_M6S04_MSG_27[] = "M6S04Msg_27";
    constexpr char KEY_M6S04_MSG_28[] = "M6S04Msg_28";
    constexpr char KEY_M6S04_MSG_29[] = "M6S04Msg_29";
    constexpr char KEY_M6S04_MSG_30[] = "M6S04Msg_30";
    constexpr char KEY_M6S04_MSG_31[] = "M6S04Msg_31";
    constexpr char KEY_M6S04_MSG_32[] = "M6S04Msg_32";
    constexpr char KEY_M6S04_MSG_33[] = "M6S04Msg_33";
    constexpr char KEY_M6S04_MSG_34[] = "M6S04Msg_34";
    constexpr char KEY_M6S04_MSG_35[] = "M6S04Msg_35";
    constexpr char KEY_M6S04_MSG_36[] = "M6S04Msg_36";
    constexpr char KEY_M6S04_MSG_37[] = "M6S04Msg_37";
    constexpr char KEY_M6S04_MSG_38[] = "M6S04Msg_38";
    constexpr char KEY_M6S04_MSG_39[] = "M6S04Msg_39";
    constexpr char KEY_M6S04_MSG_40[] = "M6S04Msg_40";
    constexpr char KEY_M6S04_MSG_41[] = "M6S04Msg_41";
    constexpr char KEY_M6S04_MSG_42[] = "M6S04Msg_42";
    constexpr char KEY_M6S04_MSG_43[] = "M6S04Msg_43";
    constexpr char KEY_M6S04_MSG_44[] = "M6S04Msg_44";
    constexpr char KEY_M6S04_MSG_45[] = "M6S04Msg_45";
    constexpr char KEY_M6S04_MSG_46[] = "M6S04Msg_46";
    constexpr char KEY_M6S04_MSG_47[] = "M6S04Msg_47";
    constexpr char KEY_M6S04_MSG_48[] = "M6S04Msg_48";
    constexpr char KEY_M6S04_MSG_49[] = "M6S04Msg_49";
    constexpr char KEY_M6S04_MSG_50[] = "M6S04Msg_50";
    constexpr char KEY_M6S04_MSG_51[] = "M6S04Msg_51";
    constexpr char KEY_M6S04_MSG_52[] = "M6S04Msg_52";
    constexpr char KEY_M6S04_MSG_53[] = "M6S04Msg_53";
    constexpr char KEY_M6S04_MSG_54[] = "M6S04Msg_54";
    constexpr char KEY_M6S04_MSG_55[] = "M6S04Msg_55";
    constexpr char KEY_M6S04_MSG_56[] = "M6S04Msg_56";
    constexpr char KEY_M6S04_MSG_57[] = "M6S04Msg_57";
    constexpr char KEY_M6S04_MSG_58[] = "M6S04Msg_58";
    constexpr char KEY_M6S04_MSG_59[] = "M6S04Msg_59";
    constexpr char KEY_M6S04_MSG_60[] = "M6S04Msg_60";
    constexpr char KEY_M6S04_MSG_61[] = "M6S04Msg_61";

    // =========================
    // Map 7, Stage 00 Messages
    // =========================

    constexpr char KEY_M7S00_MSG_1[]  = "M7S00Msg_1";
    constexpr char KEY_M7S00_MSG_2[]  = "M7S00Msg_2";
    constexpr char KEY_M7S00_MSG_3[]  = "M7S00Msg_3";
    constexpr char KEY_M7S00_MSG_4[]  = "M7S00Msg_4";
    constexpr char KEY_M7S00_MSG_5[]  = "M7S00Msg_5";
    constexpr char KEY_M7S00_MSG_6[]  = "M7S00Msg_6";
    constexpr char KEY_M7S00_MSG_7[]  = "M7S00Msg_7";
    constexpr char KEY_M7S00_MSG_8[]  = "M7S00Msg_8";
    constexpr char KEY_M7S00_MSG_9[]  = "M7S00Msg_9";
    constexpr char KEY_M7S00_MSG_10[] = "M7S00Msg_10";
    constexpr char KEY_M7S00_MSG_11[] = "M7S00Msg_11";
    constexpr char KEY_M7S00_MSG_12[] = "M7S00Msg_12";
    constexpr char KEY_M7S00_MSG_13[] = "M7S00Msg_13";
    constexpr char KEY_M7S00_MSG_14[] = "M7S00Msg_14";
    constexpr char KEY_M7S00_MSG_15[] = "M7S00Msg_15";
    constexpr char KEY_M7S00_MSG_16[] = "M7S00Msg_16";
    constexpr char KEY_M7S00_MSG_17[] = "M7S00Msg_17";
    constexpr char KEY_M7S00_MSG_18[] = "M7S00Msg_18";
    constexpr char KEY_M7S00_MSG_19[] = "M7S00Msg_19";
    constexpr char KEY_M7S00_MSG_20[] = "M7S00Msg_20";
    constexpr char KEY_M7S00_MSG_21[] = "M7S00Msg_21";
    constexpr char KEY_M7S00_MSG_22[] = "M7S00Msg_22";
    constexpr char KEY_M7S00_MSG_23[] = "M7S00Msg_23";
    constexpr char KEY_M7S00_MSG_24[] = "M7S00Msg_24";
    constexpr char KEY_M7S00_MSG_25[] = "M7S00Msg_25";
    constexpr char KEY_M7S00_MSG_26[] = "M7S00Msg_26";
    constexpr char KEY_M7S00_MSG_27[] = "M7S00Msg_27";
    constexpr char KEY_M7S00_MSG_28[] = "M7S00Msg_28";
    constexpr char KEY_M7S00_MSG_29[] = "M7S00Msg_29";
    constexpr char KEY_M7S00_MSG_30[] = "M7S00Msg_30";
    constexpr char KEY_M7S00_MSG_31[] = "M7S00Msg_31";
    constexpr char KEY_M7S00_MSG_32[] = "M7S00Msg_32";
    constexpr char KEY_M7S00_MSG_33[] = "M7S00Msg_33";
    constexpr char KEY_M7S00_MSG_34[] = "M7S00Msg_34";

    // =========================
    // Map 7, Stage 01 Messages
    // =========================

    constexpr char KEY_M7S01_MSG_1[]  = "M7S01Msg_1";
    constexpr char KEY_M7S01_MSG_2[]  = "M7S01Msg_2";
    constexpr char KEY_M7S01_MSG_3[]  = "M7S01Msg_3";
    constexpr char KEY_M7S01_MSG_4[]  = "M7S01Msg_4";
    constexpr char KEY_M7S01_MSG_5[]  = "M7S01Msg_5";
    constexpr char KEY_M7S01_MSG_6[]  = "M7S01Msg_6";
    constexpr char KEY_M7S01_MSG_7[]  = "M7S01Msg_7";
    constexpr char KEY_M7S01_MSG_8[]  = "M7S01Msg_8";
    constexpr char KEY_M7S01_MSG_9[]  = "M7S01Msg_9";
    constexpr char KEY_M7S01_MSG_10[] = "M7S01Msg_10";
    constexpr char KEY_M7S01_MSG_11[] = "M7S01Msg_11";
    constexpr char KEY_M7S01_MSG_12[] = "M7S01Msg_12";
    constexpr char KEY_M7S01_MSG_13[] = "M7S01Msg_13";
    constexpr char KEY_M7S01_MSG_14[] = "M7S01Msg_14";
    constexpr char KEY_M7S01_MSG_15[] = "M7S01Msg_15";
    constexpr char KEY_M7S01_MSG_16[] = "M7S01Msg_16";
    constexpr char KEY_M7S01_MSG_17[] = "M7S01Msg_17";
    constexpr char KEY_M7S01_MSG_18[] = "M7S01Msg_18";
    constexpr char KEY_M7S01_MSG_19[] = "M7S01Msg_19";
    constexpr char KEY_M7S01_MSG_20[] = "M7S01Msg_20";
    constexpr char KEY_M7S01_MSG_21[] = "M7S01Msg_21";
    constexpr char KEY_M7S01_MSG_22[] = "M7S01Msg_22";
    constexpr char KEY_M7S01_MSG_23[] = "M7S01Msg_23";
    constexpr char KEY_M7S01_MSG_24[] = "M7S01Msg_24";
    constexpr char KEY_M7S01_MSG_25[] = "M7S01Msg_25";
    constexpr char KEY_M7S01_MSG_26[] = "M7S01Msg_26";
    constexpr char KEY_M7S01_MSG_27[] = "M7S01Msg_27";
    constexpr char KEY_M7S01_MSG_28[] = "M7S01Msg_28";
    constexpr char KEY_M7S01_MSG_29[] = "M7S01Msg_29";
    constexpr char KEY_M7S01_MSG_30[] = "M7S01Msg_30";
    constexpr char KEY_M7S01_MSG_31[] = "M7S01Msg_31";
    constexpr char KEY_M7S01_MSG_32[] = "M7S01Msg_32";
    constexpr char KEY_M7S01_MSG_33[] = "M7S01Msg_33";
    constexpr char KEY_M7S01_MSG_34[] = "M7S01Msg_34";
    constexpr char KEY_M7S01_MSG_35[] = "M7S01Msg_35";
    constexpr char KEY_M7S01_MSG_36[] = "M7S01Msg_36";
    constexpr char KEY_M7S01_MSG_37[] = "M7S01Msg_37";
    constexpr char KEY_M7S01_MSG_38[] = "M7S01Msg_38";
    constexpr char KEY_M7S01_MSG_39[] = "M7S01Msg_39";
    constexpr char KEY_M7S01_MSG_40[] = "M7S01Msg_40";
    constexpr char KEY_M7S01_MSG_41[] = "M7S01Msg_41";
    constexpr char KEY_M7S01_MSG_42[] = "M7S01Msg_42";
    constexpr char KEY_M7S01_MSG_43[] = "M7S01Msg_43";
    constexpr char KEY_M7S01_MSG_44[] = "M7S01Msg_44";
    constexpr char KEY_M7S01_MSG_45[] = "M7S01Msg_45";
    constexpr char KEY_M7S01_MSG_46[] = "M7S01Msg_46";
    constexpr char KEY_M7S01_MSG_47[] = "M7S01Msg_47";
    constexpr char KEY_M7S01_MSG_48[] = "M7S01Msg_48";
    constexpr char KEY_M7S01_MSG_49[] = "M7S01Msg_49";
    constexpr char KEY_M7S01_MSG_50[] = "M7S01Msg_50";
    constexpr char KEY_M7S01_MSG_51[] = "M7S01Msg_51";
    constexpr char KEY_M7S01_MSG_52[] = "M7S01Msg_52";
    constexpr char KEY_M7S01_MSG_53[] = "M7S01Msg_53";
    constexpr char KEY_M7S01_MSG_54[] = "M7S01Msg_54";
    constexpr char KEY_M7S01_MSG_55[] = "M7S01Msg_55";
    constexpr char KEY_M7S01_MSG_56[] = "M7S01Msg_56";
    constexpr char KEY_M7S01_MSG_57[] = "M7S01Msg_57";
    constexpr char KEY_M7S01_MSG_58[] = "M7S01Msg_58";
    constexpr char KEY_M7S01_MSG_59[] = "M7S01Msg_59";
    constexpr char KEY_M7S01_MSG_60[] = "M7S01Msg_60";
    constexpr char KEY_M7S01_MSG_61[] = "M7S01Msg_61";
    constexpr char KEY_M7S01_MSG_62[] = "M7S01Msg_62";
    constexpr char KEY_M7S01_MSG_63[] = "M7S01Msg_63";
    constexpr char KEY_M7S01_MSG_64[] = "M7S01Msg_64";
    constexpr char KEY_M7S01_MSG_65[] = "M7S01Msg_65";
    constexpr char KEY_M7S01_MSG_66[] = "M7S01Msg_66";
    constexpr char KEY_M7S01_MSG_67[] = "M7S01Msg_67";
    constexpr char KEY_M7S01_MSG_68[] = "M7S01Msg_68";
    constexpr char KEY_M7S01_MSG_69[] = "M7S01Msg_69";
    constexpr char KEY_M7S01_MSG_70[] = "M7S01Msg_70";
    constexpr char KEY_M7S01_MSG_71[] = "M7S01Msg_71";
    constexpr char KEY_M7S01_MSG_72[] = "M7S01Msg_72";
    constexpr char KEY_M7S01_MSG_73[] = "M7S01Msg_73";
    constexpr char KEY_M7S01_MSG_74[] = "M7S01Msg_74";
    constexpr char KEY_M7S01_MSG_75[] = "M7S01Msg_75";
    constexpr char KEY_M7S01_MSG_76[] = "M7S01Msg_76";
    constexpr char KEY_M7S01_MSG_77[] = "M7S01Msg_77";
    constexpr char KEY_M7S01_MSG_78[] = "M7S01Msg_78";
    constexpr char KEY_M7S01_MSG_79[] = "M7S01Msg_79";
    constexpr char KEY_M7S01_MSG_80[] = "M7S01Msg_80";
    constexpr char KEY_M7S01_MSG_81[] = "M7S01Msg_81";
    constexpr char KEY_M7S01_MSG_82[] = "M7S01Msg_82";

    // =========================
    // Map 7, Stage 02 Messages
    // =========================

    constexpr char KEY_M7S02_MSG_1[]   = "M7S02Msg_1";
    constexpr char KEY_M7S02_MSG_2[]   = "M7S02Msg_2";
    constexpr char KEY_M7S02_MSG_3[]   = "M7S02Msg_3";
    constexpr char KEY_M7S02_MSG_4[]   = "M7S02Msg_4";
    constexpr char KEY_M7S02_MSG_5[]   = "M7S02Msg_5";
    constexpr char KEY_M7S02_MSG_6[]   = "M7S02Msg_6";
    constexpr char KEY_M7S02_MSG_7[]   = "M7S02Msg_7";
    constexpr char KEY_M7S02_MSG_8[]   = "M7S02Msg_8";
    constexpr char KEY_M7S02_MSG_9[]   = "M7S02Msg_9";
    constexpr char KEY_M7S02_MSG_10[]  = "M7S02Msg_10";
    constexpr char KEY_M7S02_MSG_11[]  = "M7S02Msg_11";
    constexpr char KEY_M7S02_MSG_12[]  = "M7S02Msg_12";
    constexpr char KEY_M7S02_MSG_13[]  = "M7S02Msg_13";
    constexpr char KEY_M7S02_MSG_14[]  = "M7S02Msg_14";
    constexpr char KEY_M7S02_MSG_15[]  = "M7S02Msg_15";
    constexpr char KEY_M7S02_MSG_16[]  = "M7S02Msg_16";
    constexpr char KEY_M7S02_MSG_17[]  = "M7S02Msg_17";
    constexpr char KEY_M7S02_MSG_18[]  = "M7S02Msg_18";
    constexpr char KEY_M7S02_MSG_19[]  = "M7S02Msg_19";
    constexpr char KEY_M7S02_MSG_20[]  = "M7S02Msg_20";
    constexpr char KEY_M7S02_MSG_21[]  = "M7S02Msg_21";
    constexpr char KEY_M7S02_MSG_22[]  = "M7S02Msg_22";
    constexpr char KEY_M7S02_MSG_23[]  = "M7S02Msg_23";
    constexpr char KEY_M7S02_MSG_24[]  = "M7S02Msg_24";
    constexpr char KEY_M7S02_MSG_25[]  = "M7S02Msg_25";
    constexpr char KEY_M7S02_MSG_26[]  = "M7S02Msg_26";
    constexpr char KEY_M7S02_MSG_27[]  = "M7S02Msg_27";
    constexpr char KEY_M7S02_MSG_28[]  = "M7S02Msg_28";
    constexpr char KEY_M7S02_MSG_29[]  = "M7S02Msg_29";
    constexpr char KEY_M7S02_MSG_30[]  = "M7S02Msg_30";
    constexpr char KEY_M7S02_MSG_31[]  = "M7S02Msg_31";
    constexpr char KEY_M7S02_MSG_32[]  = "M7S02Msg_32";
    constexpr char KEY_M7S02_MSG_33[]  = "M7S02Msg_33";
    constexpr char KEY_M7S02_MSG_34[]  = "M7S02Msg_34";
    constexpr char KEY_M7S02_MSG_35[]  = "M7S02Msg_35";
    constexpr char KEY_M7S02_MSG_36[]  = "M7S02Msg_36";
    constexpr char KEY_M7S02_MSG_37[]  = "M7S02Msg_37";
    constexpr char KEY_M7S02_MSG_38[]  = "M7S02Msg_38";
    constexpr char KEY_M7S02_MSG_39[]  = "M7S02Msg_39";
    constexpr char KEY_M7S02_MSG_40[]  = "M7S02Msg_40";
    constexpr char KEY_M7S02_MSG_41[]  = "M7S02Msg_41";
    constexpr char KEY_M7S02_MSG_42[]  = "M7S02Msg_42";
    constexpr char KEY_M7S02_MSG_43[]  = "M7S02Msg_43";
    constexpr char KEY_M7S02_MSG_44[]  = "M7S02Msg_44";
    constexpr char KEY_M7S02_MSG_45[]  = "M7S02Msg_45";
    constexpr char KEY_M7S02_MSG_46[]  = "M7S02Msg_46";
    constexpr char KEY_M7S02_MSG_47[]  = "M7S02Msg_47";
    constexpr char KEY_M7S02_MSG_48[]  = "M7S02Msg_48";
    constexpr char KEY_M7S02_MSG_49[]  = "M7S02Msg_49";
    constexpr char KEY_M7S02_MSG_50[]  = "M7S02Msg_50";
    constexpr char KEY_M7S02_MSG_51[]  = "M7S02Msg_51";
    constexpr char KEY_M7S02_MSG_52[]  = "M7S02Msg_52";
    constexpr char KEY_M7S02_MSG_53[]  = "M7S02Msg_53";
    constexpr char KEY_M7S02_MSG_54[]  = "M7S02Msg_54";
    constexpr char KEY_M7S02_MSG_55[]  = "M7S02Msg_55";
    constexpr char KEY_M7S02_MSG_56[]  = "M7S02Msg_56";
    constexpr char KEY_M7S02_MSG_57[]  = "M7S02Msg_57";
    constexpr char KEY_M7S02_MSG_58[]  = "M7S02Msg_58";
    constexpr char KEY_M7S02_MSG_59[]  = "M7S02Msg_59";
    constexpr char KEY_M7S02_MSG_60[]  = "M7S02Msg_60";
    constexpr char KEY_M7S02_MSG_61[]  = "M7S02Msg_61";
    constexpr char KEY_M7S02_MSG_62[]  = "M7S02Msg_62";
    constexpr char KEY_M7S02_MSG_63[]  = "M7S02Msg_63";
    constexpr char KEY_M7S02_MSG_64[]  = "M7S02Msg_64";
    constexpr char KEY_M7S02_MSG_65[]  = "M7S02Msg_65";
    constexpr char KEY_M7S02_MSG_66[]  = "M7S02Msg_66";
    constexpr char KEY_M7S02_MSG_67[]  = "M7S02Msg_67";
    constexpr char KEY_M7S02_MSG_68[]  = "M7S02Msg_68";
    constexpr char KEY_M7S02_MSG_69[]  = "M7S02Msg_69";
    constexpr char KEY_M7S02_MSG_70[]  = "M7S02Msg_70";
    constexpr char KEY_M7S02_MSG_71[]  = "M7S02Msg_71";
    constexpr char KEY_M7S02_MSG_72[]  = "M7S02Msg_72";
    constexpr char KEY_M7S02_MSG_73[]  = "M7S02Msg_73";
    constexpr char KEY_M7S02_MSG_74[]  = "M7S02Msg_74";
    constexpr char KEY_M7S02_MSG_75[]  = "M7S02Msg_75";
    constexpr char KEY_M7S02_MSG_76[]  = "M7S02Msg_76";
    constexpr char KEY_M7S02_MSG_77[]  = "M7S02Msg_77";
    constexpr char KEY_M7S02_MSG_78[]  = "M7S02Msg_78";
    constexpr char KEY_M7S02_MSG_79[]  = "M7S02Msg_79";
    constexpr char KEY_M7S02_MSG_80[]  = "M7S02Msg_80";
    constexpr char KEY_M7S02_MSG_81[]  = "M7S02Msg_81";
    constexpr char KEY_M7S02_MSG_82[]  = "M7S02Msg_82";
    constexpr char KEY_M7S02_MSG_83[]  = "M7S02Msg_83";
    constexpr char KEY_M7S02_MSG_84[]  = "M7S02Msg_84";
    constexpr char KEY_M7S02_MSG_85[]  = "M7S02Msg_85";
    constexpr char KEY_M7S02_MSG_86[]  = "M7S02Msg_86";
    constexpr char KEY_M7S02_MSG_87[]  = "M7S02Msg_87";
    constexpr char KEY_M7S02_MSG_88[]  = "M7S02Msg_88";
    constexpr char KEY_M7S02_MSG_89[]  = "M7S02Msg_89";
    constexpr char KEY_M7S02_MSG_90[]  = "M7S02Msg_90";
    constexpr char KEY_M7S02_MSG_91[]  = "M7S02Msg_91";
    constexpr char KEY_M7S02_MSG_92[]  = "M7S02Msg_92";
    constexpr char KEY_M7S02_MSG_93[]  = "M7S02Msg_93";
    constexpr char KEY_M7S02_MSG_94[]  = "M7S02Msg_94";
    constexpr char KEY_M7S02_MSG_95[]  = "M7S02Msg_95";
    constexpr char KEY_M7S02_MSG_96[]  = "M7S02Msg_96";
    constexpr char KEY_M7S02_MSG_97[]  = "M7S02Msg_97";
    constexpr char KEY_M7S02_MSG_98[]  = "M7S02Msg_98";
    constexpr char KEY_M7S02_MSG_99[]  = "M7S02Msg_99";
    constexpr char KEY_M7S02_MSG_100[] = "M7S02Msg_100";
    constexpr char KEY_M7S02_MSG_101[] = "M7S02Msg_101";
    constexpr char KEY_M7S02_MSG_102[] = "M7S02Msg_102";
    constexpr char KEY_M7S02_MSG_103[] = "M7S02Msg_103";
    constexpr char KEY_M7S02_MSG_104[] = "M7S02Msg_104";
    constexpr char KEY_M7S02_MSG_105[] = "M7S02Msg_105";
    constexpr char KEY_M7S02_MSG_106[] = "M7S02Msg_106";
    constexpr char KEY_M7S02_MSG_107[] = "M7S02Msg_107";
    constexpr char KEY_M7S02_MSG_108[] = "M7S02Msg_108";
    constexpr char KEY_M7S02_MSG_109[] = "M7S02Msg_109";
    constexpr char KEY_M7S02_MSG_110[] = "M7S02Msg_110";
    constexpr char KEY_M7S02_MSG_111[] = "M7S02Msg_111";
    constexpr char KEY_M7S02_MSG_112[] = "M7S02Msg_112";
    constexpr char KEY_M7S02_MSG_113[] = "M7S02Msg_113";
    constexpr char KEY_M7S02_MSG_114[] = "M7S02Msg_114";
    constexpr char KEY_M7S02_MSG_115[] = "M7S02Msg_115";
    constexpr char KEY_M7S02_MSG_116[] = "M7S02Msg_116";
    constexpr char KEY_M7S02_MSG_117[] = "M7S02Msg_117";
    constexpr char KEY_M7S02_MSG_118[] = "M7S02Msg_118";
    constexpr char KEY_M7S02_MSG_119[] = "M7S02Msg_119";
    constexpr char KEY_M7S02_MSG_120[] = "M7S02Msg_120";
    constexpr char KEY_M7S02_MSG_121[] = "M7S02Msg_121";
    constexpr char KEY_M7S02_MSG_122[] = "M7S02Msg_122";
    constexpr char KEY_M7S02_MSG_123[] = "M7S02Msg_123";
    constexpr char KEY_M7S02_MSG_124[] = "M7S02Msg_124";
    constexpr char KEY_M7S02_MSG_125[] = "M7S02Msg_125";
    constexpr char KEY_M7S02_MSG_126[] = "M7S02Msg_126";
    constexpr char KEY_M7S02_MSG_127[] = "M7S02Msg_127";
    constexpr char KEY_M7S02_MSG_128[] = "M7S02Msg_128";
    constexpr char KEY_M7S02_MSG_129[] = "M7S02Msg_129";
    constexpr char KEY_M7S02_MSG_130[] = "M7S02Msg_130";
    constexpr char KEY_M7S02_MSG_131[] = "M7S02Msg_131";
    constexpr char KEY_M7S02_MSG_132[] = "M7S02Msg_132";
    constexpr char KEY_M7S02_MSG_133[] = "M7S02Msg_133";
    constexpr char KEY_M7S02_MSG_134[] = "M7S02Msg_134";
    constexpr char KEY_M7S02_MSG_135[] = "M7S02Msg_135";
    constexpr char KEY_M7S02_MSG_136[] = "M7S02Msg_136";
    constexpr char KEY_M7S02_MSG_137[] = "M7S02Msg_137";
    constexpr char KEY_M7S02_MSG_138[] = "M7S02Msg_138";
    constexpr char KEY_M7S02_MSG_139[] = "M7S02Msg_139";
    constexpr char KEY_M7S02_MSG_140[] = "M7S02Msg_140";
    constexpr char KEY_M7S02_MSG_141[] = "M7S02Msg_141";
    constexpr char KEY_M7S02_MSG_142[] = "M7S02Msg_142";
    constexpr char KEY_M7S02_MSG_143[] = "M7S02Msg_143";
    constexpr char KEY_M7S02_MSG_144[] = "M7S02Msg_144";

    // =========================
    // Map 7, Stage 03 Messages
    // =========================

    constexpr char KEY_M7S03_MSG_1[]  = "M7S03Msg_1";
    constexpr char KEY_M7S03_MSG_2[]  = "M7S03Msg_2";
    constexpr char KEY_M7S03_MSG_3[]  = "M7S03Msg_3";
    constexpr char KEY_M7S03_MSG_4[]  = "M7S03Msg_4";
    constexpr char KEY_M7S03_MSG_5[]  = "M7S03Msg_5";
    constexpr char KEY_M7S03_MSG_6[]  = "M7S03Msg_6";
    constexpr char KEY_M7S03_MSG_7[]  = "M7S03Msg_7";
    constexpr char KEY_M7S03_MSG_8[]  = "M7S03Msg_8";
    constexpr char KEY_M7S03_MSG_9[]  = "M7S03Msg_9";
    constexpr char KEY_M7S03_MSG_11[] = "M7S03Msg_11";
    constexpr char KEY_M7S03_MSG_12[] = "M7S03Msg_12";
    constexpr char KEY_M7S03_MSG_13[] = "M7S03Msg_13";
    constexpr char KEY_M7S03_MSG_14[] = "M7S03Msg_14";
    constexpr char KEY_M7S03_MSG_15[] = "M7S03Msg_15";
    constexpr char KEY_M7S03_MSG_16[] = "M7S03Msg_16";
    constexpr char KEY_M7S03_MSG_17[] = "M7S03Msg_17";
    constexpr char KEY_M7S03_MSG_18[] = "M7S03Msg_18";
    constexpr char KEY_M7S03_MSG_19[] = "M7S03Msg_19";
    constexpr char KEY_M7S03_MSG_20[] = "M7S03Msg_20";
    constexpr char KEY_M7S03_MSG_21[] = "M7S03Msg_21";
    constexpr char KEY_M7S03_MSG_22[] = "M7S03Msg_22";
    constexpr char KEY_M7S03_MSG_23[] = "M7S03Msg_23";
    constexpr char KEY_M7S03_MSG_24[] = "M7S03Msg_24";
    constexpr char KEY_M7S03_MSG_25[] = "M7S03Msg_25";
    constexpr char KEY_M7S03_MSG_26[] = "M7S03Msg_26";
    constexpr char KEY_M7S03_MSG_27[] = "M7S03Msg_27";
    constexpr char KEY_M7S03_MSG_28[] = "M7S03Msg_28";
    constexpr char KEY_M7S03_MSG_29[] = "M7S03Msg_29";
    constexpr char KEY_M7S03_MSG_30[] = "M7S03Msg_30";
    constexpr char KEY_M7S03_MSG_31[] = "M7S03Msg_31";
    constexpr char KEY_M7S03_MSG_32[] = "M7S03Msg_32";
    constexpr char KEY_M7S03_MSG_33[] = "M7S03Msg_33";
    constexpr char KEY_M7S03_MSG_34[] = "M7S03Msg_34";
    constexpr char KEY_M7S03_MSG_35[] = "M7S03Msg_35";
    constexpr char KEY_M7S03_MSG_36[] = "M7S03Msg_36";
    constexpr char KEY_M7S03_MSG_37[] = "M7S03Msg_37";
    constexpr char KEY_M7S03_MSG_38[] = "M7S03Msg_38";
    constexpr char KEY_M7S03_MSG_39[] = "M7S03Msg_39";
    constexpr char KEY_M7S03_MSG_40[] = "M7S03Msg_40";
    constexpr char KEY_M7S03_MSG_41[] = "M7S03Msg_41";
    constexpr char KEY_M7S03_MSG_42[] = "M7S03Msg_42";
    constexpr char KEY_M7S03_MSG_43[] = "M7S03Msg_43";
    constexpr char KEY_M7S03_MSG_44[] = "M7S03Msg_44";
    constexpr char KEY_M7S03_MSG_45[] = "M7S03Msg_45";
    constexpr char KEY_M7S03_MSG_46[] = "M7S03Msg_46";
    constexpr char KEY_M7S03_MSG_47[] = "M7S03Msg_47";
    constexpr char KEY_M7S03_MSG_48[] = "M7S03Msg_48";
    constexpr char KEY_M7S03_MSG_49[] = "M7S03Msg_49";
    constexpr char KEY_M7S03_MSG_50[] = "M7S03Msg_50";
    constexpr char KEY_M7S03_MSG_51[] = "M7S03Msg_51";
    constexpr char KEY_M7S03_MSG_52[] = "M7S03Msg_52";
    constexpr char KEY_M7S03_MSG_53[] = "M7S03Msg_53";
    constexpr char KEY_M7S03_MSG_54[] = "M7S03Msg_54";
    constexpr char KEY_M7S03_MSG_55[] = "M7S03Msg_55";
    constexpr char KEY_M7S03_MSG_56[] = "M7S03Msg_56";
    constexpr char KEY_M7S03_MSG_57[] = "M7S03Msg_57";
    constexpr char KEY_M7S03_MSG_58[] = "M7S03Msg_58";
    constexpr char KEY_M7S03_MSG_59[] = "M7S03Msg_59";
    constexpr char KEY_M7S03_MSG_60[] = "M7S03Msg_60";
    constexpr char KEY_M7S03_MSG_61[] = "M7S03Msg_61";
    constexpr char KEY_M7S03_MSG_62[] = "M7S03Msg_62";
    constexpr char KEY_M7S03_MSG_63[] = "M7S03Msg_63";
    constexpr char KEY_M7S03_MSG_64[] = "M7S03Msg_64";
    constexpr char KEY_M7S03_MSG_65[] = "M7S03Msg_65";
    constexpr char KEY_M7S03_MSG_66[] = "M7S03Msg_66";
    constexpr char KEY_M7S03_MSG_67[] = "M7S03Msg_67";
    constexpr char KEY_M7S03_MSG_68[] = "M7S03Msg_68";
    constexpr char KEY_M7S03_MSG_69[] = "M7S03Msg_69";
    constexpr char KEY_M7S03_MSG_70[] = "M7S03Msg_70";
    constexpr char KEY_M7S03_MSG_71[] = "M7S03Msg_71";
    constexpr char KEY_M7S03_MSG_72[] = "M7S03Msg_72";
    constexpr char KEY_M7S03_MSG_73[] = "M7S03Msg_73";
    constexpr char KEY_M7S03_MSG_74[] = "M7S03Msg_74";
    constexpr char KEY_M7S03_MSG_75[] = "M7S03Msg_75";
    constexpr char KEY_M7S03_MSG_76[] = "M7S03Msg_76";
    constexpr char KEY_M7S03_MSG_77[] = "M7S03Msg_77";
    constexpr char KEY_M7S03_MSG_78[] = "M7S03Msg_78";
    constexpr char KEY_M7S03_MSG_79[] = "M7S03Msg_79";
    constexpr char KEY_M7S03_MSG_80[] = "M7S03Msg_80";
    constexpr char KEY_M7S03_MSG_81[] = "M7S03Msg_81";
    constexpr char KEY_M7S03_MSG_82[] = "M7S03Msg_82";
    constexpr char KEY_M7S03_MSG_83[] = "M7S03Msg_83";
    constexpr char KEY_M7S03_MSG_84[] = "M7S03Msg_84";
    constexpr char KEY_M7S03_MSG_85[] = "M7S03Msg_85";
}
