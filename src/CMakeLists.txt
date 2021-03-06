cmake_minimum_required(VERSION 3.13)
project(src)

set(CMAKE_CXX_STANDARD 14)

include_directories(cdx)
include_directories(ConvertFRUA)
include_directories(FRUAExport)
include_directories(FRUAExport/jsoncpp-src-0.5.0/include)
include_directories(FRUAExport/jsoncpp-src-0.5.0/include/json)
include_directories(FRUAExport/jsoncpp-src-0.5.0/src/lib_json)
include_directories(FRUAExport/jsoncpp-src-0.5.0/src/test_lib_json)
include_directories(Launch)
include_directories(Shared)
include_directories(UAFWin)
include_directories(UAFWinEd)

add_executable(src
        cdx/cdx.h
        cdx/cdxclist.h
        cdx/cdxdebug.cpp
        cdx/cdxdebug.h
        cdx/cdxfile.cpp
        cdx/cdxfile.h
        cdx/cdximage.cpp
        cdx/cdximage.h
        cdx/cdximagebase.cpp
        cdx/cdximagebase.h
        cdx/cdximagebmp.cpp
        cdx/cdximagebmp.h
        cdx/cdximagejpg.cpp
        cdx/cdximagejpg.h
        cdx/cdximagepcx.cpp
        cdx/cdximagepcx.h
        cdx/cdximagepng.cpp
        cdx/cdximagepng.h
        cdx/cdximagepsd.cpp
        cdx/cdximagepsd.h
        cdx/cdximagetga.cpp
        cdx/cdximagetga.h
        cdx/Cdxinput.cpp
        cdx/cdxinput.h
        cdx/cdxiso.cpp
        cdx/cdxlayer.cpp
        cdx/cdxlayer.h
        cdx/cdxlink.cpp
        cdx/cdxlink.h
        cdx/cdxlog.cpp
        cdx/cdxlog.h
        cdx/cdxmap.cpp
        cdx/cdxmap.h
        cdx/cdxmapcell.cpp
        cdx/cdxmidi.cpp
        cdx/cdxmidi.h
        cdx/Cdxmusic.cpp
        cdx/cdxmusic.h
        cdx/cdxmusiccd.cpp
        cdx/cdxpacker.cpp
        cdx/cdxpacker.h
        cdx/cdxresource.cpp
        cdx/cdxresource.h
        cdx/cdxscreen.cpp
        cdx/cdxscreen.h
        cdx/cdxsound.cpp
        cdx/Cdxsound.h
        cdx/cdxsoundbuffer.cpp
        cdx/cdxsprite.cpp
        cdx/cdxsprite.h
        cdx/cdxspritelist.cpp
        cdx/cdxstructs.h
        cdx/cdxsurface.cpp
        cdx/cdxsurface.h
        cdx/cdxtemplate.cpp
        cdx/cdxtemplate.h
        cdx/Cdxtile.cpp
        cdx/cdxtile.h
        cdx/cdxtypes.h
        cdx/ddutils.cpp
        cdx/dxerrors.cpp
        ConvertFRUA/Class.cpp
        ConvertFRUA/ConvertFRUA.cpp
        ConvertFRUA/ConvertFRUA.h
        ConvertFRUA/Event_AddNPC.cpp
        ConvertFRUA/Event_Camp.cpp
        ConvertFRUA/Event_Chain.cpp
        ConvertFRUA/Event_Combat.cpp
        ConvertFRUA/Event_CombatTreasure.cpp
        ConvertFRUA/Event_Encounter.cpp
        ConvertFRUA/Event_GainExperience.cpp
        ConvertFRUA/Event_GiveDamage.cpp
        ConvertFRUA/Event_GiveTreasure.cpp
        ConvertFRUA/Event_GuidedTour.cpp
        ConvertFRUA/Event_HealParty.cpp
        ConvertFRUA/Event_Journal.cpp
        ConvertFRUA/Event_Monster.cpp
        ConvertFRUA/Event_NPCSays.cpp
        ConvertFRUA/Event_Null.cpp
        ConvertFRUA/Event_PassTime.cpp
        ConvertFRUA/Event_PAssword.cpp
        ConvertFRUA/Event_Quest.cpp
        ConvertFRUA/Event_QuestionButton.cpp
        ConvertFRUA/Event_QuestionList.cpp
        ConvertFRUA/Event_QuestionYesNo.cpp
        ConvertFRUA/Event_Random.cpp
        ConvertFRUA/Event_RemoveNPC.cpp
        ConvertFRUA/Event_Rest.cpp
        ConvertFRUA/Event_Shop.cpp
        ConvertFRUA/Event_SmallTown.cpp
        ConvertFRUA/Event_Sound.cpp
        ConvertFRUA/Event_SpecialItems.cpp
        ConvertFRUA/Event_TakePArtyItems.cpp
        ConvertFRUA/Event_Tavern.cpp
        ConvertFRUA/Event_TavernTales.cpp
        ConvertFRUA/Event_Teleporter.cpp
        ConvertFRUA/Event_Temple.cpp
        ConvertFRUA/Event_TextStatement.cpp
        ConvertFRUA/Event_Tour.cpp
        ConvertFRUA/Event_TrainingHall.cpp
        ConvertFRUA/Event_Utilities.cpp
        ConvertFRUA/Event_Vault.cpp
        ConvertFRUA/Event_WhoPays.cpp
        ConvertFRUA/Event_WhoTries.cpp
        ConvertFRUA/Events.h
        ConvertFRUA/Globals.cpp
        ConvertFRUA/Items.cpp
        ConvertFRUA/J.cpp
        ConvertFRUA/J.h
        ConvertFRUA/Levels.cpp
        ConvertFRUA/MapGrid.cpp
        ConvertFRUA/Miscellaneous.cpp
        ConvertFRUA/Monsters.cpp
        ConvertFRUA/Resource.h
        ConvertFRUA/Spells.cpp
        ConvertFRUA/StdAfx.cpp
        ConvertFRUA/StdAfx.h
        ConvertFRUA/StepEvents.cpp
        ConvertFRUA/TransferModule.cpp
        ConvertFRUA/Translate.h
        FRUAExport/jsoncpp-src-0.5.0/include/json/autolink.h
        FRUAExport/jsoncpp-src-0.5.0/include/json/config.h
        FRUAExport/jsoncpp-src-0.5.0/include/json/features.h
        FRUAExport/jsoncpp-src-0.5.0/include/json/forwards.h
        FRUAExport/jsoncpp-src-0.5.0/include/json/json.h
        FRUAExport/jsoncpp-src-0.5.0/include/json/reader.h
        FRUAExport/jsoncpp-src-0.5.0/include/json/value.h
        FRUAExport/jsoncpp-src-0.5.0/include/json/writer.h
        FRUAExport/jsoncpp-src-0.5.0/src/jsontestrunner/main.cpp
        FRUAExport/jsoncpp-src-0.5.0/src/lib_json/json_batchallocator.h
        FRUAExport/jsoncpp-src-0.5.0/src/lib_json/json_internalarray.inl
        FRUAExport/jsoncpp-src-0.5.0/src/lib_json/json_internalmap.inl
        FRUAExport/jsoncpp-src-0.5.0/src/lib_json/json_reader.cpp
        FRUAExport/jsoncpp-src-0.5.0/src/lib_json/json_value.cpp
        FRUAExport/jsoncpp-src-0.5.0/src/lib_json/json_valueiterator.inl
        FRUAExport/jsoncpp-src-0.5.0/src/lib_json/json_writer.cpp
        FRUAExport/jsoncpp-src-0.5.0/src/test_lib_json/jsontest.cpp
        FRUAExport/jsoncpp-src-0.5.0/src/test_lib_json/jsontest.h
        FRUAExport/jsoncpp-src-0.5.0/src/test_lib_json/main.cpp
        FRUAExport/autolink.h
        FRUAExport/config.h
        FRUAExport/features.h
        FRUAExport/forwards.h
        FRUAExport/FRUAArtFiles.cpp
        FRUAExport/FRUAArtFiles.h
        FRUAExport/FRUAExport.cpp
        FRUAExport/FRUAGameDat.cpp
        FRUAExport/FRUAGameDat.h
        FRUAExport/FRUAItems.cpp
        FRUAExport/FRUAItems.h
        FRUAExport/FRUALevels.cpp
        FRUAExport/FRUALevels.h
        FRUAExport/FRUAMonster.cpp
        FRUAExport/FRUAMonster.h
        FRUAExport/FRUASoundFiles.cpp
        FRUAExport/FRUASoundFiles.h
        FRUAExport/FRUASpells.cpp
        FRUAExport/FRUASpells.h
        FRUAExport/FRUAStringTables.cpp
        FRUAExport/FRUAStringTables.h
        FRUAExport/FRUAStructs.h
        FRUAExport/FRUAVocab.cpp
        FRUAExport/FRUAVocab.h
        FRUAExport/globals.cpp
        FRUAExport/globals.h
        FRUAExport/json.h
        FRUAExport/reader.h
        FRUAExport/stdafx.cpp
        FRUAExport/stdafx.h
        FRUAExport/Utilities.cpp
        FRUAExport/Utilities.h
        FRUAExport/value.h
        FRUAExport/writer.h
        gpdl/GPDL.cpp
        Launch/Launch.cpp
        Launch/Launch.h
        Launch/LaunchDlg.cpp
        Launch/LaunchDlg.h
        Launch/resource.h
        Shared/ASL.cpp
        Shared/ASL.h
        Shared/bass.h
        Shared/Char.cpp
        Shared/Char.h
        Shared/class.cpp
        Shared/class.h
        Shared/ConfigFile.cpp
        Shared/ConfigFile.h
        Shared/Externs.h
        Shared/FileParse.cpp
        Shared/FileParse.h
        Shared/GameEvent.cpp
        Shared/GameEvent.h
        Shared/GameRules.cpp
        Shared/GameRules.h
        Shared/GlobalData.cpp
        Shared/GlobalData.h
        Shared/Globals.cpp
        Shared/GPDLcomp.cpp
        Shared/GPDLcomp.h
        Shared/GPDLexec.cpp
        Shared/GPDLexec.h
        Shared/GPDLOpCodes.h
        Shared/Graphics.cpp
        Shared/Graphics.h
        Shared/Items.cpp
        Shared/Items.h
        Shared/Level.cpp
        Shared/Level.h
        Shared/Money.cpp
        Shared/Money.h
        Shared/Monster.cpp
        Shared/Monster.h
        Shared/Movie.cpp
        Shared/Movie.h
        Shared/Party.cpp
        Shared/Party.h
        Shared/PicData.cpp
        Shared/PicData.h
        Shared/PicSlot.cpp
        Shared/PicSlot.h
        Shared/png.h
        Shared/pngconf.h
        Shared/ProjectVersion.h
        Shared/Property.cpp
        Shared/Property.h
        Shared/regexp.cpp
        Shared/regexp.h
        Shared/RunTimeIF.cpp
        Shared/RunTimeIF.h
        Shared/SharedQueue.h
        Shared/SoundMgr.cpp
        Shared/SoundMgr.h
        Shared/Specab.cpp
        Shared/Specab.h
        Shared/Spell.cpp
        Shared/Spell.h
        Shared/StackWalker.cpp
        Shared/StackWalker.h
        Shared/stdafx.cpp
        Shared/stdafx.h
        Shared/SurfaceMgr.h
        Shared/taglist.cpp
        Shared/Taglist.h
        Shared/Thread.h
        Shared/Timer.cpp
        Shared/Timer.h
        Shared/traits.cpp
        Shared/traits.h
        Shared/Viewport.cpp
        Shared/Viewport.h
        Shared/zconf.h
        Shared/zlib.h
        UAFWin/CDXBitmapFont.cpp
        UAFWin/CDXBitmapFont.h
        UAFWin/CharStatsForm.cpp
        UAFWin/CharStatsForm.h
        UAFWin/Combatant.cpp
        UAFWin/Combatant.h
        UAFWin/Combatants.cpp
        UAFWin/Combatants.h
        UAFWin/CombatSummary.h
        UAFWin/CProcinp.cpp
        UAFWin/CProcinp.h
        UAFWin/Dgngame.cpp
        UAFWin/Dgngame.h
        UAFWin/Disptext.cpp
        UAFWin/Disptext.h
        UAFWin/Drawtile.cpp
        UAFWin/Drawtile.h
        UAFWin/Dungeon.cpp
        UAFWin/Dungeon.h
        UAFWin/FormattedText.cpp
        UAFWin/FormattedText.h
        UAFWin/Forth.cpp
        UAFWin/GameMenu.cpp
        UAFWin/GameMenu.h
        UAFWin/Getinput.cpp
        UAFWin/Getinput.h
        UAFWin/ItemsForm.cpp
        UAFWin/ItemsForm.h
        UAFWin/Mainfrm.cpp
        UAFWin/Mainfrm.h
        UAFWin/MouseThread.cpp
        UAFWin/MouseThread.h
        UAFWin/ObjectScripts.cpp
        UAFWin/ObjectScripts.h
        UAFWin/Path.cpp
        UAFWin/Path.h
        UAFWin/Resource.h
        UAFWin/RestTimeForm.cpp
        UAFWin/RestTimeForm.h
        UAFWin/RunEvent.cpp
        UAFWin/Screen.cpp
        UAFWin/Screen.h
        UAFWin/SpellForm.cpp
        UAFWin/SpellForm.h
        UAFWin/TextForm.cpp
        UAFWin/TextForm.h
        UAFWin/Treas.cpp
        UAFWin/Treas.h
        UAFWin/XBrowseForFolder.cpp
        UAFWin/XBrowseForFolder.h
        UAFWinEd/AccessDlg.cpp
        UAFWinEd/AccessDlg.h
        UAFWinEd/Addnpc.cpp
        UAFWinEd/Addnpc.h
        UAFWinEd/AddSpecialObject.cpp
        UAFWinEd/AddSpecialObject.h
        UAFWinEd/AnimationStyle.cpp
        UAFWinEd/AnimationStyle.h
        UAFWinEd/ArrowHeaderCtrl.cpp
        UAFWinEd/ArrowHeaderCtrl.h
        UAFWinEd/AskSureDlg.cpp
        UAFWinEd/AskSureDlg.h
        UAFWinEd/AttibuteDataEdit.cpp
        UAFWinEd/AttibuteDataEdit.h
        UAFWinEd/AttributeSelect.cpp
        UAFWinEd/AttributeSelect.h
        UAFWinEd/BaseDialog.cpp
        UAFWinEd/BaseDialog.h
        UAFWinEd/BkGndPicDlg.cpp
        UAFWinEd/BkGndPicDlg.h
        UAFWinEd/Buttons.cpp
        UAFWinEd/Buttons.h
        UAFWinEd/Campevnt.cpp
        UAFWinEd/Campevnt.h
        UAFWinEd/Cbattres.cpp
        UAFWinEd/Cbattres.h
        UAFWinEd/CharacterEditor.cpp
        UAFWinEd/CharacterEditor.h
        UAFWinEd/Chardlg.cpp
        UAFWinEd/Chardlg.h
        UAFWinEd/ChooseCombatMonsters.cpp
        UAFWinEd/ChooseCombatMonsters.h
        UAFWinEd/Chooseit.cpp
        UAFWinEd/Chooseit.h
        UAFWinEd/ChooseMonster.cpp
        UAFWinEd/ChooseMonster.h
        UAFWinEd/ChooseNPC.cpp
        UAFWinEd/ChooseNPC.h
        UAFWinEd/ChooseOptions.cpp
        UAFWinEd/ChooseOptions.h
        UAFWinEd/ChooseSpeclAbDlg.cpp
        UAFWinEd/ChooseSpeclAbDlg.h
        UAFWinEd/ChurchDlg.cpp
        UAFWinEd/ChurchDlg.h
        UAFWinEd/ClassDlg.cpp
        UAFWinEd/ClassDlg.h
        UAFWinEd/Colorbox.cpp
        UAFWinEd/Colorbox.h
        UAFWinEd/CombatEvent.cpp
        UAFWinEd/CombatEvent.h
        UAFWinEd/CombatMonsterDetails.cpp
        UAFWinEd/CombatMonsterDetails.h
        UAFWinEd/CombatTreasureDlg.cpp
        UAFWinEd/CombatTreasureDlg.h
        UAFWinEd/CrossReference.h
        UAFWinEd/CrossReferenceDlg.cpp
        UAFWinEd/Dbutils.cpp
        UAFWinEd/Dbutils.h
        UAFWinEd/DesignList.cpp
        UAFWinEd/DesignList.h
        UAFWinEd/DesignStatsDlg.cpp
        UAFWinEd/DesignStatsDlg.h
        UAFWinEd/DiffLvlData.cpp
        UAFWinEd/DiffLvlData.h
        UAFWinEd/DiffLvlSettings.cpp
        UAFWinEd/DiffLvlSettings.h
        UAFWinEd/DlgPicture.cpp
        UAFWinEd/DlgPicture.h
        UAFWinEd/EditAttributes.cpp
        UAFWinEd/EditAttributes.h
        UAFWinEd/EditBGSlots.cpp
        UAFWinEd/EditBGSlots.h
        UAFWinEd/EditorOptions.cpp
        UAFWinEd/EditorOptions.h
        UAFWinEd/EditText.cpp
        UAFWinEd/EditText.h
        UAFWinEd/EditWallSlots.cpp
        UAFWinEd/EditWallSlots.h
        UAFWinEd/Encnter.cpp
        UAFWinEd/Encnter.h
        UAFWinEd/EntryPointDlg.cpp
        UAFWinEd/EntryPointDlg.h
        UAFWinEd/EventViewer.cpp
        UAFWinEd/EventViewer.h
        UAFWinEd/GameResChange.cpp
        UAFWinEd/GameResChange.h
        UAFWinEd/GetMoney.cpp
        UAFWinEd/GetMoney.h
        UAFWinEd/GetMoneySackData.cpp
        UAFWinEd/GetMoneySackData.h
        UAFWinEd/Givedmg.cpp
        UAFWinEd/Givedmg.h
        UAFWinEd/Giveexp.cpp
        UAFWinEd/Giveexp.h
        UAFWinEd/GlobalDataDlg.cpp
        UAFWinEd/GlobalDataDlg.h
        UAFWinEd/GlobalSndChooser.cpp
        UAFWinEd/GlobalSndChooser.h
        UAFWinEd/Globtext.cpp
        UAFWinEd/Globtext.h
        UAFWinEd/HealParty.cpp
        UAFWinEd/HealParty.h
        UAFWinEd/Help.cpp
        UAFWinEd/Help.h
        UAFWinEd/IconPicDlg.cpp
        UAFWinEd/IconPicDlg.h
        UAFWinEd/IconPicPoolDlg.cpp
        UAFWinEd/IconPicPoolDlg.h
        UAFWinEd/ImportFRUAData.cpp
        UAFWinEd/ImportFRUAData.h
        UAFWinEd/ImportWarning.cpp
        UAFWinEd/ImportWarning.h
        UAFWinEd/ItemDB.cpp
        UAFWinEd/ItemDB.h
        UAFWinEd/ItemDBDlg.cpp
        UAFWinEd/ItemDBDlg.h
        UAFWinEd/Itemdlg.cpp
        UAFWinEd/Itemdlg.h
        UAFWinEd/ItemEditor.cpp
        UAFWinEd/ItemEditor.h
        UAFWinEd/ItemKeyQuestDlg.cpp
        UAFWinEd/ItemKeyQuestDlg.h
        UAFWinEd/JournalAddEventDlg.cpp
        UAFWinEd/JournalAddEventDlg.h
        UAFWinEd/JournalDataDlg.cpp
        UAFWinEd/JournalDataDlg.h
        UAFWinEd/JournalEntryDlg.cpp
        UAFWinEd/JournalEntryDlg.h
        UAFWinEd/ListSort.cpp
        UAFWinEd/ListSort.h
        UAFWinEd/LogicBlock.cpp
        UAFWinEd/LogicBlock.h
        UAFWinEd/MagicalItemProp.cpp
        UAFWinEd/magicalitemprop.h
        UAFWinEd/MainFrm.cpp
        UAFWinEd/MainFrm.h
        UAFWinEd/Mapstyle.cpp
        UAFWinEd/Mapstyle.h
        UAFWinEd/MoneyConfigDlg.cpp
        UAFWinEd/MoneyConfigDlg.h
        UAFWinEd/MonsterAttackDetails.cpp
        UAFWinEd/MonsterAttackDetails.h
        UAFWinEd/MonsterAttacksDlg.cpp
        UAFWinEd/MonsterAttacksDlg.h
        UAFWinEd/MonsterDBDlg.cpp
        UAFWinEd/MonsterDBDlg.h
        UAFWinEd/MonsterEditor.cpp
        UAFWinEd/MonsterEditor.h
        UAFWinEd/Npcsays.cpp
        UAFWinEd/Npcsays.h
        UAFWinEd/PassTimeDlg.cpp
        UAFWinEd/PassTimeDlg.h
        UAFWinEd/Password.cpp
        UAFWinEd/Password.h
        UAFWinEd/PlayMovieDlg.cpp
        UAFWinEd/PlayMovieDlg.h
        UAFWinEd/PreviewFileDlg.cpp
        UAFWinEd/PreviewFileDlg.h
        UAFWinEd/QButtonOptions.cpp
        UAFWinEd/QButtonOptions.h
        UAFWinEd/QListOptions.cpp
        UAFWinEd/QListOptions.h
        UAFWinEd/QuestEventData.cpp
        UAFWinEd/QuestEventData.h
        UAFWinEd/QuestionYesNoData.cpp
        UAFWinEd/QuestionYesNoData.h
        UAFWinEd/RandomEvent.cpp
        UAFWinEd/RandomEvent.h
        UAFWinEd/Removnpc.cpp
        UAFWinEd/Removnpc.h
        UAFWinEd/RequestFilename.cpp
        UAFWinEd/RequestFilename.h
        UAFWinEd/resource.h
        UAFWinEd/ScrollPicDlg.cpp
        UAFWinEd/ScrollPicDlg.h
        UAFWinEd/SearchParamDlg.cpp
        UAFWinEd/SearchParamDlg.h
        UAFWinEd/SelectFolderDialog.cpp
        UAFWinEd/SelectFolderDialog.h
        UAFWinEd/SelectLevel.cpp
        UAFWinEd/SelectLevel.h
        UAFWinEd/SelectPicDlg.cpp
        UAFWinEd/SelectPicDlg.h
        UAFWinEd/shelllink.cpp
        UAFWinEd/shelllink.h
        UAFWinEd/Shopdlg.cpp
        UAFWinEd/Shopdlg.h
        UAFWinEd/SmallPicDlg.cpp
        UAFWinEd/SmallPicDlg.h
        UAFWinEd/SmallPicPoolDlg.cpp
        UAFWinEd/SmallPicPoolDlg.h
        UAFWinEd/SmallTownEvent.cpp
        UAFWinEd/SmallTownEvent.h
        UAFWinEd/SortableListCtrl.cpp
        UAFWinEd/SortableListCtrl.h
        UAFWinEd/SoundChooser.cpp
        UAFWinEd/SoundChooser.h
        UAFWinEd/SoundEventDlg.cpp
        UAFWinEd/SoundEventDlg.h
        UAFWinEd/SpecAbMsgEnter.cpp
        UAFWinEd/SpecAbMsgEnter.h
        UAFWinEd/SpecAbMsgs.cpp
        UAFWinEd/SpecAbMsgs.h
        UAFWinEd/SpecialItemDlg1.cpp
        UAFWinEd/SpecialItemDlg1.h
        UAFWinEd/SpecialKeyBlockage.cpp
        UAFWinEd/SpecialKeyBlockage.h
        UAFWinEd/SpellAttribute.cpp
        UAFWinEd/SpellAttribute.h
        UAFWinEd/SpellDBDlgEx.cpp
        UAFWinEd/SpellDBDlgEx.h
        UAFWinEd/Spelldlg.cpp
        UAFWinEd/Spelldlg.h
        UAFWinEd/SpellEditor.cpp
        UAFWinEd/SpellEditor.h
        UAFWinEd/StartingEquipDlg.cpp
        UAFWinEd/StartingEquipDlg.h
        UAFWinEd/StepEventDlg.cpp
        UAFWinEd/StepEventDlg.h
        UAFWinEd/TakePartyItems.cpp
        UAFWinEd/TakePartyItems.h
        UAFWinEd/Tavdlg.cpp
        UAFWinEd/Tavdlg.h
        UAFWinEd/TavernTalesDlg.cpp
        UAFWinEd/TavernTalesDlg.h
        UAFWinEd/TeleportDlg.cpp
        UAFWinEd/TeleportDlg.h
        UAFWinEd/TestPartySelectDlg.cpp
        UAFWinEd/TestPartySelectDlg.h
        UAFWinEd/TextEvent.cpp
        UAFWinEd/TextEvent.h
        UAFWinEd/TitleScreenConfig.cpp
        UAFWinEd/TitleScreenConfig.h
        UAFWinEd/TitleScreenData.cpp
        UAFWinEd/TitleScreenData.h
        UAFWinEd/Tour.cpp
        UAFWinEd/Tour.h
        UAFWinEd/Traindlg.cpp
        UAFWinEd/Traindlg.h
        UAFWinEd/TransferEvent.cpp
        UAFWinEd/TransferEvent.h
        UAFWinEd/UAFWinEd.cpp
        UAFWinEd/UAFWinEd.h
        UAFWinEd/UAFWinEdDoc.cpp
        UAFWinEd/UAFWinEdDoc.h
        UAFWinEd/UAFWinEdView.cpp
        UAFWinEd/UAFWinEdView.h
        UAFWinEd/UAImport.cpp
        UAFWinEd/UAImport.h
        UAFWinEd/UtilitiesEventData.cpp
        UAFWinEd/UtilitiesEventData.h
        UAFWinEd/VaultEvent.cpp
        UAFWinEd/VaultEvent.h
        UAFWinEd/VersionDlg.cpp
        UAFWinEd/VersionDlg.h
        UAFWinEd/WallPicDlg.cpp
        UAFWinEd/WallPicDlg.h
        UAFWinEd/WhoPaysDlg.cpp
        UAFWinEd/WhoPaysDlg.h
        UAFWinEd/WhoTriesDlg.cpp
        UAFWinEd/WhoTriesDlg.h
        UAFWinEd/XBrowseForFolder.cpp
        UAFWinEd/XBrowseForFolder.h
        UAFWinEd/Zonedlg.cpp
        UAFWinEd/Zonedlg.h)
